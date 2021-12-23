#-------------------------------------------------
#
# Project created by QtCreator 2014-02-17T16:17:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MarryMe
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp \
    driver/uart.cpp \
    mainWindows.cpp

HEADERS  += \
    driver/uart.h \
    driver/absInterface.h \
    mainWindows.h

FORMS    += \
    mainWindows.ui

include(qextserialport/src/qextserialport.pri)
