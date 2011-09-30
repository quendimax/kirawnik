#
# Created: 29.09.11
#      by: Maksim Tamkovich
#

TEMPLATE = lib
CONFIG += plugin
TARGET = kplugin_standartheaders
DESTDIR = ../../../share/kirawnik/plugins/
CONFIG += warn_on
CONFIG += debug_and_release
#QMAKE_CXXFLAGS_DEBUG += -O1

INCLUDEPATH = . ../../

unix:TMP_BUILD_PATH = /tmp/$$TARGET
win32:TMP_BUILD_PATH = c:/Windows/Temp/$$TARGET

MOC_DIR = $$TMP_BUILD_PATH
RCC_DIR = $$TMP_BUILD_PATH
OBJECTS_DIR = $$TMP_BUILD_PATH


HEADERS = StandartHeadersPlugin.h \
          StandartHeaders.h

SOURCES = StandartHeadersPlugin.cpp \
          StandartHeaders.cpp
