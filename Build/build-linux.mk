# tools
QMAKE                         := /opt/QtSDK/Desktop/Qt/473/gcc/bin/qmake
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

QtFlashTool.Dependency.Dirs := \
 $(wildcard Lib/QtLinux/*)

QtFlashTool.Output.Files := *.xml *.xsd *.ini *.so *.bin *.sh *.qhc *.qch *.txt \
	Updater FlashToolUpdater flash_tool \
	bin lib plugins \
	codecs imageformats sqldrivers

#Step1: set up environment
.PHONY: set-up
set-up:
	$(eval QtFlashTool.SRC.Path := $(shell $(PWD)))
	$(MKDIR) -p $(OUTPUT_PATH)
	
#Step2: qmake *.pro + make	
all: set-up 
	$(CDDIR) $(OUTPUT_PATH);$(PWD);$(QMAKE) $(QtFlashTool.SRC.Path) $(QtFlashTool.CCFLAGS);$(MAKE);
	$(CP) $(QtFlashTool.Dependency.Files) $(OUTPUT_PATH)
	$(CP) -R $(QtFlashTool.Dependency.Dirs) $(OUTPUT_PATH)
	$(eval QtPlugins := $(OUTPUT_PATH)/codecs  $(OUTPUT_PATH)/imageformats  $(OUTPUT_PATH)/sqldrivers)
	$(RM) -rf $(QtPlugins)
	$(CP) -R $(OUTPUT_PATH)/plugins/* $(OUTPUT_PATH)
	@echo $(BUILD_TYPE) version build pass
	
.PHONY: clean
clean: 
	$(CDDIR) $(OUTPUT_PATH);$(PWD);$(MAKE) clean
	$(CDDIR) $(OUTPUT_PATH);$(PWD);$(RM) -rf $(QtFlashTool.Output.Files)


