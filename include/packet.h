/**
 * Structure of packet for client-server interaction
 */
#ifndef __PACKET_H
#define __PACKET_H


#include <sys/time.h>
#include "player.h"
#include "const.h"


#define USER_COUNT 4
#define ANSWER_COUNT 4

/**
 * Package sent by client to server.
 * @define REQ_JOIN       Search any room.
 * @define REQ_CREATE     Search empty room.
 * @define REQ_DISCONNECT Client disconnected (not real request).
 * @param  type           Type of room search.
 * @param  room_size      Size of required room.
 * @param  username       Client's username.
 */
#define REQ_JOIN        0
#define REQ_CREATE      1
#define REQ_DISCONNECT -1
struct pack_request {
    char type;
    unsigned char room_size;
    char username[USERNAME_LEN];
};
typedef struct pack_request request_t;


/**
 * Packet sent by client to server in game-time (Player's answer).
 * @param ans       Number of player's answer [0-3].
 * @param timestamp Client-side time of player's answer.
 */
struct pack_ans {
    unsigned char ans;
    struct timeval timestamp;
};
typedef struct pack_ans ans_t;


/**
 * Packet sent by room to client.
 * @param occupancy Number of users already in the room.
 * @param room_size Size of room.
 * @param id        Client's id in room.
 * @param usernames Array of joined user's usernames. Defined only <occupancy>
 *                  of them.
 */
struct pack_wait {
    unsigned char occupancy;
    unsigned char room_size;
    unsigned char id;
    char usernames[USER_COUNT][USERNAME_LEN];
};
typedef struct pack_wait waitinfo_t;


/**
 * Packet sent by room in game-time.
 * @param  score     Score of every player. Interpreted as signed decimal
 *                   number, sign show player's participation in the game.
 * @param  quest_num Question number (round number). Starts from 1, 0 defines
 *                   game over.
 * @param  quest     Question string.
 * @param  q_len     Question length.
 * @param  ans       Answer's strings.
 * @param  ans_len   Answer's length.
 */
struct pack_game {
    unsigned char score[USER_COUNT];
    unsigned char quest_num;
    char quest[Q_LEN];
    char ans[ANSWER_COUNT][A_LEN];
};
typedef struct pack_game gameinfo_t;


/**
 * Server's packet structure.
 * @define S_WAIT Packet has 'wait' information (see 'struct pack_wait').
 * @define S_GAME Packet has 'game' information (see 'struct pack_game').
 * @param  type   Define the information of packet. Values: S_WAIT, S_GAME.
 * @param  p_wait 'Wait' information.
 * @param  p_game 'Game' information.
 */
#define S_WAIT 0
#define S_GAME 1
struct s_pack {
    int type;
    union {
        waitinfo_t p_wait;
        gameinfo_t p_game;
    };
};
typedef struct s_pack spack_t;


/**
 * Client's packet structure.
 * @define C_JOIN Packet has 'join' information (see 'struct pack_request').
 * @define C_ANS  Packet has 'answer' information (see 'struct pack_ans').
 * @param  type   Define the information of packet. Values: C_JOIN, C_ANS.
 * @param  p_join 'Join' information.
 * @param  p_ans  'Answer' information.
 */
#define C_JOIN 0
#define C_ANS 1
struct c_pack {
    int type;
    union {
        request_t p_req;
        ans_t p_ans;
    };
};
typedef struct c_pack cpack_t;


#endif // __PACKET_H
