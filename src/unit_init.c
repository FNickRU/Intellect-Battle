#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <dlfcn.h>

#include <dirent.h>
#include "unit.h"

const char* unitfolder = "/units"; 

struct unit* unit_create ()
{
    struct unit *u;
    u = (struct unit*)malloc(sizeof(struct unit));
    u->quest = (char *)malloc(sizeof(char) * 200);
    if (!(u->quest)) {
        free(u);
        return NULL;
    }
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
    u->rans = 0;
    u->next = NULL;
    u->prev = NULL;
    return u;
}

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
    
	
	for (i = slen; enterpath[i]!='/' ; i--) {
	}
	enterpath[i] = 0;
	char* unitpath = (char*)malloc(strlen(enterpath)+strlen(unitfolder)+1);
	unitpath[0] = 0;
	strcat(unitpath,enterpath);
	strcat(unitpath,unitfolder);
	
	DIR *d;
	struct dirent *dir;
	d = opendir(unitpath);
	if (d) {
		while ((dir = readdir(d)) != NULL) {
            if (strrchr(dir->d_name,'.') == NULL) continue;
			if (strcmp(strrchr(dir->d_name,'.'),".units") == 0) {
				strcpy(fullpath,unitpath);
				strcat(fullpath,"/");
				strcat(fullpath,dir->d_name);
                printf("fullpath = %s\n",fullpath);
                unitFile = fopen (fullpath, "r");
                
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
                    //printf("s=%s\nd=%d\n",input,input[0]-48);
                    end->rans = input[0]-48;
                    if ((end->rans > 0) && (end->rans < 5)) {
                        if (!err_next) {
                            num_of_unit++;
                            tmp = end;
                            end = unit_create();
                            tmp->next = end;
                            end->prev = tmp;
                            /*
                            printf("q = %s\n",end->prev->quest);
                            printf("1 = %s\n",end->prev->ans[0]);
                            printf("2 = %s\n",end->prev->ans[1]);
                            printf("3 = %s\n",end->prev->ans[2]);
                            printf("4 = %s\n",end->prev->ans[3]);
                            printf("rans = %d\n",end->prev->rans);
                            */
                            
                        }
                    }
                }
			}
		}
		printf("Вопросов было загруженно: %d\n",num_of_unit);
        
        tmp = end->prev;
        free(end->quest);
        free(end->ans[0]);
        free(end->ans[1]);
        free(end->ans[2]);
        free(end->ans[3]);
        free(end);
        end = tmp;
        
        end->next = first;
        first->prev = end;
		closedir(d);
		free(unitpath);
	}

	return end;
}

//int main (int argc, char* argv[])
//{
//    init_lib(NULL,argv);
//}
