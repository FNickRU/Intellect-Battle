/**
 * Player structure definition.
 */
#ifndef __PLAYER_H
#define __PLAYER_H


/**
 * Structure of player. Workers are creating, rooms are using.
 * @define USERNAME_LEN Length of player's nickname.
 * @param  socket       Socket's descriptor for connection with player.
 * @param  username     Player's nickname.
 */
#define USERNAME_LEN 40
struct player {
    int socket;
    char username[USERNAME_LEN];
};

#endif // __PLAYER_H
