/**
 * Unit structure definition.
 */
#ifndef __UNIT_H
#define __UNIT_H

#include "const.h"

/**
 * Structure describing questions.
 * @param  quest     Question.
 * @param  ans       Array of answers.
 * @param  rans      Number of right answer.
 * @param  next      Pointer to next unit.
 * @param  prev      Pointer to previous unit.
 */
typedef struct unit unit_t;
struct unit {
    char quest[Q_LEN];
    char ans[ANS_COUNT][A_LEN];
    unsigned char rans;
    unit_t *next;
    unit_t *prev;
};

/**
 * Initializate questions.
 * @return unit deque pointer
 */
unit_t *unit_deque_init();

/**
 * Free the unit structure
 * @param deque - pointer to any unit from deque
 */
int unit_deque_free(unit_t *deque);

#endif // __UNIT_H
