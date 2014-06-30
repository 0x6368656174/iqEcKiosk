#-------------------------------------------------
#
# Project created by QtCreator 2013-11-07T14:06:52
#
#-------------------------------------------------

QT       += core sql

QT       -= gui

TARGET = iqorm
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    iqormmodel.cpp \
    iqormpropertydescription.cpp \
    iqormobject.cpp \
    iqormcore.cpp \
    iqormabstractsynchronizer.cpp \
    iqormlocalsynchronizer.cpp \
    iqormnullsynchronizer.cpp \
    iqormabstractdatasource.cpp \
    iqormsqldatasource.cpp \
    iqormnulldatasource.cpp

HEADERS += \
    iqormpropertydescription.h \
    iqormobject.h \
    iqormmodel.h \
    iqormcore.h \
    iqormabstractsynchronizer.h \
    iqormlocalsynchronizer.h \
    iqormnullsynchronizer.h \
    iqormabstractdatasource.h \
    iqormsqldatasource.h \
    iqormnulldatasource.h
