#ifndef INTRODUCTION_H
#define INTRODUCTION_H

/***************************************************************************************************
*
*   FUNCTION NAME: UDP_INTRODUCTION_01
*
***************************************************************************************************/
/**
* @brief
*   UDP is used by applications that do not require the level of service of TCP or that wish to use communications services (e.g., multicast or broadcast delivery)
    not available from TCP. [Note: In this test we verify that DUT will deny UDP message with broadcast destination Address.
    Note: this test inverts the RFC requirement due to security negotiations]
*
* @return int : 0: DUT :  The DUT discards the UDP packet that contains destination address equal to <AIface-0-BcastIP>
 *        int : 1: DUT :  The DUT accepts the UDP packet that contains destination address equal to <AIface-0-BcastIP>
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int UDP_INTRODUCTION_01();
/***************************************************************************************************
*
*   FUNCTION NAME: UDP_INTRODUCTION_02
*
***************************************************************************************************/
/**
* @brief
*   UDP is used by applications that do not require the level of service of TCP or that wish to use communications services (e.g., multicast or broadcast delivery)
    not available from TCP. [Note: In this test we verify that DUT will deny UDP message with multicast destination Address.
    Note: this test inverts the RFC requirement due to security negotiations]
*
* @return int : 0: DUT :  The DUT discards the UDP packet that contains destination address equal to <allSystemCastAddr>
 *        int : 1: DUT :  The DUT accepts the UDP packet that contains destination address equal to <allSystemCastAddr>
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int UDP_INTRODUCTION_02();
/***************************************************************************************************
*
*   FUNCTION NAME: UDP_INTRODUCTION_03
*
***************************************************************************************************/
/**
* @brief
*   If a datagram arrives addressed to a UDP port for which there is no pending LISTEN call, UDP SHOULD send an ICMP Port Unreachable message.
*
* @return int : 0: DUT :  The DUT sends <ICMP-Dest-Unrchbl> Message | : 1: DUT :  The DUT  didn't send ICMP packet
 *
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int UDP_INTRODUCTION_03();

#endif //INTRODUCTION_H