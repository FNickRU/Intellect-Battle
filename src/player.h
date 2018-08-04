/**
 * Player structure definition.
 */


/**
 * Structure of player. Workers is creating, rooms is using.
 * @define USERNAME_LEN Length of player's nickname.
 * @param  socket       Socket's descriptor for connection with player.
 * @param  username     Player's nickname.
 */
#define USERNAME_LEN 40
struct player {
    int socket;
    char username[USERNAME_LEN];
};
