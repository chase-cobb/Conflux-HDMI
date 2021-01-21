#CONFLUX_SOURCE variable pointing to this directory
#must be declared in the root level project Makefile 
#and this file should be included.

CXXFLAGS  += -I$(CONFLUX_SOURCE)
CXXFLAGS  += -I$(CONFLUX_SOURCE)/Common
CXXFLAGS  += -I$(CONFLUX_SOURCE)/Common/Types
CXXFLAGS  += -I$(CONFLUX_SOURCE)/HDMI_Implementations
CXXFLAGS  += -I$(CONFLUX_SOURCE)/HDMI_Implementations/XboxHDMI
CXXFLAGS  += -I$(CONFLUX_SOURCE)/HDMI_Implementations/XboxHDMI/Config

SRCS += $(CONFLUX_SOURCE)/HdmiTools.cpp
SRCS += $(CONFLUX_SOURCE)/Common/Types/RangedIntValue.cpp
SRCS += $(CONFLUX_SOURCE)/Common/Types/VersionCode.cpp
SRCS += $(CONFLUX_SOURCE)/HDMI_Implementations/Helpers.cpp
SRCS += $(CONFLUX_SOURCE)/HDMI_Implementations/HdmiInterface.cpp
SRCS += $(CONFLUX_SOURCE)/HDMI_Implementations/XboxHDMI/XboxHdmi.cpp