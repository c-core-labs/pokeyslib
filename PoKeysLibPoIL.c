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
#include "PoKeysLibCore.h"

void PK_ParsePoILStateResponse(sPoKeysDevice* device)
{
    device->PoIL.info.CodeMemorySize = device->response[8] + ((unsigned int)device->response[9] << 8);
    device->PoIL.info.DataMemorySize = device->response[10] + ((unsigned int)device->response[11] << 8);

    device->PoIL.info.Version = device->response[12];

    device->PoIL.CoreState = device->response[13] + ((unsigned int)device->response[14] << 8);
    device->PoIL.CoreDebugMode = device->response[15];
    device->PoIL.CoreDebugBreakpoint = device->response[17] + ((unsigned int)device->response[18] << 8);

    device->PoIL.PC = device->response[19] + ((unsigned int)device->response[20] << 8);
    device->PoIL.STATUS = device->response[21];

    device->PoIL.W = device->response[22] + ((unsigned int)device->response[23] << 8) +
                  ((unsigned int)device->response[24] << 16) + ((unsigned int)device->response[25] << 24);
    device->PoIL.ExceptionPC = device->response[26] + ((unsigned int)device->response[27] << 8);

    device->PoIL.functionStack.stackPtr = device->response[28] + ((unsigned int)device->response[29] << 8);
    device->PoIL.dataStack.stackPtr = device->response[30] + ((unsigned int)device->response[31] << 8);
    device->PoIL.MasterEnable = device->response[59];
    device->PoIL.currentTask = device->response[16];
}

int PK_PoILGetState(sPoKeysDevice* device)
{
    CreateRequest(device->request, 0x82, 0x00, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    PK_ParsePoILStateResponse(device);

    device->PoIL.functionStack.stackSize = 64;
    device->PoIL.dataStack.stackSize = 32;

    return PK_OK;
}

int PK_PoILSetCoreState(sPoKeysDevice* device, unsigned short state)
{
    CreateRequest(device->request, 0x82, 0x01, state & 0xFF, state >> 8, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    PK_ParsePoILStateResponse(device);

    return PK_OK;
}

int PK_PoILSetMasterEnable(sPoKeysDevice* device, unsigned char masterEnable)
{
    CreateRequest(device->request, 0x82, 0x03, masterEnable, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    PK_ParsePoILStateResponse(device);

    return PK_OK;
}

int PK_PoILResetCore(sPoKeysDevice* device)
{
    CreateRequest(device->request, 0x82, 0x02, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    PK_ParsePoILStateResponse(device);

    return PK_OK;
}

int PK_PoILSetDebugMode(sPoKeysDevice* device, unsigned char debugMode, unsigned short breakpoint)
{
    CreateRequest(device->request, 0x82, 0x05, debugMode, breakpoint & 0xFF, breakpoint >> 8);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    PK_ParsePoILStateResponse(device);

    return PK_OK;
}

int PK_PoILReadMemory(sPoKeysDevice* device, unsigned char memoryType, unsigned short address, unsigned short size, unsigned char * dest)
{
    unsigned int i;
    unsigned short address2 = 0;
    unsigned short readLen = 0;

    // Read in chunks of 54 bytes
    for (i = 0; i < size; i += 54)
    {
        if (i + 54 < size)
        {
            readLen = 54;
        }
        else readLen = size - i;

        address2 = address + i;

        CreateRequest(device->request, 0x82, 0x10, memoryType, (unsigned char)address2, (unsigned char)(address2 >> 8));
        device->request[8] = (unsigned char)readLen;
        if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

        memcpy(dest + i, device->response + 8, readLen);
    }
    return PK_OK;
}


int PK_PoILWriteMemory(sPoKeysDevice* device, unsigned char memoryType, unsigned short address, unsigned short size, unsigned char * src)
{
    unsigned int i;
    unsigned short address2 = 0;
    unsigned short writeLen = 0;

    if (memoryType == 0)
    {
        for (i = 0; i < size; i += 256)
        {
            unsigned char tmp[256];
            writeLen = size - i;
            if (writeLen > 256) writeLen = 256;

            memcpy(tmp, src + i, writeLen);
            PK_PoILWriteMemory(device, 1, 0, 256, tmp);

            // Write data to flash
            CreateRequest(device->request, 0x82, 0x15, 0, 0, (unsigned char)(i >> 8));
            if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
        }
    } else
        {

        // Write in chunks of 54 bytes
        for (i = 0; i < size; i += 54)
        {
            if (i + 54 < size)
            {
                writeLen = 54;
            }
            else writeLen = size - i;

            address2 = address + i;

            CreateRequest(device->request, 0x82, 0x15, memoryType, (unsigned char)address2, (unsigned char)(address2 >> 8));
            device->request[8] = (unsigned char)writeLen;
            memcpy(device->request + 9, src + i, writeLen);

            if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
        }
    }
    return PK_OK;
}

int PK_PoILReadSharedSlot(sPoKeysDevice* device, unsigned short firstSlotID, unsigned short slotsNum, int * dest)
{
    unsigned int i;
    unsigned short address2 = 0;
    unsigned char requestedSlots;

    // Read in chunks of 54 bytes
    for (i = 0; i < slotsNum; i += 13)
    {
        if (slotsNum - i > 13)
            requestedSlots = 13;
        else
            requestedSlots = slotsNum - i;

        address2 = firstSlotID + i;

        // Unlike other memories, address is the ID of the shared slot
        CreateRequest(device->request, 0x82, 0x10, 4, (unsigned char)address2, (unsigned char)(address2 >> 8));
        device->request[8] = (unsigned char)requestedSlots;
        if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

        memcpy((dest + i), device->response + 8, requestedSlots * 4);
    }
    return PK_OK;
}


int PK_PoILWriteSharedSlot(sPoKeysDevice* device, unsigned short firstSlotID, unsigned short slotsNum, int * src)
{
    unsigned int i;
    unsigned short address2 = 0;
    unsigned char requestedSlots;

    // Write in chunks of 54 bytes
    for (i = 0; i < slotsNum; i += 13)
    {
        if (slotsNum - i > 13)
            requestedSlots = 13;
        else
            requestedSlots = slotsNum - i;

        address2 = firstSlotID + i;

        // Unlike other memories, address is the ID of the shared slot
        CreateRequest(device->request, 0x82, 0x15, 4, (unsigned char)address2, (unsigned char)(address2 >> 8));
        device->request[8] = (unsigned char)requestedSlots;

        memcpy(device->request + 9, (src + i), requestedSlots * 4);

        if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
    }

    return PK_OK;
}

int PK_PoILEraseMemory(sPoKeysDevice* device, unsigned char memoryType)
{
    CreateRequest(device->request, 0x82, 0x16, memoryType, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    return PK_OK;
}

int PK_PoILChunkReadMemory(sPoKeysDevice * device, unsigned char * dest)
{
    unsigned int i;
    unsigned short address2 = 0;
    unsigned short readLen = 0;

    CreateRequest(device->request, 0x82, 0x11, 1, 0, 0);
    for (i = 0; i < 18; i++)
    {
        device->request[8 + i*3] = device->PoIL.monitorChunks[i].address & 0xFF;
        device->request[8 + i*3 + 1] = (device->PoIL.monitorChunks[i].address >> 8) & 0xFF;
        device->request[8 + i*3 + 2] = device->PoIL.monitorChunks[i].chunkLength;
        if (readLen + device->PoIL.monitorChunks[i].chunkLength > 55) break;

        readLen += device->PoIL.monitorChunks[i].chunkLength;
    }
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    memcpy(dest, device->response + 8, readLen);
    return PK_OK;
}


int PK_PoILChunkReadMemoryInternalAddress(sPoKeysDevice * device, unsigned char * dest)
{
    unsigned int i;
    unsigned short address2 = 0;
    unsigned short readLen = 0;

    CreateRequest(device->request, 0x82, 0x11, 5, 0, 0);
    for (i = 0; i < 18; i++)
    {
        device->request[8 + i*3] = device->PoIL.monitorChunks[i].address & 0xFF;
        device->request[8 + i*3 + 1] = (device->PoIL.monitorChunks[i].address >> 8) & 0xFF;
        device->request[8 + i*3 + 2] = device->PoIL.monitorChunks[i].chunkLength;
        if (readLen + device->PoIL.monitorChunks[i].chunkLength > 55) break;

        readLen += device->PoIL.monitorChunks[i].chunkLength;
    }
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    memcpy(dest, device->response + 8, readLen);
    return PK_OK;
}

