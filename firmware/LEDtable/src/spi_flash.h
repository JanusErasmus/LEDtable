#ifndef _SPI_H
#define _SPI_H

#include <cyg/kernel/kapi.h>
#include <cyg/kernel/diag.h>
#include <cyg/hal/hal_diag.h>
#include <cyg/io/spi.h>
#include <cyg/io/spi_stm32.h>
#include <stdlib.h>
#include <string.h>

#include <TermCMD.h>

class SpiFlash
{
private:
   static SpiFlash * __instance;
   SpiFlash();
   bool mReadyFlag;

   void global_unprotect();

   cyg_uint32 SectSize;
   cyg_uint32 NumSect;
   cyg_uint32 upperAddr;

public:
   static void init();
   static SpiFlash * get();
   bool isReady();

    bool erase(cyg_uint32 addr);
    bool write(cyg_uint32 addr,cyg_uint8 * data,cyg_uint32 len);
    bool read(cyg_uint32 addr,cyg_uint8 * data,cyg_uint32 len);

    cyg_uint32 GetNumSect();
    cyg_uint32 GetSectSize();

    static void debugRead(cTerm & t,int argc,char *argv[]);
    static void debugWrite(cTerm & t,int argc,char *argv[]);
    static void debugErase(cTerm & t,int argc,char *argv[]);
};

extern const TermCMD::cmd_list_t flashCommands[];
#endif
