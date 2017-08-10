#include <string.h>

#include "utils.h"
#include "bluenrg.h"

extern "C" {
#include <hci.h>
#include <stm32_bluenrg_ble.h>
#include <bluenrg_utils.h>
#include <gp_timer.h>
#include <bluenrg_gatt_aci.h>
#include <bluenrg_gap.h>
#include <bluenrg_gap_aci.h>
#include <bluenrg_hal_aci.h>
#include <sm.h>

#include <hci_le.h>
#include <hci_const.h>
#include <bluenrg_aci_const.h>
};


cyg_mutex_t mBLEmutex;
cyg_cond_t mBLEcond;

#define TRACE(_x, ...) INFO_TRACE("cBlueNRG", _x,  ##__VA_ARGS__)

cBlueNRG* cBlueNRG::__instance = 0;

cBlueNRG *cBlueNRG::init(cyg_uint8 * bdAddress, const char *BLEname, cyg_spi_device *dev, cyg_uint32 extiPin, cyg_uint32 resetPin)
{
   if(!__instance)
   {
      __instance = new cBlueNRG(bdAddress, BLEname, dev, extiPin, resetPin);
   }

   return __instance;
}

cBlueNRG::cBlueNRG(cyg_uint8 * bdAddress, const char *BLEname, cyg_spi_device *dev, cyg_uint32 extiPin, cyg_uint32 resetPin)
{
   mBLEname = BLEname;
   mStateListener = 0;
   mReady = false;
   mBDAddress = bdAddress;
   bnrg_expansion_board = IDB04A1;
   mServiceList = 0;
   mConnected = 0;
   mConnectable = 0;

   cyg_mutex_init(&mBLEmutex);
   cyg_cond_init(&mBLEcond, &mBLEmutex);

   TRACE("bleReady %p\n", &mBLEcond);

   setup(dev, extiPin, resetPin);

   cyg_thread_create(12,
         ble_thread,
         (cyg_addrword_t)this,
         (char *)"BLE_ISR",
         mBLEStack,
         CYGNUM_HAL_STACK_SIZE_MINIMUM,
         &mBLEThreadHandle,
         &mBLEThread);
   cyg_thread_resume(mBLEThreadHandle);

   cyg_thread_create(16,
         device_thread,
         (cyg_addrword_t)this,
         (char *)"cBlueNRG",
         mStack,
         CYGNUM_HAL_STACK_SIZE_TYPICAL,
         &mThreadHandle,
         &mThread);
   cyg_thread_resume(mThreadHandle);

   initConnection();
}


void cBlueNRG::device_thread(cyg_addrword_t args)
{
   cBlueNRG* _this = (cBlueNRG*)args;

   while(1)
   {

      HCI_Process();

      if(_this->mConnectable)
      {
         _this->mConnectable = 0;
         cyg_thread_delay(20);
         _this->setConnectable();
      }

      cyg_thread_delay(1);
   }
}



void cBlueNRG::bleReady()
{
   //TRACE("bleReady %p\n", &mBLEcond);
   cyg_cond_signal(&mBLEcond);
}

void cBlueNRG::ble_thread(cyg_addrword_t args)
{
   while(1)
   {
      cyg_mutex_lock(&mBLEmutex);
      if(BlueNRG_DataPresent() || cyg_cond_wait(&mBLEcond))
      {
         //TRACE("HCI_Isr\n");
         HCI_Isr();
      }
      cyg_mutex_unlock(&mBLEmutex);
   }
}

/*** External shadow function called by Bluetooth on new events
 *
 */
void HCI_Event_CB(void *pckt)
{
   cBlueNRG::get()->handleHCI_CB(pckt);
}

void cBlueNRG::attributeModified(uint16_t handle, uint8_t data_length, uint8_t *att_data)
{
   //TRACE("attributeModified %d\n", handle);
   //diag_dump_buf(att_data, data_length);

   cBlueNRGserviceNode *sNode = mServiceList;
   while(sNode)
   {
      cBlueNRGcharNode *cNode = sNode->service->CharacteristicList;
      while(cNode)
      {
         if((cNode->characteristic->Handle + 1) == handle)
         {
            //TRACE("- Characteristic %02X modified\n", cNode->characteristic->UUID[15]);
            cNode->characteristic->modified(att_data, data_length);
            return;
         }

         if((cNode->characteristic->Handle + 2) == handle)
         {
            //TRACE("- Notify %02X register\n", cNode->characteristic->UUID[15]);
            cNode->characteristic->setNotification(att_data[0]);
            return;
         }
         cNode = cNode->next;
      }
      sNode = sNode->next;
   }
}

void cBlueNRG::attributeWrite(uint16_t handle, uint8_t data_length, uint8_t *att_data)
{
   //TRACE("attributeWrite %d\n", handle);
   //diag_dump_buf(att_data, data_length);

   cBlueNRGserviceNode *sNode = mServiceList;
   while(sNode)
   {
      cBlueNRGcharNode *cNode = sNode->service->CharacteristicList;
      while(cNode)
      {
         if((cNode->characteristic->Handle + 1) == handle)
         {
            //TRACE("- Characteristic %02X modified\n", cNode->characteristic->UUID[15]);
            cNode->characteristic->modified(att_data, data_length);
            return;
         }

         cNode = cNode->next;
      }
      sNode = sNode->next;
   }
}

void cBlueNRG::attributeRead(evt_gatt_read_permit_req *pr)
{
   uint16_t handle = pr->attr_handle;
   //TRACE("attributeRead %d\n", handle);
   //diag_dump_buf(att_data, data_length);

   cBlueNRGserviceNode *sNode = mServiceList;
   while(sNode)
   {
      cBlueNRGcharNode *cNode = sNode->service->CharacteristicList;
      while(cNode)
      {
         if((cNode->characteristic->Handle + 1) == handle)
         {
            //TRACE("- Characteristic %d read\n", cNode->characteristic->Handle);

            uint8_t status = cNode->characteristic->readRequest(pr->data_length, pr->offset);
            if (status != BLE_STATUS_SUCCESS)
            {
                TRACE("Error while reading characteristic. 0x%02X\n", status);
            }

            return;
         }
         cNode = cNode->next;
      }
      sNode = sNode->next;
   }
}

void cBlueNRG::disconnected()
{
   connection_handle = 0;
   mConnectable = 1;
   mConnected = 0;

   if(mStateListener)
      mStateListener->setState(false);

   cBlueNRGserviceNode *n = mServiceList;
   while(n)
   {
      cBlueNRGcharNode *c = n->service->CharacteristicList;
      while(c)
      {
         c->characteristic->setNotification(0);
         c = c->next;
      }
      n = n->next;
   }


   TRACE(YELLOW("Disconnected\n"));
}

void cBlueNRG::handleHCI_CB(void *pckt)
{
   hci_uart_pckt *hci_pckt = (hci_uart_pckt *)pckt;

   /* obtain event packet */
   hci_event_pckt *event_pckt = (hci_event_pckt*)hci_pckt->data;
   if(hci_pckt->type != HCI_EVENT_PKT)
      return;

   //TRACE("evt: %04X\n", event_pckt->evt);
   switch(event_pckt->evt){
   case EVT_DISCONN_COMPLETE:
   {
      disconnected();
   }
   break;

   case EVT_LE_META_EVENT:
   {
      evt_le_meta_event *evt = (evt_le_meta_event *)event_pckt->data;
      switch(evt->subevent)
      {
      case EVT_LE_CONN_COMPLETE:
      {
         mConnected = 1;

         if(mStateListener)
            mStateListener->setState(true);

         evt_le_connection_complete *cc = (evt_le_connection_complete *)evt->data;
         connection_handle = cc->handle;
         TRACE(GREEN("Connected\n"));
      }
      break;
      }
   }
   break;

   case EVT_VENDOR:
   {
      evt_blue_aci *blue_evt = (evt_blue_aci*)event_pckt->data;
      //TRACE("EVT_VENDOR ecode: 0x%04X\n", blue_evt->ecode);
      switch(blue_evt->ecode)
      {

      case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED:
      {
         /* this callback is invoked when a GATT attribute is modified
          extract callback data and pass to suitable handler function */
         if (bnrg_expansion_board == IDB05A1) {
            evt_gatt_attr_modified_IDB05A1 *evt = (evt_gatt_attr_modified_IDB05A1*)blue_evt->data;
            attributeModified(evt->attr_handle, evt->data_length, evt->att_data);
         }
         else {
            evt_gatt_attr_modified_IDB04A1 *evt = (evt_gatt_attr_modified_IDB04A1*)blue_evt->data;
            attributeModified(evt->attr_handle, evt->data_length, evt->att_data);
         }
      }
      break;

      case EVT_BLUE_GATT_WRITE_PERMIT_REQ:
      {
         evt_gatt_write_permit_req *pr = (evt_gatt_write_permit_req*)blue_evt->data;
         //TRACE("EVT_BLUE_GATT_WRITE_PERMIT_REQ: %d bytes\n",  pr->data_length);
         attributeWrite(pr->attr_handle, pr->data_length, pr->data);

         aci_gatt_write_response(pr->conn_handle, pr->attr_handle, 1, 0, pr->data_length, pr->data);
      }
      break;

      case EVT_BLUE_GATT_READ_PERMIT_REQ:
      {
         evt_gatt_read_permit_req *pr = (evt_gatt_read_permit_req*)blue_evt->data;
         //TRACE("EVT_BLUE_GATT_READ_PERMIT_REQ: %d %d\n", pr->data_length, pr->offset);

         if(connection_handle != 0)
           aci_gatt_allow_read(connection_handle);

         attributeRead(pr);
      }
      break;
      }

   }

   break;
   }
}

cyg_bool cBlueNRG::addService(cBlueNRGservice * service, uint8_t att_count)
{
   uint8_t ret = aci_gatt_add_serv(UUID_TYPE_128, service->UUID, PRIMARY_SERVICE, (att_count * 3) + 1, &service->Handle);
   if(ret == BLE_STATUS_SUCCESS)
   {
      //insert in the back of the list
      if(!mServiceList)
      {
         //TRACE("Add first Service %d\n", service->Handle);
         mServiceList = new cBlueNRGserviceNode(service);
         return true;
      }

      cBlueNRGserviceNode *n = mServiceList;;
      while(n->next)
      {
         n = n->next;
      }


      //TRACE("Add Service %d\n", service->Handle);
      n->setNext(new cBlueNRGserviceNode(service));

      return true;
   }
   else
   {
      TRACE(RED("Add Service failed: 0x%02X\n"), ret);
   }

   return false;
}

void cBlueNRG::listServices()
{
   cBlueNRGserviceNode *n = mServiceList;
   while(n)
   {
      TRACE(" - Service %d %d\n", n->service->Handle, n->service->UUID[15]);
      n->service->listCharacteristics();
      n = n->next;
   }
}

/**
 *  The setup has to be called in thread context */
void cBlueNRG::setup(cyg_spi_device *dev, cyg_uint32 extiPin, cyg_uint32 resetPin)
{
   /* Initialize the BlueNRG HCI */
   HCI_Init();

   BNRG_SPI_Init(dev, extiPin, resetPin, bleReady);

}

void cBlueNRG::initConnection()
{
   /* Reset BlueNRG hardware */
   BlueNRG_RST();
   TRACE("Reset 1\n");

   /* get the BlueNRG HW and FW versions */
   uint8_t  hwVersion;
   uint16_t fwVersion;
   if(getBlueNRGVersion(&hwVersion, &fwVersion) == BLE_STATUS_TIMEOUT)
   {
      TRACE(RED("BLE Version TIME_OUT\n"));
      return;
   }
   else
      TRACE("HWver %d, FWver %d\n", hwVersion, fwVersion);


   /*
    * Reset BlueNRG again otherwise we won't
    * be able to change its MAC address.
    * aci_hal_write_config_data() must be the first
    * command after reset otherwise it will fail.
    */
   BlueNRG_RST();
   TRACE("Reset 2\n");

   int ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
         CONFIG_DATA_PUBADDR_LEN,
         mBDAddress);
   if(ret){
      TRACE(RED("Setting BD_ADDR failed.\n"));
   }

   uint16_t service_handle, dev_name_char_handle, appearance_char_handle;


   if (hwVersion > 0x30)// X-NUCLEO-IDB05A1 expansion board is used
      bnrg_expansion_board = IDB05A1;

   ret = aci_gatt_init();
   if(ret){
      TRACE(RED("GATT_Init failed.\n"));
      return;
   }

   if (bnrg_expansion_board == IDB05A1) {
      ret = aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);
   }
   else {
      ret = aci_gap_init_IDB04A1(GAP_PERIPHERAL_ROLE_IDB04A1, &service_handle, &dev_name_char_handle, &appearance_char_handle);
   }

   if(ret != BLE_STATUS_SUCCESS){
      TRACE(RED("GAP_Init failed.\n"));
   }


   const char *name = "KeyBLE";
   ret = aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0,
         strlen(name), (uint8_t *)name);

   if(ret){
      TRACE(RED("aci_gatt_update_char_value failed.\n"));
      return;
   }

   ret = aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED,
         OOB_AUTH_DATA_ABSENT,
         NULL,
         7,
         16,
         USE_FIXED_PIN_FOR_PAIRING,
         123456,
         BONDING);
   if (ret == BLE_STATUS_SUCCESS) {
      TRACE(GREEN("BLE Stack Initialized.\n"));
   }

   /* Set output power level */
   ret = aci_hal_set_tx_power_level(1,4);


   mReady = true;
}

void cBlueNRG::setConnectable(void)
{
   char local_name[64];
   local_name[0] = AD_TYPE_COMPLETE_LOCAL_NAME;
   local_name[1] = 0;
   strncpy(&local_name[1], mBLEname, 64);


   /* disable scan response */
   hci_le_set_scan_resp_data(0,NULL);

   cyg_thread_delay(20);

   aci_gap_set_discoverable(ADV_IND, 0, 0, PUBLIC_ADDR, NO_WHITE_LIST_USE, strlen(&local_name[1]) + 1, local_name, 0, NULL, 0, 0);

   TRACE(YELLOW("%s General Discoverable Mode.\n"), &local_name[1]);
}


cBlueNRG::~cBlueNRG()
{
}

