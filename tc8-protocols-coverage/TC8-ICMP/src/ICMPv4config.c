#include "ICMPv4config.h"
#include <string.h>

ICMPv4_config_t ICMPv4Config;

/***************************************************************************************************
*
*   FUNCTION NAME: Set_ICMP_Config
*
***************************************************************************************************/
/**
* @brief
*  This function will configure ICMPv4 protocol.
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
void Set_ICMPv4_Config(ICMPv4_config_t conf)
{
    //set ICMPv4 configutation values
    strcpy((char*)ICMPv4Config.DUT_IP, (char*)conf.DUT_IP);  
    strcpy((char*)ICMPv4Config.TESTER_IP, (char*)conf.TESTER_IP);
    ICMPv4Config.ICMP_IDENTIFIER = conf.ICMP_IDENTIFIER;
    ICMPv4Config.ICMP_SEQUENCE_NUMBER = conf.ICMP_SEQUENCE_NUMBER;
    ICMPv4Config.INVALID_CHECKSUM = conf.INVALID_CHECKSUM;
    ICMPv4Config.FRAGMENT_REASSEMBLY_TIMEOUT = conf.FRAGMENT_REASSEMBLY_TIMEOUT;
    ICMPv4Config.LISTEN_TIME = conf.LISTEN_TIME;
    ICMPv4Config.UNSUPPORTED_PROTOCOL = conf.UNSUPPORTED_PROTOCOL;
    ICMPv4Config.INVALID_ICMP_TYPE = conf.INVALID_ICMP_TYPE;
    strcpy((char*)ICMPv4Config.BROADCAST_ADDRESS, (char*)conf.BROADCAST_ADDRESS);
    ICMPv4Config.ORINGIN_TIMESTAMP_VALUE = conf.ORINGIN_TIMESTAMP_VALUE;
}

/***************************************************************************************************
*
*   FUNCTION NAME: Get_ICMPv4_Config
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
ICMPv4_config_t Get_ICMPv4_Config()
{
    //return ICMPv4 configutation values
    ICMPv4_config_t conf;
    strcpy((char*)conf.DUT_IP, (char*)ICMPv4Config.DUT_IP);  
    strcpy((char*)conf.TESTER_IP, (char*)ICMPv4Config.TESTER_IP);
    conf.ICMP_IDENTIFIER = ICMPv4Config.ICMP_IDENTIFIER;
    conf.ICMP_SEQUENCE_NUMBER = ICMPv4Config.ICMP_SEQUENCE_NUMBER;
    conf.INVALID_CHECKSUM = ICMPv4Config.INVALID_CHECKSUM;
    conf.FRAGMENT_REASSEMBLY_TIMEOUT = ICMPv4Config.FRAGMENT_REASSEMBLY_TIMEOUT;
    conf.LISTEN_TIME = ICMPv4Config.LISTEN_TIME;
    conf.UNSUPPORTED_PROTOCOL = ICMPv4Config.UNSUPPORTED_PROTOCOL;
    conf.INVALID_ICMP_TYPE = ICMPv4Config.INVALID_ICMP_TYPE;
    strcpy((char*)conf.BROADCAST_ADDRESS, (char*)ICMPv4Config.BROADCAST_ADDRESS);
    conf.ORINGIN_TIMESTAMP_VALUE = ICMPv4Config.ORINGIN_TIMESTAMP_VALUE;
    return conf;
}
