#include "PoKeysLib.h"
#include "PoKeysLibCore.h"

int PK_MatrixLEDConfigurationSet(sPoKeysDevice* device)
{
	if (device->info.iMatrixLED == 0) return PK_ERR_GENERIC;

	// Update matrix LED configuration
	CreateRequest(device->request, 0xD5, 0, 0, 0, 0);
	device->request[3] = ((device->MatrixLED[0].displayEnabled ? 1 : 0) + (device->MatrixLED[1].displayEnabled ? 2 : 0));
	device->request[4] = ((device->MatrixLED[0].rows & 0x0F) + (device->MatrixLED[0].columns & 0x0F) * 16);
	device->request[5] = ((device->MatrixLED[1].rows & 0x0F) + (device->MatrixLED[1].columns & 0x0F) * 16);

	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
	return PK_OK;
}

int PK_MatrixLEDConfigurationGet(sPoKeysDevice* device)
{
	if (device->info.iMatrixLED == 0) return PK_ERR_GENERIC;

	// Get matrix LED configuration
	CreateRequest(device->request, 0xD5, 1, 0, 0, 0);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	device->MatrixLED[0].displayEnabled = (device->response[3] & 1) > 0 ? 1 : 0;
	device->MatrixLED[0].rows = device->response[4] & 0x0F;
	device->MatrixLED[0].columns = (device->response[4] >> 4) & 0x0F;

	device->MatrixLED[1].displayEnabled = (device->response[3] & 2) > 0 ? 1 : 0;
	device->MatrixLED[1].rows = device->response[5] & 0x0F;
	device->MatrixLED[1].columns = (device->response[5] >> 4) & 0x0F;

	return PK_OK;
}

int PK_MatrixLEDUpdate(sPoKeysDevice* device)
{
	if (device->info.iMatrixLED == 0) return PK_ERR_GENERIC;

	int displayCode[] = {1, 11};

	// Update matrix LEDs
	for (int i = 0; i < device->info.iMatrixLED; i++)
	{
		if (device->MatrixLED[i].RefreshFlag)
		{
			CreateRequest(device->request, 0xD6, displayCode[i], 0, 0, 0);

			for (int j = 0; j < 8; j++)
			{
				device->request[8 + j] = device->MatrixLED[i].data[j];
			}
			if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
			device->MatrixLED[i].RefreshFlag = 0;
		}
	}

	return PK_OK;
}