#-------------------------------------------------
#
# Project created by QtCreator 2015-02-28T20:04:38
#
#-------------------------------------------------

CONFIG   += c++11
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET    = DialogueNode
TEMPLATE  = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    nodes.cpp \
    commands.cpp

HEADERS += \
    mainwindow.hpp \
    nodes.hpp \
    commands.hpp

DISTFILES += \
    COPYING.md
