#-------------------------------------------------
#
# Project created by QtCreator 2017-12-05T23:05:40
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Agora_WaWa
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    readfile.cpp \
    widget.cpp

INCLUDEPATH += sdk/include/internal/

HEADERS  += mainwindow.h \
    readfile.h \
    widget.h

FORMS    += mainwindow.ui

#LIBS += -L$$PWD/sdk/libs -lRtcEngine



#unix:!macx:
#LIBS += -L$$PWD/sdk/libs -lRtcEngine

DISTFILES += \
    config.ini

