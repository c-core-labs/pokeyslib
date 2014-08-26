# -------------------------------------------------
# Project created by QtCreator 2012-09-17T20:22:45
# -------------------------------------------------
QT -= core gui

makeDLL {
    DLL32 {
        message("Building 32-bit DLL...")
    } else {
        message("Building 64-bit DLL...")
    }
    CONFIG += dll warn_on
    DEFINES += POKEYSDLL POKEYSDLLEXPORT
} else {
    message("Building library")
    CONFIG += staticlib warn_on
}

TEMPLATE = lib


SOURCES += PoKeysLibCore.c \
    PoKeysLibFastUSB.c \
    PoKeysLibEncoders.c \
    PoKeysLibPulseEngine.c \
    PoKeysLibMatrixLED.c \
    PoKeysLibMatrixKB.c \
    PoKeysLibLCD.c \
    PoKeysLibIO.c \
    PoKeysLibDeviceData.c \
    PoKeysLibCoreSockets.c \
    PoKeysLibI2C.c \
    PoKeysLibPoNET.c \
    PoKeysLibPoIL.c \
    PoKeysLibRTC.c \
    PoKeysLibSPI.c \
    PoKeysLibPulseEngine_v2.c \
    PoKeysLib1Wire.c

win32: SOURCES += hid.c

HEADERS += PoKeysLibCoreSockets.h \
    PoKeysLibCore.h \
    hidapi.h \
    PoKeysLib.h \

win32 {
    # x86
    LIBS += -lsetupapi -lWs2_32 -liphlpapi
    makeDLL {
        DLL32 {
            TARGET = ../../dll/32/PoKeysLib
        } else {
            TARGET = ../../dll/64/PoKeysLib
        }
    } else {
        TARGET = ../../lib/PoKeysLib
    }
}
unix:!macx {
    SOURCES += hid-libusb.c
    INCLUDEPATH += /usr/include/libusb-1.0
    LIBS += -L/usr/lib/ -lusb-1.0
    HEADERS += /usr/include/libusb-1.0/libusb.h
    TARGET = PoKeys
}

macx {
    SOURCES += hid-mac.c
    DEFINES += APL=1 IBM=0 LIN=0
    QMAKE_LFLAGS += -flat_namespace -undefined suppress

    # On OS X, install MacPorts and type "sudo ports install libusb"
    INCLUDEPATH += /opt/local/include/libusb-1.0
    LIBS += -L/opt/local/lib/ -lusb-1.0
    HEADERS += /opt/local/include/libusb-1.0/libusb.h

    # The following line defines for which architectures we build.
    CONFIG += x86
    TARGET = PoKeys
}

OTHER_FILES += \
    ReadMe.txt
