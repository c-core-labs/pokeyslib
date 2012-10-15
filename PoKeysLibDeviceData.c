#include "PoKeysLib.h"
#include "PoKeysLibCore.h"
#include "stdio.h"

int PK_DeviceDataGet(sPoKeysDevice* device)
{
	int i;
    sPoKeysDevice_Data * data = &device->DeviceData;
    sPoKeysDevice_Info * info = &device->info;

    int devSeries55 = 0;
    int devSeries56 = 0;
    int devSeries27 = 0;
    int devUSB = 0;
    int devEth = 0;
    int devBootloader = 0;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

	memset(info, 0, sizeof(sPoKeysDevice_Info));

	memset(device->request, 0, 64);
	memset(device->response, 0, 64);

	// Read serial and firmware version
	CreateRequest(device->request, 0x00, 0, 0, 0, 0);
	if (SendRequest(device) == PK_OK)
    {
		data->FirmwareVersionMajor = device->response[4];
		data->FirmwareVersionMinor = device->response[5];

		data->SerialNumber = ((long)device->response[2]*256 + (long)device->response[3]);
    } else return PK_ERR_TRANSFER;

	// Read User ID
    CreateRequest(device->request, 0x03, 0, 0, 0, 0);
	if (SendRequest(device) == PK_OK)
    {
        data->UserID = device->response[2];
	} else return PK_ERR_TRANSFER;


	// Resolve device type
    if (data->SerialNumber == 0xFFFF)
    {
		data->DeviceType = 3; // old bootloader - recovery mode
		devSeries55 = 1;
		devBootloader = 1;
    
	// PoKeys56 devices have serial numbers above 20000
	} else if (data->SerialNumber >= 20000) 
    {
		// PoKeys56 bootloaders have bit 7 set in the major firmware version
		if ((data->FirmwareVersionMajor & (1 << 7)) > 0)
        {
            if (device->connectionType == PK_DeviceType_USBDevice)
            {
                data->DeviceType = 16; // PoKeys56E bootloader
				devBootloader = 1;
				devSeries56 = 1;
            }
            else
            {
                data->DeviceType = 15; // PoKeys56U bootloader
				devUSB = 1;
				devBootloader = 1;
				devSeries56 = 1;
            }
        }
        else
        {
            // PoTLog27
            if (data->FirmwareVersionMajor == 64)
            {
				if (device->connectionType == PK_DeviceType_NetworkDevice)
                {
                    data->DeviceType = 21; // PoTLog27E
					devSeries27 = 1;
					devEth = 1;
                }
                else
                {
					devUSB = 1;
					devSeries27 = 1;
                    data->DeviceType = 20; // PoTLog27U
                }
            }
            else
            {
                if (device->connectionType == PK_DeviceType_NetworkDevice)
                {
                    data->DeviceType = 11; // PoKeys56E
					devSeries56 = 1;
					devEth = 1;
                }
                else
                {
					devUSB = 1;
                    data->DeviceType = 10; // PoKeys56U
    				devSeries56 = 1;
				}
            }
        }
    }
	// PoKeys55 v3
    else if (data->SerialNumber >= 11500)
    {
		devUSB = 1;
		devSeries55 = 1;
        data->DeviceType = 2;
    }
	// PoKeys55 v2
    else if (data->SerialNumber >= 10113)
    {
		devUSB = 1;
		devSeries55 = 1;
        data->DeviceType = 1;
    }
	// PoKeys55 v1
    else
	{
		devUSB = 1;
		devSeries55 = 1;
        data->DeviceType = 0;
	}

	// Resolve the type names
	switch (data->DeviceType)
    {
        case 0:
            sprintf(data->DeviceTypeName, "generic PoKeys device");
			break;
        case 1:
            sprintf(data->DeviceTypeName, "PoKeys55");
			break;
        case 2:
            sprintf(data->DeviceTypeName, "PoKeys55");
			break;
        case 3:
            sprintf(data->DeviceTypeName, "PoKeys55 - recovery");
			break;
        case 10:
            sprintf(data->DeviceTypeName, "PoKeys56U");
			break;
        case 11:
            sprintf(data->DeviceTypeName, "PoKeys56E");
			break;
        case 15:
            sprintf(data->DeviceTypeName, "PoKeys56U - recovery");
			break;
        case 16:
            sprintf(data->DeviceTypeName, "PoKeys56E - recovery");
			break;
        case 20:
            sprintf(data->DeviceTypeName, "PoTLog27U");
			break;
        case 21:
            sprintf(data->DeviceTypeName, "PoTLog27E");
			break;
        default:
            sprintf(data->DeviceTypeName, "PoKeys");
			break;
    }

	switch (data->DeviceType)
    {
		// PoKeys55
        case 0:
		case 1:
		case 2:
			info->iPinCount = 55;
			info->iEncodersCount = 25;
			info->iPWMCount = 6;
			info->iBasicEncoderCount = 25;

			device->info.PWMinternalFrequency = 12000000;
			break;

		// PoKeys55 bootloader
		case 3:
			info->iPinCount = 0;
			info->iEncodersCount = 0;
			info->iPWMCount = 0;
			info->iBasicEncoderCount = 0;
			break;
        
		// PoKeys56U, PoKeys56E
		case 10:
		case 11:
			info->iPinCount = 55;
			info->iEncodersCount = 26;
			info->iBasicEncoderCount = 25;
			info->iPWMCount = 6;

			device->info.PWMinternalFrequency = 25000000;
			break;

		// PoKeys56U, PoKeys56E bootloader
		case 15:
        case 16:
			info->iPinCount = 0;
			info->iEncodersCount = 0;
			info->iBasicEncoderCount = 0;
			info->iPWMCount = 0;
			break;
		
		// PoTLog27U, PoTLog27E
		case 20:
		case 21:
			info->iPinCount = 55;
			info->iBasicEncoderCount = 0;
			info->iEncodersCount = 0;
			info->iPWMCount = 0;
			break;

		default:
			info->iPinCount = 0;
			info->iBasicEncoderCount = 0;
			info->iEncodersCount = 0;
			info->iPWMCount = 0;

			break;
    }

	// Read device name
    CreateRequest(device->request, 0x06, 0, 0, 0, 0);
	if (SendRequest(device) == PK_OK)
    {
		for (i = 0; i < 10; i++)
		{
			data->DeviceName[i] = device->response[8 + i];
		}
	} else return PK_ERR_TRANSFER;

	// If the device name is empty, rewrite the name with the type name
	if (strlen(data->DeviceName))
	{
        strcpy(data->DeviceName, data->DeviceTypeName);
	}

	// Read firmware build date
    CreateRequest(device->request, 0x04, 0, 0, 0, 0);
	if (SendRequest(device) == PK_OK)
    {
		for (i = 0; i < 4; i++)	data->BuildDate[i + 0] = device->response[2 + i];
	} else return PK_ERR_TRANSFER;

    CreateRequest(device->request, 0x04, 1, 0, 0, 0);
	if (SendRequest(device) == PK_OK)
    {
		for (i = 0; i < 4; i++)	data->BuildDate[i + 4] = device->response[2 + i];
	} else return PK_ERR_TRANSFER;

    CreateRequest(device->request, 0x04, 2, 0, 0, 0);
	if (SendRequest(device) == PK_OK)
    {
		for (i = 0; i < 3; i++)	data->BuildDate[i + 8] = device->response[2 + i];
		data->BuildDate[11] = 0;
	} else return PK_ERR_TRANSFER;



	// Check device capabilities
	if (!devBootloader)
	{
		if (devUSB)						info->iKeyMapping = 1;
		if (devUSB && !devSeries27)		info->iTriggeredKeyMapping = 1;
		if (devUSB && !devSeries27)		info->iKeyRepeatDelay = 1;
		if (devSeries56)				info->iDigitalCounters = 1;		
		if (devUSB)						info->iJoystickButtonAxisMapping = 1;
		if (devUSB)						info->iJoystickAnalogToDigitalMapping = 1;
		if (devSeries55 || devSeries56)	info->iFastEncoders = 3;
		if (devSeries56)				info->iUltraFastEncoders = 1;
		if (devUSB)						info->iMacros = 1;
										info->iMatrixKeyboard = 1;
		if (devUSB)						info->iMatrixKeyboardTriggeredMapping = 1;
		if (devSeries56)				info->iPoNET = 1;
		if (!devSeries27)				info->iLCD = 1;
		if (!devSeries27)				info->iMatrixLED = 2;
		if (!devSeries27)				info->iConnectionSignal = 1;
		if (!devSeries27)				info->iPoExtBus = 10;
		if (1)							info->iAnalogInputs = 1;
		if (1)	  						info->iAnalogFiltering = 1;
		if (!devSeries55)				info->iInitOutputsStart = 1;
		if (devSeries56 || devSeries27) info->iprotI2C = 1;
		if (devSeries56)				info->iprot1wire = 1;
		if (devSeries56 && !(data->FirmwareVersionMajor == 32 && data->FirmwareVersionMinor < 13)) info->iAdditionalOptions = 1;
		if (!devSeries27)				info->iLoadStatus = 1;
		if (!devSeries27)				info->iCustomDeviceName = 1;
		if (devSeries56 || devSeries27) info->iPoTLog27support = 1;
		if (devSeries56 || devSeries27) info->iSensorList = 1;
		if (devEth)						info->iWebInterface = 1;
		if (devSeries56)				info->iFailSafeSettings = 1;
		if (devSeries56 && devUSB)		info->iJoystickHATswitch = 1;
	}

	// Read activated options
    CreateRequest(device->request, 0x8F, 0, 0, 0, 0);
	if (SendRequest(device) == PK_OK)
    {
		data->ActivatedOptions = device->response[8];
	} else return PK_ERR_TRANSFER;

	if (data->ActivatedOptions & 1) info->iPulseEngine = 1;

	return PK_OK;
}

int PK_DeviceActivation(sPoKeysDevice* device)
{
	if (device == NULL) return PK_ERR_NOT_CONNECTED;

	// Send new activation code
    CreateRequest(device->request, 0x8F, 0x01, 0, 0, 0);
	memcpy(&(device->request[8]), device->DeviceData.ActivationCode, 8);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	device->DeviceData.ActivatedOptions = device->response[8];

	return PK_OK;
}

int PK_DeviceActivationClear(sPoKeysDevice* device)
{
	if (device == NULL) return PK_ERR_NOT_CONNECTED;

	// Send new activation code
    CreateRequest(device->request, 0x8F, 0xFF, 0, 0, 0);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	device->DeviceData.ActivatedOptions = device->response[8];

	return PK_OK;
}

int PK_SaveConfiguration(sPoKeysDevice* device)
{
	if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x50, 0xAA, 0x55, 0, 0);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	return PK_OK;
}
