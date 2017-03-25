#-------------------------------------------------
#
# Project created by QtCreator 2017-03-21T13:18:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tableSim
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pixel.cpp \
    clock.cpp \
    led_table.cpp \
    tetris.cpp \
    rgb.cpp

HEADERS  += mainwindow.h \
    pixel.h \
    clock.h \
    led_table.h \
    tetris.h \
    rgb.h

FORMS    += mainwindow.ui

INCLUDEPATH += "C:\Program Files (x86)\Windows Kits\10\Include\10.0.10240.0\ucrt"
LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10240.0/ucrt/x64"
