#include <stdio.h>
#include <stdlib.h>
#include <cyg/kernel/diag.h>
#include <cyg/kernel/kapi.h>

#include "debug.h"

cDebug::sDebugNode * cDebug::mDebugList = 0;

cDebug::cDebug(const char * command) : mName(command), mDebugLevel(0)
{
    //diag_printf("cDebug: Init cDebug for %s\n", mName);

    if(!mDebugList)
    {
        mDebugList = new sDebugNode(this);

        return;
    }

    sDebugNode * o = mDebugList;
    while(o)
    {
        if(!o->next)
        {
            o->next = new sDebugNode(this);
            return;
        }

        o = o->next;
    }
}

void cDebug::porcessDebug(cTerm &t, int argc,char **argv)
{
    cyg_bool levelHandeled = false;

    sDebugNode * o = mDebugList;
    if(argc > 1)
    {
        while(o)
        {
            if(o->debug && !strcmp(argv[1], o->debug->getObjectName()))
            {
                if(argc > 2)
                {
                    int lvl = atoi(argv[2]);
                    t << "Setting " << argv[1] << " debug level to " << lvl << "\n";

                    o->debug->mDebugLevel = lvl;
                }
                else
                {
                    t << "Debug level of " << argv[1] << " is " << o->debug->mDebugLevel << "\n";
                }

                levelHandeled = true;
            }

            o = o->next;
        }
    }

    if(levelHandeled)
        return;

    t << "Invalid object name...\nUsage: debug <objName> <lvl>\nTry one of the following objNames:\n";
    o = mDebugList;
    while(o)
    {
        t << " - " << o->debug->getObjectName() << "\n";

        o = o->next;
    }

    return;
}

void cDebug::setDebug(int lvl)
{
   mDebugLevel = lvl;
}

void cDebug::dbg_printf(int lvl,const char * fmt,...)
{
   if (mDebugLevel >= lvl)
   {
      char str[256];
      va_list vl;
      va_start(vl,fmt);
      vsnprintf(str,256,fmt,vl);
      va_end(vl);
      diag_printf(str);
   }
}

void cDebug::dbg_dump_buf(int lvl, void * b,int len)
{
   if (mDebugLevel >= lvl)
   {
      diag_dump_buf(b,len);
   }
}

cDebug::sDebugNode::sDebugNode(cDebug * obj)
{
    debug = obj;
    next = 0;
}

cDebug::~cDebug()
{

}


