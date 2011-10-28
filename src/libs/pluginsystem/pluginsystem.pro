#
# Created:2011-10-27
#      by: Maksim Tamkovich
#

include(../../../kirawnik.pri)

TEMPLATE = lib
CONFIG += shared
TARGET = pluginsystem
VERSION = KIRAWNIK_VERSION
DESTDIR = ../../../lib/kirawnik/

INCLUDEPATH = . ../../ ../

HEADERS = \
          PluginManager.h

SOURCES = \
          PluginManager.cpp
