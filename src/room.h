/**
 * Room's interface.
 */


#include "msg_struct.h"


/**
 * Room's final state machine. Should run in thread.
 * @param msgqid Message queue descriptor.
 * @param units  Array of questions.
 */
void room_fsm(int msgqid, unit* units);


/**
 * Send information to user. Wrapper for 'sendto' function.
 * @param  user      User's information.
 * @param  data      Pointer to data to send.
 * @param  data_size Size of data.
 * @return           0, if data sended successfully, else non-zero.
 */
int sendto_user(struct player user, void *data, unsigned int data_size);


/**
 * Receive information from user. Wrapper for 'recvfrom' function.
 * @param  user      User's information.
 * @param  data      A pointer to the memory area where information will be
 *                   recorded.
 * @param  data_size Size of data to receive.
 * @return           0, if data received successfully, else non-zero.
 */
int recvfrom_user(struct player user, void *data, unsigned int data_size);


/**
 * Shuffle array of unit indexes.
 * @param  idx_array Pointer to index array.
 * @param  size      Size of array.
 * @return           0, if array shuffled successfully, else non-zero.
 */
int shuffle(int* idx_array, unsigned int size);
