/*
 * Runner.cpp
 *
 *  Created on: 07 Apr 2017
 *      Author: Janus
 */

#include <stdlib.h>

#include "Runner.h"
#include "ws281x_driver.h"

Runner::Runner(cyg_uint8 count)
{
	mLedCount = count;
	mDiff = 1;
	mY = 0;
	mX = 0;
	cColor = 1;
}

cRGB off(0x0,0x0,0x0);
cRGB white(255,255,255);
cRGB blue(0x00, 0x00, 0xFF);
cRGB red(0xFF, 0x00, 0x00);
cRGB green(0x00, 0xFF, 0x00);
cRGB mix1(0xFF, 0xFF, 0x00);
cRGB mix2(0xFF, 0x00, 0xFF);
cRGB mix3(0x00, 0xFF, 0xFF);
//cRGB gray(0x80, 0x80, 0x80);
cRGB orange(179, 89, 0);
cRGB pink(159, 0, 80);

cyg_uint8 cColor = 0;
cRGB *pColor[] = {&off, &blue, &green, &red, &green, &white, &mix3, &green, &orange, &mix1, &orange, &pink, &green, &mix2, &mix3, &orange, &pink, 0};

void Runner::run()
{
    cWS281xDriver::get()->setPixel(mX, mY, *pColor[cColor]);
}

void Runner::next()
{

	mY += mDiff;

	if(mY >= 16)
	{
		mDiff = -1;
		mX++;
		mY += mDiff;
	}

	if(mY < 0)
	{
		mDiff = 1;
		mX++;
		mY += mDiff;
	}

	if((mX == 3) && (mY == 4))
	{
		mDiff = 1;
		mX = 0;
		mY = 0;

		cColor++;
	}
//	mY += mDiff;
//
//	if(mY > mLedCount)
//	{
//		mDiff = -1;
//		mY = mLedCount;
//
//		cColor++;
//	}
//	else if(mY < 0)
//	{
//		mDiff = 1;
//		mY = 0;
//
//		cColor++;
//	}

	if(!pColor[cColor])
		cColor = 0;
}

Runner::~Runner() {
	// TODO Auto-generated destructor stub
}

