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

int PK_PinConfigurationGet(sPoKeysDevice* device)
{
	int i;
	// Get all pin configuration
    CreateRequest(device->request, 0xC0, 0, 0, 0, 0);
	if (SendRequest(device) == PK_OK)
    {
		for (i = 0; i < device->info.iPinCount; i++)
        {
            device->Pins[i].PinFunction = device->response[8 + i];
		}
	} else return PK_ERR_TRANSFER;

	// If device supports digital counters, get the settings for them
	if (device->info.iDigitalCounters)
	{
		// Get all pin configuration - counter setup
		CreateRequest(device->request, 0xC0, 0, 1, 0, 0);
		if (SendRequest(device) == PK_OK)
		{
			for (i = 0; i < device->info.iPinCount; i++)
			{
				if (PK_IsCounterAvailable(device, i))
				{
					device->Pins[i].DigitalCounterAvailable = 1;
					device->Pins[i].CounterOptions = device->response[8 + i];
				} else
				{
					device->Pins[i].DigitalCounterAvailable = 0;
				}
			}
		} else return PK_ERR_TRANSFER;
	}

	// If the device supports key mapping, get the settings
	if (device->info.iKeyMapping)
	{
		// Get all pin key mapping - type
		CreateRequest(device->request, 0xC1, 0, 0, 0, 0);
		if (SendRequest(device) == PK_OK)
		{
			for (i = 0; i < device->info.iPinCount; i++)
			{
				if (device->Pins[i].PinFunction & PK_PinCap_digitalInput)
				{
					device->Pins[i].MappingType = device->response[8 + i];
				} else device->Pins[i].MappingType = 0;        
			}
		} else return PK_ERR_TRANSFER;	

		// Get all pin key mapping - key codes
		CreateRequest(device->request, 0xC2, 0, 0, 0, 0);
		if (SendRequest(device) == PK_OK)
		{
			for (i = 0; i < device->info.iPinCount; i++)
			{
				if (device->Pins[i].PinFunction & PK_PinCap_digitalInput)
				{
					device->Pins[i].KeyCodeMacroID = device->response[8 + i];
				} else device->Pins[i].KeyCodeMacroID = 0;        
			}
		} else return PK_ERR_TRANSFER;	

		// Get all pin key mapping - key modifiers
		CreateRequest(device->request, 0xC3, 0, 0, 0, 0);
		if (SendRequest(device) == PK_OK)
		{
			for (i = 0; i < device->info.iPinCount; i++)
			{
				if (device->Pins[i].PinFunction & PK_PinCap_digitalInput)
				{
					device->Pins[i].KeyModifier = device->response[8 + i];
				} else device->Pins[i].KeyModifier = 0;        
			}
		} else return PK_ERR_TRANSFER;	
        
		// Again, check if device supports triggered key mapping
		if (device->info.iTriggeredKeyMapping)
		{
			// Get all pin key mapping - triggered inputs
			CreateRequest(device->request, 0xD7, 11, 0, 0, 0);
			if (SendRequest(device) == PK_OK)
			{
				for (i = 0; i < device->info.iPinCount; i++)
				{
					if (device->Pins[i].PinFunction & PK_PinCap_triggeredInput)
					{
						device->Pins[i].downKeyCodeMacroID = device->response[8 + i];
					} else device->Pins[i].downKeyCodeMacroID = 0;        
				}
			} else return PK_ERR_TRANSFER;	

			CreateRequest(device->request, 0xD7, 12, 0, 0, 0);
			if (SendRequest(device) == PK_OK)
			{
				for (i = 0; i < device->info.iPinCount; i++)
				{
					if (device->Pins[i].PinFunction & PK_PinCap_triggeredInput)
					{
						device->Pins[i].downKeyModifier= device->response[8 + i];
					} else device->Pins[i].downKeyModifier = 0;        
				}
			} else return PK_ERR_TRANSFER;	

			CreateRequest(device->request, 0xD7, 13, 0, 0, 0);
			if (SendRequest(device) == PK_OK)
			{
				for (i = 0; i < device->info.iPinCount; i++)
				{
					if (device->Pins[i].PinFunction & PK_PinCap_triggeredInput)
					{
						device->Pins[i].upKeyCodeMacroID = device->response[8 + i];
					} else device->Pins[i].upKeyCodeMacroID = 0;        
				}
			} else return PK_ERR_TRANSFER;	

			CreateRequest(device->request, 0xD7, 14, 0, 0, 0);
			if (SendRequest(device) == PK_OK)
			{
				for (i = 0; i < device->info.iPinCount; i++)
				{
					if (device->Pins[i].PinFunction & PK_PinCap_triggeredInput)
					{
						device->Pins[i].upKeyModifier = device->response[8 + i];
					} else device->Pins[i].upKeyModifier = 0;        
				}
			} else return PK_ERR_TRANSFER;	
		}
	}
	return PK_OK;
}

int PK_PinConfigurationSet(sPoKeysDevice* device)
{
	int i;
	
	// Set all pin configuration
    CreateRequest(device->request, 0xC0, 1, 0, 0, 0);
	for (i = 0; i < device->info.iPinCount; i++)
    {
        device->request[8 + i] = device->Pins[i].PinFunction;
	}
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;


	// If device supports digital counters, set the settings for them
	if (device->info.iDigitalCounters)
	{
		// Get all pin configuration - counter setup
		CreateRequest(device->request, 0xC0, 0, 2, 0, 0);
		for (i = 0; i < device->info.iPinCount; i++)
		{
			device->request[8 + i] = device->Pins[i].CounterOptions;
		}
		if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
	}

	// If the device supports key mapping, set the settings
	if (device->info.iKeyMapping)
	{
		// Set all pin key mapping - type
		CreateRequest(device->request, 0xC1, 1, 0, 0, 0);
		for (i = 0; i < device->info.iPinCount; i++)
		{
			if (device->Pins[i].PinFunction & PK_PinCap_digitalInput)
			{
				device->request[8 + i] = device->Pins[i].MappingType;
			}
		}
		if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

		// Set all pin key mapping - key codes
		CreateRequest(device->request, 0xC2, 1, 0, 0, 0);
		for (i = 0; i < device->info.iPinCount; i++)
		{
			if (device->Pins[i].PinFunction & PK_PinCap_digitalInput)
			{
				device->request[8 + i] = device->Pins[i].KeyCodeMacroID;
			}
		}
		if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

		// Set all pin key mapping - key modifiers
		CreateRequest(device->request, 0xC3, 1, 0, 0, 0);
		for (i = 0; i < device->info.iPinCount; i++)
		{
			if (device->Pins[i].PinFunction & PK_PinCap_digitalInput)
			{
				device->request[8 + i] = device->Pins[i].KeyModifier;
			}
		}
		if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
        
		// Again, check if device supports triggered key mapping
		if (device->info.iTriggeredKeyMapping)
		{
			// Set all pin key mapping - triggered inputs
			CreateRequest(device->request, 0xD7, 1, 0, 0, 0);
			for (i = 0; i < device->info.iPinCount; i++)
			{
				if (device->Pins[i].PinFunction & PK_PinCap_triggeredInput)
				{
					device->request[8 + i] = device->Pins[i].downKeyCodeMacroID;
				}
			}
			if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

			CreateRequest(device->request, 0xD7, 2, 0, 0, 0);
			for (i = 0; i < device->info.iPinCount; i++)
			{
				if (device->Pins[i].PinFunction & PK_PinCap_triggeredInput)
				{
					device->request[8 + i] = device->Pins[i].downKeyModifier;
				}
			}
			if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;			

			CreateRequest(device->request, 0xD7, 3, 0, 0, 0);
			for (i = 0; i < device->info.iPinCount; i++)
			{
				if (device->Pins[i].PinFunction & PK_PinCap_triggeredInput)
				{
					device->request[8 + i] = device->Pins[i].upKeyCodeMacroID;
				}
			}
			if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

			CreateRequest(device->request, 0xD7, 4, 0, 0, 0);
			for (i = 0; i < device->info.iPinCount; i++)
			{
				if (device->Pins[i].PinFunction & PK_PinCap_triggeredInput)
				{
					device->request[8 + i] = device->Pins[i].upKeyModifier;
				}
			}
			if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
		}
	}
	return PK_OK;
}




int PK_IsCounterAvailable(sPoKeysDevice* device, unsigned char pinID)
{
    //                        1   2   3   4   5   6   7   8   9   10
    int counterSupported[] = {1,  1,  0 , 0 , 1,  1,  0 , 0 , 1,  0 ,
                              1,  0 , 0 , 0 , 1,  1,  0 , 0 , 1,  1,
                              1,  1,  1,  1,  1,  1,  1,  1,  0 , 0 ,
                              0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
                              1,  1,  1,  1,  0 , 1,  0 , 1,  1,  0 ,
                              0 , 0 , 0 , 0 , 0 , 0 , 0 };

    if ((device->DeviceData.DeviceType != 10) && (device->DeviceData.DeviceType != 11)) return 0;
    if (pinID >= 55) return 0;

    return counterSupported[pinID];
}

int PK_DigitalIOSet(sPoKeysDevice* device)
{
	int i;
	// Set digital outputs
	CreateRequest(device->request, 0xCC, 1, 0, 0, 0);
	for (i = 0; i < device->info.iPinCount; i++)
    {
        if (device->Pins[i].DigitalValueSet > 0)
        {
            device->request[8 + i / 8] |= (unsigned char)(1 << (i % 8));
        }
    }
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;   

	return PK_OK;
}

int PK_DigitalIOGet(sPoKeysDevice* device)
{
	int i;
	// Get digital inputs
	CreateRequest(device->request, 0xCC, 0, 0, 0, 0);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;   

	for (i = 0; i < device->info.iPinCount; i++)
    {
		device->Pins[i].DigitalValueGet = ((unsigned char)(device->response[8 + i / 8] & (1 << (i % 8))) > 0) ? 1 : 0;
    }

	return PK_OK;
}              

int PK_DigitalIOSetGet(sPoKeysDevice* device)
{
	int i;
	// Set digital outputs
	CreateRequest(device->request, 0xCC, 1, 0, 0, 0);
	for (i = 0; i < device->info.iPinCount; i++)
    {
        if (device->Pins[i].DigitalValueSet > 0)
        {
            device->request[8 + i / 8] |= (unsigned char)(1 << (i % 8));
        }
    }
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;   
	// Get digital inputs
	for (i = 0; i < device->info.iPinCount; i++)
    {
		device->Pins[i].DigitalValueGet = ((unsigned char)(device->response[8 + i / 8] & (1 << (i % 8))) > 0) ? 1 : 0;
    }

	return PK_OK;
}

int PK_DigitalIOSetSingle(sPoKeysDevice* device, unsigned char pinID, unsigned char pinValue)
{
	if (pinID >= device->info.iPinCount) return PK_ERR_PARAMETER;

	// Set single digital output
	CreateRequest(device->request, 0x40, pinID, pinValue, 0, 0);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;   

	if (device->response[2] != 0) return PK_ERR_GENERIC;
	return PK_OK;
}

int PK_DigitalIOGetSingle(sPoKeysDevice* device, unsigned char pinID, unsigned char* pinValue)
{
	if (pinID >= device->info.iPinCount) return PK_ERR_PARAMETER;

	// Set single digital output
	CreateRequest(device->request, 0x30, pinID, 0, 0, 0);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;   
	*pinValue = device->response[3];

	if (device->response[2] != 0) return PK_ERR_GENERIC;
	return PK_OK;
}

int PK_AnalogIOGet(sPoKeysDevice* device)
{
	int i;
	// Get analog inputs
	CreateRequest(device->request, 0x3A, 1, 0, 0, 0);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;   

	for (i = 0; i < 7; i++)
    {
		device->Pins[40 + i].AnalogValue = ((long)device->response[8 + i * 2] << 8) + (long)device->response[9 + i * 2];
    }

	return PK_OK;
}              

int PK_AnalogIOGetAsArray(sPoKeysDevice* device, unsigned int * buffer)
{
    int result = PK_AnalogIOGet(device);
    int i;

    if (result == PK_OK)
    {
        for (i = 0; i < 7; i++)
        {
            buffer[i] = device->Pins[40+i].AnalogValue;
        }
    }
    return result;
}

int PK_DigitalCounterGet(sPoKeysDevice* device)
{
	// Get analog inputs
	int requestPending = 0;
	int k = 0;
	long value = 0;
	int i, j;

	CreateRequest(device->request, 0xD8, 0, 0, 0, 0);

	for (i = 0; i < device->info.iPinCount; i++)
	{
		if (PK_IsCounterAvailable(device, i))
		{
			requestPending = 1;
			device->request[8 + k] = i;
			k++;

			if (k >= 13)
			{
				if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER; 

				for (j = 0; j < 13; j++)
				{
					value = (long)device->response[8 + j * 4];
					value += (long)device->response[9 + j * 4] << 8;
					value += (long)device->response[10 + j * 4] << 16;
					value += (long)device->response[11 + j * 4] << 24;

					device->Pins[device->request[8 + j]].DigitalCounterValue = value;
				}
				requestPending = 0;
				k = 0;
			}
		}
	}
	if (requestPending)
	{
		if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER; 

		for (j = 0; j < k; j++)
		{
			value = (long)device->response[8 + j * 4];
			value += (long)device->response[9 + j * 4] << 8;
			value += (long)device->response[10 + j * 4] << 16;
			value += (long)device->response[11 + j * 4] << 24;

			device->Pins[device->request[8 + j]].DigitalCounterValue = value;
		}
	}

	return PK_OK;
}              

int PK_PWMConfigurationSet(sPoKeysDevice* device)
{
	int n;
	// Set PWM configuration
	CreateRequest(device->request, 0xCB, 1, 0, 0, 0);

    for (n = 0; n < 6; n++)
    {
		if (device->PWM.PWMenabledChannels[n]) device->request[8] |= (unsigned char)(1 << n);

		memcpy(&(device->request[9 + n * 4]), &(device->PWM.PWMduty[n]), 4);
    }
	memcpy(&(device->request[33]), &(device->PWM.PWMperiod), 4);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;   

	return PK_OK;
}

int PK_PWMConfigurationGet(sPoKeysDevice* device)
{
	int n;
	// Get PWM configuration
	CreateRequest(device->request, 0xCB, 0, 0, 0, 0);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;   

    for (n = 0; n < 6; n++)
    {
		device->PWM.PWMenabledChannels[n] = (device->response[8] & (1<<n)) > 0 ? 1 : 0;

		memcpy(&(device->PWM.PWMduty[n]), &(device->response[9 + n * 4]), 4);
    }
	memcpy(&(device->PWM.PWMperiod), &(device->response[33]), 4);

	return PK_OK;
}

int PK_PWMUpdate(sPoKeysDevice* device)
{
	int n;
	// Update PWM values
	CreateRequest(device->request, 0xCB, 1, 1, 0, 0);

    for (n = 0; n < 6; n++)
    {
		if (device->PWM.PWMenabledChannels[n]) device->request[8] |= (unsigned char)(1 << n);

		memcpy(&(device->request[9 + n * 4]), &(device->PWM.PWMduty[n]), 4);
    }
	memcpy(&(device->request[33]), &(device->PWM.PWMperiod), 4);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;   

	return PK_OK;
}


int PK_PWMConfigurationSetDirectly(sPoKeysDevice * device, unsigned int PWMperiod, unsigned char * enabledChannels)
{
    if (PK_PWMConfigurationGet(device) != PK_OK) return PK_ERR_GENERIC;

    device->PWM.PWMperiod = PWMperiod;
    memcpy(device->PWM.PWMenabledChannels, enabledChannels, 6);
    memset(device->PWM.PWMduty, 0, 6*4);
    return PK_PWMConfigurationSet(device);
}

int PK_PWMUpdateDirectly(sPoKeysDevice * device, unsigned int * dutyCycles)
{
    memcpy(device->PWM.PWMduty, dutyCycles, 6*4);
    return PK_PWMUpdate(device);
}


int PK_PoExtBusSet(sPoKeysDevice* device)
{
	int i;
	// Set PoExtBus values
    CreateRequest(device->request, 0xDA, 1, 0, 0, 0);
	for (i = 0; i < device->info.iPoExtBus; i++)
    {
        device->request[8 + i] = device->PoExtBusData[i];
	}
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	return PK_OK;
}

int PK_PoExtBusGet(sPoKeysDevice* device)
{
	int i;
	// Get PoExtBus values
    CreateRequest(device->request, 0xDA, 2, 0, 0, 0);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
	for (i = 0; i < device->info.iPoExtBus; i++)
    {
		device->PoExtBusData[i] = device->response[8 + i];
	}

	return PK_OK;
}
