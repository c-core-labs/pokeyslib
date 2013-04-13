/*

Copyright (C) 2013 Matevž Bošnak (matevz@poscope.com)

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

int PK_SPIConfigure(sPoKeysDevice * device, unsigned char prescaler, unsigned char frameFormat)
{
    // Configure SPI
    CreateRequest(device->request, 0xE5, 0x01, prescaler, frameFormat, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
    return PK_OK;
}

int PK_SPIWrite(sPoKeysDevice * device, unsigned char * buffer, unsigned char iDataLength, unsigned char pinCS)
{
    int i;
    if (iDataLength > 16) iDataLength = 16;

    CreateRequest(device->request, 0xE5, 0x10, iDataLength, pinCS, 0);
    for (i = 0; i < iDataLength; i++)
    {
        device->request[8+i] = buffer[i];
    }
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    if (device->response[3] == 1) return PK_OK; else return PK_ERR_GENERIC;
}

int PK_SPIRead(sPoKeysDevice * device, unsigned char * buffer, unsigned char iDataLength)
{
    int i;
    if (iDataLength > 16) iDataLength = 16;

    CreateRequest(device->request, 0xE5, 0x20, iDataLength, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    if (device->response[3] == 1)
    {
        for (i = 0; i < iDataLength; i++)
        {
            buffer[i] = device->response[8+i];
        }
        return PK_OK;
    } else return PK_ERR_GENERIC;
}

