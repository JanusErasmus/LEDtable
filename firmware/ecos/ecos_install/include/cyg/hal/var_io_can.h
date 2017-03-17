#ifndef CYGONCE_HAL_VAR_IO_ETH_H
#define CYGONCE_HAL_VAR_IO_ETH_H
//=============================================================================
//
//      var_io_can.h
//
//      CAN-specific variant definitions
//
//=============================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2008, 2009, 2011 Free Software Foundation, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later
// version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License
// along with eCos; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
// As a special exception, if other files instantiate templates or use
// macros or inline functions from this file, or you compile this file
// and link it with other works to produce a work based on this file,
// this file does not by itself cause the resulting work to be covered by
// the GNU General Public License. However the source code for this file
// must still be made available in accordance with section (3) of the GNU
// General Public License v2.
//
// This exception does not invalidate any other reasons why a work based
// on this file might be covered by the GNU General Public License.
// -------------------------------------------
// ####ECOSGPLCOPYRIGHTEND####
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):   janus
// Date:        2015-08-24
// Purpose:     STM32 variant CAN specific registers
// Description:
// Usage:       Do not include this header file directly. Instead:
//              #include <cyg/hal/var_io.h>
//
//####DESCRIPTIONEND####
//
//=============================================================================

#ifndef CYGONCE_HAL_VAR_IO_H
# error Do not include var_io_eth.h directly, use var_io.h
#endif


//=============================================================================
// Ethernet CAN
//
// Connectivity devices only

#ifdef CYGHWR_HAL_CORTEXM_STM32_CONNECTIVITY

#define CYGHWR_HAL_STM32_BXCAN_MCR				0x000
#define CYGHWR_HAL_STM32_BXCAN_MSR				0x004
#define CYGHWR_HAL_STM32_BXCAN_TSR				0x008
#define CYGHWR_HAL_STM32_BXCAN_RF0R				0x00C
#define CYGHWR_HAL_STM32_BXCAN_RF1R				0x010
#define CYGHWR_HAL_STM32_BXCAN_IER				0x014
#define CYGHWR_HAL_STM32_BXCAN_ESR				0x018
#define CYGHWR_HAL_STM32_BXCAN_BTR				0x01C
#define CYGHWR_HAL_STM32_BXCAN_BTR_TS2(_reg)			((_reg >> 20) & 0x7)
#define CYGHWR_HAL_STM32_BXCAN_BTR_TS1(_reg)			((_reg >> 16) & 0xF)

//Mailbox identifier register
#define CYGHWR_HAL_STM32_BXCAN_TI0R				0x180
#define CYGHWR_HAL_STM32_BXCAN_TIxR_STID(_id)	(_id << 21)
#define CYGHWR_HAL_STM32_BXCAN_TIxR_EXID(_id)	(_id <<  3)
#define CYGHWR_HAL_STM32_BXCAN_TIxR_IDE			(1 << 2)
#define CYGHWR_HAL_STM32_BXCAN_TIxR_RTR			(1 << 1)
#define CYGHWR_HAL_STM32_BXCAN_TIxR_TXRQ		(1 << 0)

//Mailbox data length control and time stamp register
#define CYGHWR_HAL_STM32_BXCAN_TDT0R			0x184
#define CYGHWR_HAL_STM32_BXCAN_TDTxR_TIME(_tm)	(_tm << 16)
#define CYGHWR_HAL_STM32_BXCAN_TDTxR_TGT		(1 << 8)
#define CYGHWR_HAL_STM32_BXCAN_TDTxR_DLC(_len)	(_len & 0xF)

//Mailbox data low register
#define CYGHWR_HAL_STM32_BXCAN_TDL0R			0x188
//Mailbox data high register
#define CYGHWR_HAL_STM32_BXCAN_TDH0R			0x18C
#define CYGHWR_HAL_STM32_BXCAN_TI1R				0x190
#define CYGHWR_HAL_STM32_BXCAN_TDT1R			0x194
#define CYGHWR_HAL_STM32_BXCAN_TDL1R			0x198
#define CYGHWR_HAL_STM32_BXCAN_TDH1R			0x19C
#define CYGHWR_HAL_STM32_BXCAN_TI2R				0x1A0
#define CYGHWR_HAL_STM32_BXCAN_TDT2R			0x1A4
#define CYGHWR_HAL_STM32_BXCAN_TDL2R			0x1A8
#define CYGHWR_HAL_STM32_BXCAN_TDH2R			0x1AC

//Receive FIFO mailbox identifier register
#define CYGHWR_HAL_STM32_BXCAN_RI0R				0x1B0
#define CYGHWR_HAL_STM32_BXCAN_RDT0R			0x1B4
#define CYGHWR_HAL_STM32_BXCAN_RDL0R			0x1B8
#define CYGHWR_HAL_STM32_BXCAN_RDH0R			0x1BC
#define CYGHWR_HAL_STM32_BXCAN_RI1R				0x1C0
#define CYGHWR_HAL_STM32_BXCAN_RDT1R			0x1C4
#define CYGHWR_HAL_STM32_BXCAN_RDL1R			0x1C8
#define CYGHWR_HAL_STM32_BXCAN_RDH1R			0x1CC
#define CYGHWR_HAL_STM32_BXCAN_FMR				0x200
#define CYGHWR_HAL_STM32_BXCAN_FM1R				0x204
#define CYGHWR_HAL_STM32_BXCAN_FS1R				0x20C
#define CYGHWR_HAL_STM32_BXCAN_FFA1R			0x214
#define CYGHWR_HAL_STM32_BXCAN_FA1R				0x21C
#define CYGHWR_HAL_STM32_BXCAN_FxR1(_x) 		0x240 + (_x * 8)
#define CYGHWR_HAL_STM32_BXCAN_FxR2(_x) 		0x244 + (_x * 8)



// GPIO pins

// NOTE: The platform specific (re-)mapping of pins is provided in the relevant
// target specific "plf_io.h" header file. These definitions just cover the
// fixed mappings.

// CAN1
#define CYGHWR_HAL_STM32_CAN1_RX                CYGHWR_HAL_STM32_PIN_ALTFN_IN(  D,  0,  9,  NA,       FLOATING)
#define CYGHWR_HAL_STM32_CAN1_TX                CYGHWR_HAL_STM32_PIN_ALTFN_OUT( D,  1,  9, PUSHPULL, NONE, 2MHZ)

// CAN2
#define CYGHWR_HAL_STM32_CAN2_RX                CYGHWR_HAL_STM32_PIN_ALTFN_IN(  B,  5,  9,  NA,       FLOATING)
#define CYGHWR_HAL_STM32_CAN2_TX                CYGHWR_HAL_STM32_PIN_ALTFN_OUT( B,  6,  9, PUSHPULL, NONE, 2MHZ)

// Clock controls
#define CYGHWR_HAL_STM32_CAN1_CLOCK             CYGHWR_HAL_STM32_CLOCK( APB1, CAN1 )
#define CYGHWR_HAL_STM32_CAN2_CLOCK             CYGHWR_HAL_STM32_CLOCK( APB1, CAN2 )

#endif // CYGHWR_HAL_CORTEXM_STM32_CONNECTIVITY

#endif // CYGONCE_HAL_VAR_IO_ETH_H
//-----------------------------------------------------------------------------
// end of var_io_eth.h
