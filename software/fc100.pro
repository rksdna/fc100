QT += core gui widgets serialport quick

TARGET = fc100
TEMPLATE = app

include(translations/translations.pri)
include(deploy/deploy.pri)
include(common/common.pri)
include(device/device.pri)
include(widgets/widgets.pri)

SOURCES += main.cpp \
    MainWindow.cpp \
    PreferencesDialog.cpp

HEADERS += \
    MainWindow.h \
    PreferencesDialog.h

RESOURCES += \
    fc100.qrc
