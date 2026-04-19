#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "piece.h"
#include "draw.h"

typedef enum {
    STATE_MENU,
    STATE_LEADERBOARD,
    STATE_PLAYING,
    STATE_GAMEOVER,
    STATE_GAMEQUIT,
    STATE_EXIT
} GameState;

// void save_highscore(int score, int highscore) {
//     FILE *write = fopen("highscore.txt", "w");
//     if (write == NULL) return; 

//     if (score > highscore) {
//         fprintf(write, "%d", score);
//     } else {
//         fprintf(write, "%d", highscore);
//     }
//     fclose(write);
// }

int main() {
    char* user;

    int board[ROWS][COLS];
    int score = 0;
    int highscore = 0;
    int level = 1;
    int lines_cleared_total = 0;

    double last_fall_time_ms = 0;
    double fall_delay = 1.0;

    // FILE *read = fopen("highscore.txt", "r");
    // if(read == NULL){
    //     highscore = 0;
    // }else{
    //     fscanf(read, "%d", &highscore);
    //     fclose(read);
    // }

    init_graphics();
    srand(time(NULL)); 

    Tetromino current;
    Tetromino next;

    user = draw_namescreen();

    int count = 0;
    int found = 0;
    FILE *f = fopen("list.csv", "r");
    if (f) {
        char line[100];
        while (count < 100 && fgets(line, sizeof(line), f)) {
            char *token = strtok(line, ",");
            if (token) {
                if(strcmp(token,user)==0){
                    found = 1;
                    token = strtok(NULL, ",");
                    if (token) {
                        highscore = atoi(token);
                    }
                }
                count++;
            }
        }
        if(!found)highscore = 0;
        fclose(f);
    }

    GameState current_state = STATE_MENU;
    clear();

    int has_save = 0;
    char* datafilename;
    sprintf(datafilename,"%sdata.txt",user);
    FILE *test = fopen(datafilename, "r");
    if (test) {
        has_save = 1;
        fclose(test);
    }
    int currid,nextid;

    int menu_selected = 0;

    load_game_state(board,&score,&level,&fall_delay,&currid,&nextid, user);
    while (current_state != STATE_EXIT) {
        if (current_state == STATE_MENU) {
            draw_main_menu(menu_selected, highscore, has_save);
            int menu_ch = getch();
            if (menu_ch == KEY_UP) {
                menu_selected--;
                if (menu_selected == 1 && !has_save) menu_selected--;
                if (menu_selected < 0) menu_selected = 3;
            } 
            else if (menu_ch == KEY_DOWN) {
                menu_selected++;
                if (menu_selected == 1 && !has_save) menu_selected++;
                if (menu_selected > 3) menu_selected = 0;
            }
            if(menu_selected == 1 && menu_ch == '\n'){
                current = set_piece(currid);

                next = set_piece(nextid);
                
                struct timespec ts;
                clock_gettime(CLOCK_MONOTONIC, &ts);
                last_fall_time_ms = (ts.tv_sec * 1000.0) + (ts.tv_nsec / 1000000.0);
                current_state = STATE_PLAYING;
                draw_static_ui();
            }else if (menu_selected == 0 && menu_ch == '\n') {
                init_board(board);
                score = 0;
                level = 1;
                lines_cleared_total = 0;
                fall_delay = 1.0;
                
                current = generate_random_piece();
                currid = current.color-1;
                next = generate_random_piece();
                nextid = next.color-1;
                
                struct timespec ts;
                clock_gettime(CLOCK_MONOTONIC, &ts);
                last_fall_time_ms = (ts.tv_sec * 1000.0) + (ts.tv_nsec / 1000000.0);
                
                draw_static_ui(); 
                current_state = STATE_PLAYING;
            } else if (menu_selected == 3 && menu_ch == '\n') {
                if(has_save)save_highscore(user, score, highscore);
                printf("EXited the game");
                current_state = STATE_EXIT;
            } else if (menu_selected == 2 && menu_ch == '\n') {
                clear();
                draw_leaderboard();
                current_state = STATE_LEADERBOARD;
            }
        }
        else if (current_state == STATE_LEADERBOARD) {
            int ch = getch();

            if (ch == 'Q' || ch == 'q') {
                clear();
                current_state = STATE_MENU;
            }
        }
        else if (current_state == STATE_PLAYING) {
            int ch = getch();

            if (ch == KEY_UP) {
                rotate_piece(&current, board);
            } else if (ch == KEY_LEFT) {
                current.x--;
                if (check_collision(current, board)) current.x++;
            } else if (ch == KEY_RIGHT) {
                current.x++;
                if (check_collision(current, board)) current.x--;
            } else if (ch == KEY_DOWN) {
                current.y++;
                if (check_collision(current, board)) current.y--;
            } else if (ch == 'q' || ch == 'Q') {
                save_highscore(user, score, highscore);
                current_state = STATE_GAMEQUIT;
            }

            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC, &ts);
            double current_time_ms = (ts.tv_sec * 1000.0) + (ts.tv_nsec / 1000000.0);

            if ((current_time_ms - last_fall_time_ms) >= (fall_delay * 1000.0)) {
                current.y++;
                
                if (check_collision(current, board)) {
                    current.y--; 
                    lock_piece(current, board);
                    
                    int lines = clear_full_lines(board);
                    if (lines > 0) {
                        score += (lines * 100) * level;
                        lines_cleared_total += lines;
                        
                        if (lines_cleared_total >= 1) {
                            level++;
                            lines_cleared_total = 0;
                            fall_delay *= 0.85;
                            if (fall_delay < 0.1) fall_delay = 0.1; 
                        }
                    } else {
                        score += 20;
                    }

                    current = next;
                    currid = current.color-1;
                    next = generate_random_piece();
                    nextid = next.color-1;
                    
                    if (check_collision(current, board)) {
                        save_highscore(user, score, highscore);
                        has_save = 0;
                        clear_save(user);
                        current_state = STATE_GAMEOVER;
                    }
                }
                last_fall_time_ms = current_time_ms;
            }

            draw_board_data(board);
            draw_ghost_piece(current, board);
            draw_moving_piece(current);
            draw_next_piece_preview(next);
            update_ui_values(score, highscore, level);
            
            refresh();
        }

        else if (current_state == STATE_GAMEOVER) {
            clear();
            draw_game_over_screen(score);
            int go_ch = getch();
            if (go_ch != -1) { 
                clear();
                current_state = STATE_MENU;
            }
        }

        else if (current_state == STATE_GAMEQUIT) {
            clear();
            game_quit_screen(score);
            save_game_state(board, score, level, fall_delay, currid, nextid, user);
            save_highscore(user, score, highscore);
            int go_ch = getch();
            if (go_ch == 's' || go_ch == 'S') {
                clear();
                draw_static_ui();
                current_state = STATE_PLAYING;
            }else if(go_ch == 'q' || go_ch == 'Q'){
                current_state = STATE_EXIT;
            }
        }
    }

    close_graphics();

    printf("\nTETRIS: Final Score: %d\n", score);
    
    return 0;
}