#ifndef PIECE_H
#define PIECE_H

typedef struct {
    int shape[4][4];
    int color;
    int x, y;
} Tetromino;

#define ROWS 20
#define COLS 10

void rotate_piece(Tetromino *p,int board[ROWS][COLS]);
Tetromino generate_random_piece();
Tetromino set_piece(int type);

#endif
