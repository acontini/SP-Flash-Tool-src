#include "PlatformSetting.h"

#include "../Logger/Log.h"
#include "../Utility/Utils.h"
#include "../Err/BaseException.h"
#include "../Host/Inc/RuntimeMemory.h"

namespace APCore{
const int PlatformSetting::DEFAULT_BMT_BLOCKS_MT6573 = 80;
const int PlatformSetting::DEFAULT_PMT_BLOCKS_MT6573(2);

PlatformSetting::PlatformSetting() {
    m_BMT_blocks = 0;
    m_DA_DL_all_enabled = false;
    is_scatter_file_valid_ = true;
    is_storage_type_support_ = true;
    initStorageOperationTable();
}

PlatformSetting::~PlatformSetting() {
}

PlatformSetting* PlatformSetting::Clone()
{
   return new PlatformSetting(*this);
}

void PlatformSetting::initByScatterFile(const std::string &scatterFile, std::string& error_hint)
{
    std::string platformStr = scatterFile;

    size_t dpos = scatterFile.find_last_of(C_SEP_CHR);
    if (dpos != std::string::npos)
    {
        platformStr = scatterFile.substr(dpos + 1);
    }

    //Set load path for handling files in load folder
    set_load_path(scatterFile);
    try {
        Platform new_platform = GetPlatformTable().queryByScatter(platformStr);
        if(!new_platform.isChipSupported()) {
            char buff[256] = {0};
            sprintf(buff, "Platform %s not support on this version.\nPlease update to latest flashtool and try again.", platformStr.c_str());
            error_hint = buff;
            LOGE("Scatter file which is loaded is not be supported!");
            this->is_scatter_file_valid_ = false;
            return;
        } else {
            this->is_scatter_file_valid_ = true;
        }

        m_BMT_blocks = new_platform.getDefaultBMTBlocks();
        m_current_scatter = new_platform.getSimpleName();

        if(m_platform.getName() != new_platform.getName()){

            m_platform = new_platform;
            LOG("PlatformSetting::initByScatterFile(): notifying OBs with new platform: %s", m_platform.getSimpleName().c_str());
            notifyObservers();
        }
        LOGD(m_platform.Dump().c_str());
    } catch (const BaseException &ex) {
        LOGE("%s.", ex.err_msg().c_str());
        return;
    }

    std::string error_msg;
    const std::string storage = JudgeStorageOperation(platformStr);
    if (!m_storage_config.QueryStorage(m_platform.getSimpleName(), storage, error_msg)) {
        error_hint = error_msg;
        LOGW("%s.", error_msg.c_str());
        this->is_storage_type_support_ = false;
        notifyObservers();
    } else {
        this->is_storage_type_support_ = true;
        if (m_storage_config.IsOperationChangend(m_platform.getSimpleName(), storage)){
            notifyObservers();
        }
    }
}

bool PlatformSetting::initByNewScatterFile(const DL_PlatformInfo &info, std::string& error_hint)
{
    std::string chip_name(info.m_szBBChipType);
    std::string storage_type(info.m_szStorageType);

    try {
        Platform new_platform = GetPlatformTable().queryByScatter(chip_name);
        if(!new_platform.isChipSupported()) {
            char buff[256] = {0};
            sprintf(buff, "Platform %s not support on this version.\nPlease update to latest flashtool and try again.", chip_name.c_str());
            error_hint = buff;
            LOGE("Scatter file which is loaded is not be supported!");
            this->is_scatter_file_valid_ = false;
            return false;
        } else {
            this->is_scatter_file_valid_ = true;
        }

        m_BMT_blocks = new_platform.getDefaultBMTBlocks();
        m_current_scatter = new_platform.getSimpleName();

        if(m_platform.getName() != new_platform.getName()){

            m_platform = new_platform;
            LOG("PlatformSetting::initByScatterFile(): notifying OBs with new platform: %s", m_platform.getSimpleName().c_str());
            notifyObservers();
        }
        LOGD(m_platform.Dump().c_str());
    } catch (const BaseException &ex) {
        LOGE("%s.", ex.err_msg().c_str());
        return false;
    }

    std::string error_msg;
    const std::string storage = JudgeStorageOperation(storage_type);
    if (!m_storage_config.QueryStorage(m_platform.getSimpleName(), storage, error_msg)) {
        error_hint = error_msg;
        LOGW("%s.", error_msg.c_str());
        this->is_storage_type_support_ = false;
        notifyObservers();
        return false;
    } else {
        this->is_storage_type_support_ = true;
        if (m_storage_config.IsOperationChangend(m_platform.getSimpleName(), storage)){
            notifyObservers();
        }
    }

    return true;
}


void PlatformSetting::setChipName(const std::string &chipName) {
    LOG("PlatformSetting::setChipName() : %s", chipName.c_str());

    try {
        Platform new_platform = GetPlatformTable().queryByChipName(chipName);
        m_current_chip = new_platform.getName();

        if(isPlatformMatched()){
            m_platform = new_platform;
        }else{
            //scatter platform is not matched with current chip
            //TODO: throw exception.
            LOG("Platform not matched! scatter: %s, chip: %s.", m_current_scatter.c_str(), m_current_chip.c_str());
        }
    } catch (const BaseException &ex) {
        LOGE("%s.", ex.err_msg().c_str());
        return;
    }

    LOGD(m_platform.Dump().c_str());
}

bool PlatformSetting::isPlatformMatched() {
    return m_current_chip.find(m_current_scatter) != std::string::npos;
}


void PlatformSetting::notifyObservers(){
    for(std::list<IPlatformOb*>::const_iterator it = observers.begin();
        it != observers.end(); ++it){
        (*it)->onPlatformChanged();
    }
}

std::string PlatformSetting::getLoadPlatformName() const{
    LOG("PlatformSetting::getLoadPlatformName(): %s",m_current_scatter.c_str());
    return m_current_scatter;
}

std::string PlatformSetting::getTargetPlatformName() const{
    LOG("PlatformSetting::getTargetPlatformName(): %s",m_current_chip.c_str());
    return m_current_chip;
}

void PlatformSetting::setDADownloadAllEnabled(bool enabled){
    m_DA_DL_all_enabled = enabled;
    notifyObservers();
}

void PlatformSetting::initStorageOperationTable(void) {
    m_storage_oper_table_.push_back("NOR");
    m_storage_oper_table_.push_back("NAND");
    m_storage_oper_table_.push_back("EMMC");
    m_storage_oper_table_.push_back("SDMMC");
    m_storage_oper_table_.push_back("UFS");
}

std::string PlatformSetting::JudgeStorageOperation(const std::string& scatt) {
    std::string oper = "NAND";
    std::string tmp = ToUppercaseString(scatt);
    for (std::list<std::string>::const_iterator it = m_storage_oper_table_.begin();
         it != m_storage_oper_table_.end(); it++) {
        if (tmp.find(*it) != std::string::npos) {
            oper = *it;
            break;
        }
    }
    //If the other storage type could not be found,
    //nand type will be returned
    return oper;
}

void PlatformSetting::set_load_path(const std::string& scatter_file)
{
    size_t dpos = scatter_file.find_last_of('\\');
    if (dpos != std::string::npos) {
        this->load_path_ = scatter_file.substr(0, dpos);
    }
}
}


