#include <stdlib.h>

#include "unit.h"

/**
 * Consecutive cleaning of the structure
 */
int unit_free_all(struct unit *point)
{
    struct unit *tmp1 = point;
    if (!(point->next)) {
        for (int i = 0; i < 4; i++) {
            free(point->ans[i]);
        }
        free(point->quest);
        free(point);
        return -1;
    }
    struct unit *tmp2 = point->next;
    if (point == NULL) return -2;
    tmp1->next = NULL;
    while (tmp2->next != NULL) {
        for (int i = 0; i < 4; i++) {
            free(tmp2->ans[i]);
        }
        free(tmp2->quest);
        tmp1 = tmp2;
        tmp2 = tmp2->next;
        free(tmp1);
    }

    for (int i = 0; i < 4; i++) {
        free(tmp2->ans[i]);
    }
    free(tmp2->quest);
    free(tmp2);

    return 0;
}
