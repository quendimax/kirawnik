#
# Created: 2011-09-29
#      by: Maksim Tamkovich
#

include(../../../kirawnik.pri)

PLUGINNAME = StandartHeaders
TEMPLATE = lib
TARGET = $$getTarget($$PLUGINNAME)
VERSION = $$KIRAWNIK_VERSION
CONFIG += plugin
DESTDIR = ../../../lib/kirawnik/plugins/

QMAKE_PRE_LINK = "cp $${PLUGINNAME}.pluginspec $$DESTDIR"

INCLUDEPATH = . ../../ ../../libs/

HEADERS = StandartHeadersPlugin.h \
          StandartHeaders.h

SOURCES = StandartHeadersPlugin.cpp \
          StandartHeaders.cpp

OTHER_FILES = $${PLUGINNAME}.pluginspec
