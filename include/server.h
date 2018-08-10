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
    pthread_t *tid;
    int size;
};
typedef struct pool pool_t;

/**
 * This structure describes the configuration of the server.
 */
struct server_conf {
    int socket;
    int msgqid;
    pool_t workers;
    pool_t rooms;
    unit_t *units;
};
typedef struct server_conf sconf_t;

/**
 * Global server configuration.
 */
static sconf_t Server;

/**
 * Server initialization function.
 * @param wnum - size of the pool of workers
 * @param wnum - room pool size
 */
void init_server(int wnum, int rnum);

/**
 * Function of accepting new connections.
 * @return status
 */
void loop_recv();

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
 * @return status
 */
void server_finalize();

#endif //__SERVER_H
