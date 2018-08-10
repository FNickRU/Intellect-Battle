/**
 * The header of the client UI.
 * @author Silkin Michail
 * @since 08.08.18
 */

#ifndef __CLIENT_UI_H
#define __CLIENT_UI_H

/**
 * Drawing main menu with choice of which game should be started
 *
 * @param create_button reference to already created ncurses window,
 * which would be filled with "CREATE GAME"
 * @param join_button reference to already created ncurses window,
 * which would be filled with "JOIN GAME"
 * @param exit_button reference to already created ncurses window,
 * which would be filled with "EXIT GAME"
 */
void main_menu_redraw(WINDOW *create_button,
                      WINDOW *join_button,
                      WINDOW *exit_button);

/**
 * creates new input box and reads entered string
 *
 * @param nickname reference to buffer, where nickname would be presented
 * @return updated buffer in nickname
 * @warning not checking for buffer overflow
 */
void get_nickname(char *nickname);

/**
 * shows menu where user can choose one of presented room sizes
 *
 * @param nickname used to redraw Main Menu properly
 * @param create_button used to redraw Main Menu properly
 * @param join_button used to redraw Main Menu properly
 * @param exit_button used to redraw Main Menu properly
 * @param selectedBehaviour standard behaviour to be returned
 * @return selected size of the room or 'BACK_TO_MAIN_MENU' define
 */
char get_behaviour(const char *nickname,
                   WINDOW *create_button,
                   WINDOW *join_button,
                   WINDOW *exit_button,
                   char selectedBehaviour);

/**
 * redraws window of in game interface (question + answers + users)
 *
 * @param server_status reference to window server_status,
 * where outputs "status" of server
 * @param question_window reference to question_window,
 * where outputs actual "question"
 * @param answer reference to array of windows, where outputs actual "answers"
 */
void redraw_game_window(WINDOW *server_status,
                        WINDOW *question_window,
                        WINDOW *answer[4]);

/**
 * outputs nicknames of users in current room
 * @param r_info reference to info about room
 * @param nicks reference to 4 windows where should be written nicknames
 * @param bwindow reference to window, which would be used as input field.
 * Doesn't do anything (obsolete)
 */
void print_nickname(struct room_info *r_info,
                    WINDOW *nicks[4],
                    WINDOW *bwindow);

/**
 * removes 3 windows at once
 * (works like syntax sugar)
 * @param win1 window #1
 * @param win2 window #2
 * @param win3 window #3
 */
void delete_wins(WINDOW *win1, WINDOW *win2, WINDOW *win3);

/**
 * process selecting size of room
 * @return selected size of room or 'BACK_TO_MAIN_MENU' define
 */
char select_size();

/**
 * highlights selected answer
 * @param a_window window which should be highlighted
 * @param a_text text written inside 'a_window'
 * @param highlight_type type of highlightion
 * @brief highlight_type could be 1 of 3 definitions
 * (HIGHLIGHT_RED || HIGHLIGHT_BLUE || HIGHLIGHT_GREE)
 * other cases is not supported and behaviour is unpredictable
 */
void highlight_selected(WINDOW* a_window,
                        char a_text[A_LEN],
                        int highlight_type);

/**
 * main loop of playing the game
 * @param answer array of window where should be inserted answers
 * @param system_info window where system information is shown
 * @param question_window window where question is shown
 * @param r_info information about current room
 * @return CODE_FAILURE || GAME_OVER
 */
int game_loop(WINDOW *answer[4],
              WINDOW *system_info,
              WINDOW *question_window,
              struct room_info r_info);

/**
 * shows main menu where user can select create/join/exit game
 * @param nickname nickname of user
 * @return selected size of room || ROOM_JOIN || APPLICATION_EXIT
 */
char mainMenu(const char nickname[52]);

/**
 * updates values for next step of iteration
 * @param desiredSize value to be dropped
 */
void refresh_values(char *desiredSize);

/**
 * shows error window with choices RETRY + EXIT | EXIT
 * @param error_type type of error, caused this windown to be shown
 * @param is_retryable is RETRY button available
 * @return HANDLE_RETRY || HANDLE_STOP, depending on which choice was selected
 */
int error_window(int error_type, bool is_retryable);

/**
 * cycle of waiting for client
 * @param answer
 * @param system_info
 * @param question_window
 */
void wait_players(WINDOW *answer[4],
                  WINDOW *system_info,
                  WINDOW *question_window);

#endif //__CLIENT_UI_H
