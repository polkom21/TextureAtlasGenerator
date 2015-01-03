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
    pack.cpp

HEADERS  += mainwindow.h \
    createpack.h \
    pack.h

FORMS    += mainwindow.ui \
    createpack.ui

LIBS     += -lsfml-graphics
