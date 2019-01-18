QT += core gui widgets serialport

TARGET = fc100
TEMPLATE = app

include(deploy/deploy.pri)
include(translations/translations.pri)

SOURCES += main.cpp \
    MainWindow.cpp \
    PortDialog.cpp \
    Device.cpp \
    DeviceWidget.cpp \
    PopupButton.cpp \
    DisplayWidget.cpp \
    Sample.cpp \
    Computer.cpp \
    ChannelOptions.cpp \
    ChannelWidget.cpp \
    ControlOptions.cpp \
    ControlWidget.cpp \
    ToolButton.cpp

HEADERS += \
    MainWindow.h \
    PortDialog.h \
    Device.h \
    DeviceWidget.h \
    PopupButton.h \
    DisplayWidget.h \
    Sample.h \
    Computer.h \
    ChannelOptions.h \
    ChannelWidget.h \
    ControlOptions.h \
    ControlWidget.h \
    ToolButton.h

RESOURCES += \
    fc100.qrc
