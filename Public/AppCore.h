/*
 * AppCore.h
 *  The only public interface for GUI.
 *
 *  Created on: Aug 26, 2011
 *      Author: MTK81019
 */

#ifndef APPCORE_H_
#define APPCORE_H_

#include "Host.h"
#include "AppTypes.h"
#include "../Setting/ChksumSetting.h"
#include "../Rules/ChksumRule.h"
#include "../Utility/Utils.h"

#include <iostream>
#include <list>
#include <QSharedPointer>

class AppCore {
public:
    enum LoggingSwitch{
        EnableLogging,
        DisableLogging
    };

    AppCore();
    ~AppCore();

    //get a session key
    APKey NewKey();

    //load DA file
    void LoadDA(const APKey key, const std::string &da_file);

    //get DA infomation
    void GetDAInfo(const APKey key, DA_INFO *da_info);

    void LoadCertFile(const APKey key, const std::string &secu_file);

    void LoadAuthFile(const APKey key, const std::string &auth_file);

    //load rom images smartly...
    bool AutoLoadRomImages(const APKey key, const std::string &scatter_file);

    void GetPlatformInfo(const APKey key, DL_PlatformInfo &info);

    //get all roms' info for UI(list view)
    void GetROMList(const APKey key, std::list<ImageInfo>& image_list, Download_Scene scene = FORMAT_ALL_DOWNLOAD);

    static _BOOL IsEnableRom(const std::string &rom_name, Download_Scene scene = FORMAT_ALL_DOWNLOAD);

    //enable/disable a selected rom
    void EnableROM(const APKey key, int rom_index, bool enable);
    void EnableROM(const APKey key, const std::string &name, bool enable);
    //load scatter file
    bool LoadScatter(DL_SCATTER_TYPE type, const APKey key, const std::string &scatter_file);

    //load a single image
    bool LoadROM(const APKey key, const std::string &rom_file, int rom_index);

    bool VerifyImageChksum(const APKey key, const std::string &scatter_file);

    void ClearAllRBItem(const APKey key);
    void AppendRBItem(const APKey key,const ReadbackItem& item);
    void DeleteRBItem(const APKey key,unsigned int index);
    void EnableRBItem(const APKey key,unsigned int index, bool does_enable);
    void UpdateRBItem(const APKey key,const ReadbackItem& item);

    void EnableLog(LoggingSwitch logging);
    void EnableDAChksum(const APKey key, int level);
    void SetChksumSetting(const QSharedPointer<APCore::ChksumSetting> &chksumSetting);

    //check does da have been loaded
    bool IsDALoaded(const APKey key);
    bool IsSecuLoaded(const APKey key);
    bool IsAuthLoaded(const APKey key);
    int CheckSecUnlock(const APKey key, const int index);
    bool GetScatterVersion(const APKey key, char* version);
    bool GetScatterHeadInfo(const APKey key, SCATTER_Head_Info *p_info);
    bool IsComboSCIDL(const APKey key);
    void SetStopLoadFlag(const APKey key,int* stop_flag);
private:
    QSharedPointer<APCore::ChksumSetting> chksum_setting_;

};

#endif /* APPCORE_H_ */
