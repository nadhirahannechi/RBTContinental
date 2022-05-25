#ifndef IPV4FRAGMENTS_H
#define IPV4FRAGMENTS_H

#ifdef __cplusplus
extern "C"
{
#endif  // #ifdef __cplusplus

/**************************************************************************************************/
/*                                                                                                */
/*                                           Defines                                              */
/*                                                                                                */
/**************************************************************************************************/

#define INCORRECT_SOURCE_ADDRESS "192.168.121.200" // Used in IPv4_FRAGMENTS_03

/**************************************************************************************************/
/*                                                                                                */
/*                                         Prototypes                                             */
/*                                                                                                */
/**************************************************************************************************/

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_FRAGMENTS_01
*
***************************************************************************************************/
/**
* @brief
*  To assemble the fragments of an internet datagram, an internet protocol module (for example at a
*  destination host) combines internet datagrams that all have the same value for the four fields: 
*  identification, source, destination, and protocol.
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
int IPv4_FRAGMENTS_01();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_FRAGMENTS_02
*
***************************************************************************************************/
/**
* @brief
*  To assemble the fragments of an internet datagram, an internet protocol module (for example at a
*  destination host) combines internet datagrams that all have the same value for the four fields: 
*  identification, source, destination, and protocol.
*
* @return int 0: The test  was successful | 1: The test failed  
*
* @note
*  This test verifies that IP module does not assemble the fragments if identification is different.
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_FRAGMENTS_02();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_FRAGMENTS_03
*
***************************************************************************************************/
/**
* @brief
*  To assemble the fragments of an internet datagram, an internet protocol module (for example at a
*  destination host) combines internet datagrams that all have the same value for the four fields: 
*  identification, source, destination, and protocol.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
*  This test verifies that IP module does not assemble the fragments if source is different.
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_FRAGMENTS_03();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_FRAGMENTS_04
*
***************************************************************************************************/
/**
* @brief
*  To assemble the fragments of an internet datagram, an internet protocol module (for example at a
*  destination host) combines internet datagrams that all have the same value for the four fields: 
*  identification, source, destination, and protocol.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
*  This test verifies that IP module does not assemble the fragments if protocol is different.
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_FRAGMENTS_04();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_FRAGMENTS_05
*
***************************************************************************************************/
/**
* @brief
*  The fragmentation strategy is designed so than an unfragmented datagram has all zero fragmentation
*  information (MF = 0, fragment offset = 0).
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
int IPv4_FRAGMENTS_05();

#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus
#endif