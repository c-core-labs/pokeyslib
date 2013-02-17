#include "PoKeysLib.h"
#include "PoKeysLibCore.h"

int PK_PoNETSetModuleStatus(sPoKeysDevice* device)
{
  int i;

  CreateRequest(device->request, 0xDD, 0x55, device->PoNETmodule.moduleID, 0, 0);
  for (i = 0; i < 16; i++)
  {
    device->request[8 + i] = device->PoNETmodule.status[i];
  }
  
  if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
	return PK_OK;
}