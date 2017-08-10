QT += core serialport
QT -= gui

CONFIG += c++11

TARGET = txbmp
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    hdlc_sender.cpp \
    hdlc.cpp \
    bmp_reader.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    hdlc_sender.h \
    hdlc.h \
    bmp_reader.h

win32 {
INCLUDEPATH += "C:\Program Files (x86)\Windows Kits\10\Include\10.0.10240.0\ucrt"
LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10240.0/ucrt/x64"


win32:contains(QMAKE_HOST.arch, x86_64) {
    LIBS += -L$$PWD/../libusb/lib_x64
} else {
    LIBS += -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.10240.0\um\x86"
    LIBS += -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.10240.0\ucrt\x86"
    #LIBS += -llegacy_stdio_definitions
}
}
