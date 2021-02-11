# GDR - 20180617.01 >> distro/release-specific gubbins
OS=$(shell lsb_release -si)
ARCH=$(shell uname -m | sed 's/x86_//;s/i[3-6]86/32/')
VER=$(shell lsb_release -sr)

# - specifically for Ubuntu 16 - path to qmake post installation

# tools
ifeq ($(OS),Ubuntu)
  QMAKE                         := qmake
else
  # GDR - why would this be path-specific anyway?
  QMAKE                         := /opt/QtSDK/Desktop/Qt/473/gcc/bin/qmake
endif
# << GDR - 20180617.01

MAKE                          := make
RM                            := rm
MKDIR                         := mkdir
CDDIR                         := cd
CP                            := cp
MV                            := mv
CHMOD                         := chmod
PWD                           := pwd
QtFlashTool.CCFLAGS           := -r -spec linux-g++-64 CONFIG+=$(BUILD_TYPE)

# path
QtFlashTool.SRC.Path          := $(PWD)

# Dependent files
QtFlashTool.Dependency.Files := \
 $(wildcard Lib/*.xml) \
 $(wildcard Lib/*.xsd) \
 $(wildcard Lib/*.ini) \
 $(wildcard Lib/*.so)  \
 $(wildcard Lib/*.bin) \
 $(wildcard Lib/*.sh) \
 $(wildcard Lib/*.qhc) \
 $(wildcard Lib/*.qch) \
 $(wildcard Lib/Updater) \
 $(wildcard Lib/FlashToolUpdater) \

QtFlashTool.Output.Files := *.xml *.xsd *.ini *.so *.bin *.sh *.qhc *.qch *.txt \
	Updater FlashToolUpdater flash_tool

#Step1: set up environment
.PHONY: set-up
set-up:
	$(eval QtFlashTool.SRC.Path := $(shell $(PWD)))
	$(MKDIR) -p $(OUTPUT_PATH)
	
#Step2: qmake *.pro + make	
all: set-up 
	$(CDDIR) $(OUTPUT_PATH);$(PWD);$(QMAKE) $(QtFlashTool.SRC.Path) $(QtFlashTool.CCFLAGS);$(MAKE);
	$(CP) $(QtFlashTool.Dependency.Files) $(OUTPUT_PATH)
	$(eval QtPlugins := $(OUTPUT_PATH)/codecs  $(OUTPUT_PATH)/imageformats  $(OUTPUT_PATH)/sqldrivers)
	$(RM) -rf $(QtPlugins)
	@echo $(BUILD_TYPE) version build pass
	
.PHONY: clean
clean: 
	$(CDDIR) $(OUTPUT_PATH);$(PWD);$(MAKE) clean
	$(CDDIR) $(OUTPUT_PATH);$(PWD);$(RM) -rf $(QtFlashTool.Output.Files)
