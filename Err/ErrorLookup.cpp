/*
 * ErrorLookup.cpp
 *
 *  Created on: Jan 6, 2011
 *      Author: MTK81019
 */
#include "ErrorLookup.h"
#include "../Flow/fw_defs.h"
#include <cassert>
#include <string>
#include <stdio.h>


using std::string;

string ErrorLookup::BromErrorMessage(STATUS_E error_code,
        unsigned int msp_code)
{
    string prefix, errMsg, tipMsg;

    if (S_SECURITY_ERROR_BEGIN <= error_code) {
        char buf[256];
        sprintf(buf,
                " ERROR : %s (%d) , MSP ERROE CODE : 0x%02X. ",
                StatusToString(error_code), error_code, msp_code);
        prefix.append(buf);
    } else if (error_code == -1) {
        prefix.append(" ERROR : FROM UI ");
    } else {
        char buf[256];
        if((unsigned int)error_code < (unsigned int)STATUS_ERR) //for old arch lib
        {
            sprintf(buf,
                    " ERROR : %s (%d)",
                    StatusToString(error_code), error_code);
        }
        else //for new arch lib
        {
            sprintf(buf,
                    " ERROR : %s (0x%X)",
                    StatusToString(error_code), error_code);
        }
        prefix.append(buf);
    }

    switch (error_code) {
    //        case S_STOP: break;
    //        case S_UNDEFINED_ERROR: break;
    //        case S_INVALID_ARGUMENTS: break;
    //        case S_INVALID_BBCHIP_TYPE: break;
    //        case S_INVALID_EXT_CLOCK: break;
    //        case S_GET_DLL_VER_FAIL: break;
    //        case S_INVALID_BUF: break;
    //        case S_BUF_IS_NULL: break;
    //        case S_BUF_LEN_IS_ZERO: break;
    //        case S_BUF_SIZE_TOO_SMALL: break;
    //        case S_NOT_ENOUGH_MEMORY: break;

    case S_COM_PORT_OPEN_FAIL:
        errMsg.append("[COM] Failed to open COM port.");
        break;

    case S_COM_PORT_SET_TIMEOUT_FAIL:
        errMsg.append("[COM] Failed to set COM port timeout.");
        break;

        //        case S_COM_PORT_SET_STATE_FAIL: break;
        //        case S_COM_PORT_PURGE_FAIL: break;
        //        case S_FILEPATH_NOT_SPECIFIED_YET: break;

    case S_UNKNOWN_TARGET_BBCHIP:
        errMsg = "[User] Unsupported target baseband chip type!";
        tipMsg = "Please upgrade to the latest FlashTool then try again.";
        break;

        //        case S_SKIP_BBCHIP_HW_VER_CHECK: break;

    case S_UNSUPPORTED_VER_OF_BOOT_ROM:
        errMsg = "[User] Unsupported Boot ROM !";
        tipMsg = "Please upgrade to the latest FlashTool then try again.";
        break;

    case S_UNSUPPORTED_VER_OF_BOOTLOADER:
        errMsg
                = "[MAUI] Boot Loader Image file contains an unsupported version in its header! Please ask for help.";
        break;

    case S_UNSUPPORTED_VER_OF_DA:
        errMsg
                = "[DA] DA binary file contains an unsupported version in its header! Please ask for help.";
        break;

    case S_UNSUPPORTED_VER_OF_SEC_INFO:
        errMsg
                = "[MAUI] MAUI binary file contains an unsupported security version! Please ask for help.";
        break;

    case S_UNSUPPORTED_VER_OF_ROM_INFO:
        errMsg
                = "[MAUI] MAUI binary file contains an unsupported MAUI version! Please ask for help.";
        break;

    case S_SEC_INFO_NOT_FOUND:
        errMsg
                = "[User] Your ROM files doesn't enable secure! Secure-on BB must boot up with secure-on ROM files!";
        tipMsg = "Please use a secure-on ROM file then try again.";
        break;

    case S_ROM_INFO_NOT_FOUND:
        errMsg
                = "[MAUI] There doesn't exist a header in MAUI file! Please ask for help.";
        break;

    case S_CUST_PARA_NOT_SUPPORTED:
        errMsg
                = "[User] The phone doesn't contain Customer Parameter Area! Please check it again.";
        break;

    case S_CUST_PARA_WRITE_LEN_INCONSISTENT:
        errMsg
                = "[User] The size of Customer Parameter file is inconsistent with that on the phone!\r\n\
                Please check it again.";
        break;

        //        case S_SEC_RO_NOT_SUPPORTED: break;
        //        case S_SEC_RO_WRITE_LEN_INCONSISTENT: break;
        //        case S_ADDR_N_LEN_NOT_32BITS_ALIGNMENT: break;

    case S_UART_CHKSUM_ERROR:
        errMsg
                = "[H/W] Read-back SEC_RO or CUST_PARA and its checksum is mismatched \r\n\
                with the checksum BROM DLL calculates after receiving through UART! ";
        tipMsg = "Please check your UART cable is stable or not.";
        break;

    case S_UNSUPPORTED_VER_OF_EMI_INFO:
        errMsg
                = "[MAUI] MAUI binary file contains an unsupported version of EMI setting! Please ask for help.";
        break;

        //=> BOOT ROM error code (2000~2999)
        //        case S_BROM_SET_META_REG_FAIL: break;
        //        case S_BROM_SET_FLASHTOOL_REG_FAIL: break;
        //        case S_BROM_SET_REMAP_REG_FAIL: break;
        //        case S_BROM_SET_EMI_FAIL: break;

    case S_BROM_DOWNLOAD_DA_FAIL:
        errMsg = "[H/W] Fail to download DA to baseband chip's internal SRAM! ";
        tipMsg =
                "1. There's an cable communication problem between FlashTool and BootROM.\r\n"
                "2. Target might be shutdown unexpectedly or target lost power supply.\r\n"
                "[ACTION]\r\n"
                "1. Try again.\r\n"
                "2. Proceed H/W checking in download cable and target side com port.\r\n"
                "3. Monitor if power-drop occured in target side.";
        break;

    case S_BROM_CMD_STARTCMD_FAIL:
        errMsg
                = "[BROM] Can not pass bootrom start command! Possibly target power up too early.";
        break;

    case S_BROM_CMD_STARTCMD_TIMEOUT:
        errMsg = "[BROM] BootROM start command timeout!";
        break;

        //        case S_BROM_CMD_JUMP_FAIL:break;
        //        case S_BROM_CMD_WRITE16_MEM_FAIL:break;
        //        case S_BROM_CMD_READ16_MEM_FAIL:break;
        //        case S_BROM_CMD_WRITE16_REG_FAIL:break;
        //        case S_BROM_CMD_READ16_REG_FAIL:break;
        //        case S_BROM_CMD_CHKSUM16_MEM_FAIL:break;
        //        case S_BROM_CMD_WRITE32_MEM_FAIL:break;
        //        case S_BROM_CMD_READ32_MEM_FAIL:break;
        //        case S_BROM_CMD_WRITE32_REG_FAIL:break;
        //        case S_BROM_CMD_READ32_REG_FAIL:break;
        //        case S_BROM_CMD_CHKSUM32_MEM_FAIL:break;
        //        case S_BROM_JUMP_TO_META_MODE_FAIL:break;
        //        case S_BROM_WR16_RD16_MEM_RESULT_DIFF:break;

    case S_BROM_CHKSUM16_MEM_RESULT_DIFF:
        errMsg
                = "[H/W] Fail to download DA to baseband chip's internal SRAM!\n\
                         The internal SRAM has the problem that leads to 16 bits checksum error!";
        break;

    case S_BROM_BBCHIP_HW_VER_INCORRECT:
    case S_CHIP_TYPE_NOT_MATCH:
        errMsg = "[H/W] Target baseband chip is inconsistent with scatter files!\n";
        tipMsg
                = "Please select the correct scatter files for the target's baseband chip.";
        break;

    case S_BROM_FAIL_TO_GET_BBCHIP_HW_VER:
        errMsg = "[H/W] Fail to get target baseband chip H/W version!";
        tipMsg
                = "\r\n\
                1. There's an cable communication problem between FlashTool and BootROM.\r\n\
                2. Target might be shutdown unexpectedly.\r\n\
                \r\n\
                [ACTION]\r\n\
                1. Try again.\r\n\
                2. Proceed H/W checking in download cable and target side UART path.\r\n\
                3. Monitor if power-drop occured in target side.";
        break;

    case S_BROM_AUTOBAUD_FAIL:
        errMsg
                = "[H/W] BootROM start command passed but fail to multiple BootROM baudrate!";
        tipMsg
                = "\r\n\
                1. There's an cable communication problem between FlashTool and BootROM.\r\n\
                2. Target might be shutdown unexpectedly.\r\n\
                \r\n\
                [ACTION]\r\n\
                1. Try again.\r\n\
                2. If the problem still exists. Disable \"Options\"->\"Transmission option\"->\"Speed-Up BootROM baudrate\r\n\
                in advance mode (CTRL+ALT+V) then try again.\r\n\
                3. Proceed H/W checking in download cable and target side UART path.\r\n\
                4. Monitor if power-drop occured in target side.";
        break;

    case S_BROM_SPEEDUP_BAUDRATE_FAIL:
        errMsg
                = "[H/W] BootROM start command passed but fail to multiple BootROM baudrate!";
        tipMsg
                = "1. There's an cable communication problem between FlashTool and BootROM.\r\n\
                2. Target might be shutdown unexpectedly.\r\n\
                \r\n\
                [ACTION]\r\n\
                1. Try again.\r\n\
                2. If the problem still exists. Disable \"Options\"->\"Transmission option\"->\"Speed-Up BootROM baudrate\"\r\n\
                in advance mode (CTRL+ALT+V) then try again.\r\n\
                3. Proceed H/W checking in download cable and target side UART path.\r\n\
                4. Monitor if power-drop occured in target side.";
        break;

        //        case S_BROM_LOCK_POWERKEY_FAIL:break;
        //        case S_BROM_WM_APP_MSG_OUT_OF_RANGE:break;
        //        case S_BROM_NOT_SUPPORT_MT6205B:break;

    case S_BROM_EXCEED_MAX_DATA_BLOCKS:
        errMsg = "[MAUI] The number of Multi-Bin files is more than 10!";
        tipMsg = "Please don't build more than 10 binary files.";
        break;

        //        case S_BROM_EXTERNAL_SRAM_DETECTION_FAIL:break;
        //        case S_BROM_EXTERNAL_DRAM_DETECTION_FAIL:break;


        //=> DA error code (3000~3999)

    case S_DA_INT_RAM_ERROR:
        errMsg = "[H/W] DA didn't detect internal RAM!";
        tipMsg = "It maybe a SMT issue. Please ask for help.";
        break;

    case S_DA_EXT_RAM_ERROR:
        errMsg = "[H/W] DA didn't detect external RAM!";
        tipMsg
                = "1. It maybe a SMT issue.\r\n\
                2. It maybe a EMI issue.\r\n\
                Please ask for help.";
        break;

        //        case S_DA_SETUP_DRAM_FAIL:break;
        //        case S_DA_SETUP_PLL_ERR:break;

    case S_DA_DRAM_NOT_SUPPORT:
        errMsg = "[H/W] The handset doesn't support DRAM. Please ask for help.";
        break;

    case S_DA_RAM_FLOARTING:
        errMsg = "[H/W] DA detects that DRAM is floating!";
        tipMsg
                = "1. It maybe a SMT issue.\r\n\
                2. It maybe a EMI issue.\r\n\
                Please ask for help.";
        break;

    case S_DA_RAM_UNACCESSABLE:
        errMsg
                = "[H/W] DA detects that RAM is un-accessable. It maybe a SMT issue. Please ask for help.";
        break;

    case S_DA_RAM_ERROR:
        errMsg
                = "[H/W] DA detects other access error of RAM! Please ask for help.";
        break;

    case S_DA_DEVICE_NOT_FOUND:
        errMsg = "[H/W] DA didn't detect external SRAM or DRAM on the phone.";
        tipMsg
                = "1. It maybe a SMT issue.\r\n\
                2. It maybe a EMI issue.\r\n\
                3. NAND hasn't been validated.\r\n\
                Please check \"Memory_Validation_List_External.xls\" or ask for help.";
        break;

    case S_DA_NAND_UNSUPPORTED_DEV_ID:
        errMsg = "[User] Unsupported NAND Flash type!";
        tipMsg = "Please upgrade to the latest FlashTool then try again.";
        break;

    case S_DA_NAND_FLASH_NOT_FOUND:
        errMsg = "[H/W] DA didn't detect NAND Flash on the phone.";
        tipMsg
                = "1. It maybe a SMT issue.\r\n\
                2. It maybe a NFI issue.\r\n\
                3. NAND hasn't been validated.\r\n\
                Please check \"Memory_Validation_List_External.xl\" or ask for help.";
        break;

    case S_DA_SOC_CHECK_FAIL:
        errMsg = "[User] SOC verification fail!";
        break;

    case S_DA_NAND_PAGE_PROGRAM_FAILED:
        errMsg = "[H/W] DA fails to page-program NAND Flash!";
        tipMsg = "1. It maybe a SMT issue. Please ask for help.";
        break;

    case S_DA_NAND_SPARE_PROGRAM_FAILED:
        errMsg = "[H/W] DA fails to spare-program NAND Flash!";
        tipMsg = "1. It maybe a SMT issue. Please ask for help.";
        break;

    case S_DA_NAND_BLOCK_DATA_UNSTABLE:
        errMsg
                = "[H/W] Data of the block is unstable after erasing NAND Flash.!";
        tipMsg = "1. It maybe a SMT issue. Please ask for help.";
        break;

    case S_DA_INVALID_RANGE:
        errMsg = "[User] DA_MEM_CMD command fail!";
        tipMsg = "1. One of the download blocks has invalid range.\r\n"
            "2. Possibly invalid begin address or end address exceeds target flash size.\n"
            "3. Possibly logcial begin address or length exceeds check boundary.";
        break;

    case S_DA_NFB_BOOTLOADER_NOT_EXIST:
        errMsg
                = "[User] BootLoader doesn't exist on NAND flash! Please download BootLoader first!";
        break;

    case S_DA_NAND_NO_GOOD_BLOCK:
        errMsg = "[H/W] There doesn't exist any good block on NAND Flash!";
        break;

    case S_DA_BOOTLOADER_IS_TOO_LARGE:
        errMsg
                = "[MAUI] Boot Loader Image is more than 7 blocks on NAND Flash!\n\nPlease ask for help.";
        break;

    case S_DA_BLANK_FLASH:
        errMsg
                = "[User] There doesn't exist MAUI code and SEC_RO (option) on the flash.";
        tipMsg
                = "Please choose MAUI file and SEC_RO file (option) for downloading.";
        break;

    case S_DA_CODE_AREA_IS_BLANK:
        errMsg = "[User] There doesn't exist MAUI code on the flash.";
        tipMsg = "Please choose MAUI file for downloading.";
        break;

    case S_DA_SEC_RO_AREA_IS_BLANK:
        errMsg = "[User] There doesn't exist SEC_RO (option) on the flash.";
        tipMsg = "Please choose SEC_RO file (option) for downloading.";
        break;

    case S_DA_UNSUPPORTED_BBCHIP:
        errMsg
                = "[User] BROM DLL is too old to support this baseband chip on the phone!";
        tipMsg = "Please upgrade to the latest FlashTool then try again.";
        break;

    case S_DA_FAT_NOT_EXIST:
        errMsg = "[User] FAT doesn't exist on the phone!";
        break;

    case S_DA_EXT_SRAM_NOT_FOUND:
        errMsg
                = "[H/W] DA didn't detect external SRAM on the phone! Please ask for help.";
        break;

    case S_DA_EXT_DRAM_NOT_FOUND:
        errMsg
                = "[H/W] DA didn't detect external DRAM on the phone! Please ask for help.";
        break;

    case S_DA_MT_PIN_LOW:
        errMsg
                = "[H/W] DA detects that a pin is always low on the phone while memory test! Please ask for help.";
        break;

    case S_DA_MT_PIN_HIGH:
        errMsg
                = "[H/W] DA detects that a pin is always high on the phone while memory test! Please ask for help.";
        break;

    case S_DA_MT_PIN_SHORT:
        errMsg
                = "[H/W] DA detects that two pin is short on the phone while memory test! Please ask for help.";
        break;

    case S_DA_MT_BUS_ERROR:
        errMsg
                = "[H/W] DA detects bus error on the phone while memory test! Please ask for help.";
        break;

    case S_DA_MT_DEDICATED_PATTERN_ERROR:
        errMsg
                = "[H/W] DA detects error on the phone while memory test with dedicated pattern! Please ask for help.";
        break;

    case S_DA_MT_INC_PATTERN_ERROR:
        errMsg
                = "[H/W] DA detects error on the phone while memory test with incremental pattern! Please ask for help.";
        break;

    case S_DA_MT_DEC_PATTERN_ERROR:
        errMsg
                = "[H/W] DA detects error on the phone while memory test with decremental pattern! Please ask for help.";
        break;

    case S_DA_NFB_BLOCK_0_IS_BAD:
        errMsg
                = "[H/W] DA detects Block 0 of NAND Flash is bad! Please ask for help.";
        break;

        //=> FlashTool error code (4000~4999)
        //        case S_FT_CALLBACK_DA_REPORT_FAIL:break;

    case S_FT_DA_NO_RESPONSE:
        errMsg = "DA didn't send response data to FlashTool!";
        break;

    case S_FT_DA_SYNC_INCORRECT:
        errMsg = "[User] DA sync fail! Possibly download an illegal DA.";
        break;

    case S_FT_DA_VERSION_INCORRECT:
        errMsg = "[User] DA version is incorrect!";
        break;

    case S_FT_DA_INIT_SYNC_ERROR:
        errMsg = "Fail to send sync data to DA in initialization stage!";
        break;

        //        case S_FT_GET_DSP_VER_FAIL:break;

    case S_FT_CHANGE_BAUDRATE_FAIL: {
        errMsg = "Fail to change baudrate.";
    }
        break;

    case S_FT_SET_DOWNLOAD_BLOCK_FAIL:
        errMsg = "DA_MEM_CMD command fail! Possibly the following reason.";
        tipMsg
                = "1. Download block exceeds target flash size.\r\n\
                2. Download block start address is not WORD alignment.\r\n\
                3. Download block length is not multiple of WORD.";
        break;

    case S_FT_DOWNLOAD_FAIL:
        errMsg
                = "Download to flash fail! Please press CTRL+ALT+T to turn on debug log then reproduce again!";
        tipMsg
                = "BROM_DLL_V5.log and BROM_DLL.log would be created and record more detail information.";
        break;

    case S_FT_READBACK_FAIL:
        errMsg
                = "Readback from flash fail! Please press CTRL+ALT+T to turn on debug log then reproduce again!";
        tipMsg
                = "BROM_DLL_V5.log and BROM_DLL.log would be created and record more detail information.";
        break;

    case S_FT_FORMAT_FAIL:
        errMsg
                = "Format flash error! Possibly given a wrong format range, please check your format setting.";
        break;

    case S_FT_FINISH_CMD_FAIL:
        errMsg = "DA_FINISH_CMD command fail!";
        break;

        //        case S_FT_ENABLE_WATCHDOG_FAIL:break;

    case S_FT_NFB_DOWNLOAD_BOOTLOADER_FAIL:
        errMsg = "Fail to download BootLoader to NAND flash!";
        break;

    case S_FT_NFB_DOWNLOAD_CODE_FAIL:
        errMsg = "Fail to download Image(s) to NAND flash!";
        break;

        //        case S_FT_NFB_INVALID_BOOTLOADER_DRAM_SETTING:break;

    case S_FT_NAND_READADDR_NOT_PAGE_ALIGNMENT:
        errMsg
                = "[User] The begging address of read-back operation is not page-aligned on NAND Flash!";
        tipMsg = "Please check your read address again.";
        break;

    case S_FT_NAND_READLEN_NOT_PAGE_ALIGNMENT:
        errMsg
                = "[User] The length of read-back operation is not page-aligned on NAND Flash!";
        tipMsg = "Pease check your read address again.";
        break;

    case S_FT_ENABLE_DRAM_FAIL:
        errMsg = "[EMI] Enable DRAM Failed!";
        tipMsg = "Please check your load matches to your target which is to be downloaded.";
        break;

    case S_DL_GET_DRAM_SETTING_FAIL:
        errMsg = "[EMI] Obtain DRAM Failed!";
        tipMsg = "Please check your load matches to your target which is to be downloaded.";
        break;

    case S_FT_FS_FILENAME_INVALID:
        errMsg
                = "[User] The used filename is valid! Please check if your file exists.";
        break;

    case S_FT_FS_FILENAME_TOO_LONG:
        errMsg
                = "[User] The length of used filename is too long! Please check \r\n\
                your filename is valid.";
        break;

        //        case S_FT_FS_ASSERT:break;

    case S_FT_OTP_ADDR_NOT_WORD_ALIGNMENT:
        errMsg
                = "[User] The address given of OTP operation is not word-aligned!";
        tipMsg = "Please check your OTP address again.";
        break;

    case S_FT_OTP_LENGTH_NOT_WORD_ALIGNMENT:
        errMsg
                                = "[User] The length given of OTP operation is not word-aligned! \r\n Please check your OTP length again.";
        break;

    case S_FT_OTP_INVALID_ADDRESS_RANGE:
        errMsg
                                = "[User] The address given of OTP operation is out of range! \r\n Please check your OTP address and length again.";
        break;

        //        case S_FT_NAND_READ_TO_BUFFER_NOT_SUPPORT:break;
        //        case S_FT_GET_PROJECT_ID_FAIL:break;

    case S_FT_ENFB_ROM_FILE_SMALL_THAN_HEADER_DESCRIBE:
        errMsg
                = "[MAUI] The size of E-NFB Rom file is smaller than that described \r\n\
                in header of E-NFB Rom file! Please ask for help.";
        break;

    case S_FT_NEED_DOWNLOAD_ALL_FAIL:
        errMsg = "Need download all images!";
        break;

        case S_FT_GET_MEMORY_FAIL:
                errMsg = "Cannot find memory.\n";
                break;

    case S_FT_UPDATE_PARTITION_TBL_FAIL:
        errMsg = "Update NAND Flash partition table failed!";
        break;

        //        case S_FT_RW_EXTRACT_NFB_FAIL:break;
        //        case S_FT_MEMORY_TEST_FAIL:break;

        //=> FlashTool Handle error code (5000~5999)
    case S_AUTH_HANDLE_IS_NOT_READY:
        errMsg
                = "[User] This is a secure Base-band Chip! Please select the authentication file first.";
        break;

    case S_INVALID_AUTH_FILE:
        errMsg
                = "[User] This is an invalid authentication file with wrong format! Please ask for help.";
        break;

    case S_INVALID_DA_FILE:
        errMsg
                = "[User] This is an invalid DA file with wrong format! Please ask for help.";
        break;

    case S_FTHND_ILLEGAL_INDEX:
        errMsg = "";
        break;

    case S_FTHND_FILE_IS_NOT_LOADED_YET:
        errMsg = "There is file not loaded yet!!";
        tipMsg
                = "\r\n\
                1. Please check if the DA path is correct.\r\n\
                2. Please check if all the ROM files exist.\r\n\
                3. Please check if the scatter file description is sync with the exist ROM files.";
        break;

    case S_FTHND_LIST_IS_EMPTY:
        errMsg = "Download or Readback list is empty or all disabled!";
        tipMsg = "Please select at least one entry.\r\n"
                     "[For Android]Please load scatter file first from load folder.";
        break;

    case S_DL_SCAT_INCORRECT_FORMAT:
        errMsg
                = "[MAUI] The scatter file has incorrect format! Please ask for help.";
        break;

    case S_DL_SCAT_ADDR_IS_NOT_WORD_ALIGN:
        errMsg
                = "[MAUI] The address of some Load Region in the scatter file is\r\n\
                not word-aligned! Please ask for help.";
        break;

    case S_DL_SCAT_OFFSET_IS_NOT_WORD_ALIGN:
        errMsg
                = "[MAUI] The offset of some Load Region in the scatter file is\r\n\
                not word-aligned! Please ask for help.";
        break;

    case S_DL_SCAT_ADDR_IS_NOT_ASCENDING_ORDER:
        errMsg
                = "[MAUI] The addresses of all Load Region in the scatter file\r\n\
                are not ascending in order! Please ask for help.";
        break;

    case S_DL_SCAT_JUMPTABLE_IS_NOT_ABSOLUTE_ADDR:
        errMsg
                = "[MAUI] The address of jump table binary file in the scatter file\r\n\
                is not absolute address! Please ask for help.";
        break;

    case S_DL_LOAD_REGION_IS_OVERLAP:
        errMsg
                = "[MAUI] the addresses of some Load Regions in the scatter file\r\n\
                are overlapped! Please ask for help.";
        break;

    case S_DL_RESOURCE_NOT_MATCH_IN_JUMPTABLE:
        errMsg
                = "[DL_HANDLE] a bin file doesn't match any entry in jump table!";
        break;

    case S_DL_RESOURCE_MUST_DOWNLOAD_WITH_JUMPTABLE:
        errMsg
                = "[User] Resources and jump table are not selected to download \r\n\
                         together! Please select all resource you wanna download";
        break;

    case S_DL_JUMPTABLE_INCONSISTENT_WITH_SCAT:
        errMsg = "[DL_HANDLE] jump table is different from scatter!";
        break;

    case S_DL_INVALID_JUMPTABLE:
        errMsg
                = "[MAUI] Jump table is not loaded yet!\r\n\
                Because its content is not a correct jump table format.";
        break;


    case S_DL_REGION_ADDR_INCONSISTENT_WITH_RESOURCE_ADDR:
        errMsg
                = "[MAUI] The address of resource binary file in scatter file is \r\n\
                inconsistent with the address in the header of resource binary \r\n\
                file! Please ask for help.";
        break;

    case S_DL_INVALID_BOOTLOADER:
        errMsg
                = "[MAUI] The Boot Loader Image file has invalid information in its header! Please ask for help.";
        break;

    case S_DL_REMOTE_FILE_UNSUPPORTED_BY_BL_AUTOLOAD:
        errMsg = "[User] Boot Loader Image file is in remote server!";
        tipMsg = "Please use a local Boot Loader Image file in your computer.";
        break;

    case S_DLIST_BBCHIP_HW_VER_NOT_MATCHED:
        errMsg = "[H/W] Target baseband chip is inconsistent with bin files!";
        tipMsg
                = "Please select the suitable bin files for the target's baseband chip.";
        break;

    case S_UNSUPPORTED_VER_OF_AUTH_FILE:
        errMsg
                = "[User] The authentication file contains an unsupported version!";
        tipMsg
                = "Please check the FlashTool is the latest version, or ask for help.";
        break;

    case S_DL_SCAT_OPEN_FAIL:
        errMsg = "[User] The scatter file cannot be opened!";
        tipMsg
                = "Please check if your scatter file exists and is not used by other software.";
        break;

    case S_SECURITY_SLA_WRONG_AUTH_FILE:
        errMsg
                = "[User] The authentication file with a custom name is different \r\n\
                from the custom name in the handset! Please ask the owner of this \r\n\
                handset for an authentication file with a correct custom name.";
        break;

    case S_SECURITY_SLA_INVALID_AUTH_FILE:
        errMsg = "[User] An invalid authentication file is used!";
        tipMsg
                = "This error message will occurs with MSP (MTK Secure Platform) code.\r\n\
                Please keep up the MSP code and ask for help.";
        break;

    case S_SECURITY_SF_SECURE_VER_CHECK_FAIL:
        errMsg
                = "[User] The secure version of the secure-MAUI is older than that in the handset!";
        tipMsg
                = "Please choose a secure-MAUI with an equal or newer secure version.";
        break;

    case S_SECURITY_SF_HANDSET_SECURE_CUSTOM_NAME_NOT_MATCH:
        errMsg
                = "[User] The custom name of the secure-MAUI is different from that in the handset!";
        tipMsg = "Please choose a secure-MAUI with the same custom name.";
        break;

    case S_SECURITY_SF_FTCFG_LOCKDOWN:
        errMsg = "[User] The Tool configuration is locked down!";
        break;

    case S_SECURITY_SF_CODE_DOWNLOAD_FORBIDDEN:
        errMsg = "[User] To download code area is forbidden in this phone!";
        break;

    case S_SECURITY_SF_CODE_READBACK_FORBIDDEN:
        errMsg = "[User] To read-back code area is forbidden in this phone!";
        break;

    case S_SECURITY_SF_CODE_FORMAT_FORBIDDEN:
        errMsg = "[User] To format code area is forbidden in this phone!";
        break;

    case S_SECURITY_SF_SEC_RO_DOWNLOAD_FORBIDDEN:
        errMsg = "[User] To download SEC_RO is forbidden in this phone!";
        break;

    case S_SECURITY_SF_SEC_RO_READBACK_FORBIDDEN:
        errMsg = "[User] To read-back SEC_RO is forbidden in this phone!";
        break;

    case S_SECURITY_SF_SEC_RO_FORMAT_FORBIDDEN:
        errMsg = "[User] To format SEC_RO is forbidden in this phone!";
        break;

    case S_SECURITY_SF_FAT_DOWNLOAD_FORBIDDEN:
        errMsg = "[User] To download FAT area is forbidden in this phone!";
        break;

    case S_SECURITY_SF_FAT_READBACK_FORBIDDEN:
        errMsg = "[User] To read-back FAT area is forbidden in this phone!";
        break;

    case S_SECURITY_SF_FAT_FORMAT_FORBIDDEN:
        errMsg = "[User] To format FAT area is forbidden in this phone!";
        break;

    case S_SECURITY_SF_RESTRICTED_AREA_ACCESS_FORBIDDEN:
        errMsg
                = "[User] To access restricted area (SEC_RO area and Flash Tool \r\n\
                configuration area in the handset) is forbidden in this phone!";
        break;

    case S_SECURITY_SECURE_CUSTOM_NAME_NOT_MATCH_BETWEEN_AUTH_AND_DL_HANDLE:
        errMsg
                = "[User] The custom name in the authentication file is different from that in secure-MAUI!";
        tipMsg
                = "Please choose the same custom name between authentication file and secure-MAUI!";
        break;

    case S_SECURITY_DOWNLOAD_FILE_IS_CORRUPTED:
        errMsg
                = "[MAUI] Some information about security in secure-MAUI file is corrupted or broken! Please ask for help.";
        break;

    case S_PART_NO_VALID_TABLE:
        errMsg = "No valid partition table for NAND Flash!";
        tipMsg = "Please make sure there is a correct load in the target.\n"
            "\"Format->Download\" could not be executed without target layout table!\n"
            "Please \"Download\" ONLY first to create layout table on the target!";
        break;

    case S_PART_NO_SPACE_FOUND:
        errMsg = "No space found for partition table!";
        tipMsg = "Please make sure there is a correct load in the target.";
        break;

    case S_DA_UPDATE_BOOTLOADER_FILE_SIZE_EXCEEDS_BOUNDARY_ADDR:
        errMsg = "DA download bootloader exceeds partition boundary in this device!";
        tipMsg = "Possible reason is that there are too many bad blocks in bootloader reserved area.\n[Solution]:\nPlease try to Format All first and then download again.\n";
        break;
    case S_SECURITY_DAA_FAIL:
        errMsg = "DA Security download error!";
        tipMsg = "Possible reason is that security DA is not signed by a correct key.";
        break;

    //2011.08.11 add for DSP_BL check
#if 0
    case S_DA_DSPBL_CHECK_PLATFORM_FAILED:
        errMsg = "DA DSP_BL check platform error.";
        tipMsg = "Target platform is not consistent with DSP_BL, Please check the DSP_BL is correct or not.";
        break;
#endif

    case S_DA_UPDATE_BOOTLOADER_EXIST_MAGIC_NOT_MATCHED:
        errMsg = "DA DSP_BL check exist error.";
        tipMsg = "There is no DSP_BL in previous load on target, please use full set erase and download again.";
        break;

    case STATUS_DRAM_TEST_FAILED:
        errMsg = "[H/W] DA detect DRAM Flip test fail";
        tipMsg = "Maybe joint or DRAM HW issue. Please check.";
        break;

    default:
        break;
    }

    return (prefix + "\n\n" + errMsg + "\n[HINT]:\n" + tipMsg);
}

string ErrorLookup::FlashToolErrorMessage(FLASH_TOOL_RESULT ret) {
    string prefix, errMsg, tipMsg;

    if (ret == -1) {
            prefix.append("SP FLASH TOOL ERROR MESSAGE : ( UI )\n\n");
    } else {
            char buf[64];
            sprintf(buf,"SP FLASH TOOL ERROR: (%d)", ret);
            prefix.append(buf);
    }

    switch (ret) {
        case FT_EXCEPTION:
            errMsg = "[SP Flash Tool] Common Error!";
            tipMsg = "SP Flash Tool common error.";
            break;

        case FT_FIND_USB_ERROR:
            errMsg = "[Android USB] Can not find USB port !";
            tipMsg = "\n1. Lack of battery power for your target.\n"
                    "2. Hardware problem of your cable.\n"
                    "3. There is a leakage of usb hub if you are using a usb hub.\n"
                    "4. You do not install usb driver for Android download yet.\n"
                    "Please check above items or catch SP Flash Tool logs and UART4 log to find help.";
            break;

        case FT_FIND_GADGET_USB_ERROR:
            errMsg = "[Android USB] Can not find Gadget Serial USB port !";
            tipMsg = "\n1. Lack of battery power for your target.\n"
                    "2. Hardware problem of your cable.\n"
                    "3. There is a leakage of usb hub if you are using a usb hub.\n"
                    "4. You do not install Gadget Serial USB driver for USB META yet.\n"
                    "Please check above items or catch UART4 log to find help.";
            break;

        case FT_DL_PLATFORM_ERROR:
            errMsg = "[Platform Error] The load and target are inconsistent!\n";
            tipMsg = "Please select a correct load according to the target to download.";
            break;

        case FT_UPGRADE_FOLDER_OVERLAPPED:
            errMsg = "[Android Firmware Upgrade] Backup folder will be overlapped!";
            tipMsg = "The Backup destination folder exists already!"
                    "Please move existing data of this folder to another place,"
                    "then delete this folder before executing backup!";
            break;

        case FT_UPGRADE_FOLDER_NOT_EXSIT:
            errMsg = "[Android Firmware Upgrade] The restore folder does not exist!";
            tipMsg = "The restore folder is not found under \" Tool folder\\BackupNvram \" "
                    "Please move the nvram data folder under the location above, and try again.";
            break;

        case FT_UPGRADE_DIR_CREATE_FAIL:
            errMsg = "[Android Firmware Upgrade] Create backup directory fails!";
            tipMsg = "The backcup folder could not be created under \" Tool folder\\BackupNvram \" ";
            break;

        case FT_UPGRADE_FILE_NOT_EXSIT:
            errMsg = "[Android Firmware Upgrade] The restore file does not exist!";
            tipMsg = "The nvram calibration file to be restored is not found under \" Tool folder\\BackupNvram \\chip id \" \n"
                    "Please move the nvram calibration data under the location above, and try again.";
            break;

        case FT_UPGRADE_BACKUP_ROM_NOT_EXSIT:
            errMsg = "[Android Firmware Upgrade] The restore rom image does not exist!";
            tipMsg = "The restore rom image is not found in scatter file!\n"
                    "Please make sure the content of scatter file is not modified!";
            break;

        case  FT_FIND_NVRAM_ERROR:
            errMsg = "Can't find NVRAM part info on target side.";
            break;

        case FT_INVALID_FORMAT_ADDR_ERROR:
            errMsg = "Invalid format address";
            break;

        case FT_FIND_PLATFORM_ERROR:
            errMsg = "platform.xml does not exist!";
            break;

        case FT_FIND_STORAGE_ERROR:
            errMsg = "storage_setting.xml does not exist!";
            break;

        case FT_FIND_IMAGEMAP_ERROR:
            errMsg = "image_map.xml does not exist!";
            break;

        case FT_INVALIDE_SESSION_KEY:
            errMsg = "Invalid Session Key!";
            break;

        case FT_INVALID_MEM_INFO:
            errMsg = "Invalid flash memory!";
            break;

        case FT_FLASHTOOL_INVALIDE_SCATTER:
            errMsg = "The load scatter file is invalid!\n";
            tipMsg = "Please check the scatter file and select again!\n";
            break;
/*
        case ERROR_BL_INCONSISTENT:
            errMsg = "Bootloaders must be selected/deselected together.";
            break;

        case ERROR_ROM_MUST_ENABLE:
            errMsg = "The ROM must be selected.";
            break;

        case ERROR_PMT_UNAVAILABLE:
            errMsg = "Failed to get PMT info.";
            break;

        case ERROR_DATA_LOST:
            errMsg = "Data will be lost.";
            break;

        case ERROR_UNKNOWN_ROM:
            errMsg = "Unrecognized ROM detected.";
            break;

        case ERROR_ILLEGAL_ADDRESS:
            errMsg = "Invalid ROM or PMT address.";
            break;

        case ERROR_FILE_NOT_EXIST:
            errMsg = "Backup file does not exist.";
            break;


        case ERROR_CHKSUM_FAIL:
            errMsg = "Checksum failed.";
            break;
*/
        case FT_SAVE_FILE_ERROR:
            errMsg = "Save data to file failed.";
            tipMsg = "Please help to check if OTP file is ok.";
            break;

        case FT_RPMB_WRITTEN_BEFORE_DL:
            errMsg = "RPMB partition has been written before download";
            tipMsg = "Please check it.";
        default:
            break;
    }

    return (prefix + "\n\n" + errMsg + "\n[HINT]:\n" + tipMsg);
}

std::string ErrorLookup::FTStatusToString(FLASH_TOOL_RESULT err_code)
{
    std::string errMsg;

    switch(err_code)
    {
    ENUM_TO_STRING(FT_EXCEPTION)
    ENUM_TO_STRING(FT_FIND_USB_ERROR)
    ENUM_TO_STRING(FT_FIND_GADGET_USB_ERROR)
    ENUM_TO_STRING(FT_DL_PLATFORM_ERROR)
    ENUM_TO_STRING(FT_UPGRADE_FOLDER_OVERLAPPED)
    ENUM_TO_STRING(FT_UPGRADE_FOLDER_NOT_EXSIT)
    ENUM_TO_STRING(FT_UPGRADE_DIR_CREATE_FAIL)
    ENUM_TO_STRING(FT_UPGRADE_FILE_NOT_EXSIT)
    ENUM_TO_STRING(FT_UPGRADE_BACKUP_ROM_NOT_EXSIT)
    ENUM_TO_STRING(FT_FIND_NVRAM_ERROR)
    ENUM_TO_STRING(FT_INVALID_FORMAT_ADDR_ERROR)
    ENUM_TO_STRING(FT_RESTORE_ERROR)
    ENUM_TO_STRING(FT_FIND_PLATFORM_ERROR)
    ENUM_TO_STRING(FT_FIND_STORAGE_ERROR)
    ENUM_TO_STRING(FT_FIND_IMAGEMAP_ERROR)
    ENUM_TO_STRING(FT_INVALIDE_SESSION_KEY)
    /*
    ENUM_TO_STRING(ERROR_BL_INCONSISTENT)
    ENUM_TO_STRING(ERROR_ROM_MUST_ENABLE)
    ENUM_TO_STRING(ERROR_PMT_UNAVAILABLE)
    ENUM_TO_STRING(ERROR_DATA_LOST)
    ENUM_TO_STRING(ERROR_UNKNOWN_ROM)
    ENUM_TO_STRING(ERROR_ILLEGAL_ADDRESS)
    ENUM_TO_STRING(ERROR_FILE_NOT_EXIST)
    ENUM_TO_STRING(ERROR_CHKSUM_FAIL)
    */
    default:
        break;
    }

    return errMsg;
}
