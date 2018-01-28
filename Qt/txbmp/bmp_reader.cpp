#include <QDebug>

#include "bmp_reader.h"

BMPreader::BMPreader(const char *fileName)
{
    mFP = fopen(fileName, "r");
    if(mFP)
    {
        qInfo("File %s opened\n", fileName);

        unsigned char buffer[1024];
        int len = (int)fread(buffer, 1, 1024, mFP);
        if(len > 0)
        {
            qInfo(" - read %d bytes\n", len);
            int raster = buffer[10];
            mPixelLength = len - raster;
            qInfo("There is %d pixelBytes, for %d pixels\n", mPixelLength, mPixelLength / 3);

            memcpy(mPixelBuffer, &buffer[raster], mPixelLength);

//            int idx = 0;
//            for(int k = 0; k < 16; k++)
//            {
//                qInfo("%d: 0x%02X,0x%02X,0x%02X", k, mPixelBuffer[idx++], mPixelBuffer[idx++], mPixelBuffer[idx++]);
//            }
        }
    }
    else
    {
        qCritical("Could not open %s\n", fileName);
    }
}

int BMPreader::getBuffer(unsigned char *buffer, int len)
{
    if(len > mPixelLength)
        len = mPixelLength;

    if(buffer)
        memcpy(buffer, mPixelBuffer, len);

    return len;
}

BMPreader::~BMPreader()
{
    if(mFP)
    {
        fclose(mFP);
    }
}
