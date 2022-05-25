#ifndef IPV4TTL_H
#define IPV4TTL_H

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
*   FUNCTION NAME: IPv4_TTL_01
*
***************************************************************************************************/
/**
* @brief
*  A host MUST NOT send a datagram with a Time-to-Live (TTL) value of zero.
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
int IPv4_TTL_01();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_TTL_05
*
***************************************************************************************************/
/**
* @brief
*  A host MUST NOT discard a datagram just because it was received with TTL less than 2.
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
int IPv4_TTL_05();

#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus
#endif
