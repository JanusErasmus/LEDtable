#include <cyg/kernel/diag.h>
#include <cyg/io/ttyio.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <kses_term.h>
#include <TermCMD.h>
#include <utils.h>

cTerm * cTerm::__instance = NULL;

#define TRACE(_x, ...) INFO_TRACE("cTerm", _x,  ##__VA_ARGS__)

cTerm::cTerm(char * dev,cyg_uint32 b_size,const char * const prompt_str)
{
    Cyg_ErrNo err;

    mReceiver =0;

    cyg_uint32 len = strlen(dev)+1;
    mDev = new char[len];
    strcpy(mDev,dev);

    mBuffSize = b_size;
    mRxBuff = new char[mBuffSize];
    mRxIdx = 0;

    mPrompt = prompt_str;

    err = cyg_io_lookup(mDev,&mDevHandle);

    TRACE("Lookup error: %s\n",strerror(-err));

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

    prompt();

    cyg_thread_create(20,
                      cTerm::term_thread_func,
                      (cyg_addrword_t)this,
                      (char *)"Terminal",
                      mStack,
                      CYGNUM_HAL_STACK_SIZE_TYPICAL,
                      &mThreadHandle,
                      &mThread);

    cyg_thread_resume(mThreadHandle);

}


cTerm::~cTerm()
{
    delete [] mDev;
}

void cTerm::init(char * dev,cyg_uint32 b_size,const char * const prompt_str)
{
    if (__instance == NULL)
    {
        __instance = new cTerm(dev,b_size,prompt_str);
    }
}

void cTerm::setReceiver(cHDLCreceiver *receiver)
{
   if(!__instance)
      return;

   __instance->mReceiver = receiver;
}

void cTerm::term_thread_func(cyg_addrword_t arg)
{
    cTerm * t = (cTerm *)arg;

    for (;;)
    {
        t->run();
    }
}

void cTerm::run(void)
{
    mRxIdx = mBuffSize;
    cyg_io_read(mDevHandle,mRxBuff,&mRxIdx);

    mRxBuff[mRxIdx-1] = 0;

    if ( mRxIdx >= 1 )
    {
       if(mReceiver && (mRxBuff[0] == '~'))
       {
          cyg_uint8 frame[64];
          char string[4];
          cyg_uint32 index = 0;
          for(cyg_uint8 k = 1; k < (mRxIdx - 1); k++)
          {
             string[0] = mRxBuff[k++];
             string[1] = mRxBuff[k];
             string[2] = 0;
             frame[index++] = strtoul(string, 0, 16);
          }
//          diag_printf("HDLC %d\n", index);
//          diag_dump_buf(frame, index);

          if(index)
             mReceiver->pack(frame, index);
       }
       else
        process_command_line();
    }
    prompt();
}

cTerm& cTerm::operator <<(const char *str)
{
    cyg_uint32 len = strlen(str);

    cyg_io_write(mDevHandle,str,&len);

    return(*this);
}
cTerm& cTerm::operator <<(int i)
{
    char str[20];
    sprintf(str,"%d",i);
    cyg_uint32 len = strlen(str);

    cyg_io_write(mDevHandle,str,&len);

    return(*this);
}

cTerm& cTerm::operator <<(unsigned char c)
{
    char str[6];
    sprintf(str,"0x%02X",c);
    cyg_uint32 len = strlen(str);

    cyg_io_write(mDevHandle,str,&len);

    return(*this);
}

cTerm& cTerm::operator <<(unsigned int i)
{
    char str[12];
    sprintf(str,"0x%08X",i);
    cyg_uint32 len = strlen(str);

    cyg_io_write(mDevHandle,str,&len);

    return(*this);
}

cTerm& cTerm::operator <<(void * p)
{
    *this<<(unsigned int)p;
    return(*this);
}


char * cTerm::format(const char *f,...)
{
    va_list vl;
    va_start(vl,f);
    vsprintf(&mRxBuff[0],f,vl);
    va_end(vl);

    return &mRxBuff[0];
}

void cTerm::prompt()
{
    *this<<mPrompt;
}


void cTerm::process_command_line(void)
{
    char *argv[20];
    int argc = 20;

    util_parse_params(mRxBuff,argv,argc,' ',' ');

    if ( argc )
    {
        dispatch_command_line(argc,argv);
    }

}

void cTerm::dispatch_command_line(int argc,char *argv[])
{
    TermCMD::process(*this,argc,argv);
}

