QT += core
QT -= gui

QMAKE_CXXFLAGS = -I/usr/src/linux-headers-4.4.0-21/include
QMAKE_CFLAGS = -I/usr/src/linux-headers-4.4.0-21/include
TARGET = Zedboard-kernel
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    kmodule_encoder.c

