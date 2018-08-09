/**
 * Room's interface.
 */
#ifndef __ROOM_H
#define __ROOM_H


#include "msg_struct.h"
#include "unit.h"


#define LAST_QUEST 15
#define PLR_DISCONNECT -2
#define PLR_LOST -1
#define MAX_STEP 4 + 1


/**
 * Information for room.
 * @param msgqid Message queue descriptor.
 * @param units  Pointer to unit's list.
 */
struct room_info {
    int msgqid;
    struct unit *units;
};

/**
 * Room's final state machine. Should run in thread.
 * @param room_info Info to room. Should be convert to (struct room_info *).
 */
void *room_fsm(void *room_info);


/**
 * Send information to user. Wrapper for 'sendto' function.
 * @param  user      User's information.
 * @param  data      Pointer to data to send.
 * @param  data_size Size of data.
 * @return           0, if data sended successfully, else non-zero.
 */
int sendto_user(struct player user,
    void *data, unsigned int data_size);


/**
 * Receive information from user. Wrapper for 'recvfrom' function.
 * @param  user      User's information.
 * @param  data      A pointer to the memory area where information will be
 *                   recorded.
 * @param  data_size Size of data to receive.
 * @return           0, if data received successfully, else non-zero.
 */
int recvfrom_user(struct player user,
    void *data, unsigned int data_size);


#endif // __ROOM_H
