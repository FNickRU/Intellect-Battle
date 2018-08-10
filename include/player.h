/**
 * Player structure definition.
 */
#ifndef __PLAYER_H
#define __PLAYER_H


/**
 * Structure of player.
 * @define USERNAME_LEN Length of player's nickname.
 * @param  socket       Socket's descriptor for connection with player.
 * @param  username     Player's nickname.
 */
#define USERNAME_LEN 40
struct player_info {
    int socket;
    char username[USERNAME_LEN];
};
typedef struct player_info player_t;

#endif // __PLAYER_H
