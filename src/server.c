/**
 * Game server main source code.
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "server.h"
#include "worker.h"
#include "room.h"
#include "unit.h"

#define PORT 1111
#define USER_QUEUE_SIZE 4

void init_server(int wnum, int rnum)
{
    /**
     * Initialize the signal handler
     */
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = signal_handler;

    sigset_t set;
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
     * Add a Socket
     */
    if ((Server.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        server_error_handler(ERROR_INIT_SOCKET);
        exit(4);
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);

    if (bind(Server.socket, (struct sockaddr *) &server, sizeof(server)) < 0) {
        server_error_handler(ERROR_BIND_SOCKET);
        close(Server.socket);
        exit(5);
    }

    /**
     * Make message queue descriptor
     */
    key_t key = ftok("Makefile", 'w');
    int msgqid = msgget(key, IPC_CREAT | 0664);
    if (msgqid < 0) {
        server_error_handler(ERROR_MSG_QUEUE);
        close(Server.socket);
        exit(1);
    }
    Server.msgqid = msgqid;

    /**
     * Add unit point
     */
    Server.units = unit_deque_init();
    if (!Server.units) {
        server_error_handler(ERROR_INIT_UNITS);
        close(Server.socket);
        msgctl(Server.msgqid, IPC_RMID, 0);
        exit(4);
    }

    /**
     * Make pools.
     */
    pthread_t *worker_handlers = (pthread_t *) malloc(sizeof(pthread_t) * wnum);
    if (!(worker_handlers)) {
        server_error_handler(ERROR_OUT_OF_MEMORY);
        close(Server.socket);
        msgctl(Server.msgqid, IPC_RMID, 0);
        unit_deque_free(Server.units);
        exit(2);
    }
    Server.workers.tid = worker_handlers;
    Server.workers.size = wnum;

    pthread_t *room_handlers = (pthread_t*) malloc(sizeof(pthread_t) * rnum);
    if (!(room_handlers)) {
        server_error_handler(ERROR_OUT_OF_MEMORY);
        close(Server.socket);
        msgctl(Server.msgqid, IPC_RMID, 0);
        unit_deque_free(Server.units);
        free(worker_handlers);
        exit(2);
    }
    Server.workers.tid = room_handlers;
    Server.workers.size = rnum;

    /**
     * Launch pools.
     */
    worker_t workers[wnum];
    for (int idx = 0; idx < wnum; ++idx) {
        workers[idx].msgqid = Server.msgqid;
        workers[idx].sync = SYNC_OFF;
        pthread_create(&worker_handlers[idx], NULL, &worker_fsm, &workers[idx]);
    }

    room_t rooms[rnum];
    for (int idx = 0; idx < rnum; ++idx) {
        rooms[idx].msgqid = Server.msgqid;
        rooms[idx].units = Server.units;
        rooms[idx].sync = SYNC_OFF;
        pthread_create(&room_handlers[idx], NULL, &room_fsm, &rooms[idx]);
    }

    /**
     * Synchronization
     */
    int thread_idx = 0;
    while (thread_idx < wnum) {
        if (workers[thread_idx].sync == SYNC_ON) {
            ++thread_idx;
        } else {
            sleep(1);
        }
    }
    thread_idx = 0;
    while (thread_idx < rnum) {
        if (rooms[thread_idx].sync == SYNC_ON) {
            ++thread_idx;
        } else {
            sleep(1);
        }
    }
}


/**
 * Start the receive message loop
 */
void loop_recv()
{
    /**
     * Now we are ready to receive messages.
     */
    int lis_sock = Server.socket;
    listen(lis_sock, USER_QUEUE_SIZE);

    struct sockaddr_in client;

    int msg_size = sizeof(con_t) - sizeof(long);
    socklen_t len = sizeof(struct sockaddr_in);

    con_t connection = {.type = MSG_WRK, .socket = 0};
    int msgqid = Server.msgqid;

    printf("Server enters connection loop\n");

    while (1) {
        int cl_sock;
        if ((cl_sock = accept(lis_sock, (struct sockaddr *) &client,
                              &len)) < 0) {
            server_error_handler(ERROR_ACCEPT);
            server_finalize();
        }

        printf("Server received the connection from %x\n", cl_sock);

        connection.socket = cl_sock;
        msgsnd(msgqid, &connection, msg_size, 0);
    }
}

int server_error_handler(int error_code)
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
    switch (error_code) {
        case 1:
            printf("Server: Can't make a message queue!\n");
            break;
        case 2:
            printf("Server: Out of memory!\n");
            break;
        case 3:
            printf("Server: Can't initialize a unit!\n");
            break;
        case 4:
            printf("Server: Can't get a socket!\n");
            break;
        case 5:
            printf("Server: Can't bind a socket!\n");
            break;
        case 7:
            printf("Server: Can't accept the connection!\n");
            break;
        default:
            printf("Server: Unknown error!\n");
            break;
    }
    return 0;
}

void signal_handler(int signum)
{
    printf("SIGNAL = %d\n", signum);
    server_finalize();
    exit(0);
}

void server_finalize()
{
    close(Server.socket);
    msgctl(Server.msgqid, IPC_RMID, 0);

    for (int idx = 0; idx < Server.workers.size; ++idx) {
        pthread_join(Server.workers.tid[idx], NULL);
    }
    for (int idx = 0; idx < Server.rooms.size; ++idx) {
        pthread_join(Server.rooms.tid[idx], NULL);
    }

    free(Server.workers.tid);
    free(Server.rooms.tid);
    unit_deque_free(Server.units);

    printf("Server shutdown\n");

    exit(1);
}
