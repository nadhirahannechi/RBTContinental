/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   TestabilityProtocol_Intern.h
 *  @brief  AUTOSAR Base - Testability Protocol.
 *  \author uid1145 Mejdi Zayani
 *
 *  \addtogroup Testability_Protocol_package
 *  \{
 */
#ifndef TESTABILITYPROTOCOL_INTERN_H
#define TESTABILITYPROTOCOL_INTERN_H
/*==================================================================================================
*                                        Include Files
==================================================================================================*/
#include "TestabilityProtocolTypes.h"
#ifdef __cplusplus
extern "C"
{
#endif  /* #ifdef __cplusplus*/
/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
#ifndef FEATURE_TESTER_SIDE
#define TP_TRACE_DEBUG( format, ... )   printf( "\n TP: DUT: DEBUG:   %s(%d): " format, __FUNCTION__, __LINE__, ##__VA_ARGS__ )
#define TP_TRACE_INFO( format, ... )    printf( "\n TP: DUT: INFO:    %s(%d): " format, __FUNCTION__, __LINE__, ##__VA_ARGS__ )
#define TP_TRACE_WARNING( format, ... ) printf( "\n TP: DUT: WARNING: %s(%d): " format, __FUNCTION__, __LINE__, ##__VA_ARGS__ )
#define TP_TRACE_ERROR( format, ... )   printf( "\n TP: DUT: ERROR:   %s(%d): " format, __FUNCTION__, __LINE__, ##__VA_ARGS__ )
#else /* FEATURE_DUT_SIDE */
#define TP_TRACE_DEBUG( format, ... )   printf( "\n TP: TESTER: DEBUG:   %s(%d): " format, __FUNCTION__, __LINE__, ##__VA_ARGS__ )
#define TP_TRACE_INFO( format, ... )    printf( "\n TP: TESTER: INFO:    %s(%d): " format, __FUNCTION__, __LINE__, ##__VA_ARGS__ )
#define TP_TRACE_WARNING( format, ... ) printf( "\n TP: TESTER: WARNING: %s(%d): " format, __FUNCTION__, __LINE__, ##__VA_ARGS__ )
#define TP_TRACE_ERROR( format, ... )   printf( "\n TP: TESTER: ERROR:   %s(%d): " format, __FUNCTION__, __LINE__, ##__VA_ARGS__ )
#endif /* FEATURE_DUT_SIDE */

#define NLMSG_TAIL(nmsg)  ((struct rtattr *) (((void *) (nmsg)) + NLMSG_ALIGN((nmsg)->nlmsg_len)))
/*==================================================================================================
*                                             TYPES
==================================================================================================*/
struct inet_params {
	int local_port, rem_port, state, uid;
	union {
		struct sockaddr     sa;
		struct sockaddr_in  sin;
#if ENABLE_FEATURE_IPV6
		struct sockaddr_in6 sin6;
#endif
	} localaddr, remaddr;
	unsigned long rxq, txq, inode;
};
/*==================================================================================================
*                                        Global Variables
==================================================================================================*/

/*==================================================================================================
*                                     Local Functions prototypes
==================================================================================================*/

/*==================================================================================================
*                                     Global Functions prototypes
==================================================================================================*/


/***************************************************************************************************
*
*   FUNCTION NAME: TP_OpenControlChannel
*
***************************************************************************************************/
/**
* @brief
*  Open and initilialize the Communication Channel.  DUT<==Communication Channel==>TESTER
*
* @par
* @param[in] ipv4Address: Type ip4addr Contains the DUT/TESTER address
* @param[in] socketPort: Type uint16 Contains the DUT/TESTER port
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_OpenControlChannel(ip4addr ipv4Address, uint16 socketPort);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_CloseControlChannel
*
***************************************************************************************************/
/**
* @brief
*  Close the Communication Channel.  DUT<==Communication Channel==>TESTER
*
* @par
* @param[in] void
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_CloseControlChannel(void);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_ReceiveFrameCb
*
***************************************************************************************************/
/**
* @brief
*  Receive the TP message from the DUT/TESTER
*
* @par
* @param[in] void
*
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void* TP_ReceiveFrameCb(void* args);

/****************************************************************************************************
*
*   FUNCTION NAME: TP_SendFrame
*
***************************************************************************************************/
/**
* @brief
*  This function will send thoughout the Control Channel
*
* @par
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Contains testability Protocol message
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_SendFrame(TestabilityProtocolMessage_t *tpMessage);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DecodeReceivedMessage
*
***************************************************************************************************/
/**
* @brief
*  Decode and check the sanity of the received TP message
*
* @par
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Data to decode.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DecodeReceivedMessage(TestabilityProtocolMessage_t *tpMessage);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DecodeReceivedGroupID
*
***************************************************************************************************/
/**
* @brief
*  Call the requested SP from the received TP mesasge
*
* @par
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Data to decode.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DecodeReceivedGroupID(TestabilityProtocolMessage_t *tpMessage);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DecodeReceivedGroupGeneral
*
***************************************************************************************************/
/**
* @brief
*  Call the requested SP from the received TP mesasge
*
* @par
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Data to decode.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DecodeReceivedGroupGeanral(TestabilityProtocolMessage_t *tpMessage);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DecodeReceivedGroupUDP
*
***************************************************************************************************/
/**
* @brief
*  Call the requested SP from the received TP mesasge
*
* @par
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Data to decode.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DecodeReceivedGroupUDP(TestabilityProtocolMessage_t *tpMessage);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DecodeReceivedGroupTCP
*
***************************************************************************************************/
/**
* @brief
*  Call the requested SP from the received TP mesasge
*
* @par
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Data to decode.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DecodeReceivedGroupTCP(TestabilityProtocolMessage_t *tpMessage);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DecodeReceivedGroupICMPv4
*
***************************************************************************************************/
/**
* @brief
*  Call the requested SP from the received TP mesasge
*
* @par
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Data to decode.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DecodeReceivedGroupICMPv4(TestabilityProtocolMessage_t *tpMessage);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DecodeReceivedGroupICMPv6
*
***************************************************************************************************/
/**
* @brief
*  Call the requested SP from the received TP mesasge
*
* @par
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Data to decode.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DecodeReceivedGroupICMPv6(TestabilityProtocolMessage_t *tpMessage);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DecodeReceivedGroupIPv4
*
***************************************************************************************************/
/**
* @brief
*  Call the requested SP from the received TP mesasge
*
* @par
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Data to decode.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DecodeReceivedGroupIPv4(TestabilityProtocolMessage_t *tpMessage);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DecodeReceivedGroupIPv6
*
***************************************************************************************************/
/**
* @brief
*  Call the requested SP from the received TP mesasge
*
* @par
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Data to decode.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DecodeReceivedGroupIPv6(TestabilityProtocolMessage_t *tpMessage);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DecodeReceivedGroupDHCPv4
*
***************************************************************************************************/
/**
* @brief
*  Call the requested SP from the received TP mesasge
*
* @par
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Data to decode.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DecodeReceivedGroupDHCPv4(TestabilityProtocolMessage_t *tpMessage);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DecodeReceivedGroupDHCPv6
*
***************************************************************************************************/
/**
* @brief
*  Call the requested SP from the received TP mesasge
*
* @par
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Data to decode.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DecodeReceivedGroupDHCPv6(TestabilityProtocolMessage_t *tpMessage);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DecodeReceivedGroupARP
*
***************************************************************************************************/
/**
* @brief
*  Call the requested SP from the received TP mesasge
*
* @par
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Data to decode.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DecodeReceivedGroupARP(TestabilityProtocolMessage_t *tpMessage);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DecodeReceivedGroupEth
*
***************************************************************************************************/
/**
* @brief
*  Call the requested SP from the received TP mesasge
*
* @par
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Data to decode.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DecodeReceivedGroupEth(TestabilityProtocolMessage_t *tpMessage);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DecodeReceivedGroupPhy
*
***************************************************************************************************/
/**
* @brief
*  Call the requested SP from the received TP mesasge
*
* @par
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Data to decode.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DecodeReceivedGroupPhy(TestabilityProtocolMessage_t *tpMessage);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DecodeReceivedGroupCC
*
***************************************************************************************************/
/**
* @brief
*  Call the requested SP from the received TP mesasge
*
* @par
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Data to decode.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DecodeReceivedGroupCC(TestabilityProtocolMessage_t *tpMessage);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_Udp_ReceiveAndForwardSocket_thread
*
***************************************************************************************************/
/**
* @brief
*  Worker thread: will monitor the bytes received by each socket 
*
* @par
* @param[in] inputParam
*
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void*  TP_Udp_ReceiveAndForwardSocket_thread(void * inputParam);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_Tcp_ReceiveAndForwardSocket_thread
*
***************************************************************************************************/
/**
* @brief
*  Worker thread: will monitor the bytes received by each socket 
*
* @par
* @param[in] inputParam
*
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void*  TP_Tcp_ReceiveAndForwardSocket_thread(void * inputParam);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpListenAndAccept_event_thread
*
***************************************************************************************************/
/**
* @brief
*  Worker thread: will trigger a callback if a new client (socket) is connected to the server (socket) 
*
* @par
* @param[in] inputParam
*
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void*  TP_TcpListenAndAccept_event_thread(void * inputParam);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DecodeReturnCode
*
***************************************************************************************************/
/**
* @brief
*  Decode the generated error from errno type (linux based) to TP error codes
*
* @par
* @param[in] errorCode : Type int : Error code.
*
* @return TP_ResultID_t : Termination status of the executed function
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DecodeReturnCode(int errorCode);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_PrintArray
*
***************************************************************************************************/
/**
* @brief
*  Print a given array
*
* @par
* @param[in] buffer : Type uint8 : buffer.
* @param[in] bufferLength : Type uint32 : buffer length.
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void TP_PrintArray(uint8 *buffer, uint32 bufferLength);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_AddSocket
*
***************************************************************************************************/
/**
* @brief
*  Store the created socketId
*
* @par
* @param[in] socketId : Type uint16 : Socket ID
* @param[in] RecvFwdActive : Type boolean : used for forwarding
* @param[in] maxFwd : Type uint16 : Maximum length of payload to be forwarded per event.
* @param[in] maxLen : Type uint16 : Maximum count of bytes to receive over all.
* @param[in] maxLen : Type uint16 : Count of received and dropped bytes.
*
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void TP_AddSocket(uint16 socketId, boolean RecvFwdActive, uint16 dropCnt, uint16 maxFwd, uint16 maxLen);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DeleteSocket
*
***************************************************************************************************/
/**
* @brief
*  Delete a socket with a given socketId.
*
* @par
* @param[in] socketId : Type uint16 : Socket ID
*
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void TP_DeleteSocket(uint16 socketId);

/***************************************************************************************************
*
*   FUNCTION NAME: TP_FindSocket
*
***************************************************************************************************/
/**
* @brief
*  Delete a socket with a given socketId.
*
* @par
* @param[in] socketId : Type uint16 : Socket ID
*
* @return TP_socket_t*
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
struct TP_socket_t *TP_FindSocket(uint16 socketId);

/***************************************************************************************************
*
*   FUNCTION NAME: rtattr_add
*
***************************************************************************************************/
/**
* @brief
*  Add new data to rtattr.
*
* @par
* @param[in] nlmsghdr
* @param[in] maxlen
* @param[in] type
* @param[in] data
* @param[in] alen
*
* @return int status
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int rtattr_add(struct nlmsghdr *n, int maxlen, int type, const void *data, int alen);
/***************************************************************************************************
*
*   FUNCTION NAME: sendIcmpEchoRequest
*
***************************************************************************************************/
/**
* @brief
*  Send an ICMP echo request and wait for an ehco reply
*
* @par
* @param[in] ifName   : Type text
* @param[in] destAddr : Type ip4addr
* @param[in] sdata    : Type vint8
*
* @return 0 if success else error
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int sendIcmpEchoRequest(text ifName, ip4addr destAddr, vint8 sdata);

/***************************************************************************************************
*
*   FUNCTION NAME: sendIcmpEchoRequest
*
***************************************************************************************************/
/**
* @brief
*  Close all opened sockets
*
* @par
* @param[in] void
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void TP_cleanDependencies(void);

#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus*/
#endif /* TESTABILITYPROTOCOL_INTERN_H */

/** \}    end of addtogroup */