/**
 * Worker's interface.
 */
#ifndef __WORKER_H
#define __WORKER_H

#include "msg_struct.h"


/**
 * Worker's final state machine. Should run in thread.
 * @param msgqid Message queue descriptor. Should be convert to (int *)
 */
void worker_fsm(void *arg);


#endif // __WORKER_H
