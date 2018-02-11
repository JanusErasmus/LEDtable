/*
 * Runner.cpp
 *
 *  Created on: 07 Apr 2017
 *      Author: Janus
 */

#include <stdlib.h>

#include "Runner.h"
#include "ws281x_driver.h"

Runner::Runner()
{
	mDiff = 1;
	mY = 0;
	mX = 0;
	mColor = 1;
}

void Runner::run()
{
    cWS281xDriver::get()->setPixel(mX, mY, *pColor[mColor]);
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

	if((mX >= 16) || (mY >= 16))
	{
		mDiff = 1;
		mX = 0;
		mY = 0;

		   cyg_uint8 prevColor = mColor;
		   mColor = rand() % 20;
		   while((mColor == prevColor) && (mColor > PCOLOR_COUNT))
		         mColor = rand() % 20;
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



//	diag_printf("%d, %d\n", mX, mY);
}

Runner::~Runner()
{
}

