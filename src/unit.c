#include "unit.h"

#include <stdio.h>

int unit_free_all(struct unit *point)
{
    struct unit *tmp1 = point;
    struct unit *tmp2 = point->next;
    tmp1->next = NULL;
    while (tmp2->next != NULL)
    {
        //printf("Free the:%s\n",tmp2->quest);
        for (int i = 0; i < 4; i++)
        {
            free(tmp2->ans[i]);
        }
        free(tmp2->quest);
        tmp1 = tmp2;
        tmp2 = tmp2->next;
        free(tmp1);
    }
    //printf("Free the:%s\n",tmp2->quest);
    for (int i = 0; i < 4; i++)
    {
        free(tmp2->ans[i]);
    }
    free(tmp2->quest);
    free(tmp2);
    
    return 0;
}
