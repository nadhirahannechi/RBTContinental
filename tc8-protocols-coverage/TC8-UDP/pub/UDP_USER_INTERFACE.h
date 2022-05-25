#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H
/**********************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_USER_INTERFACE_01
 *   *
***********************************************************************************************************************************************/
/**
* @brief
*   A user interface should allow the creation of new receive ports, receive operations on the receive ports that return the data octets and an
    indication of source port and source address, and an operation that allows a datagram to be sent, specifying the data, source
    and destination ports and addresses to be sent. [Note: In this test, we verify that user interface allows creation of new receive ports.
    This test is only ran when <DUTSupportsDynamicInterface> is TRUE]
*
* @return int : 0: DUT: DUT creates 10 receive ports | 1: DUT: DUT didn't creates 10 receive ports
*
* @note
* -
*
* @warning
* -
*
*********************************************************************************************************************************************/
int UDP_USER_INTERFACE_01();
/*********************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_USER_INTERFACE_02
 *   *
***********************************************************************************************************************************************/
/**
* @brief
*   A user interface should allow the creation of new receive ports, receive operations on the receive ports that return the data octets and an
    indication of source port and source address, and an operation that allows a datagram to be sent, specifying the data, source
    and destination ports and addresses to be sent. [Note: In this test, we verify that user interface allows creation of new receive ports.
    This test is only ran when <DUTSupportsDynamicInterface> is TRUE]
*
* @return int : 0: DUT: DUT accepts UDP Message contains UDP data equal to <UDPDefaultData> | 1: DUT: DUT discards the UDP Message
*
* @note
* -
*
* @warning
* -
*
*********************************************************************************************************************************************/
int UDP_USER_INTERFACE_02();
/*********************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_USER_INTERFACE_03
 *   *
*************************************************************************************************************************************************/
/**
* @brief
*   A user interface should allow the creation of new receive ports, receive operations on the receive ports that return the data octets and an
    indication of source port and source address, and an operation that allows a datagram to be sent, specifying the data, source
    and destination ports and addresses to be sent. [Note: In this test, we verify that user interface allows creation of new receive ports.
    This test is only ran when <DUTSupportsDynamicInterface> is TRUE]
*
* @return int : 0: DUT: DUT accepts UDP Message contains UDP Source Port field set to <UDPConfiguration.unusedUDPSrcPort> | 1: DUT: DUT discards the UDP Message
*
* @note
* -
*
* @warning
* -
*
**********************************************************************************************************************************************/
int UDP_USER_INTERFACE_03();
/**********************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_USER_INTERFACE_03
 *   *
*************************************************************************************************************************************************/
/**
* @brief
*   A user interface should allow the creation of new receive ports, receive operations on the receive ports that return the data octets and an
    indication of source port and source address, and an operation that allows a datagram to be sent, specifying the data, source
    and destination ports and addresses to be sent. [Note: In this test, we verify that user interface allows creation of new receive ports.
    This test is only ran when <DUTSupportsDynamicInterface> is TRUE]
*
* @return int : 0: DUT: DUT accepts UDP Message contains UDP source address equal to <AIface-0-IP> | 1: DUT: DUT discards the UDP Message
*
* @note
* -
*
* @warning
* -
*
**************************************************************************************************************************************************************************/
int UDP_USER_INTERFACE_04();
/*************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_USER_INTERFACE_05
 *   *
***************************************************************************************************************************************************************************/
/**
* @brief
*   A user interface should allow the creation of new receive ports, receive operations on the receive ports that return the data octets and an
    indication of source port and source address, and an operation that allows a datagram to be sent, specifying the data, source
    and destination ports and addresses to be sent. [Note: In this test, we verify that user interface allows creation of new receive ports.
    This test is only ran when <DUTSupportsDynamicInterface> is TRUE]
*
* @return int : 0: Tester: Tester verify that received UDP Message contains UDP Source UDP Port equal to <UDPConfiguration.unusedUDPSrcPort> | 1: Tester: Tester didn't get the UDP Message
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************************************************************************************/
int UDP_USER_INTERFACE_05();
/**********************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_USER_INTERFACE_06
 *   *
**********************************************************************************************************************************************************************/
/**
* @brief
*   A user interface should allow the creation of new receive ports, receive operations on the receive ports that return the data octets and an
    indication of source port and source address, and an operation that allows a datagram to be sent, specifying the data, source
    and destination ports and addresses to be sent. [Note: In this test, we verify that user interface allows creation of new receive ports.
    This test is only ran when <DUTSupportsDynamicInterface> is TRUE]
*
* @return int : 0: Tester: Tester verify that received UDP Message contains UDP Destination UDP Port equal to <UDPConfiguration.testerUDPPort> | 1: Tester: Tester didn't get the UDP Message
*
* @note
* -
*
* @warning
* -
*
*************************************************************************************************************************************************/
int UDP_USER_INTERFACE_06();
/**************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_USER_INTERFACE_07
 *   *
**************************************************************************************************************************************************/
/**
* @brief
*   A user interface should allow the creation of new receive ports, receive operations on the receive ports that return the data octets and an
    indication of source port and source address, and an operation that allows a datagram to be sent, specifying the data, source
    and destination ports and addresses to be sent. [Note: In this test, we verify that user interface allows creation of new receive ports.
    This test is only ran when <DUTSupportsDynamicInterface> is TRUE]
*
* @return int : 0: DUT: DUT accepts UDP Message contains Source IP address is et to <DIface-0-IP> | 1: Tester: Tester didn't get the UDP Message
*
* @note
* -
*
* @warning
* -
*
*********************************************************************************************************************************************************/
int UDP_USER_INTERFACE_07();
/*********************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_USER_INTERFACE_08
 *   *
*********************************************************************************************************************************************************/
/**
* @brief
*   A user interface should allow the creation of new receive ports, receive operations on the receive ports that return the data octets and an
    indication of source port and source address, and an operation that allows a datagram to be sent, specifying the data, source
    and destination ports and addresses to be sent. [Note: In this test, we verify that user interface allows creation of new receive ports.
    This test is only ran when <DUTSupportsDynamicInterface> is TRUE]
*
* @return int : 0: DUT: DUT accepts UDP Message contains Destination IP address is et to <AIface-0-IP> | 1: Tester: Tester didn't get the UDP Message
*
* @note
* -
*
* @warning
* -
*
**********************************************************************************************************************************************************/
int UDP_USER_INTERFACE_08();

#endif //USER_INTERFACE_H