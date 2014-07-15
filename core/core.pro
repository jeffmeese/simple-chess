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
    movelist.cpp \
    piecetype.cpp \
    stopwatch.cpp \
    stopwatchholder.cpp \
    board15x12.cpp \
    standardengine.cpp \
    bitboard.cpp \
    board0x88.cpp \
    fenparser.cpp \
    board8x8.cpp

HEADERS +=\
    core_global.h \
    move.h \
    movelist.h \
    piecetype.h \
    types.h \
    stopwatch.h \
    stopwatchholder.h \
    board15x12.h \
    stalematecondition.h \
    standardengine.h \
    bitboard.h \
    board0x88.h \
    fenparser.h \
    board8x8.h
