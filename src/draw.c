#include <ncurses.h>
#include "draw.h"
#include "game.h"
#include "piece.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char* LOGO[] = {
    " ---  |---   ---  |--\\  |  --- ",
    "  |   |--     |   |__/  |  \\__ ",
    "  |   |---    |   |  \\  |  ___/"
};

void init_graphics() {
    initscr();
    noecho();
    curs_set(0);
    timeout(100);
    keypad(stdscr, TRUE);
    start_color();

    init_color(10, 1000, 843, 0);
    init_color(11, 752, 752, 752);
    init_color(12, 803, 498, 196);

    init_pair(1, COLOR_CYAN, COLOR_BLACK);    
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); 
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK); 
    init_pair(4, COLOR_GREEN, COLOR_BLACK);  
    init_pair(5, COLOR_RED, COLOR_BLACK);   
    init_pair(6, COLOR_BLUE, COLOR_BLACK);   
    init_pair(7, COLOR_WHITE, COLOR_BLACK);  
    init_pair(8, COLOR_WHITE, COLOR_BLACK);  
    init_pair(9, 10, COLOR_BLACK);
    init_pair(10, 11, COLOR_BLACK);
    init_pair(11, 12, COLOR_BLACK);
}

void draw_main_menu(int selected, int highscore, int has_save) {
    int start_y = 10;
    int start_x = 15;
    
    clear();
    
    attron(COLOR_PAIR(3));
    for (int i = 0; i < 3; i++) {
        mvprintw(start_y - 6 + i, start_x - 5, "%s", LOGO[i]);
    }
    attroff(COLOR_PAIR(3));

    int current_y = start_y;

    if (selected == 0) {
        attron(COLOR_PAIR(2));
        mvprintw(current_y, start_x - 5, " --> NEW GAME");
        attroff(COLOR_PAIR(2));
    } else {
        mvprintw(current_y, start_x, "NEW GAME");
    }
    current_y += 2;

    if (has_save) {
        if (selected == 1) {
            attron(COLOR_PAIR(2));
            mvprintw(current_y, start_x - 5, " --> CONTINUE");
            attroff(COLOR_PAIR(2));
        } else {
            mvprintw(current_y, start_x, "CONTINUE");
        }
        current_y += 2;
    }

    if (selected == 2) {
        attron(COLOR_PAIR(2));
        mvprintw(current_y, start_x - 5, " --> LEADERBOARD");
        attroff(COLOR_PAIR(2));
    } else {
        mvprintw(current_y, start_x, "LEADERBOARD");
    }
    current_y += 2;

    if (selected == 3) {
        attron(COLOR_PAIR(2));
        mvprintw(current_y, start_x - 5, " --> QUIT");
        attroff(COLOR_PAIR(2));
    } else {
        mvprintw(current_y, start_x, "QUIT");
    }

    mvprintw(current_y + 3, start_x, "ALL-TIME HIGH: %d", highscore);

    refresh();
}

void draw_static_ui() {
    clear();
    int board_x = 10;
    int board_y = 5;

    attron(COLOR_PAIR(1));
    for (int i = 0; i < 3; i++) {
        mvprintw(i + 1, board_x, "%s", LOGO[i]);
    }
    attroff(COLOR_PAIR(1));

    for (int r = 0; r <= ROWS; r++) {
        mvaddch(r + board_y, board_x - 1, '|');
        mvaddch(r + board_y, board_x + (COLS * 2), '|');
    }
    for (int c = 0; c < COLS * 2; c++) {
        mvaddch(ROWS + board_y, c + board_x, '-');
    }

    int next_x = 35;
    mvprintw(board_y, next_x, "NEXT PIECE");
    mvprintw(board_y + 1, next_x - 1, "+----------+");
    for (int i = 0; i < 4; i++) {
        mvprintw(board_y + 2 + i, next_x - 1, "|          |");
    }
    mvprintw(board_y + 6, next_x - 1, "+----------+");

    mvprintw(board_y + 8,  next_x, "SCORE    :");
    mvprintw(board_y + 9,  next_x, "HIGHSCORE:");
    mvprintw(board_y + 10, next_x, "LEVEL    :");

    mvprintw(board_y + 12, next_x, "PRESS Q TO QUIT");
    refresh();
}

void update_ui_values(int score, int highscore, int level) {
    mvprintw(13, 46, "%d", score); 
    mvprintw(14, 46, "%d", highscore);
    mvprintw(15, 46, "%d", level);
}

void draw_board_data(int b[ROWS][COLS]) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            int py = r + 5;
            int px = (c * 2) + 10;
            if (b[r][c] != 0) {
                attron(COLOR_PAIR(b[r][c]));
                mvprintw(py, px, "[]");
                attroff(COLOR_PAIR(b[r][c]));
            } else {
                mvprintw(py, px, " .");
            }
        }
    }
}

void draw_ghost_piece(Tetromino p, int b[ROWS][COLS]) {
    Tetromino ghost = p;
    while (!check_collision(ghost, b)) {
        ghost.y++;
    }
    ghost.y--;

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (ghost.shape[r][c] != 0) {
                mvprintw(ghost.y + r + 5, (ghost.x + c) * 2 + 10, "::");
            }
        }
    }
}

void draw_moving_piece(Tetromino p) {
    attron(COLOR_PAIR(p.color));
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (p.shape[r][c] != 0) {
                mvprintw(p.y + r + 5, (p.x + c) * 2 + 10, "[]");
            }
        }
    }
    attroff(COLOR_PAIR(p.color));
}

void draw_next_piece_preview(Tetromino p) {
    int box_x = 35; 
    int box_y = 7;

    for (int r = 0; r < 4; r++) {
        mvprintw(box_y + r, box_x, "        "); 
    }

    attron(COLOR_PAIR(p.color));
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (p.shape[r][c] != 0) {
                mvprintw(box_y + r, box_x + (c * 2), "[]");
            }
        }
    }
    attroff(COLOR_PAIR(p.color));
}

void draw_game_over_screen(int score) {
    int mid_y = 12;
    int mid_x = 12;
    
    attron(COLOR_PAIR(5));
    mvprintw(mid_y, mid_x, "  G A M E   O V E R  ");
    attroff(COLOR_PAIR(5));
    
    mvprintw(mid_y + 2, mid_x, " FINAL SCORE: %d", score);
    mvprintw(mid_y + 4, mid_x, " PRESS ANY KEY TO CONTINUE ");
}

void game_quit_screen(int score){
    int mid_y = 12;
    int mid_x = 12;

    mvprintw(mid_y + 2, mid_x, " FINAL SCORE: %d", score);
    mvprintw(mid_y + 4, mid_x, " ARE YOU SURE YOU WANT TO QUIT? ");
    mvprintw(mid_y + 5, mid_x, " PRESS S TO CONTINUE THE GAME ");
    mvprintw(mid_y + 6, mid_x, " PRESS Q TO QUIT ");
}


char* draw_namescreen() {
    int mid_y = 12;
    int mid_x = 12;

    static char username[50]; 
    char names[100][50];
    int scores[100];
    int count = 0;
    int found = 0;

    clear(); 
    mvprintw(mid_y + 2, mid_x, " ENTER YOUR USERNAME: ");
    refresh(); 

    echo();
    curs_set(1);
    timeout(-1); 
    flushinp(); 
    getnstr(username, 49); 
    timeout(100); 
    noecho();
    curs_set(0);


    FILE *f = fopen("list.csv", "r");
    if (f) {
        char line[100];
        while (count < 100 && fgets(line, sizeof(line), f)) {
            char *token = strtok(line, ",");
            if (token) {
                strcpy(names[count], token);
                token = strtok(NULL, ",");
                if (token) {
                    scores[count] = atoi(token);
                }
                
                if (strcmp(names[count], username) == 0) {
                    found = 1;
                }
                count++;
            }
        }
        fclose(f);
    }

    if (!found && count < 100) {
        strcpy(names[count], username);
        scores[count] = 0;
        count++;
    }

    f = fopen("list.csv", "w");
    if (f) {
        for (int i = 0; i < count; i++) {
            fprintf(f, "%s,%d\n", names[i], scores[i]);
        }
        fclose(f);
    }

    return username;
}

void draw_leaderboard(){
    int mid_y = 8;
    int mid_x = 12;

    char names[100][50];
    int scores[100];
    int count = 0;
    FILE *f = fopen("list.csv", "r");
    if (f) {
        char line[100];
        while (count < 100 && fgets(line, sizeof(line), f)) {
            char *token = strtok(line, ",");
            if (token) {
                strcpy(names[count], token);
                token = strtok(NULL, ",");
                if (token) {
                    scores[count] = atoi(token);
                }
                count++;
            }
        }
        fclose(f);
    }

    for(int i = 0;i < count;i++){
        int max = 0;
        int maxindex = i;
        for(int j = i;j < count;j++){
            if(scores[j] >= max){
                max = scores[j];
                maxindex = j;
            }
        }
        char tempname[100];
        int tempscore = scores[i];
        strcpy(tempname,names[i]);
        strcpy(names[i],names[maxindex]);
        scores[i] = scores[maxindex];
        scores[maxindex] = tempscore;
        strcpy(names[maxindex],tempname);
    }

    mvprintw(mid_y - 7, mid_x, "+-------------+");
    for (int i = 0; i < 1; i++) {
        mvprintw(mid_y - 6 + i, mid_x, "|             |");
    }
    mvprintw(mid_y - 5, mid_x, "+-------------+");

    mvprintw(mid_y - 6, mid_x+2, "LEADERBOARD");

    mvprintw(mid_y - 3, mid_x-2, " Press Q to go to Main Menu ");

    mvprintw(mid_y, mid_x - 8, " NAMES: ");
    mvprintw(mid_y + 1, mid_x - 8, " ------");
    mvprintw(mid_y, mid_x + 18, " SCORES: ");
    mvprintw(mid_y + 1, mid_x + 18, " ------");

    for(int i = 0;i < count;i++){
        char tempstring[10];
        if(i<=2){
            attron(COLOR_PAIR(i+9));
        }
        sprintf(tempstring,"%d",scores[i]);
        mvprintw(mid_y + 2 + i, mid_x - 7, names[i]);
        mvprintw(mid_y + 2 + i, mid_x+20, tempstring);
        if(i<=2){
            attroff(COLOR_PAIR(i+9));
        }
    }
}

void close_graphics() {
    endwin();
}