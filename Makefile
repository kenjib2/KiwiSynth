# Misc
#LDFLAGS = -u _printf_float
#OPT = -Os # Optimize for smaller size
# Need to run make program-boot to load the Daisy bootloader before flashing. Then press and release reset and then boot (not both together). Lights should pulsate if successful.
# To set it back to flash loading run make program-dfu
APP_TYPE = BOOT_SRAM
OPT = -O2

# Project Name
TARGET = HardwareTest

# Sources
CPP_SOURCES = $(wildcard *.cpp) $(wildcard KiwiSynth/*.cpp) $(wildcard KiwiSynth/Util/*.cpp) $(wildcard KiwiSynth/Modules/*.cpp) $(wildcard KiwiSynth/Controls/*.cpp) $(wildcard KiwiSynth/Patch/*.cpp) $(wildcard KiwiSynth/GUI/*.cpp) $(wildcard KiwiSynth/Effects/*.cpp) $(wildcard KiwiSynth/Effects/Zita/*.cpp)

# Library Locations
LIBDAISY_DIR = ../../libDaisy/
DAISYSP_DIR = ../../DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
