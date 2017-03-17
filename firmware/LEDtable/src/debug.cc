#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include <cyg/kernel/diag.h>
#include <cyg/hal/hal_tables.h>
#include <cyg/kernel/kapi.h>
#include <pkgconf/mlt_cortexm_stm32f4keystone_rom.h>

#include "debug.h"
#include "stm32cpu.h"

extern cyg_uint32 unitVersion;

cDebug::cDebugNode * cDebug::mDebugList = 0;

cDebug::cDebug(const char * command) : mName(command), mDebugLevel(0)
{
    mCmdList = 0;

    //diag_printf("cDebug: Init cDebug for %s\n", mName);

    if(!mDebugList)
    {
        mDebugList = new cDebugNode(this);
        registerFunctions();

        return;
    }

    cDebugNode * o = mDebugList;
    while(o)
    {
        if(!o->next)
        {
            o->next = new cDebugNode(this);
            return;
        }

        o = o->next;
    }
}

void cDebug::process(cKSESterminal & t, int argc,char * argv[])
{
    cDebugNode * o = mDebugList;

    if(!strcmp(argv[0], "h") || !strcmp(argv[0], "help"))
    {
        t << YELLOW("TermCMD commands:\n");
        while(o)
        {
            if(o->debug)
                o->debug->help(t);

            o = o->next;
        }
        return;
    }

    if(!strcmp(argv[0], "debug"))
    {

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
                    return;
                }

                o = o->next;
            }
        }

        t << "Invalid object name...\nUsage: debug <objName> <lvl>\nTry one of the following objNames:\n";
        o = mDebugList;
        while(o)
        {
            t << " - " << o->debug->getObjectName() << "\n";

            o = o->next;
        }

        return;
    }

    while(o)
    {
        if(o->debug)
            if(o->debug->debug(t, argc, argv))
                return;

        o = o->next;
    }

    t << RED("Unknown Command: ")<< argv[0] << ". Type help for a list of commands\n";
}

void cDebug::help(cKSESterminal & t)
{
    cmdNode * c = mCmdList;
    while(c)
    {
        if(c->description)
        {
            t <<"  "<< t.format(CYAN("%-10s "),c->command) << t.format("%-5s ", c->argDescription) << t.format("- %s\n",c->description);
        }
        else
        {
            //this is a caption
            t << t.format("%s\n", c->command);
        }
        c = c->next;
    }
}

void handleTime(cKSESterminal & t,int argc,char *argv[])
{
    time_t now = time(NULL);
    t << "RTC    : " << asctime(localtime(&now));
    now = cyg_current_time() / 100;

    if(now < 60)
    {
        t << "Up time: " << (int)now << "s\n";
    }
    else
    {
        int min = now / 60;
        now %= 60;
        if(min < 60)
        {
            t << "Up time: " << min << "m" << (int)now << "s\n";
        }
        else
        {
            int hour = min / 60;
            min %= 60;
            if(hour < 24)
            {
                t << "Up time: " << hour << "h" << min << "m" << (int)now << "s\n";
            }
            else
            {
                int day = hour / 24;
                hour %= 24;
                t << "Up time:  "<< day << "d" << hour << "h" << min << "m" << (int)now << "s\n";
            }
        }
    }
}

void showThread(cKSESterminal & t)
{
    cyg_handle_t thread = 0;
    cyg_uint16 id;
    cyg_thread_info info;
    bool flag = 0;

    t << t.format(GREEN("ID %15s%6s%12s%12s\n"), "Name", "Prior", "Stack size", "Used");

    while ( cyg_thread_get_next(&thread,&id) )
    {
        if ( !cyg_thread_get_info(thread,id,&info) )
        {
            break;
        }

        if(flag)
            t << t.format("%02d%16s%6d  0x%08X  0x%08X%5d %%\n", info.id, info.name, info.set_pri, info.stack_size, info.stack_used, (int)((info.stack_used*100)/info.stack_size));
        else
            t << t.format(CYAN("%02d%16s%6d  0x%08X  0x%08X%5d %%\n"), info.id, info.name, info.set_pri, info.stack_size, info.stack_used, (int)((info.stack_used*100)/info.stack_size));

        flag = !flag;
    }
}

void showRAM(cKSESterminal & t)
{
    extern cyg_uint32  __rom_vectors_vma;     //printf("__rom_vectors_vma 0x%08X\n",(cyg_uint32)&__rom_vectors_vma);
    extern cyg_uint32  __rom_data_start;    //printf("__rom_data_start  0x%08X\n",(cyg_uint32)&__rom_data_start);
    //extern cyg_uint32  __rom_data_end;        //printf("__rom_data_end  0x%08X\n",(cyg_uint32)&__rom_data_end);
    extern cyg_uint32  __ram_data_start;    //printf("__ram_data_start  0x%08X\n",(cyg_uint32)&__ram_data_start);
    extern cyg_uint32  __ram_data_end;      //printf("__ram_data_end    0x%08X\n",(cyg_uint32)&__ram_data_end);
    //extern cyg_uint32  _end;                //printf("__end              0x%08X\n",(cyg_uint32)&_end); cyg_thread_delay(50);
    struct mallinfo heap_info = mallinfo();


    extern cyg_uint32 __ccm_start;
    extern cyg_uint32 __ccm_end;

    cyg_uint32 text_size = (cyg_uint32)&__rom_data_start - (cyg_uint32)&__rom_vectors_vma; //this is the program address
    cyg_uint32 data_size =  (cyg_uint32)&__ram_data_end - (cyg_uint32)&__ram_data_start;
//    cyg_uint32 bss_size  = (cyg_uint32)&_end - (cyg_uint32)&__ram_data_end;
    cyg_uint32 total_rom = text_size + data_size;// bss_size;
    cyg_uint32 flash_available = CYGMEM_REGION_flash_SIZE - ((cyg_uint32)&__rom_vectors_vma - CYGMEM_REGION_flash);
    cyg_uint32 romUsage = (total_rom*100)/(flash_available);
    cyg_uint32 ramUsage = heap_info.usmblks+heap_info.uordblks;
    cyg_uint32 ccm_used = (cyg_uint32)&__ccm_end - (cyg_uint32)&__ccm_start;


    t << t.format(GREEN("Memory%21s%21s\n"), "Size", "Used");
    t << t.format("%6s  0x%08X(%7d)  0x%08X(%7d)%5d %%\n", "CCM", CYGMEM_REGION_ccm_SIZE, CYGMEM_REGION_ccm_SIZE, ccm_used, ccm_used, ((ccm_used*100)/CYGMEM_REGION_ccm_SIZE));
    t << t.format(CYAN("%6s  0x%08X(%7d)  0x%08X(%7d)%5d %%\n"), "ROM", flash_available, flash_available, total_rom, total_rom, romUsage);
    t << t.format("%6s  0x%08X(%7d)  0x%08X(%7d)%5d %%\n", "heap", heap_info.arena, heap_info.arena, ramUsage, ramUsage, (((ramUsage)*100)/heap_info.arena));

}

void showUsage(cKSESterminal & t,int argc,char *argv[])
{
    t<< t.format(YELLOW("THREAD USAGE:\n"));
    showThread(t);

    t<< t.format(YELLOW("\nMEMORY USAGE:\n"));
    showRAM(t);
}

void showVersion(cKSESterminal & t,int argc,char *argv[])
{
    t<<"Version           : "<< unitVersion<<"\n";
    t<<"Build Timestamp   : "<< __DATE__ << " " << __TIME__ <<"\n";
}


void reset(cKSESterminal & t, int argc, char *argv[])
{
    t << "Device will now RESET\n";

    cyg_thread_delay(100);
    cyg_scheduler_lock();

    HAL_WRITE_UINT32(0xE000ED00 + 0x0C, (0x5FA << 16) | (1 << 2));
}

void setTime(cKSESterminal & term, int argc,char * argv[])
{
    cyg_uint16 yy,mm,dd,h,m,s;
    if(argc > 5)
    {
        yy = strtoul(argv[1],NULL,10);
        if(yy < 1900)
            return;
        term << term.format("yy : %d\n", yy);

        mm = strtoul(argv[2],NULL,10);
        if(mm > 12)
            return;
        term << term.format("mm : %d\n", mm);

        dd = strtoul(argv[3],NULL,10);
        if(dd > 31)
            return;
        term << term.format("dd : %d\n", dd);

        h = strtoul(argv[4],NULL,10);
        if(h > 24)
            return;
        term << term.format("h  : %d\n", h);

        m = strtoul(argv[5],NULL,10);
        if(m > 60)
            return;
        term << term.format("m  : %d\n", m);

        s = strtoul(argv[6],NULL,10);
        if(s > 60)
            return;
        term << term.format("s  : %d\n", s);

        time_t timeVal;
        struct tm info;
        info.tm_year = yy - 1900;
        info.tm_mon =  mm - 1;
        info.tm_mday = dd;
        info.tm_hour = h;
        info.tm_min = m;
        info.tm_sec = s;
        timeVal = mktime(&info);

        if(!cyg_libc_time_settime(timeVal))
            term << GREEN("Updated time\n");
        else
            term << RED("Could not update time\n");
    }
    else
    {
        term<<"Not enough input values. See help\n";
    }
}

void cDebug::registerFunctions()
{
    registerHeading("SYSTEM");
    registerCommand("help", "","Show this help", 0);
    registerCommand("h", "","Show this help", 0);
    registerCommand("ver" , "", "Show the build and commit dates for this binary", showVersion);
    registerCommand("dt", "","Current system usage", showUsage);
    //registerCommand("time", "","Current system time", handleTime);
    //registerCommand("setTime","<yyyy mm dd HH MM SS>", "Set system time", setTime);

    registerCommand("pstat"    , "",            "Show processor peripheral power", stm32cpu::pStatus);
    registerCommand("reset"    , "",            "Reset processor", reset);
}

void cDebug::registerCommand(const char * cmd, const char * arg, const char * desc, func_t f)
{
    //diag_printf("cDebug: Register command %s for %s\n", cmd, mName);

    if(!mCmdList)
    {
        mCmdList = new cmdNode(cmd, arg, desc, f);
        return;
    }

    cmdNode * c = mCmdList;
    while(c)
    {
        if(!c->next)
        {
            c->next = new cmdNode(cmd, arg, desc, f);
            return;
        }

        c = c->next;
    }
}

void cDebug::registerHeading(const char * cmd)
{
    registerCommand(cmd, 0, 0, 0);
}

cyg_bool cDebug::debug(cKSESterminal & t,int argc,char *argv[])
{
    cmdNode * c = mCmdList;
    while(c)
    {
        if(!strcmp(argv[0], c->command))
        {
            if(c->function)
                c->function(t, argc, argv);
            else
                t << "No command\n";

            return true;
        }
        c = c->next;
    }

    return false;
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

cDebug::cmdNode::cmdNode(const char * cmd, const char * arg, const char * desc, func_t f)
{
    command = cmd;
    argDescription = arg;
    description = desc;
    function = f;
    next = 0;
}

cDebug::cDebugNode::cDebugNode(cDebug * obj)
{
    debug = obj;
    next = 0;
}

cDebug::~cDebug()
{

}



