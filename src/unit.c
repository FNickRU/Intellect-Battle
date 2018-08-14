#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include "unit.h"

/**
 * The path to the units folder
 */
const char* unitpath = "./units";

unit_t *unit_deque_init()
{
    int loaded = 0;

    unit_t *first, *last;
    unit_t *next = (unit_t *) calloc(sizeof(unit_t), 1);
    if (next == NULL) {
        return NULL;
    }
    first = next;

    /**
     * Open and read each the unit from the directory
     */
    DIR *directory;
    struct dirent *entry;
    printf("Units directory: %s\n", unitpath);
    directory = opendir(unitpath);
    if (directory) {
        while ((entry = readdir(directory)) != NULL) {
            /**
             * If entry actually have .units extension.
             */
            if (strrchr(entry->d_name,'.') == NULL) continue;
            if (strcmp(strrchr(entry->d_name,'.'), ".units") == 0) {
                char fullpath[255];

                strcpy(fullpath, unitpath);
                strcat(fullpath, "/");
                strcat(fullpath, entry->d_name);

                FILE *unitFile = fopen(fullpath, "r");
                if (unitFile == NULL) continue;

                printf("Load units from %s\n", fullpath);

                /**
                 * If there are suitable file, parse it.
                 * Suitable is *.units
                 * Then looking for another file
                 */
                while (!feof(unitFile)) {
                    int errorFlag = 0;
                    char input[Q_LEN * 2 + 1];

                    if (!(fgets(input, Q_LEN * 2, unitFile))) {
                        errorFlag = 1;
                    }
                    if (strlen(input) > Q_LEN - 1) {
                        errorFlag = 1;
                    }
                    input[strlen(input) - 1] = 0;

                    strncpy(next->quest, input, Q_LEN);

                    for (int idx = 0; idx < ANS_COUNT; ++idx) {
                        if (!(fgets(input, Q_LEN * 2, unitFile))) {
                            errorFlag = 1;
                        }
                        if (strlen(input) > A_LEN - 1) {
                            errorFlag = 1;
                        }
                        input[strlen(input) - 1] = 0;

                        strncpy(next->ans[idx], input, A_LEN);
                    }

                    if (!(fgets(input, Q_LEN * 2, unitFile))) {
                        errorFlag = 1;
                    }
                    if (strlen(input) > RA_LEN - 1) {
                        errorFlag = 1;
                    }
                    next->rans = input[0] - '0';

                    if ((next->rans > 0) && (next->rans < ANS_COUNT + 1)) {
                        if (!errorFlag) {
                            loaded++;
                            last = next;
                            next = (unit_t *) calloc(sizeof(unit_t), 1);
                            if (!(next)) {
                                unit_deque_free(last);
                                return NULL;
                            }
                            last->next = next;
                            next->prev = last;
                        }
                    }
                }

                fclose(unitFile);
            }
        }
        printf("Units loaded: %d\n", loaded);

        /**
         * Clean next last variable (it's empty)
         */
        free(next);

        /**
         * Loop the list.
         */
        last->next = first;
        first->prev = last;

        closedir(directory);
    }

    return last;
}

int unit_deque_free(unit_t *point)
{
    /**
     * If point == NULL.
     */
    if (!point) {
        return -2;
    }
    /**
     * If point is single unit.
     */
    if (!(point->next)) {
        free(point);
        return -1;
    }
    unit_t *tmp1 = point;
    unit_t *tmp2 = point->next;

    tmp1->next = NULL;
    while (tmp2->next != NULL) {
        tmp1 = tmp2;
        tmp2 = tmp2->next;
        free(tmp1);
    }
    free(tmp2);

    return 0;
}
