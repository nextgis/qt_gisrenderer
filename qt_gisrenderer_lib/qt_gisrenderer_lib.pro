TARGET = qt_gisrenderer_lib
TEMPLATE = lib

INSTALLS += target
#target.path=$$[ENV_VAR]/path/to/install

QT = core gui

#QMAKE_CXXFLAGS += $$(CXXFLAGS)
#QMAKE_CFLAGS += $$(CFLAGS)
#QMAKE_LFLAGS += $$(LDFLAGS)

QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11

SOURCES += \
    $$PWD/src/MapPaintSurface.cpp \
    $$PWD/src/OpenGlOffscreenSurface.cpp \
    $$PWD/src/RenderThread.cpp \

HEADERS += \
    $$PWD/src/MapPaintSurface.h \
    $$PWD/src/OpenGlOffscreenSurface.h \
    $$PWD/src/RenderThread.h \

OTHER_FILES += \
    $$PWD/uncrustify.cfg \

DEFINES *= QT_BUILD_GISRENDERER_LIB

DESTDIR = $$OUT_PWD/..
