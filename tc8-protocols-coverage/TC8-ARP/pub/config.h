/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   parameetr_configuration.h
 *  @brief 
 *  \author Abderrahim Jamaoui
 *
 *  \addtogroup TC8-ARP
 *  \{
 */

#ifndef PARA_CONF
#define PARA_CONF

#include <stdint.h> /* uint8_t */
#include <string.h>


typedef struct 
{   
    uint16_t DYNAMIC_ARP_CACHETIMEOUT;
    uint8_t HOST_1_IP[16];
    uint8_t DIface_0_IP[16];
    uint8_t ParamListenTime;
    uint8_t MAC_ADDR1[18];
    uint8_t MAC_ADDR2[18];
    uint8_t MAC_ADDR3[18];
    uint8_t DIFACE_O_MAC_ADDR[18];
    uint8_t ARBIT_MAC_ADDR[18];
    uint16_t ARP_HARDWARE_TYPE_UNKNOWN;
    uint8_t UNKNOWN_HW_ADDR_LEN;
    uint16_t ARP_PROTOCOL_UNKNOWN;
    uint8_t UNKNOWN_PROTCOL_ADDR_LEN;
    uint8_t ARP_TOLERANCE_TIME;
    uint8_t ETHERNET_ADDR_LEN;
    uint16_t ARP_HARDWARE_ETHERNET;
    uint8_t IP_ADDR_LEN;
    uint16_t ARP_PROTOCOL_IP;
    uint8_t OPERATION_REQUEST;
    uint8_t all_zeroes[18];
    uint8_t ETHERNET_BROADCAST_ADDR[18];
    uint8_t IP_FIRST_UNUSED_ADDR_INTERFACE_1[16];
    uint8_t OPERATION_RESPONSE;
}Arp_config_t;
 

void init_ARP_config();
int set_ARP_config(Arp_config_t param_list);
Arp_config_t get_ARP_config();

#endif // PARA_CONF
