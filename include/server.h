/**
 * The header of the server interface.
 */
#ifndef __SERVER_H
#define __SERVER_H


#include <pthread.h>
#include "unit.h"

#define ERROR_MSG_QUEUE     1
#define ERROR_OUT_OF_MEMORY 2
#define ERROR_INIT_UNITS    3
#define ERROR_INIT_SOCKET   4
#define ERROR_BIND_SOCKET   5
#define ERROR_RECIEVE_MSG   6
#define ERROR_ACCEPT        7

/**
* This structure describes the configuration
* of the thread pool (workers or rooms).
*/
struct pool {
    pthread_t* tid;
    int size;
};


/**
 * This structure describes the configuration of the server.
 */
struct server_conf {
    int socket;
    int msgid;
    struct pool workers;
    struct pool rooms;
    struct unit *units;
};


/**
 * Server initialization function.
 * @param db_path - path to the questions database
 * @param wnum - size of the pool of workers
 * @param wnum - room pool size
 * @return struct server_conf
 */
struct server_conf *init_server(char *db_path, int wnum, int rnum);


/**
 * Function of accepting new connections.
 * @param socket - server socket received after initialization
 * @param msgid - message queue identifier
 * @return status
 */
int loop_recv(int socket, int msgid);


/**
 * Error handling function.
 * @param errno - error code
 * @return status
 */
int server_error_handler(int errror_code);


/**
 * SIGTERM and SIGINT signal handling function.
 * @param signal - received signal
 * @return status
 */
void signal_handler(int signal);


/**
 * Function beautiful shutdown of the server.
 * @param conf - server configuration received after initialization
 * @return status
 */
int server_finalize(struct server_conf *conf);


#endif //__SERVER_H
