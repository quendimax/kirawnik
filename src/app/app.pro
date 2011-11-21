#
# Created: 2011-07-27
#      by: Maksim Tamkovich
#

include(../../kirawnik.pri)

TEMPLATE = app
TARGET = $$getTarget(kirawnik)
VERSION = $$KIRAWNIK_VERSION
DESTDIR = ../../bin

*g++* : QMAKE_LFLAGS += -Wl,-z,origin \'-Wl,-rpath,\$\$ORIGIN/../lib/kirawnik/\'

INCLUDEPATH = . .. ../libs/
LIBS += -L"../../lib/kirawnik/"
LIBS += $$libraryName(core) $$libraryName(optionsystem) $$libraryName(pluginsystem)

SOURCES = main.cpp
