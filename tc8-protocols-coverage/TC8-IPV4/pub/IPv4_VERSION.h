#ifndef IPV4VERSION_H
#define IPV4VERSION_H

#ifdef __cplusplus
extern "C"
{
#endif  // #ifdef __cplusplus

/**************************************************************************************************/
/*                                                                                                */
/*                                         Prototypes                                             */
/*                                                                                                */
/**************************************************************************************************/

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_VERSION_01
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives an IPv4 packet containing an IPv4 Header containing a Version
*  indicating a value of 4, then the DUT accepts the IPv4 Packet.
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
int IPv4_VERSION_01();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_VERSION_03
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT generates an IPv4 packet then the DUT sends an IPv4 Packet containing an
*  IPv4 Header containing a Version indicating a value of 4.
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
int IPv4_VERSION_03();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_VERSION_04
*
***************************************************************************************************/
/**
* @brief
*  A datagram whose version number is not 4 MUST be silently discarded.
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
int IPv4_VERSION_04();

#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus
#endif