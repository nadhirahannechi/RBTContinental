/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   TestabilityProtocolTypes.h
 *  @brief  AUTOSAR Base - Testability Protocol.
 *  \author uid1145 Mejdi Zayani
 *
 *  \addtogroup Testability_Protocol_package
 *  \{
 */
#ifndef TESTABILITYPROTOCOLTYPES_H
#define TESTABILITYPROTOCOLTYPES_H


/*==================================================================================================
*                                        Include Files
==================================================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <net/route.h>
#include <arpa/inet.h>
#include <linux/rtnetlink.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <netinet/ip_icmp.h>
#include "TestabilityProtocolStdTypes.h"
#ifdef __cplusplus
extern "C"
{
#endif  /* #ifdef __cplusplus*/
/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
#ifndef E_OK
#define E_OK 1
#endif
#ifndef E_NOT_OK
#define E_NOT_OK 0
#endif

#define BSW_TP_CODE

#define TESTABILITY_PROTOCOL_VERSION   0x01U
#define TESTABILITY_PROTOCOL_INTERFACE 0x01U
#define TESTABILITY_PROTOCOL_SERVICE_ID 0x0105U

#define TP_ONE_BYTE_SHIFT 0x08U
#define TP_MESSAGE_SIZE_WITHOUT_PAYLOAD  0x10U

/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/*******************************    Testability Message Format *************************************
*  
*    ------------------------------------------------------------------------------------------------
*   |           0           |           1           |           2           |           3            |
*   |------------------------------------------------------------------------------------------------|
*   | 0  1  2  3  4  5  6  7| 8  9 10 11 12 13 14 15|16 17 18 19 20 21 22 23|24 25 26 27 28 29 30 31 |
*   |------------------------------------------------------------------------------------------------|
*   |                  Service ID                   |E|      Group ID       |        Service         |
*   |                     (SID)                     |V|        (GID)        |      Primitive ID      |     
*   |               default: 0x0105                 |B|                     |         (PID)          |             
*   |------------------------------------------------------------------------------------------------|
*   |                                         Length (LEN)                                           |
*   |------------------------------------------------------------------------------------------------|
*   |                                          DON'T CARE                                            | 
*   |------------------------------------------------------------------------------------------------|
*   |    Protocol Version   |   Interface Version   |        Type ID        |       Result ID        |
*   |         (0x01)        |         (0x01)        |         (TID)         |         (RID)          |
*   |------------------------------------------------------------------------------------------------|
*   |                                       Parameter Data                                           |        
*   |                                        Payload (DAT)                                           |
*    ------------------------------------------------------------------------------------------------
*
*    EVB: Event Bit  (Protocol field that is set to 0x01U in case of an event)
*
**/

/* Message Type ID : TID = 8 Bit
*  Description: Selects request, response or event  
*
**/
typedef enum
{
    TID_REQUEST   = 0x00U,     /* non-blocking function call */
    TID_RESPONSE  = 0x08U,     /* non-blocking function return that is always followed after a request */
    TID_EVENT     = 0x02U      /* callback function call (EVB set to 1) */
} TP_MessageTypeID_t;

/* Result ID: RID = 8 Bit
*  Description: Testability Protocol Results ID 
*
**/
typedef enum
{
    /* Standard Results */
    RID_E_OK      = 0x00U,     /* E_OK */
    RID_E_NOK     = 0x01U,     /* E_NOT_OK */
    /* 0x02 - 0x7F :  Range of AUTOSAR specific error codes (Results 
                    function calls other than E_OK or E_NOT_OK)   */
    /* Testability Specific Results */
    RID_E_NTF     = 0xFFU,     /* The requested service primitive was not found */
    RID_E_PEN     = 0xFEU,     /* The Upper Tester or a service primitive is pending */
    RID_E_ISB     = 0xFDU,     /* Insufficient buffer size */
    RID_E_INV     = 0xFCU,     /* Invalid Input or Parameter */

    /* Service Primitive Specific Results */
    RID_E_ISD     = 0xEFU,     /* Invalid socket ID */
    RID_E_UCS     = 0xEEU,     /* Unable to create socket or no free socket */
    RID_E_UBS     = 0xEDU,     /* Unable to bind socket, port taken */
    RID_E_IIF     = 0xECU,     /* Invalid network or virtual interface */
    RID_E_TCP_PNA = 0xEBU,     /* TCP error: "precedence not allowed" */
    RID_E_TCP_FSU = 0xEAU,     /* TCP error: "foreign socket unspecified” */
    RID_E_TCP_ILP = 0xE9U,     /* TCP error: "connection illegal for this process" */
    RID_E_TCP_INR = 0xE8U,     /* TCP error: "insufficient resources" */
    RID_E_TCP_CAE = 0xE7U,     /* TCP error: "connection already exists" */
    RID_E_TCP_COC = 0xE6U,     /* TCP error: "connection closing" */
    RID_E_TCP_CNE = 0xE5U,     /* TCP error: "connection does not exist" */
    RID_E_TCP_CRE = 0xE4U,     /* TCP error: "connection reset" */
    RID_E_TCP_CAT = 0xE3U,     /* TCP error: "connection aborted due to user timeout" */
    RID_E_TCP_COR = 0xE2U,     /* TCP Error: "connection refused” */
    RID_E_OAP     = 0xE1U      /* TCP Error: "Operation already in progress” */ 

} TP_ResultID_t;

/* Group ID: GID = 7 Bit 
*  Description: Testability Protocol Service Group ID  
*
**/
typedef enum
{
    GID_GENERAL = 0x00U,
    GID_UDP     = 0x01U,
    GID_TCP     = 0x02U,
    GID_ICMPv4  = 0x03U,
    GID_ICMPv6  = 0x04U,
    GID_IPv4    = 0x05U,
    GID_IPv6    = 0x06U,
    GID_DHCPv4  = 0x07U,
    GID_DHCPv6  = 0x08U,
    GID_ARP     = 0x09U,
    GID_NDP     = 0x0AU,
    GID_ETH     = 0x0BU,
    GID_PHY     = 0x0CU,
    GID_CC      = 0x7FU        /* Used to close the control channel betwen DUT <=> TESTER */ 

} TP_GroupID_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: Gnenral : GID = 0x00U
*  Description: Contains general service primitives that must be provided by 
*           the Upper Tester
*
**/
typedef enum
{
    PID_GENERAL_GET_VERSION  = 0x01U,   /* Type: mandatory */
    PID_GENERAL_START_TEST   = 0x02U,   /* Type: mandatory */
    PID_GENERAL_END_TEST     = 0x03U    /* Type: mandatory */
} TP_GeneralPID_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: UDP : GID = 0x01U
*  Description: Group of UDP related service primitives
*
**/
typedef enum
{
    PID_UDP_CLOSE_SOCKET          = 0x00U,     /* Type: mandatory */
    PID_UDP_CREATE_AND_BIND       = 0x01U,     /* Type: mandatory */
    PID_UDP_SEND_DATA             = 0x02U,     /* Type: mandatory */
    PID_UDP_RECEIVE_AND_FORWARD   = 0x03U,     /* Type: mandatory */
    PID_UDP_CONFIGURE_SOCKET      = 0x06U,     /* Type: mandatory */
    PID_UDP_SHUTDOWN              = 0x07U      /* Type: extension */
} TP_UdpPID_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: TCP : GID = 0x02U
*  Description: Group of TCP related service primitives
*
**/
typedef enum
{
    PID_TCP_CLOSE_SOCKET          = 0x00U,    /* Type: mandatory */
    PID_TCP_CREATE_AND_BIND       = 0x01U,    /* Type: mandatory */
    PID_TCP_SEND_DATA             = 0x02U,    /* Type: mandatory */
    PID_TCP_RECEIVE_AND_FORWARD   = 0x03U,    /* Type: mandatory */
    PID_TCP_LISTEN_AND_ACCEPT     = 0x04U,    /* Type: mandatory */
    PID_TCP_CONNECT               = 0x05U,    /* Type: mandatory */
    PID_TCP_CONFIGURE_SOCKET      = 0x06U,    /* Type: mandatory */
    PID_TCP_SHUTDOWN              = 0x07U,    /* Type: extension */
    PID_TCP_SOCKET_STATE          = 0x7FU     /* Type: optional */
} TP_TcpPID_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: ICMP : GID = 0x03U
*  Description: Group of ICMPv4 related service primitives
*
**/
typedef enum
{
    PID_ICMPv4_ECHO_REQUEST          = 0x00U     /* Type: extension */
} TP_IcmpPID_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: ICMPv6 : GID = 0x04U
*  Description: Group of ICMPv6 related service primitives
*
**/
typedef enum
{
    PID_ICMPv6_ECHO_REQUEST          = 0x00U     /* Type: extension */
} TP_Icmpv6PID_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: IP : GID = 0x05U
*  Description: Group of IPv4 related service primitives
*
**/
typedef enum
{
    PID_IPv4_STATIC_ADDRESS        = 0x00U,    /* Type: extension */
    PID_IPv4_STATIC_ROUTE          = 0x01U     /* Type: extension */
} TP_IpPID_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: IPv6 : GID = 0x06U
*  Description: Group of IPv6 related service primitives
*
**/
typedef enum
{
    PID_IPv6_STATIC_ADDRESS        = 0x00U,    /* Type: extension */
    PID_IPv6_STATIC_ROUTE          = 0x01U     /* Type: extension */
} TP_Ipv6PID_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: DHCP : GID = 0x07U
*  Description: Group of DHCPv4 related service primitives
*
**/
typedef enum
{
    PID_DHCPv4_INIT_CLIENT      = 0x00U,    /* Type: extension */
    PID_DHCPv4_STOP_CLIENT      = 0x01U,    /* Type: extension */
    PID_DHCPv4_SET_OPTION       = 0x02U     /* Type: extension */
} TP_DhcpPID_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: DHCPv6 : GID = 0x08U
*  Description: Group of DHCPv6 related service primitives
*
**/
typedef enum
{
    PID_DHCPv6_INIT_CLIENT      = 0x00U,    /* Type: extension */
    PID_DHCPv6_STOP_CLIENT      = 0x01U,    /* Type: extension */
    PID_DHCPv6_SET_OPTION       = 0x02U     /* Type: extension */
} TP_Dhcpv6PID_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: DHCPv6 : GID = 0x09U
*  Description: Group of ARP related service primitives
*
**/
typedef enum
{
    PID_ARP_CLEAR_CACHE           = 0x00U,    /* Type: extension */
    PID_ARP_ADD_STATIC_ENTRY      = 0x01U,    /* Type: extension */
    PID_ARP_DELETE_STATIC_ENTRY   = 0x02U,    /* Type: extension */
    PID_ARP_SET_TIMEOUT           = 0x03U,    /* Type: extension */
    PID_ARP_CLEAR_TIMEOUT         = 0x04U     /* Type: extension */
} TP_ArpPID_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: ETH : GID = 0x0BU
*  Description: Group of Ethernet Interface related service primitives
*
**/
typedef enum
{
    PID_ETH_INTERFACE_UP          = 0x00U,    /* Type: extension */
    PID_ETH_INTERFACE_DOWN        = 0x01U     /* Type: extension */
} TP_EthPID_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: PHY : GID = 0x0CU
*  Description: Group of Broadr-Reach physical interface related service primitives
*
**/
typedef enum
{
    PID_PHY_READ_SIGNAL_QUALITY   = 0x00U,    /* Type: mandatory */
    PID_PHY_READ_DIAG_RESULT      = 0x01U,    /* Type: mandatory */
    PID_PHY_ACTIVATE_TEST_MODE    = 0x02U,    /* Type: mandatory */
    PID_PHY_SET_PHY_TX_MODE       = 0x03U     /* Type: mandatory */
} TP_PhyPID_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: PHY : GID = 0xFFU
*  Description: Used to close the control channel betwen DUT <=> TESTER
*
**/
typedef enum
{
    PID_CC_CLOSE_CHANNEL      = 0x00U     /* Type: optional */
} TP_CcPID_t;

/* Socket configuration
*  Description: List of socket parameters which can be configured
*
**/
typedef enum
{
    SOCKET_PARAMETER_TTL_HOP                = 0x0000U,  
    SOCKET_PARAMETER_PRIORITY               = 0x0001U,  
    SOCKET_PARAMETER_IP_DF                  = 0x0002U,
    SOCKET_PARAMETER_IP_TIMESTAMP           = 0x0003U,  
    SOCKET_PARAMETER_TOS                    = 0x0004U,  
    SOCKET_PARAMETER_MSS                    = 0x0005U,
    SOCKET_PARAMETER_DISABLE_NAGLE          = 0x0006U,  
    SOCKET_PARAMETER_DISABLE_UDP_CHECKSUM   = 0x0007U
} TP_SocketConfig_t;

/* Dhcp configuration
*  Description: List of dhcp parameters which can be configured
*
**/
typedef enum
{
    DHCP_IP_ADDRESS_LEASE_TIME                = 0x51U,  
    DHCP_MAX_MESSAGE_SIZE                     = 0x57U,  
    DHCP_CLIENT_IDENTIFIER_BY_NAME            = 0x61U,
    DHCP_CLIENT_IDENTIFIER_BY_HARDWARE        = 0x161U
} TP_DhcpConfig_t;

#pragma pack(1)
typedef struct TestabilityProtocolMessage_t
{
    uint16  Sid;                       /* Service ID: defining the Testability Service: default 0x0105  */
    uint8   EVB : 1;                   /* Event Bit */
    uint8   Gid : 7;                   /* Groupe ID */
    uint8   Pid;                       /* Service Primitive ID */        
    uint32  Length;                    /* Amount of following bytes (8 bytes + amount parameter "DAT" bytes) */      
    uint32  ReservedRegister;          /* Reserved register */          
    uint8   ProtocolVersion;           /* Default value 0x01U */
    uint8   InterfaceVersion;          /* Default value 0x01U */
    uint8   Tid;                       /* Message Type ID */
    uint8   Rid;                       /* Result ID*/
    uint8   *Payload;                  /* optional parameters "DAT" */

} TestabilityProtocolMessage_t;
#pragma pack(0)

#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus*/
#endif /* TESTABILITYPROTOCOLTYPES_H */

/** \}    end of addtogroup */