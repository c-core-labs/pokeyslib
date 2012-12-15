/*

Copyright (C) 2012 Matevž Bošnak (matevz@poscope.com)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/

#include "PoKeysLib.h"
#include "PoKeysLibCore.h"


int PK_I2CSetStatus(sPoKeysDevice* device, int activated)
{
    CreateRequest(device->request, 0xDB, activated, 0, 0, 0);
    return SendRequest(device);
}

int PK_I2CGetStatus(sPoKeysDevice* device, int* activated) // Retrieves I2C bus activation status
{
    CreateRequest(device->request, 0xDB, 0x02, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    *activated = device->response[3];
    return PK_OK;
}

int PK_I2CWriteStart(sPoKeysDevice* device, unsigned char address, unsigned char* buffer, unsigned char iDataLength)
{
    int i;
    if (iDataLength > 32) iDataLength = 32;

    CreateRequest(device->request, 0xDB, 0x10, address, iDataLength, 0);
    for (i = 0; i < iDataLength; i++)
    {
        device->request[8+i] = buffer[i];
    }
    return SendRequest(device);
}

int PK_I2CWriteStatusGet(sPoKeysDevice* device, int* status)
{
    CreateRequest(device->request, 0xDB, 0x11, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    *status = device->response[3];
    return PK_OK;
}

int PK_I2CReadStart(sPoKeysDevice* device, unsigned char address, unsigned char iDataLength)
{
    if (iDataLength > 32) iDataLength = 32;

    CreateRequest(device->request, 0xDB, 0x20, address, iDataLength, 0);
    return SendRequest(device);
}

int PK_I2CReadStatusGet(sPoKeysDevice* device, int* status, unsigned char* iReadBytes, unsigned char* buffer, unsigned char iMaxBufferLength)
{
    int i;

    CreateRequest(device->request, 0xDB, 0x21, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    *status = device->response[3];
    *iReadBytes = 0;

    if (*status == PK_I2C_STAT_COMPLETE)
    {
        *iReadBytes = device->response[9];
        if (*iReadBytes > 32)
        {
            *status = PK_I2C_STAT_ERR;
            return PK_ERR_GENERIC;
        }

        for (i = 0; i < iMaxBufferLength && i < *iReadBytes; i++)
        {
            buffer[i] = device->response[10+i];
        }
    }

    return PK_OK;
}

int PK_I2CBusScanStart(sPoKeysDevice* device)
{
    CreateRequest(device->request, 0xDB, 0x30, 0, 0, 0);
    return SendRequest(device);
}

int PK_I2CBusScanGetResults(sPoKeysDevice* device, int* status, unsigned char* presentDevices, unsigned char iMaxDevices)
{
    int i;

    if (iMaxDevices > 128) iMaxDevices = 128;

    CreateRequest(device->request, 0xDB, 0x31, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    *status = device->response[3];

    if (*status == PK_I2C_STAT_COMPLETE)
    {
        for (i = 0; i < iMaxDevices; i++)
        {
            presentDevices[i] = ((device->response[9 + i / 8] & (1 << (i % 8))) > 0) ? PK_I2C_STAT_OK : PK_I2C_STAT_ERR;
        }
    }

    return PK_OK;
}
