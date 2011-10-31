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
CONFIG(debug, debug|release): LIBS += -lcored -loptionsystemd -lpluginsystemd
						else: LIBS += -lcore -loptionsystem -lpluginsystem

SOURCES = main.cpp
