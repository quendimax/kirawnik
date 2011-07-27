#
# Created: 27.07.11
#      by: Maksim Tamkovich
#

TEMPLATE = app
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
	TARGET = kirawnikd
	CONFIG += console
}
else: TARGET = kirawnik

INCLUDEPATH = src

unix:TMP_BUILD_PATH = /tmp/$$TARGET
win32:TMP_BUILD_PATH = c:/Windows/Temp/$$TARGET

MOC_DIR = $$TMP_BUILD_PATH
RCC_DIR = $$TMP_BUILD_PATH
OBJECTS_DIR = $$TMP_BUILD_PATH

CONFIG += precompile_header
PRECOMPILED_HEADER = src/pch.h

SOURCES = \
	src/main.cpp
