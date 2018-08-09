/**
 * Question structure definition.
 */
#ifndef __UNIT_H
#define __UNIT_H


/**
 * Structure describing questions.
 * @define ANS_COUNT Number of answers.
 * @param  quest     Question.
 * @param  ans       Array of answers.
 * @param  rans      Number of right answer.
 * @param  next      Pointer to next unit.
 * @param  prev      Pointer to previous unit.
 */
#define ANS_COUNT 4
struct unit {
    char *quest;
    char *ans[ANS_COUNT];
    unsigned int rans;
    struct unit *next;
    struct unit *prev;
};

/**
 * Initializate questions.
 * @param  path     Root path
 */
struct unit* unit_init (char* path);

/**
 * Free the unit structure
 * @param  freepoint Point to any unit
 */
int unit_free_all(struct unit *freepoint);

#endif // __UNIT_H
