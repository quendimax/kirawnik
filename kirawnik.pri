# It make name with 'd' in debug mode
defineReplace(makeName) {
	CONFIG(debug, debug|release) {
		name = $${1}d
	}
	else {
		name = $${1}
	}

	return($$name)
}


defineReplace(getTarget) {
	return($$makeName($$1))
}


defineReplace(libraryName) {
	libname = $$makeName($$1)
	win32 {
		libname = $${libname}.lib
	}
	else:unix {
		libname = -l$${libname}
	}
	return($$libname)
}


defineReplace(externLibraryName) {
	libname = $$1
	win32 {
		libname = $${libname}.lib
	}
	else:unix {
		libname = -l$$libname
	}
}


#
# HEADER
#
KIRAWNIK_VERSION = 0.0.0
CONFIG += warn_on
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
	DEFINES += KIRAWNIK_DEBUG
	win32:CONFIG += console
}
DEFINES += KIRAWNIK_VERSION=\\\"$$KIRAWNIK_VERSION\\\"

*g++* {
	#QMAKE_CXXFLAGS_DEBUG += -O1
	QMAKE_CXXFLAGS += -std=c++0x
	!CONFIG(debug, debug|release) {
		QMAKE_LFLAGS += -Wl,-s
	}
}

unix {
	TMP_BUILD_PATH = /tmp/$$makeName(kirawnik)/$$TARGET
	MOC_DIR = $$TMP_BUILD_PATH
	RCC_DIR = $$TMP_BUILD_PATH
	OBJECTS_DIR = $$TMP_BUILD_PATH
}
