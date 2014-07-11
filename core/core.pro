#-------------------------------------------------
#
# Project created by QtCreator 2014-07-01T21:47:44
#
#-------------------------------------------------

QT       -= gui
CONFIG   += c++11
TARGET = core
TEMPLATE = lib

DEFINES += CORE_LIBRARY

CONFIG(release, debug|release) {
    OBJECTS_DIR = build/release
    DESTDIR = ../lib
}

CONFIG(debug, debug|release) {
    OBJECTS_DIR = build/debug
    DESTDIR = ../lib
}

SOURCES += \
    abstractboard.cpp \
    abstractengine.cpp \
    move.cpp \
    movelist.cpp \
    piecetype.cpp \
    standardboard.cpp \
    stopwatch.cpp \
    stopwatchholder.cpp \
    board15x12.cpp \
    standardengine.cpp \
    bitboard.cpp \
    board0x88.cpp \
    fenparser.cpp

HEADERS +=\
        core_global.h \
    abstractboard.h \
    abstractengine.h \
    move.h \
    movelist.h \
    piecetype.h \
    standardboard.h \
    types.h \
    stopwatch.h \
    stopwatchholder.h \
    board15x12.h \
    stalematecondition.h \
    standardengine.h \
    bitboard.h \
    board0x88.h \
    fenparser.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
