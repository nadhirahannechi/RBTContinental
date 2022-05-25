/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   TestabilityProtocol_cbTypes.h
 *  @brief  AUTOSAR Base - Testability Protocol.
 *  \author uid1145 Mejdi Zayani
 *
 *  \addtogroup Testability_Protocol_package
 *  \{
 */
#ifndef TESTABILITYPROTOCOL_CBTYPES_H
#define TESTABILITYPROTOCOL_CBTYPES_H
/*==================================================================================================
*                                        Include Files
==================================================================================================*/
#include "TestabilityProtocolTypes.h"
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

/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/* Service Primitive ID: PID = 8 Bit
*  Group ID: Gnenral : GID = 0x00U
*  Description: Structure contains callback functions related to General GID = 0x00U 
*
**/
typedef struct
{
    /** Function pointer to user defined callback.
     *  PID = 0x01U
     */
    void (*generalGetVersion_cb)(TP_ResultID_t, uint16, uint16, uint16);

    /** Function pointer to user defined callback.
     *  PID = 0x02U
     */
    void  (*GenaralStartTest_cb)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x03U
     */
    void (*GenaralEndTest_cb)(TP_ResultID_t);

} General_cb_t;


/* Service Primitive ID: PID = 8 Bit
*  Group ID: UDP : GID = 0x01U
*  Description: Structure contains callback functions related to UDP GID = 0x01U 
*
**/
typedef struct
{
    /** Function pointer to user defined callback.
     *  PID = 0x00U
     */
    void (*UdpCloseSocket_cb)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x01U
     */
    void (*UdpCreateAndBind_cb)(TP_ResultID_t, uint16);

    /** Function pointer to user defined callback.
     *  PID = 0x02U
     */
    void (*UdpSendData_cb)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x03U
     */
    void (*UdpReceiveAndForward_cb)(TP_ResultID_t, uint16);

    /** Function pointer to user defined callback for an event.
     *  PID = 0x04U
     */
    void (*UdpReceiveAndForward_event_cb)(TP_ResultID_t, uint16, uint16, ip4addr, vint8);

    /** Function pointer to user defined callback.
     *  PID = 0x06U
     */
    void (*UdpConfigureSocket_cb)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x07U
     *  Type: extension
     */
    void (*UdpShutdown_cb)(TP_ResultID_t);

} Udp_cb_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: TCP : GID = 0x02U
*  Description: Structure contains callback functions related to TCP GID = 0x02U 
*
**/
typedef struct
{
    /** Function pointer to user defined callback.
     *  PID = 0x00U
     */
    void (*TcpCloseSocket_cb)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x01U
     */
    void (*TcpCreateAndBind_cb)(TP_ResultID_t, uint16);

    /** Function pointer to user defined callback.
     *  PID = 0x02U
     */
    void (*TcpSendData_cb)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x03U
     */
    void (*TcpReceiveAndForward_cb)(TP_ResultID_t, uint16);

    /** Function pointer to user defined callback for an event.
     *  PID = 0x04U
     */
    void (*TcpReceiveAndForward_event_cb)(TP_ResultID_t, uint16, vint8);

    /** Function pointer to user defined callback.
     *  PID = 0x04U
     */
    void (*TcpListenAndAccept_cb)(TP_ResultID_t);

    /** Function pointer to user defined callback for an event.
     *  PID = 0x04U
     */
    void (*TcpListenAndAccept_event_cb)(TP_ResultID_t, uint16, uint16, uint16, ip4addr);

    /** Function pointer to user defined callback.
     *  PID = 0x05U
     */
    void (*TcpConnect_cb)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x06U
     */
    void (*TcpConfigureSocket_cb)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x07U
     *  Type: extension
     */
    void (*TcpShutdown_cb)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x7FU
     *  Type: optional
     */
    void (*TcpSocketState_cb)(TP_ResultID_t, uint8);

} Tcp_cb_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: ICMP : GID = 0x03U 
*  Description: Structure contains callback functions related to ICMP GID = 0x03U 
*
**/
typedef struct
{
    /** Function pointer to user defined callback.
     *  PID = 0x00U
     *  Type: extension
     */
    void (*IcmpEchoRequest_cb)(TP_ResultID_t);

} Icmp_cb_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: ICMP : GID = 0x04U 
*  Description: Structure contains callback functions related to ICMPv6 GID = 0x04U 
*
**/
typedef struct
{
    /** Function pointer to user defined callback.
     *  PID = 0x00U
     *  Type: extension
     */
    void (*Icmpv6EchoRequest_cb)(TP_ResultID_t);

} Icmpv6_cb_Type;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: IP : GID = 0x05U 
*  Description: Structure contains callback functions related to IP GID = 0x05U 
*
**/
typedef struct
{
    /** Function pointer to user defined callback.
     *  PID = 0x00U
     *  Type: extension
     */
    void (*Ipv4StaticAddress)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x01U
     *  Type: extension
     */
    void (*Ipv4StaticRoute)(TP_ResultID_t);

} Ipv4_cb_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: IPv6 : GID = 0x06U 
*  Description: Structure contains callback functions related to IPv6 GID = 0x06U 
*
**/
typedef struct
{
    /** Function pointer to user defined callback.
     *  PID = 0x00U
     *  Type: extension
     *  Description: Assigns a static IPv6 address and Netmask to the given network interface.
     */
    void (*Ipv6StaticAddress)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x01U
     *  Type: extension
     *  Description: Adds a static route for the network. This SP is not affecting the persistent configuration.
     */
    void (*Ipv6StaticRoute)(TP_ResultID_t);

} Ipv6_cb_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: DHCP : GID = 0x07U 
*  Description: Structure contains callback functions related to DHCP GID = 0x07U 
*
**/
typedef struct
{
    /** Function pointer to user defined callback.
     *  PID = 0x00U
     *  Type: extension
     */
    void (*DhcpInitClient)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x01U
     *  Type: extension
     */
    void (*DhcpStopClient)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x02U
     *  Type: extension
     */
    void (*DhcpSetClientOptions)(TP_ResultID_t);

} Dhcpv4_cb_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: DHCPv6 : GID = 0x08U 
*  Description: Structure contains callback functions related to DHCPv6 GID = 0x08U 
*
**/
typedef struct
{
    /** Function pointer to user defined callback.
     *  PID = 0x00U
     *  Type: extension
     */
    void (*Dhcpv6InitClient)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x01U
     *  Type: extension
     */
    void (*Dhcpv6StopClient)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x02U
     *  Type: extension
     */
    void (*Dhcpv6SetClientOptions)(TP_ResultID_t);

} Dhcpv6_cb_t;


/* Service Primitive ID: PID = 8 Bit
*  Group ID: DHCP : GID = 0x09U 
*  Description: Structure contains callback functions related to ARP GID = 0x09U 
*
**/
typedef struct
{
    /** Function pointer to user defined callback.
     *  PID = 0x00U
     *  Type: extension
     */
    void (*ClearArpCache)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x01U
     *  Type: extension
     */
    void (*AddStaticEntry)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x02U
     *  Type: extension
     */
    void (*DeleteStaticEntry)(TP_ResultID_t);

} Arp_cb_t;

/* Service Primitive ID: PID = 8 Bit
*  Group ID: ETH : GID = 0x0BU 
*  Description: Structure contains callback functions related to ETH GID = 0x0BU 
*
**/
typedef struct
{
    /** Function pointer to user defined callback.
     *  PID = 0x00U
     *  Type: extension
     */
    void (*EthInterfaceUp)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x01U
     *  Type: extension
     */
    void (*EthInterfaceDown)(TP_ResultID_t);

} Eth_cb_t;


/* Service Primitive ID: PID = 8 Bit
*  Group ID: PHY : GID = 0x0CU 
*  Description: Structure contains callback functions related to PHY GID = 0x0CU 
*
**/
typedef struct
{
    /** Function pointer to user defined callback.
     *  PID = 0x00U
     */
    void (*PhyReadSignalQuality)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x01U
     */
    void (*PhyReadDiagResult)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x02U
     *  Type: extension
     */
    void (*PhyActivateTestMode)(TP_ResultID_t);

    /** Function pointer to user defined callback.
     *  PID = 0x03U
     *  Type: extension
     */
    void (*EthSetPhyTxMode)(TP_ResultID_t);

} Phy_cb_t;

#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus*/
#endif /* TESTABILITYPROTOCOL_CBTYPES_H */

/** \}    end of addtogroup */