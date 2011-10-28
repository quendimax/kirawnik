#
# Created: 2011-07-27
#      by: Maksim Tamkovich
#

include(../../kirawnik.pri)

TEMPLATE = app
TARGET = kirawnik
VERSION = KIRAWNIK_VERSION
DESTDIR = ../../bin
CONFIG(debug, debug|release) {
	TARGET = $${TARGET}d
	win32:CONFIG += console
}

*g++* : QMAKE_LFLAGS += -Wl,--rpath="../lib/kirawnik/"

INCLUDEPATH = . .. ../libs/
LIBS += -L"../../lib/kirawnik/" -lcore -loptionsystem -lpluginsystem

SOURCES = main.cpp
