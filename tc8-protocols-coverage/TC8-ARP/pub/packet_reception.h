/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   packet_reception.h
 *  @brief 
 *  \author Abderrahim Jamaoui
 *
 *  \addtogroup TC8-ARP
 *  \{
 */

#ifndef PACKET_RECEPTION_H
#define PACKET_RECEPTION_H

/***************************************************************************************************
*
*   FUNCTION NAME: ARP_16
*
***************************************************************************************************/
/**
* @brief
*    The Address Resolution module does not set ar$tha to anything in particular, because it is 
    this value that it is trying to determine. (Note: In this test TESTER sends an ARP Request
    with ARP Target Hardware Address set to all zeroes, and, expects that DUT will send an ARP
    Response after receiving the ARP Request)  
*
* @return int : 0: DUT :  Sends ARP Response | 1: DUT :  Does send ARP Response
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_16();

/***************************************************************************************************
*
*   FUNCTION NAME: ARP_17
*
***************************************************************************************************/
/**
* @brief
*    The Address Resolution module does not set ar$tha to anything in particular, because it is 
*    this value that it is trying to determine. (Note: In this test TESTER sends an ARP Request
*    with ARP Target Hardware Address set to Ethernet Broadcast Address, and, expects that
*    DUT will send an ARP Response after receiving the ARP Request) 
*
* @return int : 0: DUT : Sends ARP Response  | 1: DUT : does not send ARP Response
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_17();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_18
*
***************************************************************************************************/
/**
* @brief
*    The Address Resolution module does not set ar$tha to anything in particular, because it is 
*    this value that it is trying to determine. (Note: In this test TESTER sends an ARP Request
     with ARP Target Hardware Address set to an arbitrary value, and, expects that DUT will
     send an ARP Response after receiving the ARP Request).
*
* @return int : 0: DUT : Sends ARP Response | 1: DUT : does not send ARP Response
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_18();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_19
*
***************************************************************************************************/
/**
* @brief
*    The Address Resolution module does not set ar$tha to anything in particular, because it is
*    this value that it is trying to determine. (Note: In this test TESTER sends an ARP Request
*    with ARP Target Hardware Address set to DUT MAC Address, Ethernet Destination Address
*    set to Ethernet Broadcast Address, and, expects that DUT will send an ARP Response after
*    receiving the ARP Request)
*
* @return int : 0: DUT : Sends ARP Response | 1: DUT : does not send ARP Response
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_19();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_20
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*    packet to the Address Resolution module which goes through an algorithm similar to the
*    following:Negative conditionals indicate an end of processing and a discarding of the packet
*    ?Do I have the hardware type in ar$hrd? (Note:In this test TESTER is configuring DUT to
*    clear its ARP Cache entries. TESTER then sends an ARP Request with hardware type field set
*    to Ethernet. All the other fields in the ARP Request Packet are set to their correct values. It
*   then expects that DUT should send an ARP Response.)  
*
* @return int : 0: DUT : Sends ARP Response | 1: DUT : does not send ARP Response
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_20();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_21
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*   packet to the Address Resolution module which goes through an algorithm similar to the
*   following:Negative conditionals indicate an end of processing and a discarding of the packet
*   ?Do I have the hardware type in ar$hrd? (Note:Here TESTER is sending correct values for all
*   the fields in the ARP Request packet except hardware type field and also TESTER is
*   configuring DUT to clear its ARP Cache entries.The hardware type field is set to an unknown
*   hardware type value, and TESTER expects that DUT will not send any ARP Response)
*
* @return int : 0: DUT : does not send ARP Response | 1: DUT : Sends ARP Response
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_21();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_22
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*    packet to the Address Resolution module which goes through an algorithm similar to the
*    following:Negative conditionals indicate an end of processing and a discarding of the packet
*    ?Do I have the hardware type in ar$hrd? (Note:In this test TESTER is configuring DUT to
*    clear its ARP Cache entries.TESTER then sends an ARP Response with hardware type field
*    set to an unknown hardware type value. All the other fields in the ARP Response Packet are
*    set to their correct values. TESTER then causes DUT to send an UDP Message and expects
*    that DUT will send an UDP Message)  
*
* @return int : 0: DUT : Sends ARP request | 1: DUT : does not send ARP request
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_22();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_26
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*    packet to the Address Resolution module which goes through an algorithm similar to the
*    following:Negative conditionals indicate an end of processing and a discarding of the packet
*    ?Do I speak the protocol in ar$pro? (Note:In this test TESTER is configuring DUT to clear its
*    ARP Cache entries. TESTER then sends an ARP Request with protocol type field set to type
*    IP. All the other fields in the ARP Request Packet are set to their correct values. It then
*    expects that DUT should send an ARP Response.)  
*
* @return int : 0: DUT : Sends ARP Response | 1: DUT : does not send ARP Response
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_26();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_27
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*    packet to the Address Resolution module which goes through an algorithm similar to the
*    following:Negative conditionals indicate an end of processing and a discarding of the packet
*    ?Do I speak the protocol in ar$pro? (Note:In this test TESTER is configuring DUT to clear its
*    ARP Cache entries. TESTER then sends an ARP Request with protocol type field set to an
*    unknown protocol type value. All the other fields in the ARP Request Packet are set to their
*    correct values. It then expects that DUT should NOT send an ARP Response.)
*
* @return int : 0: DUT : does not send ARP Response | 1: DUT : Sends ARP Response
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_27();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_28
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*    packet to the Address Resolution module which goes through an algorithm similar to the
*    following:Negative conditionals indicate an end of processing and a discarding of the packet
*   ?Do I speak the protocol in ar$pro? (Note:In this test TESTER is configuring DUT to clear its
*    ARP Cache entries.TESTER then sends an ARP Response with protocol type field set to
*    Unknown Protocol value. All the other fields in the ARP Response Packet are set to their
*    correct values. TESTER then causes DUT to send an UDP Message and expects that DUT will
*    send an ARP Request)  
*
* @return int : 0: DUT : Sends ARP Request | 1: DUT : does not send ARP Request
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_28();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_32
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*    packet to the Address Resolution module which goes through an algorithm similar to the
*    following. Negative conditionals indicate an end of processing and a discarding of the
*    packet. Merge_flag := false If the pair <protocol type, sender protocol address is already in
*    my translation table, update the ARP sender hardware address field of the entry with the
*    new information in the packet and set Merge_flag to true. (Note: Here TESTER sends an
*    ARP Request to DUT so that an entry gets added to the dut arp cache. TESTER then sends
*    another ARP Request with a different Ethernet source address to DUT to check if the
*    existing entry gets updated.) 
*
* @return int : 0: DUT : Sends ICMP Message | 1: DUT : Does not send ICMP Message
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_32();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_33
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*    packet to the Address Resolution module which goes through an algorithm similar to the
*    following. Negative conditionals indicate an end of processing and a discarding of the
*    packet. Merge_flag := false If the pair <protocol type, sender protocol address is already in
*    my translation table, update the ARP sender hardware address field of the entry with the
*    new information in the packet and set Merge_flag to true. (Note: Here TESTER sends an
*    ARP Response to DUT so that an entry gets added to the dut arp cache. TESTER then sends
*    another ARP Response with a different Ethernet source address to DUT to check if the
*    existing entry gets updated.) 
*
* @return int : 0: DUT : Sends ICMP Message | 1: DUT : Does not send ICMP Message
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_33();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_34
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*    packet to the Address Resolution module which goes through an algorithm similar to the
*    following. Negative conditionals indicate an end of processing and a discarding of the
*    packet. Merge_flag := false If the pair <protocol type, sender protocol address is already in
*    my translation table, update the ARP sender hardware address field of the entry with the
*    new information in the packet and set Merge_flag to true. (Note: Here TESTER sends an
*   ARP Request to DUT so that an entry gets added to the dut arp cache. TESTER then sends an
*    ARP Response with a different Ethernet source address to DUT to check if the existing entry
*    gets updated.)  
*
* @return int : 0: DUT : Sends ICMP Message | 1: DUT : Does not send ICMP Message
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_34();

/***************************************************************************************************
*
*   FUNCTION NAME: ARP_35
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*    packet to the Address Resolution module which goes through an algorithm similar to the
*    following. Negative conditionals indicate an end of processing and a discarding of the
*    packet. Merge_flag := false If the pair <protocol type, sender protocol address is already in
*    my translation table, update the ARP sender hardware address field of the entry with the
*    new information in the packet and set Merge_flag to true. (Note: Here TESTER sends an
*    ARP Response to DUT so that an entry gets added to the dut arp cache. TESTER then sends
*    an ARP Request with a different Ethernet source address to DUT to check if the existing
*    entry gets updated.)
*
* @return int : 0: DUT : Sends ICMP Message | 1: DUT : Does not send ICMP Message 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_35();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_36
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*    packet to the Address Resolution module which goes through an algorithm similar to the
*    following:Negative conditionals indicate an end of processing and a discarding of the packet
*    ?Am I the target protocol address? (Note: In this test TESTER is configuring DUT to clear its
*    ARP Cache entries. TESTER then sends an ARP Request with target protocol address field set
*    to <DIface-0-IP>. All the other fields in the ARP Request Packet are set to their correct
*    values. It then expects that DUT should send an ARP Response.)  
*
* @return int : 0: DUT : Sends ARP Response | 1: DUT : Does not send ARP Response
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_36();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_37
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*    packet to the Address Resolution module which goes through an algorithm similar to the
*    following:Negative conditionals indicate an end of processing and a discarding of the packet
*    ?Am I the target protocol address? (Note: In this test TESTER is configuring DUT to clear its
*   ARP Cache entries. TESTER then sends an ARP Request with target protocol address field set
*    to an IP First unused address value. All the other fields in the ARP Request Packet are set to
*    their correct values. It then expects that DUT should NOT send an ARP Response.) 
*
* @return int : 0: DUT : Does not send ARP Response | 1: DUT : Sends ARP Response
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_37();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_38
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*    packet to the Address Resolution module which goes through an algorithm similar to the
*    following:Negative conditionals indicate an end of processing and a discarding of the packet
*    ?Am I the target protocol address? (Note: In this test TESTER is configuring DUT to clear its
*    ARP Cache entries. TESTER then sends an ARP Response with target protocol address field
*    set to IP First unused address value. All the other fields in the ARP Response Packet are set
*    to their correct values. TESTER then causes DUT to send an UDP Message and expects that
*    DUT will send an ARP Request)  
*
* @return int : 0: DUT : Sends ARP Request | 1: DUT : Does not send ARP Request
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_38();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_39
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*    packet to the Address Resolution module which goes through an algorithm similar to the
*    following. Negative conditionals indicate an end of processing and a discarding of the
*    packet. If Merge_flag is false, add the triplet <protocol type, sender protocol address,
*    sender hardware address> to the translation table. (Note: In this test TESTER sends an ARP
*    Request with <sender protocol address, sender hardware address> fields set to <HOST-1-IP,
*    MAC-ADDR2>. TESTER then causes DUT to send an UDP Message and expects that DUT will
*    send an UDP Message with Ethernet Destination Address set to <MAC-ADDR2>)  
*
* @return int : 0: DUT : Sends ARP Request and Sends UDP Message | 1: DUT : Does not send ARP Request or Does not send UDP Message
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_39();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_40
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*    packet to the Address Resolution module which goes through an algorithm similar to the
*    following. Negative conditionals indicate an end of processing and a discarding of the
*    packet. If Merge_flag is false, add the triplet <protocol type, sender protocol address,
*    sender hardware address> to the translation table. (Note:In this test TESTER sends an ARP
*    Response with <sender protocol address, sender hardware address> fields set to <HOST-1-IP, MAC-ADDR3>.
*    TESTER then causes DUT to send an UDP Message and expects that DUT
*    will send an UDP Message with Ethernet Destination Address set to <MAC-ADDR3>)  
*
* @return int : 0: DUT : Sends ARP Request and Sends UDP Message | 1: DUT : Does not send ARP Request or Does not send UDP Message 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_40();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_41
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*    packet to the Address Resolution module which goes through an algorithm similar to the ollowing: 
*    ?Is the opcode ares_op$REQUEST? Yes:Set the ar$op field to ares_op$REPLY
*    (Note: In this test TESTER sends an ARP Request. All other fields in the ARP Request
*    message are set correctly. TESTER then expects that DUT will send an ARP Packet with
*    Operation code field set to Response)  
*
* @return int : 0: DUT : Sends ARP Response | 1: DUT : Does not sends ARP Response
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_41();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_42
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*    packet to the Address Resolution module which goes through an algorithm similar to the
*    following: Negative conditionals indicate an end of processing and a discarding of the
*    packet. ?Is the opcode ares_op$REQUEST? (Note:In this test TESTER sends an ARP Packet
*    with opcode field set to response value, and expects that DUT will not send any ARP
*    Response) 
*
* @return int : 0: DUT : Does not send ARP Response | 1: DUT : Send ARP Response
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_42();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_43
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the 
*    packet to the Address Resolution module which goes through an algorithm similar to the
*    following: ?Is the opcode ares_op$REQUEST? Swap hardware field, putting the local
*    hardware address in the sender field.  
*
* @return int : 0: DUT : Sends ARP Response and Ethernet Source Hardware Address is set to <DIFACE-0-MAC-ADDR>  | 1: DUT : Does not send ARP Response or Ethernet Source Hardware Address is  != <DIFACE-0-MAC-ADDR>
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_43();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_44
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the
*    packet to the Address Resolution module which goes through an algorithm similar to the following: 
*    ?Is the opcode ares_op$REQUEST? Swap protocol field, putting the local
*    protocol address in the sender field.
*
* @return int : 0: DUT : Sends ARP Response and Sender IP Address is set to <DIface-0-IP> | 1: DUT : Does not send ARP Response or Sender IP Address is != to <DIface-0-IP>
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_44();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_45
*
***************************************************************************************************/
/**
* @brief
*    When an address resolution packet is received, the receiving Ethernet module gives the 
*    packet to the Address Resolution module which goes through an algorithm similar to the
*    following: ?Is the opcode ares_op$REQUEST? Yes: Send the packet to the (new) ARP target
*    hardware address on the same hardware on which the request was received.
*
* @return int : 0: DUT : Sends ARP Response and ARP Target Hardware Address is set to <MAC-ADDR1> and Sends ARP Response and  ARP Target Hardware Address is set to <MAC-ADDR2>
                1: DUT : ARP Target Hardware Address is != to <MAC-ADDR1> or ARP Target Hardware Address is set to <MAC-ADDR2>.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_45();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_46
*
***************************************************************************************************/
/**
* @brief
*    For the 10Mbit Ethernet <ar$hrd> takes on the value <1> 
*
* @return int : 0: DUT : Sends ARP Response  and Hardware Type is set to ARP_HARDWARE_ETHERNET  | 1: DUT : Hardware Type is != to ARP_HARDWARE_ETHERNET
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_46();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_47
*
***************************************************************************************************/
/**
* @brief
*    For the 10Mbit Ethernet <ar$hln> takes on the value <6>  
*
* @return int : 0: DUT : Sends ARP Response and Hardware Address Length is set to <ETHERNET_ADDR_LEN>   | 1: DUT : Hardware Address Length is != to <ETHERNET_ADDR_LEN> 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_47();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_48
*
***************************************************************************************************/
/**
* @brief
*    If no packets are received from a host for a suitable length of time, the address 
*    resolution entry is forgotten. 
*
* @return int : 0: DUT : Sends ICMP Message and Sends ARP Request  | 1: DUT : Does not send ICMP Message or does not send ARP Request
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_48();
/***************************************************************************************************
*
*   FUNCTION NAME: ARP_49
*
***************************************************************************************************/
/**
* @brief
*    It may be desirable to have table aging and/or timeouts. (Note: In this test case TESTER
*    expects that DUT should delete a given Dynamic ARP Cache Entry even if it is being used.)
*
* @return int : 0: DUT : Sends UDP Message then Sends UDP Message then Sends ARP Request  | 1: DUT : Does not send ICMP Message or does not send ARP Request
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ARP_49();


#endif // PACKET_RECEPTION_H