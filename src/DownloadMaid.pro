# -----------------------------------------------------------
# This file is generated by the Qt Visual Studio Integration.
# -----------------------------------------------------------

# This is a reminder that you are using a generated .pro file.
# Remove it when you are finished editing this file.
message("You are running qmake on a generated .pro file. This may not work!")


TEMPLATE = app
TARGET = DownloadMaid
DESTDIR = ../bin/Win32Debug
QT += network xml qtestlib
CONFIG += debug help
DEFINES += QT_XML_LIB QT_NETWORK_LIB
INCLUDEPATH += ./GeneratedFiles \
    ./GeneratedFiles/Debug \
    .
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

#Include file(s)
include(DownloadMaid.pri)

#Windows resource file
win32:RC_FILE = DownloadMaid.rc
