#include "PoKeysLib.h"
#include "PoKeysLibCore.h"

int PK_LCDConfigurationGet(sPoKeysDevice* device)
{	
	// Get LCD configuration
    CreateRequest(device->request, 0xD0, 1, 0, 0, 0);
	if (SendRequest(device) == PK_OK)
    {
		device->LCD.Configuration = device->response[3];
		device->LCD.Rows = device->response[4];
		device->LCD.Columns = device->response[5];
	} else return PK_ERR_TRANSFER;

	return PK_OK;
}

int PK_LCDConfigurationSet(sPoKeysDevice* device)
{
	// Set LCD configuration
    CreateRequest(device->request, 0xD0, 0, device->LCD.Configuration, device->LCD.Rows, device->LCD.Columns);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    // Initialize LCD
    CreateRequest(device->request, 0xD1, 0, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    // Clear LCD
    CreateRequest(device->request, 0xD1, 0x10, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;


	return PK_OK;
}

int PK_LCDUpdate(sPoKeysDevice* device)
{
	unsigned char * lines[] = { device->LCD.line1, device->LCD.line2, device->LCD.line3, device->LCD.line4 };
	// Update LCD contents
	for (int n = 0; n < 4; n++)
	{
		if (device->LCD.RowRefreshFlags & (1<<n))
		{
			CreateRequest(device->request, 0xD1, 0x85, n + 1, 0, 0);
			for (int i = 0; i < 20; i++)
			{
                device->request[8 + i] = lines[n][i];
			}
			if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
			device->LCD.RowRefreshFlags &= ~(1<<n);
		}
	}
	return PK_OK;
}
