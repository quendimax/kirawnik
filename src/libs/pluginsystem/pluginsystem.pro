#
# Created:2011-10-27
#      by: Maksim Tamkovich
#

include(../../../kirawnik.pri)

TEMPLATE = lib
TARGET = $$getTarget(pluginsystem)
VERSION = $$KIRAWNIK_VERSION
CONFIG += shared
DESTDIR = ../../../lib/kirawnik/

INCLUDEPATH = . ../../ ../

HEADERS = \
		PluginManager.h \
		PluginObject.h

SOURCES = \
		PluginManager.cpp \
		PluginObject.cpp
