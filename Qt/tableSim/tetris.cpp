#include <QDebug>

#include "tetris.h"

tetris::tetris()
{
    spriteX = 0;
    spriteY = 0;

    // Null out the pointers contained in the array:
    for (int x = 0; x < 16; ++x)
    {
      for (int y = 0; y < 16; ++y)
      {
          mGrid[x][y] = 0;
      }
    }

    for (int x = 0; x < 4; ++x)
    {
      for (int y = 0; y < 4; ++y)
      {
          sprite[x][y] = 0;
      }
    }



    nextPiece();

}

void  tetris::nextPiece()
{
    for (int x = 0; x < 4; ++x)
    {
        for (int y = 0; y < 4; ++y)
        {
            if(sprite[x][y])
                delete sprite[x][y];

            sprite[x][y] = 0;
        }
    }

    int piece = (rand() % 7);
    //qDebug() << piece;

    spriteX++;
    if(spriteX >= 14)
        spriteX--;

    for (int x = 0; x < 4; ++x)
    {
        for (int y = 0; y < 4; ++y)
        {
            if(sprite[x][y] && ((spriteX + x) >= 14))
                spriteX--;
        }
    }

    switch(piece)
    {
    case 0: //tower
        sprite[1][0] = new RGB(0, 0, 255);
        sprite[1][1] = new RGB(0, 0, 255);
        sprite[1][2] = new RGB(0, 0, 255);
        sprite[1][3] = new RGB(0, 0, 255);
        break;
    case 1://box
        sprite[1][1] = new RGB(0, 255, 255);
        sprite[1][2] = new RGB(0, 255, 255);
        sprite[2][1] = new RGB(0, 255, 255);
        sprite[2][2] = new RGB(0, 255, 255);
        break;
    case 2:  //pyramid
        sprite[1][1] = new RGB(0, 0, 255);
        sprite[0][2] = new RGB(0, 0, 255);
        sprite[1][2] = new RGB(0, 0, 255);
        sprite[2][2] = new RGB(0, 0, 255);
        break;
    case 3: //Left leaner
        sprite[0][1] = new RGB(255, 0, 255);
        sprite[1][1] = new RGB(255, 0, 255);
        sprite[1][2] = new RGB(255, 0, 255);
        sprite[2][2] = new RGB(255, 0, 255);
        break;
    case 4: //Right leaner
        sprite[2][1] = new RGB(255, 0, 0);
        sprite[1][1] = new RGB(255, 0, 0);
        sprite[1][2] = new RGB(255, 0, 0);
        sprite[0][2] = new RGB(255, 0, 0);
        break;
    case 5: //Left knight
        sprite[1][1] = new RGB(255, 255, 0);
        sprite[2][1] = new RGB(255, 255, 0);
        sprite[2][2] = new RGB(255, 255, 0);
        sprite[2][3] = new RGB(255, 255, 0);
        break;
    default:
    case 6: //Right knight
        sprite[2][1] = new RGB(255, 255, 255);
        sprite[1][1] = new RGB(255, 255, 255);
        sprite[1][2] = new RGB(255, 255, 255);
        sprite[1][3] = new RGB(255, 255, 255);
        break;
    }
}

void tetris::copySprite(int gridX, int gridY)
{
    for (int x = 0; x < 4; ++x)
    {
        for (int y = 0; y < 4; ++y)
        {
            if(sprite[x][y])
            {
                if(mGrid[gridX + x][gridY + y])
                {
                    delete mGrid[gridX + x][gridY + y];
                }

                mGrid[gridX + x][gridY + y] = new RGB(sprite[x][y]->red, sprite[x][y]->green, sprite[x][y]->blue);
            }
        }
    }

    nextPiece();
}

bool tetris::spriteCollide()
{
    for (int y = 0; y < 4; ++y)
    {
    for (int x = 0; x < 4; ++x)
    {
        if(sprite[x][y])
        {
            if(mGrid[spriteX + x][spriteY + y + 1])
                return true;

            if((spriteY + y) >= 15)
                return true;
        }
    }
    }

    return false;
}

void tetris::getGrid(RGB **grid)
{
    for (int x = 0; x < 16; ++x)
    {
        for (int y = 0; y < 16; ++y)
        {
            RGB **gridPtr = (grid + 16*x + y);
            if(mGrid[x][y])
            {
                if(!*gridPtr)
                {
                    *gridPtr = new RGB();
                }

                (*gridPtr)->red = mGrid[x][y]->red;
                (*gridPtr)->green = mGrid[x][y]->green;
                (*gridPtr)->blue = mGrid[x][y]->blue;
            }
            else
            {
                if(*gridPtr)
                {
                    delete *gridPtr;
                    *gridPtr = 0;
                }
            }
        }
    }

    for (int x = 0; x < 4; ++x)
    {
        for (int y = 0; y < 4; ++y)
        {
            RGB **gridPtr = (grid + 16*(spriteX + x) + spriteY + y);
            if(sprite[x][y])
            {
                if(!*gridPtr)
                {
                    *gridPtr = new RGB();
                }

                (*gridPtr)->red = sprite[x][y]->red;
                (*gridPtr)->green = sprite[x][y]->green;
                (*gridPtr)->blue = sprite[x][y]->blue;
            }
        }
    }


    spriteY++;
    if(spriteCollide())
    {
        copySprite(spriteX, spriteY);

        spriteY = 0;
    }
}
