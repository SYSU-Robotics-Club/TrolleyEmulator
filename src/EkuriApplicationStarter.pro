#-------------------------------------------------
#
# Project created by QtCreator 2015-09-12T23:15:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EkuriApplicationStarter
TEMPLATE = app


SOURCES += main.cpp\
        starterwindow.cpp \
    ekurisplashscreen.cpp \
    car.cpp \
    coreservice.cpp \
    sensor.cpp \
    buildcarwindow.cpp

HEADERS  += starterwindow.h \
    ekurisplashscreen.h \
    car.h \
    coreservice.h \
    sensor.h \
    buildcarwindow.h

FORMS    += starterwindow.ui \
    buildcarwindow.ui

RESOURCES += \
    image.qrc
