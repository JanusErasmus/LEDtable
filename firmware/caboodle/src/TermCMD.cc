#include <stdio.h>
#include <cyg/io/flash.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <pkgconf/hal.h>
#ifdef CYGPKG_HAL_CORTEXM_STM32_STM32F4KEYSTONE
#include <pkgconf/mlt_cortexm_stm32f4keystone_rom.h>
#endif
#ifdef CYGPKG_HAL_ARM_AT91SAM7XEK
#include <pkgconf/mlt_arm_at91sam7s512_rom.h>
#endif
#include "TermCMD.h"
#include "utils.h"
#include "debug.h"

cModem * TermCMD::mMdm = 0;

TermCMD::TermCMD()
{
}

void TermCMD::process(cTerm & t, int argc,char *argv[])
{
   cyg_uint8 index = 0;
   cmd_list_t* cmdList = mCmdTable[index++];

   if(!strcmp(argv[0], "debug"))
   {
      cDebug::porcessDebug(t, argc, argv);
      return;
   }


   while(cmdList)
   {
      cmd_list_t* t_ptr = 0;
      int k = 0;
      do
      {
         t_ptr = &cmdList[k++];
         if(!t_ptr || !t_ptr->cmd)
            break;

         //Special AT commands for modem
         if(!strncmp(argv[0],"AT",2))
         {
            if(mMdm)
               mMdm->ATcmd(t, argc, argv);

            return;
         }

         if(t_ptr->f && !strcmp(argv[0],t_ptr->cmd))
         {
            t_ptr->f(t, argc, argv);
            return;
         }
      }while(t_ptr->cmd);

      cmdList = mCmdTable[index++];
   }

   diag_printf(RED("Unknown Command \'%s\'. Type help for a list of commands\n"), argv[0]);
}

void TermCMD::help(cTerm & t,int argc,char *argv[])
{
   t<<(GREEN("TermCMD commands:\n"));

   cyg_uint8 index = 0;
   cmd_list_t* cmdList = mCmdTable[index++];
   while(cmdList)
   {
      cmd_list_t* t_ptr = 0;
      int k = 0;
      do
      {
         t_ptr = &cmdList[k++];
         if(!t_ptr || !t_ptr->cmd)
            break;

         if(t_ptr->f)
         {
            char txt[16];
            sprintf(txt,"%s %s", t_ptr->cmd, t_ptr->argDesc);
            t<<"  "<<t.format("%-10s - ",txt)<<t.format("%s\n",t_ptr->desc);
         }
         else
         {
            //this is a caption
            t<<t.format(BLUE("%s\n"), t_ptr->cmd);
         }

      }while(t_ptr->cmd);

      cmdList = mCmdTable[index++];
   }

}


void showThread(cTerm & t)
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

void showRAM(cTerm & t)
{
   extern cyg_uint32  __rom_vectors_vma;   //printf("__rom_vectors_vma 0x%08X\n",(cyg_uint32)&__rom_vectors_vma);
   extern cyg_uint32  __rom_data_start;    //printf("__rom_data_start  0x%08X\n",(cyg_uint32)&__rom_data_start);
   //extern cyg_uint32  __rom_data_end;    //printf("__rom_data_end  0x%08X\n",(cyg_uint32)&__rom_data_end);
   extern cyg_uint32  __ram_data_start;    //printf("__ram_data_start  0x%08X\n",(cyg_uint32)&__ram_data_start);
   extern cyg_uint32  __ram_data_end;      //printf("__ram_data_end    0x%08X\n",(cyg_uint32)&__ram_data_end);
   //extern cyg_uint32  _end;              //printf("__end              0x%08X\n",(cyg_uint32)&_end); cyg_thread_delay(50);
   struct mallinfo heap_info = mallinfo();


   cyg_uint32 text_size = (cyg_uint32)&__rom_data_start - (cyg_uint32)&__rom_vectors_vma; //this is the program address
   cyg_uint32 data_size =  (cyg_uint32)&__ram_data_end - (cyg_uint32)&__ram_data_start;
   //    cyg_uint32 bss_size  = (cyg_uint32)&_end - (cyg_uint32)&__ram_data_end;
   cyg_uint32 total_rom = text_size + data_size;// bss_size;

#ifdef CYGPKG_HAL_CORTEXM_STM32_STM32F4KEYSTONE
   cyg_uint32 flash_available = CYGMEM_REGION_flash_SIZE - ((cyg_uint32)&__rom_vectors_vma - CYGMEM_REGION_flash);
#endif
#ifdef CYGPKG_HAL_ARM_AT91SAM7XEK
   cyg_uint32 flash_available = CYGMEM_REGION_rom_SIZE - ((cyg_uint32)&__rom_vectors_vma - CYGMEM_REGION_rom);
#endif
   cyg_uint32 romUsage = (total_rom*100)/(flash_available);

   cyg_uint32 ramUsage = heap_info.usmblks + heap_info.uordblks;

   t << t.format(GREEN("Memory%21s%21s\n"), "Size", "Used");
   t << t.format(CYAN("%6s  0x%08X(%7d)  0x%08X(%7d)%5d %%\n"), "ROM", flash_available, flash_available, total_rom, total_rom, romUsage);
   t << t.format("%6s  0x%08X(%7d)  0x%08X(%7d)%5d %%\n", "heap", heap_info.arena, heap_info.arena, ramUsage, ramUsage, (((ramUsage)*100)/heap_info.arena));

}


extern const char* SWversion;

void showVersion(cTerm & t,int argc,char *argv[])
{
   t<<"Version           : "<< SWversion <<"\n";
   t<<"Build Timestamp   : "<< __DATE__ << " " << __TIME__ <<"\n";
}

void showUsage(cTerm & t,int argc,char *argv[])
{
   showThread(t);
   t << "\n";
   showRAM(t);
}

void handleTime(cTerm & t,int argc,char **argv)
{
   time_t now = time(0);
   t << "RTC    : " << asctime(localtime(&now));
   now = cyg_current_time() / 100;

   char timeString[32];
   timeToString(now, timeString);
   t << "Up time: " << timeString << "\n";
}

extern void softwareReset();

void resetProcessor(cTerm & t,int argc,char **argv)
{
   softwareReset();
}

const TermCMD::cmd_list_t shellCommands[] =
{
      {"SYSTEM"    ,0,0,0},
      {"h",       "",            "Show this help info", TermCMD::help},
      {"help",    "",            "Show this help info", TermCMD::help},
      {"ver",     "",            "Show the build and commit dates for this binary", showVersion},
      {"dt",      "",            "Dump Thread Info", showUsage},
      {"time",    "",            "Current system time", handleTime},
      {"reset",   "",            "Reset processor", resetProcessor},
      {0,0,0,0}
};
