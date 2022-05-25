#ifndef IPV4ADDRESSING_H
#define IPV4ADDRESSING_H

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
*   FUNCTION NAME: IPv4_ADDRESSING_01
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives an IPv4 packet containing an IPv4 Header containing a 
*  Destination Address indicating a value of Limited Broadcast, then the DUT accepts the IPv4 Packet.
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
int IPv4_ADDRESSING_01();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_ADDRESSING_02
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives an IPv4 packet containing an IPv4 Header containing a 
*  Destination Address indicating a value of Directed Broadcast, then the DUT discards the IPv4 
*  Packet silently.
*
* @return 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_ADDRESSING_02();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_ADDRESSING_03
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives an IPv4 packet containing an IPv4 Header containing a 
*  Destination Address indicating a value of Loop Back, then the DUT discards the IPv4 packet 
*  silently.
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
int IPv4_ADDRESSING_03();

#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus
#endif
