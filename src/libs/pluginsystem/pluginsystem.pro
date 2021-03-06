#
# Created:2011-10-27
#      by: Maksim Tamkovich
#

include(../../../kirawnik.pri)

TEMPLATE = lib
TARGET = $$getTarget(pluginsystem)
VERSION = $$KIRAWNIK_VERSION
CONFIG += shared
QT += xml
DESTDIR = ../../../lib/kirawnik/

INCLUDEPATH = . ../../ ../

HEADERS = \
		PluginManager.h \
		PluginObject.h \
		PluginSpec.h \
		PluginSpecHandler.h

SOURCES = \
		PluginManager.cpp \
		PluginObject.cpp \
		PluginSpec.cpp \
		PluginSpecHandler.cpp
