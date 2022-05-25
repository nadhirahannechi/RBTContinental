#ifndef ICMPV4CONFIG_H
#define ICMPV4CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif  // #ifdef __cplusplus

#include "stdint.h"
typedef struct
{
    uint8_t DUT_IP[16];                                            /* IP address of the DUT */
    uint8_t TESTER_IP[16];                                         /* IP address of the Tester */
    uint16_t ICMP_IDENTIFIER;                               /* Identifier used in the ICMP Messages to identify an ICMP Message */
    uint16_t ICMP_SEQUENCE_NUMBER;                          /* Sequence Numbers used in ICMP Messages to identify an ICMP Message */
    uint16_t INVALID_CHECKSUM;                              /* This is the checksum which is different from the calculated checksum */
    uint8_t FRAGMENT_REASSEMBLY_TIMEOUT;                   /* The fragment reassembly timeout. This defaults to 15 seconds. */
    uint8_t LISTEN_TIME;                                   /* This is the maximum time interval for which TESTER waits for an ICMP Reply packet.
                                                                   NOTE: This defaults to 3 seconds unless DUT configuration specifies otherwise. */
    uint8_t UNSUPPORTED_PROTOCOL;                          /* This is an IP protocol number that is not supported by the DUT. */
    uint8_t INVALID_ICMP_TYPE;                             /* This defines an invalid ICMP Type */
    uint8_t BROADCAST_ADDRESS[16];                                 /* IP broadcast address */
    uint32_t ORINGIN_TIMESTAMP_VALUE;                           /* Time the sender last touched a message before sending it. It is 32 bits of 
                                                                   milliseconds since midnight UT */
}ICMPv4_config_t;

extern ICMPv4_config_t ICMPv4Config;

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
void Set_ICMPv4_Config(ICMPv4_config_t conf);

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
ICMPv4_config_t Get_ICMPv4_Config();

#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus
#endif
