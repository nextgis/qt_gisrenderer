#-------------------------------------------------
#
# Project created by QtCreator 2016-02-16T21:35:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_gisrenderer
TEMPLATE = app

#QMAKE_CXXFLAGS += $$(CXXFLAGS)
#QMAKE_CFLAGS += $$(CFLAGS)
#QMAKE_LFLAGS += $$(LDFLAGS)

QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/MainWindow.cpp \
    $$PWD/src/MapPaintSurface.cpp \
    $$PWD/src/MapWidget.cpp \
    $$PWD/src/OpenGlOffscreenSurface.cpp \

HEADERS += \
    $$PWD/src/MainWindow.h \
    $$PWD/src/MapPaintSurface.h \
    $$PWD/src/MapWidget.h \
    $$PWD/src/OpenGlOffscreenSurface.h \

OTHER_FILES += \
    $$PWD/uncrustify.cfg \
