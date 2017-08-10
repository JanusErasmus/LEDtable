#include <cyg/kernel/diag.h>
#include <string.h>

#include "utils.h"
#include "bluenrg_service.h"

extern "C" {
#include <bluenrg_gatt_aci.h>
#include <hci.h>
}

cBlueNRGservice::cBlueNRGservice(const uint8_t *uuid) : UUID(uuid), Handle(0), CharacteristicList(0)
{
}

cyg_bool cBlueNRGservice::add(cBlueNRGchar * characteristic)
{
   if(!Handle)
   {
      INFO_TRACE("cBlueNRGsrv", RED("Add service before adding char\n"));
      return false;
   }

   uint8_t evtMask = GATT_DONT_NOTIFY_EVENTS;
   if(characteristic->Properties & CHAR_PROP_READ)
      evtMask |= GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP;

   if(characteristic->Properties & (CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP))
      evtMask |= GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP;// | GATT_NOTIFY_ATTRIBUTE_WRITE;

   uint8_t ret = aci_gatt_add_char(Handle,
         UUID_TYPE_128,
         characteristic->UUID,
         characteristic->Length,
         characteristic->Properties,
         ATTR_PERMISSION_NONE,
         evtMask,
         16,
         0,
         &characteristic->Handle);
   cyg_thread_delay(2);

   if(ret)
   {
      INFO_TRACE("cBlueNRGsrv", RED("Char add error %02X\n"), ret);
      return false;
   }

   characteristic->setServiceHandle(Handle);

//   //Descriptors does not work when notifications are enabled
//   if(characteristic->Properties & CHAR_PROP_NOTIFY)
//   {
//      charactFormat charFormat;
//      charFormat.format = FORMAT_UINT16;
//      charFormat.exp = -1;
//      charFormat.unit = UNIT_UNITLESS;
//      charFormat.name_space = 0;
//      charFormat.desc = 0;
//      uint16_t descHandle;
//      uint16_t uuid16 = CHAR_FORMAT_DESC_UUID;
//      ret = aci_gatt_add_char_desc(Handle,
//            characteristic->Handle,
//            UUID_TYPE_16,
//            (uint8_t *)&uuid16,
//            7,
//            7,
//            (void *)&charFormat,
//            ATTR_PERMISSION_NONE,
//            ATTR_ACCESS_READ_WRITE,
//            0,
//            16,
//            0,
//            &descHandle);
//   }

   if(ret != BLE_STATUS_SUCCESS)
   {
      INFO_TRACE("cBlueNRGsrv", RED("Char desc add error %02X\n"), ret);
      return false;
   }

   //insert in the back of the list
   if(!CharacteristicList)
   {
      //INFO_TRACE("cBlueNRGsrv", "Add first Char %d\n", characteristic->Handle);
      CharacteristicList = new cBlueNRGcharNode(characteristic);

      return true;
   }

   cBlueNRGcharNode *n = CharacteristicList;;
   while(n->next)
   {
      n = n->next;
   }


   //INFO_TRACE("cBlueNRGsrv", "Add Char %d\n", characteristic->Handle);
   n->setNext(new cBlueNRGcharNode(characteristic));

   return true;
}

void cBlueNRGservice::listCharacteristics()
{
   cBlueNRGcharNode *n = CharacteristicList;
   while(n)
   {
      INFO_TRACE("cBlueNRGsrv", "   - Characteristic %d %d\n", n->characteristic->Handle, n->characteristic->UUID[15]);
      n = n->next;
   }
}

cBlueNRGservice::~cBlueNRGservice()
{
}

