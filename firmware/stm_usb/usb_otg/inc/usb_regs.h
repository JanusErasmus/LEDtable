#ifndef __USB_OTG_REGS_H__
#define __USB_OTG_REGS_H__

#include <cyg/kernel/diag.h>

#include "../../usb_dev/inc/usb_conf.h"

#define USB_OTG_HS_BASE_ADDR                 0x40040000
#define USB_OTG_FS_BASE_ADDR                 0x50000000

#define USB_OTG_CORE_GLOBAL_REGS_OFFSET      0x000
#define USB_OTG_DEV_GLOBAL_REG_OFFSET        0x800
#define USB_OTG_DEV_IN_EP_REG_OFFSET         0x900
#define USB_OTG_EP_REG_OFFSET                0x20
#define USB_OTG_DEV_OUT_EP_REG_OFFSET        0xB00
#define USB_OTG_HOST_GLOBAL_REG_OFFSET       0x400
#define USB_OTG_HOST_PORT_REGS_OFFSET        0x440
#define USB_OTG_HOST_CHAN_REGS_OFFSET        0x500
#define USB_OTG_CHAN_REGS_OFFSET             0x20
#define USB_OTG_PCGCCTL_OFFSET               0xE00
#define USB_OTG_DATA_FIFO_OFFSET             0x1000
#define USB_OTG_DATA_FIFO_SIZE               0x1000


#define USB_OTG_MAX_TX_FIFOS                 15

#define USB_OTG_HS_MAX_PACKET_SIZE           512
#define USB_OTG_FS_MAX_PACKET_SIZE           64
#define USB_OTG_MAX_EP0_SIZE                 64

typedef struct _USB_OTG_GREGS  //000h
{
  volatile cyg_uint32 GOTGCTL;      /* USB_OTG Control and Status Register    000h*/
  volatile cyg_uint32 GOTGINT;      /* USB_OTG Interrupt Register             004h*/
  volatile cyg_uint32 GAHBCFG;      /* Core AHB Configuration Register    008h*/
  volatile cyg_uint32 GUSBCFG;      /* Core USB Configuration Register    00Ch*/
  volatile cyg_uint32 GRSTCTL;      /* Core Reset Register                010h*/
  volatile cyg_uint32 GINTSTS;      /* Core Interrupt Register            014h*/
  volatile cyg_uint32 GINTMSK;      /* Core Interrupt Mask Register       018h*/
  volatile cyg_uint32 GRXSTSR;      /* Receive Sts Q Read Register        01Ch*/
  volatile cyg_uint32 GRXSTSP;      /* Receive Sts Q Read & POP Register  020h*/
  volatile cyg_uint32 GRXFSIZ;      /* Receive FIFO Size Register         024h*/
  volatile cyg_uint32 DIEPTXF0_HNPTXFSIZ;   /* EP0 / Non Periodic Tx FIFO Size Register 028h*/
  volatile cyg_uint32 HNPTXSTS;     /* Non Periodic Tx FIFO/Queue Sts reg 02Ch*/
  volatile cyg_uint32 GI2CCTL;      /* I2C Access Register                030h*/
  cyg_uint32 Reserved34;  /* PHY Vendor Control Register        034h*/
  volatile cyg_uint32 GCCFG;        /* General Purpose IO Register        038h*/
  volatile cyg_uint32 CID;          /* User ID Register                   03Ch*/
  cyg_uint32  Reserved40[48];   /* Reserved                      040h-0FFh*/
  volatile cyg_uint32 HPTXFSIZ; /* Host Periodic Tx FIFO Size Reg     100h*/
  volatile cyg_uint32 DIEPTXF[USB_OTG_MAX_TX_FIFOS];/* dev Periodic Transmit FIFO */
}
USB_OTG_GREGS;
/**
  * @}
  */


/** @defgroup __device_Registers
  * @{
  */
typedef struct _USB_OTG_DREGS // 800h
{
  volatile cyg_uint32 DCFG;         /* dev Configuration Register   800h*/
  volatile cyg_uint32 DCTL;         /* dev Control Register         804h*/
  volatile cyg_uint32 DSTS;         /* dev Status Register (RO)     808h*/
  cyg_uint32 Reserved0C;           /* Reserved                     80Ch*/
  volatile cyg_uint32 DIEPMSK;   /* dev IN Endpoint Mask         810h*/
  volatile cyg_uint32 DOEPMSK;  /* dev OUT Endpoint Mask        814h*/
  volatile cyg_uint32 DAINT;     /* dev All Endpoints Itr Reg    818h*/
  volatile cyg_uint32 DAINTMSK; /* dev All Endpoints Itr Mask   81Ch*/
  cyg_uint32  Reserved20;          /* Reserved                     820h*/
  cyg_uint32 Reserved9;       /* Reserved                     824h*/
  volatile cyg_uint32 DVBUSDIS;    /* dev VBUS discharge Register  828h*/
  volatile cyg_uint32 DVBUSPULSE;  /* dev VBUS Pulse Register      82Ch*/
  volatile cyg_uint32 DTHRCTL;     /* dev thr                      830h*/
  volatile cyg_uint32 DIEPEMPMSK; /* dev empty msk             834h*/
  volatile cyg_uint32 DEACHINT;    /* dedicated EP interrupt       838h*/
  volatile cyg_uint32 DEACHMSK;    /* dedicated EP msk             83Ch*/
  cyg_uint32 Reserved40;      /* dedicated EP mask           840h*/
  volatile cyg_uint32 DINEP1MSK;  /* dedicated EP mask           844h*/
  cyg_uint32  Reserved44[15];      /* Reserved                 844-87Ch*/
  volatile cyg_uint32 DOUTEP1MSK; /* dedicated EP msk            884h*/
}
USB_OTG_DREGS;
/**
  * @}
  */


/** @defgroup __IN_Endpoint-Specific_Register
  * @{
  */
typedef struct _USB_OTG_INEPREGS
{
  volatile cyg_uint32 DIEPCTL; /* dev IN Endpoint Control Reg 900h + (ep_num * 20h) + 00h*/
  cyg_uint32 Reserved04;             /* Reserved                       900h + (ep_num * 20h) + 04h*/
  volatile cyg_uint32 DIEPINT; /* dev IN Endpoint Itr Reg     900h + (ep_num * 20h) + 08h*/
  cyg_uint32 Reserved0C;             /* Reserved                       900h + (ep_num * 20h) + 0Ch*/
  volatile cyg_uint32 DIEPTSIZ; /* IN Endpoint Txfer Size   900h + (ep_num * 20h) + 10h*/
  volatile cyg_uint32 DIEPDMA; /* IN Endpoint DMA Address Reg    900h + (ep_num * 20h) + 14h*/
  volatile cyg_uint32 DTXFSTS;/*IN Endpoint Tx FIFO Status Reg 900h + (ep_num * 20h) + 18h*/
  cyg_uint32 Reserved18;             /* Reserved  900h+(ep_num*20h)+1Ch-900h+ (ep_num * 20h) + 1Ch*/
}
USB_OTG_INEPREGS;
/**
  * @}
  */


/** @defgroup __OUT_Endpoint-Specific_Registers
  * @{
  */
typedef struct _USB_OTG_OUTEPREGS
{
  volatile cyg_uint32 DOEPCTL;       /* dev OUT Endpoint Control Reg  B00h + (ep_num * 20h) + 00h*/
  volatile cyg_uint32 DOUTEPFRM;   /* dev OUT Endpoint Frame number B00h + (ep_num * 20h) + 04h*/
  volatile cyg_uint32 DOEPINT;              /* dev OUT Endpoint Itr Reg      B00h + (ep_num * 20h) + 08h*/
  cyg_uint32 Reserved0C;                    /* Reserved                         B00h + (ep_num * 20h) + 0Ch*/
  volatile cyg_uint32 DOEPTSIZ; /* dev OUT Endpoint Txfer Size   B00h + (ep_num * 20h) + 10h*/
  volatile cyg_uint32 DOEPDMA;              /* dev OUT Endpoint DMA Address  B00h + (ep_num * 20h) + 14h*/
  cyg_uint32 Reserved18[2];                 /* Reserved B00h + (ep_num * 20h) + 18h - B00h + (ep_num * 20h) + 1Ch*/
}
USB_OTG_OUTEPREGS;
/**
  * @}
  */


/** @defgroup __Host_Mode_Register_Structures
  * @{
  */
typedef struct _USB_OTG_HREGS
{
  volatile cyg_uint32 HCFG;             /* Host Configuration Register    400h*/
  volatile cyg_uint32 HFIR;      /* Host Frame Interval Register   404h*/
  volatile cyg_uint32 HFNUM;         /* Host Frame Nbr/Frame Remaining 408h*/
  cyg_uint32 Reserved40C;                   /* Reserved                       40Ch*/
  volatile cyg_uint32 HPTXSTS;   /* Host Periodic Tx FIFO/ Queue Status 410h*/
  volatile cyg_uint32 HAINT;   /* Host All Channels Interrupt Register 414h*/
  volatile cyg_uint32 HAINTMSK;   /* Host All Channels Interrupt Mask 418h*/
}
USB_OTG_HREGS;
/**
  * @}
  */


/** @defgroup __Host_Channel_Specific_Registers
  * @{
  */
typedef struct _USB_OTG_HC_REGS
{
  volatile cyg_uint32 HCCHAR;
  volatile cyg_uint32 HCSPLT;
  volatile cyg_uint32 HCINT;
  volatile cyg_uint32 HCGINTMSK;
  volatile cyg_uint32 HCTSIZ;
  volatile cyg_uint32 HCDMA;
  cyg_uint32 Reserved[2];
}
USB_OTG_HC_REGS;
/**
  * @}
  */


/** @defgroup __otg_Core_registers
  * @{
  */
typedef struct USB_OTG_core_regs //000h
{
  USB_OTG_GREGS         *GREGS;
  USB_OTG_DREGS         *DREGS;
  USB_OTG_HREGS         *HREGS;
  USB_OTG_INEPREGS      *INEP_REGS[USB_OTG_MAX_TX_FIFOS];
  USB_OTG_OUTEPREGS     *OUTEP_REGS[USB_OTG_MAX_TX_FIFOS];
  USB_OTG_HC_REGS       *HC_REGS[USB_OTG_MAX_TX_FIFOS];
  volatile cyg_uint32         *HPRT0;
  volatile cyg_uint32         *DFIFO[USB_OTG_MAX_TX_FIFOS];
  volatile cyg_uint32         *PCGCCTL;
}
USB_OTG_CORE_REGS , *PUSB_OTG_CORE_REGS;
typedef union _USB_OTG_OTGCTL_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 sesreqscs :
    1;
cyg_uint32 sesreq :
    1;
cyg_uint32 Reserved2_7 :
    6;
cyg_uint32 hstnegscs :
    1;
cyg_uint32 hnpreq :
    1;
cyg_uint32 hstsethnpen :
    1;
cyg_uint32 devhnpen :
    1;
cyg_uint32 Reserved12_15 :
    4;
cyg_uint32 conidsts :
    1;
cyg_uint32 Reserved17 :
    1;
cyg_uint32 asesvld :
    1;
cyg_uint32 bsesvld :
    1;
cyg_uint32 currmod :
    1;
cyg_uint32 Reserved21_31 :
    11;
  }
  b;
} USB_OTG_OTGCTL_TypeDef ;
typedef union _USB_OTG_GOTGINT_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 Reserved0_1 :
    2;
cyg_uint32 sesenddet :
    1;
cyg_uint32 Reserved3_7 :
    5;
cyg_uint32 sesreqsucstschng :
    1;
cyg_uint32 hstnegsucstschng :
    1;
cyg_uint32 reserver10_16 :
    7;
cyg_uint32 hstnegdet :
    1;
cyg_uint32 adevtoutchng :
    1;
cyg_uint32 debdone :
    1;
cyg_uint32 Reserved31_20 :
    12;
  }
  b;
} USB_OTG_GOTGINT_TypeDef ;
typedef union _USB_OTG_GAHBCFG_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 glblintrmsk :
    1;
cyg_uint32 hburstlen :
    4;
cyg_uint32 dmaenable :
    1;
cyg_uint32 Reserved :
    1;
cyg_uint32 nptxfemplvl_txfemplvl :
    1;
cyg_uint32 ptxfemplvl :
    1;
cyg_uint32 Reserved9_31 :
    23;
  }
  b;
} USB_OTG_GAHBCFG_TypeDef ;
typedef union _USB_OTG_GUSBCFG_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 toutcal :
    3;
cyg_uint32 phyif :
    1;
cyg_uint32 ulpi_utmi_sel :
    1;
cyg_uint32 fsintf :
    1;
cyg_uint32 physel :
    1;
cyg_uint32 ddrsel :
    1;
cyg_uint32 srpcap :
    1;
cyg_uint32 hnpcap :
    1;
cyg_uint32 usbtrdtim :
    4;
cyg_uint32 nptxfrwnden :
    1;
cyg_uint32 phylpwrclksel :
    1;
cyg_uint32 otgutmifssel :
    1;
cyg_uint32 ulpi_fsls :
    1;
cyg_uint32 ulpi_auto_res :
    1;
cyg_uint32 ulpi_clk_sus_m :
    1;
cyg_uint32 ulpi_ext_vbus_drv :
    1;
cyg_uint32 ulpi_int_vbus_indicator :
    1;
cyg_uint32 term_sel_dl_pulse :
    1;
cyg_uint32 Reserved :
    6;
cyg_uint32 force_host :
    1;
cyg_uint32 force_dev :
    1;
cyg_uint32 corrupt_tx :
    1;
  }
  b;
} USB_OTG_GUSBCFG_TypeDef ;
typedef union _USB_OTG_GRSTCTL_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 csftrst :
    1;
cyg_uint32 hsftrst :
    1;
cyg_uint32 hstfrm :
    1;
cyg_uint32 intknqflsh :
    1;
cyg_uint32 rxfflsh :
    1;
cyg_uint32 txfflsh :
    1;
cyg_uint32 txfnum :
    5;
cyg_uint32 Reserved11_29 :
    19;
cyg_uint32 dmareq :
    1;
cyg_uint32 ahbidle :
    1;
  }
  b;
} USB_OTG_GRSTCTL_TypeDef ;
typedef union _USB_OTG_GINTMSK_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 Reserved0 :
    1;
cyg_uint32 modemismatch :
    1;
cyg_uint32 otgintr :
    1;
cyg_uint32 sofintr :
    1;
cyg_uint32 rxstsqlvl :
    1;
cyg_uint32 nptxfempty :
    1;
cyg_uint32 ginnakeff :
    1;
cyg_uint32 goutnakeff :
    1;
cyg_uint32 Reserved8 :
    1;
cyg_uint32 i2cintr :
    1;
cyg_uint32 erlysuspend :
    1;
cyg_uint32 usbsuspend :
    1;
cyg_uint32 usbreset :
    1;
cyg_uint32 enumdone :
    1;
cyg_uint32 isooutdrop :
    1;
cyg_uint32 eopframe :
    1;
cyg_uint32 Reserved16 :
    1;
cyg_uint32 epmismatch :
    1;
cyg_uint32 inepintr :
    1;
cyg_uint32 outepintr :
    1;
cyg_uint32 incomplisoin :
    1;
cyg_uint32 incomplisoout :
    1;
cyg_uint32 Reserved22_23 :
    2;
cyg_uint32 portintr :
    1;
cyg_uint32 hcintr :
    1;
cyg_uint32 ptxfempty :
    1;
cyg_uint32 Reserved27 :
    1;
cyg_uint32 conidstschng :
    1;
cyg_uint32 disconnect :
    1;
cyg_uint32 sessreqintr :
    1;
cyg_uint32 wkupintr :
    1;
  }
  b;
} USB_OTG_GINTMSK_TypeDef ;
typedef union _USB_OTG_GINTSTS_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 curmode :
    1;
cyg_uint32 modemismatch :
    1;
cyg_uint32 otgintr :
    1;
cyg_uint32 sofintr :
    1;
cyg_uint32 rxstsqlvl :
    1;
cyg_uint32 nptxfempty :
    1;
cyg_uint32 ginnakeff :
    1;
cyg_uint32 goutnakeff :
    1;
cyg_uint32 Reserved8 :
    1;
cyg_uint32 i2cintr :
    1;
cyg_uint32 erlysuspend :
    1;
cyg_uint32 usbsuspend :
    1;
cyg_uint32 usbreset :
    1;
cyg_uint32 enumdone :
    1;
cyg_uint32 isooutdrop :
    1;
cyg_uint32 eopframe :
    1;
cyg_uint32 intimerrx :
    1;
cyg_uint32 epmismatch :
    1;
cyg_uint32 inepint:
    1;
cyg_uint32 outepintr :
    1;
cyg_uint32 incomplisoin :
    1;
cyg_uint32 incomplisoout :
    1;
cyg_uint32 Reserved22_23 :
    2;
cyg_uint32 portintr :
    1;
cyg_uint32 hcintr :
    1;
cyg_uint32 ptxfempty :
    1;
cyg_uint32 Reserved27 :
    1;
cyg_uint32 conidstschng :
    1;
cyg_uint32 disconnect :
    1;
cyg_uint32 sessreqintr :
    1;
cyg_uint32 wkupintr :
    1;
  }
  b;
} USB_OTG_GINTSTS_TypeDef ;
typedef union _USB_OTG_DRXSTS_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 epnum :
    4;
cyg_uint32 bcnt :
    11;
cyg_uint32 dpid :
    2;
cyg_uint32 pktsts :
    4;
cyg_uint32 fn :
    4;
cyg_uint32 Reserved :
    7;
  }
  b;
} USB_OTG_DRXSTS_TypeDef ;
typedef union _USB_OTG_GRXSTS_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 chnum :
    4;
cyg_uint32 bcnt :
    11;
cyg_uint32 dpid :
    2;
cyg_uint32 pktsts :
    4;
cyg_uint32 Reserved :
    11;
  }
  b;
} USB_OTG_GRXFSTS_TypeDef ;
typedef union _USB_OTG_FSIZ_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 startaddr :
    16;
cyg_uint32 depth :
    16;
  }
  b;
} USB_OTG_FSIZ_TypeDef ;
typedef union _USB_OTG_HNPTXSTS_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 nptxfspcavail :
    16;
cyg_uint32 nptxqspcavail :
    8;
cyg_uint32 nptxqtop_terminate :
    1;
cyg_uint32 nptxqtop_timer :
    2;
cyg_uint32 nptxqtop :
    2;
cyg_uint32 chnum :
    2;    
cyg_uint32 Reserved :
    1;
  }
  b;
} USB_OTG_HNPTXSTS_TypeDef ;
typedef union _USB_OTG_DTXFSTSn_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 txfspcavail :
    16;
cyg_uint32 Reserved :
    16;
  }
  b;
} USB_OTG_DTXFSTSn_TypeDef ;
typedef union _USB_OTG_GI2CCTL_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 rwdata :
    8;
cyg_uint32 regaddr :
    8;
cyg_uint32 addr :
    7;
cyg_uint32 i2cen :
    1;
cyg_uint32 ack :
    1;
cyg_uint32 i2csuspctl :
    1;
cyg_uint32 i2cdevaddr :
    2;
cyg_uint32 dat_se0:
    1;
cyg_uint32 Reserved :
    1;
cyg_uint32 rw :
    1;
cyg_uint32 bsydne :
    1;
  }
  b;
} USB_OTG_GI2CCTL_TypeDef ;
typedef union _USB_OTG_GCCFG_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 Reserved_in :
    16;
cyg_uint32 pwdn :
    1;
cyg_uint32 i2cifen :
    1;
cyg_uint32 vbussensingA :
    1;
cyg_uint32 vbussensingB :
    1;
cyg_uint32 sofouten :
    1;
cyg_uint32 disablevbussensing :
    1;
cyg_uint32 Reserved_out :
    10;
  }
  b;
} USB_OTG_GCCFG_TypeDef ;

typedef union _USB_OTG_DCFG_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 devspd :
    2;
cyg_uint32 nzstsouthshk :
    1;
cyg_uint32 Reserved3 :
    1;
cyg_uint32 devaddr :
    7;
cyg_uint32 perfrint :
    2;
cyg_uint32 Reserved13_17 :
    5;
cyg_uint32 epmscnt :
    4;
  }
  b;
} USB_OTG_DCFG_TypeDef ;
typedef union _USB_OTG_DCTL_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 rmtwkupsig :
    1;
cyg_uint32 sftdiscon :
    1;
cyg_uint32 gnpinnaksts :
    1;
cyg_uint32 goutnaksts :
    1;
cyg_uint32 tstctl :
    3;
cyg_uint32 sgnpinnak :
    1;
cyg_uint32 cgnpinnak :
    1;
cyg_uint32 sgoutnak :
    1;
cyg_uint32 cgoutnak :
    1;
cyg_uint32 Reserved :
    21;
  }
  b;
} USB_OTG_DCTL_TypeDef ;
typedef union _USB_OTG_DSTS_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 suspsts :
    1;
cyg_uint32 enumspd :
    2;
cyg_uint32 errticerr :
    1;
cyg_uint32 Reserved4_7:
    4;
cyg_uint32 soffn :
    14;
cyg_uint32 Reserved22_31 :
    10;
  }
  b;
} USB_OTG_DSTS_TypeDef ;
typedef union _USB_OTG_DIEPINTn_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 xfercompl :
    1;
cyg_uint32 epdisabled :
    1;
cyg_uint32 ahberr :
    1;
cyg_uint32 timeout :
    1;
cyg_uint32 intktxfemp :
    1;
cyg_uint32 intknepmis :
    1;
cyg_uint32 inepnakeff :
    1;
cyg_uint32 emptyintr :
    1;
cyg_uint32 txfifoundrn :
    1;
cyg_uint32 Reserved08_31 :
    23;
  }
  b;
} USB_OTG_DIEPINTn_TypeDef ;
typedef union _USB_OTG_DIEPINTn_TypeDef   USB_OTG_DIEPMSK_TypeDef ;
typedef union _USB_OTG_DOEPINTn_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 xfercompl :
    1;
cyg_uint32 epdisabled :
    1;
cyg_uint32 ahberr :
    1;
cyg_uint32 setup :
    1;
cyg_uint32 Reserved04_31 :
    28;
  }
  b;
} USB_OTG_DOEPINTn_TypeDef ;
typedef union _USB_OTG_DOEPINTn_TypeDef   USB_OTG_DOEPMSK_TypeDef ;

typedef union _USB_OTG_DAINT_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 in :
    16;
cyg_uint32 out :
    16;
  }
  ep;
} USB_OTG_DAINT_TypeDef ;

typedef union _USB_OTG_DTHRCTL_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 non_iso_thr_en :
    1;
cyg_uint32 iso_thr_en :
    1;
cyg_uint32 tx_thr_len :
    9;
cyg_uint32 Reserved11_15 :
    5;
cyg_uint32 rx_thr_en :
    1;
cyg_uint32 rx_thr_len :
    9;
cyg_uint32 Reserved26_31 :
    6;
  }
  b;
} USB_OTG_DTHRCTL_TypeDef ;
typedef union _USB_OTG_DEPCTL_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 mps :
    11;
cyg_uint32 reserved :
    4;
cyg_uint32 usbactep :
    1;
cyg_uint32 dpid :
    1;
cyg_uint32 naksts :
    1;
cyg_uint32 eptype :
    2;
cyg_uint32 snp :
    1;
cyg_uint32 stall :
    1;
cyg_uint32 txfnum :
    4;
cyg_uint32 cnak :
    1;
cyg_uint32 snak :
    1;
cyg_uint32 setd0pid :
    1;
cyg_uint32 setd1pid :
    1;
cyg_uint32 epdis :
    1;
cyg_uint32 epena :
    1;
  }
  b;
} USB_OTG_DEPCTL_TypeDef ;
typedef union _USB_OTG_DEPXFRSIZ_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 xfersize :
    19;
cyg_uint32 pktcnt :
    10;
cyg_uint32 mc :
    2;
cyg_uint32 Reserved :
    1;
  }
  b;
} USB_OTG_DEPXFRSIZ_TypeDef ;
typedef union _USB_OTG_DEP0XFRSIZ_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 xfersize :
    7;
cyg_uint32 Reserved7_18 :
    12;
cyg_uint32 pktcnt :
    2;
cyg_uint32 Reserved20_28 :
    9;
cyg_uint32 supcnt :
    2;
    cyg_uint32 Reserved31;
  }
  b;
} USB_OTG_DEP0XFRSIZ_TypeDef ;
typedef union _USB_OTG_HCFG_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 fslspclksel :
    2;
cyg_uint32 fslssupp :
    1;
  }
  b;
} USB_OTG_HCFG_TypeDef ;
typedef union _USB_OTG_HFRMINTRVL_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 frint :
    16;
cyg_uint32 Reserved :
    16;
  }
  b;
} USB_OTG_HFRMINTRVL_TypeDef ;

typedef union _USB_OTG_HFNUM_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 frnum :
    16;
cyg_uint32 frrem :
    16;
  }
  b;
} USB_OTG_HFNUM_TypeDef ;
typedef union _USB_OTG_HPTXSTS_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 ptxfspcavail :
    16;
cyg_uint32 ptxqspcavail :
    8;
cyg_uint32 ptxqtop_terminate :
    1;
cyg_uint32 ptxqtop_timer :
    2;
cyg_uint32 ptxqtop :
    2;
cyg_uint32 chnum :
    2;
cyg_uint32 ptxqtop_odd :
    1;
  }
  b;
} USB_OTG_HPTXSTS_TypeDef ;
typedef union _USB_OTG_HPRT0_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 prtconnsts :
    1;
cyg_uint32 prtconndet :
    1;
cyg_uint32 prtena :
    1;
cyg_uint32 prtenchng :
    1;
cyg_uint32 prtovrcurract :
    1;
cyg_uint32 prtovrcurrchng :
    1;
cyg_uint32 prtres :
    1;
cyg_uint32 prtsusp :
    1;
cyg_uint32 prtrst :
    1;
cyg_uint32 Reserved9 :
    1;
cyg_uint32 prtlnsts :
    2;
cyg_uint32 prtpwr :
    1;
cyg_uint32 prttstctl :
    4;
cyg_uint32 prtspd :
    2;
cyg_uint32 Reserved19_31 :
    13;
  }
  b;
} USB_OTG_HPRT0_TypeDef ;
typedef union _USB_OTG_HAINT_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 chint :
    16;
cyg_uint32 Reserved :
    16;
  }
  b;
} USB_OTG_HAINT_TypeDef ;
typedef union _USB_OTG_HAINTMSK_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 chint :
    16;
cyg_uint32 Reserved :
    16;
  }
  b;
} USB_OTG_HAINTMSK_TypeDef ;
typedef union _USB_OTG_HCCHAR_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 mps :
    11;
cyg_uint32 epnum :
    4;
cyg_uint32 epdir :
    1;
cyg_uint32 Reserved :
    1;
cyg_uint32 lspddev :
    1;
cyg_uint32 eptype :
    2;
cyg_uint32 multicnt :
    2;
cyg_uint32 devaddr :
    7;
cyg_uint32 oddfrm :
    1;
cyg_uint32 chdis :
    1;
cyg_uint32 chen :
    1;
  }
  b;
} USB_OTG_HCCHAR_TypeDef ;
typedef union _USB_OTG_HCSPLT_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 prtaddr :
    7;
cyg_uint32 hubaddr :
    7;
cyg_uint32 xactpos :
    2;
cyg_uint32 compsplt :
    1;
cyg_uint32 Reserved :
    14;
cyg_uint32 spltena :
    1;
  }
  b;
} USB_OTG_HCSPLT_TypeDef ;
typedef union _USB_OTG_HCINTn_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 xfercompl :
    1;
cyg_uint32 chhltd :
    1;
cyg_uint32 ahberr :
    1;
cyg_uint32 stall :
    1;
cyg_uint32 nak :
    1;
cyg_uint32 ack :
    1;
cyg_uint32 nyet :
    1;
cyg_uint32 xacterr :
    1;
cyg_uint32 bblerr :
    1;
cyg_uint32 frmovrun :
    1;
cyg_uint32 datatglerr :
    1;
cyg_uint32 Reserved :
    21;
  }
  b;
} USB_OTG_HCINTn_TypeDef ;
typedef union _USB_OTG_HCTSIZn_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 xfersize :
    19;
cyg_uint32 pktcnt :
    10;
cyg_uint32 pid :
    2;
cyg_uint32 dopng :
    1;
  }
  b;
} USB_OTG_HCTSIZn_TypeDef ;
typedef union _USB_OTG_HCGINTMSK_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 xfercompl :
    1;
cyg_uint32 chhltd :
    1;
cyg_uint32 ahberr :
    1;
cyg_uint32 stall :
    1;
cyg_uint32 nak :
    1;
cyg_uint32 ack :
    1;
cyg_uint32 nyet :
    1;
cyg_uint32 xacterr :
    1;
cyg_uint32 bblerr :
    1;
cyg_uint32 frmovrun :
    1;
cyg_uint32 datatglerr :
    1;
cyg_uint32 Reserved :
    21;
  }
  b;
} USB_OTG_HCGINTMSK_TypeDef ;
typedef union _USB_OTG_PCGCCTL_TypeDef 
{
  cyg_uint32 d32;
  struct
  {
cyg_uint32 stoppclk :
    1;
cyg_uint32 gatehclk :
    1;
cyg_uint32 Reserved :
    30;
  }
  b;
} USB_OTG_PCGCCTL_TypeDef ;

/**
  * @}
  */ 


/** @defgroup USB_REGS_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_REGS_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_REGS_Exported_FunctionsPrototype
  * @{
  */ 
/**
  * @}
  */ 


#endif //__USB_OTG_REGS_H__


/**
  * @}
  */ 

/**
  * @}
  */ 
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

