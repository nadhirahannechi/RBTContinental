/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   TestabilityProtocol_api.h
 *  @brief  AUTOSAR Base - Testability Protocol.
 *  \author uid1145 Mejdi Zayani
 *
 *  \addtogroup Testability_Protocol_package
 *  \{
 */
#ifndef TESTABILITYPROTOCOLAPI_H
#define TESTABILITYPROTOCOLAPI_H
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

/*==================================================================================================
*                                        Global Variables
==================================================================================================*/

/*==================================================================================================
*                                     Local Functions prototypes
==================================================================================================*/

/*==================================================================================================
*                                     Global Functions prototypes
==================================================================================================*/




/***************************************************************************************************
*
*   FUNCTION NAME: TP_GenaralGetVersion
*
***************************************************************************************************/
/**
* @brief
*  This SP will return the testability protocol version of the used protocol 
*  and service primitive implementation. The testability protocol version is
*  bound to the TC release version the protocol is based on. The current version is TC1.2.0. 
*
* @par
* @param[in] Function pointer to user defined callback.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_GenaralGetVersion(void (*generalGetVersion_cb)(TP_ResultID_t, uint16, uint16, uint16));

/***************************************************************************************************
*
*   FUNCTION NAME: TP_GenaralStartTest
*
***************************************************************************************************/
/**
* @brief
*  The purpose of this SP is to have a defined entry tag in trace at the point 
*  in time the test case was started. This SP does not have any request parameters. 
*
* @par
* @param[in] Function pointer to user defined callback.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_GenaralStartTest(void (*GenaralStartTest_cb)(TP_ResultID_t));

/***************************************************************************************************
*
*   FUNCTION NAME: TP_GenaralEndTest
*
***************************************************************************************************/
/**
* @brief
*  The purpose of this SP is to reset the Upper Tester. All sockets of the test channel will be closed, 
*  counters are set to the default value, buffers are cleared and active service primitives will be terminated. 
*  Another purpose of this SP is to have a defined entry tag in trace at the point in time the test case was stopped. 
*  
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Contains Testability Protocol message
* @param[in] tcId : Type uint16 : The test case ID going to be terminated.
* @param[in] tsName : Type text : The test suite name.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note The parameters (tcId and tsName) may be ignored by the testability module.
* -
*  
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_GenaralEndTest(void (*GenaralEndTest_cb)(TP_ResultID_t), uint16 tcId, text tsName);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_UdpCloseSocket
*
***************************************************************************************************/
/**
* @brief
*  Closes a socket.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : Socket that should be closed.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_UdpCloseSocket(void (*UdpCloseSocket_cb)(TP_ResultID_t), uint16 socketId);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_UdpCreateAndBind
*
***************************************************************************************************/
/**
* @brief
*  Creates a socket and optionally binds this socket to a port and a local IP address.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] doBind : Type boolean : true: bind will be performed | false: no bind will be performed.
* @param[in] localPort : Type uint16 : Local port to bind (0xFFFF: PORT_ANY)
* @param[in] localAddr : Type ipxaddr : Local address (n=4:IPv4 or n=16:IPv6) 
*                        Any IP: If all address bytes are zero The domain is selected by the type of address 
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_UdpCreateAndBind(void (*UdpCreateAndBind_cb)(TP_ResultID_t, uint16),boolean doBind, uint16 localPort, ip4addr localAddr);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_UdpSendData
*
***************************************************************************************************/
/**
* @brief
*  Sends data to a target. 
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : Local socket used to perform the transmission.
* @param[in] totalLen : Type uint16 : Total length: repeat data up to that length (in bytes). 
*                       In case the value of totalLen is smaller than the length of data, 
*                       the full length of data will be transmitted.
* @param[in] destPort : Type uint16 : Destination port.
* @param[in] destAddr : Type destAddr : Destination address.
* @param[in] data : Type vint8 : Data to transmit.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_UdpSendData(void (*TP_UdpSendData_cb)(TP_ResultID_t), uint16 socketId, uint16 totalLen, uint16 destPort, ip4addr destAddr,
                                                            vint8 data);


/***************************************************************************************************
*
*   FUNCTION NAME: TP_UdpReceiveAndForward
*
***************************************************************************************************/
/**
* @brief
*  Data that will be received after the call of this SP will be forwarded to the test system. 
*  The amount of forwarded data per received datagram (UDP) or bulk of stream data (TCP) can be limited using maxFwd. 
*  The original length of this data unit can be obtained by fullLen. The process will repeat itself (active phase) 
*  until the maximum amount of data defined by maxLen was received or END_TEST was called (inactive phase). 
*  UDP: No further requirements. (see 6.12.2 UDP Receive and Count)
*  TCP: In the inactive phase (e.g. prior the first call) all data received will be discarded or ignored. 
*       When called all data that was received on the specified socked prior the call of this SP will be consumed
*  in order to open the TCP receive window. All data that is received during the active phase of this SP will be consumed 
*  up to the maximum amount of data defined by maxLen. (see 6.12.4 TCP Client Receive and Forward).
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] Function pointer to user defined callback to receive an event.
* @param[in] socketId : Type uint16 : The Socket selected for forwarding.
* @param[in] maxFwd : Type uint16 : Maximum length of payload to be forwarded per event.
* @param[in] maxLen : Type uint16 : Maximum count of bytes to receive over all (0xFFFF: limitless)

*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_UdpReceiveAndForward(void (*TP_UdpReceiveAndForward_cb)(TP_ResultID_t, uint16), void (*TP_UdpReceiveAndForward_event_cb)(TP_ResultID_t, uint16, uint16, ip4addr, vint8),
                                                         uint16 socketId, uint16 maxFwd, uint16 maxLen);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_UdpConfigureSocket
*
***************************************************************************************************/
/**
* @brief
*  This SP is used to select and set parameters that can be configured on a socket basis. More parameters may be 
*  supported in following versions of this document or by non-standard extensions (Parameter IDs starting
*  with 0xFFFF, 0xFFFE… and so forth)
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : socket that should be configured.
* @param[in] paramId : Type uint16 : Selects the parameter to be configured:
*                      0x0000  (1 Byte): TTL/Hop Limit
*                      0x0001  (1 Byte): Priority (traffic class/DSCP & ECN)
*                      0x0002  (1 Byte): IP DF DontFragment
*                      0x0003  (N Bytes): IP Timestamp Option data as stored in the IP header option 4 as described by RFC 791 page 22 
*                      0x0004  (1 Byte): IP Type of Service TOS encoded as defined by RFC 791 page 29 (Delay, Throughout, Reliability, Cost, MBZ) 
*                      0x0005  (2 Byte): Set MSS MaxSegmentSize (valid values 5001460) 
*                      0x0006  (1 Byte): Enable/disable Nagle Algorithm parameter (enabled=1) 
*                      0x0007  (1 Byte): Enable/disable the transmission of the UDP checksum (enabled=1)
* @param[in] paramVal  : Type vint8 : The value of the selected parameter that must match the corresponding length. 
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_UdpConfigureSocket(void (*UdpConfigureSocket_cb)(TP_ResultID_t), uint16 socketId, uint16 paramId, vint8 paramVal);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_UdpShutdown
*
***************************************************************************************************/
/**
* @brief
*  Shuts down a socket.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : Socket that should shutdown.
* @param[in] typeId : Type uint8 : Selects the way the socket is shutdown: 
*                                        0x00: further reception will be disallowed
*                                        0x01: further transmission will be disallowed.
*                                        0x02: further transmission and reception will be disallowed.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_UdpShutdown(void (*UdpShutdown_cb)(TP_ResultID_t), uint16 socketId, uint8 typeId);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpCloseSocket
*
***************************************************************************************************/
/**
* @brief
*  Closes a socket.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : Socket that should be closed.
* @param[in] abort : Type boolean : When true: closes the socket immediately, the stack is not waiting for 
*                                   outstanding transmissions and acknowledgements 
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpCloseSocket(void (*TcpCloseSocket_cb)(TP_ResultID_t), uint16 socketId, boolean abort);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpCreateAndBind
*
***************************************************************************************************/
/**
* @brief
*  Creates a socket and optionally binds this socket to a port and a local IP address.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] doBind : Type boolean : true: bind will be performed | false: no bind will be performed.
* @param[in] localPort : Type uint16 : Local port to bind (0xFFFF: PORT_ANY)
* @param[in] localAddr : Type ipxaddr : Local address (n=4:IPv4 or n=16:IPv6) 
*                        Any IP: If all address bytes are zero The domain is selected by the type of address 
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpCreateAndBind(void (*TcpCreateAndBind_cb)(TP_ResultID_t, uint16),boolean doBind, uint16 localPort, 
                                                    ip4addr localAddr);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpSendData
*
***************************************************************************************************/
/**
* @brief
*  Sends data to a target. 
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : Local socket used to perform the transmission.
* @param[in] totalLen : Type uint16 : Total length: repeat data up to that length (in bytes). 
*                       In case the value of totalLen is smaller than the length of data, 
*                       the full length of data will be transmitted.
* @param[in] flags : Type uint8 : Bit 7: reserved 
*                                 Bit 6: reserved
*                                 Bit 5: URG
*                                 Bit 4: reserved
*                                 Bit 3: PSH
*                                 Bit 2: reserved
*                                 Bit 1: reserved
*                                 Bit 0: reserved 
* @param[in] data : Type vint8 : Data to transmit.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpSendData(void (*TP_TcpSendData_cb)(TP_ResultID_t), uint16 socketId, uint16 totalLen, 
                                                                                    uint8 flags, vint8 data);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpReceiveAndForward
*
***************************************************************************************************/
/**
* @brief
*  Data that will be received after the call of this SP will be forwarded to the test system. 
*  The amount of forwarded data per received datagram (UDP) or bulk of stream data (TCP) can be limited using maxFwd. 
*  The original length of this data unit can be obtained by fullLen. The process will repeat itself (active phase) 
*  until the maximum amount of data defined by maxLen was received or END_TEST was called (inactive phase). 
*  UDP: No further requirements. (see 6.12.2 UDP Receive and Count)
*  TCP: In the inactive phase (e.g. prior the first call) all data received will be discarded or ignored. 
*       When called all data that was received on the specified socked prior the call of this SP will be consumed
*  in order to open the TCP receive window. All data that is received during the active phase of this SP will be consumed 
*  up to the maximum amount of data defined by maxLen. (see 6.12.4 TCP Client Receive and Forward).
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] Function pointer to user defined callback to receive an event.
* @param[in] socketId : Type uint16 : The Socket selected for forwarding.
* @param[in] maxFwd : Type uint16 : Maximum length of payload to be forwarded per event.
* @param[in] maxLen : Type uint16 : Maximum count of bytes to receive over all (0xFFFF: limitless)

*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpReceiveAndForward(void (*TP_TcpReceiveAndForward_cb)(TP_ResultID_t, uint16),  void (*TP_TcpReceiveAndForward_event_cb)(TP_ResultID_t, uint16, vint8),
                                                        uint16 socketId, uint16 maxFwd, uint16 maxLen);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpListenAndAccept
*
***************************************************************************************************/
/**
* @brief
*  Marks a socket as listen socket that will be used to accept incoming connections. Whenever a new 
*  connection was established this SP provides the socket ID of the new connection together with the listen
*  socket, client port, and address in an event. 
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] Function pointer to user defined callback to receive an event.
* @param[in] listenSocketId : Type uint16 : Local socket that should listen.
* @param[in] maxCon : Type uint16 : Maximum number of connections allowed to establish.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpListenAndAccept(void (*TP_TcpListenAndAccept_cb)(TP_ResultID_t), void (*TP_TcpListenAndAccept_event_cb)(TP_ResultID_t, uint16, uint16, uint16, ip4addr),
                                                            uint16 listenSocketId, uint16 maxCon);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpConnect
*
***************************************************************************************************/
/**
* @brief
*  Triggers a TCP connection to a remote destination.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : TCP socket that should connect to a remote destination.
* @param[in] destPort : Type uint16 : Port of the remote destination.
* @param[in] destAddr : Type ip4addr : IP address of the remote destination.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpConnect(void (*TP_TcpConnect_cb)(TP_ResultID_t), uint16 socketId, uint16 destPort, ip4addr destAddr);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpConfigureSocket
*
***************************************************************************************************/
/**
* @brief
*  This SP is used to select and set parameters that can be configured on a socket basis. More parameters may be 
*  supported in following versions of this document or by non-standard extensions (Parameter IDs starting
*  with 0xFFFF, 0xFFFE… and so forth)
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : socket that should be configured.
* @param[in] paramId : Type uint16 : Selects the parameter to be configured:
*                      0x0000  (1 Byte): TTL/Hop Limit
*                      0x0001  (1 Byte): Priority (traffic class/DSCP & ECN)
*                      0x0002  (1 Byte): IP DF DontFragment
*                      0x0003  (N Bytes): IP Timestamp Option data as stored in the IP header option 4 as described by RFC 791 page 22 
*                      0x0004  (1 Byte): IP Type of Service TOS encoded as defined by RFC 791 page 29 (Delay, Throughout, Reliability, Cost, MBZ) 
*                      0x0005  (2 Byte): Set MSS MaxSegmentSize (valid values 5001460) 
*                      0x0006  (1 Byte): Enable/disable Nagle Algorithm parameter (enabled=1) 
*                      0x0007  (1 Byte): Enable/disable the transmission of the UDP checksum (enabled=1)
* @param[in] paramVal  : Type vint8 : The value of the selected parameter that must match the corresponding length. 
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpConfigureSocket(void (*TP_TcpConfigureSocket_cb)(TP_ResultID_t), uint16 socketId, uint16 paramId, vint8 paramVal);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpShutdown
*
***************************************************************************************************/
/**
* @brief
*  Shuts down a socket.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : Socket that should shutdown.
* @param[in] typeId : Type uint8 : Selects the way the socket is shutdown: 
*                                        0x00: further reception will be disallowed
*                                        0x01: further transmission will be disallowed.
*                                        0x02: further transmission and reception will be disallowed.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpShutdown(void (*TcpShutdown_cb)(TP_ResultID_t), uint16 socketId, uint8 typeId);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpSocketState
*
***************************************************************************************************/
/**
* @brief
*  Return the state of a socket.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] localPort : Type uint16 : local port.
* @param[in] localAddr : Type ip4addr : local IP address.
* @param[in] destPort : Type uint16 : local port.
* @param[in] destAddr : Type ip4addr : local IP address.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpSocketState(void (*TP_TcpSocketState_cb)(TP_ResultID_t, uint8), uint16 localPort, ip4addr localAddr, 
                                                                    uint16 destPort, ip4addr destAddr);


/***************************************************************************************************
*
*   FUNCTION NAME: TP_Ipv4StaticAddress
*
***************************************************************************************************/
/**
* @brief
*  Assigns a static IP address and Netmask to the given network interface.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS)
* @param[in] addr : Type ip4addr The subnet for the route.
* @param[in] netMask  : Type uint8 The subnet mask for the route in CIDR-notation.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_Ipv4StaticAddress(void (*TP_Ipv4StaticAddress_cb)(TP_ResultID_t), text ifName, ip4addr addr, uint8 netmask);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_Ipv4StaticRoute
*
***************************************************************************************************/
/**
* @brief
*  Adds a static route for the network. This SP is not affecting the persistent configuration.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : Type text The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS)
* @param[in] subNeT : Type ip4addr The subnet for the route.
* @param[in] netMask  : Type uint8 The subnet mask for the route in CIDR-notation.
* @param[in] gatway : Type ip4addr The gateway IP address for the route.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_Ipv4StaticRoute(void (*TP_Ipv4StaticRoute_cb)(TP_ResultID_t), text ifName, ip4addr subNet,
                                                     uint8 netMask, ip4addr gateway);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DhcpInitClient
*
***************************************************************************************************/
/**
* @brief
*  Initialize the DHCP Client by use of network interface and port.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS)
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DhcpInitClient(void (*TP_DhcpInitClient_cb)(TP_ResultID_t), text ifName);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DhcpStopClient
*
***************************************************************************************************/
/**
* @brief
*  Shutdown the DHCP Client by use of network interface and port.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS)
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DhcpStopClient(void (*TP_DhcpStopClient_cb)(TP_ResultID_t), text ifName);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DhcpSetClientOptions
*
***************************************************************************************************/
/**
* @brief
*  Sets DHCP Client options.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS).
* @param[in] code : DHCP option code:
*                                       51d  [4  Byte]:   IP address lease time.
*                                       57d  [2  Byte]:   Maximum message size.
*                                       61d  [1…*  Byte]: Client identifier by name.
*                                       161d [6  Byte]:   Client identifier by hardware address.
* @param[in] value : DHCP option value selected by the code parameter using the corresponding byte size.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DhcpSetClientOptions(void (*TP_DhcpSetClientOptions_cb)(TP_ResultID_t), text ifName, uint8 code, vint8 value);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_Dhcpv6InitClient
*
***************************************************************************************************/
/**
* @brief
*  Initialize the DHCPv6 Client by use of network interface and port.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS)
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_Dhcpv6InitClient(void (*TP_Dhcpv6InitClient_cb)(TP_ResultID_t), text ifName);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_Dhcpv6StopClient
*
***************************************************************************************************/
/**
* @brief
*  Shutdown the DHCPv6 Client by use of network interface and port.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS)
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_Dhcpv6StopClient(void (*TP_Dhcpv6StopClient_cb)(TP_ResultID_t), text ifName);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_EthInterfaceUp
*
***************************************************************************************************/
/**
* @brief
*  Enables an Ethernet interface or virtual interface. This SP is not affecting the persistent configuration.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS)
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_EthInterfaceUp(void (*TP_EthInterfaceUp_cb)(TP_ResultID_t), text ifName);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_EthInterfaceDown
*
***************************************************************************************************/
/**
* @brief
*  Disables an Ethernet interface or virtual interface. This SP is not affecting the persistent configuration.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS)
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_EthInterfaceDown(void (*TP_EthInterfaceDown_cb)(TP_ResultID_t), text ifName);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_ClearArpCache
*
***************************************************************************************************/
/**
* @brief
*  Clear the dynamic entries in the ARP Cache of DUT.
*
* @par
* @param[in] Function pointer to user defined callback.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_ClearArpCache(void (*TP_ClearArpCache_cb)(TP_ResultID_t));

/***************************************************************************************************
*
*   FUNCTION NAME: TP_AddStaticEntry
*
***************************************************************************************************/
/**
* @brief
*  Add a static entry in DUT ARP Cache.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] addip the Internet address of the entry to add.
* @param[in] addmac the hardware address of the entry to add.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_AddStaticEntry(void (*TP_AddStaticEntry_cb)(TP_ResultID_t), vint8 addip, vint8 addmac);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DeleteStaticEntry
*
***************************************************************************************************/
/**
* @brief
*  Delete a static entry in DUT ARP Cache.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] addip the Internet address of the entry to delete.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DeleteStaticEntry(void (*TP_DeleteStaticEntry_cb)(TP_ResultID_t), vint8 addip);
/***************************************************************************************************
*
*   FUNCTION NAME: TP_IcmpEchoRequest
*
***************************************************************************************************/
/**
* @brief
*  Issues the transmission of an ICMP Echo Request
*
* @par
* @param[in] ifName     : Type text     : Optional: The name of the network interface (e.g.
*                                               “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-
*                                               BE6E-101110033111}” or “0” depending of the
*                                               OS)
* @param[in] destAddr   : Type ipxaddr  : The destination address.
* @param[in] data       : Type vint8    : Payload to transmit
*
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_IcmpEchoRequest(void (*TP_IcmpEchoRequest_cb)(TP_ResultID_t), text ifName, ip4addr destAddr, vint8 data);

#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus*/
#endif /* TESTABILITYPROTOCOLAPI_H */

/** \}    end of addtogroup */
