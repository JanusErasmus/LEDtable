/*
 * Runner.h
 *
 *  Created on: 07 Apr 2017
 *      Author: Janus
 */
#include <cyg/kernel/kapi.h>
#ifndef SRC_RUNNER_H_
#define SRC_RUNNER_H_

class Runner
{

	cyg_uint8 mX;
    cyg_int8 mY;
    cyg_int8 mDiff;
    cyg_uint8 mColor;

public:
	Runner();
	virtual ~Runner();

	void setStart(cyg_uint8 idx){ mY = idx;};

	void run();
	void next();
};

#endif /* SRC_RUNNER_H_ */
