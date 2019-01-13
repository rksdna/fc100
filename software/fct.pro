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
    DeviceChannel.cpp \
    DeviceModeWidget.cpp \
    DeviceMode.cpp \
    PopupButton.cpp \
    DeviceDisplayWidget.cpp \
    DeviceDisplay.cpp \
    DeviceSample.cpp \
    ButtonGroup.cpp

HEADERS += \
    MainWindow.h \
    PortDialog.h \
    Device.h \
    DeviceWidget.h \
    DeviceChannelWidget.h \
    DeviceChannel.h \
    DeviceModeWidget.h \
    DeviceMode.h \
    PopupButton.h \
    DeviceDisplayWidget.h \
    DeviceDisplay.h \
    DeviceSample.h \
    ButtonGroup.h

RESOURCES += \
    fct.qrc
