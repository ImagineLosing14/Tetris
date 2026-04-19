#ifndef DRAW_H
#define DRAW_H

#include "piece.h"
#include "game.h"

void init_graphics();
void refresh_screen();
void close_graphics();

char* draw_namescreen();
void draw_main_menu(int selected, int highscore,int has_save);
void draw_game_over_screen(int score);
void game_quit_screen(int score);
void draw_leaderboard();

void draw_static_ui();
void update_ui_values(int score, int highscore, int level);

void draw_board_data(int b[ROWS][COLS]);
void draw_moving_piece(Tetromino p);
void draw_ghost_piece(Tetromino p, int b[ROWS][COLS]);
void draw_next_piece_preview(Tetromino p);

#endif