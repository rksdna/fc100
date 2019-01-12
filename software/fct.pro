QT += core gui widgets serialport

TARGET = fct
TEMPLATE = app

include(deploy/deploy.pri)
include(translations/translations.pri)

SOURCES += main.cpp \
    MainWindow.cpp \
    PortDialog.cpp \
    Device.cpp \
    DeviceWidget.cpp \
    DeviceChannelWidget.cpp \
    DeviceChannel.cpp

HEADERS += \
    MainWindow.h \
    PortDialog.h \
    Device.h \
    DeviceWidget.h \
    DeviceChannelWidget.h \
    DeviceChannel.h

RESOURCES += \
    fct.qrc
