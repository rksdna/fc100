QT += core gui widgets serialport

TARGET = fct
TEMPLATE = app

include(deploy/deploy.pri)
include(translations/translations.pri)

SOURCES += main.cpp \
    MainWindow.cpp \
    PortDialog.cpp \
    Device.cpp \
    DeviceWidget.cpp

HEADERS += \
    MainWindow.h \
    PortDialog.h \
    Device.h \
    DeviceWidget.h

RESOURCES += \
    fct.qrc
