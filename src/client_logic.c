#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "client_logic.h"

int sendto_server(cpack_t send_pack)
{
    if (send(Client.socket, &send_pack, sizeof(send_pack), 0) < 0) {
        return CODE_FAILURE;
    }
    return CODE_SUCCESS;
}

int recvfrom_server(spack_t *recv_pack)
{
    if (recv(Client.socket, recv_pack, sizeof(recv_pack), 0) < 0) {
        return CODE_FAILURE;
    }
    return CODE_SUCCESS;
}

void init(char* name)
{
    Client.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    int name_len = strlen(name);
    if (name_len >= USERNAME_LEN) {
        name_len = USERNAME_LEN - 1;
    }

    strncpy(Client.username, name, name_len);

    Client.username[USERNAME_LEN - 1] = '\0';
}

int connect_to_server()
{
    struct sockaddr_in server;

    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP_STR);
    server.sin_port = htons(SERVER_PORT);

    return connect(Client.socket, (struct sockaddr *) &server, sizeof(server));
}

int send_conf(char type, char size)
{
    cpack_t conf;

    conf.type = C_JOIN;

    switch (type) {
        case REQ_JOIN:
            conf.p_req.type = REQ_JOIN;
            conf.p_req.room_size = USER_COUNT;
            break;

        case REQ_CREATE:
            conf.p_req.type = REQ_CREATE;
            conf.p_req.room_size = size;
            break;

        default:
            return CODE_FAILURE;
    }

    strncpy(conf.p_req.username, Client.username, USERNAME_LEN);

    return sendto_server(conf);
}

int wait_for_players(roominfo_t *room)
{
    if (room == NULL) {
        return CODE_FAILURE;
    }

    spack_t room_conf;
    if (recvfrom_server(&room_conf) == CODE_FAILURE) {
        return CODE_FAILURE;
    }

    switch (room_conf.type) {
        case S_WAIT:
            room->occupancy = room_conf.p_wait.occupancy;
            room->room_size = room_conf.p_wait.room_size;
            room->id = room_conf.p_wait.id;

            for (int idx = 0; idx < USER_COUNT; ++idx) {
                strncpy(room->usernames[idx], room_conf.p_wait.usernames[idx],
                        USERNAME_LEN);
                room->score[idx] = 0;
            }

            if (room->occupancy < room->room_size) {
                return WAIT_MORE;
            } else {
                return START_GAME;
            }
            break;

        /**
         * wait_for_players() shouldn't receive game packets
         */
        case S_GAME:
            return CODE_FAILURE;

        default:
            return CODE_FAILURE;
            break;
    }

    /**
     * Dead code.
     */
    return CODE_FAILURE;
}


int get_unit(unit_t *unit, roominfo_t *room)
{
    if (unit == NULL || room == NULL) {
        return CODE_FAILURE;
    }

    spack_t room_conf;

    if (recvfrom_server(&room_conf) == CODE_FAILURE) {
        return CODE_FAILURE;
    }

    if (room_conf.p_game.quest_num == 0) {
        return GAME_OVER;
    }

    for (int idx = 0; idx < USER_COUNT; ++idx) {
        room->score[idx] = room_conf.p_game.score[idx];
    }

    if (is_loser(*room)) {
        return GAME_OVER;
    }

    strncpy(unit->quest, room_conf.p_game.quest, Q_LEN);
    for (int it = 0; it < ANSWER_COUNT; ++it) {
        strncpy(unit->ans[it], room_conf.p_game.ans[it], A_LEN);
    }

    return GAME_CONT;
}


int send_ans(char ans, struct timeval timestamp)
{
    if (ans < 0 || ans >= ANSWER_COUNT) {
        return CODE_FAILURE;
    }

    cpack_t answer;
    answer.type = C_ANS;
    answer.p_ans.ans = ans;
    answer.p_ans.timestamp = timestamp;

    return sendto_server(answer);
}


int is_loser(roominfo_t room)
{
    if (room.score[room.id] > 0) {
        return 0;
    }
    return 1;
}


int finalize()
{
    return close(Client.socket);
}
