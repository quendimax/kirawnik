#
# Created: 2011-09-29
#      by: Maksim Tamkovich
#

include(../../../kirawnik.pri)

TEMPLATE = lib
TARGET = $$getTarget(StandartHeaders)
VERSION = $$KIRAWNIK_VERSION
CONFIG += plugin
DESTDIR = ../../../lib/kirawnik/plugins/

INCLUDEPATH = . ../../ ../../libs/

HEADERS = StandartHeadersPlugin.h \
          StandartHeaders.h

SOURCES = StandartHeadersPlugin.cpp \
          StandartHeaders.cpp
