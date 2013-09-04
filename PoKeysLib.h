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

#ifndef __POKEYSLIB
	#define __POKEYSLIB

	#include "stdint.h"

#ifdef POKEYSDLL
	#ifdef POKEYSDLLEXPORT
		#define POKEYSDECL __declspec(dllexport)
	#else
		#define POKEYSDECL __declspec(dllimport)
	#endif
#else
	#define POKEYSDECL
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	// Pin capabilities / configuration
	enum ePK_PinCap
	{
		PK_PinCap_pinRestricted = 0,		// Pin is not used
		PK_PinCap_reserved = 1,				// --
		PK_PinCap_digitalInput = 2,			// Digital input
		PK_PinCap_digitalOutput = 4,		// Digital output
		PK_PinCap_analogInput = 8,			// Analog input (only on selected pins)
		PK_PinCap_analogOutput = 16,		// Analog output (only on selected pins)
		PK_PinCap_triggeredInput = 32,		// Triggered input
		PK_PinCap_digitalCounter = 64,		// Digital counter (only on selected pins)
		PK_PinCap_invertPin = 128			// Invert digital pin polarity (set together with digital input, output or triggered input)
	};



    // Pin capabilities / configuration
    typedef enum
    {
        PK_AllPinCap_digitalInput,          // Digital input supported
        PK_AllPinCap_digitalOutput,         // Digital output supported
        PK_AllPinCap_analogInput,           // Analog input supported
        PK_AllPinCap_analogOutput,          // Analog output supported
        PK_AllPinCap_keyboardMapping,
        PK_AllPinCap_triggeredInput,        // Triggered input supported
        PK_AllPinCap_digitalCounter,        // Digital counter supported
        PK_AllPinCap_PWMOut,                // PWM output supported
        PK_AllPinCap_fastEncoder1A,
        PK_AllPinCap_fastEncoder1B,
        PK_AllPinCap_fastEncoder1I,
        PK_AllPinCap_fastEncoder2A,
        PK_AllPinCap_fastEncoder2B,
        PK_AllPinCap_fastEncoder2I,
        PK_AllPinCap_fastEncoder3A,
        PK_AllPinCap_fastEncoder3B,
        PK_AllPinCap_fastEncoder3I,
        PK_AllPinCap_ultraFastEncoderA,
        PK_AllPinCap_ultraFastEncoderB,
        PK_AllPinCap_ultraFastEncoderI,
        PK_AllPinCap_LCD_E,
        PK_AllPinCap_LCD_RW,
        PK_AllPinCap_LCD_RS,
        PK_AllPinCap_LCD_D4,
        PK_AllPinCap_LCD_D5,
        PK_AllPinCap_LCD_D6,
        PK_AllPinCap_LCD_D7
    } ePK_AllPinCap;


    typedef enum
    {
        PK_Device_Bootloader    = (1<<0),
        PK_Device_Bootloader55  = (1<<1),
        PK_Device_Bootloader56  = (1<<2),
        PK_Device_Bootloader56U = (1<<3),
        PK_Device_Bootloader56E = (1<<4),

        PK_Device_55            = (1<<10),
        PK_Device_55v1          = (1<<11),
        PK_Device_55v2          = (1<<12),
        PK_Device_55v3          = (1<<13),

        PK_Device_56            = (1<<20),
        PK_Device_56U           = (1<<21),
        PK_Device_56E           = (1<<22),
        PK_Device_27            = (1<<23),
        PK_Device_27U           = (1<<24),
        PK_Device_27E           = (1<<25)
    } ePK_DeviceTypes;



    typedef struct
    {
        int cap;
        int pinStart;
        int pinEnd;
        int additionalCheck;
        int devTypes;
    } sPoKeys_PinCapabilities;






	// Connection type
	enum ePK_DeviceConnectionType
	{
		PK_DeviceType_USBDevice = 0,
		PK_DeviceType_NetworkDevice = 1
	};

	// Pulse engine state
	enum ePK_PEState
	{
		PK_PEState_peSTOPPED = 0,				// Pulse engine is stopped
		PK_PEState_peINTERNAL = 1,				// Internal motion controller is in use
		PK_PEState_peBUFFER = 2,				// Buffered operation mode is in use
		PK_PEState_peJOGGING = 10,				// Jogging mode enabled
		PK_PEState_peSTOPPING = 11,				// Pulse engine is stopping
		PK_PEState_peHOME = 20,					// All axes are homed
		PK_PEState_peHOMING = 21,				// Axes homing is in progress
		PK_PEState_peSTOP_LIMIT = 100,			// Pulse engine stopped due to limit reached
		PK_PEState_peSTOP_EMERGENCY = 101		// Pulse engine stopped due to emergency switch
	};

	// Pulse engine axis state
	enum ePK_PEAxisState
	{
		PK_PEAxisState_axSTOPPED     =  0,		// Axis is stopped
		PK_PEAxisState_axREADY    =  1,			// Axis ready
		PK_PEAxisState_axRUNNING    =  2,		// Axis is running
		PK_PEAxisState_axHOME    =  10,			// Axis is homed
		PK_PEAxisState_axHOMINGSTART  =  11,	// Homing procedure is starting on axis
		PK_PEAxisState_axHOMINGSEARCH  =  12,	// Homing procedure first step - going to home
		PK_PEAxisState_axHOMINGBACK     =  13,	// Homing procedure second step - slow homing
		PK_PEAxisState_axERROR    =  20,		// Axis error
		PK_PEAxisState_axLIMIT     =  30		// Axis limit tripped
	};

	// Return codes for various functions
    enum ePK_RETURN_CODES
    {
		PK_OK                           = 0,
		PK_ERR_GENERIC                  = -1,
		PK_ERR_NOT_CONNECTED			= -5,
		PK_ERR_TRANSFER                 = -10,
		PK_ERR_PARAMETER                = -20,
		PK_ERR_CANNOT_CLAIM_USB         = -100,
		PK_ERR_CANNOT_CONNECT           = -101
    };

    enum ePK_I2C_STATUS
    {
        PK_I2C_STAT_ERR                 = 0,    // An error occured
        PK_I2C_STAT_OK                  = 1,    // All is OK
        PK_I2C_STAT_COMPLETE            = 1,    // Operation complete
        PK_I2C_STAT_IN_PROGRESS         = 0x10  // Operation still in progress
    };

	// PoKeys device information
	typedef struct 
	{
		int iPinCount;							// Number of pins, physically on the device
		int iPWMCount;							// Number of pins that support PWM output
		int iBasicEncoderCount;					// Number of basic encoders
		int iEncodersCount;						// Number of encoder slots available
		int iFastEncoders;						// Number of fast encoders supported
		int iUltraFastEncoders;					// Number of available ultra fast encoders
		long PWMinternalFrequency;				// Main PWM peripheral clock
		int iAnalogInputs;						// Number of available analog inputs

		int iKeyMapping;						// Device supports key mapping (acts as a USB keyboard)
		int iTriggeredKeyMapping;				// Device supports triggered key mapping
		int iKeyRepeatDelay;					// Device supports user customizable key repeat rates and delays
		int iDigitalCounters;					// Device supports digital counters
		int iJoystickButtonAxisMapping;			// Device supports mapping of joystick buttons
		int iJoystickAnalogToDigitalMapping;	// Device supports mapping of analog inputs to digital keys
		int iMacros;							// Device supports customizable macro sequences
		int iMatrixKeyboard;					// Device supports matrix keyboard
		int iMatrixKeyboardTriggeredMapping;	// Device supports matrix keyboard triggered key mapping
		int iLCD;								// Device supports alphanumeric LCD display
		int iMatrixLED;							// Device supports matrix LED display
		int iConnectionSignal;					// Device supports connection signal output
		int iPoExtBus;							// Device supports PoExtBus digital outputs
		int iPoNET;								// Device supports PoNET bus devices
		int iAnalogFiltering;					// Device supports analog inputs low-pass digital filtering
		int iInitOutputsStart;					// Device supports initializing outputs at startup
		int iprotI2C;							// Device supports I2C bus (master)
		int iprot1wire;							// Device supports 1-wire bus (master)
		int iAdditionalOptions;					// Device supports additional options with activation keys
		int iLoadStatus;						// Device supports reporting load status
		int iCustomDeviceName;					// Device supports specifying custom device names
		int iPoTLog27support;					// Device supports PoTLog27 firmware
		int iSensorList;						// Device supports sensor lists
		int iWebInterface;						// Device supports web interface
		int iFailSafeSettings;					// Device supports fail-safe mode
		int iJoystickHATswitch;					// Device supports joystick HAT switch mapping
		int iPulseEngine;						// Device supports Pulse engine
	} sPoKeysDevice_Info;

	// Device-specific data of the PoKeys device
	typedef struct 
	{
		long SerialNumber;						// Serial number of the device
		long FirmwareVersionMajor;				// Major firmware version number v(1+[4-7]).([0-3]) - upper 4 bits plus 1 for first part, lower 4 bits for second part
		long FirmwareVersionMinor;				// Minor firmware version number
		char DeviceName[30];					// Device name (generic or user-specified)
		char DeviceTypeName[30];				// Device type name
		char BuildDate[12];						// Build date string
		unsigned char UserID;					// Device user ID
		unsigned char DeviceType;				// Device type code
		unsigned char ActivatedOptions;			// Additional activated options - bit 0 for Pulse engine
        unsigned char DeviceLockStatus;			// Device lock status (if 1, device is locked)
		unsigned char ActivationCode[8];		// Activation code (when activating the device additional options)
        unsigned long DeviceTypeID;             // ePK_DeviceTypes ID
	} sPoKeysDevice_Data;

	// Pin-specific data
	typedef struct 
	{
		long DigitalCounterValue;				// Digital counter current value (on supported pins when PinFunction is set to digital counter - use PK_IsCounterAvailable to check the pin)
		long AnalogValue;						// Analog input value (on supported pins when PinFunction is set as analog input)
		unsigned char PinFunction;				// Pin function code - see ePK_PinCap for values 
		unsigned char CounterOptions;			// Digital counter settings (on supported pins)
		unsigned char DigitalValueGet;			// Digital input value read
		unsigned char DigitalValueSet;			// Digital output value set
		unsigned char DigitalCounterAvailable;	// 1 if digital counter is available on this pin
		unsigned char MappingType;				// Digital input to USB keyboard mapping type - selects between direct key mapping and mapping to macro
		unsigned char KeyCodeMacroID;			// USB keyboard key code or macro ID (depends on MappingType)
		unsigned char KeyModifier;				// USB keyboard key modifier
		unsigned char downKeyCodeMacroID;		// USB keyboard down key code (for triggered mapping)
		unsigned char downKeyModifier;			// USB keyboard down key modifier (for triggered mapping)
		unsigned char upKeyCodeMacroID;			// USB keyboard up key code (for triggered mapping)
		unsigned char upKeyModifier;			// USB keyboard up key modifier (for triggered mapping)
	} sPoKeysPinData;

	// Encoder-specific data
	typedef struct 
	{
		long encoderValue;						// Encoder current value
		unsigned char encoderOptions;			// Encoder options -    bit 0: enable encoder 
												//						bit 1: 4x sampling 
												//						bit 2: 2x sampling 
												//						bit 3: reserved 
												//						bit 4: direct key mapping for direction A 
												//						bit 5: mapped to macro for direction A 
												//						bit 6: direct key mapping for direction B 
												//						bit 7: mapped to macro for direction B
		unsigned char channelApin;				// Channel A encoder pin
		unsigned char channelBpin;				// Channel B encoder pin
		unsigned char dirAkeyCode;				// USB keyboard key code for direction A
		unsigned char dirAkeyModifier;			// USB keyboard key modifier for direction A
		unsigned char dirBkeyCode;				// USB keyboard key code for direction B
		unsigned char dirBkeyModifier;			// USB keyboard key modifier for direction B
		unsigned char reserved;					// placeholder
	} sPoKeysEncoder;

	// PWM-specific data
	typedef struct 
	{
		uint32_t PWMperiod;						// PWM period, shared among all channels
		uint32_t *PWMduty;						// PWM duty cycles (range between 0 and PWM period)
		unsigned char *PWMenabledChannels;		// List of enabled PWM channels
	} sPoKeysPWM;

	// Matrix keyboard specific data
	typedef struct 
	{
		unsigned char matrixKBconfiguration;	// Matrix keyboard configuration (set to 1 to enable matrix keyboard support)
		unsigned char matrixKBwidth;			// Matrix keyboard width (number of columns)
		unsigned char matrixKBheight;			// Matrix keyboard height (number of rows)
		unsigned char reserved;					// placeholder
		unsigned char matrixKBcolumnsPins[8];	// List of matrix keyboard column connections
		unsigned char matrixKBrowsPins[16];		// List of matrix keyboard row connections
		unsigned char macroMappingOptions[128];			// Selects between direct key mapping and mapping to macro sequence for each key (assumes fixed width of 8 columns)
		unsigned char keyMappingKeyCode[128];			// USB keyboard key code for each key (assumes fixed width of 8 columns), also down key code in triggered mapping mode
		unsigned char keyMappingKeyModifier[128];		// USB keyboard key modifier, also down key modifier in triggered mapping mode (assumes fixed width of 8 columns)
		unsigned char keyMappingTriggeredKey[128];		// Selects between normal direct key mapping and triggered key mapping for each key (assumes fixed width of 8 columns)
		unsigned char keyMappingKeyCodeUp[128];			// USB keyboard up key code in triggered mapping mode (assumes fixed width of 8 columns)
		unsigned char keyMappingKeyModifierUp[128];		// USB keyboard up key modifier in triggered mapping mode (assumes fixed width of 8 columns)
		unsigned char matrixKBvalues[128];		// Current state of each matrix keyboard key (assumes fixed width of 8 columns)
	} sMatrixKeyboard;


	// LCD-specific data
	typedef struct 
	{
		unsigned char Configuration;			// LCD configuration byte - 0: disabled, 1: enabled on primary pins, 2: enabled on secondary pins
		unsigned char Rows;						// Number of LCD module rows
		unsigned char Columns;					// Number of LCD module columns
		unsigned char RowRefreshFlags;			// Flag for refreshing data - bit 0: row 1, bit 1: row 2, bit 2: row 3, bit 3: row 4

		unsigned char line1[20];				// Line 1 buffer
		unsigned char line2[20];				// Line 2 buffer
		unsigned char line3[20];				// Line 3 buffer
		unsigned char line4[20];				// Line 4 buffer
	} sPoKeysLCD;

	// Matrix LED specific data
	typedef struct 
	{
		unsigned char displayEnabled;			// Display enabled byte - set to 1 to enable the display
		unsigned char rows;						// Number of Matrix LED rows
		unsigned char columns;					// Number of Matrix LED columns
		unsigned char RefreshFlag;				// Flag for refreshing data - set to 1 to refresh the display
		unsigned char data[8];					// Matrix LED buffer - one byte per row (assumes 8 columns)
	} sPoKeysMatrixLED;

	// Pulse engine information
	typedef struct 
	{
		unsigned char nrOfAxes;					// Number of supported axes
		unsigned char maxPulseFrequency;		// Maximum pulse frequency
		unsigned char bufferDepth;				// Motion buffer depth
		unsigned char slotTiming;				// Slot timing for buffer mode (in ms)
	} sPoKeysPEinfo;

	// Pulse engine buffer
	typedef struct 
	{
		unsigned char * buffer;					// Motion buffer (see the bufferDepth above for buffer size), 1 byte per axis (3 bytes per slot entry for 3-axis pulse engine)
		unsigned char newEntries;				// Number of new entries included in the buffer
		unsigned char entriesAccepted;			// Number of the entries accepted by the device
		unsigned char FreeBufferSize;			// Number of free slots in the device's buffer	
		unsigned char reserved;					// placeholder
	} sPoKeysPEbuffer;

	// Pulse engine structure
	typedef struct 
	{
		sPoKeysPEinfo info;						// Pulse engine information
		sPoKeysPEbuffer buffer;					// Pulse engine buffer stuff

		uint32_t * ReferencePosition;			// Reference position (32-bit for each axis)
		uint32_t * CurrentPosition;				// Current position (32-bit for each axis)
		uint32_t * MaxSpeed;					// Maximum speed (in pulses / second)
		uint32_t * MaxAcceleration;				// Maximum acceleration (in pulses / second / second)
		uint32_t * MaxDecceleration;			// Maximum decceleration (in pulses / second / second)
		unsigned char * AxisState;				// Axes state values - see ePK_PEAxisState for possible values
		
		uint32_t * MPGjogMultiplier;			// Multiplier for the internal MPG jog mode (for each axis)
		unsigned char * MPGaxisEncoder;			// Encoder ID for the internal MPG jog mode (for each axis)
		unsigned char MPGjogActivated;			// Internal MPG jog mode configuration (set to 1 to enable)

		unsigned char PulseEngineEnabled;		// Pulse engine enabled flag (0: disabled, 1: enabled)
		unsigned char PulseEngineState;			// Pulse engine state - see ePK_PEState for possible values
		
		unsigned char LimitConfigP;				// Limit (positive direction) switch configuration (bit-mapped, bit 0: axis 1 switch present, bit 1: axis 2 switch present, ...)
		unsigned char LimitConfigN;				// Limit (negative direction) switch configuration (bit-mapped, bit 0: axis 1 switch present, bit 1: axis 2 switch present, ...)
		unsigned char LimitStatusP;				// Limit (positive direction) switch status (bit-mapped)
		unsigned char LimitStatusN;				// Limit (negative direction) switch status (bit-mapped)
		unsigned char HomeConfig;				// Home switch configuration (bit-mapped)
		unsigned char HomeStatus;				// Home switch status (bit-mapped)

		unsigned char DirectionChange;			// Direction change configuration (bit-mapped) - if bit is set, the appropriate axis direction is inverted

		unsigned char HomingDirectionChange;	// Homing direction change (bit-mapped) - if bit is set, the appropriate axis homing direction is inverted, default motion is in negative direction
		unsigned char HomingSpeed;				// Homing speed in % of maximum speed
		unsigned char HomingReturnSpeed;		// Homing speed in second step in % of maximum speed
		unsigned char AxesHomingFlags;			// Flags for setting which axis should start homing procedure

		unsigned char kb48CNCenabled;			// If set to 1, kbd48CNC is used directly by PoKeys device for Pulse engine control
		unsigned char ChargePumpEnabled;		// Charge pump configuration (set to 1 to enable 5 kHz charge pump output on pin 53)

		unsigned char EmergencySwitchPolarity;	// Polarity change of the emergency switch (by default, normally-closed emergency switch should be used between pin 52 and GND)
		unsigned char reserved[3];				// placeholder
	} sPoKeysPE;

    // PoNET module data
    typedef struct
    {
        unsigned char status[16];
        unsigned char moduleID;
        unsigned char reserved[3];
    } sPoNETmodule;


    // PoIL-related structures

    // PoIL core info
    typedef struct
    {
        unsigned int DataMemorySize;
        unsigned int CodeMemorySize;
        unsigned int Version;
    } sPoILinfo;

    // PoIL stack info
    typedef struct
    {
        unsigned int stackPtr;
        unsigned int stackSize;

        unsigned char StackContents[1024];
    } sPoILStack;

    // Monitor mode memory chunk descriptor
    typedef struct
    {
        unsigned short address;
        unsigned char chunkLength;
        unsigned char reserved;
    } sPoILmemoryChunk;

    // PoIL core status
    typedef struct
    {
        sPoILinfo info;

        unsigned int MasterEnable;
        unsigned int currentTask;
        unsigned int STATUS;
        unsigned int W;
        unsigned int PC;
        unsigned int ExceptionPC;
        unsigned int CoreState;
        unsigned int CoreDebugMode;
        unsigned int CoreDebugBreakpoint;

        sPoILStack functionStack;
        sPoILStack dataStack;

        unsigned char codeMemoryPage[256];
        unsigned char dataMemoryPage[256];

        sPoILmemoryChunk monitorChunks[18];
    } sPoILStatus;



    typedef struct
    {
        unsigned char SEC;
        unsigned char MIN;
        unsigned char HOUR;
        unsigned char DOW;
        unsigned char DOM;
        unsigned char tmp;
        unsigned short DOY;
        unsigned short MONTH;
        unsigned short YEAR;
    } sPoKeysRTC;



	// Main PoKeys structure
	typedef struct
	{
		void* devHandle;						// Communication device handle

		sPoKeysDevice_Info info;				// PoKeys device info
		sPoKeysDevice_Data DeviceData;			// PoKeys device-specific data

		sPoKeysPinData* Pins;					// PoKeys pins
		sPoKeysEncoder* Encoders;				// PoKeys encoders

		sMatrixKeyboard matrixKB;				// Matrix keyboard structure
		sPoKeysPWM PWM;							// PWM outputs structure
		sPoKeysMatrixLED* MatrixLED;			// Matrix LED structure
		sPoKeysLCD LCD;							// LCD structure
		sPoKeysPE* PulseEngine;					// Pulse engine structure (available only when Pulse engine is supported and activated)
        sPoNETmodule PoNETmodule;
        sPoILStatus PoIL;
        sPoKeysRTC RTC;

		unsigned char FastEncodersConfiguration;		// Fast encoders configuration, invert settings and 4x sampling (see protocol specification for details)
		unsigned char FastEncodersOptions;				// Fast encoders additional options
		unsigned char UltraFastEncoderConfiguration;	// Ultra fast encoder configuration (see protocol specification for details)
		unsigned char UltraFastEncoderOptions;			// Ultra fast encoder additional options
		unsigned int UltraFastEncoderFilter;			// Ultra fast encoder digital filter setting

		unsigned char* PoExtBusData;			// PoExtBus outputs buffer

		unsigned char connectionType;			// Connection type
		unsigned char requestID;				// Communication request ID
		unsigned char request[64];				// Communication buffer
		unsigned char response[64];				// Communication buffer
	} sPoKeysDevice;

	// Network device structure - used for network device enumeration
	typedef struct
	{
		long SerialNumber;						// Serial number
		long FirmwareVersionMajor;				// Firmware version - major
		long FirmwareVersionMinor;				// Firmware version - minor
		unsigned char IPaddress[4];				// IP address of the device
		unsigned char hostIP[4];				// IP address of the host PC
		unsigned char UserID;					// User ID
		unsigned char DHCP;						// DHCP setting of the device
		unsigned char reserved[2];				// placeholder
	} sPoKeysNetworkDeviceSummary;

	// Enumerate USB devices. Returns number of USB devices detected.
	POKEYSDECL int PK_EnumerateUSBDevices(void);
	// Enumerate network devices. Return the number of ethernet devices detected and the list of detected devices (parameter devices) is filled with devices' data
	POKEYSDECL int PK_EnumerateNetworkDevices(sPoKeysNetworkDeviceSummary * devices, int timeout);
    POKEYSDECL int PK_SearchNetworkDevices(sPoKeysNetworkDeviceSummary * devices, int timeout, int serialNumberToFind);

	// Connect to USB PoKeys device, returns pointer to a newly created PoKeys device structure. Returns NULL if the connection is not successfull
	POKEYSDECL sPoKeysDevice* PK_ConnectToDevice(int deviceIndex);
	// Connect to a PoKeys device with the specific serial number. Returns pointer to a newly created PoKeys device structure. Returns NULL if the connection is not successfull
	POKEYSDECL sPoKeysDevice* PK_ConnectToDeviceWSerial(long serialNumber, int checkForNetworkDevicesAndTimeout);
	// Connect to a network PoKeys device. Returns NULL if the connection is not successfull
	POKEYSDECL sPoKeysDevice* PK_ConnectToNetworkDevice(sPoKeysNetworkDeviceSummary * device);
	// Disconnect from a PoKeys device
	POKEYSDECL void PK_DisconnectDevice(sPoKeysDevice* device);
	// Returns connection type of the specified device
	POKEYSDECL int PK_GetCurrentDeviceConnectionType(sPoKeysDevice* device);
	// Save current configuration in the device
	POKEYSDECL int PK_SaveConfiguration(sPoKeysDevice* device);

    // Check pin capabilities
    POKEYSDECL int PK_CheckPinCapability(sPoKeysDevice* device, unsigned int pin, ePK_AllPinCap cap);

    // Clone an existing device data structure into a new one
    POKEYSDECL void PK_CloneDeviceStructure(sPoKeysDevice* original, sPoKeysDevice *destination);
    // Release the device data structure from memory
    POKEYSDECL void PK_ReleaseDeviceStructure(sPoKeysDevice* device);

	// Retrieve device-specific information (this also gets automatically called when the connection with the device is established)
	POKEYSDECL int PK_DeviceDataGet(sPoKeysDevice * device);
	// Start device activation with the activation code, specified in the device structure
	POKEYSDECL int PK_DeviceActivation(sPoKeysDevice * device);
	// Clear activated options in the device
	POKEYSDECL int PK_DeviceActivationClear(sPoKeysDevice * device);

	// Retrieve pin configuration from the device
	POKEYSDECL int PK_PinConfigurationGet(sPoKeysDevice* device);
	// Send pin configuration to device
	POKEYSDECL int PK_PinConfigurationSet(sPoKeysDevice* device);

	// Retrieve encoder configuration from the device
	POKEYSDECL int PK_EncoderConfigurationGet(sPoKeysDevice* device);
	// Send encoder configuration to device
	POKEYSDECL int PK_EncoderConfigurationSet(sPoKeysDevice* device);
	// Retrieve encoder values from device
	POKEYSDECL int PK_EncoderValuesGet(sPoKeysDevice* device);
	// Send encoder values to device
	POKEYSDECL int PK_EncoderValuesSet(sPoKeysDevice* device);

	// Set digital outputs values
	POKEYSDECL int PK_DigitalIOSet(sPoKeysDevice* device);
	// Get digital inputs values
	POKEYSDECL int PK_DigitalIOGet(sPoKeysDevice* device);
	// Set digital outputs and get digital input values in one call
	POKEYSDECL int PK_DigitalIOSetGet(sPoKeysDevice* device);
	// Set single digital output
	POKEYSDECL int PK_DigitalIOSetSingle(sPoKeysDevice* device, unsigned char pinID, unsigned char pinValue);
	// Get single digital input value
	POKEYSDECL int PK_DigitalIOGetSingle(sPoKeysDevice* device, unsigned char pinID, unsigned char* pinValue);
	
	// Set PoExtBus outputs
	POKEYSDECL int PK_PoExtBusSet(sPoKeysDevice* device);
	// Get current PoExtBus outputs values
	POKEYSDECL int PK_PoExtBusGet(sPoKeysDevice* device);

	// Get digital counter values
	POKEYSDECL int PK_DigitalCounterGet(sPoKeysDevice* device);
	// Check whether digital counter is available for the specified pin. Return True if digital counter is supported.
	POKEYSDECL int PK_IsCounterAvailable(sPoKeysDevice* device, unsigned char pinID);

	// Get analog input values
	POKEYSDECL int PK_AnalogIOGet(sPoKeysDevice* device);
    // Get analog input values as an array
    POKEYSDECL int PK_AnalogIOGetAsArray(sPoKeysDevice* device, unsigned int * buffer);

	// Get matrix keyboard configuration
	POKEYSDECL int PK_MatrixKBConfigurationGet(sPoKeysDevice* device);
	// Set matrix keyboard configuration
	POKEYSDECL int PK_MatrixKBConfigurationSet(sPoKeysDevice* device);
	// Get matrix keyboard current key states
	POKEYSDECL int PK_MatrixKBStatusGet(sPoKeysDevice* device);
	
	// Set PWM outputs configuration
	POKEYSDECL int PK_PWMConfigurationSet(sPoKeysDevice* device);
	// Update PWM output duty cycles (PWM period is left unchanged)
	POKEYSDECL int PK_PWMUpdate(sPoKeysDevice* device);
	// Retrieve PWM configuration
	POKEYSDECL int PK_PWMConfigurationGet(sPoKeysDevice* device);


    POKEYSDECL int PK_PWMConfigurationSetDirectly(sPoKeysDevice * device, unsigned int PWMperiod, unsigned char * enabledChannels);
    POKEYSDECL int PK_PWMUpdateDirectly(sPoKeysDevice * device, unsigned int * dutyCycles);

	// Get LCD configuration
	POKEYSDECL int PK_LCDConfigurationGet(sPoKeysDevice* device);
	// Set LCD configuration
	POKEYSDECL int PK_LCDConfigurationSet(sPoKeysDevice* device);
	// Update LCD contents (only the lines with the refresh flag set)
	POKEYSDECL int PK_LCDUpdate(sPoKeysDevice* device);

	// Set matrix LED configuration
	POKEYSDECL int PK_MatrixLEDConfigurationSet(sPoKeysDevice* device);
	// Get matrix LED configuration
	POKEYSDECL int PK_MatrixLEDConfigurationGet(sPoKeysDevice* device);
	// Update matrix LED (only the displays with refresh flag set)
	POKEYSDECL int PK_MatrixLEDUpdate(sPoKeysDevice* device);

	// Get pulse engine information
	POKEYSDECL int PK_PEInfoGet(sPoKeysDevice* device);
    // Get pulse engine status (current position, limit/home status, axes status)
	POKEYSDECL int PK_PEStatusGet(sPoKeysDevice* device);
	// Set pulse engine status (to enable/disable the pulse engine and the charge pump)
	POKEYSDECL int PK_PEStatusSet(sPoKeysDevice* device);
	// Set pulse engine state
	POKEYSDECL int PK_PEStateSet(sPoKeysDevice* device);
	// Set current position
	POKEYSDECL int PK_PECurrentPositionSet(sPoKeysDevice* device);
	// Get pulse engine configuration for each axis (limit, home switches, directions) 
	POKEYSDECL int PK_PEAxisConfigurationGet(sPoKeysDevice* device);
	// Set pulse engine configuration for each axis (limit, home switches, directions) 
	POKEYSDECL int PK_PEAxisConfigurationSet(sPoKeysDevice* device);
	// Enable/Disable kbd48CNC
	POKEYSDECL int PK_PEKeyboardConfigurationGet(sPoKeysDevice* device);
	// Get enable/disable kbd48CNC status
	POKEYSDECL int PK_PEKeyboardConfigurationSet(sPoKeysDevice* device);
	// Start homing procedure for selected axes
	POKEYSDECL int PK_PEHomingStart(sPoKeysDevice* device);
	// Get pulse engine parameters (speeds, accelerations for each axis, homing speed and direction, emergency switch polarity)
	POKEYSDECL int PK_PEParametersGet(sPoKeysDevice* device);
	// Set pulse engine parameters (speeds, accelerations for each axis, homing speed and direction, emergency switch polarity)
	POKEYSDECL int PK_PEParametersSet(sPoKeysDevice* device);
	// Execute internal controller move action (only when internal controller is activated)
	POKEYSDECL int PK_PEMove(sPoKeysDevice* device);
	// Transfer motion buffer to device, also retrieves the free space in the device's motion buffer
	POKEYSDECL int PK_PEBufferFill(sPoKeysDevice* device);
	// Flush motion buffer in the device
	POKEYSDECL int PK_PEBufferFlush(sPoKeysDevice* device);
	// Retrieve the amount of free space in the device's motion buffer
	POKEYSDECL int PK_PEBufferFreeSizeGet(sPoKeysDevice* device);
	// Get configuration for MPG internal jogging
	POKEYSDECL int PK_PEMPGJogConfigurationGet(sPoKeysDevice* device);
	// Set configuration for MPG internal jogging
	POKEYSDECL int PK_PEMPGJogConfigurationSet(sPoKeysDevice* device);


    // I2C operations status return ePK_I2C_STATUS, described above
    // Set I2C status - does nothing in the device as I2C is ON all the time
    POKEYSDECL int PK_I2CSetStatus(sPoKeysDevice* device, int activated);
    // Retrieves I2C bus activation status
    POKEYSDECL int PK_I2CGetStatus(sPoKeysDevice* device, int* activated);
    // Execute write to the specified address. iDataLength specifies how many bytes should be sent from the buffer (0 to 32)
    POKEYSDECL int PK_I2CWriteStart(sPoKeysDevice* device, unsigned char address, unsigned char* buffer, unsigned char iDataLength);
    // Get write operation status
    POKEYSDECL int PK_I2CWriteStatusGet(sPoKeysDevice* device, int* status);
    // Execute read from the specified address. iDataLength specifies how many bytes should be requested
    POKEYSDECL int PK_I2CReadStart(sPoKeysDevice* device, unsigned char address, unsigned char iDataLength);
    // Get read operation results. iReadBytes returns the number of bytes read from the selected device, iMaxBufferLength specifies how many bytes buffer can accept
    POKEYSDECL int PK_I2CReadStatusGet(sPoKeysDevice* device, int* status, unsigned char* iReadBytes, unsigned char* buffer, unsigned char iMaxBufferLength);
    // Execute bus scan
    POKEYSDECL int PK_I2CBusScanStart(sPoKeysDevice* device);
    // Get bus scan results. iMaxDevices specifies how big presentDevices buffer is. presentDevices returns one entry per device
    POKEYSDECL int PK_I2CBusScanGetResults(sPoKeysDevice* device, int* status, unsigned char* presentDevices, unsigned char iMaxDevices);


    POKEYSDECL int PK_PoNETSetModuleStatus(sPoKeysDevice* device);

    // SPI operations
    POKEYSDECL int PK_SPIConfigure(sPoKeysDevice * device, unsigned char prescaler, unsigned char frameFormat);
    POKEYSDECL int PK_SPIWrite(sPoKeysDevice * device, unsigned char * buffer, unsigned char iDataLength, unsigned char pinCS);
    POKEYSDECL int PK_SPIRead(sPoKeysDevice * device, unsigned char * buffer, unsigned char iDataLength);


    // PoIL commands
    POKEYSDECL int PK_PoILGetState(sPoKeysDevice* device);
    POKEYSDECL int PK_PoILSetCoreState(sPoKeysDevice* device, unsigned short state);
    POKEYSDECL int PK_PoILSetMasterEnable(sPoKeysDevice* device, unsigned char masterEnable);
    POKEYSDECL int PK_PoILResetCore(sPoKeysDevice* device);
    POKEYSDECL int PK_PoILSetDebugMode(sPoKeysDevice* device, unsigned char debugMode, unsigned short breakpoint);
    POKEYSDECL int PK_PoILReadMemory(sPoKeysDevice* device, unsigned char memoryType, unsigned short address, unsigned short size, unsigned char * dest);
    POKEYSDECL int PK_PoILWriteMemory(sPoKeysDevice* device, unsigned char memoryType, unsigned short address, unsigned short size, unsigned char * src);
    POKEYSDECL int PK_PoILEraseMemory(sPoKeysDevice* device, unsigned char memoryType);
    POKEYSDECL int PK_PoILChunkReadMemory(sPoKeysDevice * device, unsigned char * dest);
    POKEYSDECL int PK_PoILChunkReadMemoryInternalAddress(sPoKeysDevice * device, unsigned char * dest);
    POKEYSDECL int PK_PoILReadSharedSlot(sPoKeysDevice* device, unsigned short firstSlotID, unsigned short slotsNum, int * dest);
    POKEYSDECL int PK_PoILWriteSharedSlot(sPoKeysDevice* device, unsigned short firstSlotID, unsigned short slotsNum, int * src);

    // RTC commands (real-time clock)
    POKEYSDECL int PK_RTCGet(sPoKeysDevice* device);
    POKEYSDECL int PK_RTCSet(sPoKeysDevice* device);

    extern int LastRetryCount;
    extern int LastWaitCount;
#ifdef __cplusplus
}
#endif

#endif
