#include <malloc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#include "room.h"
#include "packet.h"

enum State {INIT, WAIT, GAME, GAMEOVER, FIN, EXIT};

void *room_fsm(void *arg)
{
    room_t *info = (room_t *) arg;

    int msgqid = info->msgqid;
    unit_t *cur_unit = info->units;

    info->sync = SYNC_ON;

    join_t msg;
    spack_t spack;
    cpack_t cpack;

    char score[USER_COUNT];
    player_t players[USER_COUNT];

    unsigned char room_size, last_plr_id, quest_num;

    int state = INIT;
    char user_alive_counter;
    while (state != EXIT) {
        switch (state) {
            case INIT:
                printf("Room %lx wait for first connection\n", pthread_self());

                room_size = 0;
                last_plr_id = 0;
                quest_num = 1;

                bzero(&spack, sizeof(spack));

                if (msgrcv(msgqid, &msg, sizeof(msg), MSG_ROOM, 0) < 0) {
                    printf("Room %lx: message queue failed! (Reason: %s)\n",
                           pthread_self(), strerror(errno));

                    state = FIN;
                    break;
                }
                printf("Room %lx received connection from %x\n",
                       pthread_self(), msg.player.socket);


                room_size = msg.room_size;

                players[0] = msg.player;
                score[0] = 0;

                spack.type = S_WAIT;
                spack.p_wait.occupancy = 1;
                spack.p_wait.room_size = room_size;
                spack.p_wait.id = 0;
                strcpy(spack.p_wait.usernames[0], players[0].username);

                if (sendto_user(players[0], &spack, sizeof(spack)) < 0) {
                    printf("Room %lx: Failed send package to user!\n",
                           pthread_self());

                    state = FIN;
                } else {
                    printf("Room %lx initialized\n", pthread_self());

                    state = WAIT;
                }

                ++last_plr_id;
                break;

            case WAIT:
                printf("Room %lx wait for connection (%d/%d)\n",
                       pthread_self(), last_plr_id, room_size);

                if (msgrcv(msgqid, &msg, sizeof(msg), MSG_JOIN, 0) < 0) {
                    printf("Room %lx: message queue failed! (Reason: %s)\n",
                           pthread_self(), strerror(errno));

                    state = FIN;
                    break;
                }
                printf("Room %lx received connection from %x\n",
                       pthread_self(), msg.player.socket);

                players[last_plr_id] = msg.player;
                score[last_plr_id] = 0;

                spack.type = S_WAIT;
                spack.p_wait.occupancy = last_plr_id + 1;
                spack.p_wait.room_size = room_size;
                strcpy(spack.p_wait.usernames[last_plr_id],
                        players[last_plr_id].username);

                if (last_plr_id + 1 == room_size) {
                    state = GAME;
                }
                ++last_plr_id;

                for (int id = 0; id < last_plr_id; ++id) {
                    spack.p_wait.id = id;

                    if (sendto_user(players[id], &spack, sizeof(spack)) < 0) {
                        printf("Room %lx: Failed send package to user %d!\n",
                                pthread_self(), players[id].socket);

                        score[id] = PLR_DISCONNECT;
                        break;
                    }
                }
                break;

            case GAME:
                printf("Room %lx: round %d!\n", pthread_self(), quest_num);

                if (quest_num != 0) {
                    int step = rand() % MAX_STEP + 1;

                    for (int it = 0; it < step; ++it) {
                        cur_unit = cur_unit->next;
                    }

                    strcpy(spack.p_game.quest, cur_unit->quest);

                    for (int it = 0; it < ANS_COUNT; ++it) {
                        strcpy(spack.p_game.ans[it], cur_unit->ans[it]);
                    }
                } else {
                    state = GAMEOVER;
                    break;
                }

                spack.type = S_GAME;
                for (int i = 0; i < room_size; ++i) {
                    spack.p_game.score[i] = score[i];
                }
                spack.p_game.quest_num = quest_num;


                /**
                 * Sending questions to players.
                 */
                user_alive_counter = 0;

                for (int id = 0; id < room_size; ++id) {
                    if (score[id] >= PLR_LOST) {
                        if (sendto_user(players[id], &spack,
                                        sizeof(spack)) < 0) {
                            printf("Room %lx: Player %s disconnected!\n",
                                   pthread_self(), players[id].username);

                            score[id] = PLR_DISCONNECT;
                        } else {
                            user_alive_counter++;
                        }
                    }

                    if (score[id] == PLR_LOST) {
                        score[id] = PLR_DISCONNECT;
                    }
                }

                if (user_alive_counter <= 1) {
                    state = GAMEOVER;
                    break;
                }

                /**
                 * Receiving answers from players.
                 */
                for (int id = 0; id < room_size; ++id) {
                    if (score[id] > PLR_LOST) {
                        if (recvfrom_user(players[id], &cpack,
                                          sizeof(cpack)) < 0) {
                              printf("Room %lx: Player %s disconnected!\n",
                                     pthread_self(), players[id].username);

                              score[id] = PLR_DISCONNECT;
                        } else if (cpack.p_ans.ans == cur_unit->rans) {
                            score[id]++;
                        } else {
                            score[id] = PLR_LOST;
                        }
                    }
                }

                if (quest_num == LAST_QUEST) {
                    quest_num = 0;
                } else {
                    ++quest_num;
                }
                break;

            case GAMEOVER:
                printf("Room %lx: game over\n", pthread_self());

                for (int id = 0; id < room_size; ++id) {
                    close(players[id].socket);
                }

                state = INIT;
                break;

            case FIN:
                printf("Room %lx finalized\n", pthread_self());

                for (int id = 0; id < last_plr_id; ++id) {
                    close(players[id].socket);
                }

                state = EXIT;
                break;
        }
    }

    return NULL;
}


int sendto_user(player_t user, void *data, unsigned int data_size)
{
    return send(user.socket, data, data_size, 0);
}


int recvfrom_user(player_t user, void *data, unsigned int data_size)
{
    return recv(user.socket, data, data_size, 0);
}
