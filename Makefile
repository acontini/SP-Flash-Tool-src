# Usage 
# make HOST=(linux/windows) BUILD_TYPE=(debug/release)
# The objects and execution file will be move to ../_Output/
# Windows known issues:1.jom.exe could not be executed to make; 2. eval to assign variable does not work 
#

# general config
TARGET              := 
HOST	            := $(if $(HOST),$(HOST),linux)
BUILD_TYPE          := $(if $(BUILD_TYPE),$(BUILD_TYPE),debug)
#BUILDCHANGELIST     := $(if $(BUILDCHANGELIST),$(BUILDCHANGELIST),CHANGE_LIST_NOT_SPECIFIED)
#BUILDOWNER          := $(if $(BUILDOWNER),$(BUILDOWNER),OWNER_NOT_SPECIFIED)
#BUILD_CONFIG_H      := Build/build_cfg.h

# tools
ifeq ($(HOST),linux)
TOOLS_DIR           := ./Build/Tools/$(HOST)
OUTPUT_PATH         := ../_Output/$(HOST)/$(BUILD_TYPE)
TEE                 := tee
MAKE                := make
else
OUTPUT_PATH         := ..\_Output\$(HOST)
TEE                 := $(TOOLS_DIR)\tee.exe
MAKE                := make.exe
endif


ifeq (off,$(LOG))

all:
.PHONY: FORCE

include Build/build-$(HOST).mk

else
$(if $(filter debug release,$(BUILD_TYPE)),,$(error BUILD_TYPE "$(BUILD_TYPE)" invalid; should be debug or release))
$(if $(filter linux windows,$(HOST)),,$(error HOST "$(HOST)" invalid; should be linux or windows))
LOG := build_log.txt
$(MAKECMDGOALS): recursive-make
recursive-make:
	$(MAKE) BUILD_TYPE=$(BUILD_TYPE) HOST=$(HOST) LOG=off $(MAKECMDGOALS) 2>&1 | $(TEE) $(LOG)
endif

#.PHONY: builder-config
#builder-config:
#	$(if $(LINUX), @chmod a+w $(BUILD_CONFIG_H), @attrib -r $(BUILD_CONFIG_H))
#	@echo #ifndef _BUILD_CONFIG_ > $(BUILD_CONFIG_H) 
#	@echo #define _BUILD_CONFIG_  >> $(BUILD_CONFIG_H) 
#	@echo #define BUILDCHANGELIST "$(BUILDCHANGELIST)" >> $(BUILD_CONFIG_H)  
#	@echo #define BUILDOWNER "$(BUILDOWNER)" >> $(BUILD_CONFIG_H)  
#	@echo #endif >> $(BUILD_CONFIG_H) 
