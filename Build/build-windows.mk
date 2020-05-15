VC_Tools_Path                 := "C:\Program Files\Microsoft Visual Studio 9.0\VC\bin"

# tools
QMAKE				          := C:\QtSDK\Desktop\Qt\4.7.4\msvc2008\bin\qmake.exe
JOM 				          := "C:\Program Files\Microsoft Visual Studio 9.0\VC\bin\nmake.exe"
RM                            := $(TOOLS_DIR)\rm.exe
MKDIR                         := $(TOOLS_DIR)\mkdir.exe
CP                            := $(TOOLS_DIR)\cp.exe
CHMOD                         := $(TOOLS_DIR)\chmod.exe
PWD                           := $(TOOLS_DIR)\pwd.exe
MV                            := $(TOOLS_DIR)\mv.exe
CDDIR                         := cd
SETUP_VCVAR                   := call "C:\Program Files\Microsoft Visual Studio 9.0\VC\bin\vcvars32.bat"

FlashTool.CCFLAGS           := -r -spec win32-msvc2008 "CONFIG+=$(BUILD_TYPE)"

# Dependent files
FlashTool.Dependency.Files := \
 $(wildcard Lib/*.xml) \
 $(wildcard Lib/*.xsd) \
 $(wildcard Lib/*.ini) \
 $(wildcard Lib/*.dll)  \
 $(wildcard Lib/*.bin) \
 $(wildcard Lib/*.qhc) \
 $(wildcard Lib/*.qch) \
 $(wildcard Lib/*.exe) \
 $(wildcard Lib/*.json)

FlashTool.Dependency.Dirs := \
 Lib/QtWin/$(BUILD_TYPE)
 
FlashTool.Output.Files := *.xml *.xsd *.ini *.dll *.bin *.qch *.qhc *.txt *.exe *.json\
	codecs imageformats sqldrivers $(BUILD_TYPE)

#Step1: set up environment
.PHONY: set-up
set-up:
	$(SETUP_VCVAR)
	$(MKDIR) -p $(OUTPUT_PATH)
	
#Step2: qmake *.pro + make	
all: set-up   
	$(CDDIR) $(OUTPUT_PATH) & $(QMAKE) $(WORKING_DIR) $(FlashTool.CCFLAGS)  
	$(CDDIR) $(OUTPUT_PATH) & $(SETUP_VCVAR) & env.exe -u MAKE -u MAKEFLAGS $(JOM)
	$(eval FlashTool.Dir := $(OUTPUT_PATH)/$(BUILD_TYPE))
	$(CP) $(FlashTool.Dependency.Files) $(FlashTool.Dir)
	$(CP) -R $(FlashTool.Dependency.Dirs) $(FlashTool.Dir)
	$(eval QtFlashTool.Special.Dirs := $(FlashTool.Dir)/$(BUILD_TYPE)/codecs  $(FlashTool.Dir)/$(BUILD_TYPE)/imageformats  $(FlashTool.Dir)/$(BUILD_TYPE)/sqldrivers)
	$(RM) -rf $(FlashTool.Dir)/codecs $(FlashTool.Dir)/imageformats $(FlashTool.Dir)/sqldrivers
	$(MV) -u $(QtFlashTool.Special.Dirs) $(FlashTool.Dir)
	$(CP) -f $(FlashTool.Dir)/$(BUILD_TYPE)/* $(FlashTool.Dir)
	$(RM) -rf $(FlashTool.Dir)/$(BUILD_TYPE)
	@echo $(BUILD_TYPE) version build pass
	
.PHONY: clean
clean: 
	$(CDDIR) $(OUTPUT_PATH) & $(SETUP_VCVAR) & env.exe -u MAKE -u MAKEFLAGS $(JOM) clean
	$(CDDIR) $(OUTPUT_PATH)/$(BUILD_TYPE) & $(RM) -rf $(FlashTool.Output.Files)
