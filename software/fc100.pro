QT += core gui widgets serialport qml quick

TARGET = fc100
TEMPLATE = app

include(deploy/deploy.pri)
include(common/common.pri)
include(device/device.pri)
include(widgets/widgets.pri)
include(translations/translations.pri)

SOURCES += main.cpp \
    MainWindow.cpp \
    DeviceWidget.cpp \
    DeviceDialog.cpp

HEADERS += \
    MainWindow.h \
    DeviceWidget.h \
    DeviceDialog.h

RESOURCES += \
    fc100.qrc
