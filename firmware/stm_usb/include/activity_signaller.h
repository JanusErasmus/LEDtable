#ifndef _ACTIVITY_SIGNALLER_H
#define _ACTIVITY_SIGNALLER_H

class ActivitySignaller
{
public:
    ActivitySignaller(){};
    virtual ~ActivitySignaller(){};

    virtual void signalTx() = 0;
    virtual void signalRx() = 0;
};
#endif // _ACTIVITY_SIGNALLER_H
