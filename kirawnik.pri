KIRAWNIK_VERSION = 0.0.0
CONFIG += warn_on
CONFIG += debug_and_release

*g++* {
	#QMAKE_CXXFLAGS_DEBUG += -O1
	QMAKE_CXXFLAGS += -std=c++0x
}

unix {
	TMP_BUILD_PATH = /tmp/$$TARGET
	MOC_DIR = $$TMP_BUILD_PATH
	RCC_DIR = $$TMP_BUILD_PATH
	OBJECTS_DIR = $$TMP_BUILD_PATH
}

