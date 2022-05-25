#ifndef DATAGRAMLENGTH_H
#define DATAGRAMLENGTH_H


/***************************************************************************************************
*
*   FUNCTION NAME: UDP_DatagramLength_01
*
***************************************************************************************************/
/**
* @brief
*    Ensure that when the DUT receives a truncated UDP packet (a packet with the length field smaller
    than the actual size of the data coming from the Ethernet frame) then the DUT discards the UDP packet.
*
* @return int : 0: DUT :  The DUT discards the UDP packet | 1: DUT :  The DUT accepts the UDP packet
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int UDP_DatagramLength_01();



#endif //DATAGRAMLENGTH_H