QT += core gui widgets serialport

TARGET = fc100
TEMPLATE = app

include(deploy/deploy.pri)
include(widgets/widgets.pri)
include(controls/controls.pri)
include(translations/translations.pri)

SOURCES += main.cpp \
    Device.cpp \
    HardwareDevice.cpp

HEADERS += \
    Device.h \
    HardwareDevice.h

RESOURCES += \
    fc100.qrc
