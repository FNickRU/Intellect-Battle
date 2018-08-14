#include "client_ui.h"

int main()
{
    char nickname[52];
    int  errCode;
    initscr();
    keypad(stdscr, 1);
    cbreak();
    start_color();
    get_nickname(nickname);
    init(nickname);

    mousemask(KEY_MOUSE, NULL);
    //------------------------MAIN_MENU_WINDOW INITIALIZATION------------------
    WINDOW *create_button = newwin(MENU_CREATE_H, MENU_CREATE_W, SPACER,
                                   (TERMINAL_WIDTH - MENU_CREATE_W) / 2);
    WINDOW *join_button = newwin(MENU_JOIN_H, MENU_JOIN_W,
                                 MENU_CREATE_H + SPACER * 2,
                                 (TERMINAL_WIDTH - MENU_JOIN_W) / 2);
    WINDOW *exit_button = newwin(MENU_EXIT_H, MENU_EXIT_W,
                                 MENU_CREATE_H + MENU_JOIN_H + SPACER * 3,
                                 (TERMINAL_WIDTH - MENU_EXIT_W)/ 2);
    //------------------------MAIN_MENU_WINDOW INITIALIZATION------------------

    //------------------------PLAY_MENU WINDOW INITIALIZATION------------------
    WINDOW *server_status = newwin(1, TERMINAL_WIDTH, QUESTION_HEIGHT, 0);
    WINDOW *question_window = newwin(QUESTION_HEIGHT, QUESTION_WIDTH, 0,
                                     (TERMINAL_WIDTH - QUESTION_WIDTH) / 2);
    WINDOW *answer[4];
    for (int i = 0; i < 4; i++) {
        answer[i] = newwin(ANSWER_HEIGHT, ANSWER_WIDTH,
                           QUESTION_HEIGHT + SPACER + ANSWER_HEIGHT * i, 0);
    }
    refresh();
    //------------------------PLAY_MENU WINDOW INITIALIZATION------------------
    curs_set(false);
    int error_handle = HANDLE_OK;
    errCode = connect_to_server();
    while (errCode == CODE_FAILURE) {
        error_handle = error_window(ERR_CONN_FAIL, true);
        switch (error_handle){
            case HANDLE_RETRY:
                errCode = connect_to_server();
                break;
            case HANDLE_STOP:
                endwin();
                finalize();
                exit(APPLICATION_ERROR);
            default:
                break;
        }
    }
    //Main menu
    char type, roomSize;
    char behaviour = BACK_TO_MAIN_MENU;
    while(behaviour != APPLICATION_EXIT) {
        behaviour = get_behaviour(nickname, create_button, join_button,
                                  exit_button, behaviour);
        if (behaviour == APPLICATION_EXIT){
            break;
        } else {
            if (behaviour == 1) {
                type = REQ_JOIN;
                roomSize = MAX_ROOM_SIZE;
            } else {
                type = REQ_CREATE;
                roomSize = behaviour;
            }
        }
        //------------------------------------
        errCode = send_conf(type, roomSize);
        while (errCode == CODE_FAILURE) {
            error_handle = error_window(ERR_CONF_SEND_FAIL, true);
            switch (error_handle) {
                case HANDLE_RETRY:
                    errCode = send_conf(type, roomSize);
                    break;
                case HANDLE_STOP:
                    endwin();
                    finalize();
                    exit(APPLICATION_ERROR);
                default:
                    break;
            }
        }
        //------------------------------------
        redraw_game_window(server_status, question_window, answer);
        wait_players(answer, server_status, question_window);
        refresh_values(&behaviour);
    }
    endwin();
}

void redraw_game_window(WINDOW *server_status, WINDOW *question_window,
                        WINDOW *answer[4])
{
    clear();
    refresh();
    mvwprintw(server_status, 0, 0, "SERVER IS [OK]");
    box(question_window, 0, 0);
    for (int i = 0; i < 4; i++) {
        box(answer[i], 0, 0);
        wrefresh(answer[i]);
    }
    wrefresh(question_window);
    wrefresh(server_status);
}

char get_behaviour(const char *nickname, WINDOW *create_button,
                   WINDOW *join_button, WINDOW *exit_button,
                   char selectedBehaviour)
{
    while (selectedBehaviour == BACK_TO_MAIN_MENU) {
        main_menu_redraw(create_button, join_button, exit_button);
        selectedBehaviour = mainMenu(nickname);
    }
    return selectedBehaviour;
}

void get_nickname(char *nickname)
{
    WINDOW *info = newwin(1, 50, 1, 0);
    WINDOW *get_username = newwin(3, 50, 2, 0);
    WINDOW *input_field = newwin(1, 48, 3, 1);
    mvwprintw(info, 0, 0, "Enter your nickname: ");
    wrefresh(info);
    box(get_username, 0, 0);
    wrefresh(get_username);
    nickname[0] = 0;
    while (nickname[0] == 0) {
        wscanw(input_field, "%s", nickname);
    }
}

void main_menu_redraw(WINDOW *create_button, WINDOW *join_button,
                      WINDOW *exit_button)
{
    clear();
    box(create_button, 0, 0);
    box(join_button, 0, 0);
    box(exit_button, 0, 0);
    mvwprintw(create_button, 2,
              (MENU_CREATE_W / 2) - strlen("create game") / 2, "CREATE GAME");
    mvwprintw(join_button, 2,
              (MENU_JOIN_W / 2) - strlen("join game") / 2, "JOIN GAME");
    mvwprintw(exit_button, 2,
              (MENU_EXIT_W / 2) - strlen("exit game") / 2, "EXIT GAME");
    refresh();
    wrefresh(create_button);
    wrefresh(join_button);
    wrefresh(exit_button);
}

void *progress_show(void *arg)
{
    init_pair(10, COLOR_WHITE, COLOR_WHITE);
    init_pair(11, COLOR_BLUE, COLOR_BLUE);
    init_pair(12, COLOR_RED, COLOR_RED);

    WINDOW *wait = newwin(PROGRESS_WINDOW_BORDER_H, PROGRESS_WINDOW_BORDER_W, PROGRESS_WINDOW_BORDER_OFFT_Y, PROGRESS_WINDOW_BORDER_OFFT_X);
    WINDOW *progress = derwin(wait, PROGRESS_WINDOW_BORDER_H - 2, PROGRESS_WINDOW_BORDER_W - 2, 1, 1);
    wclear(wait);
    wclear(progress);
    wrefresh(wait);
    wrefresh(progress);
    box(progress, 0, 0);
    box(wait, 0, 0);
    int i = 0;
    int interationNum = 0;
    wattron(progress, COLOR_PAIR(10));
    while(1) {
        waddch(progress, '#');
        if (i > 48 * (3 - interationNum)) {
            wattroff(progress, COLOR_PAIR(10 + interationNum));
            interationNum++;
            if (interationNum >= 3) {
                interationNum = 0;
            }
            wmove(progress, interationNum, 0);
            wattron(progress, COLOR_PAIR(10 + interationNum));
            i = 0;
            sleep(1);
        } else {
            i++;
        }
        wrefresh(progress);
        wrefresh(wait);
    }
}

void wait_players(WINDOW *answer[4], WINDOW *system_info,
                  WINDOW *question_window)
{
    int errCode = WAIT_MORE;
    int error_handle;
    //-----------------
    struct room_info r_info;
    //-----------------
    WINDOW *connectedStatus = newwin(1, 50, 6, 38);
    wclear(connectedStatus);

    //set queue as 1/4 because no info
    wprintw(connectedStatus, "1 / 4");
    wrefresh(connectedStatus);

    pthread_t drawer;
    pthread_create(&drawer, 0, &progress_show, NULL);

    while(errCode == WAIT_MORE) {
        //-----------------
        errCode = wait_for_players(&r_info);
        while (errCode == CODE_FAILURE) {
            error_handle = error_window(ERR_WAIT_FAIL, true);
            switch (error_handle) {
                case HANDLE_RETRY:
                    errCode = wait_for_players(&r_info);
                    break;
                case HANDLE_STOP:
                    pthread_cancel(drawer);
                    pthread_join(drawer, NULL);
                    endwin();
                    finalize();
                    exit(APPLICATION_ERROR);
                default:
                    break;
            }
        }
        wclear(connectedStatus);
        wprintw(connectedStatus, "%i / %i", r_info.occupancy + 1,
                r_info.room_size);
        wrefresh(connectedStatus);
        //-----------------
    }
    pthread_cancel(drawer);
    pthread_join(drawer, NULL);
    clear();
    refresh();
    errCode = game_loop(answer, system_info, question_window, r_info);

    if (errCode == CODE_FAILURE) {
        error_handle = error_window(ERR_CONN_FAIL, false);
        switch (error_handle) {
            case HANDLE_RETRY:
                break;
            case HANDLE_STOP:
                endwin();
                finalize();
                exit(APPLICATION_ERROR);
            default:
                break;
        }
    }
}

char select_size()
{
    int val;
    MEVENT event;
    clear();
    WINDOW *choices[3];
    mvwprintw(stdscr, 0, (TERMINAL_WIDTH / 2) - strlen("SELECT SIZE OF THE ROOM") / 2,
              "SELECT SIZE OF THE ROOM");
    wrefresh(stdscr);
    refresh();
    for (int i = 0; i < 3; i++) {
        choices[i] = newwin(SELECT_BOX_H,
                            SELECT_BOX_W, SPACER + SELECT_BOX_H * i,
                            (TERMINAL_WIDTH - SELECT_BOX_W) / 2);
        box(choices[i], 0, 0);
        mvwprintw(choices[i], 2, 5, "%i", i + 2);
        wrefresh(choices[i]);
    }
    WINDOW *back_button = newwin(BACK_BOX_H, BACK_BOX_W,
                                 SPACER + SELECT_BOX_H * 3,
                                 (TERMINAL_WIDTH - BACK_BOX_W) / 2);
    box(back_button, 0, 0);
    mvwprintw(back_button, 2, 8, "BACK");
    wrefresh(back_button);
    while (1) {
        val = getch();
        if (val == KEY_MOUSE) {
            getmouse(&event);
            if (event.x >= (TERMINAL_WIDTH - SELECT_BOX_W) / 2 &&
                event.x < (TERMINAL_WIDTH - SELECT_BOX_W) / 2 + SELECT_BOX_W) {
                if (event.y >= SPACER) {
                    int winId = (event.y - SPACER) / SELECT_BOX_H;
                    switch (winId) {
                        case 0:
                            delete_wins(choices[0], choices[1], choices[2]);
                            delwin(back_button);
                            return ROOM_FOR_TWO;
                        case 1:
                            delete_wins(choices[0], choices[1], choices[2]);
                            delwin(back_button);
                            return ROOM_FOR_THREE;
                        case 2:
                            delete_wins(choices[0], choices[1], choices[2]);
                            delwin(back_button);
                            return ROOM_FOR_FOUR;
                    }
                }
            }
            if (event.x >= (TERMINAL_WIDTH - BACK_BOX_W) / 2 &&
                event.x < (TERMINAL_WIDTH - BACK_BOX_W) / 2 + BACK_BOX_W) {
                /**
                 * SELECT_BOX_H - height of each box of room-size choices
                 * current condition detects all clicks under choices, but not higher than SPACER + BACK_BOX_H
                 */
                if (event.y >= SELECT_BOX_H * 3 + SPACER &&
                    event.y < SELECT_BOX_H * 3 + SPACER + BACK_BOX_H) {
                    delete_wins(choices[0], choices[1], choices[2]);
                    delwin(back_button);
                    return BACK_TO_MAIN_MENU;
                }
            }

        }
    }
}

void delete_wins(WINDOW *win1, WINDOW *win2, WINDOW *win3)
{
    delwin(win1);
    delwin(win2);
    delwin(win3);
}

void highlight_selected(WINDOW* a_window, char a_text[A_LEN],
                        int highlight_type)
{
    WINDOW* inner_text = derwin(a_window, ANSWER_HEIGHT - 2, ANSWER_WIDTH - 2,
                                1, 1);
    switch (highlight_type) {
        case HIGHLIGHT_BLUE:
            init_pair(FREE_COLOR_PAIR, COLOR_BLACK, COLOR_BLUE);
        case HIGHLIGHT_GREEN:
            init_pair(FREE_COLOR_PAIR, COLOR_BLACK, COLOR_GREEN);
        case HIGHLIGHT_RED:
            init_pair(FREE_COLOR_PAIR, COLOR_BLACK, COLOR_RED);
        default:
            init_pair(FREE_COLOR_PAIR, COLOR_BLACK, COLOR_BLUE);
    }
    wattron(inner_text, COLOR_PAIR(FREE_COLOR_PAIR));
    wprintw(inner_text, a_text);
    for (uint i = 0; i < (ANSWER_WIDTH * ANSWER_HEIGHT) - strlen(a_text); i++) {
        waddch(inner_text, ' ');
    }
    wattroff(inner_text, COLOR_PAIR(FREE_COLOR_PAIR));
    wrefresh(inner_text);
}

int game_loop(WINDOW *answer[4], WINDOW *system_info,
              WINDOW *question_window, struct room_info r_info)
{
    MEVENT event;
    int status;
    int val;
    WINDOW *nicks[4];
    WINDOW *bwindow = NULL;

    redraw_game_window(system_info, question_window, answer);
    mvwprintw(system_info, USERS_TXT_OFFT_Y, USERS_TXT_OFFT_X, "Users:");
    wrefresh(system_info);
    print_nickname(&r_info, nicks, bwindow);
    struct unit question;

    status = get_unit(&question, &r_info);
    if (status == CODE_FAILURE) {
        return CODE_FAILURE;
    }
    struct timeval time;

    WINDOW *innertext = derwin(question_window,
                               QUESTION_HEIGHT - 2, QUESTION_WIDTH - 2, 1, 1);
    wprintw(innertext, question.quest);
    wrefresh(innertext);
    for (int i = 0; i < 4; i++) {
        innertext = derwin(answer[i], ANSWER_HEIGHT - 2, ANSWER_WIDTH - 2,
                           1, 1);
        wprintw(innertext, question.ans[i]);
        wrefresh(innertext);
    }

    while (status == GAME_CONT) {
        val = getch();
        if (val == KEY_MOUSE) {
            getmouse(&event);
            if (event.x < ANSWER_WIDTH && event.y > QUESTION_HEIGHT + SPACER) {
                char winId = (char)((event.y - (QUESTION_HEIGHT + SPACER)) / 4);
                highlight_selected(answer[winId], question.ans[winId],
                                   HIGHLIGHT_BLUE);
                gettimeofday(&time, NULL);
                status = send_ans(winId, time);
                if (status != CODE_SUCCESS) {
                    break;
                }
                status = get_unit(&question, &r_info);
                if (status != CODE_SUCCESS) {
                    break;
                }
                if (is_loser(r_info) == GAME_OVER) {
                    highlight_selected(answer[winId], question.ans[winId],
                                       HIGHLIGHT_RED);
                    sleep(1);
                    error_window(GAME_LOST, true);
                    status = GAME_OVER;
                    break;
                } else {
                    highlight_selected(answer[winId], question.ans[winId],
                                       HIGHLIGHT_GREEN);
                    sleep(1);
                }
            }
        }
    }
    return status;
}

char mainMenu(const char nickname[52])
{
    MEVENT event;
    mvwprintw(stdscr, 0, 0, "Logged as %s", nickname);
    int val;
    while(1) {
        val = getch();
        if (val == KEY_MOUSE) {
            getmouse(&event);
            if (event.x > (TERMINAL_WIDTH - MENU_CREATE_W) / 2
                && event.x < (TERMINAL_WIDTH - MENU_CREATE_W) / 2 + MENU_CREATE_W) {
                if (event.y >= SPACER) {
                    int winId = (event.y - SPACER) / (MENU_CREATE_H + SPACER);
                    int clickedPoint = (event.y - SPACER) % (MENU_CREATE_H + SPACER);
                    if (clickedPoint >= MENU_JOIN_H) continue;
                    switch (winId) {
                        case 0://SELECTED CREATE
                            return select_size();
                        case 1://SELECTED JOIN
                            return ROOM_JOIN;
                        case 2://SELECTED EXIT
                            return APPLICATION_EXIT;
                        default:
                            return APPLICATION_EXIT;
                    }
                }
            }
        }
    }
}

void refresh_values(char *desiredSize)
{
    *desiredSize = BACK_TO_MAIN_MENU;
}

void print_nickname(struct room_info *r_info, WINDOW *nicks[4], WINDOW *bwindow)
{
    for (int i = 0; i < r_info->occupancy; i++) {
        bwindow = newwin(NICKNAME_WIN_H,
                         NICKNAME_WIN_W,
                         NICKNAME_SPACER - NICKNAME_WIN_H * i,
                         TERMINAL_WIDTH - NICKNAME_WIN_W);
        nicks[i] = derwin(bwindow,
                          NICKNAME_WIN_H - 2,
                          NICKNAME_WIN_W - 2,
                          1,
                          1);
        wprintw(nicks[i], "%s", (*r_info).usernames[i]);
        box(bwindow, 0, 0);
        wrefresh(bwindow);
    }
    for (int i = r_info->occupancy; i < MAX_ROOM_SIZE; i++) {
        bwindow = newwin(NICKNAME_WIN_H,
                         NICKNAME_WIN_W,
                         NICKNAME_SPACER - NICKNAME_WIN_H * i,
                         TERMINAL_WIDTH - NICKNAME_WIN_W);
        nicks[i] = derwin(bwindow,
                          NICKNAME_WIN_H - 2,
                          NICKNAME_WIN_W - 2,
                          1,
                          1);
        wprintw(nicks[i], "NO_PLAYER");
        box(bwindow, 0, 0);
        wrefresh(bwindow);
    }
}

int error_window(int error_type, bool is_retryable)
{
    clear();
    refresh();
    WINDOW *err_win = newwin(ERR_BOX_H, ERR_BOX_W, ERR_BOX_OFFT_Y, ERR_BOX_OFFT_X);
    WINDOW *err_txt = derwin(err_win, ERR_BOX_H - 2, ERR_BOX_W - 2, 1, 1);
    WINDOW *retry = derwin(err_win, RETRY_BOX_H, RETRY_BOX_W, REL_RETRY_BOX_OFFT_Y, REL_RETRY_BOX_OFFT_X);
    WINDOW *stop  = derwin(err_win, EXIT_BOX_H, EXIT_BOX_W, REL_EXIT_BOX_OFFT_Y, REL_EXIT_BOX_OFFT_X);
    box(stop, 0, 0);
    box(err_win, 0, 0);
    if (is_retryable) {
        box(retry, 0, 0);
        mvwprintw(retry, 1, 1, "RETRY");
    }
    mvwprintw(stop, 1, 1, "EXIT");
    switch (error_type) {
        case ERR_CONN_FAIL:
            mvwprintw(err_txt, 0, 0,
                      "ERROR OCCURED DURING CONNECTION (%s)", strerror(errno));
            break;
        case ERR_CONF_SEND_FAIL:
            mvwprintw(err_txt, 0, 0,
                    "ERROR OCCURED DURING SENDING CONFIGURATION TO SERVER (%s)",
                    strerror(errno));
            break;
        case ERR_WAIT_FAIL:
            mvwprintw(err_txt, 0, 0,
                "ERROR OCCURED DURING WAITING FOR PLAYERS (%s)",
                strerror(errno));
            break;
        case GAME_LOST:
            mvwprintw(err_txt, 0, 0,
                      "Oh no, you lost!",
                      strerror(errno));
            break;
        default:
            return HANDLE_STOP;
    }
    MEVENT event;
    int val;
    wrefresh(err_win);
    wrefresh(retry);
    while(1) {
        val = getch();
        if (val == KEY_MOUSE) {
            getmouse(&event);
            if (is_retryable && event.x >= ERR_BOX_OFFT_X + REL_RETRY_BOX_OFFT_X
                && event.x < ERR_BOX_OFFT_X + REL_RETRY_BOX_OFFT_X + RETRY_BOX_W) {
                if (event.y >= ERR_BOX_OFFT_Y + REL_RETRY_BOX_OFFT_Y && event.y < ERR_BOX_OFFT_Y + REL_RETRY_BOX_OFFT_Y + RETRY_BOX_H) {
                    clear();
                    return HANDLE_RETRY;
                }
            } else {
                if (event.x >= ERR_BOX_OFFT_X + REL_EXIT_BOX_OFFT_X
                    && event.x < ERR_BOX_OFFT_X + REL_EXIT_BOX_OFFT_X + EXIT_BOX_W) {
                    if (event.y >= ERR_BOX_OFFT_Y + REL_RETRY_BOX_OFFT_Y && event.y < ERR_BOX_OFFT_Y + REL_RETRY_BOX_OFFT_Y + EXIT_BOX_H) {
                        return HANDLE_STOP;
                    }
                }
            }
        }
    }
}
