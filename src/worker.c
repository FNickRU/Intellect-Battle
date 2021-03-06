#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <errno.h>

#include "worker.h"

enum State {WAIT, RECV, PROC, ERROR, FIN};

/**
 * Inner error-handling function. Not part of interface.
 * @param state - state of the machine on which error occurred
 * @param arg - additional argument, that depends on state (sockd or join_type)
 */
void worker_error_handler(int state, int arg);

void *worker_fsm(void *arg)
{
    worker_t *info = (worker_t *) arg;

    int msgqid = info->msgqid;
    info->sync = SYNC_ON;

    con_t con;
    cpack_t pack;
    request_t req;
    join_t join_msg;
    int socket;
    int err_type = -1, err_arg = -1;
    int state = WAIT;
    while (state != FIN) {
        switch (state) {
            case WAIT:
                printf("Worker %lx wait for connection\n", pthread_self());

                if (msgrcv(msgqid, &con, sizeof(con), MSG_WRK, 0) < 0) {
                    printf("Worker %lx: message queue failed! (Reason: %s)\n",
                           pthread_self(), strerror(errno));

                    state = FIN;
                } else {
                    printf("Worker %lx received connection from %x\n",
                           pthread_self(), con.socket);

                    socket = con.socket;
                    state = RECV;
                }
                break;

            case RECV:
                printf("Worker %lx wait for join/create request\n",
                       pthread_self());

                if (recv(socket, &pack, sizeof(pack), 0) < 0) {
                    state = ERROR;
                    err_type = RECV, err_arg = socket;
                } else {
                    printf("Worker %lx received request\n",
                           pthread_self());

                    req = pack.p_req;
                    state = PROC;
                }
                break;
            case PROC:
                join_msg.player.socket = socket;
                strncpy(join_msg.player.username, req.username, USERNAME_LEN);

                if (!strcmp(req.username, "")) {
                    printf("Worker %lx received disconnect from %x\n",
                           pthread_self(), socket);

                    req.type = REQ_DISCONNECT;
                } else {
                    printf("Worker %lx send to rooms pool user info: %s/%x\n",
                           pthread_self(), req.username, socket);
                }

                switch (req.type) {
                    case REQ_JOIN:
                        printf("Worker %lx send join request\n",
                               pthread_self());

                        join_msg.type = MSG_JOIN;
                        join_msg.room_size = req.room_size;
                        msgsnd(msgqid, &join_msg, sizeof(join_msg), 0);
                        state = WAIT;
                        break;

                    case REQ_CREATE:
                        printf("Worker %lx send 'create' request\n",
                               pthread_self());

                        join_msg.type = MSG_CREATE;
                        join_msg.room_size = req.room_size;
                        msgsnd(msgqid, &join_msg, sizeof(join_msg), 0);
                        state = WAIT;
                        break;

                    case REQ_DISCONNECT:
                        state = WAIT;
                        break;

                    default:
                        err_type = PROC, err_arg = req.type;
                }
                break;

            case ERROR:
                worker_error_handler(err_type, err_arg);
                state = WAIT;
                break;
        }
    }

    return NULL;
}

void worker_error_handler(int state, int arg)
{
    switch (state) {
        case RECV:
            printf("Worker %lx: connection error with %d!\n",
                   pthread_self(), arg);
            break;

        case PROC:
            printf("Worker %lx: unexpected jointype (%d)!\n",
                   pthread_self(), arg);
            break;

        default:
            printf("Worker %lx: unknown error (arg: %d)!\n",
                   pthread_self(), arg);
    }
}
