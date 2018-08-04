/**
 * Question structure definition.
 */
#ifndef __UNIT_H
#define __UNIT_H


/**
 * Structure describing questions.
 * @define QUEST_LEN Question string length.
 * @define ANS_LEN   Answer string length.
 * @define ANS_COUNT Number of answers.
 * @param  quest     Question.
 * @param  ans       Array of answers.
 * @param  rans      Number of right answer.
 */
#define QUEST_LEN 240
#define ANS_LEN 80
#define ANS_COUNT 4
struct unit {
    char *quest;
    unsigned int q_len;
    char *ans[ANS_COUNT];
    unsigned int ans_len[ANS_COUNT];
    unsigned int rans;
    struct unit *next;
    struct unit *prev;
};

#endif // __UNIT_H
