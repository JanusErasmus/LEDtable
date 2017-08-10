#ifndef BLUENRG_SERVICE_H_
#define BLUENRG_SERVICE_H_
#include <stdint.h>

extern "C" {
#include <ble_status.h>
}

#include "bluenrg_char.h"

class cBlueNRGservice
{
public:
	const uint8_t *UUID;
	uint16_t Handle;

	cBlueNRGcharNode *CharacteristicList;

	cBlueNRGservice(const uint8_t *uuid);
	virtual ~cBlueNRGservice();

	cyg_bool add(cBlueNRGchar * characteristic);

	void listCharacteristics();
};

class cBlueNRGserviceNode
{
public:
	cBlueNRGservice *service;
	cBlueNRGserviceNode *next;

	cBlueNRGserviceNode(cBlueNRGservice *s = 0) : service(s), next(0){};
	void setNext(cBlueNRGserviceNode *n){ next = n; };
};

#endif /* BLUENRG_SERVICE_H_ */
