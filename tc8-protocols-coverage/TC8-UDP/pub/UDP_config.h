#include "TestabilityProtocol_Intern.h"

/*==================================================================================================
*                                             TYPES
==================================================================================================*/

typedef struct 
{   
    uint16 testerUDPPort;
    uint16 testerUDPPort2;
    uint16 unusedUDPDstPort1;
    uint16 unusedUDPSrcPort;
    uint16 DUTUDPPort;
    
    uint16 udpUserDataSize;
    uint16 calculatedUDPChecksum;
    uint16 incorrectUDPChecksum;
    uint8 UDPDefaultData[1024];
    uint16 UDPDefaultDataLen;
    uint8 UDPData[1024];
    uint16 UDPDataLen;
    
    uint8 Dst_IP_addr[16];
    uint8 Host_1_IP[16];
    uint8 Host_2_IP[16];
    uint8 AIface_0_BcastIP[16];
    uint8 allSystemMCastAddr[16];
    
} UDP_config_t;


/***************************************************************************************************
*
*   FUNCTION NAME: Set_UDP_Config
*
***************************************************************************************************/
/**
* @brief
*  This function will configure the global values.
*  
* @parameters
* @param[in] configuration: The new configuration. 
*
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
    void Set_UDP_Config(UDP_config_t configuration);

/***************************************************************************************************
*
*   FUNCTION NAME: Get_UDP_Config
*
***************************************************************************************************/
/**
* @brief
*  This function will return the configuration values.
*  
* @parameters
* @param[in] void 
*
* @return Current configutation
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
    UDP_config_t Get_UDP_Config(void);
