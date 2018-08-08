#include "client_logic.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>


#define SERVER_PORT 1111


void init(char* name)
{
    Client_info.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int name_len = strlen(name);
    if (name_len > USERNAME_LEN)
        name_len = USERNAME_LEN;
    strncpy(Client_info.username, name, name_len);
}


int connect_to_server()
{
    struct sockaddr_in server;
    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(SERVER_PORT);
    return connect(Client_info.socket, (struct sockaddr *) &server,
        sizeof(server));
}


int send_conf(char type, char size)
{
    struct c_pack conf;
    conf.type = C_JOIN;
    switch (type) {
    case REQ_JOIN:
        conf.p_join.type = REQ_JOIN;
        conf.p_join.room_size = USER_COUNT;
        break;
    case REQ_CREATE:
        conf.p_join.type = REQ_CREATE;
        conf.p_join.room_size = size;
        break;
    default:
        return CODE_FAILURE;
        break;
    }
    strncpy(conf.p_join.username, Client_info.username, USERNAME_LEN);
    return send(Client_info.socket, &conf, sizeof(conf), 0);
}



int wait_for_players(struct room_info *room)
{
    if (room == NULL)
        return CODE_FAILURE;
    struct s_pack room_conf;
    if (recv(Client_info.socket, &room_conf, sizeof(room_conf), 0) < 0)
        return CODE_FAILURE;
    switch (room_conf.type) {
    case S_WAIT:
        room->occupancy = room_conf.p_wait.occupancy;
        room->room_size = room_conf.p_wait.room_size;
        room->id = room_conf.p_wait.id;
        for (int i = 0; i < USER_COUNT; i++) {
            strncpy(room->usernames[i], room_conf.p_wait.usernames[i],
                USERNAME_LEN);
            room->score[i] = 0;
        }
        if (room->occupancy < room->room_size)
            return WAIT_MORE;
        else
            return START_GAME;
        break;
    case S_GAME:
    default:
        return CODE_FAILURE;
        break;
    }
    /**
     * Dead code.
     */
    return CODE_FAILURE;
}


int get_unit(struct unit *u, struct room_info *room)
{
    if (u == NULL || room == NULL)
        return CODE_FAILURE;
    struct s_pack room_conf;
    if (recv(Client_info.socket, &room_conf, sizeof(room_conf), 0) < 0)
        return CODE_FAILURE;
    if (room_conf.p_game.quest_num == 0)
        return GAME_OVER;
    for (int i = 0; i < USER_COUNT; i++)
        room->score[i] = room_conf.p_game.score[i];
    if (room->score[room->id] < 0)
        return GAME_OVER;
    strncpy(u->quest, room_conf.p_game.quest, Q_LEN);
    for (int i = 0; i < ANSWER_COUNT; i++)
        strncpy(u->ans[i], room_conf.p_game.ans[i], A_LEN);
    return GAME_CONT;
}


int send_ans(char ans, struct timeval timestamp)
{
    if (ans < 0 || ans >= ANSWER_COUNT)
        return CODE_FAILURE;
    struct c_pack answer;
    answer.type = C_ANS;
    answer.p_ans.ans = ans;
    answer.p_ans.timestamp = timestamp;
    return send(Client_info.socket, &answer, sizeof(answer), 0);
}


int is_loser(struct room_info room)
{
    if (room.score[room.id] > 0)
        return 1;
    return 0;
}


int finalize()
{
    return close(Client_info.socket);
}
