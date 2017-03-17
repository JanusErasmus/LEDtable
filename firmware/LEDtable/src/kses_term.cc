#include <cyg/kernel/diag.h>
#include <cyg/io/ttyio.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "kses_term.h"
#include "debug.h"

cKSESterminal * cKSESterminal::__instance = NULL;

cKSESterminal::cKSESterminal(char * dev,cyg_uint32 b_size,const char * const prompt_str)
{
    cyg_uint32 len = strlen(dev)+1;
    mDev = new char[len];
    strcpy(mDev,dev);

    mBuffSize = b_size;
    mRxBuff = new char[mBuffSize];
    mPrevBuff = new char[mBuffSize];
    mPrevBuff[0] = 0;
    mRxIdx = 0;

    mPrompt = prompt_str;

    Cyg_ErrNo err = cyg_io_lookup(mDev,&mDevHandle);
    diag_printf("cKSESterminal RX device - %s: %s \n", dev, strerror(-err));

    cyg_tty_info_t tty_info;
    len = sizeof(tty_info);
    cyg_io_get_config(mDevHandle,
                      CYG_IO_GET_CONFIG_TTY_INFO,
                      &tty_info,
                      &len);

    //diag_printf("TERM: TTY in_flags = 0x%08X, out_flags = 0x%08X\n",tty_info.tty_in_flags,tty_info.tty_out_flags);

    tty_info.tty_in_flags = (CYG_TTY_IN_FLAGS_CR | CYG_TTY_IN_FLAGS_ECHO);

    cyg_io_set_config(mDevHandle,
                      CYG_IO_SET_CONFIG_TTY_INFO,
                      &tty_info,
                      &len);

    cyg_thread_create(TERM_PRIOR,
                         term_thread_func,
                         (cyg_addrword_t)this,
                         (char *)"Terminal",
                         mStack,
                         TERM_STACK_SIZE,
                         &mThreadHandle,
                         &mThread);

       cyg_thread_resume(mThreadHandle);
}

void cKSESterminal::init(char * dev,cyg_uint32 b_size,const char * const prompt_str)
{
    if (__instance == NULL)
    {
        __instance = new cKSESterminal(dev,b_size,prompt_str);
    }
}

void cKSESterminal::term_thread_func(cyg_addrword_t arg)
{
    if(!arg)
        return;

    cKSESterminal * _this = (cKSESterminal*)arg;

    _this->banner();
    _this->prompt();

    while(1)
    {
        _this->process_command_line();
    }
}

cKSESterminal& cKSESterminal::operator <<(const char *str)
{
    cyg_uint32 len = strlen(str);

    cyg_io_write(mDevHandle,str,&len);

    return(*this);
}
cKSESterminal& cKSESterminal::operator <<(int i)
{
    char str[20];
    sprintf(str,"%d",i);
    cyg_uint32 len = strlen(str);

    cyg_io_write(mDevHandle,str,&len);

    return(*this);
}

cKSESterminal& cKSESterminal::operator <<(unsigned char c)
{
    char str[6];
    sprintf(str,"0x%02X",c);
    cyg_uint32 len = strlen(str);

    cyg_io_write(mDevHandle,str,&len);

    return(*this);
}

cKSESterminal& cKSESterminal::operator <<(unsigned int i)
{
    char str[12];
    sprintf(str,"0x%08X",i);
    cyg_uint32 len = strlen(str);

    cyg_io_write(mDevHandle,str,&len);

    return(*this);
}
cKSESterminal& cKSESterminal::operator <<(void * p)
{
    *this<<(unsigned int)p;
    return(*this);
}


char * cKSESterminal::format(const char *f,...)
{
    va_list vl;
    va_start(vl,f);
    vsprintf(&mRxBuff[0],f,vl);
    va_end(vl);

    return &mRxBuff[0];
}

void cKSESterminal::banner()
{
    *this<<"Terminal started on device \""<<mDev<<"\"\n";
}

void cKSESterminal::prompt()
{
    *this << mPrompt;
}

cKSESterminal::eArrowPress cKSESterminal::wasArrow()
{
    if(mRxIdx > 3)
    {
        if(mRxBuff[0] == 0x1B && mRxBuff[1] == 0x5B)
        {
            switch(mRxBuff[2])
            {
            case 0x41:
                return upArrow;
            case 0x42:
                return downArrow;
            case 0x43:
                return rightArrow;
            case 0x44:
                return leftArrow;
            default:
                diag_printf("\n\nTHIS WAS 0x%02X\n\n\n", mRxBuff[2]);
                break;
            }
        }
    }
    return noArrow;
}

void cKSESterminal::process_command_line(void)
{

    char *argv[20];
    int argc = 20;

    mRxIdx = mBuffSize;
    cyg_io_read(mDevHandle, mRxBuff,&mRxIdx);

    mRxBuff[mRxIdx-1] = 0;

    if (mRxIdx >= 2 )
    {
        //check for arrow presses
        eArrowPress arrow = wasArrow();
        if(arrow && mPrevBuff[0])
        {
            if(arrow == upArrow)
            {
                char buff[4] = {0x1B, 0x5B, 0x43, '\n'};
                diag_printf(buff);


                //diag_printf("\n\nPrevious command: %s\n\n\n", mPrevBuff);
                memcpy(mRxBuff, mPrevBuff, mBuffSize);
                util_parse_params(mRxBuff,argv,argc,' ',' ');
                if ( argc )
                {
                    cDebug::process(*this,argc,argv);
                }

                prompt();
            }

            return;
        }

        memcpy(mPrevBuff, mRxBuff, mRxIdx);
        mPrevBuff[mRxIdx - 1] = 0;

        util_parse_params(mRxBuff,argv,argc,' ',' ');

        if ( argc )
        {
            cDebug::process(*this,argc,argv);
        }
    }
    prompt();
}

cKSESterminal::~cKSESterminal()
{
    delete [] mDev;
}
