#include "game.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void init_board(int board[ROWS][COLS]) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            board[r][c] = 0;
        }
    }
}

int check_collision(Tetromino p,int board[ROWS][COLS]) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (p.shape[r][c] != 0) {
                int board_x = p.x + c;
                int board_y = p.y + r;

                if (board_x < 0 || board_x >= COLS) return 1;

                if (board_y >= ROWS) return 1;

                if (board_y >= 0 && board[board_y][board_x] != 0) return 1;
            }
        }
    }
    return 0;
}

void lock_piece(Tetromino p,int board[ROWS][COLS]) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (p.shape[r][c] != 0) {
                int board_y = p.y + r;
                int board_x = p.x + c;
                
                if (board_y >= 0 && board_y < ROWS && board_x >= 0 && board_x < COLS) {
                    board[board_y][board_x] = p.color;
                }
            }
        }
    }
}

int clear_full_lines(int board[ROWS][COLS]) {
    int lines_cleared = 0;

    for (int r = ROWS - 1; r >= 0; r--) {
        int is_full = 1;
        for (int c = 0; c < COLS; c++) {
            if (board[r][c] == 0) {
                is_full = 0;
                break;
            }
        }

        if (is_full) {
            lines_cleared++;
            for (int r_above = r; r_above > 0; r_above--) {
                for (int c = 0; c < COLS; c++) {
                    board[r_above][c] = board[r_above - 1][c];
                }
            }
    
            for (int c = 0; c < COLS; c++) {
                board[0][c] = 0;
            }
            r++; 
        }
    }
    return lines_cleared;
}

void save_highscore(char username[], int score, int highscore) {
    if(score <= highscore)return;
    char names[100][50];
    int scores[100];
    int count = 0;
    int found = 0;

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
                    scores[count] = score;
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
    return;
}

void save_game_state(int board[ROWS][COLS], int score, int level, double delay,int curr, int next, char* user) {
    char filename[70];
    sprintf(filename,"%sdata.txt",user);
    FILE *f = fopen(filename, "w");
    if (f == NULL) return;

    fprintf(f, "%d %d %lf\n", score, level, delay);

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            fprintf(f, "%d ", board[r][c]);
        }
        fprintf(f, "\n");
    }
    fprintf(f,"%d %d \n",curr,next);
    fclose(f);
}

int load_game_state(int board[ROWS][COLS], int *score, int *level, double *delay, int *curr, int *next, char* user) {
    char filename[70];
    sprintf(filename,"%sdata.txt",user);
    FILE *f = fopen(filename, "r");
    if (f == NULL) return 0;

    if (fscanf(f, "%d %d %lf", score, level, delay) != 3) {
        fclose(f);
        return 0;
    }

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            fscanf(f, "%d", &board[r][c]);
        }
    }

    fscanf(f,"%d %d",curr,next);
    fclose(f);
    return 1;
}

void clear_save(char* user) {
    char filename[70];
    sprintf(filename,"%sdata.txt",user);
    remove(filename);
}