/**
 * Worker's interface.
 */
#ifndef __WORKER_H
#define __WORKER_H

#include "msg_struct.h"
#include "packet.h"

/**
 * Information for worker.
 * @param msgqid Message queue descriptor.
 * @param sync   Synchronization variable
 */
struct worker_info {
    int msgqid;
    char sync;
};
typedef struct worker_info worker_t;

/**
 * Worker's final state machine. Should run in thread.
 * @param arg Message queue descriptor. Should be convert to (int *)
 */
void *worker_fsm(void *arg);

#endif // __WORKER_H
