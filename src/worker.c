#include <sys/msg.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "worker.h"

enum State {WAIT, RECV, PROC, ERROR, FIN};

/**
 * Inner error-handling function. Not part of interface.
 * @param state - state of the machine on which error occurred
 * @param arg - additional argument, that depends on state (sockd or join_type)
 */
void error_handler(int state, int arg);

void worker_fsm(void *arg)
{
    int msgqid = *((int *) arg);

    struct msg cnnct;
    struct msg_w req;
    struct c_pack pack;
    struct pack_join join;
    int state = WAIT;
    int socket;
    int err_type = -1, err_arg = -1;
    while (state != FIN) {
        switch (state) {
            case WAIT:
                if (msgrcv(msgqid, &cnnct, sizeof(cnnct), MSG_WRK, 0) < 0) {
                    state = FIN;
                } else {
                    socket = cnnct.socket;
                    state = RECV;
                }
                break;
            case RECV:
                if (recv(socket, &pack, sizeof(join), 0) < 0) {
                    state = ERROR;
                    err_type = RECV, err_arg = socket;
                } else {
                    join = pack.p_join;
                    state = PROC;
                }
                break;
            case PROC:
                req.player_info.socket = socket;
                strncpy(req.player_info.username, join.username, USERNAME_LEN);
                switch (join.type) {
                    case REQ_JOIN:
                        req.type = MSG_JOIN;
                        req.room_size = 4;
                        msgsnd(msgqid, &req, sizeof(req), 0);
                        state = WAIT;
                        break;
                    case REQ_CREATE:
                        req.type = MSG_CREATE;
                        req.room_size = join.room_size;
                        msgsnd(msgqid, &req, sizeof(req), 0);
                        state = WAIT;
                        break;
                    default:
                        err_type = PROC, err_arg = join.type;
                }
                break;
            case ERROR:
                error_handler(err_type, err_arg);
                state = WAIT;
                break;
        }
    }
}

void error_handler(int state, int arg)
{
    switch (state) {
        case RECV:
            printf("Worker %X: connection error (sockd: %d)!\n", getpid(), arg);
            break;
        case PROC:
            printf("Worker %X: unexpected jointype (%d)!\n", getpid(), arg);
            break;
        default:
            printf("Worker %X: unknown error (arg: %d)!\n", getpid(), arg);
    }
}
