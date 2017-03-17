#ifndef _DEBUG_H_
#define _DEBUG_H_
#include <cyg/kernel/diag.h>
#include <kses_term.h>
#include <utils.h>

class cDebug
{
    struct sDebugNode
    {
        cDebug * debug;
        sDebugNode * next;

        sDebugNode(cDebug * obj = 0);
    };

    const char * mName;
    static sDebugNode * mDebugList;

    const char * getObjectName(){ return mName; };

protected:
    virtual ~cDebug();

    int mDebugLevel;

    void dbg_printf(int lvl,const char * fmt,...);
    void dbg_dump_buf(int lvl, void * b,int len);


public:
    cDebug(const char * command);
    virtual void setDebug(int lvl);

    static void porcessDebug(cTerm &t, int argc,char **argv);
};

#endif //Include Guard
