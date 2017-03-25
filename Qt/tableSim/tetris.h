#ifndef TETRIS_H
#define TETRIS_H
#include "rgb.h"

class tetris
{
    RGB *mGrid[16][16];

    int spriteX;
    int spriteY;
    RGB *sprite[4][4];

    void nextPiece();
    void clearArray(RGB ** grid, int x, int y);
    void copySprite(int gridX, int gridY);
    bool spriteCollide();

public:
    tetris();

    void getGrid(RGB **grid);
};

#endif // TETRIS_H
