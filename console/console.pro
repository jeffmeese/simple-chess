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
MOC_DIR   = moc

APPNAME = console

include ( ../common.pri)
include (../libs.pri)

SOURCES += main.cpp \
    application.cpp \
    perft.cpp \
    consolegame.cpp \
    ../common/game.cpp \
    ../common/engine.cpp

HEADERS += \
    application.h \
    perft.h \
    consolegame.h \
    ../common/board.h \
    ../common/engine.h \
    ../common/game.h

OTHER_FILES += \
    ../libs.pri
