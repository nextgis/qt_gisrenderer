TARGET = qt_gisrenderer_app
TEMPLATE = app

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#QMAKE_CXXFLAGS += $$(CXXFLAGS)
#QMAKE_CFLAGS += $$(CFLAGS)
#QMAKE_LFLAGS += $$(LDFLAGS)

QMAKE_CXXFLAGS += -std=c++11


SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/MainWindow.cpp \
    $$PWD/src/MapWidget.cpp \

HEADERS += \
    $$PWD/src/MainWindow.h \
    $$PWD/src/MapWidget.h \

OTHER_FILES += \
    $$PWD/uncrustify.cfg \

DEPENDPATH += \
    . \
    $$PWD/../qt_gisrenderer_lib \

INCLUDEPATH += \
    $$PWD/../qt_gisrenderer_lib/src \

LIBS += \
    -L$$OUT_PWD/.. -lqt_gisrenderer_lib \
