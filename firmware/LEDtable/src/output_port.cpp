#include <cyg/kernel/diag.h>
#include <stdio.h>
#include <stdlib.h>

#include "output_port.h"

cOutput* cOutput::__instance = NULL;

/**
 * Initialize output ports
 *
 * @param portNumbers    A list of the output port numbers PA0 = 0, PA1 = 1 through to PF7 = 87
 *                         Use defined MACROS of input_port.h to generate port numbers INPUTx_(y) where x = A-F and y = 0-7
 * @param portCount        Number of ports in the list
 */
void cOutput::init(cyg_uint32* portNumbers, cyg_uint8 portCount)
{
    diag_printf("Initializing cOutput\n");
    if(!__instance)
    {
        __instance = new cOutput(portNumbers, portCount);
    }
}

cOutput* cOutput::get()
{
    return __instance;
}


cOutput::cOutput(cyg_uint32* portNumbers, cyg_uint8 portCount) : mOutputCnt(portCount)
{
    mOutputList = new cyg_uint32[mOutputCnt];
    memcpy(mOutputList, portNumbers, sizeof(cyg_uint32) * mOutputCnt);

    mAcitveLow = new cyg_uint32[mOutputCnt];
    memset(mAcitveLow, 0, sizeof(cyg_uint32) * mOutputCnt);

    setupPorts(mOutputList, mOutputCnt);

}

void cOutput::setupPorts(cyg_uint32* ports, cyg_uint8 count)
{
    for (int k = 0; k < count; k++)
    {
        CYGHWR_HAL_STM32_GPIO_SET (ports[k]);
    }
}

bool cOutput::setPortState(cyg_uint8 num, bool state)
{
    if(num >= mOutputCnt)
        return false;

    updateOutput(num, state);

    return true;
}

void cOutput::setActiveLow(cyg_uint8 portNumber)
{
    if(portNumber >= mOutputCnt)
            return;

    bool state = getPortState(portNumber);

    mAcitveLow[portNumber] = 1;

    updateOutput(portNumber, state);

}

void cOutput::updateOutput(cyg_uint8 num, bool state)
{
    if(num >= mOutputCnt)
        return;

    //if active low, invert state
    if(mAcitveLow[num])
        state = !state;

    CYGHWR_HAL_STM32_GPIO_OUT (mOutputList[num], state);

}


bool cOutput::getPortState(cyg_uint8 num)
{
    if(num >= mOutputCnt)
            return 0;

    int state = false;
    CYGHWR_HAL_STM32_GPIO_IN (mOutputList[num], &state);

    //if active low, invert state
    if(mAcitveLow[num])
        state = !state;

    return state;
}



