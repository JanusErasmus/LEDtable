#ifndef BMPREADER_H
#define BMPREADER_H
#include <stdio.h>

class BMPreader
{
    FILE *mFP;
    int mPixelLength;
    unsigned char* mPixelBuffer[1024];

public:
    BMPreader(const char *fileName);
    ~BMPreader();

    int getBuffer(unsigned char *buffer, int len);
};

#endif // BMPREADER_H
