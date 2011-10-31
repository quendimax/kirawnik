#
# Created: 2011-07-27
#      by: Maksim Tamkovich
#

include(../../kirawnik.pri)

TEMPLATE = app
TARGET = $$getTarget(kirawnik)
VERSION = $$KIRAWNIK_VERSION
DESTDIR = ../../bin

*g++* : QMAKE_LFLAGS += -Wl,--rpath="../lib/kirawnik/"

INCLUDEPATH = . .. ../libs/
LIBS += -L"../../lib/kirawnik/"
LIBS += -l$$libraryName(core) -l$$libraryName(optionsystem) -l$$libraryName(pluginsystem)

SOURCES = main.cpp
