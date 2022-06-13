# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = QtGuiApplication1
DESTDIR = ../x64/Debug
QT += core sql network gui svg multimedia widgets printsupport websockets charts datavisualization
CONFIG += debug
DEFINES += _UNICODE WIN64 QT_SQL_LIB QT_WIDGETS_LIB QT_NETWORK_LIB QT_WEBSOCKETS_LIB QT_DATAVISUALIZATION_LIB QT_MULTIMEDIA_LIB QT_PRINTSUPPORT_LIB QT_CHARTS_LIB QT_SVG_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/$(ConfigurationName)
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/$(ConfigurationName)
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
HEADERS += ./common.h \
    ./GeneratedFiles/ui_multiwindow.h \
    ./multiwindow.h \
    ./lookahead_alert.h \
    ./qcustomplot.h \
    ./communication.h \
    ./dem_update.h \
    ./locdatabase.h
SOURCES += ./communication.cpp \
    ./locdatabase.cpp \
    ./dem_update.cpp \
    ./lookahead_alert.cpp \
    ./main.cpp \
    ./multiwindow.cpp \
    ./qcustomplot.cpp
FORMS += ./multiwindow.ui
RESOURCES += multiwindow.qrc
