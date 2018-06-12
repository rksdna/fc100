QT += core gui widgets serialport

TARGET = fc
TEMPLATE = app

include(../deploy/deploy.pri)
include(../device/device.pri)

SOURCES += main.cpp \
    MainWidget.cpp

HEADERS += MainWidget.h
