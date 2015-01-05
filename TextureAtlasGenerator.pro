#-------------------------------------------------
#
# Project created by QtCreator 2014-12-21T21:12:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TextureAtlasGenerator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    createpack.cpp \
    pack.cpp \
    settings.cpp

HEADERS  += mainwindow.h \
    createpack.h \
    pack.h \
    settings.h

FORMS    += mainwindow.ui \
    createpack.ui \
    settings.ui

LIBS     += -lsfml-graphics
