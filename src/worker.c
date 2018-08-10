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
void error_handler(int state, int arg);

void *worker_fsm(void *arg)
{
    int msgqid = *((int *) arg);

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
                printf("Worker %x wait for connection\n", pthread_self());

                if (msgrcv(msgqid, &con, sizeof(con), MSG_WRK, 0) < 0) {
                    printf("Worker %x: message queue failed! (Reason: %s)\n",
                           pthread_self(), strerror(errno));

                    state = FIN;
                } else {
                    printf("Worker %x received connection from %x\n",
                           getpid(),
                           con.socket);

                    socket = con.socket;
                    state = RECV;
                }
                break;

            case RECV:
                printf("Worker %x wait for join/create request\n",
                       pthread_self());

                if (recv(socket, &pack, sizeof(pack), 0) < 0) {
                    state = ERROR;
                    err_type = RECV, err_arg = socket;
                } else {
                    printf("Worker %x wait for join/create request\n",
                           pthread_self());

                    req = pack.p_req;
                    state = PROC;
                }
                break;
            case PROC:
                join_msg.player.socket = socket;
                strncpy(join_msg.player.username, req.username, USERNAME_LEN);

                printf("Worker %x send to rooms pool user info: %s/%x\n",
                       pthread_self(), req.username, socket);

                switch (req.type) {
                    case REQ_JOIN:
                        printf("Worker %x send join request\n", pthread_self());

                        join_msg.type = MSG_JOIN;
                        join_msg.room_size = req.room_size;
                        msgsnd(msgqid, &join_msg, sizeof(join_msg), 0);
                        state = WAIT;
                        break;

                    case REQ_CREATE:
                        printf("Worker %x send 'create' request\n",
                               pthread_self());

                        join_msg.type = MSG_CREATE;
                        join_msg.room_size = req.room_size;
                        msgsnd(msgqid, &join_msg, sizeof(join_msg), 0);
                        state = WAIT;
                        break;
                    default:
                        err_type = PROC, err_arg = req.type;
                }
                break;

            case ERROR:
                error_handler(err_type, err_arg);
                state = WAIT;
                break;
        }
    }

    return NULL;
}

void error_handler(int state, int arg)
{
    switch (state) {
        case RECV:
            printf("Worker %x: connection error with %d!\n",
                   pthread_self(), arg);
            break;

        case PROC:
            printf("Worker %x: unexpected jointype (%d)!\n",
                   pthread_self(), arg);
            break;

        default:
            printf("Worker %x: unknown error (arg: %d)!\n",
                   pthread_self(), arg);
    }
}
