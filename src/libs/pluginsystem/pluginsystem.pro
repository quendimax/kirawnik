#
# Created:2011-10-27
#      by: Maksim Tamkovich
#

TEMPLATE = lib
TARGET = pluginsystem
DESTDIR = ../../../lib/kirawnik/
CONFIG += warn_on
CONFIG += debug_and_release
QMAKE_CXXFLAGS += -std=c++0x
#QMAKE_CXXFLAGS_DEBUG += -O1

INCLUDEPATH = . ../../ ../
LIBS += -L"../../../lib/kirawnik/" -lcore

unix {
	TMP_BUILD_PATH = /tmp/$$TARGET
	MOC_DIR = $$TMP_BUILD_PATH
	RCC_DIR = $$TMP_BUILD_PATH
	OBJECTS_DIR = $$TMP_BUILD_PATH
}

HEADERS = \
          PluginManager.h

SOURCES = \
          PluginManager.cpp
