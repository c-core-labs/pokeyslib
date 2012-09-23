#include "hidapi.h"
#include <stdio.h>
#include <string.h>

#ifndef __POKEYSLIBCORE
	#define __POKEYSLIBCORE

	void InitializeNewDevice(sPoKeysDevice* device);
	void CleanDevice(sPoKeysDevice* device);
	void PK_DisconnectNetworkDevice(sPoKeysDevice* device);

	int CreateRequest(unsigned char * request, unsigned char type, unsigned char param1, unsigned char param2, unsigned char param3, unsigned char param4);
	unsigned char getChecksum(unsigned char * data);
	int SendRequest(sPoKeysDevice * device);


#endif
