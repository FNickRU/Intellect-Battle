#include <sys/msg.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include "worker.h"

enum State {WAIT, RECV, PROC, ERROR, FIN};

void worker_fsm(void *arg)
{
    int msgqid = *((int *) arg);

    struct msg cnnct;
    struct msg_w msg_to_room;
    int state = WAIT;
    int socket;
    char request[USERNAME_LEN + 2];
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
                if (recv(socket, request, sizeof(request), 0) < 0) {
                    state = ERROR;
                } else {
                    state = PROC;
                }
                break;
            case PROC:
                msg_to_room.player_info.socket = socket;
                strcpy(msg_to_room.player_info.username, request + 1);
                switch (request[0]) {
                    case MSG_JOIN:
                        msg_to_room.type = MSG_JOIN;
                        msg_to_room.room_size = 4;
                        msgsnd(msgqid, &msg_to_room, sizeof(msg_to_room), 0);
                        state = WAIT;
                        break;
                    case MSG_CREATE:
                        msg_to_room.type = MSG_CREATE;
                        msg_to_room.room_size = request + 41;
                        msgsnd(msgqid, &msg_to_room, sizeof(msg_to_room), 0);
                        state = WAIT;
                        break;
                    default:
                        state = ERROR;
                }
                break;
            case ERROR:
                printf("Error occured!");
                state = WAIT;
                break;
        }
    }
}
