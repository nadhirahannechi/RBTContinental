#include "DHCP_config.h"
#include <string.h>

Dhcp_config_t DHCP_config;

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
* @return void  0 if Done with success
*               1 if set_DHCP_config failed
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void set_DHCP_config(Dhcp_config_t param_list)
{
     DHCP_config.ParamLeaseTime = param_list.ParamLeaseTime;
     DHCP_config.ParamToleranceTime = param_list.ParamToleranceTime;
     DHCP_config.ParamProcessTime = param_list.ParamProcessTime;
     DHCP_config.ParamListenTime = param_list.ParamListenTime;
     DHCP_config.ParamFirstRetransmissionInterval = param_list.ParamFirstRetransmissionInterval;
     strcpy((char*)DHCP_config.DHCP_MAGIC_COOKIE, (char*)param_list.DHCP_MAGIC_COOKIE);
     strcpy((char*)DHCP_config.IP_BROADCAST_ADDRESS, (char*)param_list.IP_BROADCAST_ADDRESS);
     strcpy((char*)DHCP_config.SERVERn_IP_ADDRESS, (char*)param_list.SERVERn_IP_ADDRESS);
     strcpy((char*)DHCP_config.SERVERn_IP_POOL_L_M, (char*)param_list.SERVERn_IP_POOL_L_M);
     strcpy((char*)DHCP_config.SERVERn_IP_POOL_NETMASK, (char*)param_list.SERVERn_IP_POOL_NETMASK);
     strcpy((char*)DHCP_config.MAC_UNUSED_ADDRESS, (char*)param_list.MAC_UNUSED_ADDRESS);
     strcpy((char*)DHCP_config.IP_UNUSED_ADDRESS, (char*)param_list.IP_UNUSED_ADDRESS);
     strcpy((char*)DHCP_config.IP_UNUSED_NET_MASK, (char*)param_list.IP_UNUSED_NET_MASK);
     DHCP_config.HIGH_LEASE_TIME = param_list.HIGH_LEASE_TIME;
     DHCP_config.VERY_HIGH_LEASE_TIME = param_list.VERY_HIGH_LEASE_TIME;
     DHCP_config.REMOTE_CLIENTn_LEASE_TIME = param_list.REMOTE_CLIENTn_LEASE_TIME;
     DHCP_config.REMOTE_CLIENTn_T1 = param_list.REMOTE_CLIENTn_T1;
     DHCP_config.REMOTE_CLIENTn_T2 = param_list.REMOTE_CLIENTn_T2;
     DHCP_config.extractedXID = param_list.extractedXID;
     DHCP_config.extractedSeconds = param_list.extractedSeconds;
     strcpy((char*)DHCP_config.SERVER2_IP_ADDRESS, (char*)param_list.SERVER2_IP_ADDRESS);
     strcpy((char*)DHCP_config.extractedSrcHwAddr, (char*)param_list.extractedSrcHwAddr);
     strcpy((char*)DHCP_config.DIface_0, (char*)param_list.DIface_0);
}