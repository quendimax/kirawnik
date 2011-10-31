KIRAWNIK_VERSION = 0.0.0
CONFIG += warn_on
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
	DEFINES += KIRAWNIK_DEBUG
	win32:CONFIG += console
}

*g++* {
	#QMAKE_CXXFLAGS_DEBUG += -O1
	QMAKE_CXXFLAGS += -std=c++0x
}

unix {
	TMP_BUILD_PATH = /tmp/kirawnik/$$TARGET
	MOC_DIR = $$TMP_BUILD_PATH
	RCC_DIR = $$TMP_BUILD_PATH
	OBJECTS_DIR = $$TMP_BUILD_PATH
}

defineReplace(getTarget) {
	CONFIG(debug, debug|release) {
		name = $${1}d
	}
	else {
		name = $${1}
	}

	return($$name)
}
