#include "piece.h"
#include "game.h"
#include <stdlib.h>
#include <time.h>


int SHAPES[7][4][4] = {
    {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}}, 
    {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}, 
    {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}, 
    {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}, 
    {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}, 
    {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}, 
    {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}  
};

Tetromino generate_random_piece() {
    static int seeded = 0;
    if (!seeded) { srand(time(NULL)); seeded = 1; }

    int type = rand() % 7;
    Tetromino p;
    p.x = 3;
    p.y = 0;
    p.color = type + 1;

    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            p.shape[i][j] = SHAPES[type][i][j];
    return p;
}

Tetromino set_piece(int type) {
    Tetromino p;
    p.x = 3;
    p.y = 0;
    p.color = type + 1;

    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            p.shape[i][j] = SHAPES[type][i][j];
    return p;
}

void rotate_piece(Tetromino *p,int board[ROWS][COLS]) {
    int temp[4][4] = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[j][3 - i] = p->shape[i][j];
        }
    }
    
    int old_shape[4][4];
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            old_shape[i][j] = p->shape[i][j];
            p->shape[i][j] = temp[i][j];
        }
    }

    if (check_collision(*p,board)) {
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                p->shape[i][j] = old_shape[i][j];
            }
        }
    }
}