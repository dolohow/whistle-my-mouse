#-------------------------------------------------
#
# Project created by QtCreator 2015-01-07T21:24:48
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = whistle-my-mouse
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -llibqrealfourier

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
