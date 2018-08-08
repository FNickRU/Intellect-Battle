//
// Created by michail on 08.08.18.
//

#ifndef PROJECT_CLIENT_UI_H
#define PROJECT_CLIENT_UI_H

/**
 * Drawing main menu with choice of which game should be started
 * @param create_button reference to already created ncurses window, which would be filled with "CREATE GAME"
 * @param join_button reference to already created ncurses window, which would be filled with "JOIN GAME"
 * @param exit_button reference to already created ncurses window, which would be filled with "EXIT GAME"
 */
void main_menu_redraw(WINDOW *create_button, WINDOW *join_button, WINDOW *exit_button);

/**
 * creates new input box and reads entered string
 * @param nickname reference to buffer, where nickname would be presented
 * @return updated buffer in nickname
 * @warning not checking for buffer overflow
 */
void get_nickname(char *nickname);

/**
 * shows menu where user can choose one of presented room sizes
 * @param nickname used to redraw Main Menu properly
 * @param create_button used to redraw Main Menu properly
 * @param join_button used to redraw Main Menu properly
 * @param exit_button used to redraw Main Menu properly
 * @param selectedBehaviour standard behaviour to be returned
 * @return selected size of the room or 'BACK_TO_MAIN_MENU' define
 */
char
get_behaviour(const char *nickname, WINDOW *create_button, WINDOW *join_button, WINDOW *exit_button,
              char selectedBehaviour);

/**
 * redraws window of in game interface (question + answers + users)
 * @param server_status reference to window server_status, where outputs "status" of server
 * @param question_window reference to question_window, where outputs actual "question"
 * @param answer reference to array of windows, where outputs actual "answers"
 */
void redraw_game_window(WINDOW *server_status, WINDOW *question_window, WINDOW *answer[4]);

/**
 * outputs nicknames of users in current room
 * @param r_info reference to info about room
 * @param nicks reference to 4 windows where should be written nicknames
 * @param bwindow reference to window, which would be used as input field. Doesn't do anything (obsolete)
 */
void print_nickname(struct room_info *r_info, WINDOW *nicks[4], WINDOW *bwindow);

void delete_wins(WINDOW *win1, WINDOW *win2, WINDOW *win3);

char select_size();

//shows buttons of available room sizes
void highlight_selected(WINDOW* a_window, char a_text[A_LEN], int highlight_type);

//main game loop
int game_loop(WINDOW *answer[4], WINDOW *system_info, WINDOW *question_window, struct room_info r_info);

void print_nickname(struct room_info *r_info, WINDOW *nicks[4], WINDOW *bwindow);

//returns size of room, desired by user
char mainMenu(const char nickname[52]);

void refresh_values(char *desiredSize);

int error_window(int error_type, bool is_retryable);

void wait_players(WINDOW *answer[4], WINDOW *system_info, WINDOW *question_window);

#endif //PROJECT_CLIENT_UI_H
