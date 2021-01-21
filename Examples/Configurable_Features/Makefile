XBE_TITLE = "ConfluxConfigExample"
GEN_XISO = $(XBE_TITLE).iso
NXDK_CXX = y
NXDK_SDL = y
DEBUG = y
CFLAGS="-D_XBOX"
CXXFLAGS="-D_XBOX"

#Store the path to the Conflux Source directory
#This var is also used by the Conflux Makefile, and must
#be declared.
CONFLUX_SOURCE = $(CURDIR)/../../Source

#Include the Conflux Makefile to build dependencies
include $(CONFLUX_SOURCE)/Makefile

SRCS += $(CURDIR)/main.cpp

include $(NXDK_DIR)/Makefile
