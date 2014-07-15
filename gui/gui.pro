#-------------------------------------------------
#
# Project created by QtCreator 2014-06-25T23:22:36
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
TARGET = scgui
TEMPLATE = app
MOC_DIR = moc
APPNAME = gui

include ( ../common.pri)
include (../libs.pri)

SOURCES += \
  main.cpp\
  mainwindow.cpp \
  application.cpp \
  boardview.cpp \
  imagemap.cpp \
  guigame.cpp \
  ../common/game.cpp \
  ../common/engine.cpp

HEADERS  += \
  mainwindow.h \
  application.h \
  boardview.h \
  imagemap.h \
  guigame.h \
  ../common/board.h \
  ../common/engine.h \
  ../common/game.h

FORMS    += mainwindow.ui

RESOURCES += \
    gui.qrc
