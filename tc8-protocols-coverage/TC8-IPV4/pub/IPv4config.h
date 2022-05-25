#ifndef IPV4CONFIG_H
#define IPV4CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif  // #ifdef __cplusplus

#define LOOPBACK_ADDRESS                        "127.0.0.1"         /* The internal host loopback address */
#define IP_VERSION_4                            0x04                /* Indicates that IPv4 is used in the current message */
#include<stdint.h>
typedef struct
{
    uint8_t DUT_IP[16];                                                /* IP address of the DUT */
    uint8_t TESTER_IP[16];                                             /* IP address of the Tester */
    uint16_t DUT_PORT;                                          /* Port of the DUT */
    uint16_t TESTER_PORT;                                       /* Port of the Tester */
    uint16_t INVALID_CHECKSUM;                                  /* This is the checksum which is different from the calculated checksum */
    uint16_t TTL;                                               /* Time To Live field value which must be less than 2 */
    uint16_t LARGE_TTL_VALUE;                                   /* Time To Live value in a fragmented packet. 
                                                                       NOTE: This value is greater than the initial reassemble timer setting <ipIniReassembleTimeout> */
    uint16_t LOW_TTL_VALUE;                                     /* Time To Live value in a fragmented packet. 
                                                                       NOTE: This value is less than the initial reassemble timer setting <ipIniReassembleTimeout> */
    uint8_t DIRECTED_BROADCAST_ADDRESS[16];                            /* The directed broadcast address addresses a specific group in the network. 
                                                                       {<Network-number>, -1}, e.g. 192.168.255.255 */
    uint8_t LIMITED_BROADCAST_ADDRESS[16];                             /* The limited broadcast address addresses every host on the connected physical network. 
                                                                       {-1, -1} -> 255.255.255.255
                                                                       NOTE: It is not applicable for class D and class E */
    uint16_t LISTEN_TIME;                                       /* This is the maximum time interval for which TESTER waits for an ICMP Reply packet.
                                                                       This defaults to 3 seconds unless DUT configuration specifies otherwise. */
    uint16_t IP_INI_REASSEMBLE_TIMEOUT;                         /* Default value: depends on used controller. Time to wait for cleaning of the buffer
                                                                       with fragmented and malfored messages. */
    uint16_t FRAGMENT_REASSEMBLY_TIMEOUT;                       /* The fragment reassembly timeout. This defaults to 15 seconds */
    uint16_t IP_VERSION;                                        /* Indicates the version used in the current message */
    uint16_t IP_TYPE_ICMP;                                      /* Indicates the following protocol type, in this case ICMP */
    uint16_t IP_TYPE_TCP;                                       /* Indicates the following protocol type, in this case TCP */
    uint16_t MTU;                                               /* The Maximum Transmission Unit size */
} IPv4_config_t;

extern IPv4_config_t IPv4Config;

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
void Set_IPv4_Config(IPv4_config_t conf);

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
IPv4_config_t Get_IPv4_Config();

#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus
#endif
