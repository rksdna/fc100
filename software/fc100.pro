QT += core gui widgets serialport

TARGET = fc100
TEMPLATE = app

include(deploy/deploy.pri)
include(controls/controls.pri)
include(translations/translations.pri)

SOURCES += main.cpp \
    Device.cpp

HEADERS += \
    Device.h

RESOURCES += \
    fc100.qrc
