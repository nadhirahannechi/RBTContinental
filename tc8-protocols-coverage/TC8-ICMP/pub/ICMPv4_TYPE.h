#ifndef ICMPV4TYPE_H
#define ICMPV4TYPE_H

#ifdef __cplusplus
extern "C"
{
#endif  // #ifdef __cplusplus

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_04
*
***************************************************************************************************/
/**
* @brief
*  If fragment zero is not available then no time exceeded need be sent at all.
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
int ICMPv4_TYPE_04();

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_05
*
***************************************************************************************************/
/**
* @brief
*  If the gateway or host processing a datagram finds a problem with the header parameters such 
*  that it cannot complete processing the datagram it must discard the datagram.
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
int ICMPv4_TYPE_05();

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_08
*
***************************************************************************************************/
/**
* @brief
*  The data received in the echo message must be returned in the echo reply message.
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
int ICMPv4_TYPE_08();

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_09
*
***************************************************************************************************/
/**
* @brief
*  The identifier and sequence number may be used by the echo sender to aid in matching the replies
*  with the echo requests.
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
int ICMPv4_TYPE_09();

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_10
*
***************************************************************************************************/
/**
* @brief
*  The checksum is the 16-bit one's complement of the one's complement sum of the ICMP message
*  starting with the ICMP Type. For computing the checksum, the checksum field should be zero.
*  If the total length is odd, the received data is padded with one octet of zeros for computing 
*  the checksum.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
*  This tests that a node does not send ICMP Echo Reply if ICMP Checksum is incorrect.
*
* @warning
* -
*
***************************************************************************************************/
int ICMPv4_TYPE_10();

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_11
*
***************************************************************************************************/
/**
* @brief
*  The data received (a timestamp) in the message is returned in the reply together with an 
*  additional timestamp.
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
int ICMPv4_TYPE_11();

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_12
*
***************************************************************************************************/
/**
* @brief
*  The identifier and sequence number may be used by the echo sender to aid in matching the replies
*  with the requests.
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
int ICMPv4_TYPE_12();

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_16
*
***************************************************************************************************/
/**
* @brief
*  A host SHOULD NOT implement these messages. 
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
*  Referring to Information Request or Information Reply Message.
*
* @warning
* -
*
***************************************************************************************************/
int ICMPv4_TYPE_16();

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_18
*
***************************************************************************************************/
/**
* @brief
*  A host must respond to an IPv4 Packet containnig an <unsupportedProtocol>, by sending a 
*  Destination Unreachable message including the Protcol Unreachable code.
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
int ICMPv4_TYPE_18();

/***************************************************************************************************
*
*   FUNCTION NAME: ICMPv4_TYPE_22
*
***************************************************************************************************/
/**
* @brief
*  A host must respond to all ICMP Echo Requests sent to it, by sending an ICMP Echo Reply back to
*  the sender of ICMP Echo Request.
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
int ICMPv4_TYPE_22();

#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus
#endif