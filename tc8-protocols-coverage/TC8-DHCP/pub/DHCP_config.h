#ifndef DHCP_CONFIG_H
#define DHCP_CONFIG_H

#include <stdint.h>

typedef struct{
    uint32_t ParamLeaseTime;
    uint8_t ParamToleranceTime;
    uint8_t ParamProcessTime;
    uint8_t ParamListenTime;
    uint8_t ParamFirstRetransmissionInterval;
    uint8_t DHCP_MAGIC_COOKIE[4];
    uint8_t  IP_BROADCAST_ADDRESS[16];
    uint8_t  SERVERn_IP_ADDRESS[16];
    uint8_t  SERVERn_IP_POOL_L_M[16];
    uint8_t  SERVERn_IP_POOL_NETMASK[16];
    uint8_t  MAC_UNUSED_ADDRESS[18];
    uint8_t  IP_UNUSED_ADDRESS[16];
    uint8_t  IP_UNUSED_NET_MASK[16];
    uint16_t HIGH_LEASE_TIME;
    uint16_t VERY_HIGH_LEASE_TIME;
    uint8_t REMOTE_CLIENTn_LEASE_TIME;
    uint8_t REMOTE_CLIENTn_T1;
    uint8_t REMOTE_CLIENTn_T2;
    uint32_t extractedXID;
    uint16_t extractedSeconds;
    uint8_t SERVER2_IP_ADDRESS[16];
    uint8_t extractedSrcHwAddr[18];
    uint8_t DIface_0[20];
    uint16_t PORT_UDP_SRC;
    uint16_t PORT_UDP_DST;

}Dhcp_config_t;


/***************************************************************************************************
*
*   FUNCTION NAME: set_DHCP_config
*
***************************************************************************************************/
/**
* @brief
* This function will configure the parameters used in the tests.
*
* @parameters
* @param[in] Dhcp_config_t : a parameter to configure for DHCP
*
* @return
* -
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void set_DHCP_config(Dhcp_config_t param_list);


#endif