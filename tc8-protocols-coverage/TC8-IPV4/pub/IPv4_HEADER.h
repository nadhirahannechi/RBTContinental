#ifndef IPV4HEADER_H
#define IPV4HEADER_H

#ifdef __cplusplus
extern "C"
{
#endif  // #ifdef __cplusplus

/**************************************************************************************************/
/*                                                                                                */
/*                                           Defines                                              */
/*                                                                                                */
/**************************************************************************************************/

#define INCORRECT_DESTINATION_ADDRESS "192.168.120.120" /* Used in IPv4_HEADER_04 */

/**************************************************************************************************/
/*                                                                                                */
/*                                         Prototypes                                             */
/*                                                                                                */
/**************************************************************************************************/

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_HEADER_01
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT is requested to generate an IPv4 packet, then the DUT generates an IPv4 
*  Packet containing an IPv4 Header containing a Total Length indicating a value greater than or 
*  equal to 20.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_HEADER_01();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_HEADER_02
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives an IPv4 packet containing an IPv4 Header containing a Header
*  Length indicating a value less than 20, then the DUT discards the IPv4 Packet silently.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_HEADER_02();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_HEADER_03
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT is requested to generate an IPv4 packet, then the DUT sends an IPv4 
*  Packet containing an IPv4 Header containing a Source Address indicating one of its defined IPv4 
*  addresses.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_HEADER_03();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_HEADER_04
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives an IPv4 packet containing an IPv4 Header containing a Destination
*  Address indicating a value different from the DUT’s IPv4 address and is not a Broadcast or Multicast
*  address, then the DUT discards the IPv4 Packet silently.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_HEADER_04();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_HEADER_05
*
***************************************************************************************************/
/**
* @brief
*  All hosts must be prepared to accept datagrams of up to 576 octets.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_HEADER_05();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_HEADER_08
*
***************************************************************************************************/
/**
* @brief
*  Internet Header Length is the length of the internet header in 32 bit words, and thus points to 
*  the beginning of the data. Note that the minimum value for a correct header is 5. 
*
* @return int 0: The test  was successful | 1: The test failed
*
* @note
*  Tests that DUT discards a packet with total length smaller than implied by IHL value.
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_HEADER_08();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_HEADER_09
*
***************************************************************************************************/
/**
* @brief
*  Total Length is the length of the datagram, measured in octets, including internet header and 
*  data.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
*  Tests that DUT discards a packet with total length bigger than the actual transmitted data.
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_HEADER_09();

#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus
#endif