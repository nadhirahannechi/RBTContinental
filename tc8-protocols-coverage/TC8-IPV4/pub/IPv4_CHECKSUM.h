#ifndef IPV4CHECKSUM_H
#define IPV4CHECKSUM_H

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
*   FUNCTION NAME: IPv4_CHECKSUM_02
*
***************************************************************************************************/
/**
* @brief
*  If the header checksum fails, the internet datagram is discarded at once by the entity which 
*  detects the error.
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
int IPv4_CHECKSUM_02();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_CHECKSUM_05
*
***************************************************************************************************/
/**
* @brief
*  The checksum field is the 16 bit one's complement of the one's complement sum of all 16 bit words
*  in the header. For purposes of computing the checksum, the value of the checksum field is zero.
*
* @return int 0: The test  was successful | 1: The test failed  
*
* @note
*  Here we send an Echo Request with checksum calculated according to rfc. DUT receives this Echo 
*  Request, verifies the Echo Request and then sends Echo Reply. We then verify that DUT uses the 
*  checksum calculation method according to rfc.
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_CHECKSUM_05();

#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus
#endif