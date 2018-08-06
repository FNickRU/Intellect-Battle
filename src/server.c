/**
 * Game server main source code.
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <arpa/inet.h>
#include <string.h>

//#include <pthread.h>
//#include <sys/types.h>
//#include <sys/ipc.h>
//#include <sys/socket.h>
//#include <unistd.h>

#include "server.h"

#define WNUM 5
#define RNUM 10
#define PORT 1111

/**
 * ERROR CODE
 * 1 - Can't make a msg queue
 * 2 - Out of memory
 * 3 - Can't initialize a units
 * 4 - Can't get a socket
 * 5 - Can't bind a socket
 */

/*
struct server_conf *init_server(char *db_path,int wnum, int rnum);

int loop_recv(int socket, int msgid);

int error_handler(int errno);

int signal_handler(int signal);

int finalize(struct server_conf *conf);
*/

//enum STATE {ERROR, INIT, LOOP, FIN};

//TODO: WORKER_FUNCTION <-> WORKER_FUNCTION_NAME from worker.h
void *WORKER_FUNCTION(void *arg) 
{
    return 0;
}

//TODO: ROOM_FUNCTION <-> ROOM_FUNCTION_NAME from room.
void *ROOM_FUNCTION(void *arg)
{
    return 0;
}

int main(int argc, char *argv[])
{    
    struct server_conf *cfg;
    cfg = init_server(argv[0],WNUM,RNUM);
    loop_recv(cfg->socket,cfg->msgid);
    
    return 0;
}

struct server_conf *init_server(char *db_path,int wnum, int rnum)
{
    /**
     * Make qeue descriptor
     */
    key_t key = ftok("Makefile", 'w');
    int msgqid = msgget(key, IPC_CREAT | 0664);
    if (msgqid < 0) {
        //perror("Can't create a msg queue"); transferred to error_handler
        error_handler(1);
    }
    
    /**
     * Make worker pool.
     */
    pthread_t *worker_handlers = (pthread_t*)malloc(sizeof(pthread_t)*wnum);
    if (!(worker_handlers)) {
        error_handler(2);
    }
    for (int i = 0; i < wnum; i++) {
        //TODO: WORKER_FUNCTION <-> WORKER_FUNCTION_NAME
        pthread_create(&worker_handlers[i], NULL, &WORKER_FUNCTION, &msgqid);
    }
    
    /**
     * Make room pool.
     */
    pthread_t *room_handlers = (pthread_t*)malloc(sizeof(pthread_t)*rnum);
    if (!(room_handlers)) {
        error_handler(2);
    }
    for (int i = 0; i < rnum; i++) {
        //TODO: ROOM_FUNCTION <-> ROOM_FUNCTION_NAME
        pthread_create(&room_handlers[i], NULL, &ROOM_FUNCTION, &msgqid);
    }
    
    /**
     * Inializing a server configuration
     */
    struct server_conf *cfg = (struct server_conf*)malloc(sizeof(struct server_conf));
    if (!(cfg)) {
        error_handler(2);
    }
    
    /**
     * Add Socket
     */
    cfg->socket;
    if ((cfg->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        //perror("Can't get UDP socket"); transferred to error_handler
        error_handler(4);
    }
    //TODO: Get right IP adress.
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(PORT);
    
    if (bind(cfg->socket, &addr, sizeof(struct sockaddr_in)) < 0) {
        //perror("Can't bind a socket"); transferred to error_handler
        error_handler(5);
    }
    
    /**
     * Add msgid
     */
    cfg->msgid = msgqid;
    
    /**
     * Add pool rooms
     */
    cfg->rooms.tid = room_handlers;
    cfg->rooms.size = rnum;
    
    /**
     * Add pool workers
     */
    cfg->workers.tid = worker_handlers;
    cfg->workers.size = wnum;
    
    /**
     * Add unit point
     */
    cfg->units = unit_init(db_path);
    if (!(cfg->units)) {
        error_handler(3);
    }
    
    return cfg;
}

int loop_recv(int socket, int msgid)
{
    //
    
    //
    return 0;
}

int error_handler(int errno)
{
    //?????
    return 0;
}

int signal_handler(int signal)
{
    //???
    return 0;
}

int finalize(struct server_conf *conf)
{
    //???
    return 0;
}
