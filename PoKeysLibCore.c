#include <stdlib.h>
#include "PoKeysLib.h"
#include "PoKeysLibCore.h"
#include "PoKeysLibCoreSockets.h"
#include "string.h"
#include "stdio.h"
//#define PK_COM_DEBUG

// Connection specific commands
int PK_EnumerateUSBDevices()
{
    int numDevices = 0;
    struct hid_device_info *devs, *cur_dev;

    devs = hid_enumerate(0x1DC3, 0x1001);
    cur_dev = devs;

    while (cur_dev)
    {
        /*printf("Device Found\n");
        printf("  Serial:      %ls\n", cur_dev->serial_number);
        printf("  Product:      %ls\n", cur_dev->product_string);
        printf("  Interface:    %d\n",  cur_dev->interface_number);
        printf("\n");*/
        if (cur_dev->interface_number == 1) numDevices++;
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    return numDevices;
}

int PK_GetCurrentDeviceConnectionType(sPoKeysDevice* device)
{
	return device->connectionType;
}

void InitializeNewDevice(sPoKeysDevice* device)
{
	int i;
	memset(&device->info, 0, sizeof(sPoKeysDevice_Info));
	memset(&device->DeviceData, 0, sizeof(sPoKeysDevice_Info));
	memset(&device->matrixKB, 0, sizeof(sMatrixKeyboard));
	memset(&device->PWM, 0, sizeof(sPoKeysPWM));
	memset(&device->LCD, 0, sizeof(sPoKeysLCD));
	

	device->FastEncodersConfiguration = 0;
	device->FastEncodersOptions = 0;
	device->UltraFastEncoderConfiguration = 0;
	device->UltraFastEncoderOptions = 0;

	device->UltraFastEncoderFilter = 0;

	memset(device->request, 0, 64);
	memset(device->response, 0, 64);

	PK_DeviceDataGet(device);

	device->Pins = (sPoKeysPinData*)malloc(sizeof(sPoKeysPinData) * device->info.iPinCount);
	memset(device->Pins, 0, sizeof(sPoKeysPinData) * device->info.iPinCount);

	for (i = 0; i < device->info.iPinCount; i++)
	{
		if (PK_IsCounterAvailable(device, i))
		{
			device->Pins[i].DigitalCounterAvailable = 1;
		} else
		{
			device->Pins[i].DigitalCounterAvailable = 0;
		}
	}

	device->Encoders = (sPoKeysEncoder*)malloc(sizeof(sPoKeysEncoder) * device->info.iEncodersCount);
	memset(device->Encoders, 0, sizeof(sPoKeysEncoder) * device->info.iEncodersCount);

	device->PWM.PWMduty = (uint32_t*)malloc(sizeof(uint32_t) * device->info.iPWMCount);
	memset(device->PWM.PWMduty, 0, sizeof(uint32_t) * device->info.iPWMCount);

	device->PWM.PWMenabledChannels = (unsigned char*)malloc(sizeof(unsigned char) * device->info.iPWMCount);
	memset(device->PWM.PWMenabledChannels, 0, sizeof(unsigned char) * device->info.iPWMCount);

	device->PoExtBusData = (unsigned char*)malloc(sizeof(unsigned char) * device->info.iPoExtBus);

	device->MatrixLED = (sPoKeysMatrixLED*)malloc(sizeof(sPoKeysMatrixLED) * device->info.iEncodersCount);
	memset(device->MatrixLED, 0, sizeof(sPoKeysMatrixLED) * device->info.iMatrixLED);

	if (device->info.iPulseEngine)
	{
		device->PulseEngine = (sPoKeysPE*)malloc(sizeof(sPoKeysPE));

		PK_PEInfoGet(device);

		// Allocate buffer
		device->PulseEngine->buffer.buffer = (unsigned char*)malloc(sizeof(unsigned char) * device->PulseEngine->info.bufferDepth);
		memset(device->PulseEngine->buffer.buffer, 0, sizeof(unsigned char) * device->PulseEngine->info.bufferDepth);

		device->PulseEngine->ReferencePosition = (uint32_t*)malloc(sizeof(uint32_t) * device->PulseEngine->info.nrOfAxes);
		memset(device->PulseEngine->ReferencePosition, 0, sizeof(uint32_t) * device->PulseEngine->info.nrOfAxes);
		device->PulseEngine->CurrentPosition = (uint32_t*)malloc(sizeof(uint32_t) * device->PulseEngine->info.nrOfAxes);
		memset(device->PulseEngine->CurrentPosition, 0, sizeof(uint32_t) * device->PulseEngine->info.nrOfAxes);
		device->PulseEngine->MaxSpeed = (uint32_t*)malloc(sizeof(uint32_t) * device->PulseEngine->info.nrOfAxes);
		memset(device->PulseEngine->MaxSpeed, 0, sizeof(uint32_t) * device->PulseEngine->info.nrOfAxes);
		device->PulseEngine->MaxAcceleration = (uint32_t*)malloc(sizeof(uint32_t) * device->PulseEngine->info.nrOfAxes);
		memset(device->PulseEngine->MaxAcceleration, 0, sizeof(uint32_t) * device->PulseEngine->info.nrOfAxes);
		device->PulseEngine->MaxDecceleration = (uint32_t*)malloc(sizeof(uint32_t) * device->PulseEngine->info.nrOfAxes);
		memset(device->PulseEngine->MaxDecceleration, 0, sizeof(uint32_t) * device->PulseEngine->info.nrOfAxes);
		device->PulseEngine->AxisState = (unsigned char*)malloc(sizeof(unsigned char) * device->PulseEngine->info.nrOfAxes);
		memset(device->PulseEngine->AxisState, 0, sizeof(unsigned char) * device->PulseEngine->info.nrOfAxes);

		device->PulseEngine->MPGjogMultiplier = (uint32_t*)malloc(sizeof(uint32_t) * device->PulseEngine->info.nrOfAxes);
		memset(device->PulseEngine->MPGjogMultiplier, 0, sizeof(uint32_t) * device->PulseEngine->info.nrOfAxes);
		device->PulseEngine->MPGaxisEncoder = (unsigned char*)malloc(sizeof(unsigned char) * device->PulseEngine->info.nrOfAxes);
		memset(device->PulseEngine->MPGaxisEncoder, 0, sizeof(unsigned char) * device->PulseEngine->info.nrOfAxes);

	} else device->PulseEngine = NULL;
}

void CleanDevice(sPoKeysDevice* device)
{
	free(device->Pins);
	free(device->Encoders);
	free(device->PWM.PWMduty);
	free(device->PWM.PWMenabledChannels);
	free(device->PoExtBusData);
	free(device->MatrixLED);
	if (device->PulseEngine != NULL)
	{
		free(device->PulseEngine->buffer.buffer);
		
		free(device->PulseEngine->ReferencePosition);
		free(device->PulseEngine->CurrentPosition);
		free(device->PulseEngine->MaxSpeed);
		free(device->PulseEngine->MaxAcceleration);
		free(device->PulseEngine->MaxDecceleration);
		free(device->PulseEngine->AxisState);

		free(device->PulseEngine->MPGjogMultiplier);
		free(device->PulseEngine->MPGaxisEncoder);
		free(device->PulseEngine);
	}
}

sPoKeysDevice* PK_ConnectToDevice(int deviceIndex)
{
    int numDevices = 0;
    struct hid_device_info *devs, *cur_dev;
    sPoKeysDevice* tmpDevice;

    devs = hid_enumerate(0x1DC3, 0x1001);
    cur_dev = devs;

    while (cur_dev)
    {
        /*printf("Device Found\n");
        printf("  Serial:      %ls\n", cur_dev->serial_number);
        printf("  Product:      %ls\n", cur_dev->product_string);
        printf("  Interface:    %d\n",  cur_dev->interface_number);
        printf("\n");//*/
        if (cur_dev->interface_number == 1)
        {
            if (numDevices == deviceIndex)
            {
				tmpDevice = (sPoKeysDevice*)malloc(sizeof(sPoKeysDevice));

                //printf("Connect to this device...");
				tmpDevice->devHandle = (void*)hid_open_path(cur_dev->path);

				tmpDevice->connectionType = PK_DeviceType_USBDevice;

				if (tmpDevice->devHandle != NULL)
				{
					InitializeNewDevice(tmpDevice);
				} else
				{
					free(tmpDevice);
					tmpDevice = NULL;
				}
                //hid_set_nonblocking(devHandle);
                hid_free_enumeration(devs);
                return tmpDevice;
            }
            numDevices++;
        }
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    return NULL;
}

sPoKeysDevice* PK_ConnectToDeviceWSerial(long serialNumber, int checkForNetworkDevicesAndTimeout)
{
    int numDevices = 0;
    struct hid_device_info *devs, *cur_dev;
    int k;
    sPoKeysDevice* tmpDevice;
    char serialSearch[8];

    devs = hid_enumerate(0x1DC3, 0x1001);
    cur_dev = devs;

    sprintf(serialSearch, "2.%05lu", serialNumber);

    while (cur_dev)
    {
        /*printf("Device Found\n");
        printf("  Serial:      %ls\n", cur_dev->serial_number);
        printf("  Product:      %ls\n", cur_dev->product_string);
        printf("  Interface:    %d\n",  cur_dev->interface_number);
        printf("\n");//*/
        if (cur_dev->interface_number == 1)
        {
			if (cur_dev->serial_number[0] != 'P')
			{
				int k;

				for (k = 0; k < 8 && cur_dev->serial_number[k] != 0; k++)
				{
					if (cur_dev->serial_number[k] != serialSearch[k]) break;
				}

				if (k == 7)
				{
					tmpDevice = (sPoKeysDevice*)malloc(sizeof(sPoKeysDevice));

					//printf("Connect to this device...");
					tmpDevice->devHandle = (void*)hid_open_path(cur_dev->path);

					tmpDevice->connectionType = PK_DeviceType_USBDevice;
					if (tmpDevice->devHandle != NULL)
					{
						InitializeNewDevice(tmpDevice);
					} else
					{
						free(tmpDevice);
						tmpDevice = NULL;
					}
					//hid_set_nonblocking(devHandle);
					hid_free_enumeration(devs);
					return tmpDevice;
				}
			} else
			{
				// Old, PoKeys55 device - we must to connect and read the serial number...
				tmpDevice = (sPoKeysDevice*)malloc(sizeof(sPoKeysDevice));
				tmpDevice->devHandle = (void*)hid_open_path(cur_dev->path);

				if (tmpDevice->devHandle != NULL)
				{
					InitializeNewDevice(tmpDevice);
				} else
				{
					free(tmpDevice);
					tmpDevice = NULL;
					hid_free_enumeration(devs);
					return NULL;
				}
                hid_free_enumeration(devs);

				tmpDevice->connectionType = PK_DeviceType_USBDevice;
				if (tmpDevice->DeviceData.SerialNumber == serialNumber)
				{
					return tmpDevice;
				} else
				{
					CleanDevice(tmpDevice);
					free(tmpDevice);
				}
			}
            
            numDevices++;
        }
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

	if (checkForNetworkDevicesAndTimeout)
	{
		sPoKeysNetworkDeviceSummary * devices = (sPoKeysNetworkDeviceSummary*)malloc(sizeof(sPoKeysNetworkDeviceSummary) * 16);
		int iNet = PK_EnumerateNetworkDevices(devices, checkForNetworkDevicesAndTimeout);
		for (k = 0; k < iNet; k++)
		{
			//printf("\nNetwork device found, serial = %lu at %u.%u.%u.%u", devices[k].SerialNumber, devices[k].IPaddress[0], devices[k].IPaddress[1], devices[k].IPaddress[2], devices[k].IPaddress[3]);
			if (devices[k].SerialNumber == serialNumber)
			{
				tmpDevice = PK_ConnectToNetworkDevice(&devices[k]);
				if (tmpDevice == NULL)
				{
					CleanDevice(tmpDevice);
					free(tmpDevice);
					//printf("\nProblem connecting to the device...");
				} else
				{
					free(devices);
					InitializeNewDevice(tmpDevice);
					return tmpDevice;
				}
			}
		}
		free(devices);
	}

    return NULL;
}

void PK_DisconnectDevice(sPoKeysDevice* device)
{
    if (device != NULL)
    {
		if (device->connectionType == PK_DeviceType_NetworkDevice) 
		{
			PK_DisconnectNetworkDevice(device);
		} else
		{
			if ((hid_device*)device->devHandle != NULL)
			{
				hid_close((hid_device*)device->devHandle);
			}
		}

		CleanDevice(device);
		free(device);
    }
}

int CreateRequest(unsigned char * request, unsigned char type, unsigned char param1, unsigned char param2, unsigned char param3, unsigned char param4)
{
    memset(request, 0, 64);

    request[1] = type;
    request[2] = param1;
    request[3] = param2;
    request[4] = param3;
    request[5] = param4;

    return PK_OK;
}

unsigned char getChecksum(unsigned char * data)
{
    unsigned char temp = 0;
    int i;

    for (i = 0; i < 7; i++)
    {
            temp += data[i];
    }
    return temp;
}


//#define PK_COM_DEBUG
int SendRequest(sPoKeysDevice* device)
{
    // Initialize variables
    int waits = 0;
    int retries = 0;
    int result = 0;
    unsigned char bufferOut[65] = {0};
    #ifdef PK_COM_DEBUG
        int i;
    #endif
    hid_device * devHandle = (hid_device*)device->devHandle;

    if (device == NULL) return PK_ERR_GENERIC;
	if (device->connectionType == PK_DeviceType_NetworkDevice)
	{
		return SendEthRequest(device);
	}


    if (devHandle == NULL) return PK_ERR_GENERIC;


    // Request sending loop
    while (retries++ < 2)
    {
            device->request[0] = 0xBB;
            device->request[6] = ++device->requestID;
            device->request[7] = getChecksum(device->request);

            memcpy(bufferOut + 1, device->request, 64);

            #ifdef PK_COM_DEBUG
                    printf("\n * SEND ");
                    for (i = 0; i < 10; i++)
                    {
                            printf("%X ", bufferOut[i]);
                    }
            #endif

            result = hid_write(devHandle, bufferOut, 65);

            // In case of an error, try sending again
            if (result < 0)
            {
                    //printf(" ERR %u", result);
                    retries++;
                    continue;
            }

            waits = 0;

            // Request receiving loop
            while (waits++ < 20)
            {
                result = hid_read(devHandle, device->response, 65);

                // Error is not an option
                if (result < 0)
                {
                        //printf(" Receive ERR %u", result);
                        break;
                }

#ifdef PK_COM_DEBUG
        printf("\n * RECV ");
        for (i = 0; i < 10; i++)
        {
                printf("%X ", device->response[i]);
        }

        printf(" (request ID: %X ?= %X", device->response[6], device->requestID);
#endif

                // Check the header and the request ID
                if (device->response[0] == 0xAA && device->response[6] == device->requestID)
                {
                        // This is it. Return from this function
                        return PK_OK;
                }
            }
    }

    return PK_ERR_TRANSFER;
}



//// Get the number of PoKeys devices
//int CPoKeys::EnumerateDevicesLibusb()
//{
//    int devNum = 0;
//    int devPoKeys = 0;
//    int i, r;
//
//    libusb_device **devList;
//    struct libusb_device_descriptor desc;
//
//    devNum = libusb_get_device_list (NULL, &devList);
//
//    for (i = 0; i < devNum; i++)
//    {
//            r = libusb_get_device_descriptor(devList[i], &desc);
//            if (r < 0)
//            {
//                    continue;
//            }
//
//            if (desc.idVendor == VENDOR_ID && desc.idProduct == PRODUCT_ID)
//            {
//                    devPoKeys++;
//                    continue;
//            }
//    }
//
//    libusb_free_device_list(devList, 1);
//
//    return devPoKeys;
//}
//// Connect to PoKeys device
//int CPoKeys::ConnectToDeviceLibusb(int deviceIndex)
//{
//    int devNum = 0;
//    int devPoKeys = 0;
//    int i, r;
//    int result;
//
//    libusb_device **devList;
//    struct libusb_device_descriptor desc;
//
//    if (connected) DisconnectDevice();
//
//    printf("Connecting to the PoKeys device...");
//
//    devNum = libusb_get_device_list (NULL, &devList);
//
//    for (i = 0; i < devNum; i++)
//    {
//            r = libusb_get_device_descriptor(devList[i], &desc);
//            if (r < 0)
//            {
//                printf("\nError retrieving descriptor...");
//                    continue;
//            }
//
//            if (desc.idVendor == VENDOR_ID && desc.idProduct == PRODUCT_ID)
//            {
//                printf("\nPoKeys found!");
//                    if (devPoKeys == deviceIndex)
//                    {
//
//                            r = libusb_open(devList[i], &devh);
//
//                            if (r < 0)
//                            {
//                                printf("\nError opening the device...");
//                                    return -1;
//                            }
//
//                            if (devh != NULL)
//                            {
//                                    // The HID has been detected.
//                                    libusb_detach_kernel_driver(devh, INTERFACE_NUMBER);
//                                    result = libusb_claim_interface(devh, INTERFACE_NUMBER);
//                                    if (result >= 0)
//                                    {
//                                            libusb_free_device_list(devList, 1);
//                                            connected = 1;
//                                            return 0;
//                                    }
//                                    else
//                                    {
//                                        printf("\nUnable to claim the interface! %d", result);
//                                            // Unable to claim the interface
//                                            libusb_free_device_list(devList, 1);
//                                            return -2;
//                                    }
//                            }
//                            else
//                            {
//                                    // Unable to connect to device
//                                    printf("\nUnable to open the device...");
//                                    libusb_free_device_list(devList, 1);
//                                    return -3;
//                            }
//                    }
//                    devPoKeys++;
//                    continue;
//            }
//    }
//
//    libusb_free_device_list(devList, 1);
//    return -4;
//}
//
//// Connect to PoKeys device
//int ConnectToDeviceWSerial(int serialNumber, int reserved)
//{
//        int devNum = 0;
//        int devPoKeys = 0;
//        int i, r;
//        int result;
//
//        libusb_device **devList;
//        struct libusb_device_descriptor desc;
//        char buffer[] = "0.00000";
//        char buffer2[] = "0.00000";
//
//        if (connected) DisconnectDevice();
//
//        devNum = libusb_get_device_list (NULL, &devList);
//
//        //sprintf(buffer2, "2.%05u", serialNumber);
//
//        for (i = 0; i < devNum; i++)
//        {
//                r = libusb_get_device_descriptor(devList[i], &desc);
//                if (r < 0)
//                {
//                        continue;
//                }
//
//                if (desc.idVendor == VENDOR_ID && desc.idProduct == PRODUCT_ID)
//                {
//                        r = libusb_open(devList[i], &devh);
//
//                        if (r < 0 || devh == NULL)
//                        {
//                                //printf("\nCan not open the device to read the descriptor...");
//                                continue;
//                        }
//
//                        libusb_get_string_descriptor_ascii(devh, desc.iSerialNumber, (unsigned char *)buffer, 8);
//
//                        if (strcmp(buffer, buffer2) == 0)
//                        {
//                                if (devh != NULL)
//                                {
//                                        // The HID has been detected.
//                                        libusb_detach_kernel_driver(devh, INTERFACE_NUMBER);
//                                        result = libusb_claim_interface(devh, INTERFACE_NUMBER);
//                                        if (result >= 0)
//                                        {
//                                                libusb_free_device_list(devList, 1);
//                                                connected = 1;
//                                                return PK_OK;
//                                        }
//                                        else
//                                        {
//                                                // Unable to claim the interface
//                                                libusb_free_device_list(devList, 1);
//                                                return PK_ERR_CANNOT_CLAIM_USB;
//                                        }
//                                }
//                                else
//                                {
//                                        // Unable to connect to device
//                                        libusb_free_device_list(devList, 1);
//                                        return PK_ERR_CANNOT_CONNECT;
//                                }
//                        } else
//                        {
//                                libusb_close(devh);
//                        }
//                        devPoKeys++;
//                        continue;
//                }
//        }
//
//        libusb_free_device_list(devList, 1);
//        // No devices detected, return OK
//        return PK_OK;
//}


//
//void CPoKeys::DisconnectDeviceLibusb()
//{
//        if (connected)
//        {
//                libusb_release_interface(devh, 0);
//                libusb_close(devh);
//        }
//        connected = 0;
//}
//
//
//int CPoKeys::SendRequestLibusb(unsigned char *request, unsigned char *response)
//{
//        // Initialize variables
//        int waits = 0;
//        int retries = 0;
//        int result = 0;
//        int bytes_transferred;
//        #ifdef PK_COM_DEBUG
//                int i;
//        #endif
//
//        // Request sending loop
//        while (retries < 2)
//        {
//                request[0] = 0xBB;
//                request[6] = ++requestID;
//                request[7] = getChecksum(request);
//
//                #ifdef PK_COM_DEBUG
//                        printf("\n * SEND ");
//                        for (i = 0; i < 8; i++)
//                        {
//                                printf("%X ", request[i]);
//                        }
//                #endif
//
//                // Write data to the device.
//                result = libusb_interrupt_transfer(devh,
//                        0x04,  request, 64,
//                        &bytes_transferred, TIMEOUT_MS);
//
//                // In case of an error, try sending again
//                if (result < 0)
//                {
//                        //printf("ERR");
//                        retries++;
//                        continue;
//                }
//
//                waits = 0;
//
//                // Request receiving loop
//                while (waits < 20)
//                {
//                        // Read data from the device.
//                        result = libusb_interrupt_transfer(devh,
//                                0x84, response, 64,
//                                &bytes_transferred, TIMEOUT_MS);
//
//                        // Error is not an option
//                        if (result < 0)
//                        {
//                                //printf("Error...");
//                                break;
//                        }
//
//                        // Check the header and the request ID
//                        if (response[0] == 0xAA && response[6] == requestID)
//                        {
//                                #ifdef PK_COM_DEBUG
//                                        printf("\n * RECV ");
//                                        for (i = 0; i < 8; i++)
//                                        {
//                                                printf("%X ", response[i]);
//                                        }
//                                #endif
//                                // This is it. Return from this function
//                                return PK_OK;
//                        }
//                }
//        }
//
//        return PK_ERR_TRANSFER;
//}
//
//
//
