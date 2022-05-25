/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   common.h
 *  @brief  Internally used in abstration network api
 *  \author Aymen SALEM - Abderrahim JAMAOUI
 *
 *  \addtogroup SW_package
 *  \{
 */

#ifndef COMMON_H
#define COMMON_H

/*==================================================================================================
*                                        Include Files
==================================================================================================*/

#include<string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <cstdio>


/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

// Maximum transmission unit
#define MAX_SIZE_IP_PACKET 65535
#define MAX_SIZE_DATA MAX_SIZE_IP_PACKET - 20 - 8
// Log functions
#define Net_Abst_API_TRACE_DEBUG( format, ... )   printf( "\n Net_Abst_API DEBUG: %s(%d): " format, __FUNCTION__, __LINE__, ##__VA_ARGS__ )
#define Net_Abst_API_INFO( format, ... )    printf( "\n Net_Abst_API INFO: %s(%d): " format, __FUNCTION__, __LINE__, ##__VA_ARGS__ )
#define Net_Abst_API_WARNING( format, ... ) printf( "\n Net_Abst_API WARNING: %s(%d): " format, __FUNCTION__, __LINE__, ##__VA_ARGS__ )
#define Net_Abst_API_ERROR( format, ... )   printf( "\n Net_Abst_API ERROR: %s(%d): " format, __FUNCTION__, __LINE__, ##__VA_ARGS__ )

// Protocol layer code 
#define ETH_LAYER_Code              143     /* Ethernet layer code		   */
#define IP_LAYER_Code               4       /* IP layer code			   */
#define ICMP_LAYER_Code             1       /* ICMP layer code			   */
#define UDP_LAYER_Code              17      /* UDP layer code			   */
#define TCP_LAYER_Code              6       /* TCP layer code			   */
#define PAYLOAD_LAYER_Code          108     /* Payload layer code		   */
#define ARP_LAYER_Code              92      /* ARP layer code			   */
#define DHCP_LAYER_Code             300     /* DHCP layer code			   */
#define TCP_OPTIONS_LAYER_Code      400     /* TCP OPTIONS layer code      */
#define IP_OPTION_LAYER_Code        500     /* IP OPTIONS layer code       */
#define ICMP_TIME_STAMP_LAYER_CODE  600     /* ICMP TIME STAMP LAYER CODE  */

// Protocol over ethernet
#define ETH_IP  0x0800
#define ETH_ARP 0x0806

// icmp message type
#define ICMP_ECHOREPLY		0	/* Echo Reply			*/
#define ICMP_DEST_UNREACH	3	/* Destination Unreachable	*/
#define ICMP_SOURCE_QUENCH	4	/* Source Quench		*/
#define ICMP_REDIRECT		5	/* Redirect (change route)	*/
#define ICMP_ECHO_REQUEST	8	/* Echo Request			*/
#define ICMP_TIME_EXCEEDED	11	/* Time Exceeded		*/
#define ICMP_PARAMETERPROB	12	/* Parameter Problem		*/
#define ICMP_TIMESTAMP		13	/* Timestamp Request		*/
#define ICMP_TIMESTAMPREPLY	14	/* Timestamp Reply		*/
#define ICMP_INFO_REQUEST	15	/* Information Request		*/
#define ICMP_INFO_REPLY		16	/* Information Reply		*/
#define ICMP_ADDRESS		17	/* Address Mask Request		*/
#define ICMP_ADDRESSREPLY	18	/* Address Mask Reply		*/
#define NR_ICMP_TYPES		18

// Dhcp constants
#define MAX_DHCP_CHADDR_LENGTH           16
#define MAX_DHCP_SNAME_LENGTH            64
#define MAX_DHCP_FILE_LENGTH             128
#define MAX_DHCP_OPTIONS_LENGTH          312
#define BOOTREQUEST     1
#define BOOTREPLY       2

// DHCP types
#define DHCPDISCOVER    1
#define DHCPOFFER       2
#define DHCPREQUEST     3
#define DHCPDECLINE     4
#define DHCPACK         5
#define DHCPNACK        6
#define DHCPRELEASE     7


// Dhcp port in udp layer
#define ETHERNET_HARDWARE_ADDRESS            1     // used in htype field of dhcp packet 
#define ETHERNET_HARDWARE_ADDRESS_LENGTH     6     // length of Ethernet hardware addresses 

#define DHCP_SERVER_PORT   67
#define DHCP_CLIENT_PORT   68

// Dhcp option indice
#define DHCP_OPTION_MESSAGE_TYPE        53
#define DHCP_OPTION_REQUESTED_ADDRESS   50
#define DHCP_OPTION_LEASE_TIME          51
#define DHCP_OPTION_RENEWAL_TIME        58
#define DHCP_OPTION_REBINDING_TIME      59


// Arp message types
#define	ARP_REQUEST	1		/* ARP request.  */
#define	ARP_REPLY	2		/* ARP reply.  */

/*==================================================================================================
*                                             TYPES
==================================================================================================*/

// Filter strcut
typedef struct{
    int ETHType;
    uint16_t arp_operation;
    char  spa[16];
    char  tpa[16];
    int IPproto;
    char SrcIPAddress[16];
    char DstIPAddress[16];
    int SrcPort;
    int DstPort;
}filter;



/*==================================================================================================
*                                        Global Variables
==================================================================================================*/

/*==================================================================================================
*                                     local Functions prototypes
==================================================================================================*/

/*==================================================================================================
*                                     Global Functions prototypes
==================================================================================================*/
/***************************************************************************************************
*
*   FUNCTION NAME: mac_to_string
*
***************************************************************************************************/
/**
* @brief
*  This function will convert the mac address  from unsigned char* (uint8_t) to string.
*  
* @parameters
* @param[in] m : the mac address in a pointer to unsigned char *
*
* @return String : the mac address.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
std::string mac_to_string(unsigned char * m);

/***************************************************************************************************
*
*   FUNCTION NAME: string_to_mac
*
***************************************************************************************************/
/**
* @brief
*  This function will convert the mac address  from string to unsigned char *.
*  
* @parameters
* @param[in] m : the mac address in string
* @param[in] p : the mac address in a pointer to unsigned char * 
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
void string_to_mac(std::string m, unsigned char p[]);

/***************************************************************************************************
*
*   FUNCTION NAME: string_to_ipadress
*
***************************************************************************************************/
/**
* @brief
*  This function will convert the ip address  from string to uint8_t.
*  
* @parameters
* @param[in] S_IP_ADDR : the ip address in string
* @param[in] Uint8_IP_ADDR : the ip address in a pointer to unsigned char * 
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
void string_to_ipadress(std::string S_IP_ADDR,uint8_t * Uint8_IP_ADDR);

/***************************************************************************************************
*
*   FUNCTION NAME: ipadress_to_string
*
***************************************************************************************************/
/**
* @brief
*  This function will convert the ip address  from unsigned char* (uint8_t) to string.
*  
* @parameters
* @param[in] ipADD: ip address in a pointer to unsigned char *
*
* @return String : the ip address.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
std::string ipadress_to_string(uint8_t * ipADD);

/***************************************************************************************************
*
*   FUNCTION NAME: in_cksum
*
***************************************************************************************************/
/**
* @brief
*  This function will compute the checksum
*  
* @parameters
* @param[in] data:  a pointer to unsigned short
* @param[in] nbytes: the data length
*
* @return unsigned short : the computed checksum.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
unsigned short in_cksum(unsigned short *data, int nbytes);
#endif
