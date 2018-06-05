QT += core gui widgets

TARGET = fc
TEMPLATE = app

include(../deploy/deploy.pri)

SOURCES += main.cpp \
    MainWidget.cpp

HEADERS += MainWidget.h
