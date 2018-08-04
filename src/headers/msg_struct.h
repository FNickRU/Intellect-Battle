/**
 * Message queue structures definitions.
 */


#include "player.h"


/**
 * Message types.
 * @define MSG_JOIN   Join-to-room request.
 * @define MSG_CREATE 'Create'-room request.
 * @define MSG_WRK    Messages to worker.
 * @define MSG_ROOM   Messages to room.
 */
#define MSG_JOIN 1
#define MSG_CREATE 2
#define MSG_WRK 3
#define MSG_ROOM (-2)


/**
 * Message structure from server.
 */
struct msg {
    long type;
    int socket;
};


/**
 * Message structure from worker.
 */
struct msg_w {
    long type;
    unsigned int room_size;
    struct player player_info;
};
