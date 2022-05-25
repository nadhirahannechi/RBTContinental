#include "IPv4config.h"
#include <string.h>

IPv4_config_t IPv4Config;

/**************************************************************************************************/
/*                                                                                                */
/*                                         Prototypes                                             */
/*                                                                                                */
/**************************************************************************************************/

/***************************************************************************************************
*
*   FUNCTION NAME: Set_IP_Config
*
***************************************************************************************************/
/**
* @brief
*  This function will configure IPv4 protocol.
*  
* @parameters
* @param[in] conf: The new configuration. 
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
void Set_IPv4_Config(IPv4_config_t conf)
{
    //set IPv4 configutation values
    strcpy((char*)IPv4Config.DUT_IP, (char*)conf.DUT_IP);  
    strcpy((char*)IPv4Config.TESTER_IP, (char*)conf.TESTER_IP);
    IPv4Config.DUT_PORT = conf.DUT_PORT;
    IPv4Config.TESTER_PORT = conf.TESTER_PORT;
    IPv4Config.INVALID_CHECKSUM = conf.INVALID_CHECKSUM;
    IPv4Config.TTL = conf.TTL;
    IPv4Config.LARGE_TTL_VALUE = conf.LARGE_TTL_VALUE;
    IPv4Config.LOW_TTL_VALUE = conf.LOW_TTL_VALUE;
    strcpy((char*)IPv4Config.DIRECTED_BROADCAST_ADDRESS, (char*)conf.DIRECTED_BROADCAST_ADDRESS);
    strcpy((char*)IPv4Config.LIMITED_BROADCAST_ADDRESS, (char*)conf.LIMITED_BROADCAST_ADDRESS);
    IPv4Config.LISTEN_TIME = conf.LISTEN_TIME;
    IPv4Config.IP_INI_REASSEMBLE_TIMEOUT = conf.IP_INI_REASSEMBLE_TIMEOUT;
    IPv4Config.FRAGMENT_REASSEMBLY_TIMEOUT = conf.FRAGMENT_REASSEMBLY_TIMEOUT;
    IPv4Config.IP_VERSION = conf.IP_VERSION;
    IPv4Config.IP_TYPE_ICMP = conf.IP_TYPE_ICMP;
    IPv4Config.IP_TYPE_TCP = conf.IP_TYPE_TCP;
    IPv4Config.MTU = conf.MTU;
}

/***************************************************************************************************
*
*   FUNCTION NAME: Get_#include "ICMPv4config.h"
#include <string.h>
_Config
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
IPv4_config_t Get_IPv4_Config()
{
    //return IPv4 configutation values
    IPv4_config_t conf;
    strcpy((char*)conf.DUT_IP, (char*)IPv4Config.DUT_IP);  
    strcpy((char*)conf.TESTER_IP, (char*)IPv4Config.TESTER_IP);
    conf.DUT_PORT = IPv4Config.DUT_PORT;
    conf.TESTER_PORT = IPv4Config.TESTER_PORT;
    conf.INVALID_CHECKSUM = IPv4Config.INVALID_CHECKSUM;
    conf.TTL = IPv4Config.TTL;
    conf.LARGE_TTL_VALUE = IPv4Config.LARGE_TTL_VALUE;
    conf.LOW_TTL_VALUE = IPv4Config.LOW_TTL_VALUE;
    strcpy((char*)conf.DIRECTED_BROADCAST_ADDRESS, (char*)IPv4Config.DIRECTED_BROADCAST_ADDRESS);
    strcpy((char*)conf.LIMITED_BROADCAST_ADDRESS, (char*)IPv4Config.LIMITED_BROADCAST_ADDRESS);
    conf.LISTEN_TIME = IPv4Config.LISTEN_TIME;
    conf.IP_INI_REASSEMBLE_TIMEOUT = IPv4Config.IP_INI_REASSEMBLE_TIMEOUT;
    conf.FRAGMENT_REASSEMBLY_TIMEOUT = IPv4Config.FRAGMENT_REASSEMBLY_TIMEOUT;
    conf.IP_VERSION = IPv4Config.IP_VERSION;
    conf.IP_TYPE_ICMP = IPv4Config.IP_TYPE_ICMP;
    conf.IP_TYPE_TCP = IPv4Config.IP_TYPE_TCP;
    conf.MTU = conf.MTU;
}
