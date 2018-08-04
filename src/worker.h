/**
 * Worker's interface.
 */


#include "msg_struct.h"


/**
 * Worker's final state machine. Should run in thread.
 * @param msgqid Message queue descriptor.
 */
void worker_fsm(int msgqid);
