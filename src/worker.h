/**
 * Worker's interface.
 */
#ifndef __WORKER_H
#define __WORKER_H

#include "headers/msg_struct.h"


/**
 * Worker's final state machine. Should run in thread.
 * @param msgqid Message queue descriptor. Should be convert to (int *)
 */
void worker_fsm(void *msgqid);


#endif // __WORKER_H
