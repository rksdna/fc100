QT += core gui widgets serialport qml quick

TARGET = fc100
TEMPLATE = app

include(deploy/deploy.pri)
include(device/device.pri)
include(widgets/widgets.pri)
include(translations/translations.pri)

SOURCES += main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h

RESOURCES += \
    fc100.qrc
