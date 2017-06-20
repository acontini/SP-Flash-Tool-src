#include "SCIRBCommand.h"
#include "./Logger/Log.h"
#include "./Utility/FileUtils.h"
#include "./Resource/Handle/RBHandle.h"
#include "./XMLParser/XMLDocument.h"
#include "../Resource/ResourceManager.h"
#include "../UI/src/MainWindowCallback.h"

namespace APCore
{
SCIRBCommand::SCIRBCommand(APKey key):
    ICommand(key),
    rb_setting_(NULL),
    key_(key)
{
}

SCIRBCommand::~SCIRBCommand()
{

}

unsigned long long SCIRBCommand::calculateFlashSize(DA_REPORT_T &da_report)
{
    unsigned long long flash_size=0;

    if(da_report.m_nand_ret == S_DONE)
    {
        flash_size = da_report.m_nand_flash_size;
    }
    else if(da_report.m_emmc_ret == S_DONE)
    {
        flash_size = da_report.m_emmc_boot1_size + da_report.m_emmc_boot2_size+ da_report.m_emmc_gp1_size
                + da_report.m_emmc_gp2_size+da_report.m_emmc_gp3_size + da_report.m_emmc_gp4_size
                + da_report.m_emmc_rpmb_size + da_report.m_emmc_ua_size;
    }

    return flash_size;
}

void SCIRBCommand::read_pmt(const QSharedPointer<Connection> &conn)
{
    FLASHTOOL_API_HANDLE_T ft_handle =
            conn->FTHandle();

    unsigned int count = 0;
    PART_INFO* p_part_info = NULL;
    PART_INFO* p_tmp_part_info = NULL;

    int ret = FlashTool_ReadPartitionCount(ft_handle, &count);

    if( S_DONE != ret ) {
        LOG("ERROR: FlashTool_ReadPartitionCount fail, errorcode is %d", ret);
        return;
    }

    p_tmp_part_info = new PART_INFO[count];
    ret = FlashTool_ReadPartitionInfo(ft_handle, p_tmp_part_info, count);

    if(S_DONE != ret){
        LOG("ERROR: FlashTool_ReadPartitionInfo fail, errorcode is %d", ret);
        return;
    }

    unsigned long long flash_size = calculateFlashSize(conn->da_report());

    /*add preloader/SGPT/PGPT partition info
    preloader: start_adrr=0, length=boot1.size, part_id: EMMC_PART_BOOT1
    PGPT: start_addr=0, length=p_tmp_part_info[0].start_addr, part_id:EMMC_PART_USER
    SGPT: start_addr=p_tmp_part_info[count-1].(start_addr+length), length = flashsize - start_addr, part_id= EMMC_PART_USER
    */
    bool needModifyPartInfo = p_tmp_part_info[0].part_id != 0 && p_tmp_part_info[0].begin_addr != 0;//part_id ==0, EMMC_PART_UNKNOWN
    if(needModifyPartInfo)
    {
      count += 3; //preloader, PGPT, SGPT
    }
    p_part_info = new PART_INFO[count];
    for(unsigned int i=0; i<(needModifyPartInfo?count-3:count); i++)
    {
        p_part_info[i] = p_tmp_part_info[i];
    }
    if(needModifyPartInfo)
    {
        strcpy(p_part_info[count-3].name, "PGPT");
        p_part_info[count-3].begin_addr = 0;
        p_part_info[count-3].image_length = p_tmp_part_info[0].begin_addr;
        p_part_info[count-3].part_id = 8; //EMMC_PART_USER

        strcpy(p_part_info[count-2].name, "SGPT");
        p_part_info[count-2].begin_addr = p_tmp_part_info[count-4].begin_addr + p_tmp_part_info[count-4].image_length;
        p_part_info[count-2].image_length = conn->da_report().m_emmc_ua_size - p_part_info[count-2].begin_addr;
        p_part_info[count-2].part_id = 8; //EMMC_PART_USER

        strcpy(p_part_info[count-1].name, "preloader");
        p_part_info[count-1].begin_addr = 0;
        p_part_info[count-1].image_length = conn->da_report().m_emmc_boot1_size;
        p_part_info[count-1].part_id = 1; //EMMC_PART_BOOT1

    }
    /*end of add preloader/SGPT/PGPT partition info*/

    //bool lastFlag = false;
    unsigned int block_number = 0;

    std::string file_path;

    for(unsigned int i = 0; i < count; i++)
    {
        CloneInfo *info = new CloneInfo();
        std::string name( p_part_info[i].name);

        LOG("Begin partition %s", name.c_str());

        info->setPartitionName(name);

        LOG("Begin create partition clone part-id...");

        info->setPartID((unsigned int)p_part_info[i].part_id);

        LOG("Begin create partition clone info length...");

        info->setPartitionLen(p_part_info[i].image_length);

        if( name.find("RSV") != std::string::npos ||
                name.find("BMTPOOL") != std::string::npos ||
                (p_part_info[i].begin_addr & 0xFFFF0000) == 0xFFFF0000 )
        {
            //lastFlag = true;
            block_number = p_part_info[i].begin_addr & 0x0000FFFF;

            info->setStartAddress(flash_size - 128*1024*block_number);
        }
        else
        {
            info->setStartAddress(p_part_info[i].begin_addr);
        }

        if(info->partition_len()<=512)
        {
            //lastFlag = true;
            info->setPartitionLen(flash_size - info->start_addr());
        }

        LOG("Start addr(%I64X), Length(%I64X)", info->start_addr(),info->partition_len());

        file_path = folder_path_ + C_SEP_STR + p_part_info[i].name;

        if(FileUtils::CreateFiles(file_path))
            info->setImageLocation(file_path);

        CLONE_ITEM item(info);

        cloneItem_list_.push_back(item);
    }

    if(NULL != p_tmp_part_info)
    {
        delete[] p_tmp_part_info;
        p_tmp_part_info = NULL;
    }

    if(NULL != p_part_info)
    {
        delete[] p_part_info;
        p_part_info = NULL;
    }
}

void SCIRBCommand::DoReadback(const QSharedPointer<Connection> &conn)
{
    APCore::RBHandle *rb_handle = GET_RB_HANDLE(key_);

    int i = 0;

    rb_handle->ClearAll();

    std::list<CLONE_ITEM>::const_iterator it = cloneItem_list_.begin();

    while (it != cloneItem_list_.end())
    {
        LOGD("reading: %08llx, %08llx, %s",
             (*it)->start_addr(),
             (*it)->partition_len(), (*it)->image_path().c_str());

        ReadbackItem item(
                    i++,
                    true,
                    (*it)->start_addr(),
                    (*it)->partition_len(),
                    (*it)->image_path(),
                    NUTL_READ_PAGE_SPARE,
                    (*it)->part_id());

        rb_handle->AppendItem(item);

        ++ it;
    }

    if (rb_handle->GetCount() > 0)
    {
        DoCommand(conn);
        rb_handle->ClearAll();

        SaveCheckSum();

        SaveXMLFile();
    }
    else
    {
        LOGD("nothing to read");
    }
}

void SCIRBCommand::SaveCheckSum()
{
    std::list<CLONE_ITEM>::const_iterator it = cloneItem_list_.begin();
    unsigned char chksum_value = 0;

    RomMemChecksumArg chksum_arg;
    chksum_arg.chksum = chksum_value;
    chksum_arg.m_cb_rom_mem_checksum_init = MainWindowCallback::memChecksumInit;
    chksum_arg.m_cb_rom_mem_checksum = MainWindowCallback::memChecksumPro;

    while(it != cloneItem_list_.end())
    {
        FileUtils::ComputeCloneCheckSum((*it)->image_path(), chksum_arg);

        LOG("checksum_value(0x%x)", chksum_arg.chksum);
        (*it)->setChksumValue(chksum_arg.chksum);
        it++;
    }
}

void SCIRBCommand::SaveXMLFile()
{
    std::string file_path = folder_path_ + C_SEP_STR + "clone_info.xml";

    XML::Document document("1.0","UTF-8","clone-info");
    XML::Node rootNode = document.GetRootNode();

    for(std::list<CLONE_ITEM>::const_iterator it = cloneItem_list_.begin();
        it != cloneItem_list_.end(); ++it)
    {
        (*it)->SaveXML(rootNode);
    }

    document.Beautify();
    document.Save(file_path);
}

void SCIRBCommand::DoCommand(const QSharedPointer<Connection> &conn)
{
    rb_setting_->CreateCommand(key_)->exec(conn);
}

void SCIRBCommand::exec(const QSharedPointer<Connection> &conn)
{
    conn->ConnectDA();

    read_pmt(conn);

    DoReadback(conn);
}

}
