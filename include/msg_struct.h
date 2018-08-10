/**
 * Message queue structures definitions.
 */
#ifndef __MSG_STRUCT_H
#define __MSG_STRUCT_H

#include "player.h"

/**
 * Message types.
 * @define MSG_JOIN   Join-to-room request.
 * @define MSG_CREATE 'Create'-room request.
 * @define MSG_WRK    Messages to worker.
 * @define MSG_ROOM   Messages to room.
 */
#define MSG_JOIN   1
#define MSG_CREATE 2
#define MSG_WRK    3
#define MSG_ROOM (-2)

/**
 * Message structure from server.
 */
struct connection {
    long type;
    int socket;
};
typedef struct connection con_t;

/**
 * Message structure from worker.
 */
struct join_info {
    long type;
    unsigned char room_size;
    player_t player;
};
typedef struct join_info join_t;

#endif // __MSG_STRUCT_H
