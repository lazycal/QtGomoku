#-------------------------------------------------
#
# Project created by QtCreator 2016-08-30T20:32:35
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtGomoku
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    chboard.cpp \
    chess.cpp \
    ipdialog.cpp

HEADERS  += dialog.h \
    chboard.h \
    chess.h \
    ipdialog.h

FORMS    += dialog.ui
