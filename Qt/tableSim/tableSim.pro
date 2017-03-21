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
    led_table.cpp

HEADERS  += mainwindow.h \
    pixel.h \
    clock.h \
    led_table.h

FORMS    += mainwindow.ui
