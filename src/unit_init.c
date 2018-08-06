#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <dlfcn.h>

#include <dirent.h>

const char* unitfolder = "/units"; 

struct unit* unit_init (char* path)
{
	int i = 0;
	int slen = strlen(path);	
	char* enterpath = path;
	char fullpath[255];

	int libdsc = 0;

	struct unit *end;
	
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
			if (strcmp(strrchr(dir->d_name,'.'),".units") == 0) {
				strcpy(fullpath,unitpath);
				strcat(fullpath,"/");
				strcat(fullpath,dir->d_name);
                printf("fullpath = %s\n",fullpath);
				/*
                parse
                add to list
				*/
                    end = NULL;
                //not like that
			}
		}
		
		closedir(d);
		free(unitpath);
	}

	return end;
}

//int main (int argc, char* argv[])
//{
//    init_lib(NULL,argv);
//}
