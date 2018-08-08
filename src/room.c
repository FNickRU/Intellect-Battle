#include "room.h"
#include "packet.h"
#include <malloc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>


enum State {INIT, WAIT, GAME, GAMEOVER, FIN, EXIT};


void room_fsm(void *room_info)
{
    struct room_info room_i = *((struct room_info *) room_info);

    int msgid = room_i.msgqid;
    struct unit *units = room_i.units;

    int room_size,
        player_id,
        num_quest,
        step;
    char score[USER_COUNT];
    struct msg_w msg;
    struct player *players = NULL;
    struct s_pack spack;
    struct c_pack cpack;

    int state = INIT;
    while (state != EXIT) {
        switch (state) {
            case INIT:
                player_id = 0;
                room_size = 0;
                num_quest = 1;
                bzero(&spack, sizeof(spack));

                if (msgrcv(msgid, &msg, sizeof(msg), MSG_ROOM, 0) < 0) {
                    state = FIN;
                    printf("Room %x: Message queue failed!\n", getpid());
                    break;
                }

                room_size = msg.room_size;

                players = malloc(room_size * sizeof(players));
                if (players == NULL) {
                    state = FIN;
                    printf("Room %x: Memory allocation failed!\n", getpid());
                    break;
                }

                players[player_id] = msg.player_info;
                score[player_id] = 0;

                spack.type = S_WAIT;
                spack.p_wait.occupancy = player_id;
                spack.p_wait.room_size = room_size;
                spack.p_wait.id = player_id;
                strcpy(spack.p_wait.usernames[player_id],
                        players[player_id].username);

                state = WAIT;

                if (sendto_user(players[player_id],
                    (void*)&spack, sizeof(spack)) < 0) {
                    state = FIN;
                    printf("Room %x: Failed send package to user!\n", getpid());
                } else {
                    ++player_id;
                }
                break;

            case WAIT:
                if (msgrcv(msgid, &msg, sizeof(msg), MSG_JOIN, 0) < 0) {
                    state = FIN;
                    printf("Room %x: Message queue failed!\n", getpid());
                    break;
                }

                players[player_id] = msg.player_info;
                score[player_id] = 0;

                spack.type = S_WAIT;
                spack.p_wait.occupancy = player_id;
                spack.p_wait.room_size = room_size;
                strcpy(spack.p_wait.usernames[player_id],
                        players[player_id].username);
                ++player_id;

                if(player_id == room_size) {
                    state = GAME;
                }

                for (int i = 0; i < player_id; ++i) {
                    spack.p_wait.id = i;

                    if (sendto_user(players[i],
                        (void*)&spack, sizeof(spack)) < 0) {
                        state = FIN;
                        printf("Room %x: Failed send package to user!\n",
                                getpid());
                        break;
                    }
                }
                break;

            case GAME:
                if (num_quest != 0) {
                    step = rand() % MAX_STEP;
                    for (int i = 0; i < step; ++i) {
                        units = units->next;
                    }

                    strcpy(spack.p_game.quest, units->quest);
                    for (int i = 0; i < ANS_COUNT; ++i) {
                        strcpy(spack.p_game.ans[i], units->ans[i]);
                    }
                }

                spack.type = S_GAME;
                for (int i = 0; i < room_size; ++i) {
                    spack.p_game.score[i] = score[i];
                }
                spack.p_game.quest_num = num_quest;


                /* Sending questions to players */
                for(player_id = 0; player_id < room_size; ++player_id) {
                    if (score[player_id] >= PLR_LOST &&
                        sendto_user(players[player_id],
                                    (void*)&spack, sizeof(spack)) < 0) {
                        score[player_id] = PLR_DISCONNECT;
                        printf("Room %x: Player %s disconnected!\n",
                                getpid(), players[player_id].username);
                    }
                    if (score[player_id] == PLR_LOST) {
                        score[player_id] = PLR_DISCONNECT;
                    }
                }

                /* Receiving answers from players */
                for(player_id = 0; player_id < room_size; ++player_id) {
                    if (score[player_id] >= 0 &&
                        recvfrom_user(players[player_id],
                                    (void*)&cpack, sizeof(cpack)) < 0) {
                        score[player_id] = PLR_DISCONNECT;
                        printf("Room %x: Player %s disconnected!\n",
                                getpid(), players[player_id].username);
                    } else if (cpack.p_ans.ans == units->rans) {
                        score[player_id]++;
                    } else {
                        score[player_id] = PLR_LOST;
                    }
                }

                if (num_quest > LAST_QUEST) {
                    num_quest = 0;
                } else if (num_quest == 0) {
                    state = GAMEOVER;
                } else {
                    ++num_quest;
                }
                break;

            case GAMEOVER:
                for (player_id = 0; player_id < room_size; ++player_id) {
                    close(players[player_id].socket);
                }
                free(players);
                players = NULL;
                state = INIT;
                break;

            case FIN:
                for (player_id = 0; player_id < room_size; ++player_id) {
                    if (close(players[player_id].socket) < 0) {
                        break;
                    }
                }
                if (players != NULL) {
                    free(players);
                }
                state = EXIT;
                break;
        }
    }
}


int sendto_user(struct player user, void *data, unsigned int data_size)
{
    return send(user.socket, data, data_size, 0);
}


int recvfrom_user(struct player user, void *data, unsigned int data_size)
{
    return recv(user.socket, data, data_size, 0);
}
