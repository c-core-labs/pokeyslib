#include "PoKeysLib.h"
#include "PoKeysLibCore.h"

#ifdef WIN32
    #include "windows.h"
    #include "Winsock.h"
    #include "conio.h"
    #include <iphlpapi.h>
#else
    #include <unistd.h>
    #include <stdlib.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <net/if.h>
    #include <sys/ioctl.h>
#endif
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "PoKeysLib.h"
#include <vector>

//#define DEBUG_PoKeysLibSockets

#ifdef DEBUG_PoKeysLibSockets
    #define debug_printf printf
#else
    #define debug_printf (void)
#endif


// Get a list of interfaces - source: Jeremy @ http://developerweb.net/viewtopic.php?id=5085
typedef unsigned long uint32;

#if defined(__FreeBSD__) || defined(BSD) || defined(__APPLE__) || defined(__linux__)
    #define USE_GETIFADDRS 1
    #define SOCKADDR struct sockaddr

    #include <ifaddrs.h>

    uint32 SockAddrToUint32(struct sockaddr * a)
    {
       return ((a)&&(a->sa_family == AF_INET)) ? ntohl(((struct sockaddr_in *)a)->sin_addr.s_addr) : 0;
    }
#endif

std::vector<uint32> GetBroadcastAddresses()
{
    std::vector<uint32> list;
    list.clear();

#if defined(USE_GETIFADDRS)
    // BSD-style implementation
    struct ifaddrs * ifap;
    if (getifaddrs(&ifap) == 0)
    {
        struct ifaddrs * p = ifap;
        while(p)
        {
            uint32 ifaAddr  = SockAddrToUint32(p->ifa_addr);
            if (ifaAddr > 0)
                list.push_back(((struct sockaddr_in *)p->ifa_broadaddr)->sin_addr.s_addr);

            p = p->ifa_next;
        }
        freeifaddrs(ifap);
    }
#elif defined(WIN32)
    // Windows XP style implementation

    // Adapted from example code at http://msdn2.microsoft.com/en-us/library/aa365917.aspx
    // Now get Windows' IPv4 addresses table.  Once again, we gotta call GetIpAddrTable()
    // multiple times in order to deal with potential race conditions properly.
    MIB_IPADDRTABLE * ipTable = NULL;
    {
      ULONG bufLen = 0;
      for (int i=0; i<5; i++)
      {
         DWORD ipRet = GetIpAddrTable(ipTable, &bufLen, false);
         if (ipRet == ERROR_INSUFFICIENT_BUFFER)
         {
            free(ipTable);  // in case we had previously allocated it
            ipTable = (MIB_IPADDRTABLE *) malloc(bufLen);
         }
         else if (ipRet == NO_ERROR) break;
         else
         {
            free(ipTable);
            ipTable = NULL;
            break;
         }
     }
   }

   if (ipTable)
   {
      // Try to get the Adapters-info table, so we can given useful names to the IP
      // addresses we are returning.  Gotta call GetAdaptersInfo() up to 5 times to handle
      // the potential race condition between the size-query call and the get-data call.
      // I love a well-designed API :^P
      IP_ADAPTER_INFO * pAdapterInfo = NULL;
      {
         ULONG bufLen = 0;
         for (int i=0; i<5; i++)
         {
            DWORD apRet = GetAdaptersInfo(pAdapterInfo, &bufLen);
            if (apRet == ERROR_BUFFER_OVERFLOW)
            {
               free(pAdapterInfo);  // in case we had previously allocated it
               pAdapterInfo = (IP_ADAPTER_INFO *) malloc(bufLen);
            }
            else if (apRet == ERROR_SUCCESS) break;
            else
            {
               free(pAdapterInfo);
               pAdapterInfo = NULL;
               break;
            }
         }
      }

      for (DWORD i=0; i<ipTable->dwNumEntries; i++)
      {
         const MIB_IPADDRROW & row = ipTable->table[i];

         /*
         // Now lookup the appropriate adaptor-name in the pAdaptorInfos, if we can find it
         const char * name = NULL;
         const char * desc = NULL;
         if (pAdapterInfo)
         {
            IP_ADAPTER_INFO * next = pAdapterInfo;
            while((next)&&(name==NULL))
            {
               IP_ADDR_STRING * ipAddr = &next->IpAddressList;
               while(ipAddr)
               {

                  if (Inet_AtoN(ipAddr->IpAddress.String) == ntohl(row.dwAddr))
                  {
                     name = next->AdapterName;
                     desc = next->Description;
                     break;
                  }

                  ipAddr = ipAddr->Next;
               }
               next = next->Next;
            }
         }
         */
         /*
         char buf[128];
         if (name == NULL)
         {
            sprintf(buf, "unnamed-%i", i);
            name = buf;
         }*/

         uint32 ipAddr  = ntohl(row.dwAddr);
         uint32 netmask = ntohl(row.dwMask);
         uint32 baddr   = ipAddr & netmask;
         if (row.dwBCastAddr) baddr |= ~netmask;

         list.push_back(baddr);
         /*
         char ifaAddrStr[32];  Inet_NtoA(ipAddr,  ifaAddrStr);
         char maskAddrStr[32]; Inet_NtoA(netmask, maskAddrStr);
         char dstAddrStr[32];  Inet_NtoA(baddr,   dstAddrStr);
         printf("  Found interface:  name=[%s] desc=[%s] address=[%s] netmask=[%s] broadcastAddr=[%s]\n", name, desc?desc:"unavailable", ifaAddrStr, maskAddrStr, dstAddrStr);
        */
      }

      free(pAdapterInfo);
      free(ipTable);
   }
#else
   // Dunno what we're running on here!
#  error "Don't know how to implement PrintNetworkInterfaceInfos() on this OS!"
#endif

    return list;
}

#ifdef WIN32
    WSADATA wsaData;

    int InitWinsock()
    {
            // Initialize Winsock - version 2.2
            if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
            {
                    debug_printf("Winsock startup failed!\n");
                    return -1;
            }
            return 0;
    }

    int TerminateWinsock()
    {
            WSACleanup();
            return 0;
    }
#endif


int PK_EnumerateNetworkDevices(sPoKeysNetworkDeviceSummary * devices, int timeout)
{
    //Broadcast the message
    int t = timeout; // 100 ms timeout
    int UDPbroadcast = 1;
    int status = 0;
    int BufLen = 0;
    char SendBuf[1];


    debug_printf("Enumerating network PoKeys devices...\n");

#ifdef WIN32
    SOCKET txSocket;
    sockaddr_in remoteEP;

	if (InitWinsock() != 0) 
	{
		debug_printf("InitWinsock error!");
		return 0;
    }
#else
    int txSocket;
    struct sockaddr_in remoteEP;
    fd_set fds;
    struct timeval stimeout;

#endif
	
    // Create socket for discovery packet
    if ((txSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        debug_printf("Error creating socket\n");
        return 0;
    }
    if (setsockopt( txSocket, SOL_SOCKET, SO_BROADCAST, (char *)&UDPbroadcast, sizeof UDPbroadcast) == -1)
    {
        debug_printf("Error setting broadcast option\n");
        return 0;
    }

#ifdef WIN32
    if (setsockopt( txSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&t, sizeof(t)) == -1)
    {
        debug_printf("Error setting SO_RCVTIMEO option\n");
        return 0;
    }
    if (setsockopt( txSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&t, sizeof(t)) == -1)
    {
        debug_printf("Error setting SO_SNDTIMEO option\n");
        return 0;
    }
#endif


    debug_printf("Sending discovery request...\n");

    std::vector<uint32> addr = GetBroadcastAddresses();
    for (int i = 0; i < addr.size(); i++)
    {
        uint32 a = (uint32)addr[i];
        debug_printf("Sending request to %d.%d.%d.%d... ", (a&0xFF),(a>>8)&0xFF, (a>>16)&0xFF, (a>>24)&0xFF);

        // Send discovery request...
        remoteEP.sin_family = AF_INET;
        remoteEP.sin_port = htons(20055);
        remoteEP.sin_addr.s_addr = a; // inet_addr("255.255.255.255");

        if (sendto(txSocket, SendBuf, BufLen, 0, (SOCKADDR *)&remoteEP, sizeof(remoteEP)) == -1)
        {
            debug_printf("Failed\n");
#ifdef WIN32
            closesocket(txSocket);
            WSACleanup();
#else
            close(txSocket);
#endif
            return 0;
        }
        debug_printf(" done\n");
    }


    debug_printf("Waiting for responses...\n");
    unsigned char rcvbuf[500];
    int nrOfDetectedBoards = 0;

    sPoKeysNetworkDeviceSummary * device;


#ifdef WIN32

#else
    // Wait for discovery response...
    stimeout.tv_sec = 0;
    stimeout.tv_usec = 1000 * timeout;

    FD_ZERO(&fds);
    FD_SET(txSocket, &fds);
#endif


    while(nrOfDetectedBoards < 16)
    {
        // Receive response from devices
#ifdef WIN32
#else
        if (select(txSocket + 1, &fds, NULL, NULL, &stimeout) < 0)
        {
            debug_printf("Error in select...\n");
            close(txSocket);
            return 0;
        }

        if (FD_ISSET(txSocket, &fds) == 0) break;
#endif

        debug_printf("Retrieving data...\n");
        status = recv(txSocket, (char *)rcvbuf, sizeof(rcvbuf), 0);

        if (status < 0)
        {
            debug_printf("Error receiving data...\n");
            break;
        }

        // Get IP address and receive message
        if (status > 0)
        {
			// Parse the response
			debug_printf("Received response...\n");

			debug_printf("  User ID: %u\n", rcvbuf[0]);
			debug_printf("  Serial: %u\n", (int)(256 * (int)rcvbuf[1] + rcvbuf[2]));
			debug_printf("  Version: %u.%u\n", rcvbuf[3], rcvbuf[4]);
			debug_printf("  IP address: %u.%u.%u.%u\n", rcvbuf[5], rcvbuf[6], rcvbuf[7], rcvbuf[8]);
			debug_printf("  DHCP: %u\n", rcvbuf[9]);
			debug_printf("  Host IP address: %u.%u.%u.%u\n", rcvbuf[10], rcvbuf[11], rcvbuf[12], rcvbuf[13]);             

			// Save the device info
			device = &devices[nrOfDetectedBoards];
			//device->userID = rcvbuf[0];
			device->SerialNumber = (int)(256 * (int)rcvbuf[1] + rcvbuf[2]);
			device->FirmwareVersionMajor = rcvbuf[3];
			device->FirmwareVersionMinor = rcvbuf[4];
			memcpy(device->IPaddress, rcvbuf + 5, 4);
			device->DHCP = rcvbuf[9];
			memcpy(device->hostIP, rcvbuf + 10, 4);

		    nrOfDetectedBoards++;
            status = 0;
        }
		else
		{
			if (nrOfDetectedBoards == 0)
			{
				debug_printf("\n No Boards detected\n");
			}
		}


    }


#ifdef WIN32
    closesocket(txSocket);
#else
    close(txSocket);
#endif

    return nrOfDetectedBoards;
}

sPoKeysDevice* PK_ConnectToNetworkDevice(sPoKeysNetworkDeviceSummary * device)
{
    int t = 500; // 500 ms timeout
    // Create target endpoint
    struct sockaddr_in remoteEP;

    debug_printf("\nConnecting to PoKeys device ... ");
    // Create temporary device object
    sPoKeysDevice * tmpDevice = new sPoKeysDevice();

    tmpDevice->connectionType = PK_DeviceType_NetworkDevice; // Network device


    uint32 addr = (uint32)device->IPaddress[0] + ((uint32)device->IPaddress[1] << 8) + ((uint32)device->IPaddress[2] << 16) + ((uint32)device->IPaddress[3] << 24);
    // Set up the RecvAddr structure with the broadcast ip address and correct port number
    remoteEP.sin_family = AF_INET;
    remoteEP.sin_port = htons(20055);
    remoteEP.sin_addr.s_addr = addr;

    //debug_printf(" %lu", addr);

    // Create socket
#ifdef WIN32
    if ((SOCKET)(tmpDevice->devHandle = (void*)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
#else
    tmpDevice->devHandle = malloc(sizeof(int));
    if ((*(int *)tmpDevice->devHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
#endif
    {
        CleanDevice(tmpDevice);
        delete tmpDevice;

        return NULL; // Couldn't create the socket
    }

#ifdef WIN32
	// Set socket options
	setsockopt((SOCKET)tmpDevice->devHandle, SOL_SOCKET, SO_RCVTIMEO, (char *)&t, sizeof(t));
    setsockopt((SOCKET)tmpDevice->devHandle, SOL_SOCKET, SO_SNDTIMEO, (char *)&t, sizeof(t));
#endif


	// Connect to target IP
#ifdef WIN32
    if (connect((SOCKET)tmpDevice->devHandle, (SOCKADDR *)&remoteEP, sizeof(remoteEP)) == -1)
#else
    if (connect(*(int *)tmpDevice->devHandle, (struct sockaddr *)&remoteEP, sizeof(remoteEP)) == -1)
#endif
    {
		debug_printf(" ERROR");
		CleanDevice(tmpDevice);
		delete tmpDevice;

        return NULL; // Couldn't connect
    }

	debug_printf(" Connected\n");

    debug_printf("Initializing the device object... ");
    InitializeNewDevice(tmpDevice);
    debug_printf("done\n");


	//printf("Connected to PoKeys device at %s", addrBuf);
	return tmpDevice;
}

void PK_DisconnectNetworkDevice(sPoKeysDevice* device)
{
    if (device == NULL) return;
	if (device->connectionType != PK_DeviceType_NetworkDevice) return;

    debug_printf("\nClosing connection...");

#ifdef WIN32
    if ((SOCKET)device->devHandle)
		closesocket((SOCKET)device->devHandle);
#else
    close(*(int *)device->devHandle);
    free(device->devHandle);
#endif
}


int SendEthRequest(sPoKeysDevice* device)
{    
    int retries1 = 0;
    int retries2 = 0;
    int result;

#ifdef WIN32
#else
    fd_set fds;
    struct timeval stimeout;
#endif

    if (device == NULL) return PK_ERR_GENERIC;
    if (device->connectionType != PK_DeviceType_NetworkDevice) return PK_ERR_GENERIC;
    if (device->devHandle == NULL) return PK_ERR_GENERIC;


    while (1)
    {
        // Form the request packet
        device->requestID++;

        device->request[0] = 0xBB;
        device->request[6] = device->requestID;
        device->request[7] = getChecksum(device->request);

        //memcpy(requestBuffer, device->request, 64);

        debug_printf("\nSending...");
        // Send the data

#ifdef WIN32
		if (send((SOCKET)device->devHandle, (char *)device->request, 64, 0) != 64)
#else
        if (send(*(int*)device->devHandle, (char *)device->request, 64, 0) != 64)
#endif
        {
			debug_printf("Error sending TCP report\nAborting...\n");
			return -1;
		}

		// Wait for the response
		while(1)
		{
#ifdef WIN32
            result = recv((SOCKET)device->devHandle, (char *)device->response, 64, 0);
#else
            FD_ZERO(&fds);
            FD_SET(*(int*)device->devHandle, &fds);

            stimeout.tv_sec = 0;
            stimeout.tv_usec = 1000 * 100;

            result = select(*(int*)device->devHandle + 1, &fds, NULL, NULL, &stimeout);

            if (result == 0 || result == -1)
            {
                // Timeout...
                debug_printf("Timeout!");
                if (++retries1 > 10) break;
                continue;
            }

            //if (FD_ISSET(*(int*)device->devHandle, &fds) == 0)

            result = recv(*(int*)device->devHandle, (char *)device->response, 64, 0);
#endif

			// 64 bytes received?
			if (result == 64)
			{
				if (device->response[0] == 0xAA && device->response[6] == device->requestID)
				{
					if (device->response[7] == getChecksum(device->response))
					{
						debug_printf(" Received!");
						return PK_OK;
					} else 
					{
						debug_printf("!! Wrong checksum...");
					}
				} else
				{
					debug_printf("!! Wrong response received!");
					break;
				}
			}
			else if (result == 0)
				debug_printf("Connection closed\n");
			else
#ifdef WIN32
                debug_printf("recv failed: %d\n", WSAGetLastError());
#else
                debug_printf("recv failed\n");
#endif

            
			if (++retries1 > 10) break;
		}

        if (retries2++ > 3) break;
    }

	debug_printf("Error - timeout...");
	return PK_ERR_TRANSFER;

}
