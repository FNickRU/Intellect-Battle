/**
 * The header of the server interface.
 */
#ifndef __SERVER_H
#define __SERVER_H


#include <pthread.h>


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
    pool workers;
    pool rooms;
};


/**
 * Server initialization function.
 * @param db_path - path to the questions database
 * @param wnum - size of the pool of workers
 * @param wnum - room pool size
 * @return struct server_conf
 */
struct server_conf init_server(char *db_path, int wnum, int rnum);


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
int error_handler(int errno);


/**
 * SIGTERM and SIGINT signal handling function.
 * @param signal - received signal
 * @return status
 */
int signal_handler(int signal);


/**
 * Function beautiful shutdown of the server.
 * @param conf - server configuration received after initialization
 * @return status
 */
int finalize(struct server_conf *conf);


#endif //__SERVER_H
