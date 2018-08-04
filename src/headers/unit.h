/**
 * Question structure definition.
 */


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
    char quest[QUEST_LEN];
    char ans[ANS_COUNT][ANS_LEN];
    unsigned int rans;
};
