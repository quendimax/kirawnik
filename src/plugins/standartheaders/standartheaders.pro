#
# Created: 29.09.11
#      by: Maksim Tamkovich
#

TEMPLATE = lib
CONFIG += plugin
TARGET = kplugin_standartheaders
DESTDIR = ../../../lib/kirawnik/
CONFIG += warn_on
CONFIG += debug_and_release
QMAKE_CXXFLAGS += -std=c++0x
#QMAKE_CXXFLAGS_DEBUG += -O1

INCLUDEPATH = . ../../ ../../libs/

unix {
	TMP_BUILD_PATH = /tmp/$$TARGET
	MOC_DIR = $$TMP_BUILD_PATH
	RCC_DIR = $$TMP_BUILD_PATH
	OBJECTS_DIR = $$TMP_BUILD_PATH
}

HEADERS = StandartHeadersPlugin.h \
          StandartHeaders.h

SOURCES = StandartHeadersPlugin.cpp \
          StandartHeaders.cpp
