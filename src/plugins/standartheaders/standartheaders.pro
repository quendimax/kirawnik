#
# Created: 2011-09-29
#      by: Maksim Tamkovich
#

include(../../../kirawnik.pri)

TEMPLATE = lib
TARGET = $$getTarget(kplugin_standartheaders)
VERSION = $$KIRAWNIK_VERSION
CONFIG += plugin
DESTDIR = ../../../lib/kirawnik/

INCLUDEPATH = . ../../ ../../libs/

HEADERS = StandartHeadersPlugin.h \
          StandartHeaders.h

SOURCES = StandartHeadersPlugin.cpp \
          StandartHeaders.cpp
