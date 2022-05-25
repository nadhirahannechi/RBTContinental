#ifndef INVALID_ADDRESSES_H
#define INVALID_ADDRESSES_H
/***************************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_INVALID_ADDRESSES_01
 *   *
****************************************************************************************************************************************************************************************/
/**
* @brief
*   A UDP datagram received with an invalid IP source address (e.g., a broadcast or multicast address) must be discarded by UDP or by the IP layer (see Section 3.2.1.3). [Note: In this
     test, we verify UDP Message with multicast address as source address. ]
*
* @return int : 0: DUT: DUT discards the UDP packet | 1: DUT: DUT accept the UPD packet
*
* @note
* -
*
* @warning
* -
*
****************************************************************************************************************************************************************************************/
int UDP_INVALID_ADDRESSES_01();
/****************************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_INVALID_ADDRESSES_02
 *   *
*****************************************************************************************************************************************************************************************/
/**
* @brief
*   A UDP datagram received with an invalid IP source address (e.g., a broadcast or multicast address) must be discarded by UDP or by the IP layer (see Section 3.2.1.3). [Note: In this
     test, we verify UDP Message with broadcast address as source address. ]
*
* @return int : 0: DUT: DUT discards the UDP packet that contains data equal to <UDPDefaultData> | 1: DUT: DUT accept the UPD packet that contains UDP data equal to <UDPDefaultData>
*
* @note
* -
*
* @warning
* -
*
*****************************************************************************************************************************************************************************************/
int UDP_INVALID_ADDRESSES_02();

#endif //INVALID_ADDRESSES_H