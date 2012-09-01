#-------------------------------------------------
#
# Project created by QtCreator 2012-08-27T13:13:06
#
#-------------------------------------------------

QT       += core gui

TARGET = TrajSys
TEMPLATE = app

# MAC opencv dir
#INCLUDEPATH += /usr/local/include
#LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video

# Windows opencv dir
INCLUDEPATH += F:/Software/opencv/mingw/install/include
LIBS += -LF:/Software/opencv/mingw/install/lib
LIBS += -lopencv_core242 -lopencv_highgui242 -lopencv_imgproc242 -lopencv_video242

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
    kernel/taskmanager.cpp \
    gui/sourcespage.cpp \
    gui/trackerspage.cpp \
    gui/appspage.cpp \
    kernel/trackermanager.cpp \
    kernel/appmanager.cpp \
    kernel/klttracker.cpp \
    kernel/coherentfilter.cpp \
    utils/trackio.cpp \
    utils/clusterio.cpp \
    utils/frameio.cpp \
    kernel/sourcemanager.cpp

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
    utils/const.h \
    kernel/taskmanager.h \
    gui/sourcespage.h \
    gui/trackerspage.h \
    gui/appspage.h \
    kernel/trackermanager.h \
    kernel/appmanager.h \
    kernel/klttracker.h \
    kernel/coherentfilter.h \
    utils/trackio.h \
    utils/clusterio.h \
    utils/frameio.h \
    kernel/sourcemanager.h \
    utils/utils.h
