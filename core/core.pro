#-------------------------------------------------
#
# Project created by QtCreator 2014-07-01T21:47:44
#
#-------------------------------------------------

QT       -= gui
CONFIG   += c++11
TARGET    = core
TEMPLATE  = lib
DEFINES  += CORE_LIBRARY
CONFIG += staticlib

APPNAME = core

include (../common.pri)

CONFIG(release, debug|release) {
  DESTDIR = $$BUILDROOT/lib
  TARGET=core
}

CONFIG(debug, debug|release) {
  DESTDIR = $$BUILDROOT/lib
  TARGET=cored
}

SOURCES += \
    move.cpp \
    piecetype.cpp \
    stopwatch.cpp \
    stopwatchholder.cpp \
    standardengine.cpp \
    board0x88.cpp \
    fen.cpp \
    util.cpp

HEADERS +=\
    core_global.h \
    move.h \
    piecetype.h \
    types.h \
    stopwatch.h \
    stopwatchholder.h \
    stalematecondition.h \
    standardengine.h \
    board0x88.h \
    fen.h \
    array2d.h \
    util.h
