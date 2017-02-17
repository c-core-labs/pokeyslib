# Python library for PoKeys devices
#
# Copyright (C) 2014 Matev\vz Bo\vsnak (matevz@poscope.com)
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

from ctypes import *
import sys
import time

# Pin capabilities / configuration
class ePK_PinCap():
    PK_PinCap_pinRestricted  = 0           # Pin is not used
    PK_PinCap_reserved       = 1           # --
    PK_PinCap_digitalInput   = 2           # Digital input
    PK_PinCap_digitalOutput  = 4           # Digital output
    PK_PinCap_analogInput    = 8           # Analog input (only on selected pins)
    PK_PinCap_analogOutput   = 16          # Analog output (only on selected pins)
    PK_PinCap_triggeredInput = 32          # Triggered input
    PK_PinCap_digitalCounter = 64          # Digital counter (only on selected pins)
    PK_PinCap_invertPin      = 128          # Invert digital pin polarity (set together with digital input 

# Pin capabilities / configuration
class ePK_AllPinCap():
    PK_AllPinCap_digitalInput = 1             # Digital input supported
    PK_AllPinCap_digitalOutput = 2            # Digital output supported
    PK_AllPinCap_analogInput = 3              # Analog input supported
    PK_AllPinCap_MFanalogInput = 4            # Analog input supported
    PK_AllPinCap_analogOutput = 5             # Analog output supported
    PK_AllPinCap_keyboardMapping = 6
    PK_AllPinCap_triggeredInput = 7           # Triggered input supported
    PK_AllPinCap_digitalCounter = 8           # Digital counter supported
    PK_AllPinCap_PWMOut = 9                   # PWM output supported
    PK_AllPinCap_fastEncoder1A = 10
    PK_AllPinCap_fastEncoder1B = 11
    PK_AllPinCap_fastEncoder1I = 12
    PK_AllPinCap_fastEncoder2A = 13
    PK_AllPinCap_fastEncoder2B = 14
    PK_AllPinCap_fastEncoder2I = 15
    PK_AllPinCap_fastEncoder3A = 16
    PK_AllPinCap_fastEncoder3B = 17
    PK_AllPinCap_fastEncoder3I = 18
    PK_AllPinCap_ultraFastEncoderA = 19
    PK_AllPinCap_ultraFastEncoderB = 20
    PK_AllPinCap_ultraFastEncoderI = 21
    PK_AllPinCap_LCD_E = 22
    PK_AllPinCap_LCD_RW = 23
    PK_AllPinCap_LCD_RS = 24
    PK_AllPinCap_LCD_D4 = 25
    PK_AllPinCap_LCD_D5 = 26
    PK_AllPinCap_LCD_D6 = 27
    PK_AllPinCap_LCD_D7 = 28
  
class ePK_DeviceTypeMask():
    PK_DeviceMask_Bootloader    = (1<<0)
    PK_DeviceMask_Bootloader55  = (1<<1)
    PK_DeviceMask_Bootloader56  = (1<<2)
    PK_DeviceMask_Bootloader56U = (1<<3)
    PK_DeviceMask_Bootloader56E = (1<<4)
    PK_DeviceMask_Bootloader58  = (1<<5)

    PK_DeviceMask_55            = (1<<10)
    PK_DeviceMask_55v1          = (1<<11)
    PK_DeviceMask_55v2          = (1<<12)
    PK_DeviceMask_55v3          = (1<<13)

    PK_DeviceMask_56            = (1<<14)
    PK_DeviceMask_56U           = (1<<15)
    PK_DeviceMask_56E           = (1<<16)
    PK_DeviceMask_27            = (1<<17)
    PK_DeviceMask_27U           = (1<<18)
    PK_DeviceMask_27E           = (1<<19)

    PK_DeviceMask_57            = (1<<20)
    PK_DeviceMask_57U                 = (1<<24)   
    PK_DeviceMask_57E                 = (1<<25)   
    PK_DeviceMask_57CNC               = (1<<26)   
    PK_DeviceMask_57CNCdb25           = (1<<27)   
    PK_DeviceMask_57Utest             = (1<<28)   
    PK_DeviceMask_58                  = (1<<21)   
    PK_DeviceMask_PoPLC58             = (1<<22)   
    PK_DeviceMask_PoKeys16RF          = (1<<23)   


class ePK_DeviceTypeID():
    PK_DeviceID_Bootloader55  = 3
    PK_DeviceID_Bootloader56U = 15
    PK_DeviceID_Bootloader56E = 16
    PK_DeviceID_Bootloader58  = 41

    PK_DeviceID_55v1          = 0
    PK_DeviceID_55v2          = 1
    PK_DeviceID_55v3          = 2

    PK_DeviceID_56U           = 10
    PK_DeviceID_56E           = 11
    PK_DeviceID_27U           = 20
    PK_DeviceID_27E           = 21
    PK_DeviceID_57U           = 30        
    PK_DeviceID_57E           = 31        
    PK_DeviceID_PoKeys57CNC   = 32
    PK_DeviceID_PoKeys57CNCdb25 = 38        
    PK_DeviceID_PoKeys57Utest   = 39        
    PK_DeviceID_57U_v0        = 28
    PK_DeviceID_57E_v0        = 29


    PK_DeviceID_58EU          = 40
    PK_DeviceID_PoPLC58       = 50


# Connection type
class ePK_DeviceConnectionType():
    PK_DeviceType_USBDevice     = 0
    PK_DeviceType_NetworkDevice = 1
    PK_DeviceType_FastUSBDevice       = 2         


class ePK_DeviceConnectionParam():
    PK_ConnectionParam_TCP = 0
    PK_ConnectionParam_UDP = 1


# Pulse engine state
class ePK_PEState():
    PK_PEState_peSTOPPED        = 0            # Pulse engine is stopped
    PK_PEState_peINTERNAL       = 1            # PEv1: Internal motion controller is in use  PEv2: not used
    PK_PEState_peBUFFER         = 2            # PEv1: Buffered operation mode is in use  PEv2: not used
    PK_PEState_peRUNNING        = 3            # Pulse engine is activated

    PK_PEState_peJOGGING        = 10           # Jogging mode enabled
    PK_PEState_peSTOPPING       = 11           # Pulse engine is stopping

    PK_PEState_peHOME           = 20           # All axes are homed
    PK_PEState_peHOMING         = 21           # Axes homing is in progress

    PK_PEState_pePROBECOMPLETE  = 30           # All axes are homed
    PK_PEState_pePROBE          = 31           # Axes probing is in progress
    PK_PEState_pePROBEERROR     = 32           # Error occured during probing

    PK_PEState_peHYBRIDPROBE_STOPPING = 40
    PK_PEState_peHYBRIDPROBE_COMPLETE = 41

    PK_PEState_peSTOP_LIMIT     = 100          # Pulse engine stopped due to limit reached
    PK_PEState_peSTOP_EMERGENCY = 101          # Pulse engine stopped due to emergency switch


# Pulse engine axis state
class ePK_PEAxisState():
    PK_PEAxisState_axSTOPPED      =  0         # Axis is stopped
    PK_PEAxisState_axREADY        =  1         # Axis ready
    PK_PEAxisState_axRUNNING      =  2         # Axis is running

    PK_PEAxisState_axHOME         =  10        # Axis is homed
    PK_PEAxisState_axHOMINGSTART  =  11        # Homing procedure is starting on axis
    PK_PEAxisState_axHOMINGSEARCH =  12        # Homing procedure first step - going to home
    PK_PEAxisState_axHOMINGBACK   =  13        # Homing procedure second step - slow homing

    PK_PEAxisState_axPROBED       =  14        # Probing completed for this axis
    PK_PEAxisState_axPROBESTART   =  15        # Probing procedure is starting on axis
    PK_PEAxisState_axPROBESEARCH  =  16        # Probing procedure - probing

    PK_PEAxisState_axERROR        =  20        # Axis error
    PK_PEAxisState_axLIMIT        =  30        # Axis limit tripped

class ePK_PEv2_AxisConfig():
    PK_AC_ENABLED            = (1 << 0)        # Axis enabled
    PK_AC_INVERTED           = (1 << 1)        # Axis inverted
    PK_AC_INTERNAL_PLANNER   = (1 << 2)        # Axis uses internal motion planner
    PK_AC_POSITION_MODE      = (1 << 3)        # Internal motion planner for this axis is in position mode
    PK_AC_INVERTED_HOME      = (1 << 4)        # Axis homing direction is inverted
    PK_AC_SOFT_LIMIT_ENABLED = (1 << 5)        # Use soft-limits for this axis
    PK_AC_ENABLED_MASKED     = (1 << 7)        # Use output enable pin masking


class ePK_PEv2_AxisSwitchOptions():
    PK_ASO_SWITCH_LIMIT_N        = (1 << 0)    # Limit- switch
    PK_ASO_SWITCH_LIMIT_P        = (1 << 1)    # Limit+ switch
    PK_ASO_SWITCH_HOME           = (1 << 2)    # Home switch
    PK_ASO_SWITCH_COMBINED_LN_H  = (1 << 3)    # Home switch is shared with Limit- switch
    PK_ASO_SWITCH_COMBINED_LP_H  = (1 << 4)    # Home switch is shared with Limit+ switch
    PK_ASO_SWITCH_INVERT_LIMIT_N = (1 << 5)    # Invert limit- switch polarity
    PK_ASO_SWITCH_INVERT_LIMIT_P = (1 << 6)    # Invert limit+ switch polarity
    PK_ASO_SWITCH_INVERT_HOME    = (1 << 7)    # Invert home switch polarity

# Return codes for various functions
class ePK_RETURN_CODES():
    PK_OK                   = 0 
    PK_ERR_GENERIC          = -1 
    PK_ERR_NOT_CONNECTED    = -5 
    PK_ERR_TRANSFER         = -10 
    PK_ERR_PARAMETER        = -20 
    PK_ERR_NOT_SUPPORTED              = -30       
    PK_ERR_CANNOT_CLAIM_USB = -100 
    PK_ERR_CANNOT_CONNECT   = -101

class ePK_I2C_STATUS():
    PK_I2C_STAT_ERR         = 0                # An error occured
    PK_I2C_STAT_OK          = 1                # All is OK
    PK_I2C_STAT_COMPLETE    = 1                # Operation complete
    PK_I2C_STAT_IN_PROGRESS = 0x10             # Operation still in progress

class ePK_LCD_MODE():
    PK_LCD_MODE_DIRECT      = 0
    PK_LCD_MODE_BUFFERED    = 1    

class sPoKeys_PinCapabilities(Structure):
    _fields_ = [
        ("cap", c_int32),
        ("pinStart", c_uint32),
        ("pinEnd", c_uint32),
        ("additionalCheck", c_uint32),
        ("devTypes", c_uint32)]

# PoKeys device information
class sPoKeysDevice_Info(Structure):
    _fields_ = [
        ("iPinCount", c_uint32),         # Number of pins, physically on the device
        ("iPWMCount", c_uint32),         # Number of pins that support PWM output
        ("iBasicEncoderCount", c_uint32),       # Number of basic encoders
        ("iEncodersCount", c_uint32),       # Number of encoder slots available
        ("iFastEncoders", c_uint32),       # Number of fast encoders supported
        ("iUltraFastEncoders", c_uint32),       # Number of available ultra fast encoders
        ("PWMinternalFrequency", c_uint32),     # Main PWM peripheral clock
        ("iAnalogInputs", c_uint32),       # Number of available analog inputs
        ("iKeyMapping", c_uint32),         # Device supports key mapping (acts as a USB keyboard)
        ("iTriggeredKeyMapping", c_uint32),       # Device supports triggered key mapping
        ("iKeyRepeatDelay", c_uint32),       # Device supports user customizable key repeat rates and delays
        ("iDigitalCounters", c_uint32),       # Device supports digital counters
        ("iJoystickButtonAxisMapping", c_uint32),     # Device supports mapping of joystick buttons
        ("iJoystickAnalogToDigitalMapping", c_uint32),   # Device supports mapping of analog inputs to digital keys
        ("iMacros", c_uint32),         # Device supports customizable macro sequences
        ("iMatrixKeyboard", c_uint32),       # Device supports matrix keyboard
        ("iMatrixKeyboardTriggeredMapping", c_uint32),   # Device supports matrix keyboard triggered key mapping
        ("iLCD", c_uint32),           # Device supports alphanumeric LCD display
        ("iMatrixLED", c_uint32),         # Device supports matrix LED display
        ("iConnectionSignal", c_uint32),       # Device supports connection signal output
        ("iPoExtBus", c_uint32),         # Device supports PoExtBus digital outputs
        ("iPoNET", c_uint32),         # Device supports PoNET bus devices
        ("iAnalogFiltering", c_uint32),       # Device supports analog inputs low-pass digital filtering
        ("iInitOutputsStart", c_uint32),       # Device supports initializing outputs at startup
        ("iprotI2C", c_uint32),         # Device supports I2C bus (master)
        ("iprot1wire", c_uint32),         # Device supports 1-wire bus (master)
        ("iAdditionalOptions", c_uint32),       # Device supports additional options with activation keys
        ("iLoadStatus", c_uint32),         # Device supports reporting load status
        ("iCustomDeviceName", c_uint32),       # Device supports specifying custom device names
        ("iPoTLog27support", c_uint32),       # Device supports PoTLog27 firmware
        ("iSensorList", c_uint32),         # Device supports sensor lists
        ("iWebInterface", c_uint32),       # Device supports web interface
        ("iFailSafeSettings", c_uint32),       # Device supports fail-safe mode
        ("iJoystickHATswitch", c_uint32),       # Device supports joystick HAT switch mapping
        ("iPulseEngine", c_uint32),         # Device supports Pulse engine
        ("iPulseEnginev2", c_uint32),         # Device supports Pulse engine v2
        ("iEasySensors", c_uint32),         # Device supports EasySensors
        ("reserved", c_uint32*3)]         # Placeholder


class sPoKeysPEv2info(Structure):
    _fields_ = [
        ("nrOfAxes", c_uint8),
        ("maxPulseFrequency", c_uint8),
        ("bufferDepth", c_uint8),
        ("slotTiming", c_uint8),

        ("reserved", c_uint8 * 4)]

# Pulse engine v2 structure...
class sPoKeysPEv2(Structure):
    _fields_ = [
        ("info", sPoKeysPEv2info),              # Pulse engine info

        ("AxesState", c_uint8*8),              # Axis states (bit-mapped) - see ePK_PEAxisState
        ("AxesConfig", c_uint8*8),             # Axis configuration - see ePK_PEv2_AxisConfig
        ("AxesSwitchConfig", c_uint8*8),       # Axis switch configuration - see ePK_PEv2_AxisSwitchOptions
        ("CurrentPosition", c_int32*8),        # Current position
        ("PositionSetup", c_int32*8),          # Position to be set as current position
        ("ReferencePositionSpeed", c_int32*8), # Reference position or speed (position or pulses/s)
        ("InvertAxisEnable", c_int8*8),       # Invert axis enable signal

        ("SoftLimitMaximum", c_int32*8),       # Soft limit maximum position
        ("SoftLimitMinimum", c_int32*8),       # Soft limit minimum position

        ("HomingSpeed", c_uint8*8),            # Homing speed per axis (in %)
        ("HomingReturnSpeed", c_uint8*8),      # Homing return speed per axis (in % of the homing speed)

        ("HomeOffsets", c_int32*8),            # Home position offset
        ("HomingAlgorithm", c_uint8*8),        # Homing algorithm configuration

        ("FilterLimitMSwitch", c_uint8*8),     # Digital filter for limit- switch
        ("FilterLimitPSwitch", c_uint8*8),     # Digital filter for limit+ switch
        ("FilterHomeSwitch", c_uint8*8),       # Digital filter for home switch

        ("ProbePosition", c_int32*8),          # Position where probe detected change
        ("ProbeMaxPosition", c_int32*8),       # Maximum position to travel to until stopping and returning error
        ("MaxSpeed", c_float*8),               # Maximum axis speed (in pulses per ms)
        ("MaxAcceleration", c_float*8),        # Maximum axis acceleration (in pulses/ms/ms)
        ("MaxDecceleration", c_float*8),       # Maximum axis deceleration (in pulses/ms/ms)


        ("MPGjogMultiplier", c_int32*8),       # MPG jog multiplier value
        ("MPGjogEncoder", c_uint8*8),          # MPG jog encoder ID
        ("PinHomeSwitch", c_uint8*8),          # Home switch pin (0 for external dedicated input)
        ("PinLimitMSwitch", c_uint8*8),        # Limit- switch pin (0 for external dedicated input)
        ("PinLimitPSwitch", c_uint8*8),        # Limit+ switch pin (0 for external dedicated input)
        ("AxisEnableOutputPins", c_uint8*8),   # Axis enabled output pin (0 for external dedicated output)
        ("reserved", c_uint8*56),              # Motion buffer entries - moved further down...
        ("ReservedSafety", c_uint8*8),

            # ------ 64-bit region boundary ------
        ("PulseEngineEnabled", c_uint8),        # Pulse engine enabled status, also number of enabled axes
        ("PulseGeneratorType", c_uint8),        # Pulse engine generator type (0: external, 1: internal 3ch)
        ("ChargePumpEnabled", c_uint8),         # Charge pump output enabled
        ("EmergencySwitchPolarity", c_uint8),   # Emergency switch polarity (set to 1 to invert)

        ("PulseEngineActivated", c_uint8),      # Pulse engine activation status
        ("LimitStatusP", c_uint8),              # Limit+ status (bit-mapped)
        ("LimitStatusN", c_uint8),              # Limit- status (bit-mapped)
        ("HomeStatus", c_uint8),                # Home status (bit-mapped)

        ("ErrorInputStatus", c_uint8),          # Stepper motor driver error inputs status (bit-mapped)
        ("MiscInputStatus", c_uint8),           # Miscelenous digital inputs...
        ("LimitOverride", c_uint8),             # Limit override status
        ("LimitOverrideSetup", c_uint8),        # Limit override configuration

            # State of pulse engine - see ePoKeysPEState
        ("PulseEngineState", c_uint8),

        ("AxisEnabledMask", c_uint8),           # Bit-mapped ouput enabled mask
        ("EmergencyInputPin", c_uint8),
        ("reserved2", c_uint8),

            # ------ 64-bit region boundary ------
        ("param1", c_uint8),                    # Parameter 1 value
        ("param2", c_uint8),
        ("param3", c_uint8),

        ("AxisEnabledStatesMask", c_uint8),     # Bit-mapped states, where axis enabled and charge pump signals are active
        ("PulseEngineStateSetup", c_uint8),     # Pulse engine new state configuration

        ("SoftLimitStatus", c_uint8),           # Bit-mapped soft-limit statuses per axes
        ("ExternalRelayOutputs", c_uint8),      # External relay outputs
        ("ExternalOCOutputs", c_uint8),         # External open-collector outputs
        ("PulseEngineBufferSize", c_uint8),     # Buffer size information...

        ("motionBufferEntriesAccepted", c_uint8),
        ("newMotionBufferEntries", c_uint8),

        ("HomingStartMaskSetup", c_uint8),      # Bit-mapped axes indexes to be homed
        ("ProbeStartMaskSetup", c_uint8),       # Bit-mapped axis indexes for probing

        ("ProbeInput", c_uint8),                # Probe input (0:disabled, 1-8:external inputs, 9+ Pin ID-9)
        ("ProbeInputPolarity", c_uint8),        # Probe input polarity
        ("ProbeStatus", c_uint8),               # Probe status (probe completion bit-mapped status)

                                                # ------ 64-bit region 
        ("MotionBuffer", c_uint8*448),          # Motion buffer entries

            # ------ 64-bit region boundary ------
        ("ProbeSpeed", c_float),                # Probe speed (ratio of the maximum speed)
        ("reservedf", c_float),

        ("BacklashWidth", c_uint16*8),          # Half of real backlash width
        ("BacklashRegister", c_int16*8),        # Current value of the backlash register
        ("BacklashAcceleration", c_uint8*8),    # in pulses per ms^2
        ("BacklashCompensationEnabled", c_uint8),
        ("reserved_back", c_uint8*3),

        ("TriggerPreparing", c_uint8),
        ("TriggerPrepared", c_uint8),
        ("TriggerPending", c_uint8),
        ("TriggerActive", c_uint8),

        ("SpindleSpeedEstimate", c_int32),
        ("SpindlePositionError", c_int32),
        ("SpindleRPM", c_uint32),

        ("DedicatedLimitNInputs", c_uint8),
        ("DedicatedLimitPInputs", c_uint8),
        ("DedicatedHomeInputs", c_uint8),
        ("TriggerIngnoredAxisMask", c_uint8)]     


# PoStep driver configuration
class sPoPoStepDriverConfig(Structure):
    _fields_ = [
        # Status
        ("SupplyVoltage", c_uint8),
        ("Temperature", c_uint8),
        ("InputStatus", c_uint8),
        ("DriverStatus", c_uint8),
        ("FaultStatus", c_uint8),
        ("UpdateState", c_uint8),

        # Settings
        ("DriverMode", c_uint8),
        ("StepMode", c_uint8),
        ("Current_FS", c_uint16),
        ("Current_Idle", c_uint16),
        ("Current_Overheat", c_uint16),
        ("TemperatureLimit", c_uint8),

        # Configuration
        ("AddressI2C", c_uint8),
        ("DriverType", c_uint8),
        ("UpdateConfig", c_uint8),

        ("reserved", c_uint8 * 6)]

# PoKeys-PoStep interface
class sPoKeysPoStepInterface(Structure):
        _fields_ = [
            ("drivers", sPoPoStepDriverConfig * 8),
            ("EnablePoStepCommunication", c_uint8),
            ("reserved", c_uint8 * 7)]

    

# Device-specific data of the PoKeys device
class sPoKeysDevice_Data(Structure):
    _fields_ = [
        ("DeviceTypeID", c_uint32),        # ePK_DeviceTypes ID
        ("SerialNumber", c_uint32),        # Serial number of the device
        ("DeviceName", c_char * 30),      # Device name (generic or user-specified)
        ("DeviceTypeName", c_char * 30),      # Device type name
        ("BuildDate", c_char * 12),      # Build date string
        ("ActivationCode", c_uint8 * 8),            # Activation code (when activating the device additional options)
        ("FirmwareVersionMajor", c_uint8),      # Major firmware version number v(1+[4-7]).([0-3]) - upper 4 bits plus 1 for first part, lower 4 bits for second part
        ("FirmwareVersionMinor", c_uint8),      # Minor firmware version number
        ("UserID", c_uint8),        # Device user ID
        ("DeviceType", c_uint8),        # Device type code
        ("ActivatedOptions", c_uint8),      # Additional activated options - bit 0 for Pulse engine
        ("DeviceLockStatus", c_uint8),      # Device lock status (if 1, device is locked)
        ("HWtype", c_uint8),              # HW type reported by the device
        ("FWtype", c_uint8),              # FW type reported by the device
        ("ProductID", c_uint8),              
        ("SecondaryFirmwareVersionMajor", c_uint8),   
        ("SecondaryFirmwareVersionMinor", c_uint8),   
        ("deviceIsBootloader", c_uint8),
        ("reserved", c_uint8*4)]


# Pin-specific data
class sPoKeysPinData(Structure):
    _fields_ = [
        ("DigitalCounterValue", c_uint32),             # Digital counter current value (on supported pins when PinFunction is set to digital counter - use PK_IsCounterAvailable to check the pin)
        ("AnalogValue", c_uint32),                     # Analog input value (on supported pins when PinFunction is set as analog input)
        ("PinFunction", c_uint8),                      # Pin function code - see ePK_PinCap for values
        ("CounterOptions", c_uint8),                   # Digital counter settings (on supported pins)
        ("DigitalValueGet", c_uint8),                  # Digital input value read
        ("DigitalValueSet", c_uint8),                  # Digital output value set
        ("DigitalCounterAvailable", c_uint8),          # 1 if digital counter is available on this pin
        ("MappingType", c_uint8),                      # Digital input to USB keyboard mapping type - selects between direct key mapping and mapping to macro
        ("KeyCodeMacroID", c_uint8),                   # USB keyboard key code or macro ID (depends on MappingType)
        ("KeyModifier", c_uint8),                      # USB keyboard key modifier
        ("downKeyCodeMacroID", c_uint8),               # USB keyboard down key code (for triggered mapping)
        ("downKeyModifier", c_uint8),                  # USB keyboard down key modifier (for triggered mapping)
        ("upKeyCodeMacroID", c_uint8),                 # USB keyboard up key code (for triggered mapping)
        ("upKeyModifier", c_uint8),                    # USB keyboard up key modifier (for triggered mapping)
        ("preventUpdate", c_uint8),                   
        ("reserved", c_uint8*3)]


# Encoder-specific data
class sPoKeysEncoder(Structure):
    _fields_ = [
        ("encoderValue", c_int32),    # Encoder current value
        ("encoderOptions", c_uint8),    # Encoder options -    bit 0: enable encoder 
                                              #            bit 1: 4x sampling 
                                              #      bit 2: 2x sampling 
                                              #      bit 3: reserved 
                                              #      bit 4: direct key mapping for direction A 
                                              #      bit 5: mapped to macro for direction A 
                                              #      bit 6: direct key mapping for direction B 
                                              #      bit 7: mapped to macro for direction B
        ("channelApin", c_uint8),    # Channel A encoder pin
        ("channelBpin", c_uint8),    # Channel B encoder pin
        ("dirAkeyCode", c_uint8),        # USB keyboard key code for direction A
        ("dirAkeyModifier", c_uint8),      # USB keyboard key modifier for direction A
        ("dirBkeyCode", c_uint8),        # USB keyboard key code for direction B
        ("dirBkeyModifier", c_uint8),      # USB keyboard key modifier for direction B
        ("reserved", c_uint8*5) ]        # placeholder

# PWM-specific data
class sPoKeysPWM(Structure):
    _fields_ = [
        ("PWMperiod", c_uint32),      # PWM period, shared among all channels
        ("reserved", c_uint32),
        ("PWMduty", POINTER(c_uint32)),          # PWM duty cycles (range between 0 and PWM period)
        ("PWMenabledChannels", POINTER(c_uint8)),  # List of enabled PWM channels
        ("PWMpinIDs", POINTER(c_uint8))]

# Matrix keyboard specific data
class sMatrixKeyboard(Structure):
    _fields_ = [
        ("matrixKBconfiguration", c_uint8),  # Matrix keyboard configuration (set to 1 to enable matrix keyboard support)
        ("matrixKBwidth", c_uint8),    # Matrix keyboard width (number of columns)
        ("matrixKBheight", c_uint8),    # Matrix keyboard height (number of rows)
        ("reserved", c_uint8*5),      # placeholder
        ("matrixKBcolumnsPins", c_uint8 * 8),  # List of matrix keyboard column connections
        ("matrixKBrowsPins", c_uint8 * 16),  # List of matrix keyboard row connections
        ("macroMappingOptions", c_uint8 * 128),    # Selects between direct key mapping and mapping to macro sequence for each key (assumes fixed width of 8 columns)
        ("keyMappingKeyCode", c_uint8 * 128),      # USB keyboard key code for each key (assumes fixed width of 8 columns", ), also down key code in triggered mapping mode
        ("keyMappingKeyModifier", c_uint8 * 128),  # USB keyboard key modifier, also down key modifier in triggered mapping mode (assumes fixed width of 8 columns)
        ("keyMappingTriggeredKey", c_uint8 * 128), # Selects between normal direct key mapping and triggered key mapping for each key (assumes fixed width of 8 columns)
        ("keyMappingKeyCodeUp", c_uint8 * 128),    # USB keyboard up key code in triggered mapping mode (assumes fixed width of 8 columns)
        ("keyMappingKeyModifierUp", c_uint8 * 128),# USB keyboard up key modifier in triggered mapping mode (assumes fixed width of 8 columns)
        ("matrixKBvalues", c_uint8 * 128)]      # Current state of each matrix keyboard key (assumes fixed width of 8 columns)


# LCD-specific data
class sPoKeysLCD(Structure):
    _fields_ = [
        ("Configuration", c_uint8),    # LCD configuration byte - 0: disabled, 1: enabled on primary pins, 2: enabled on secondary pins
        ("Rows", c_uint8),      # Number of LCD module rows
        ("Columns", c_uint8),      # Number of LCD module columns
        ("RowRefreshFlags", c_uint8),    # Flag for refreshing data - bit 0: row 1, bit 1: row 2, bit 2: row 3, bit 3: row 4

        ("reserved", c_uint8 * 4),

        ("line1", c_uint8 * 20),    # Line 1 buffer
        ("line2", c_uint8 * 20),    # Line 2 buffer
        ("line3", c_uint8 * 20),    # Line 3 buffer
        ("line4", c_uint8 * 20),    # Line 4 buffer
        ("customCharacters", c_uint8 * 64)]    # Buffer for custom characters

# Matrix LED specific data
class sPoKeysMatrixLED(Structure):
    _fields_ = [
        ("displayEnabled", c_uint8),    # Display enabled byte - set to 1 to enable the display
        ("rows", c_uint8),      # Number of Matrix LED rows
        ("columns", c_uint8),      # Number of Matrix LED columns
        ("RefreshFlag", c_uint8),    # Flag for refreshing data - set to 1 to refresh the display
        ("data", c_uint8 * 8),      # Matrix LED buffer - one byte per row (assumes 8 columns)
        ("reserved", c_uint8 * 4) ]


# PoNET module data
class sPoNETmodule(Structure):
    _fields_ = [
        ("statusIn", c_uint8*16),
        ("statusOut", c_uint8*16),
        ("moduleID", c_uint8),
        ("i2cAddress", c_uint8),
        ("moduleType", c_uint8),
        ("moduleSize", c_uint8),
        ("moduleOptions", c_uint8),
        ("PWMduty", c_uint8),
        ("lightValue", c_uint8),
        ("PoNETstatus", c_uint8) ]


# PoIL-related structures

# PoIL core info
class sPoILinfo(Structure):
    _fields_ = [
        ("DataMemorySize", c_uint32),
        ("CodeMemorySize", c_uint32),
        ("Version", c_uint32),
        ("reserved", c_uint32)]

# PoIL stack info
class sPoILStack(Structure):
    _fields_ = [
        ("stackPtr", c_uint32),
        ("stackSize", c_uint32),
        ("StackContents", c_uint8 * 1024)]

# Monitor mode memory chunk descriptor
class sPoILmemoryChunk(Structure):
    _fields_ = [
        ("address", c_uint16),
        ("chunkLength", c_uint8),
        ("reserved", c_uint8*5)]

class sPoILTask(Structure):
    _fields_ = [
        ("taskPeriod", c_uint16),
        ("taskRealPeriod", c_uint16),
        ("taskRealPeriodFiltered", c_uint16),
        ("taskStatus", c_uint8),
        ("taskLoad", c_uint8)]

# PoIL core status
class sPoILStatus(Structure):
        _fields_ = [
        ("info", sPoILinfo),

        ("MasterEnable", c_uint32),
        ("currentTask", c_uint32),
        ("STATUS", c_uint32),
        ("W", c_uint32),
        ("PC", c_uint32),
        ("ExceptionPC", c_uint32),
        ("CoreState", c_uint32),
        ("CoreDebugMode", c_uint32),
        ("CoreDebugBreakpoint", c_uint32),
        ("reserved0", c_uint32),

        ("functionStack", sPoILStack),
        ("dataStack", sPoILStack),

        ("codeMemoryPage", c_uint8*256),
        ("dataMemoryPage", c_uint8*256),

        ("monitorChunks", sPoILmemoryChunk*18),

        ("tasks", sPoILTask*32),
        ("inactiveLoad", c_uint8),
        ("taskCount", c_uint8),
        ("reserved", c_uint8*2)]



# EasySensor structure
class sPoKeysEasySensor(Structure):
    _fields_ = [
        ("sensorValue", c_int32),             # Current sensor value

        ("sensorType", c_uint8),              # Type of the sensor
        ("sensorRefreshPeriod", c_uint8),     # Refresh period in 0.1s
        ("sensorFailsafeConfig", c_uint8),    # Failsafe configuration (bits 0-5: timeout in seconds, bit 6: invalid=0, bit 7: invalid=0x7FFFFFFF)
        ("sensorReadingID", c_uint8),         # Sensor reading selection (see Protocol description document for details)
        ("sensorID", c_uint8 * 8),            # 8 byte sensor ID - see protocol specifications for details

        ("sensorOKstatus", c_uint8),          # Sensor OK status
        ("reserved", c_uint8 * 7)]


# Custom sensor unit descriptor
class sPoKeysCustomSensorUnit(Structure):
    _fields_ = [
    ("HTMLcode", c_uint8 * 32),            # 32 character custom sensor unit HTML code
    ("simpleText", c_uint8 * 8)]           # 8 character custom sensor unit text




class sPoKeysRTC(Structure):
    _fields_ = [
        ("SEC", c_uint8),
        ("MIN", c_uint8),
        ("HOUR", c_uint8),
        ("DOW", c_uint8),
        ("DOM", c_uint8),
        ("tmp", c_uint8),
        ("DOY", c_uint16),
        ("MONTH", c_uint16),
        ("YEAR", c_uint16),
        ("reserved", c_uint32)]
# CAN message structure
class sPoKeysCANmsg(Structure):
    _fields_ = [
        ("id", c_uint32),                             
        ("data", c_uint8*8),                          
        ("len", c_uint8),                             
        ("format", c_uint8),                          
        ("type", c_uint8),                            
    ]



# Network device info structure - used for network device information/setup
class sPoKeysNetworkDeviceInfo(Structure):
    _fields_ = [
        ("IPAddressCurrent", c_uint8 * 4),      # Current (temporary) address may differ from the setup one
        ("IPAddressSetup", c_uint8 * 4),        # Setup IP address
        ("Subnetmask", c_uint8 * 4),            # Subnet mask
        ("DefaultGateway", c_uint8 * 4),        # Default gateway
        ("TCPtimeout", c_uint16),                # TCP timeout value
        ("AdditionalNetworkOptions", c_uint8),  # Additional network options (see PoKeysLib.h for more info)
        ("DHCP", c_uint8)]                      # DHCP setting of the device


# Other device peripheral data
class sPoKeysOtherPeripherals(Structure):
    _fields_ = [
        ("AnalogRCFilter", c_uint32) ]


# Main PoKeys structure
class sPoKeysDevice(Structure):
    _fields_ = [
        ("devHandle", c_void_p),        # Communication device handle
        ("devHandle2", c_void_p),        # Communication device handle
        
        ("info", sPoKeysDevice_Info),      # PoKeys device info
        ("DeviceData", sPoKeysDevice_Data),      # PoKeys device-specific data
        ("netDeviceData", c_void_p),
        
        ("Pins", POINTER(sPoKeysPinData)),      # PoKeys pins
        ("Encoders", POINTER(sPoKeysEncoder)),    # PoKeys encoders
        
        ("matrixKB", sMatrixKeyboard),      # Matrix keyboard structure
        ("PWM", sPoKeysPWM),        # PWM outputs structure
        ("MatrixLED", POINTER(sPoKeysMatrixLED)),    # Matrix LED structure
        ("LCD", sPoKeysLCD),        # LCD structure
        ("PEv2", sPoKeysPEv2),    # Pulse engine v2 structure        
        ("PoSteps", sPoKeysPoStepInterface), # PoKeys-PoStep interface

        ("PoNETmodule", sPoNETmodule),  
        ("PoIL", sPoILStatus),  
        ("RTC", sPoKeysRTC),

        ("EasySensors", POINTER(sPoKeysEasySensor)),    # EasySensors array

        ("otherPeripherals", sPoKeysOtherPeripherals),
        
        ("FastEncodersConfiguration", c_uint8),    # Fast encoders configuration, invert settings and 4x sampling (see protocol specification for details)
        ("FastEncodersOptions", c_uint8),      # Fast encoders additional options
        ("UltraFastEncoderConfiguration", c_uint8),          # Ultra fast encoder configuration (see protocol specification for details)
        ("UltraFastEncoderOptions", c_uint8),    # Ultra fast encoder additional options
        ("UltraFastEncoderFilter", c_uint32),      # Ultra fast encoder digital filter setting
        
        ("PoExtBusData", c_void_p),              # PoExtBus outputs buffer
        
        ("connectionType", c_uint8),      # Connection type
        ("connectionParam", c_uint8),      # Additional connection parameter
        
        ("requestID", c_uint8),        # Communication request ID
        ("reserved", c_uint8),
        
        ("sendRetries", c_uint32),
        ("readRetries", c_uint32),
        ("socketTimeout", c_uint32),

        ("request", c_uint8*68),        # Communication buffer
        ("response", c_uint8*68),        # Communication buffer

        ("multiPartData", c_uint8*448),        # Communication buffer
        ("reserved64", c_uint64),
        ("multiPartBuffer", c_void_p)]
        

sPoKeysDevicePtr = POINTER(sPoKeysDevice)

# Network device structure - used for network device enumeration
class sPoKeysNetworkDeviceSummary(Structure):
    _fields_ = [
        ("SerialNumber", c_long),      # Serial number
        ("IPaddress", c_ubyte*4),      # IP address of the device
        ("hostIP", c_ubyte*4),      # IP address of the host PC
        ("FirmwareVersionMajor", c_ubyte),    # Firmware version - major
        ("FirmwareVersionMinor", c_ubyte),    # Firmware version - minor
        ("UserID", c_ubyte),      # User ID
        ("DHCP", c_ubyte),        # DHCP setting of the device
        ("HWtype", c_ubyte),        # HW type, reported by device
        ("useUDP", c_ubyte)]        # If set to 1, UDP connection will be established with the device

class PoKeysDevice:
    def __init__(self, dllPath):
        self.libObj = cdll.LoadLibrary(dllPath)
        self.device = 0
        self.libObj.PK_EnumerateUSBDevices()

    def __del__(self):
        self.libObj.PK_DisconnectDevice(self.device)

    def ShowAllDevices(self):
        devConnect = self.libObj.PK_ConnectToDevice
        devConnect.restype = sPoKeysDevicePtr
        
        i = self.libObj.PK_EnumerateUSBDevices()

        for k in range(0, i):
            testDev = devConnect(k)

            try:
                print("Device " + str(k) + ": " + str(testDev.contents.DeviceData.DeviceName.decode("ascii")) + " (" + str(testDev.contents.DeviceData.DeviceTypeName.decode("ascii")) + ")")
                print(" Serial number: " + str(testDev.contents.DeviceData.SerialNumber))
                print(" User ID: " + str(testDev.contents.DeviceData.UserID))
                print(" Firmware version: " + str(int(1 + testDev.contents.DeviceData.FirmwareVersionMajor / 16)) + "." +
                                              str(testDev.contents.DeviceData.FirmwareVersionMajor % 16) + "." +
                                              str(testDev.contents.DeviceData.FirmwareVersionMinor) + " [" + str(testDev.contents.DeviceData.BuildDate.decode("ascii")) + "]")
                
            except ValueError:
                print("Requested device not found!")

            self.libObj.PK_DisconnectDevice(testDev)
            
    def PK_ConnectToDeviceWSerial(self, serial, checkEthernet = 0, useUDP = True):
        self.Disconnect()

        if useUDP:
            devConnect = self.libObj.PK_ConnectToDeviceWSerial_UDP
        else:
            devConnect = self.libObj.PK_ConnectToDeviceWSerial

        devConnect.restype = sPoKeysDevicePtr

        self.device = devConnect(serial, checkEthernet)

        try:
            print("Connected to device " + str(self.device.contents.DeviceData.DeviceName.decode("ascii")))
            return 0
        except ValueError:
            print("Requested device not found!")
            return -1

    def PK_ConnectToDevice(self, index):
        self.Disconnect()

        devConnect = self.libObj.PK_ConnectToDevice
        devConnect.restype = sPoKeysDevicePtr

        self.device = devConnect(index)

        try:
            print("Connected to device " + str(self.device.contents.DeviceData.DeviceName.decode("ascii")))
            return 0
        except ValueError:
            print("Requested device not found!")
            return -1


    def Disconnect(self):
        if self.device != 0:
            self.libObj.PK_DisconnectDevice(self.device)
            self.device = 0

    def PK_GetCurrentDeviceConnectionType(self):
        return self.libObj.PK_GetCurrentDeviceConnectionType(self.device)
    
    def PK_SaveConfiguration(self):
        return self.libObj.PK_SaveConfiguration(self.device)

    def PK_ClearConfiguration(self):
        return self.libObj.PK_ClearConfiguration(self.device)



    # Retrieve device-specific information (this also gets automatically called when the connection with the device is established)
    def PK_DeviceDataGet(self):
        return self.libObj.PK_DeviceDataGet(self.device)

        

    # Retrieve pin configuration from the device
    def PK_PinConfigurationGet(self):
        return self.libObj.PK_PinConfigurationGet(self.device)
    # Send pin configuration to device
    def PK_PinConfigurationSet(self):
        return self.libObj.PK_PinConfigurationSet(self.device)

        

    # Retrieve encoder configuration from the device
    def PK_EncoderConfigurationGet(self):
        return self.libObj.PK_EncoderConfigurationGet(self.device)
    # Send encoder configuration to device
    def PK_EncoderConfigurationSet(self):
        return self.libObj.PK_EncoderConfigurationSet(self.device)
    # Retrieve encoder values from device
    def PK_EncoderValuesGet(self):
        return self.libObj.PK_EncoderValuesGet(self.device)
    # Send encoder values to device
    def PK_EncoderValuesSet(self):
        return self.libObj.PK_EncoderValuesSet(self.device)

    # Set digital outputs values
    def PK_DigitalIOSet(self):
        return self.libObj.PK_DigitalIOSet(self.device)
    # Get digital inputs values
    def PK_DigitalIOGet(self):
        return self.libObj.PK_DigitalIOGet(self.device)
    # Set digital outputs and get digital input values in one call
    def PK_DigitalIOSetGet(self):
        return self.libObj.PK_DigitalIOSetGet(self.device)
    # Set single digital output
    def PK_DigitalIOSetSingle(self, pinID, pinValue):
        return self.libObj.PK_DigitalIOSetSingle(self.device, pinID, pinValue)
    # Get single digital input value
    def PK_DigitalIOGetSingle(self, pinID):
        pinValue = c_uint8(0)
        self.libObj.PK_DigitalIOGetSingle(self.device, pinID, POINTER(pinValue))
        return pinValue

    # Set PoExtBus outputs
    def PK_PoExtBusSet(self):
        return self.libObj.PK_PoExtBusSet(self.device)
    # Get current PoExtBus outputs values
    def PK_PoExtBusGet(self):
        return self.libObj.PK_PoExtBusGet(self.device)


    # Get digital counter values
    def PK_DigitalCounterGet(self):
        return self.libObj.PK_DigitalCounterGet(self.device)
    # Check whether digital counter is available for the specified pin. Return True if digital counter is supported.
    def PK_IsCounterAvailable(self, pinID):
        return self.libObj.PK_IsCounterAvailable(self.device, pinID)

    # Get analog input values
    def PK_AnalogIOGet(self):
        return self.libObj.PK_AnalogIOGet(self.device)
    # Get analog filter configuration
    def PK_AnalogRCFilterGet(self):
        return self.libObj.PK_AnalogRCFilterGet(self.device)
    # Set analog filter configuration
    def PK_AnalogRCFilterSet(self):
        return self.libObj.PK_AnalogRCFilterSet(self.device)

    # Get matrix keyboard configuration
    def PK_MatrixKBConfigurationGet(self):
        return self.libObj.PK_MatrixKBConfigurationGet(self.device)
    # Set matrix keyboard configuration
    def PK_MatrixKBConfigurationSet(self):
        return self.libObj.PK_MatrixKBConfigurationSet(self.device)
    # Get matrix keyboard current key states
    def PK_MatrixKBStatusGet(self):
        return self.libObj.PK_MatrixKBStatusGet(self.device)


    # Set PWM outputs configuration
    def PK_PWMConfigurationSet(self):
        return self.libObj.PK_PWMConfigurationSet(self.device)
    # Update PWM output duty cycles (PWM period is left unchanged)
    def PK_PWMUpdate(self):
        return self.libObj.PK_PWMUpdate(self.device)
    # Retrieve PWM configuration
    def PK_PWMConfigurationGet(self):
        return self.libObj.PK_PWMConfigurationGet(self.device)
        
    def PK_PWMConfigurationSetDirectly(self, period, dutyCycles):        
        channelData = c_ubyte * 8

        # Copy the Python array to c_byte array
        buf = channelData()
        for i in range(0, 8):
            buf[i] = dutyCycles[i]

        return self.libObj.PK_PWMConfigurationSetDirectly(self.device, period, buf)

    def PK_PWMUpdateDirectly(self, dutyCycles):
        channelData = c_ubyte * 8

        # Copy the Python array to c_byte array
        buf = channelData()
        for i in range(0, 8):
            buf[i] = dutyCycles[i]
        
        return self.libObj.PK_PWMUpdateDirectly(self.device, buf)


    # Get LCD configuration
    def PK_LCDConfigurationGet(self):
        return self.libObj.PK_LCDConfigurationGet(self.device)
    # Set LCD configuration
    def PK_LCDConfigurationSet(self):
        return self.libObj.PK_LCDConfigurationSet(self.device)
    # Update LCD contents (only the lines with the refresh flag set)
    def PK_LCDUpdate(self):
        return self.libObj.PK_LCDUpdate(self.device)
    # Transfer custom characters from device->LCD.customCharacters array
    def PK_LCDSetCustomCharacters(self):
        return self.libObj.PK_LCDSetCustomCharacters(self.device)
    # Change between modes PK_LCD_MODE_DIRECT and PK_LCD_MODE_BUFFERED
    def PK_LCDChangeMode(self, mode):
        return self.libObj.PK_LCDChangeMode(self.device, mode)

    # !!!!!!!!!!
    # The following LCD-related functions can be used in PK_LCD_MODE_DIRECT mode only
    # !!!!!!!!!!

    # Initialize LCD module
    def PK_LCDInit(self):
        return self.libObj.PK_LCDInit(self.device)
    # Clear LCD screen contents
    def PK_LCDClear(self):
        return self.libObj.PK_LCDClear(self.device)
    # Move cursor to the specified position
    def PK_LCDMoveCursor(self, row, column):
        return self.libObj.PK_LCDMoveCursor(self.device, row, column)
    # Print string to LCD
    def PK_LCDPrint(self, text):
        # Convert text to bytes
        bytes = str.encode(text)
        return self.libObj.PK_LCDPrint(self.device, bytes, len(text))
    # Put single character on LCD
    def PK_LCDPutChar(self, character):
        return self.libObj.PK_LCDPutChar(self.device, character)
    # Change LCD entry mode register
    def PK_LCDEntryModeSet(self, cursorMoveDirection, displayShift):
        return self.libObj.PK_LCDEntryModeSet(self.device, cursorMoveDirection, displayShift)
    # Change LCD display on/off control register
    def PK_LCDDisplayOnOffControl(self, displayOnOff, cursorOnOff, cursorBlinking):
        return self.libObj.PK_LCDDisplayOnOffControl(self.device, displayOnOff, cursorOnOff, )



    # Set matrix LED configuration
    def PK_MatrixLEDConfigurationSet(self):
        return self.libObj.PK_MatrixLEDConfigurationSet(self.device)
    # Get matrix LED configuration
    def PK_MatrixLEDConfigurationGet(self):
        return self.libObj.PK_MatrixLEDConfigurationGet(self.device)
    # Update matrix LED (only the displays with refresh flag set)
    def PK_MatrixLEDUpdate(self):
        return self.libObj.PK_MatrixLEDUpdate(self.device)



    # Get status of Pulse engine
    def PK_PEv2_StatusGet(self):
        return self.libObj.PK_PEv2_StatusGet(self.device)
    # Get additional status data
    def PK_PEv2_Status2Get(self):
        return self.libObj.PK_PEv2_Status2Get(self.device)
    # Configure (setup) the pulse engine
    def PK_PEv2_PulseEngineSetup(self):
        return self.libObj.PK_PEv2_PulseEngineSetup(self.device)
    # Retrieve single axis parameters. Axis ID is in param1
    def PK_PEv2_AxisConfigurationGet(self):
        return self.libObj.PK_PEv2_AxisConfigurationGet(self.device)
    # Set single axis parameters. Axis ID is in param1
    def PK_PEv2_AxisConfigurationSet(self):
        return self.libObj.PK_PEv2_AxisConfigurationSet(self.device)
    # Set positions - param2 is used for bit-mapped axis selection
    def PK_PEv2_PositionSet(self):
        return self.libObj.PK_PEv2_PositionSet(self.device)
    # Set pulse engine state
    def PK_PEv2_PulseEngineStateSet(self):
        return self.libObj.PK_PEv2_PulseEngineStateSet(self.device)
    # Execute the move. Position or speed is specified by the ReferencePositionSpeed
    def PK_PEv2_PulseEngineMove(self):
        return self.libObj.PK_PEv2_PulseEngineMove(self.device)
    # Read external outputs state - save them to ExternalRelayOutputs and ExternalOCOutputs
    def PK_PEv2_ExternalOutputsGet(self):
        return self.libObj.PK_PEv2_ExternalOutputsGet(self.device)
    # Set external outputs state (from ExternalRelayOutputs and ExternalOCOutputs)
    def PK_PEv2_ExternalOutputsSet(self):
        return self.libObj.PK_PEv2_ExternalOutputsSet(self.device)
    # Transfer motion buffer to device. The number of new entries (newMotionBufferEntries) must be specified
    # The number of accepted entries is saved to motionBufferEntriesAccepted.
    # In addition, pulse engine state is read (PEv2_GetStatus)
    def PK_PEv2_BufferFill(self):
        return self.libObj.PK_PEv2_BufferFill(self.device)
    def PK_PEv2_BufferFillLarge(self):
        return self.libObj.PK_PEv2_BufferFillLarge(self.device)
    # Clear motion buffer in device
    def PK_PEv2_BufferClear(self):
        return self.libObj.PK_PEv2_BufferClear(self.device)
    # Reboot pulse engine v2
    def PK_PEv2_PulseEngineReboot(self):
        return self.libObj.PK_PEv2_PulseEngineReboot(self.device)
    # Start the homing procedure. Home offsets must be provided in the HomeOffsets
    # Axes to home are selected as bit-mapped HomingStartMaskSetup value
    def PK_PEv2_HomingStart(self):
        return self.libObj.PK_PEv2_HomingStart(self.device)
    # Finish the homing procedure
    def PK_PEv2_HomingFinish(self):
        return self.libObj.PK_PEv2_HomingFinish(self.device)
    # Star the probing procedure.
    # ProbeMaxPosition defines the maximum position in position ticks where probing error will be thrown
    # ProbeSpeed defines the probing speed (1 = max speed)
    # ProbeInput defines the extenal input (values 1-8) or PoKeys pin (0-based Pin ID + 9)
    # ProbeInputPolarity defines the polarity of the probe signal
    def PK_PEv2_ProbingStart(self):
        return self.libObj.PK_PEv2_ProbingStart(self.device)
    def PK_PEv2_ProbingHybridStart(self):
        return self.libObj.PK_PEv2_ProbingHybridStart(self.device)
    # Finish the probing procedure. Probe position and status are saved to ProbePosition and ProbeStatus
    def PK_PEv2_ProbingFinish(self):
        return self.libObj.PK_PEv2_ProbingFinish(self.device)
    # Same as previous command, except for pulse engine states not being reset to 'STOPPED'
    def PK_PEv2_ProbingFinishSimple(self):
        return self.libObj.PK_PEv2_ProbingFinishSimple(self.device)

    def PK_PEv2_ThreadingPrepareForTrigger(self):
        return self.libObj.PK_PEv2_ThreadingPrepareForTrigger(self.device)
    def PK_PEv2_ThreadingForceTriggerReady(self):
        return self.libObj.PK_PEv2_ThreadingForceTriggerReady(self.device)
    def PK_PEv2_ThreadingTrigger(self):
        return self.libObj.PK_PEv2_ThreadingTrigger(self.device)
    def PK_PEv2_ThreadingRelease(self):
        return self.libObj.PK_PEv2_ThreadingRelease(self.device)
    def PK_PEv2_ThreadingStatusGet(self):
        return self.libObj.PK_PEv2_ThreadingStatusGet(self.device)
    def PK_PEv2_ThreadingCancel(self):
        return self.libObj.PK_PEv2_ThreadingCancel(self.device)
    def PK_PEv2_ThreadingSetup(self, sensorMode, ticksPerRevolution, tagetSpindleRPM):
        return self.libObj.PK_PEv2_ThreadingSetup(self.device, sensorMode, ticksPerRevolution,)

    def PK_PEv2_BacklashCompensationSettings_Get(self):
        return self.libObj.PK_PEv2_BacklashCompensationSettings_Get(self.device)
    def PK_PEv2_BacklashCompensationSettings_Set(self):
        return self.libObj.PK_PEv2_BacklashCompensationSettings_Set(self.device)


    # Get the configuration of EasySensors
    def PK_EasySensorsSetupGet(self):
        return self.libObj.PK_EasySensorsSetupGet(self.device)
    # Set the configuration of EasySensors
    def PK_EasySensorsSetupSet(self):
        return self.libObj.PK_EasySensorsSetupSet(self.device)
    # Get all EasySensors values
    def PK_EasySensorsValueGetAll(self):
        return self.libObj.PK_EasySensorsValueGetAll(self.device)





    def PK_EasySensorConfigure_1wire(self, slot, pinID, ROM, readingID, period, failsafe):
        if slot >= self.device.contents.info.iEasySensors:
            return False

        S = self.device.contents.EasySensors[slot]

        if ROM[0] == 0x10: # DS18S20
            S.sensorType = 0x18
        elif ROM[0] == 0x28: # DS18B20
            S.sensorType = 0x19
        elif ROM[0] == 0x3A: # DS2413
            S.sensorType = 0x1A
        else:
            # Unknown type
            return False

        S.sensorID[0] = pinID
        for i in range(1,8):
            S.sensorID[i] = ROM[i]

        S.sensorFailsafeConfig = failsafe
        S.sensorReadingID = readingID
        S.sensorRefreshPeriod = period

        return self.PK_EasySensorsSetupSet() == ePK_RETURN_CODES.PK_OK

        
    # I2C operations status return ePK_I2C_STATUS, described above
    # Set I2C status - does nothing in the device as I2C is ON all the time
    def PK_I2CSetStatus(self, activated):
        self.libObj.PK_I2CSetStatus(self.device, activated)
    
    
    # Retrieves I2C bus activation status
    def PK_I2CGetStatus(self):
        status = c_uint8(0)
        self.libObj.PK_I2CGetStatus(self.device, POINTER(status))
        
        return status
        
    # Execute write to the specified address. iDataLength specifies how many bytes should be sent from the buffer (0 to 32)
    def PK_I2CWriteStart(self, address, buffer):
    
        # If user provided only single integer, transfer it...
        if isinstance(buffer, int):
          numBytes = 1
          I2Cdata = c_ubyte * 1
          buf = I2Cdata(buffer)
          
        else:
          # Construct a c_byte array of numBytes bytes
          numBytes = len(buffer)
          I2Cdata = c_ubyte * numBytes
          
          # Copy the Python array to c_byte array
          buf = I2Cdata()
          for i in range(0, numBytes):
            buf[i] = buffer[i]
            
    
        self.libObj.PK_I2CWriteStart(self.device, address, buf, numBytes)
    
    # Get write operation status
    def PK_I2CWriteStatusGet(self):
        status = c_uint8(0)

        func = self.libObj.PK_I2CWriteStatusGet
        func.argtypes = [sPoKeysDevicePtr, POINTER(c_uint8)]
                
        func(self.device, status)
        return status.value

    # Execute read from the specified address. iDataLength specifies how many bytes should be requested
    def PK_I2CReadStart(self, address, iDataLength):
        self.libObj.PK_I2CReadStart(self.device, address, iDataLength)
    
    # Get read operation results. iReadBytes returns the number of bytes read from the selected device, iMaxBufferLength specifies how many bytes buffer can accept
    def PK_I2CReadStatusGet(self):
        bufferLen = 32
        bufferType = c_ubyte * bufferLen
        buffer = bufferType()
        ReadLen = c_ubyte(0)

        status = c_uint8(0)

        func = self.libObj.PK_I2CReadStatusGet
        func.argtypes = [sPoKeysDevicePtr, POINTER(c_uint8), POINTER(c_uint8), POINTER(bufferType), c_uint8]
        
        func(self.device, status, ReadLen, buffer, bufferLen)
        
        return status.value, [buffer[i] for i in range(0, ReadLen.value)]


    def PK_I2CWrite(self, address, data):
        self.PK_I2CWriteStart(address, data)

        # Wait for response
        while True:
          status = self.PK_I2CWriteStatusGet()

          if status == 0x10:
            time.sleep(0.01)
          elif status == 0x01:
            break
          else:
            print("Error - returned " + hex(status))
            break

        return 1

    def PK_I2CRead(self, address, dataNum):
        self.PK_I2CReadStart(address, dataNum)

        # Wait for response
        while True:
          status, data = self.PK_I2CReadStatusGet()

          if status == 0x10:
            time.sleep(0.01)
          elif status == 0x01:
            return data
          else:
            print("Error - returned " + hex(status))
            return []

        return [] 
    
    
    # Execute bus scan
    def PK_I2CBusScanStart(self):
        self.libObj.PK_I2CBusScanStart(self.device)
    
    # Get bus scan results. iMaxDevices specifies how big presentDevices buffer is. presentDevices returns one entry per device
    def PK_I2CBusScanGetResults(self):
        status = c_uint8(0)
        deviceList = c_uint8 * 128
        presentDevices = deviceList()

        func = self.libObj.PK_I2CBusScanGetResults
        func.argtypes = [sPoKeysDevicePtr, POINTER(c_uint8), POINTER(deviceList), c_uint8]
    
        func(self.device, status, presentDevices, 128)
        return [presentDevices[i] for i in range(0, 128)]




    def PK_PoNETGetPoNETStatus(self):
        return self.libObj.PK_PoNETGetPoNETStatus(self.device)
    def PK_PoNETGetModuleSettings(self):
        return self.libObj.PK_PoNETGetModuleSettings(self.device)
    def PK_PoNETGetModuleStatusRequest(self):
        return self.libObj.PK_PoNETGetModuleStatusRequest(self.device)
    def PK_PoNETGetModuleStatus(self):
        return self.libObj.PK_PoNETGetModuleStatus(self.device)
    def PK_PoNETSetModuleStatus(self):
        return self.libObj.PK_PoNETSetModuleStatus(self.device)
    def PK_PoNETSetModulePWM(self):
        return self.libObj.PK_PoNETSetModulePWM(self.device)
    def PK_PoNETGetModuleLightRequest(self):
        return self.libObj.PK_PoNETGetModuleLightRequest(self.device)
    def PK_PoNETGetModuleLight(self):
        return self.libObj.PK_PoNETGetModuleLight(self.device)

    # 1-wire operations
    # Set 1-wire activation status
    def PK_1WireStatusSet(self, activated):
        return self.libObj.PK_1WireStatusSet(self.device, activated)
    # Get 1-wire activation status
    def PK_1WireStatusGet(self):
        status = c_uint8(0)
        self.libObj.PK_1WireStatusGet(self.device, POINTER(status))
        return status


    # Start 1-wire write and read operation
    def PK_1WireWriteReadStart(self, read_count, data, pinID = 0):
        # If user provided only single integer, transfer it...
        if isinstance(data, int):
            numBytes = 1
            wireData = c_ubyte * 1
            buf = wireData(data)
          
        else:
            # Construct a c_byte array of numBytes bytes
            numBytes = len(data)
            wireData = c_ubyte * numBytes

            # Copy the Python array to c_byte array
            buf = wireData()
            for i in range(0, numBytes):
                buf[i] = data[i]

        return self.libObj.PK_1WireWriteReadStartEx(self.device, pinID, numBytes, read_count, buf)

    # Get the result of the read operation
    def PK_1WireReadStatusGet(self):
        bufferLen = 32
        bufferType = c_ubyte * bufferLen
        buffer = bufferType()
        ReadLen = c_ubyte(0)

        status = c_uint8(0)

        func = self.libObj.PK_1WireReadStatusGet
        func.argtypes = [sPoKeysDevicePtr, POINTER(c_uint8), POINTER(c_uint8), POINTER(bufferType), c_uint8]

        func(self.device, status, ReadLen, buffer)

        return status.value, [buffer[i] for i in range(0, ReadLen.value)]

        
    # Get the result of the read operation
    def PK_1WireRead(self):
        # Wait for response
        while True:
            status, data = self.PK_1WireReadStatusGet()

            if status == 0x10:
                time.sleep(0.01)
            elif status == 0x01:
                return data
            else:
                print("Error - returned " + hex(status))
            return []
        return []

    def PK_1WireScan(self, pinID, retries = 5):
        ROMs = []

        for r in range(retries):
            scanROMs = self.PK_1WireScan_int(pinID)

            # Check each found device
            for i in range(len(scanROMs)):
                tmp = scanROMs[i]
                if tmp[7] != self.GetDallasCRC(tmp, 7) or tmp[0] == 0:
                    # CRC is not correct...
                    continue

                # Check if tmp is already in ROMs
                result = False
                for e in range(len(ROMs)):
                    if all([ROMs[e][i] == tmp[i] for i in range(8)]):
                        result = True
                        break

                if result == False:
                    ROMs.append(tmp)

                time.sleep(0.1)

        return ROMs

    # Scan for 1-wire devices on the selected pin
    def PK_1WireScan_int(self, pinID):
        # Stop any previous scans
        if self.libObj.PK_1WireBusScanStop(self.device) != ePK_RETURN_CODES.PK_OK:
            return []

        # Start the scan
        status = self.libObj.PK_1WireBusScanStart(self.device, pinID)
        if status != ePK_RETURN_CODES.PK_OK:
            return []

        t0 = time.time()
        sensors = []

        # Scan for up to 2 seconds
        while time.time() - t0 < 2:
            time.sleep(0.05)

            opResult = c_uint8(0)
            scanResult = c_uint8(0)
            tmpROM = c_uint8 * 8
            ptrROM = tmpROM()

            func = self.libObj.PK_1WireBusScanGetResults
            func.argtypes = [sPoKeysDevicePtr, POINTER(c_uint8), POINTER(c_uint8), POINTER(tmpROM)]

            func(self.device, opResult, scanResult, ptrROM)

            if scanResult.value == 0:
                # Scan still pending
                continue
            elif scanResult.value == 2:
                # Scan was complete, no more sensors
                break

            elif (scanResult.value & 1) == 1:
                # Sensor ROM address received
                ROM = [ptrROM[i] for i in range(0, 8)]
                sensors.append(ROM)

                if self.libObj.PK_1WireBusScanContinue(self.device) != ePK_RETURN_CODES.PK_OK:
                    return []

                if scanResult.value == 3:
                    # This was the last sensor
                    break

            else:
                # Unknown operation result
                break

        # Stop the scan
        self.libObj.PK_1WireBusScanStop(self.device)

        return sensors


    def GetDallasCRC(self, data, length):
        shift_reg = 0
        data_bit = 0
        sr_lsb = 0
        fb_bit = 0

        for i in range(length):
            for j in range(8):
                data_bit = (data[i] >> j) & 1
                sr_lsb = shift_reg & 1
                fb_bit = (data_bit ^ sr_lsb) & 1
                shift_reg >>= 1

                if fb_bit:
                    shift_reg ^= 0x8c

        return shift_reg & 0xff

    # SPI operations
    #
    # SPI pin	PoKeys56U pin ID	PoKeys56E/PoKeys57E pin ID
    # MOSI	9	                23
    # MISO	10	                28
    # SCK	11	                25


    def PK_SPIConfigure(self, prescaler, frameFormat):
        return self.libObj.PK_SPIConfigure(self.device, prescaler, frameFormat)
    def PK_SPIWrite(self, data, pinCS):
        # If user provided only single integer, transfer it...
        if isinstance(data, int):
            numBytes = 1
            SPIdata = c_ubyte * 1
            buf = SPIdata(data)
        else:
            # Construct a c_byte array of numBytes bytes
            numBytes = len(data)
            SPIdata = c_ubyte * numBytes

            # Copy the Python array to c_byte array
            buf = SPIdata()
            for i in range(0, numBytes):
                buf[i] = data[i]

        return self.libObj.PK_SPIWrite(self.device, buf, numBytes, pinCS)

    def PK_SPIRead(self, ReadLen):
        bufferLen = 32
        bufferType = c_ubyte * bufferLen
        buffer = bufferType()

        func = self.libObj.PK_SPIRead
        func.argtypes = [sPoKeysDevicePtr, POINTER(c_uint8), c_uint8]

        func(self.device, buffer, ReadLen)

        return [buffer[i] for i in range(0, ReadLen)]

    def PK_SPI(self, data, pinCS):
        self.PK_SPIWrite(data, pinCS)

        if isinstance(data, int):
            return self.PK_SPIRead(1)
        else:
            # Construct a c_byte array of numBytes bytes
            return self.PK_SPIRead(len(data))
        


    # PoIL commands
    def PK_PoILGetState(self):
        return self.libObj.PK_PoILGetState(self.device)
    def PK_PoILSetCoreState(self, state):
        return self.libObj.PK_PoILSetCoreState(self.device, state)
    def PK_PoILSetMasterEnable(self, masterEnable):
        return self.libObj.PK_PoILSetMasterEnable(self.device, masterEnable)
    def PK_PoILResetCore(self):
        return self.libObj.PK_PoILResetCore(self.device)
    def PK_PoILSetDebugMode(self, debugMode, breakPointAddress):
        return self.libObj.PK_PoILSetDebugMode(self.device, debugMode, breakPointAddress)
    def PK_PoILReadMemory(self, memoryType, address, size):
        #self.libObj.PK_PoILReadMemory(self.device, uint8_t memoryType, uint16_t address, uint16_t size, uint8_t * dest)
        return []
    def PK_PoILWriteMemory(self, memoryType, address, data):
        #return self.libObj.PK_PoILWriteMemory(self.device, uint8_t memoryType, uint16_t address, uint16_t size, uint8_t * src)
        return 0;
    def PK_PoILEraseMemory(self, memoryType):
        return self.libObj.PK_PoILEraseMemory(self.device, memoryType)
    def PK_PoILChunkReadMemory(self):
        #return self.libObj.PK_PoILChunkReadMemory(sPoKeysDevice * device, uint8_t * dest)
        return []
    def PK_PoILChunkReadMemoryInternalAddress(self):
        #return self.libObj.PK_PoILChunkReadMemoryInternalAddress(sPoKeysDevice * device, uint8_t * dest)
        return []
    def PK_PoILReadSharedSlot(self, firstSlot, slotsNum):
        #return self.libObj.PK_PoILReadSharedSlot(self.device, uint16_t firstSlotID, uint16_t slotsNum, int32_t * dest)
        return []
    def PK_PoILWriteSharedSlot(self, firstSlot, slotsData):
        #return self.libObj.PK_PoILWriteSharedSlot(self.device, uint16_t firstSlotID, uint16_t slotsNum, int32_t * src)
        return 0;
    def PK_PoILTaskStatus(self):
        return self.libObj.PK_PoILTaskStatus(sPoKeysDevice * device)

    # RTC commands (real-time clock)
    def PK_RTCGet(self):
        return self.libObj.PK_RTCGet(self.device)
    def PK_RTCSet(self):
        return self.libObj.PK_RTCSet(self.device)


def convertToPythonArray(c_array, size):
    return [c_array[i] for i in range(0, size)]



# Author of the following function: BreizhGatch
# (http://stackoverflow.com/questions/1375897/how-to-get-the-signed-integer-value-of-a-long-in-python)
def getSignedNumber(number, bitLength):
    mask = (2 ** bitLength) - 1
    if number & (1 << (bitLength - 1)):
        return number | ~mask
    else:
        return number & mask


if __name__ == "__main__":
    import sys


