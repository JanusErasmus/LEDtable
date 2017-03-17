#ifndef CYGONCE_I2C_STM32_H
#define CYGONCE_I2C_STM32_H
//==========================================================================
//
//      devs/i2c/cortex/stm32/current/src/i2c_stm32.h
//
//      I2C driver for STM CORTEX processors
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 2008 Free Software Foundation, Inc.                        
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
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):     Janus Erasmus
// Contributors:  
// Date:          2011-10-10
// Description:   I2C driver for STM32
//####DESCRIPTIONEND####
//==========================================================================


//==========================================================================
//                               INCLUDES
//==========================================================================
#include <pkgconf/devs_i2c_cortexm_stm32.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/hal/drv_api.h>


//==========================================================================
// Single I2C bus sepecififc data
//==========================================================================
typedef struct cyg_stm32_i2c_extra {
#ifdef CYGHWR_DEVS_I2C_CORTEXM_STM32_MULTIPLE_BUSES    
    // Put statically initialized fields first.
    cyg_uint32       i2c_base;    // Per-bus h/w details
    cyg_vector_t     i2c_isrvec;
    int              i2c_isrpri;
    cyg_uint32       i2c_pclk;    // peripheral clock of 
    cyg_uint32       i2c_bus_freq;// I2C bus frequency (e.g. 100 kHz, 400 kHz)
#endif // CYGHWR_DEVS_I2C_CORTEXM_STM32_MULTIPLE_BUSES    
    
    cyg_uint8        i2c_addr;
    cyg_uint32       i2c_count;
    const cyg_uint8* i2c_txbuf;
    cyg_uint8*       i2c_rxbuf;
    cyg_bool         i2c_rxnak;

    cyg_uint32       i2c_flag;
    cyg_uint32       i2c_delay;

    cyg_drv_mutex_t  i2c_lock; // For synchronizing between DSR and foreground
    cyg_drv_cond_t   i2c_wait;
    cyg_handle_t     i2c_interrupt_handle;// For initializing the interrupt
    cyg_interrupt    i2c_interrupt_data;
} cyg_stm32_i2c_extra;


//==========================================================================
// I2C driver interface
//==========================================================================
externC void        cyg_stm32_i2c_init(struct cyg_i2c_bus*);
externC cyg_uint32  cyg_stm32_i2c_tx(const cyg_i2c_device*, 
                                       cyg_bool, const cyg_uint8*, 
                                       cyg_uint32, cyg_bool);
externC cyg_uint32  cyg_stm32_i2c_rx(const cyg_i2c_device*, 
                                       cyg_bool, cyg_uint8*, 
                                       cyg_uint32, cyg_bool, cyg_bool);
externC void        cyg_stm32_i2c_stop(const cyg_i2c_device*);

//==========================================================================
// I2C bus declaration macros
//=========================================================================
#ifdef CYGHWR_DEVS_I2C_CORTEXM_STM32_MULTIPLE_BUSES
# define CYG_STM32_I2C_BUS(_name_, _init_fn_, _base_, _isr_vec_, _isr_pri_, \
                             _pclk_, _i2c_bus_freq_)                    \
  static cyg_stm32_i2c_extra _name_ ## _extra = {                     \
  i2c_base     : _base_,                                                \
  i2c_isrvec   : _isr_vec_,                                             \
  i2c_isrpri   : _isr_pri_,                                             \
  i2c_pclk     : _pclk_,                                                \
  i2c_bus_freq : _i2c_bus_freq_,                                        \
  i2c_count    :  0,                                                    \
  i2c_txbuf    :  NULL,                                                 \
  i2c_rxbuf    :  NULL,                                                 \
  i2c_flag     :  0                                                     \
  } ;                                                                   \
  CYG_I2C_BUS(_name_,                                                   \
              _init_fn_,                                                \
              &cyg_stm32_i2c_tx,                                      \
              &cyg_stm32_i2c_rx,                                      \
              &cyg_stm32_i2c_stop,                                    \
              (void*) & ( _name_ ## _extra)) ;

#else // !CYGHWR_DEVS_I2C_CORTEXM_STM32_MULTIPLE_BUSES
# define CYG_STM32_I2C_BUS(_name_, _init_fn_, _base_, _isr_vec_, _isr_pri_, \
                             _pclk_, _i2c_bus_freq_)                    \
  static cyg_stm32_i2c_extra _name_ ## _extra = {                     \
  i2c_count    :  0,                                                    \
  i2c_txbuf    :  NULL,                                                 \
  i2c_rxbuf    :  NULL,                                                 \
  i2c_flag     :  0                                                     \
  } ;                                                                   \
  CYG_I2C_BUS(_name_,                                                   \
              _init_fn_,                                                \
              cyg_stm32_i2c_tx,                                       \
              cyg_stm32_i2c_rx,                                       \
              cyg_stm32_i2c_stop,                                     \
              (void*) & ( _name_ ## _extra)) ;
#endif // CYGHWR_DEVS_I2C_CORTEX_STM32_MULTIPLE_BUSES

//-----------------------------------------------------------------------------
#endif // #endif CYGONCE_I2C_STM32_H
