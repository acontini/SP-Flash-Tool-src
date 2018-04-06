# -------------------------------------------------
# Project created by QtCreator 2011-08-01T21:54:23
# -------------------------------------------------
#QT += core
#QT -= gui
TARGET = flash_tool
QT    += xmlpatterns
QT += webkit network
#QT += testlib

# CONFIG   += console
#CONFIG -= app_bundle
DEFINES += QT_NO_CAST_FROM_CAST
TRANSLATIONS +=Flashtool_en.ts Flashtool_zh_CN.ts Flashtool_zh_TW.ts
TEMPLATE = app
win32:RELEASE_LIBRARY_FILES += $$PWD/Lib/*.lib $$PWD/lib/*.dll $$PWD/Lib/QtWin/*.lib $$PWD/lib/QtWin/*.dll
unix:RELEASE_LIBRARY_FILES += $$PWD/Lib/*.so* $$PWD/Lib/QtLinux/*.so*
win32:TARGET_FILE = $$quote($$OUT_PWD)/release/flash_tool.exe
unix:TARGET_FILE += $$quote($$OUT_PWD)/flash_tool
MISC_FILES = $$PWD/Lib/*.xml $$PWD/Lib/*.bin $$PWD/*.sh $$PWD/Lib/*.xsd
# copy command in windows cannot support c:/xxx/xxx/xxx and only support c:\xxx\xxx\xxx
win32:MISC_FILES = $$replace(MISC_FILES, /, \\)
win32:RELEASE_LIBRARY_FILES = $$replace(RELEASE_LIBRARY_FILES, /, \\)
win32:TARGET_FILE = $$replace(TARGET_FILE, /, \\)
win32:COMMAND_SEPERATE = &
unix:MISC_FILES = $$replace(MISC_FILES, \\, /)
unix:RELEASE_LIBRARY_FILES = $$replace(RELEASE_LIBRARY_FILES, \\, /)
unix:TARGET_FILE = $$replace(TARGET_FILE, \\, /)
unix:COMMAND_SEPERATE = ;

win32:{
    DEFINES += "_CRT_SECURE_NO_WARNINGS"
}
unix:{
    DEFINES += "_LINUX"
    QMAKE_LFLAGS += -Wl,-rpath,.
    QMAKE_LFLAGS += -Wl,-rpath,lib
    QMAKE_LFLAGS += -Wl,-rpath,qt
}

win32:LIBS += -L$$quote($$PWD/lib) \
    -L$$quote($$PWD/lib/QtWin) \
    -lFlashToolLib \
    -lFlashtoollibEx \
    -lSLA_Challenge

unix:LIBS += -L$$quote($$PWD/Lib) \
    -L$$quote($$PWD/Lib/QtLinux) \
    -lflashtool \
    -lflashtoolEx \
    -lsla_challenge

SOURCES += \
    Conn/Connection.cpp \
    Logger/Log.cpp \
    Public/AppCore.cpp \
    Resource/Handle/DAHandle.cpp \
    Resource/Handle/DLHandle.cpp \
    main.cpp \
    XMLParser/XMLNodeImpl.cpp \
    XMLParser/XMLDocument.cpp \
    XMLParser/tinyxmlparser.cpp \
    XMLParser/tinyxmlerror.cpp \
    XMLParser/tinyxml.cpp \
    XMLParser/tinystr.cpp \
    XMLParser/XMLNode.cpp \
    Logger/Logger.cpp \
    XMLParser/XMLDocumentImpl.cpp \
    Utility/Utils.cpp \
    Utility/USBCOMFinderWrapper.cpp \
    Resource/ResourceManager.cpp \
    Resource/CoreHandle.cpp \
    Cmd/DADownloadAll.cpp \
    Cmd/FormatCommand.cpp \
    Err/BromException.cpp \
    Err/AppException.cpp \
    Err/ErrorLookup.cpp \
    Err/BaseException.cpp \
    UI/src/MainWindow.cpp \
    UI/src/TabWidgetBase.cpp \
    UI/src/DownloadWidget.cpp \
    UI/src/ReadBackWidget.cpp \
    UI/src/MemoryTestWidget.cpp \
    UI/src/DeviceTestWidget.cpp \
    UI/src/ParameterWidget.cpp \
    UI/src/AboutDialog.cpp \
    UI/src/OptionDialog.cpp \
    UI/src/MainController.cpp \
    UI/src/ProcessingAnimation.cpp \
    UI/src/ProcessingDialog.cpp \
    UI/src/AsyncResourceLoader.cpp \
    Arg/BootRomArg.cpp \
    UI/src/MainWindowCallback.cpp \
    Setting/DADownloadAllSetting.cpp \
    UI/src/BackgroundWorker.cpp \
    Err/UserStopException.cpp \
    Setting/ConnSetting.cpp \
    Setting/FormatSetting.cpp \
    ConsoleMode/Config.cpp \
    ConsoleMode/GeneralSetting.cpp \
    ConsoleMode/ConsoleModeEntry.cpp \
    Setting/ConnBromUARTSetting.cpp \
    Setting/ConnBromUSBSetting.cpp \
    FlashToolApp.cpp \
    Arg/BromMemtestArg.cpp \
    Setting/MemoryTestSetting.cpp \
    Cmd/MemoryTestCommand.cpp \
    Setting/DeviceTestSetting.cpp \
    Cmd/DeviceTestCommand.cpp \
    Arg/BromReadbackArg.cpp \
    Setting/ReadbackSetting.cpp \
    Cmd/ReadbackCommand.cpp \
    Resource/Handle/RBHandle.cpp \
    UI/src/ReadBackAddressDialog.cpp \
    Conn/USBSetting.cpp \
    ConsoleMode/SchemaValidator.cpp \
    Arg/BromDownloadArg.cpp \
    Arg/BromFormatArg.cpp \
    Rules/PlatformRule.cpp \
    Rules/StorageRule.cpp \
    Utility/FileUtils.cpp \
    Setting/PlatformSetting.cpp \
    Arg/EMMCFormatArg.cpp \
    Arg/NANDForamtArg.cpp \
    Arg/AutoFormatArg.cpp \
    Rules/ReadbackRule.cpp \
    Cmd/OTPCommand.cpp \
    Cmd/FirmwareUpgradeCommand.cpp \
    Setting/FirmwareUpgradeSetting.cpp \
    Arg/BromOTPArg.cpp \
    Cmd/WriteMemoryCommand.cpp \
    Setting/WriteMemorySetting.cpp \
    Setting/OTPSetting.cpp \
    UI/src/OTPWriteDialog.cpp \
    UI/src/OTPReadDialog.cpp \
    Utility/version.cpp \
    Arg/WriteMemoryArg.cpp \
    UI/src/OkDialog.cpp \
    UI/src/FormatWidget.cpp \
    UI/src/WriteMemoryWidget.cpp \
    UI/src/UpdateDialog.cpp \
    UI/src/CheckUpdateDialog.cpp \
    UI/src/AsyncUpdater.cpp \
    Arg/BromChksumArg.cpp \
    Setting/ChksumSetting.cpp \
    Rules/ChksumRule.cpp \
    Flow/romfactory.cpp \
    Flow/dlproxy.cpp \
    Flow/defaultrom.cpp \
    Flow/concreterom.cpp \
    Flow/dlproxyimpl.cpp \
    Flow/ErrString.cpp \
    Flow/storhelper.cpp \
    Utility/constString.cpp \
    UI/src/PlatformObj.cpp \
    Resource/Handle/ScretHandle.cpp \
    Resource/Handle/AuthHandle.cpp \
    Cmd/CertDLCommand.cpp \
    Setting/CertDLSetting.cpp \
    UI/src/WelcomeWidget.cpp \
    UI/src/Assistant.cpp \
    UI/src/CheckHeader.cpp \
    Flow/NvramVerify.cpp \
    Flow/restoreworker.cpp \
    ConsoleMode/CommandLineArguments.cpp \
    Cmd/WatchDogCommand.cpp \
    Setting/WatchDogSetting.cpp \
    Utility/IniItem.cpp \
    Utility/SizeCalcHelper.cpp \
    Conn/ConnImpl.cpp \
    Conn/ConnMedia.cpp \
    Conn/ConnArgBox.cpp \
    Cmd/EfuseCommand.cpp \
    Setting/EfuseSetting.cpp \
    UI/src/connob.cpp \
    UI/src/BromAdapterWidget.cpp \
    Setting/ComboFormatSetting.cpp \
    Cmd/ComboFormatCommand.cpp \
    Cmd/BromAdapterCommand.cpp \
    Arg/BromAdapterArg.cpp \ 
    Setting/BromAdapterSetting.cpp \
    Public/RomInfoFactory.cpp \
    UI/src/SDMMCWidget.cpp \
    UI/src/NandWidgetd.cpp \
    UI/src/EMMCWidget.cpp \
    UI/src/ChipInfoWidget.cpp \
    UI/src/NorFlashWidget.cpp \
    Arg/SDMMCAutoFormatArg.cpp \
    UI/src/ScatterObserver.cpp \
    Setting/DLOnlySetting.cpp \
    Cmd/DLOnlyCommand.cpp \
    Setting/SecDLSetting.cpp \
    Cmd/SecDLCommand.cpp \
    Utility/sendreport.cpp \
    UI/src/SCIDownloadWidget.cpp \
    UI/src/CloneDownloadWidget.cpp \
    UI/src/EncryDialog.cpp \
    UI/src/DLDelegate.cpp \
    Cmd/SCIRBCommand.cpp \
    Setting/SCIRBSetting.cpp \
    Setting/SCIRestoreSetting.cpp \
    Cmd/SCIRestoreCommand.cpp \
    Utility/CloneInfo.cpp \
    Setting/SCIDownloadSetting.cpp \
    Cmd/SCIDownloadCommand.cpp \
    Setting/DLPlugFmtAllSetting.cpp \
    Cmd/DLPlugFmtAllCommand.cpp \
    TimeStamp/TempPlcCtrl.cpp \
    ConsoleMode/CommandSetting.cpp \
    Arg/GeneralArg.cpp \
    Cmd/GeneralCommand.cpp \
    Cmd/MacroCommand.cpp \
    Arg/UFSFormatArg.cpp \
    UI/src/UfsWidget.cpp \
    Arg/NORFormatArg.cpp \
    JSONParser/json.cpp \
    Setting/DLWithoutScatterSetting.cpp \
    Cmd/DLWithoutScatterCommand.cpp \
    Setting/ReadbackWithoutScatterSetting.cpp \
    Cmd/ReadbackWithoutScatterCommand.cpp \
    Setting/SetBootModeSetting.cpp \
    Cmd/SetBootModeCommand.cpp \
    Setting/CheckRPMBSetting.cpp \
    Cmd/CheckRPMBCommand.cpp \
    Setting/ReadbackSettingAssist.cpp

# ONLY Windows related begins here
win32:SOURCES += Host/Win/RuntimeMemory.cpp \
    Host/Win/DeviceConn.cpp

# ONLY Linux related begins here
unix:SOURCES += Host/Linux/RuntimeMemory.cpp \
    Host/Linux/DeviceScan.cpp \
    Host/Linux/DeviceConn.cpp

HEADERS += \
    Conn/Connection.h \
    Logger/Log.h \
    Public/Host.h \
    Public/AppCore.h \
    Resource/Handle/DAHandle.h \
    Resource/Handle/DLHandle.h \
    XMLParser/XMLNodeImpl.h \
    XMLParser/XMLDocument.h \
    XMLParser/tinyxml.h \
    XMLParser/tinystr.h \
    XMLParser/XMLSerializable.h \
    XMLParser/XMLNode.h \
    Logger/Logger.h \
    ResourceManager/NonCopyable.h \
    BootRom/mtk_status.h \
    BootRom/mtk_resource.h \
    BootRom/mtk_mcu.h \
    BootRom/host.h \
    BootRom/flashtool_handle.h \
    BootRom/flashtool_api.h \
    BootRom/flashtool.h \
    BootRom/external_memory_config.h \
    BootRom/DOWNLOAD.H \
    BootRom/com_enum.h \
    BootRom/brom.h \
    Cmd/ICommand.h \
    Utility/Utils.h \
    Utility/USBCOMFinderWrapper.h \
    Resource/ResourceManager.h \
    ResourceManager/HandlerManager.h \
    ResourceManager/Common.h \
    Resource/CoreHandle.h \
    Cmd/DADownloadAll.h \
    Cmd/FormatCommand.h \
    Err/Exception.h \
    Err/BromException.h \
    Err/AppException.h \
    Err/ErrorLookup.h \
    Err/BaseException.h \
    UI/src/MainWindow.h \
    UI/src/TabWidgetBase.h \
    UI/src/DownloadWidget.h \
    UI/src/ReadBackWidget.h \
    UI/src/MemoryTestWidget.h \
    UI/src/DeviceTestWidget.h \
    UI/src/ParameterWidget.h \
    UI/src/AboutDialog.h \
    UI/src/OptionDialog.h \
    UI/src/MainController.h \
    UI/src/ProcessingAnimation.h \
    UI/src/ProcessingDialog.h \
    UI/src/ICallback.h \
    UI/src/AsyncResourceLoader.h \
    Public/AppTypes.h \
    Arg/BootRomArg.h \
    UI/src/MainWindowCallback.h \
    Setting/ISetting.h \
    Setting/DADownloadAllSetting.h \
    UI/src/BackgroundWorker.h \
    Err/UserStopException.h \
    Setting/ConnSetting.h \
    Setting/FormatSetting.h \
    ConsoleMode/Config.h \
    ConsoleMode/GeneralSetting.h \
    ConsoleMode/ConsoleModeEntry.h \
    Setting/ConnBromUARTSetting.h \
    Setting/ConnBromUSBSetting.h \
    FlashToolApp.h \
    Arg/BromMemtestArg.h \
    Setting/MemoryTestSetting.h \
    Cmd/MemoryTestCommand.h \
    Setting/DeviceTestSetting.h \
    Cmd/DeviceTestCommand.h \
    Arg/BromReadbackArg.h \
    Setting/ReadbackSetting.h \
    Cmd/ReadbackCommand.h \
    Resource/Handle/RBHandle.h \
    UI/src/ReadBackAddressDialog.h \
    Conn/USBSetting.h \
    ConsoleMode/SchemaValidator.h \
    Arg/BromDownloadArg.h \
    Rules/StorageRule.h \
    Rules/PlatformRule.h \
    Utility/FileUtils.h \
    Setting/PlatformSetting.h \
    Arg/BromFormatArg.h \
    Arg/AutoFormatArg.h \
    Arg/NANDFormatArg.h \
    Arg/EMMCFormatArg.h \
    Arg/UFSFormatArg.h \
    Arg/NORFormatArg.h \
    Arg/BromOTPArg.h \
    Cmd/OTPCommand.h \
    Cmd/FirmwareUpgradeCommand.h \
    Setting/FirmwareUpgradeSetting.h \
    Cmd/WriteMemoryCommand.h \
    Arg/WriteMemoryArg.h \
    Setting/WriteMemorySetting.h \
    Setting/OTPSetting.h \
    UI/src/OTPReadDialog.h \
    Utility/version.h \
    UI/src/OTPWriteDialog.h \
    UI/src/OkDialog.h \
    UI/src/FormatWidget.h \
    UI/src/WriteMemoryWidget.h \
    UI/src/UpdateDialog.h \
    UI/src/CheckUpdateDialog.h \
    UI/src/AsyncUpdater.h \
    UI/src/UfsWidget.h \
    Arg/BromChksumArg.h \
    Setting/ChksumSetting.h \
    Rules/ChksumRule.h \
    Flow/romfactory.h \
    Flow/rombase.h \
    Flow/fw_defs.h \
    Flow/dlproxy.h \
    Flow/defaultrom.h \
    Flow/concreterom.h \
    Flow/dlproxyimpl.h \
    Flow/storhelper.h \
    Utility/constString.h \
    UI/src/PlatformObj.h \
    Resource/Handle/ScretHandle.h \
    Resource/Handle/AuthHandle.h \
    Cmd/CertDLCommand.h \
    Setting/CertDLSetting.h \
    UI/src/WelcomeWidget.h \
    UI/src/Assistant.h \
    UI/src/CheckHeader.h \
    Flow/NvramVerify.h \
    Flow/restoreworker.h \
    ConsoleMode/CommandLineArguments.h \
    Cmd/WatchDogCommand.h \
    Setting/WatchDogSetting.h \
    Utility/IniItem.h \
    Utility/SizeCalcHelper.h \
    Conn/ConnImpl.h \
    Conn/ConnMedia.h \
    Conn/ConnArgBox.h \
    Cmd/EfuseCommand.h \
    Setting/EfuseSetting.h \
    UI/src/connob.h \
    Resource/Handle/SuperHandle.h \
    UI/src/BromAdapterWidget.h \
    Setting/BromAdapterSetting.h \
    Cmd/BromAdapterCommand.h \
    Arg/BromAdapterArg.h \ 
    BootRom/SLA_Challenge.h \
    Setting/ComboFormatSetting.h \
    Cmd/ComboFormatCommand.h \
    Host/Inc/RuntimeMemory.h \
    Host/Inc/DeviceConn.h \
    Public/RomInfoFactory.h \
    UI/src/SDMMCWidget.h \
    UI/src/NandWidget.h \
    UI/src/EMMCWidget.h \
    UI/src/ChipInfoWidget.h \
    UI/src/NorFlashWidget.h \
    Arg/SDMMCAutoFormatArg.h \
    UI/src/ScatterObserver.h \
    Setting/DLOnlySetting.h \
    Cmd/DLOnlyCommand.h \
    Setting/SecDLSetting.h \
    Cmd/SecDLCommand.h \
    Utility/sendreport.h \
    UI/src/SCIDownloadWidget.h \
    UI/src/CloneDownloadWidget.h \
    UI/src/EncryDialog.h \
    UI/src/DLDelegate.h \
    Cmd/SCIRBCommand.h \
    Setting/SCIRBSetting.h \
    Setting/SCIRestoreSetting.h \
    Cmd/SCIRestoreCommand.h \
    Utility/CloneInfo.h \
    Setting/SCIDownloadSetting.h \
    Cmd/SCIDownloadCommand.h \
    Setting/DLPlugFmtAllSetting.h \
    Cmd/DLPlugFmtAllCommand.h \
    TimeStamp/TempPlcCtrl.h \
    ConsoleMode/CommandSetting.h \
    Arg/GeneralArg.h \
    Cmd/GeneralCommand.h \
    Cmd/MacroCommand.h \
    JSONParser/json.h \
    Setting/DLWithoutScatterSetting.h \
    BootRom/flashtoolex_api.h \
    BootRom/flashtoolex_struct.h \
    Cmd/DLWithoutScatterCommand.h \
    BootRom/type_define.h \
    BootRom/common_struct.h \
    Setting/ReadbackWithoutScatterSetting.h \
    Cmd/ReadbackWithoutScatterCommand.h \
    Setting/SetBootModeSetting.h \
    Cmd/SetBootModeCommand.h \
    Setting/CheckRPMBSetting.h \
    Cmd/CheckRPMBCommand.h \
    Setting/ReadbackSettingAssist.h

linux:HEADERS += Linux/WINDEF.H \
    Linux/linux_flash_tool.h \
    Host/Linux/DeviceScan.h

OTHER_FILES += Lib/FlashToolLib.lib \
    Rules/image_map.xml

FORMS += \
    UI/forms/SecuritySettingDialog.ui \
    UI/forms/ReadBackAddressDialog.ui \
    UI/forms/ProcessingDialog.ui \
    UI/forms/MainWindow.ui \
    UI/forms/AboutDialog.ui \
    UI/forms/TabWidget/ReadBackWidget.ui \
    UI/forms/TabWidget/ParameterWidget.ui \
    UI/forms/TabWidget/MemoryTestWidget.ui \
    UI/forms/TabWidget/DeviceTestWidget.ui \
    UI/forms/TabWidget/DownloadWidget.ui \
    UI/forms/OTPReadDialog.ui \
    UI/forms/OTPWritedialog.ui\
    UI/forms/OkDialog.ui \
    UI/forms/TabWidget/FormatWidget.ui \
    UI/forms/OptionDialog.ui \
    UI/forms/TabWidget/WriteMemoryWidget.ui \
    UI/forms/UpdateDialog.ui \
    UI/forms/CheckUpdateDialog.ui \
    UI/forms/TabWidget/WelcomeWidget.ui \
    UI/forms/TabWidget/BromAdapterWidget.ui \
    UI/forms/SDMMCWidget.ui \
    UI/forms/NandWidget.ui \
    UI/forms/EMMCWidget.ui \
    UI/forms/ChipInfoWidget.ui \
    UI/forms/NorFlashWidget.ui \
    UI/forms/TabWidget/SCIDownloadWidget.ui \
    UI/forms/TabWidget/CloneDownloadWidget.ui \
    UI/forms/EncryDialog.ui \
    UI/forms/UfsWidget.ui


RESOURCES += \
    UI/icon.qrc \
    UI/translations.qrc \
    UI/html.qrc

RC_FILE = UI/flashtool.rc
