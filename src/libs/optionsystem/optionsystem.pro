#
# Created: 2011-10-27
#      by: Maksim Tamkovich
#

include(../../../kirawnik.pri)

TEMPLATE = lib
TARGET = $$getTarget(optionsystem)
VERSION = $$KIRAWNIK_VERSION
CONFIG += shared
DESTDIR = ../../../lib/kirawnik/

INCLUDEPATH = . ../../ ../

HEADERS = \
		ColorOptionWidget.h \
		ColorButton.h \
		FontOptionWidget.h \
		OptionDialog.h \
		OptionWidget.h \
		PluginDetailsView.h \
		PluginOptionWidget.h

SOURCES = \
		ColorOptionWidget.cpp \
		ColorButton.cpp \
		FontOptionWidget.cpp \
		OptionDialog.cpp \
		PluginDetailsView.cpp \
		PluginOptionWidget.cpp

FORMS = \
		PluginDetailsView.ui \
		PluginOptionWidget.ui
