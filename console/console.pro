#-------------------------------------------------
#
# Project created by QtCreator 2014-06-25T23:21:44
#
#-------------------------------------------------

QT       += core
QT       -= gui
CONFIG   += console
CONFIG   += c++11
CONFIG   -= app_bundle
TEMPLATE  = app
TARGET    = sc

include ( ../common.pri)

LIBS += ../lib/libcore.so
PRE_TARGETDEPS += ../lib/libcore.so

SOURCES += main.cpp \
    application.cpp \
    perft.cpp \
    consolegame.cpp \
    ../common/game.cpp \
    ../common/engine.cpp

HEADERS += \
    application.h \
    perft.h \
    ../common/board.h \
    ../common/engine.h \
    consolegame.h \
    ../common/game.h
