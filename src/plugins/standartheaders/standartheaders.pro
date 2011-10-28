#
# Created: 29.09.11
#      by: Maksim Tamkovich
#

include(../../../kirawnik.pri)

TEMPLATE = lib
CONFIG += plugin
TARGET = kplugin_standartheaders
VERSION = 0.0.0
DESTDIR = ../../../lib/kirawnik/

INCLUDEPATH = . ../../ ../../libs/

HEADERS = StandartHeadersPlugin.h \
          StandartHeaders.h

SOURCES = StandartHeadersPlugin.cpp \
          StandartHeaders.cpp
