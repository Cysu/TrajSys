#-------------------------------------------------
#
# Project created by QtCreator 2012-08-27T13:13:06
#
#-------------------------------------------------

QT       += core gui

TARGET = TrajSys
TEMPLATE = app


SOURCES += gui/main.cpp\
    gui/mainwindow.cpp \
    utils/klt/writeFeatures.c \
    utils/klt/trackFeatures.c \
    utils/klt/storeFeatures.c \
    utils/klt/selectGoodFeatures.c \
    utils/klt/pyramid.c \
    utils/klt/pnmio.c \
    utils/klt/klt_util.c \
    utils/klt/klt.c \
    utils/klt/error.c \
    utils/klt/convolve.c \
    gui/mainwizard.cpp \
    gui/modepage.cpp \
    gui/offlinefilelistpage.cpp \
    gui/offlinetrackerpage.cpp

HEADERS  += gui/mainwindow.h \
    utils/klt/pyramid.h \
    utils/klt/pnmio.h \
    utils/klt/klt_util.h \
    utils/klt/klt.h \
    utils/klt/error.h \
    utils/klt/convolve.h \
    utils/klt/base.h \
    gui/guiutils.h \
    gui/mainwizard.h \
    gui/modepage.h \
    gui/offlinefilelistpage.h \
    gui/offlinetrackerpage.h \
    utils/const.h

OTHER_FILES +=

RESOURCES +=
