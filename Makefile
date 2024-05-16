# Misc
LDFLAGS = -u _printf_float

# Project Name
TARGET = HardwareTest

# Sources
CPP_SOURCES = $(wildcard *.cpp) $(wildcard KiwiSynth/*.cpp) $(wildcard KiwiSynth/Controls/*.cpp) $(wildcard KiwiSynth/Patch/*.cpp)

# Library Locations
LIBDAISY_DIR = ../../libDaisy/
DAISYSP_DIR = ../../DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
