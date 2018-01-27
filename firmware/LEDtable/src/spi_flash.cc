#include <cyg/kernel/kapi.h>
#include <cyg/io/flash.h>


#include "spi_dev.h"
#include "spi_flash.h"
#include "utils.h"

SpiFlash * SpiFlash::__instance = 0;

#define TRACE(_x, ...) INFO_TRACE( "SpiFlash" , _x,  ##__VA_ARGS__)

void SpiFlash::init()
{
   if(__instance == 0)
   {
      __instance = new SpiFlash();
   }
}

SpiFlash * SpiFlash::get()
{
   return __instance;
}

SpiFlash::SpiFlash() : mReadyFlag(false)
{
	SectSize = 0;
	NumSect = 0;
	upperAddr = 0;

	cyg_flash_info_t info;
	          cyg_flash_init(0);
	          int ret = cyg_flash_get_info(0,&info);
	          TRACE("cyg_flash_info return: %d\n", ret);
	          if(ret)
	          {
	             return;
	          }

              TRACE("SPI_FLASH: Device enabled\n");
              TRACE("Spansion - number of block infos: 0x%8.0X\n",info.num_block_infos);
              TRACE("         - block count: 0x%8.0X\n",info.block_info[0].blocks);
              TRACE("         - block size 0x%8.0X\n",info.block_info[0].block_size);
              TRACE("         - START address 0x%8.0X \n",info.start);
              TRACE("         - end address 0x%8.0X \n",info.end);

//	global_unprotect();
//	cyg_thread_delay(100);

	/*//Unprotect a single sector
     tx_buff[0] = 0x39;
     tx_buff[1] = 0x00;
     tx_buff[2] = 0x00;
     tx_buff[3] = 0x00;
     cyg_spi_transfer(Spi_Device,false,4,tx_buff,0);*/

	mReadyFlag = true;
}

bool SpiFlash::isReady()
{
	return mReadyFlag;
}

cyg_uint32 SpiFlash::GetSectSize()
{
   return SectSize;
}

cyg_uint32 SpiFlash::GetNumSect()
{
   return NumSect;
}

#define FLASH_WREN  0x06
#define FLASH_WRSR  0x01

void SpiFlash::global_unprotect()
{
        cyg_uint8 tx_buff[4];
    tx_buff[0] = FLASH_WREN;
    cyg_spi_transfer(&stm32_flash_dev,false,1,tx_buff,0);

    //global unprotect FLASH
    tx_buff[0] = FLASH_WRSR;
    tx_buff[1] = 0x01;
    tx_buff[2] = 0x00;
    cyg_spi_transfer(&stm32_flash_dev,false,3,tx_buff,0);

//    waitReady();
}


bool SpiFlash::read(cyg_uint32 addr, cyg_uint8 *data_ptr, cyg_uint32 len)
{
    if(cyg_flash_read(addr, data_ptr, len, 0) < 0)
       return false;

	return true;
}

bool SpiFlash::write(cyg_uint32 addr, cyg_uint8 *data_ptr, cyg_uint32 len)
{
    bool stat = false;

    if(cyg_flash_program(addr, data_ptr, len, 0) < 0)
       return false;

	return stat;
}

bool SpiFlash::erase(cyg_uint32 addr)
{
    if(cyg_flash_erase(addr, 0x10, 0) < 0)
       return false;

	return true;
}

void SpiFlash::debugRead(cTerm & t,int argc,char *argv[])
{
    if(!(__instance && __instance->mReadyFlag))
        return;

    if (argc > 2)
    {
        cyg_uint32 addr = (cyg_uint32)strtoul(argv[1],NULL,16);
        cyg_uint32 num = (cyg_uint32)strtoul(argv[2],NULL,16);

        cyg_uint8 buff[num];

        int success = cyg_flash_read(addr, buff, num, NULL);
        diag_printf("Reading: 0x%08X Success %d\n", addr, success);

        if (1)//success > 0)
        {
            diag_printf(GREEN("Read %d bytes from serial flash at address 0x%08X\n"), num, addr);
            diag_dump_buf(buff, num);
        }
        else
        {
            diag_printf(RED("Error reading from serial flash!\n"));
        }
    }
    else
    {
        diag_printf("You need to supply an address and number of bytes to read\n");
    }
}

void SpiFlash::debugWrite(cTerm & t,int argc,char *argv[])
{
    if(!(__instance && __instance->mReadyFlag))
        return;

    if (argc > 3)
    {
        cyg_uint32 addr = (cyg_uint32)strtoul(argv[1],NULL,16);
        cyg_uint32 num = (cyg_uint32)strtoul(argv[2],NULL,16);
        cyg_uint8 val = (cyg_uint8)strtoul(argv[3],NULL,16);

        cyg_uint8 buff[num];

        memset(buff,val,num);


        int success = cyg_flash_program(addr, buff, num, NULL);

        if (success < 0)
        {
            diag_printf(RED("Error writing to serial flash!\n"));
        }
        else
        {
            diag_printf(GREEN("Wrote %d bytes of 0x%02X to serial flash at address 0x%08X\n"), num, val, addr);
        }
    }
    else
    {
        diag_printf("You need to supply an address, number of bytes and value to write\n");
    }
}

void SpiFlash::debugErase(cTerm & t,int argc,char *argv[])
{
    if(!(__instance && __instance->mReadyFlag))
        return;

    if (argc > 1)
    {
        cyg_uint32 addr = (cyg_uint32)strtoul(argv[1],NULL,16);

        int success = cyg_flash_erase(addr, 0x10, NULL);

        if (success != 0)
        {
            diag_printf(RED("Error erasing serial flash! E:0x%02X\n"), success);
        }
        else
        {
            diag_printf(GREEN("Erased serial flash sector at address 0x%08X. E:0x%02X\n\n"), addr, success);
        }
    }
    else
    {
        diag_printf("You need to supply an address in a sector to erase\n");
    }
}

const TermCMD::cmd_list_t flashCommands[] =
{
      {"SPI_FLASH"    ,0,0,0},
      {"flrd", "<addr> <num>",       "Read num bytes from serial flash at addr",       SpiFlash::debugRead},
      {"flwr", "<addr> <num> <val>", "Write num bytes of val to serial flash at addr", SpiFlash::debugWrite},
      {"fle",  "<addr>",             "Erase serial flash sector containing addr",      SpiFlash::debugErase},
      {0, 0, 0},
};

