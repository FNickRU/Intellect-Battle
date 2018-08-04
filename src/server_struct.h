/**
 * Server structures definitions.
 */


#include "unit.h"


/**
 * Structure describing array of workers or rooms.
 * @param array Pointer to array.
 * @param size  Size of array.
 */
struct pool {
    pthread* array;
    unsigned int size;
};


/**
 * Structure describing the server.
 * @param socket   Server's socket descriptor.
 * @param msgqid   Message queue's descriptor for server's modules interactions.
 * @param workers  Array of worker's threads.
 * @param rooms    Array of room's threads.
 * @param units    Array of questions.
 * @param unit_num Number of questions.
 */
struct system_info {
    int socket;
    int msgqid;
    pool workers;
    pool rooms;
    unit *units;
    unsigned int unit_num;
};
