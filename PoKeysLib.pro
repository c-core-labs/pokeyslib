# -------------------------------------------------
# Project created by QtCreator 2012-09-17T20:22:45
# -------------------------------------------------
QT -= core gui

TEMPLATE = lib

CONFIG += staticlib warn_on debug

SOURCES += PoKeysLibCore.cpp \
    sPoKeysLibEncoders.cpp \
    PoKeysLibPulseEngine.cpp \
    PoKeysLibMatrixLED.cpp \
    PoKeysLibMatrixKB.cpp \
    PoKeysLibLCD.cpp \
    PoKeysLibIO.cpp \
    PoKeysLibDeviceData.cpp \
    PoKeysLibCoreSockets.cpp

win32: SOURCES += hid.c
unix: SOURCES += hid-libusb.c

HEADERS += PoKeysLibCoreSockets.h \
    PoKeysLibCore.h \
    hidapi.h \
    ../include/PoKeysLib.h \

win32 {
    LIBS += -lsetupapi -lWs2_32
}
unix {
    INCLUDEPATH += /usr/include/libusb-1.0
    LIBS += -L/usr/lib/ -lusb-1.0
    HEADERS += /usr/include/libusb-1.0/libusb.h
}

TARGET = ../lib/PoKeysLib

OTHER_FILES += \
    ReadMe.txt
