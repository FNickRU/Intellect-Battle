/**
 * Structure of packet for client-server interaction
 */
#ifndef __PACKET_H
#define __PACKET_H


#include <sys/time.h>
#include "player.h"
#include "unit.h"


#define USER_COUNT 4


/**
 * Package sent by client to server.
 * @define REQ_JOIN   Search any room.
 * @define REQ_CREATE Search empty room.
 * @param  type       Type of room search.
 * @param  room_size  Size of required room.
 * @param  username   Client's username.
 */
#define REQ_JOIN 0
#define REQ_CREATE 1
struct pack_join {
    char type;
    char room_size;
    char username[USERNAME_LEN];
};


/**
 * Packet sent by client to server in game-time (Player's answer).
 * @param ans       Number of player's answer [0-3].
 * @param timestamp Client-side time of player's answer.
 */
struct pack_ans {
    char ans;
    struct timeval timestamp;
};


/**
 * Packet sent by room to client.
 * @param occupancy Number of users already in the room.
 * @param room_size Size of room.
 * @param id        Client's id in room.
 * @param usernames Array of joined user's usernames. Defined only <occupancy>
 *                  of them.
 */
struct pack_wait {
    char occupancy;
    char room_size;
    char id;
    char usernames[USER_COUNT][USERNAME_LEN];
};


/**
 * Packet sent by room in game-time.
 * @param score     Score of every player. Interpreted as signed decimal
 *                  number, sign show player's participation in the game.
 * @param quest_num Question number (round number). Starts from 1, 0 defines
 *                  game over.
 * @param quest     Question string.
 * @param q_len     Question length.
 * @param ans       Answer's strings.
 * @param ans_len   Answer's length.
 */
struct pack_game {
    char score[USER_COUNT];
    unsigned char quest_num;
    char *quest;
    unsigned int q_len;
    char *ans[ANS_COUNT];
    unsigned int ans_len[ANS_COUNT];
};


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
        struct pack_wait p_wait;
        struct pack_game p_game;
    };
};


/**
 * Client's packet structure.
 * @define C_JOIN Packet has 'join' information (see 'struct pack_join').
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
        struct pack_join p_join;
        struct pack_ans p_ans;
    };
};


#endif // __PACKET_H
