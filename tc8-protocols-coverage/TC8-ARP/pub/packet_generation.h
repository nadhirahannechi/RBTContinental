/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   packet_generations.h
 *  @brief 
 *  \author Abderrahim Jamaoui
 *
 *  \addtogroup TC8-ARP
 *  \{
 */
#ifndef PACKET_GENERATION_H
#define PACKET_GENERATION_H
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_01
*
***************************************************************************************************/
/**
* @brief
*   The Address Resolution module tries to find the <protocol type, target protocol address>
    pair in a table. If it finds the pair, it gives the corresponding 48.bit Ethernet address back to
    the caller (hardware driver) which then transmits the packet. (Note: Here DUT is configured
    to add a static entry <HOST-1-IP, MAC-ADDR1> in its ARP cache. TESTER then causes DUT to
    send an UDP Message and expects that DUT will NOT send any ARP Request).  
*
* @return int : 0: DUT : send ARP Request | 1: DUT : Does not send ARP Request.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_01();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_02
*
***************************************************************************************************/
/**
* @brief
*   The Address Resolution module tries to find the <protocol type, target protocol address> 
    pair in a table. If it finds the pair, it gives the corresponding 48.bit Ethernet address back to
    the caller (hardware driver) which then transmits the packet. (Note: Here DUT is configured 
    to add a static entry <HOST-1-IP, MAC-ADDR1> in its ARP cache. TESTER then causes DUT to
    send an UDP Message).   
*
* @return int : 0: DUT : Sends ICMP Echo Request | 1: DUT : Does not Send ICMP Echo Request
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_02();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_03
*
***************************************************************************************************/
/**
* @brief
*   The Address Resolution module tries to find the <protocol type, target protocol address>
    pair in a table. If it finds the pair, it gives the corresponding 48.bit Ethernet address back to
    the caller (hardware driver) which then transmits the packet.
    (Note: The objective of the test case is to validate the ARP Learning mechanism on ARP
    requests. Here TESTER sends an ARP Request to DUT so that an entry <HOST-1-IP, MACADDR1>
    gets added in DUT's ARP cache. TESTER then causes DUT to send an UDP Message and expects that DUT will NOT send any ARP Request.)


*
* @return int : 0: DUT : Does not send ARP Request  | 1: DUT : send ARP Request 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_03();

/***************************************************************************************************
*
*   FUNCTION NAME: ARP_04
*
***************************************************************************************************/
/**
* @brief
*   The Address Resolution module tries to find the <protocol type, target protocol address>
    pair in a table. If it finds the pair, it gives the corresponding 48.bit Ethernet address back to
    the caller (hardware driver) which then transmits the packet. (Note: Here TESTER sends an
    ARP Request to DUT so that an entry <HOST-1-IP, MAC-ADDR1> gets added in DUT's ARP
    cache. TESTER then causes DUT to send an UDP Message.)  


*
* @return int : 0: DUT :  Sends ICMP Message MAC-ADDR1 | 1: DUT :  Sends ICMP Message != MAC-ADDR1
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_04();

/***************************************************************************************************
*
*   FUNCTION NAME: ARP_05
*
***************************************************************************************************/
/**
* @brief
*   The Address Resolution module tries to find the <protocol type, target protocol address>
    pair in a table. If it finds the pair, it gives the corresponding 48.bit Ethernet address back to
    the caller (hardware driver) which then transmits the packet. (Note: Here TESTER sends an
    ARP Response to DUT so that an entry <HOST-1-IP, MAC-ADDR1> gets added in DUT's ARP
    cache. TESTER then causes DUT to send an UDP Message and expects that DUT will NOT
    send any ARP Request.)  

*
* @return int : 0: DUT :   Does not send ARP Request | 1: DUT : Sends ARP Request 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_05();

/***************************************************************************************************
*
*   FUNCTION NAME: ARP_06
*
***************************************************************************************************/
/**
* @brief
*   The Address Resolution module tries to find the <protocol type, target protocol address>
    pair in a table. If it finds the pair, it gives the corresponding 48.bit Ethernet address back to
    the caller (hardware driver) which then transmits the packet. (Note: Here TESTER sends an
    ARP Response to DUT so that an entry <HOST-1-IP, MAC-ADDR1> gets added in DUT's arp
    cache. TESTER then causes DUT to send an UDP Message.) 

*
* @return int : 0: DUT :  Sends ICMP Message MAC-ADDR1 | 1: DUT :  Sends ICMP Message != MAC-ADDR1 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_06();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_07
*
***************************************************************************************************/
/**
* @brief
*   The Address Resolution module tries to find the <protocol type, target protocol address> 
    pair in a table. If it does not, it probably informs the caller that it is throwing the packet
    away (on the assumption the packet will be retransmitted by a higher network layer), and
    generates an Ethernet packet with a type field of ether_type$ADDRESS_RESOLUTION.  

*
* @return int : 0: DUT :    Sends ARP Request | 1: DUT : Does not send ARP Request
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_07();


/***************************************************************************************************
*
*   FUNCTION NAME: ARP_08
*
***************************************************************************************************/
/**
* @brief
*     The Address Resolution module sets the ar$hrd field to ares_hrd$Ethernet.  

*
* @return int : 0: DUT : Sends ARP Request contains Hardware Type is set to ARP_HARDWARE_ETHERNET  | 1: DUT : Sends ARP Request contains Hardware Type != ARP_HARDWARE_ETHERNET
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_08();

/***************************************************************************************************
*
*   FUNCTION NAME: ARP_09
*
***************************************************************************************************/
/**
* @brief
*     The Address Resolution module sets the ar$pro to the protocol type that is being resolved.  

*
* @return int : 0: DUT : Sends ARP Request contains  Protocol Type is set to ARP_PROTOCOL_IP  | 1: DUT : Sends ARP Request contains  Protocol Type != ARP_PROTOCOL_IP
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_09();

/***************************************************************************************************
*
*   FUNCTION NAME: ARP_10
*
***************************************************************************************************/
/**
* @brief
*     The Address Resolution module sets the ar$hln to 6 (the number of bytes in a 48.bit Ethernet address)    

*
* @return int : 0: DUT : Sends ARP Request contains  Hardware Address Length is set to <ETHERNET_ADDR_LEN>   | 1: DUT : Sends ARP Request contains  Hardware Address Length != <ETHERNET_ADDR_LEN> 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_10();

/***************************************************************************************************
*
*   FUNCTION NAME: ARP_11
*
***************************************************************************************************/
/**
* @brief
*     The Address Resolution module sets the ar$pln to the length of an address in that protocol      

*
* @return int : 0: DUT : Sends ARP Request contains  Protocol Address Length is set to IP_ADDR_LEN   | 1: DUT : Sends ARP Request contains  Protocol Address Length != IP_ADDR_LEN 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_11();

/***************************************************************************************************
*
*   FUNCTION NAME: ARP_12
*
***************************************************************************************************/
/**
* @brief
*     The Address Resolution module sets the ar$op to ares_op$REQUEST       

*
* @return int : 0: DUT : Sends ARP Request contains   Operation code is set to OPERATION_REQUEST    | 1: DUT : Sends ARP Request  Operation code is set to OPERATION_REQUEST  
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_12();

/***************************************************************************************************
*
*   FUNCTION NAME: ARP_13
*
***************************************************************************************************/
/**
* @brief
*     The Address Resolution module sets the ar$sha with the 48.bit ethernet address of itself.       

*
* @return int : 0: DUT : Sends ARP Request contains ARP Sender Hardware Address is set to <DIFACE-0-MAC-ADDR>   | 1: DUT : Sends ARP Request  ARP Sender Hardware Address != <DIFACE-0-MAC-ADDR>   
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_13();

/***************************************************************************************************
*
*   FUNCTION NAME: ARP_14
*
***************************************************************************************************/
/**
* @brief
*     The Address Resolution module sets the ar$spa with the protocol address of itself.       

*
* @return int : 0: DUT : Sends ARP Request contains Source IP Address is set to <DIface-0-IP>    | 1: DUT : Sends ARP Request  Source IP Address != <DIface-0-IP>    
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_14();

/***************************************************************************************************
*
*   FUNCTION NAME: ARP_15
*
***************************************************************************************************/
/**
* @brief
*     The Address Resolution module sets the ar$tpa with the protocol address of the machine that is trying to be accessed        

*
* @return int : 0: DUT : Sends ARP Request contains  Destination IP Address is set to <HOST-1-IP>     | 1: DUT : Sends ARP Request   Destination IP Address is set to <HOST-1-IP>    
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_15();



#endif // PACKET_GENERATION_H