#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dirent.h>
#include "unit.h"

/**
 * The path to the units folder
 */
const char* unitfolder = "/units"; 


/**
 * Creates an empty structure object
 */
struct unit* unit_create ()
{
    struct unit *u;
    
    /**
     * Getting a place for question
     */
    u = (struct unit*)malloc(sizeof(struct unit));
    u->quest = (char *)malloc(sizeof(char) * 200);
    if (!(u->quest)) {
        free(u);
        return NULL;
    }
    
    /**
     * Getting a place for answers
     */
    for (int i = 0; i < 4; i++) {
        u->ans[i]= (char *)malloc(sizeof(char) * 80);
        if (!(u->ans[i])) {
            for (int j = i; j >= 0 ; j--) {
                free(u->ans[j]);
            }
            free(u);
            return NULL;
        }
    }
    
    /**
     * Initializing fields with zeros
     */
    u->rans = 0;
    u->next = NULL;
    u->prev = NULL;
    return u;
}

/**
 * Function of downloading questions from the folder
 */
struct unit* unit_init (char* path)
{
	int i = 0;
	int slen = strlen(path);	
    int err_next = 0;
	char* enterpath = path;
	char fullpath[255];
    char input[255];
    FILE *unitFile;
    int num_of_unit = 0;

	int libdsc = 0;

	struct unit *end;
    struct unit *tmp;
    struct unit *first;
    end = unit_create();
    if (end == NULL) return end;
    first = end;
    
	/**
     * Magic with paths
     */
	for (i = slen; enterpath[i]!='/' ; i--) {
	}
	enterpath[i] = 0;
	char* unitpath = (char*)malloc(strlen(enterpath)+strlen(unitfolder)+1);
	unitpath[0] = 0;
	strcat(unitpath,enterpath);
	strcat(unitpath,unitfolder);
	
    /**
     * Open and read each the unit from the directory
     */
	DIR *d;
	struct dirent *dir;
	d = opendir(unitpath);
	if (d) {
		while ((dir = readdir(d)) != NULL) {
            /**
             * More magic
             */
            if (strrchr(dir->d_name,'.') == NULL) continue;
			if (strcmp(strrchr(dir->d_name,'.'),".units") == 0) {
				strcpy(fullpath,unitpath);
				strcat(fullpath,"/");
				strcat(fullpath,dir->d_name);
                printf("fullpath = %s\n",fullpath);
                unitFile = fopen (fullpath, "r");
                if (unitFile == NULL) continue;
                
                /**
                 * If there are suitable file, parse it.
                 * Suitable is *.units
                 * Then looking for another file
                 */
                while (!feof(unitFile)) {
                    err_next = 0;
                    if (!(fgets(input,250,unitFile))) {
                        err_next = 1;
                    }
                    if (strlen(input) > 199) {
                        err_next = 1;
                    }
                    input[strlen(input)-1] = 0;
                    strncpy(end->quest,input,200);
                    for (int i = 0; i < 4; i++) {
                        if (!(fgets(input,250,unitFile))) {
                            err_next = 1;
                        }
                        if (strlen(input) > 79) {
                            err_next = 1;
                        }
                        input[strlen(input)-1] = 0;
                        strncpy(end->ans[i],input,80);
                    }
                    if (!(fgets(input,250,unitFile))) {
                        err_next = 1;
                    }
                    if (strlen(input) > 10) {
                        err_next = 1;
                    }
                    end->rans = input[0]-48;
                    if ((end->rans > 0) && (end->rans < 5)) {
                        if (!err_next) {
                            num_of_unit++;
                            tmp = end;
                            end = unit_create();
                            if (!(end)) 
                            {
                                unit_free_all(tmp);
                                return NULL;
                            }
                            tmp->next = end;
                            end->prev = tmp;
                        }
                    }
                }
                //End of parse
			}
		}
		printf("Вопросов было загруженно: %d\n",num_of_unit);
        
        /**
         * Clean the temp variable
         */
        tmp = end->prev;
        free(end->quest);
        free(end->ans[0]);
        free(end->ans[1]);
        free(end->ans[2]);
        free(end->ans[3]);
        free(end);
        end = tmp;
        
        /**
         * Loop the list.
         * Close the directory and return the pointer
         */
        end->next = first;
        first->prev = end;
		closedir(d);
		free(unitpath);
	}

	return end;
    
}
