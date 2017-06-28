#include "../Utility/Utils.h"
#include "../Public/Host.h"
#include "../Err/Exception.h"
#include "../Logger/Log.h"
#include "../Host/Inc/RuntimeMemory.h"
#include "IniItem.h"
#include "FileUtils.h"
#include <QRegExp>
#include <QTime>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <QSettings>
#include <QPainter>
#include <QTextStream>

std::map<unsigned int, std::string>romstring_map;

QString EnumToRomString(HW_StorageType_E storage_,  U32 id)
{
    LOG("EnumToRomString: storage_(%d),region_id(%d) ",storage_, id);
    if(HW_STORAGE_EMMC == storage_)
    {
        switch(id)
        {
        case EMMC_PART_BOOT1:
            return "EMMC_BOOT_1";

        case EMMC_PART_BOOT2:
            return "EMMC_BOOT_2";

        case EMMC_PART_RPMB:
            return "EMMC_RPMB";

        case EMMC_PART_GP1:
            return "EMMC_GP1";

        case EMMC_PART_GP2:
            return "EMMC_GP2";

        case EMMC_PART_GP3:
            return "EMMC_GP3";

        case EMMC_PART_GP4:
            return "EMMC_GP4";

        case EMMC_PART_USER:
            return "EMMC_USER";

        case EMMC_PART_BOOT1_BOOT2:
            return "EMMC_BOOT1_BOOT2";
        }
    }
    else if (HW_STORAGE_UFS == storage_)
    {
        switch(id)
        {
        case UFS_PART_LU0:
            return "UFS_LU0";

        case UFS_PART_LU1:
            return "UFS_LU1";

        case UFS_PART_LU2:
            return "UFS_LU2";

        case UFS_PART_LU0_LU1:
            return "UFS_LU0_LU1";
        }
    }

    return "";
}

std::string DownloadScenetEnumToString(Download_Scene scene)
{
    switch(scene) {
        ENUM_TO_STRING(FORMAT_ALL_DOWNLOAD)
        ENUM_TO_STRING(FIRMWARE_UPGRADE)
        ENUM_TO_STRING(DOWNLOAD_ONLY)
        ENUM_TO_STRING(WIPE_DATA)
        default:
            return "??";
        }
}

std::string EfuseExtraKeyToString(EFUSE_KEY key)
{
    switch(key) {
    ENUM_TO_STRING(C_LOCK)
    ENUM_TO_STRING(C_CTRL1_LOCK)
    ENUM_TO_STRING(C_CTRL0_LOCK)
    ENUM_TO_STRING(C_DATA0_LOCK)
    ENUM_TO_STRING(C_DATA1_LOCK)
    ENUM_TO_STRING(C_DATA2_LOCK)
    ENUM_TO_STRING(C_DATA3_LOCK)
    ENUM_TO_STRING(C_DATA4_LOCK)
    ENUM_TO_STRING(C_DATA5_LOCK)
    ENUM_TO_STRING(CTRL1)
    ENUM_TO_STRING(SEC_CTRL1)
    ENUM_TO_STRING(C_CTRL_0)
    ENUM_TO_STRING(C_CTRL_1)
    ENUM_TO_STRING(C_DATA_0)
    ENUM_TO_STRING(C_DATA_1)
    ENUM_TO_STRING(C_DATA_2)
    ENUM_TO_STRING(C_DATA_3)
    ENUM_TO_STRING(C_DATA_4)
    ENUM_TO_STRING(C_DATA_5)
    ENUM_TO_STRING(SEC_CAP)
    ENUM_TO_STRING(PROD_EN)
    ENUM_TO_STRING(CUSTK)
    default:
        return "??";
    }
}
/*
EMMC_Part_E GetPartID(int index)
{
    EMMC_Part_E ret = EMMC_PART_UNKNOWN;

    switch(index)
    {
      case 0:
        ret = EMMC_PART_BOOT1;
        break;

    case 1:
        ret = EMMC_PART_BOOT2;
        break;

    case 2:
        ret = EMMC_PART_RPMB;
        break;

    case 3:
        ret = EMMC_PART_USER;
        break;
    }

    return ret;
}
*/
int Utils::GetPortNumFromStr(const QString& str)
{
    QRegExp regex("COM(\\d+)");
    if(regex.indexIn(str)!= -1){
        QString port = regex.cap(1);
        return port.toInt();
    }
    return 0;
}

U64 Utils::ConvertStrToAddr(const QString addr_str)
{
    bool convert_ok = true;

    U64 addr = addr_str.toULongLong(&convert_ok,16);

    return convert_ok?addr:0;
}

QString Utils::GetAddrFromIniFile(const BBCHIP_TYPE  bbchip, const QString &file_path)
{
    IniItem item("BromAdapterTool.ini", "ETT_START_ADDRESS_", "bbchip");

    for(int i = 0; i < MAX_SUPPORT_BB; i++)
    {
        QString sectionName = QString("ETT_START_ADDRESS_%1").arg(QString::number(i));

        item.SetSectionName(sectionName);

        QString chip = item.GetStringValue().toLower();
        QString path = file_path.toLower();

        if((path.length() > 0 && path.indexOf(chip) >= 0)
                || BBChipTypeToName(bbchip) == chip)
        {
            item.SetItemName("address");

            return item.GetStringValue();
        }
    }
    return QString();
}

QString Utils::GetPlatformFromScatter(const QString& scatter_file)
{
    QString short_name = ExtractFileFromPath(scatter_file);

    QRegExp regex("[m|M][t|T](\\d{4})");
    if(regex.indexIn(short_name)!= -1){
        QString platform_id = regex.cap(1);
        LOG("get platform from scatter: MT%s",platform_id.toAscii().constData());
        return "MT"+platform_id;
    }
    LOG("[Error]get platform from scatter failed!(%s)", scatter_file.toAscii().constData());
    return QString::null;
}

HW_StorageType_E Utils::GetStorageTypeFromScatter(const QString& scatter_file)
{
    QString short_name = ExtractFileFromPath(scatter_file);

    int emmc_pos = short_name.indexOf("EMMC",0,Qt::CaseInsensitive);
    if(emmc_pos != -1)
    {
        return HW_STORAGE_EMMC;
    }

    int sdmmc_pos = short_name.indexOf("SDMMC",0,Qt::CaseInsensitive);
    if(sdmmc_pos != -1)
    {
        return HW_STORAGE_SDMMC;
    }

    int nor_pos = short_name.indexOf("NOR",0,Qt::CaseInsensitive);
    if(nor_pos != -1)
    {
        return HW_STORAGE_NOR;
    }

    int ufs_pos = short_name.indexOf("UFS",0,Qt::CaseInsensitive);
    if(ufs_pos != -1)
    {
        return HW_STORAGE_UFS;
    }

    return HW_STORAGE_NAND;
}


bool Utils::IsFoundDLImageByScatterFile(DL_HANDLE_T p_dl_handle,
                                   ROM_INFO *p_rom_info,
                                        const std::string &rom_name)
{
    Q_ASSERT((p_dl_handle != NULL) && "IsFindDLImage(): p_dl_handle is NULL!");
    Q_ASSERT((p_rom_info != NULL) && "IsFindDLImage(): p_rom_info is NULL!");
    Q_ASSERT((!rom_name.empty()) && "IsFindDLImage(): rom_name is empty!");

    LOG("IsFoundDLImageByScatterFile(): image to be found is (%s).", rom_name.c_str());
    int ret = 0;
    unsigned short count = 0;
    bool is_found = false;
    if (S_DONE == (ret = DL_GetCount(p_dl_handle, &count)))
    {
        ROM_INFO rom[MAX_LOAD_SECTIONS];
        if (S_DONE == (ret = DL_Rom_GetInfoAll(p_dl_handle, rom, MAX_LOAD_SECTIONS)))
        {
            for (int i = 0; i < count; i++)
            {
                if( 0 == rom_name.compare(rom[i].name) )
                {
                    memcpy(p_rom_info, (rom + i), sizeof(ROM_INFO));
                    LOG("IsFoundDLImageByScatterFile(): \
                        Original rom info: rom name(%s), begin_addr(0x%x), \
                        image info: image name(%s), begin_addr(0x%x).",
                        rom[i].name, rom[i].begin_addr,
                        p_rom_info->name, p_rom_info->begin_addr);
                    is_found = true;
                    break;
                }
            }
        }
        else
        {
            LOG("ERROR: IsFindDLImage(): DL_Rom_GetInfoAll() failed, brom error code(%d)!", ret);
            return is_found;
        }
    }
    else
    {
        LOG("ERROR: IsFindDLImage(): DL_GetCount() failed, brom error code(%d)!", ret);
        return is_found;
    }

    return is_found;
}

int Utils::GetPartitionInfo(const FLASHTOOL_API_HANDLE_T &flashtool_handle,
                            const std::string &part_name,
                            PART_INFO &part_info)
{
    Q_ASSERT( NULL != flashtool_handle );
    int ret;
    unsigned int rom_count = 0;
    bool isImgNameMatch = false;
    PART_INFO *pPartInfo = NULL;

    do {
        ret = FlashTool_ReadPartitionCount(flashtool_handle, &rom_count);
        if( S_DONE != ret ) {
            LOG("ERROR: FlashTool_ReadPartitionCount fail, errorcode is %d", ret);
            break;
        }
        pPartInfo = new PART_INFO[rom_count];

        ret = FlashTool_ReadPartitionInfo(flashtool_handle, pPartInfo, rom_count);
        if( S_DONE != ret ) {
            LOG("ERROR: FlashTool_ReadPartitionInfo fail!, errorcode is %d", ret);
            break;
        }

        std::string rom_name;
        for(unsigned int i = 0; i < rom_count; i++){
            rom_name = pPartInfo[i].name;
            if(ToUppercaseString(rom_name) == part_name){
                part_info.begin_addr= pPartInfo[i].begin_addr;
                part_info.image_length = pPartInfo[i].image_length;
                isImgNameMatch = true;
                break;
            }
        }

        if(!isImgNameMatch){
            LOG("ERROR: Image that is wanted to read back does not be found!");
            ret = S_FT_READBACK_FAIL;
            break;
        }
    }while(0);

    if( NULL != pPartInfo ){
        delete[] pPartInfo;
        pPartInfo = NULL;
    }

    return ret;
}

QString Utils::GetCurrentFileName(int index)
{
    char infoBuffer[256] = { 0 };

    QDateTime current = QDateTime::currentDateTime();
    QDateTime date;

    switch(index)
    {
    case 0:
        date = current.addDays(-1);
        break;

    case 1:
        date = current.addDays(-7);
        break;

    case 2:
        date = current.addMonths(-1);
        break;

    case 3:
        date = current.addMonths(-3);
        break;
    }

    sprintf(infoBuffer,
            "%02d-%02d-%02d",
            date.date().month(), date.date().day(), date.date().year());

    return "SP_FT_Dump_" + QString(infoBuffer);
}


/**
 * Return string to uppercase
 *
 * @param std::string or char*
 *
 * @return std::string with all characters are uppercase
 */
//template <typename T>
std::string ToUppercaseString(const std::string& str)
{
    std::string tmpstr(str);
    std::transform(tmpstr.begin(), tmpstr.end(), tmpstr.begin(), toupper);
    return tmpstr;
}

std::string ToLowercaseString(const std::string& str)
{
    std::string tmpstr(str);
    std::transform(tmpstr.begin(), tmpstr.end(), tmpstr.begin(), tolower);
    return tmpstr;
}

#if 0
// this func is not implemented right!
std::string WStr2Str(const std::wstring& ws)
{
    std::string temp(ws.length(), '\0');
    std::copy(ws.begin(), ws.end(), temp.begin());
    return temp;
}
#endif

std::wstring Str2WStr(const std::string& s)
{
    const size_t newsize = (s.size())*(sizeof(wchar_t)/sizeof(char)) + 1;
    wchar_t* _dest = NULL;
    int ret = 0;
    std::wstring wcString;

    try {
        _dest = new wchar_t[newsize];
        wmemset(_dest, 0, newsize);
        ret = mbstowcs(_dest, s.c_str(), newsize);
        if ( -1 == ret) {
             LOG("Str2WStr(): mbstowcs() failed!");
        }
        wcString = _dest;
    } catch (std::exception& ex)    {
        LOG("Str2WStr(): Standard exception: %s!", ex.what());
        if (NULL != _dest) {
            delete[] _dest;
        }
    }

    if (NULL != _dest) {
        delete[] _dest;
    }

    return wcString;
}

std::string Utils::ReplaceAllSubstring(const std::string &ori_str, const std::string &sub_str, const std::string &new_sub_str)
{
    std::string temp(ori_str);
    size_t pos(0);
    size_t len_of_sub_str(sub_str.length());
    size_t len_of_new_sub_str(new_sub_str.length());
    while(1) {
        pos = temp.find(sub_str, pos);
        if (pos == std::string::npos) {
            break;
        } else {
            temp.replace(pos, len_of_sub_str, new_sub_str);
            pos += len_of_new_sub_str;
        }
    }
    return temp;
}

std::string Utils::PrettyByteString(U64 bytes)
{
    size_t i = 0;
    char buf[32];
    const char unit[] = "BKMGT";

    // potential overflow!
    bytes *= 100;

    while (bytes >= 1000*100 && i<sizeof(unit)-1)
    {
        bytes /= 1024;
        ++ i;
    }

    sprintf(buf, "%d.%02d%c",
            (int)bytes/100,
            (int)bytes%100,
            (char)unit[i]);

    return buf;
}

QString Utils::ULLToHex(U64 src, unsigned int size)
{
     char buf[24];

     memset(buf, 0, sizeof(buf));

     #ifdef _WIN32
       _snprintf(buf, size, "0x%016I64x", src);
     #else
       snprintf(buf, size, "0x%016llx", src);
     #endif

     return QString::fromAscii(buf);
}

int Utils::GetRomFilesTotalSize(DL_HANDLE_T &dl_handle, U64 *file_size)
{
     U64 file_size_temp(0);
     int ret = S_DONE;
     ROM_INFO roms_info[MAX_LOAD_SECTIONS];

     unsigned short rom_count(0);
     ret = DL_GetCount(dl_handle, &rom_count);

     if(ret != S_DONE)
     {
          LOGE("Get rom count failed(%d).", ret);
          return ret;
     }

     ret = DL_Rom_GetInfoAll(dl_handle, roms_info, MAX_LOAD_SECTIONS);
     if(ret != S_DONE)
     {
          LOGE("Get Rom Info failed.(%d)", ret);
          return ret;
     }

     for(int i =0; i < rom_count; i++)
     {
          if(roms_info[i].enable)
          {
               file_size_temp += roms_info[i].filesize;
          }
     }

     (*file_size) = file_size_temp;

     LOGD("Total size should be download is %sB.", PrettyByteString(*file_size).c_str());

     return S_DONE;

}

const QString Utils::ExtClockToName(const EXT_CLOCK  ext_clock) {
    switch(ext_clock)
    {
    case EXT_13M:
        return "EXT_13M";

    case EXT_26M:
        return QString("EXT_26M");

    case EXT_39M:
        return "EXT_39M";

    case EXT_52M:
        return "EXT_52M";

    case AUTO_DETECT_EXT_CLOCK:
        return "AUTO_DETECT_EXT_CLOCK";

    case UNKNOWN_EXT_CLOCK:
        return "UNKNOWN_EXT_CLOCK";

    default:
        return "??";
    }
}

const QString Utils::ramType(HW_RAMType_E type)
{
    switch (type)
    {
    case HW_RAM_SRAM:
        return "SRAM";

    case HW_RAM_DRAM:
        return "DRAM";

    default:
        return QString("Unknown");
    }
}

std::string Utils::GetSpecifalFolderPath()
{
#ifdef _WIN32
    QSettings reg("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
                       QSettings::NativeFormat);
    QString commonAppDir = reg.value("Common AppData").toString();



    if (!commonAppDir.isEmpty()) {
        return commonAppDir.toStdString() + "\\SP_FT_Logs";
    } else {
        return ABS_PATH("SP_FT_Logs");
    }
#else
    return "/tmp/SP_FT_Logs";
#endif
}

QImage Utils::AddStringToImage(const QString& imagePath, const QString& str, const QString& owner)
{
    QImage image = QPixmap(imagePath).toImage();

    QPainter painter(&image);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);


    QPen pen = painter.pen();
    QColor color;
    color.setRgb(237, 145,33);
    pen.setColor(color);
    QFont font = painter.font();
    font.setBold(true);
    font.setPixelSize(35);

    painter.setPen(pen);
    painter.setFont(font);

    painter.drawText(image.rect(), Qt::AlignCenter, str);

    font.setBold(false);
    font.setPixelSize(20);
    painter.setFont(font);

    painter.drawText(5, image.height()-100, owner);

    return image;
}

bool Utils::parserJSONFile(const QString filePath, QtJson::JsonObject &jsonObject)
{
    QFile file(filePath);
    QString json;
    if(!file.open(QFile::ReadOnly | QFile::Text))
        json = "";
    else
    {
        QTextStream in(&file);
        json = in.readAll();
    }

    if(json.isEmpty()){
        LOGE("Could not read json file %s\n", filePath.toLocal8Bit().constData());
        return false;
    }

    bool ok = false;
    jsonObject = QtJson::parse(json, ok).toMap();

    if(!ok){
        LOGE("An error occured during parsing json file %s\n", filePath.toLocal8Bit().constData());
    }

    return ok;
}
