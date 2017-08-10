#ifndef BLUENRG_H_
#define BLUENRG_H_
#include <cyg/kernel/kapi.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/io/spi.h>

extern "C" {
#include <bluenrg_gatt_aci.h>
};

#include <bluenrg_service.h>

class cBlueNRGstateListener
{
public:
   cBlueNRGstateListener(){};
   virtual ~cBlueNRGstateListener(){};

   virtual void setState(bool ConnectionState) = 0;
};

class cBlueNRG
{
   static cBlueNRG *__instance;
   cBlueNRGserviceNode *mServiceList;

   cBlueNRGstateListener *mStateListener;

   volatile uint16_t connection_handle;
   volatile cyg_bool mReady;
   cyg_uint8 * mBDAddress;
   const char *mBLEname;

   cBlueNRG(cyg_uint8 * bdAddress, const char *BLEname, cyg_spi_device *dev, cyg_uint32 extiPin, cyg_uint32 resetPin);
   virtual ~cBlueNRG();

   cyg_uint8 bnrg_expansion_board;

   cyg_bool mConnected;
   cyg_uint8 mConnectable;
   cyg_uint8 mBLEStack[CYGNUM_HAL_STACK_SIZE_MINIMUM];
   cyg_thread mBLEThread;
   cyg_handle_t mBLEThreadHandle;
   static void ble_thread(cyg_addrword_t args);

   cyg_uint8 mStack[CYGNUM_HAL_STACK_SIZE_TYPICAL];
   cyg_thread mThread;
   cyg_handle_t mThreadHandle;
   static void device_thread(cyg_addrword_t args);
   void run();

   static void bleReady();

   void attributeModified(uint16_t handle, uint8_t data_length, uint8_t *att_data);
   void attributeWrite(uint16_t handle, uint8_t data_length, uint8_t *att_data);
   void attributeRead(evt_gatt_read_permit_req *pr);

   void disconnected();

public:
   static cBlueNRG *init(cyg_uint8 * bdAddress, const char *BLEname, cyg_spi_device *dev, cyg_uint32 extiPin, cyg_uint32 resetPin);
   static cBlueNRG *get(){ return __instance; };
   cyg_bool ready(){ return mReady; };
   cyg_bool connected(){ return mConnected; };
   void setStateListener(cBlueNRGstateListener *listener){ mStateListener = listener; };

   void handleHCI_CB(void *pckt);

   void setup(cyg_spi_device *dev, cyg_uint32 extiPin, cyg_uint32 resetPin);
   void initConnection();

   void setConnectable(void);

   cyg_bool addService(cBlueNRGservice * service, uint8_t att_count);

   void listServices();
};

#endif /* BLUENRG_H_ */
