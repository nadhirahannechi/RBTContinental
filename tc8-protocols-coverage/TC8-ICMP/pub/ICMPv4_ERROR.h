#ifndef ICMPV4ERROR_H
#define ICMPV4ERROR_H

#ifdef __cplusplus
extern "C"
{
#endif  // #ifdef __cplusplus

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_ERROR_02
*
***************************************************************************************************/
/**
* @brief
*  Also ICMP messages are only sent about errors in handling fragment zero of fragmented datagrams. 
*
* @return int 0: The test  was successful | 1: The test failed
*
* @note
*  This tests that ICMP error message is sent on receiving the fragment having Fragment 
*  Offset field set to zero. This test is ran when <DUTSupportsIPOptions> is TRUE.
*
* @warning
* -
*
***************************************************************************************************/
int ICMPv4_ERROR_02();

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_ERROR_03
*
***************************************************************************************************/
/**
* @brief
*  Also ICMP messages are only sent about errors in handling fragment zero of fragmented datagrams.
*
* @return int 0: The test  was successful | 1: The test failed
*
* @note
*  This tests that ICMP error message is not sent on receiving non zero fragment i.e Fragment
*  Offset field set to non-zero value. This test is ran when <DUTSupportsIPOptions> is TRUE.
*
* @warning
* -
*
***************************************************************************************************/
int ICMPv4_ERROR_03();

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_ERROR_04
*
***************************************************************************************************/
/**
* @brief
*  An ICMP error message MUST NOT be sent as the result of receiving a datagram destined to an IP
*  broadcast.
*
* @return int 0: The test  was successful | 1: The test failed
*
* @note
*  This test is ran when <DUTSupportsIPOptions> is TRUE.
*
* @warning
* -
*
***************************************************************************************************/
int ICMPv4_ERROR_04();

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_ERROR_05
*
***************************************************************************************************/
/**
* @brief
*  If an ICMP message of unknown type is received, it MUST be silently discarded.
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
int ICMPv4_ERROR_05();

#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus
#endif