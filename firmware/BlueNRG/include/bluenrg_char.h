#ifndef BLUENRG_CHAR_H_
#define BLUENRG_CHAR_H_
#include <stdint.h>

extern "C" {
#include <ble_status.h>
#include <bluenrg_gatt_server.h>
}

class cBlueNRGservice;
class cBlueNRGchar;

class cBlueNRGlistener
{
public:
   cBlueNRGlistener(){};
   virtual ~cBlueNRGlistener(){};

   virtual void handleData(uint8_t *data, uint8_t len) = 0;
};

class cBlueNRGsender
{
public:
   cBlueNRGsender(){};
   virtual ~cBlueNRGsender(){};

   virtual uint32_t readData(uint8_t *buff, uint32_t len) = 0;
};

class cBlueNRGchar
{
   uint16_t mServiceHandle;
   //uint8_t *mData;
   uint8_t mNotify;

   cBlueNRGlistener *mListener;
   cBlueNRGsender *mReader;

public:
   const uint8_t *UUID;
   uint8_t Properties;
   uint16_t Handle;
   uint8_t Length;

   cBlueNRGchar(const uint8_t *uuid, uint8_t properties, uint8_t maxLen);
   virtual ~cBlueNRGchar();

   void setListener(cBlueNRGlistener *listener){ mListener = listener; };
   void setReader(cBlueNRGsender *reader){ mReader = reader; };

   void setServiceHandle(uint16_t handle);

   void modified(uint8_t *data, uint8_t len);
   void setNotification(uint8_t state);

   tBleStatus update(uint8_t *data);
   tBleStatus updateHDLC(uint8_t *data, uint32_t len);
   tBleStatus readRequest(uint8_t length, uint16_t offset);
};


class cBlueNRGcharNode
{
public:
   cBlueNRGchar *characteristic;
   cBlueNRGcharNode *next;



   cBlueNRGcharNode(cBlueNRGchar *c = 0) : characteristic(c), next(0){};
   //	void setService(cBlueNRGservice *s){ service = s; };
   void setNext(cBlueNRGcharNode *n){ next = n; };
};

#endif /* BLUENRG_CHAR_H_ */
