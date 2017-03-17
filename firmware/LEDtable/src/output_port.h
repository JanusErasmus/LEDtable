#ifndef RELAY_H_
#define RELAY_H_

#include "kses_term.h"


class cOutput
{
    static cOutput* __instance;
    cOutput(cyg_uint32* portNumbers, cyg_uint8 portCount);

    cyg_uint8 mOutputCnt;
    cyg_uint32* mOutputList;
    cyg_uint32* mAcitveLow;

    void updateOutput(cyg_uint8, bool);
    void setupPorts(cyg_uint32* ports, cyg_uint8 count);

public:
    static void init(cyg_uint32* portNumbers, cyg_uint8 portCount);
    static cOutput* get();

    bool getPortState(cyg_uint8);
    bool setPortState(cyg_uint8, bool);

    void setActiveLow(cyg_uint8 portNumber);
    static void debugOutputs(cKSESterminal & t,int argc,char *argv[]);
};

#endif /* RELAY_H_ */
