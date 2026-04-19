#ifndef GAME_H
#define GAME_H

#include "piece.h" 

#define ROWS 20
#define COLS 10

void save_highscore(char username[], int score, int highscore);
void init_board(int board[ROWS][COLS]);
int check_collision(Tetromino p,int board[ROWS][COLS]);
void lock_piece(Tetromino p,int board[ROWS][COLS]); 
int clear_full_lines(int board[ROWS][COLS]);

int load_game_state(int board[ROWS][COLS], int *score, int *level, double *delay, int *curr, int *next, char* user);
void save_game_state(int board[ROWS][COLS], int score, int level, double fall_delay, int curr, int next, char* user);
void clear_save(char* user);
#endif