#ifndef _EXTERNAL_MEMORY_CONFIG_H
#define _EXTERNAL_MEMORY_CONFIG_H


typedef enum
{
    ExternalMemoryType_Nonexistent = 0,
    ExternalMemoryType_PSRAM,
    ExternalMemoryType_DDR,
    ExternalMemoryType_DDR2
} ExternalMemoryType;


typedef struct
{
    unsigned int EMI_CONI_Value;
    unsigned int EMI_DRVA_Value;
    unsigned int EMI_DRVB_Value;
    unsigned int EMI_CONJ_Value;
    unsigned int EMI_CONK_Value;
    unsigned int EMI_CONL_Value;
    unsigned int EMI_IOCL_Value;
    unsigned int EMI_GENA_Value;
    unsigned int EMI_GEND_Value;
    unsigned int EMI_DRCT_Value;
    unsigned int EMI_PPCT_Value;
    unsigned int EMI_SLCT_Value;
    unsigned int EMI_ABCT_Value;
    unsigned int EMI_DUTB_Value;
    
} DDR_Config_v1;


typedef struct
{
    unsigned int EMI_CONI_Value;
    unsigned int EMI_CONJ_Value;
    unsigned int EMI_CONK_Value;
    unsigned int EMI_CONL_Value;
    unsigned int EMI_CONN_Value;

    unsigned int EMI_GENA_Value;
    unsigned int EMI_GEND_Value;
    
    unsigned int EMI_DRCT_Value;
    unsigned int EMI_PPCT_Value;
    unsigned int EMI_ABCT_Value;

    unsigned int EMI_DQSA_Value;
    unsigned int EMI_DQSB_Value;
} DDR2_Config_v1;


typedef struct
{
    ExternalMemoryType memoryType;
    unsigned int initFlowVersion;

    union
    {
        DDR_Config_v1 DDR_v1;
        DDR2_Config_v1 DDR2_v1;
    } config;

} ExternalMemoryConfig;

#endif // _EXTERNAL_MEMORY_CONFIG_H
