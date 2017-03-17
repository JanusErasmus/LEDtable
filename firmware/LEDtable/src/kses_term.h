#ifndef _TERM_H_
#define _TERM_H_
#include <cyg/kernel/kapi.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/io/io.h>

#include "definitions.h"

class cKSESterminal
{
    enum eArrowPress
    {
        noArrow,
        upArrow,
        downArrow,
        leftArrow,
        rightArrow
    };

    static cKSESterminal * __instance;
    cyg_uint32 mRxIdx;
    cyg_uint32 mBuffSize;
    cyg_io_handle_t mDevHandle;
    char * mDev;
    char * mRxBuff;
    char * mPrevBuff;
    const char * mPrompt;
    cyg_uint8 mStack[TERM_STACK_SIZE];
    cyg_thread mThread;
    cyg_handle_t mThreadHandle;
    static void term_thread_func(cyg_addrword_t arg);

    void banner();
    eArrowPress wasArrow();
    void process_command_line();
    void prompt();

    cKSESterminal(char * dev,cyg_uint32 b_size,const char * const prompt_str);

    //VT100
    bool isVT100();

public:
    static void init(char * dev,cyg_uint32 b_size,const char * const prompt_str);

    static cyg_bool run();

    cKSESterminal& operator<<(void *);
    cKSESterminal& operator<<(const char *);
    cKSESterminal& operator<<(int);
    cKSESterminal& operator<<(unsigned int);
    cKSESterminal& operator<<(unsigned char);
    char * format(const char *f,...);
    virtual ~cKSESterminal();
};

#endif  //Include Guard
