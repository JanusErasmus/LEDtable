#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "utils.h"
#include "kses_term.h"

class cDebug
{
    typedef void (*func_t)(cKSESterminal & t,int argc,char *argv[]);
    struct cmdNode
    {
        const char* command;
        const char* argDescription;
        const char* description;
        func_t function;
        cmdNode * next;

        cmdNode(const char * cmd, const char * arg, const char * desc, func_t f);
    };
    cmdNode * mCmdList;

    void registerFunctions();

    struct cDebugNode
    {
        cDebug * debug;
        cDebugNode * next;

        cDebugNode(cDebug * obj = 0);
    };

    const char * mName;
    static cDebugNode * mDebugList;

    cyg_bool debug(cKSESterminal & t,int argc,char *argv[]);
    void help(cKSESterminal & t);

    const char * getObjectName(){ return mName; };

protected:
    virtual ~cDebug();

    int mDebugLevel;

    void registerHeading(const char * cmd);
    void registerCommand(const char * cmd, const char * arg, const char * desc, func_t f);

    void dbg_printf(int lvl,const char * fmt,...);
    void dbg_dump_buf(int lvl, void * b,int len);


public:
    cDebug(const char * command);
    virtual void setDebug(int lvl);


    static void process(cKSESterminal & term, int argc,char * argv[]);


};

#endif //Include Guard
