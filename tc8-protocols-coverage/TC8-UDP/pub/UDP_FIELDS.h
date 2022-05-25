#ifndef FIELDS_H
#define FIELDS_H
/********************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_FIELDS_01
 *   *
********************************************************************************************************************************************************************************/
/**
* @brief
*    Source Port is an optional field, when meaningful, it indicates the port of the sending
     process, and may be assumed to be the port to which a reply should be addressed in the
     absence of any other information. A user interface should allow the creation of new receive
     ports [Note: In this test, we verify that application can specify source port].
*
* @return int : 0: Tester :  The Tester receive a UDP Message contains source UDP Port field is set to UDPConfiguration.unusedUDPDstPort1 | 1:Tester :  The Tester didn't receive the  UDP message
*
* @note
* -
*
* @warning
* -
*
************************************************************************************************************************************************************************************/
int UDP_FIELDS_01();
/************************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_FIELDS_02
 *   *
***********************************************************************************************************************************************************************************/
/**
* @brief
*    Source Port is an optional field, when meaningful, it indicates the port of the sending
     process, and may be assumed to be the port to which a reply should be addressed in the
     absence of any other information. [Note: In this test, we verify that a UDP messages must
     include a destination port]
*
* @return int : 0: DUT:Tester :  The Tester receive a UDP Message contains Destination UDP Port field is set to UDPConfiguration.unusedUDPSrcPort | 1:Tester :  The Tester didn't receive the UDP message
*
* @note
* -
*
* @warning
* -
*
*************************************************************************************************************************************************************************************/
int UDP_FIELDS_02();
/*************************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_FIELDS_03
 *   *
*************************************************************************************************************************************************************************************/
/**
* @brief
*    Source Port is an optional field, when meaningful, it indicates the port of the sending
     process, and may be assumed to be the port to which a reply should be addressed in the
     absence of any other information. If not used, a value of zero is inserted. [Note: In this test,
     we verify that using the value 0 for UDP Source Port is valid and DUT accepts such a
     Message]
*
* @return int : 0: DUT :  The DUT received UDP message contains UDP Source port set to 0 | 1: DUT :  The DUT did not get a UDP Message contains UDP Source Port set to 0
*
* @note
* -
*
* @warning
* -
*
**************************************************************************************************************************************************************************************/
int UDP_FIELDS_03();
/**************************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_FIELDS_04
 *   *
*************************************************************************************************************************************************************************************/
/**
* @brief
*    Destination Port has a meaning within the context of a particular internet destination
     address. [Note: In this test, we verify that DUT can send UDP message at same destination
     port to more than one different IP addresses.]
*
* @return int : 0: Tester: Received 2 UDP Message contains Destination IP Address filed set to <Host-1-IP> and Destination UDP Port field set to <UDPConfiguration.testerUDPPort> for 1st Message
 * and second Message contains Destination IP Address set to <Host-2-IP> and UDP Port field set to <UDPConfiguration.testerUDPPort>
 * int : 1:  Tester didn't get 2 UDP Message
* @note
* -
*
* @warning
* -
*
**************************************************************************************************************************************************************************************/
int UDP_FIELDS_04();
/*************************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_FIELDS_05
 *   *
**************************************************************************************************************************************************************************************/
/**
* @brief
*    Destination Port has a meaning within the context of a particular internet destination
     address. [Note: In this test, we verify that DUT can receive send UDP message at same
     destination port to more than one different IP addresses.]
*
* @return int : 0: DUT :  The DUT received 1st UDP message contains UDP data equal to <UDPData> and 2nd UDP message contains UDP data equal to <UDPDefaultDat>
 *        int 1: DUT :  The DUT didn't receive UDP messages
*
* @note
* -
*
* @warning
* -
*
*************************************************************************************************************************************************************************************/
int UDP_FIELDS_05();
/************************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_FIELDS_06
 *   *
*************************************************************************************************************************************************************************************/
/**
* @brief
*    Length is the length in octets of this user datagram including this header and the data.
     [Note: In this test, we configure DUT to send <UDPConfiguration.udpUserDataSize> size of data so that the
     length in Header is (<UDPConfiguration.udpUserDataSize > + 8(UDP Header Size)) bytes]
*
* @return int : int : 0: Tester : Verify that DUT send UDP message contains UDP header Length field is set to (<UDPConfiguration.udpUserDataSize>+8)
 *        int : 1: Tester :  Tester didn't get UDP message From tester or Tester received UDP message contains UDP header Length different to (<udoUserDataSize>+8)

*
* @note
* -
*
* @warning
* -
*
*************************************************************************************************************************************************************************************/
int UDP_FIELDS_06();
/************************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_FIELDS_07
 *   *
*************************************************************************************************************************************************************************************/
/**
* @brief
*    Length is the length in octets of this user datagram including this header and the data.
     [Note: In this test, we configure DUT to send 0 (zero) size of data so that the length in
     Header is 8(UDP Header Size)) bytes]
*
* @return int : 0: Tester :  Tester Verify that received UDP message from DUT contains UDP header Length set to 8
 *        int : 1: Tester :  Tester receive UDP message from DUT contains UDP header Length different to 8 or DUT didn't send UDP Message
**
* @note
* -
*
* @warning
* -
*
********************************************************************************************************************************************************************************/
int UDP_FIELDS_07();
/********************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_FIELDS_08
 *   *
*******************************************************************************************************************************************************************************/
/**
* @brief
*    Length is the length in octets of this user datagram including this header and the data.
     [Note: Check that the DUT discards the received datagram in case the total length of the
     datagram is less than 8 bytes]
*
*  @return int : 0: DUT :  The DUT discards the UDP Message | 1: DUT :  The DUT accepts the UDP Message

* @note
* -
*
* @warning
* -
*
*******************************************************************************************************************************************************************************/
int UDP_FIELDS_08();
/******************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_FIELDS_09
 *   *
********************************************************************************************************************************************************************************/
/**
* @brief
*    Length is the length in octets of this user datagram including this header and the data.
     [Note: Check that the DUT discards the received datagram in case the total length of the
     datagram is zero (‘0’)]
*
* @return int : 0: DUT :  The DUT discards the UDP Message | 1: DUT :  The DUT accepts the UDP Message
*
* @note
* -
*
* @warning
* -
*
*******************************************************************************************************************************************************************************/
int UDP_FIELDS_09();
/******************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_FIELDS_10
 *   *
******************************************************************************************************************************************************************************/
/**
* @brief
*    Length is the length in octets of this user datagram including this header and the data.
     [Note: Check that the DUT discards the received datagram in case the length value in the
     header is greater than the actual length of the datagram]
*
* @return int : 0: DUT :  The DUT discards the UDP Message | 1: DUT :  The DUT accepts the UDP Message
*
* @note
* -
*
* @warning
* -
*
********************************************************************************************************************************************************************************/
int UDP_FIELDS_10();
/****************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_FIELDS_12
 *   *
******************************************************************************************************************************************************************************/
/**
* @brief
*    Length is the length in octets of this user datagram including this header and the data.
     [Note: Check that the DUT accepts the received datagram in case the length value in the
     header is set to the maximum allowed value.
     - IPv4: 65,507 bytes (65,535 − 8 byte UDP header − 20 byte IP header)
     - IPv6: 65,535 bytes]
*
* @return int : 0: DUT :  The DUT received the UDP Message | 1: DUT :  The DUT didn't get the UDP Message
*
* @note
* -
*
* @warning
* -
*
*****************************************************************************************************************************************************************************/
int UDP_FIELDS_12();
/**************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_FIELDS_13
 *   *
***************************************************************************************************************************************************************************/
/**
* @brief
*    Checksum is the 16-bit one's complement of the one's complement sum of a pseudo header
     of information from the IP header, the UDP header, and the data, padded with zero octets
     at the end (if necessary) to make a multiple of two octets. [Note: In this test, we verify that
     DUT calculates UDP checksum correctly. While calculating UDP checksum the padded byte
     is needed to be considered.]
*
* @return int : 0: Tester :Tester Verify that received UDP Message from DUT contains UDP Header Checksum field is set to <calculatedUDPChecksum>
 *        int : 1: Tester : Tester received UDP Message from DUT contains UPD Header Checksum different to <calculatedUDPChecksum>
*
* @note
* -
*
* @warning
* -
*
************************************************************************************************************************************************************************/
int UDP_FIELDS_13();
/************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_FIELDS_14
 *   *
***********************************************************************************************************************************************************************/
/**
* @brief
*    Checksum is the 16-bit one's complement of the one's complement sum of a pseudo header
     of information from the IP header, the UDP header, and the data, padded with zero octets
     at the end (if necessary) to make a multiple of two octets. [Note: In this test, we verify that
     DUT calculates UDP checksum correctly. While calculating UDP checksum the padded byte
     is not required.]
*
* @return int : 0: Tester :Tester Verify that received UDP Message from DUT contains UDP Header Checksum field is set to <calculatedUDPChecksum>
 *        int : 1: Tester : Tester received UDP Message from DUT contains UPD Header Checksum different to <calculatedUDPChecksum>
*
* @note
* -
*
* @warning
* -
*
*************************************************************************************************************************************************************************/
int UDP_FIELDS_14();
/************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_FIELDS_15
 *   *
************************************************************************************************************************************************************************/
/**
* @brief
*    If a UDP datagram is received with a checksum that is non-zero and invalid, UDP MUST
     silently discard the datagram. [Note: In this test, we verify that DUT will not accept UDP
     message with incorrect checksum]
*
* @return int : 0: DUT :  The DUT didn't get the UDP Message | 1: DUT :  The DUT accepts the UDP Message
*
* @note
* -
*
* @warning
* -
*
*******************************************************************************************************************************************************************/
int UDP_FIELDS_15();
/******************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_FIELDS_16
 *   *
******************************************************************************************************************************************************************/
/**
* @brief
*    An all zero transmitted checksum value means that the transmitter generated no checksum
     (for debugging or for higher level protocols that don't care). An application MAY optionally
     be able to control whether a UDP checksum will be generated, but it MUST default to
     checksumming on. [Note: In this test, we verify that DUT accepts UDP datagram with zero
     checksum]
*
* @return int : 0: DUT :  The DUT accept the UDP Message contains UDP data equal to <UDPDefaultData> | 1: DUT :  The DUT didn't get the UDP Message
*
* @note
* -
*
* @warning
* -
*
*******************************************************************************************************************************************************************/
int UDP_FIELDS_16();

#endif //FIELDS_H