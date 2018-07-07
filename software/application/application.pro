QT += core gui widgets serialport

TARGET = fc
TEMPLATE = app

include(../deploy/deploy.pri)
include(../device/device.pri)

SOURCES += main.cpp \
    MainWindow.cpp \
    PortDialog.cpp

HEADERS += \
    MainWindow.h \
    PortDialog.h
