/**
 * The header of the client interface.
 */
#ifndef __CLIENT_H
#define __CLIENT_H


#include "packet.h"


/**
 * @define CODE_SUCCESS Function return successfully.
 * @define CODE_FAILURE Function fail.
 */
#define CODE_SUCCESS 0
#define CODE_FAILURE (-1)

/**
 * Client-side system information.
 */
struct player Client_info;


/**
 * Wrapper for 'send'.
 * @param  send_pack Packet to send.
 * @return           CODE_SUCCESS if packet sended successfully, else
 *                   CODE_FAILURE.
 */
int sendto_server(struct c_pack send_pack);


/**
 * Wrapper for 'recv'
 * @param  recv_pack Pointer to packet, where will be recorded received data.
 * @return           CODE_SUCCESS if packet received successfully, else 
 *                   CODE_FAILURE.
 */
int recvfrom_server(struct s_pack *recv_pack);


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
 * @return CODE_SUCCESS if socket is connected, else CODE_FAILURE.
 */
int connect_to_server();


/**
 * Send request to server (search).
 * @param  type Type of searching. Values: [REQ_JOIN, REQ_CREATE]
 * @param  size Size of room. Valuable only for REQ_CREATE.
 * @return      CODE_SUCCESS, if request sended successfully, else
 *              CODE_FAILURE.
 */
int send_conf(char type, char size);


/**
 * Wait server's response and interprets it.
 * @define START_GAME Game is started.
 * @define WAIT_MORE  Room is waiting new players.
 * @param  room       Room's information. Structure should be created before
 *                    using. Function only change it.
 * @return            [START_GAME, WAIT_MORE, CODE_FAILURE]
 */
#define START_GAME 0
#define WAIT_MORE 1
int wait_for_players(struct room_info *room);


/**
 * Get unit from server (and game info).
 * @define GAME_OVER Game is over.
 * @define GAME_CONT Game is continue.
 * @param  u         Unit's information. Structure should be created before
 *                   using. Function only change it.
 * @param  room      Room's information. Structure should be created before
 *                   using. Function only change it.
 * @return           [GAME_OVER, GAME_CONT, CODE_FAILURE]
 */
#define GAME_OVER 0
#define GAME_CONT 1
int get_unit(struct unit *u, struct room_info *room);


/**
 * Send client's answer to server.
 * @param  ans       Number of answer [0-3].
 * @param  timestamp Client-side time of answer.
 * @return           CODE_SUCCESS, if answer sended successfully, else
 *                   CODE_FAILURE.
 * @note timestamp is dead arg.
 */
int send_ans(char ans, struct timeval timestamp);


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
