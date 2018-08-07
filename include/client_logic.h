/**
 * The header of the client interface.
 */
#ifndef __CLIENT_H
#define __CLIENT_H


#include "packet.h"


/**
 * Client-side system information.
 */
struct player Client_info;


/**
 * Client-side information about room.
 * @param occupancy Number of joined players.
 * @param room_size Max size of room.
 * @param id        Client's id into room.
 * @param usernames Array of user's nicknames.
 * @param score     Array of user's scores.
 */
struct room_info {
    char occupancy;
    char room_size;
    char id;
    char usernames[USER_COUNT][USERNAME_LEN];
    char score[USER_COUNT];
};


/**
 * Client-side unit's structure.
 * @param  quest Question.
 * @param  ans   Answers.
 */
struct unit {
    char quest[Q_LEN];
    char ans[ANSWER_COUNT][A_LEN];
};


/**
 * Initialize information about user.
 * @param name User's nickname.
 */
void init(char* name);


/**
 * Connect to server (socket connect wrapper).
 * @return 0 if socket is connected, else non-zero.
 */
int connect();


/**
 * Send request to server (search).
 * @param  type Type of searching. Values: [REQ_JOIN, REQ_CREATE]
 * @param  size Size of room. Valuable only for REQ_CREATE.
 * @return      0, if request sended successfully, else non-zero.
 */
int send_conf(char type, char size);


/**
 * Wait server response and interprets it.
 * @param  room Room's information. Structure should be created before
 *              using. Function only change it.
 * @return      0 - game is started, 1 - wait more, -1 - error
 */
int wait_for_players(struct room_info *room);


/**
 * Get unit from server (and game info).
 * @param  u    Pointer to unit structure. Structure should be created before
 *              using. Function only change it.
 * @param  room Room's information. Structure should be created before
 *              using. Function only change it.
 * @return      0, if game is over, -1 on error, else > 0.
 */
int get_unit(struct unit *u, struct room_info *room);


/**
 * Send client's answer to server.
 * @param  ans Number of answer [0-3].
 * @return     0, if answer sended successfully.
 */
int send_ans(char ans);


/**
 * Check if client is loser.
 * @param  room Room's information.
 * @return      0 if client is not loser, else 1 [boolean-like]
 */
int is_loser(struct room_info room);


/**
 * Finalize all data
 * @return 0, if all data finalized successfully.
 */
int finalize();


#endif // __CLIENT_H
