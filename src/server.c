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
#include "unit.h"

#define PORT 1111
#define BACKLOG 4

struct server_conf *finalize_conf;

struct server_conf *init_server(char *db_path,int wnum, int rnum)
{
 
    /**
     * initialize the signal handler
     */
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
        server_error_handler(ERROR_MSG_QUEUE);
        exit(1);
    }
    
    /**
     * Make pools.
     */
    pthread_t *worker_handlers = (pthread_t*)malloc(sizeof(pthread_t)*wnum);
    if (!(worker_handlers)) {
        server_error_handler(ERROR_OUT_OF_MEMORY);
        exit(2);
    }
    pthread_t *room_handlers = (pthread_t*)malloc(sizeof(pthread_t)*rnum);
    if (!(room_handlers)) {
        server_error_handler(ERROR_OUT_OF_MEMORY);
        free(worker_handlers);
        exit(2);
    }
    
    /**
     * Inializing a server configuration
     */
    struct server_conf *cfg = (struct server_conf*)malloc(sizeof(struct server_conf));
    if (!(cfg)) {
        server_error_handler(ERROR_OUT_OF_MEMORY);
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
     * Add a Socket
     */
    cfg->socket;
    if ((cfg->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        server_error_handler(ERROR_INIT_SOCKET);
        free(worker_handlers);
        free(room_handlers);
        free(cfg);
        exit(4);
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(PORT);
    
    if (bind(cfg->socket, &addr, sizeof(struct sockaddr_in)) < 0) {
        close(cfg->socket);
        server_error_handler(ERROR_BIND_SOCKET);
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
        server_error_handler(ERROR_INIT_UNITS);
        free(worker_handlers);
        free(room_handlers);
        free(cfg);
        close(cfg->socket);
        exit(4);
    }
    
    finalize_conf = cfg;
    
    return cfg;
}


/**
 * Start the receive message loop
 */
int loop_recv(int socket, int msgid)
{
    struct msg c_msg;
    int msg_size = sizeof(struct msg) - sizeof(long);
    
    /**
     *  Now we are ready to receive messages.
     */
    listen(socket, BACKLOG);
    
    
    c_msg.type = MSG_WRK; 
    socklen_t socket_length = sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;
    while (1) {
        char request[250];
        
        int serverd;
        if ((serverd = accept(socket, &client_addr, &socket_length)) < 0) {
            server_error_handler(ERROR_ACCEPT);
            finalize(finalize_conf);
        }
        
        /**
         * Put the message into the queue
         */
        c_msg.socket = serverd;
        
        msgsnd(msgid, &c_msg, msg_size, 0);
    }
    return 0;
}

int server_error_handler(int errno)
{
/**
 * ERROR CODE
 * 1 - Can't make a msg queue
 * 2 - Out of memory
 * 3 - Can't initialize a units
 * 4 - Can't get a socket
 * 5 - Can't bind a socket
 * 6 - Old errorcode
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
    msgctl(conf->msgid, IPC_RMID, 0);
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
