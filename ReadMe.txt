PoKeysLib

---- About ----
PoKeysLib is a multi-platform library which allows an application to interface 
PoKeys55, PoKeys56U and PoKeys56E devices.
It is meant as a replacement and enhancement of the original .NET-based commun-
ication library for Windows.


---- License ----
This library is released under GNU Lesser General Public License (LGPL). By us-
ing this library in any way, you agree with the terms described in License.txt

This library uses sources from HIDAPI library.

---- Installation ----
Preferred:
GNU GCC compiler is used on Linux and OS X to compile and install the library. 
Use the following command
  make -f Makefile.noqmake install
on Linux and
  make -f Makefile.noqmake.osx install
on OS X. These will also install the library in /usr/lib folder and copy the 
header file to /usr/include. Sudo may be required to gain write access to these
two folders

Also:
qmake can be used to build the librray using the attached project file 
PoKeysLib.pro.


---- Usage ----
PoKeysLib library has to be linked or included in the client application. 
On Linux and OS X, it depends on libusb-1.0 library that must be installed 
before compiling PoKeysLib.

Example code: 
http://www.mypokeys.com/new-cross-platform-library-for-all-pokeys-devices

---- PoKeys56U device rules setup (Linux) ----
(coming soon)

---- Library status - verified commands ----
[Windows Linux OSX]
[xxx] Enumerating USB devices
[xx ] Enumerating network devices
[xxx] Connecting to USB devices
[xx ] Connecting to network devices
[xxx] Reading device data
[xxx] Reading digital inputs
[xxx] Writing digital outputs
[xxx] PoExtBus writing
[xxx] PoExtBus reading
[xxx] LCD operations
[xxx] Matrix LED
[xxx] Setting pin functions
[xxx] Reading pin functions
[   ] Setting pin key codes
[   ] Reading pin key codes
[x  ] PWM operations
[xxx] Pulse engine operations
[   ] Matrix keyboard setup
[   ] Matrix keyboard reading
[xxx] Using encoders
[x  ] I2C operations
[x  ] SPI operation
[x  ] PoIL operations
[   ]
[   ]
[   ]
[   ]
[   ]
[   ]
[   ]
[   ]

---- Credits ----
Author: Matevž Bošnak (matevz@poscope.com)


---- Change log ----
03.10.2013: Support for network device data, pin capability function, new devices, bug fixes
13.04.2013: Mulitple updates: PWM, PoIL, SPI, RTC
15.12.2012: Support for I2C added
24.10.2012: Public release
16.10.2012: OS X version of the library tested
08.10.2012: Updated library - pure C, tested on Raspberry Pi
23.09.2012: Initial Windows+Linux library

