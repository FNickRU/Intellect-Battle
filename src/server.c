/**
 * Game server main source code.
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>

#include "server.h"
#include "worker.h"
#include "room.h"

#define PORT 11115
#define BACKLOG 4

#define ERROR_MSG_QUEUE     1
#define ERROR_OUT_OF_MEMORY 2
#define ERROR_INIT_UNITS    3
#define ERROR_INIT_SOCKET   4
#define ERROR_BIND_SOCKET   5 
#define ERROR_RECIEVE_MSG   6
#define ERROR_ACCEPT        7

//Заглушка
void room_fsm(void *msgid)
{
    return 0;
}
void worker_fsm(void *msgid)
{
    return 0;
}


//TODO: Transer its to another place.
struct mbuf {
    long type;
    int addr;
    char buff[250];
};

struct server_conf *finalize_conf;


struct server_conf *init_server(char *db_path,int wnum, int rnum)
{
    
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = signal_handler;
    sigset_t   set; 
    sigemptyset(&set);                                                             
    sigaddset(&set, SIGUSR1); 
    sigaddset(&set, SIGUSR2);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGABRT);
    act.sa_mask = set;
    sigaction(SIGUSR1, &act, 0);
    sigaction(SIGUSR2, &act, 0);
    sigaction(SIGINT, &act, 0);
    sigaction(SIGABRT, &act, 0);
    
    
    /**
     * Make qeue descriptor
     */
    key_t key = ftok("Makefile", 'w');
    int msgqid = msgget(key, IPC_CREAT | 0664);
    if (msgqid < 0) {
        //perror("Can't create a msg queue"); transferred to error_handler
        error_handler(ERROR_MSG_QUEUE);
        exit(1);
    }
    
    /**
     * Make pools.
     */
    pthread_t *worker_handlers = (pthread_t*)malloc(sizeof(pthread_t)*wnum);
    if (!(worker_handlers)) {
        error_handler(ERROR_OUT_OF_MEMORY);
        exit(2);
    }
    pthread_t *room_handlers = (pthread_t*)malloc(sizeof(pthread_t)*rnum);
    if (!(room_handlers)) {
        error_handler(ERROR_OUT_OF_MEMORY);
        free(worker_handlers);
        exit(2);
    }
    
    /**
     * Inializing a server configuration
     */
    struct server_conf *cfg = (struct server_conf*)malloc(sizeof(struct server_conf));
    if (!(cfg)) {
        error_handler(ERROR_OUT_OF_MEMORY);
        free(worker_handlers);
        free(room_handlers);
        exit(2);
    }
    
    /**
     * Make worker pool.
     */
    for (int i = 0; i < wnum; i++) {
        pthread_create(&worker_handlers[i], NULL, &room_fsm, &msgqid);
    }
    
    /**
     * Make room pool.
     */
    for (int i = 0; i < rnum; i++) {
        pthread_create(&room_handlers[i], NULL, &worker_fsm, &msgqid);
    }
    
    /**
     * Add Socket
     */
    cfg->socket;
    if ((cfg->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        error_handler(ERROR_INIT_SOCKET);
        free(worker_handlers);
        free(room_handlers);
        free(cfg);
        exit(4);
    }
    
    //TODO: Get right IP adress.
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(PORT);
    
    if (bind(cfg->socket, &addr, sizeof(struct sockaddr_in)) < 0) {
        //perror("Can't bind a socket"); transferred to error_handler
        close(cfg->socket);
        error_handler(ERROR_BIND_SOCKET);
        free(worker_handlers);
        free(room_handlers);
        free(cfg);
        exit(5);
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
        error_handler(ERROR_INIT_UNITS);
        free(worker_handlers);
        free(room_handlers);
        free(cfg);
        close(cfg->socket);
        exit(4);
    }
    
    finalize_conf = cfg;
    
    return cfg;
}

int loop_recv(int socket, int msgid)
{
    struct mbuf msg;
    int msg_size = sizeof(struct mbuf) - sizeof(long);
    
    listen(socket, BACKLOG);
    
    //TODO:
    msg.type = 0; 
    socklen_t socket_length = sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;
    while (1) {
        //TODO:
        char request[250];
        
        int serverd;
        if ((serverd = accept(socket, &client_addr, &socket_length)) < 0) {
            error_handler(ERROR_ACCEPT);
            finalize(finalize_conf);
        }
        /**
         * get message from client
         */
        if (recv(serverd, request, 250, 0) <= 0) {
            error_handler(ERROR_RECIEVE_MSG);
            finalize(finalize_conf);
        }

        /**
         * put the message into the queue
         */
        msg.addr = serverd;
        
        strncpy(msg.buff, request, sizeof(request));
        
        msgsnd(msgid, &msg, msg_size, 0);
    }
    return 0;
}

int error_handler(int errno)
{
/**
 * ERROR CODE
 * 1 - Can't make a msg queue
 * 2 - Out of memory
 * 3 - Can't initialize a units
 * 4 - Can't get a socket
 * 5 - Can't bind a socket
 * 6 - Can't recieve msg from LOOP
 * 7 - Can't accept the connection
*/
    switch ( errno )  
    {
        case 1:
            printf("ERROR: Can't make a msg queue\n");
            break;
        case 2:
            printf("ERROR: Out of memory\n");
            break;
        case 3:
            printf("ERROR: Can't initialize a unit\n");
            break;
        case 4:
            printf("ERROR: Can't get a socket\n");
            break;
        case 5:
            printf("ERROR: Can't bind a socket\n");
            break;
        case 6:
            printf("ERROR: Can't recieve msg from LOOP\n");
            break;
        case 7:
            printf("ERROR: Can't accept the connection\n");
            break;
        default:
            printf("ERROR: The errno is not included in the switch\n");
            break;
    }
    return 0;
}

int signal_handler(int signal)
{
    printf("SIGNAL = %d\n",signal);
    finalize(finalize_conf);
    exit(0);
    return 0;
}

int finalize(struct server_conf *conf)
{
    close(conf->socket);
    
    for (int i = 0 ; i < conf->workers.size ; i++)
    {
        pthread_join(conf->workers.tid[i], NULL);
    }
    for (int i = 0 ; i < conf->rooms.size ; i++)
    {
        pthread_join(conf->rooms.tid[i], NULL);
    }
    
    unit_free_all(conf->units);
    
    free(conf);
    exit(1);
    return 0;
}
