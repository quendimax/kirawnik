#
# Created: 2011-10-27
#      by: Maksim Tamkovich
#

include(../../../kirawnik.pri)

TEMPLATE = lib
CONFIG += shared
TARGET = optionsystem
VERSION = KIRAWNIK_VERSION
DESTDIR = ../../../lib/kirawnik/

INCLUDEPATH = . ../../ ../

HEADERS = \
		ColorOptionWidget.h \
		ColorButton.h \
		FontOptionWidget.h \
		OptionDialog.h \
		OptionWidget.h \
		PluginOptionWidget.h

SOURCES = \
		ColorOptionWidget.cpp \
		ColorButton.cpp \
		FontOptionWidget.cpp \
		OptionDialog.cpp \
		PluginOptionWidget.cpp
