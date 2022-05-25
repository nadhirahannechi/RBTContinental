/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   TestabilityProtocol.c
 *  @brief  AUTOSAR Base - Testability Protocol.
 *  \author uid1145 Mejdi Zayani
 *
 *  \addtogroup Testability_Protocol_package
 *  \{
 */

/*==================================================================================================
*                                        Include Files
==================================================================================================*/
#include "TestabilityProtocol_api.h"
#include "TestabilityProtocol_cbTypes.h"
#include "TestabilityProtocol_Intern.h"
/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
#define TP_MAJOR_VERSION 0x01U /*The current Testability Protocol version is TC1.2.0. */
#define TP_MINOR_VERSION 0x02U /*The current Testability Protocol version is TC1.2.0. */
#define TP_PATCH_VERSION 0x00U /*The current Testability Protocol version is TC1.2.0. */
#define TP_VERSION_SIZE 0x03U

#define TP_MESSAGE_LENGTH_WITHOUT_PAYLOAD 0x10u
#define ZERO_VALUE 0x00U
#define TP_PAYLOAD_SIZE_BRUT 0x08U
#define SIZE_OF_BOM_NULL_TERMINATION 0x04U
#define BOM_CODE 0xEFBBBF
#define BYTE_ORDER_MARK_B 0xEFU
#define BYTE_ORDER_MARK_O 0xBBU
#define BYTE_ORDER_MARK_M 0xBFU

#define ERROR_RETURN_CODE (-1)
#define SEREVR_PORT 56000
#define PORT_ANY 0xFFFFU

/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/*==================================================================================================
*                                          Global Variables
==================================================================================================*/

/* Dynamic array for the created threads */
extern TP_threadSocket_t threadSocket;
extern struct TP_socket_t *headSocketId_t;
/*==================================================================================================
*                                          Local Functions
==================================================================================================*/

/*==================================================================================================
*                                          Global Functions
==================================================================================================*/

/* Genaral Group cb function */
extern General_cb_t General_cb_func;
/* UDP Group cb function */
extern Udp_cb_t Udp_cb_func;
/* TCP Group cb function */
extern Tcp_cb_t Tcp_cb_func;
/* IPv4 Group cb function */
extern Ipv4_cb_t IPv4_cb_func;
/* Dhcpv4 Group cb function */
extern Dhcpv4_cb_t Dhcpv4_cb_func;
/* Dhcpv6 Group cb function */
extern Dhcpv6_cb_t Dhcpv6_cb_func;
/* ETH Group cb function */
extern Eth_cb_t Eth_cb_func;
/* aRP Group cb function */
extern Arp_cb_t Arp_cb_func;
/* ICMP Group cb function */
extern Icmp_cb_t Icmp_cb_func;

/* Global variable used to exit all threads */
extern boolean CancelThreadStatus;
/***************************************************************************************************
*
*   FUNCTION NAME: TP_GenaralGetVersion
*
***************************************************************************************************/
/**
* @brief
*  This SP will return the testability protocol version of the used protocol
*  and service primitive implementation. The testability protocol version is
*  bound to the TC release version the protocol is based on. The current version is TC1.2.0.
*
* @par
* @param[in] Function pointer to user defined callback.
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
TP_ResultID_t TP_GenaralGetVersion(void (*TP_generalGetVersion_cb)(TP_ResultID_t, uint16, uint16, uint16))
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        General_cb_func.generalGetVersion_cb = TP_generalGetVersion_cb;
        TP_TRACE_DEBUG("Construct the TP message");
        /* Construct the TP message */
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_GENERAL;
        tpMessage->Pid = PID_GENERAL_GET_VERSION;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size = 8 + 0  */
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;
        /* Result ID */
        tpMessage->Rid = RID_E_OK;
        if (TP_generalGetVersion_cb != NULL_PTR)
        {
            General_cb_func.generalGetVersion_cb = TP_generalGetVersion_cb;
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_GENERAL_GET_VERSION");
        }
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_generalGetVersion_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_GENERAL;
        tpMessage->Pid = PID_GENERAL_GET_VERSION;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc(TP_VERSION_SIZE * sizeof(uint16), sizeof(uint16));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + TP_VERSION_SIZE * sizeof(uint16); /* 8 + payload size */

            /* TP Major Version */
            tpMessage->Payload[0] = ZERO_VALUE;
            tpMessage->Payload[1] = TP_MAJOR_VERSION;
            /* TP Minor Version */
            tpMessage->Payload[2] = ZERO_VALUE;
            tpMessage->Payload[3] = TP_MINOR_VERSION;
            /* TP Patch Verison */
            tpMessage->Payload[4] = ZERO_VALUE;
            tpMessage->Payload[5] = TP_PATCH_VERSION;

            /* Result ID */
            tpMessage->Rid = RID_E_OK;

            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
        }
        else
        {
            /* Can't allocate memory */
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size */
            /* Result ID */
            tpMessage->Rid = RID_E_ISB;

            /* send_message_without_payload() */
            Result = TP_SendFrame(tpMessage);
        }
        free(tpMessage);
    }
    else
    {
        Result = RID_E_NOK;
    }

#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_GenaralStartTest
*
***************************************************************************************************/
/**
* @brief
*  The purpose of this SP is to have a defined entry tag in trace at the point
*  in time the test case was started. This SP does not have any request parameters.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Contains Testability Protocol message
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
TP_ResultID_t TP_GenaralStartTest(void (*TP_GenaralStartTest_cb)(TP_ResultID_t))
{
    TP_ResultID_t Result = E_NOT_OK;
    /* Log the starting time of test suites */
    time_t now;
    char *systemTime = ctime(&now);
    if (systemTime != NULL_PTR)
    {
        TP_TRACE_DEBUG("Starting time is : %s", ctime(&now));
    }
    CancelThreadStatus = FALSE;
    /* Remove all registred CBs */
    General_cb_func = (General_cb_t){NULL_PTR};
    Udp_cb_func = (Udp_cb_t){NULL_PTR};
    Tcp_cb_func = (Tcp_cb_t){NULL_PTR};
    IPv4_cb_func = (Ipv4_cb_t){NULL_PTR};
    Dhcpv4_cb_func = (Dhcpv4_cb_t){NULL_PTR};
    Dhcpv6_cb_func = (Dhcpv6_cb_t){NULL_PTR};
    Arp_cb_func = (Arp_cb_t){NULL_PTR};
    Eth_cb_func = (Eth_cb_t){NULL_PTR};
    Icmp_cb_func = (Icmp_cb_t){NULL_PTR};
#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_GENERAL;
        tpMessage->Pid = PID_GENERAL_START_TEST;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size = 8 + 0  */
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;
        /* Result ID */
        tpMessage->Rid = RID_E_OK;
        /* Register the user callback */
        if (TP_GenaralStartTest_cb != NULL_PTR)
        {
            General_cb_func.GenaralStartTest_cb = TP_GenaralStartTest_cb;
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_GENERAL_START_TEST");
        }
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_GenaralStartTest_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_GENERAL;
        tpMessage->Pid = PID_GENERAL_START_TEST;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size = 8 + 0  */
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;
        /* Initialize sockets */
        while (headSocketId_t != NULL_PTR)
        {
            Result |= shutdown(headSocketId_t->socketId, SHUT_RDWR);
            Result |= close(headSocketId_t->socketId);
            pthread_cancel(headSocketId_t->pthread_id);
            TP_DeleteSocket(headSocketId_t->socketId);
        }
        headSocketId_t = NULL_PTR;
        /* Initialize threads */
        while (threadSocket.numOfThread > 0)
        {
            pthread_cancel(threadSocket.pthread_id[threadSocket.numOfThread - 1]);
            threadSocket.numOfThread--;
        }
        threadSocket.pthread_id = NULL_PTR;
        /* Result ID */
        tpMessage->Rid = RID_E_OK;
        Result = TP_SendFrame(tpMessage);
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_ISB;
    }

#endif /* FEATURE_TESTER_SIDE */

    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_GenaralEndTest
*
***************************************************************************************************/
/**
* @brief
*  The purpose of this SP is to reset the Upper Tester. All sockets of the test channel will be closed,
*  counters are set to the default value, buffers are cleared and active service primitives will be terminated.
*  Another purpose of this SP is to have a defined entry tag in trace at the point in time the test case was stopped.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] tpMessage : Type TestabilityProtocolMessage_t : Contains Testability Protocol message
* @param[in] tcId : Type uint16 : The test case ID going to be terminated.
* @param[in] tsName : Type text : The test suite name.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note The parameters (tcId and tsName) may be ignored by the testability module.
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_GenaralEndTest(void (*TP_GenaralEndTest_cb)(TP_ResultID_t), uint16 tcId, text tsName)
{
    TP_ResultID_t Result = E_NOT_OK;
    /* Log the Ending time of test suites */
    time_t now;
#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_GENERAL;
        tpMessage->Pid = PID_GENERAL_END_TEST;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;
        /* Result ID */
        tpMessage->Rid = RID_E_OK;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc(sizeof(uint16) + sizeof(uint16) + tsName.dataLength, sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + sizeof(uint16) + sizeof(uint16) + tsName.dataLength; /* 8 + payload size */

            /* Copy tcId */
            memcpy(&tpMessage->Payload[0], &tcId, sizeof(uint16));
            /* Copy tsName dataLength */
            memcpy(&tpMessage->Payload[2], &tsName.dataLength, sizeof(uint16));
            /*  Copy tsName Data */
            memcpy(&tpMessage->Payload[4], tsName.Data, tsName.dataLength);

            /* Register the user callback */
            if (TP_GenaralEndTest_cb != NULL_PTR)
            {
                General_cb_func.GenaralEndTest_cb = TP_GenaralEndTest_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_GENERAL_END_TEST");
            }
            time(&now);
            char * systemTime = ctime(&now);
            if (systemTime != NULL_PTR)
            {
                TP_TRACE_INFO("Test case ended: Test case Id = %d, Name = %s, Ending time is = %s", tcId, tsName.Data, ctime(&now));
            }
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
            CancelThreadStatus = TRUE;
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_ISB;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_ISB;
    }

#else /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_GenaralEndTest_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_GENERAL;
        tpMessage->Pid = PID_GENERAL_END_TEST;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size = 8 + 0  */
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;
        /* Cancel all threads */
        CancelThreadStatus = TRUE;
        /* Close all opened threads */
        while (threadSocket.numOfThread > 0)
        {
            pthread_cancel(threadSocket.pthread_id[threadSocket.numOfThread -1]);
            threadSocket.numOfThread--;
        }
        threadSocket.pthread_id = NULL_PTR;
        /* Close all opened sockets */
        struct linger l;
        l.l_onoff = 1;
        l.l_linger = 0;
        while (headSocketId_t != NULL_PTR)
        {
            setsockopt(headSocketId_t->socketId, SOL_SOCKET, SO_LINGER, &l, sizeof(l));
            Result |= close(headSocketId_t->socketId);
            pthread_cancel(headSocketId_t->pthread_id);
            TP_DeleteSocket(headSocketId_t->socketId);
        }
        headSocketId_t = NULL_PTR;
        /* Result ID */
        tpMessage->Rid = RID_E_OK;
        time(&now);
        char *systemTime = ctime(&now);
        if (systemTime != NULL_PTR)
        {
            TP_TRACE_INFO("Test case ended: Test case Id = %d, Name = %s, Ending time is = %s", tcId, tsName.Data, ctime(&now));
        }
        Result |= TP_SendFrame(tpMessage);
        free(tpMessage);

        /* Remove all registred CBs */
        General_cb_func = (General_cb_t){NULL_PTR};
        Udp_cb_func = (Udp_cb_t){NULL_PTR};
        Tcp_cb_func = (Tcp_cb_t){NULL_PTR};
        IPv4_cb_func = (Ipv4_cb_t){NULL_PTR};
        Arp_cb_func = (Arp_cb_t){NULL_PTR};
        Eth_cb_func = (Eth_cb_t){NULL_PTR};
        Icmp_cb_func = (Icmp_cb_t){NULL_PTR};
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_ISB;
    }

#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_UdpCloseSocket
*
***************************************************************************************************/
/**
* @brief
*  Closes a socket.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : Socket that should be closed.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_UdpCloseSocket(void (*TP_UdpCloseSocket_cb)(TP_ResultID_t), uint16 socketId)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_UDP;
        tpMessage->Pid = PID_UDP_CLOSE_SOCKET;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((sizeof(socketId)), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + sizeof(socketId); /* 8 + payload size */

            /* Copy socket ID */
            memcpy(&tpMessage->Payload[0], &socketId, sizeof(uint16));
            /* Result ID */
            tpMessage->Rid = RID_E_OK;
            /* Register the user callback */
            if (TP_UdpCloseSocket_cb != NULL_PTR)
            {
                Udp_cb_func.UdpCloseSocket_cb = TP_UdpCloseSocket_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_UDP_CLOSE_SOCKET");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_UdpCloseSocket_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_UDP;
        tpMessage->Pid = PID_UDP_CLOSE_SOCKET;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        /* Close the requested Socket */
        TP_socket_t *tempSocketNode = TP_FindSocket(socketId);
        if (tempSocketNode != NULL_PTR)
        {
            /* Close immediatlly the socket */
            if (close(socketId) == ERROR_RETURN_CODE)
            {
                TP_TRACE_ERROR("Failed to close socket, error code= %d", errno);
                tpMessage->Rid = TP_DecodeReturnCode(errno);
            }
            else
            {
                /* Socket closed successfully */
                /* Cancel its thread */
                if (pthread_cancel(tempSocketNode->pthread_id) != ZERO_VALUE)
                {
                    TP_TRACE_ERROR("Failed to cancel thread id = %lu, socket = %d, error code = %d", tempSocketNode->pthread_id, socketId, errno);
                }
                /* remove the socketId from the list */
                TP_DeleteSocket(socketId);
                tpMessage->Rid = RID_E_OK;
            }
        }
        else
        {
            tpMessage->Rid = RID_E_ISD;
        }
        Result = TP_SendFrame(tpMessage);
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_ISB;
    }

#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_UdpCreateAndBind
*
***************************************************************************************************/
/**
* @brief
*  Creates a socket and optionally binds this socket to a port and a local IP address.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] doBind : Type boolean : true: bind will be performed | false: no bind will be performed.
* @param[in] localPort : Type uint16 : Local port to bind (0xFFFF: PORT_ANY)
* @param[in] localAddr : Type ipxaddr : Local address (n=4:IPv4 or n=16:IPv6)
*                        Any IP: If all address bytes are zero The domain is selected by the type of address
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_UdpCreateAndBind(void (*TP_UdpCreateAndBind_cb)(TP_ResultID_t, uint16), boolean doBind, uint16 localPort, ip4addr localAddr)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_UDP;
        tpMessage->Pid = PID_UDP_CREATE_AND_BIND;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = calloc((sizeof(doBind) + sizeof(localPort) + sizeof(localAddr.dataLength) + localAddr.dataLength), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (sizeof(doBind) + sizeof(localPort) + sizeof(localAddr.dataLength) + localAddr.dataLength); /* 8 + payload size */

            /* Copy doBind parameter */
            tpMessage->Payload[0] = doBind;
            /* Copy localPort parameter */
            memcpy(&tpMessage->Payload[1], &localPort, sizeof(uint16));
            /* Copy localAddr dataLength parameter */
            memcpy(&tpMessage->Payload[3], &localAddr.dataLength, sizeof(uint16));
            /* Copy localAddr Data parameter */
            memcpy(&tpMessage->Payload[5], localAddr.Data, localAddr.dataLength);

            /* Result ID */
            tpMessage->Rid = RID_E_OK;

            /* Register the user callback */
            if (TP_UdpCreateAndBind_cb != NULL_PTR)
            {
                Udp_cb_func.UdpCreateAndBind_cb = TP_UdpCreateAndBind_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_UDP_CREATE_AND_BIND");
            }

            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_ISB;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_ISB;
    }

#else  /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_UdpCreateAndBind_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_UDP;
        tpMessage->Pid = PID_UDP_CREATE_AND_BIND;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        /* Create the Requested Socket */

        struct sockaddr_in serverAddr = {ZERO_VALUE};

        int socketId = ERROR_RETURN_CODE;

        if (localAddr.dataLength == IPV4_ADDRESS_SIZE)
        {
            socketId = socket(PF_INET, SOCK_DGRAM, ZERO_VALUE);

            if (socketId == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = TP_DecodeReturnCode(errno);
            }
            else
            {
                TP_TRACE_DEBUG("New UDP Socket is created socketId = %d", socketId);
                /* save the created socketId */
                TP_AddSocket(socketId, FALSE, ZERO_VALUE, ZERO_VALUE, ZERO_VALUE);

                TP_socket_t *tempSocketNode = TP_FindSocket(socketId);
                if (tempSocketNode != NULL_PTR)
                {
                    threadSocket.numOfThread++;
                    threadSocket.pthread_id = (pthread_t *)realloc(threadSocket.pthread_id, threadSocket.numOfThread * sizeof(pthread_t) * sizeof(uint8));
                    int *socketIdthread = malloc(sizeof(socketId));
                    if ((socketIdthread != NULL_PTR) && (threadSocket.pthread_id != NULL_PTR))
                    {
                        *socketIdthread = socketId;
                        if (pthread_create(&threadSocket.pthread_id[threadSocket.numOfThread - 1], NULL, TP_Udp_ReceiveAndForwardSocket_thread, socketIdthread) == ZERO_VALUE)
                        {
                            /* Thread created successfully */
                            TP_TRACE_DEBUG("thread TP_Udp_ReceiveAndForwardSocket_thread created successfully, socketId = %d", socketId);
                            /* Store the threadId related to the created socket */
                            tempSocketNode->pthread_id = threadSocket.pthread_id[threadSocket.numOfThread - 1];

                            tpMessage->Rid = RID_E_OK;
                        }
                        else
                        {
                            TP_TRACE_ERROR("cannot create thread TP_Udp_ReceiveAndForwardSocket_thread, socketId = %d", socketId);
                            tpMessage->Rid = RID_E_NOK;
                        }
                        //free(socketIdthread);
                        /* Bind the socket to a given IP addr and port */
                        if (doBind == TRUE)
                        {
                            /* assign IP, PORT */
                            serverAddr.sin_family = AF_INET;
                            serverAddr.sin_addr.s_addr = localAddr.Data[0] + (localAddr.Data[1] << 8) + (localAddr.Data[2] << 16) + (localAddr.Data[3] << 24);
                            serverAddr.sin_port = htons(localPort);

                            /* Binding newly created socket to given IP and verification */
                            if ((bind(socketId, (struct sockaddr *)&serverAddr, sizeof(serverAddr))) != 0)
                            {
                                TP_TRACE_ERROR("Failed to bind socket, error code = %d", errno);
                                tpMessage->Rid = TP_DecodeReturnCode(errno);
                            }
                            else
                            {
                                TP_TRACE_DEBUG("bind socket sucessfully, socketId = %d", socketId);
                                tpMessage->Rid = RID_E_OK;
                            }
                        }
                        else
                        {
                            tpMessage->Rid = RID_E_OK;
                        }
                    }
                    else
                    {
                        TP_TRACE_ERROR("Cannot create a thread to handle the received data on the newly created socket");
                        tpMessage->Rid = RID_E_NOK;
                    }
                }
                else
                {
                    TP_TRACE_ERROR("Cannot find the socketId in the global linked list, socketId = %d", socketId);
                    tpMessage->Rid = RID_E_ISD;
                }
            }
        }
        else if (localAddr.dataLength == IPV6_ADDRESS_SIZE)
        {
            /* Convert the given localAddr
            char ipAddr[64] = {ZERO_VALUE};
            sprintf(ipAddr, "%d.%d.%d.%d", *localAddr.ipv4_Addr.Data, localAddr.ipv4_Addr.Data[1], localAddr.ipv4_Addr.Data[2], localAddr.ipv4_Addr.Data[3]);
            // assign IP, PORT
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_addr.s_addr = htonl(ipAddr);
            serverAddr.sin_port = htons(localPort)
            // Binding newly created socket to given IP and verification
            if ((bind(socketId, (struct sockaddr *)&serverAddr, sizeof(serverAddr))) != 0)
            {
                tpMessage->Rid = TP_DecodeReturnCode(errno);
            }
            else
            {
                tpMessage->Rid = RID_E_OK;
            } */
        }
        else
        {
            tpMessage->Rid = RID_E_NOK;
        }
        /* Send the SocketId to the tester */
        if (tpMessage->Rid == RID_E_OK)
        {

            /* Construct the payload */
            tpMessage->Payload = (uint8 *)calloc(sizeof(uint16), sizeof(uint8));

            if (tpMessage->Payload != NULL_PTR)
            {

                tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + sizeof(uint16); /* 8 + payload size */

                /* Copy socketId response */
                memcpy(&tpMessage->Payload[0], &socketId, sizeof(uint16));

                /* send_message_with_payload() */
                Result = TP_SendFrame(tpMessage);
                free(tpMessage->Payload);
            }
            else
            {
                /* Can't allocate memory */
                tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size */
                /* Result ID */
                tpMessage->Rid = RID_E_ISB;

                /* send_message_without_payload() */
                Result = TP_SendFrame(tpMessage);
            }
        }
        else
        {
            /* send_message_without_payload() */
            Result = TP_SendFrame(tpMessage);
        }
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }
    Result = tpMessage->Rid;
#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_UdpSendData
*
***************************************************************************************************/
/**
* @brief
*  Sends data to a target.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : Local socket used to perform the transmission.
* @param[in] totalLen : Type uint16 : Total length: repeat data up to that length (in bytes).
*                       In case the value of totalLen is smaller than the length of data,
*                       the full length of data will be transmitted.
* @param[in] destPort : Type uint16 : Destination port.
* @param[in] destAddr : Type destAddr : Destination address.
* @param[in] data : Type vint8 : Data to transmit.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_UdpSendData(void (*TP_UdpSendData_cb)(TP_ResultID_t), uint16 socketId, uint16 totalLen, uint16 destPort, ip4addr destAddr,
                             vint8 data)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_UDP;
        tpMessage->Pid = PID_UDP_SEND_DATA;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;
        /* Construct the payload */
        tpMessage->Payload = calloc((sizeof(socketId) + sizeof(totalLen) + sizeof(destPort) + sizeof(destAddr.dataLength) +
                                     sizeof(data.dataLength) + destAddr.dataLength + data.dataLength),
                                    sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (sizeof(socketId) + sizeof(totalLen) + sizeof(destPort) + sizeof(destAddr.dataLength) +
                                                        sizeof(data.dataLength) + destAddr.dataLength + data.dataLength); /* 8 + payload size */

            /* Copy SocketId parameter */
            memcpy(&tpMessage->Payload[0], &socketId, sizeof(uint16));
            /* Copy totalLen parameter */
            memcpy(&tpMessage->Payload[2], &totalLen, sizeof(uint16));
            /* Copy destPort parameter */
            memcpy(&tpMessage->Payload[4], &destPort, sizeof(uint16));
            /* Copy destAddr dataLength parameter */
            memcpy(&tpMessage->Payload[6], &destAddr.dataLength, sizeof(uint16));
            /* Copy destAddr Data parameter */
            memcpy(&tpMessage->Payload[8], destAddr.Data, destAddr.dataLength);
            /* Copy data dataLength parameter */
            memcpy(&tpMessage->Payload[8 + destAddr.dataLength], &data.dataLength, sizeof(uint16));
            /* Copy data Data parameter */
            memcpy(&tpMessage->Payload[10 + destAddr.dataLength], data.Data, data.dataLength);

            /* Result ID */
            tpMessage->Rid = RID_E_OK;

            /* Register the user callback */
            if (TP_UdpSendData_cb != NULL_PTR)
            {
                Udp_cb_func.UdpSendData_cb = TP_UdpSendData_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_UDP_SEND_DATA");
            }

            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_UdpSendData_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_UDP;
        tpMessage->Pid = PID_UDP_SEND_DATA;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        /* Create the server addr */
        struct sockaddr_in serverAddr = {ZERO_VALUE};

        /* assign IP, PORT */
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = destAddr.Data[0] + (destAddr.Data[1] << 8) + (destAddr.Data[2] << 16) + (destAddr.Data[3] << 24);
        serverAddr.sin_port = htons(destPort);

        /* In case the value of totalLen is smaller than the length of data, the full length of data will be transmitted */
        int bytesSent = ZERO_VALUE;
        if (data.dataLength >= totalLen)
        {
            /* send the data */
            bytesSent = sendto(socketId, data.Data, data.dataLength, ZERO_VALUE, (const struct sockaddr *)&serverAddr, sizeof(serverAddr));
            /* Check if the Data is sent successfully */
            if (bytesSent != data.dataLength)
            {
                tpMessage->Rid = RID_E_NOK;
                TP_TRACE_ERROR("Could not send data bytesSent = %d, data.dataLength = %d, socketId = %d", bytesSent, data.dataLength, socketId);
            }
            else
            {
                tpMessage->Rid = RID_E_OK;
                TP_TRACE_ERROR("send data bytesSent = %d, data.dataLength = %d, socketId = %d", bytesSent, data.dataLength, socketId);
            }
        }
        else
        {
            /* Repeat data up to that totalLen (in bytes) */
            vint8 payloadToTranmit;
            int dataTransmited = ZERO_VALUE;
            payloadToTranmit.dataLength = totalLen;
            payloadToTranmit.Data = calloc(totalLen, sizeof(uint8));

            if (payloadToTranmit.Data != NULL_PTR)
            {
                /* Construct the Payload */

                while (((dataTransmited + data.dataLength) < totalLen) && (CancelThreadStatus == FALSE))
                {
                    memcpy(payloadToTranmit.Data + dataTransmited, data.Data, data.dataLength);
                    dataTransmited += data.dataLength;
                }
                /* Copy the remaining data */
                memcpy(payloadToTranmit.Data + dataTransmited, data.Data, (totalLen - dataTransmited));
                /* send the data */
                bytesSent = sendto(socketId, payloadToTranmit.Data, totalLen, ZERO_VALUE, (const struct sockaddr *)&serverAddr, sizeof(serverAddr));
                /* Check if the Data is sent successfully */
                if (bytesSent != totalLen)
                {
                    tpMessage->Rid = RID_E_NOK;
                    TP_TRACE_ERROR("Could not send data bytesSent = %d, data.dataLength = %d, socketId = %d", bytesSent, data.dataLength, socketId);
                }
                else
                {
                    tpMessage->Rid = RID_E_OK;
                    TP_TRACE_ERROR("send data bytesSent = %d, data.dataLength = %d, socketId = %d", bytesSent, data.dataLength, socketId);
                }
            }
            else
            {
                /* Can't allocate memory */
                tpMessage->Rid = RID_E_NOK;
            }
        }
        Result = TP_SendFrame(tpMessage);
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_UdpReceiveAndForward
*
***************************************************************************************************/
/**
* @brief
*  Data that will be received after the call of this SP will be forwarded to the test system.
*  The amount of forwarded data per received datagram (UDP) or bulk of stream data (TCP) can be limited using maxFwd.
*  The original length of this data unit can be obtained by fullLen. The process will repeat itself (active phase)
*  until the maximum amount of data defined by maxLen was received or END_TEST was called (inactive phase).
*  UDP: No further requirements. (see 6.12.2 UDP Receive and Count)
*  TCP: In the inactive phase (e.g. prior the first call) all data received will be discarded or ignored.
*       When called all data that was received on the specified socked prior the call of this SP will be consumed
*  in order to open the TCP receive window. All data that is received during the active phase of this SP will be consumed
*  up to the maximum amount of data defined by maxLen. (see 6.12.4 TCP Client Receive and Forward).
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] Function pointer to user defined callback to receive an event.
* @param[in] socketId : Type uint16 : The Socket selected for forwarding.
* @param[in] maxFwd : Type uint16 : Maximum length of payload to be forwarded per event.
* @param[in] maxLen : Type uint16 : Maximum count of bytes to receive over all (0xFFFF: limitless)

*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_UdpReceiveAndForward(void (*TP_UdpReceiveAndForward_cb)(TP_ResultID_t, uint16), void (*TP_UdpReceiveAndForward_event_cb)(TP_ResultID_t, uint16, uint16, ip4addr, vint8), uint16 socketId, uint16 maxFwd, uint16 maxLen)
{

    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_UDP;
        tpMessage->Pid = PID_UDP_RECEIVE_AND_FORWARD;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = calloc((sizeof(socketId) + sizeof(maxFwd) + sizeof(maxLen)), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (sizeof(socketId) + sizeof(maxFwd) + sizeof(maxLen)); /* 8 + payload size */

            /* Copy SocketId parameter */
            memcpy(&tpMessage->Payload[0], &socketId, sizeof(uint16));
            /* Copy maxFwd parameter */
            memcpy(&tpMessage->Payload[2], &maxFwd, sizeof(uint16));
            /* Copy maxLen parameter */
            memcpy(&tpMessage->Payload[4], &maxLen, sizeof(uint16));

            /* Result ID */
            tpMessage->Rid = RID_E_OK;

            /* Register the user callback */
            if (TP_UdpReceiveAndForward_cb != NULL_PTR)
            {
                Udp_cb_func.UdpReceiveAndForward_cb = TP_UdpReceiveAndForward_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_UDP_RECEIVE_AND_FORWARD");
            }

            /* Register the user callback to receive an event*/
            if (TP_UdpReceiveAndForward_event_cb != NULL_PTR)
            {
                Udp_cb_func.UdpReceiveAndForward_event_cb = TP_UdpReceiveAndForward_event_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_UDP_RECEIVE_AND_FORWARD");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_UdpReceiveAndForward_cb;
    (void)TP_UdpReceiveAndForward_event_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_UDP;
        tpMessage->Pid = PID_UDP_RECEIVE_AND_FORWARD;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc(sizeof(uint16), sizeof(uint8));
        /* get the node containing the socketId */
        TP_socket_t *tempSocketNode = TP_FindSocket(socketId);
        if ((tpMessage->Payload != NULL_PTR) && (tempSocketNode != NULL_PTR))
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + sizeof(uint16); /* 8 + payload size */

            TP_TRACE_DEBUG("socketId %d , tempSocketNode->dropCnt %d", socketId, tempSocketNode->dropCnt);
            /* Copy the dropCnt */
            memcpy(&tpMessage->Payload[0], &tempSocketNode->dropCnt, sizeof(uint16));

            /* Set the socketId for forwarding */
            tempSocketNode->RecvFwdActive = TRUE;

            /* Set the maxFwd parameter */
            tempSocketNode->maxFwd = maxFwd;
            /* Set the maxLen parameter */
            tempSocketNode->maxLen = maxLen;

            /* Result ID */
            tpMessage->Rid = RID_E_OK;

            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
        }
        else if (tempSocketNode == NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size */
            /* Result ID */
            tpMessage->Rid = RID_E_ISD;

            /* send_message_without_payload() */
            Result = TP_SendFrame(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size */
            /* Result ID */
            tpMessage->Rid = RID_E_ISB;

            /* send_message_without_payload() */
            Result = TP_SendFrame(tpMessage);
        }
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_UdpConfigureSocket
*
***************************************************************************************************/
/**
* @brief
*  This SP is used to select and set parameters that can be configured on a socket basis. More parameters may be
*  supported in following versions of this document or by non-standard extensions (Parameter IDs starting
*  with 0xFFFF, 0xFFFE… and so forth)
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : socket that should be configured.
* @param[in] paramId : Type uint16 : Selects the parameter to be configured:
*                      0x0000  (1 Byte): TTL/Hop Limit
*                      0x0001  (1 Byte): Priority (traffic class/DSCP & ECN)
*                      0x0002  (1 Byte): IP DF DontFragment
*                      0x0003  (N Bytes): IP Timestamp Option data as stored in the IP header option 4 as described by RFC 791 page 22
*                      0x0004  (1 Byte): IP Type of Service TOS encoded as defined by RFC 791 page 29 (Delay, Throughout, Reliability, Cost, MBZ)
*                      0x0005  (2 Byte): Set MSS MaxSegmentSize (valid values 5001460)
*                      0x0006  (1 Byte): Enable/disable Nagle Algorithm parameter (enabled=1)
*                      0x0007  (1 Byte): Enable/disable the transmission of the UDP checksum (enabled=1)
* @param[in] paramVal  : Type vint8 : The value of the selected parameter that must match the corresponding length.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_UdpConfigureSocket(void (*TP_UdpConfigureSocket_cb)(TP_ResultID_t), uint16 socketId, uint16 paramId, vint8 paramVal)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_UDP;
        tpMessage->Pid = PID_UDP_CONFIGURE_SOCKET;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((sizeof(socketId) + sizeof(paramId) + sizeof(paramVal.dataLength) + paramVal.dataLength), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + sizeof(socketId) + sizeof(paramId) + sizeof(paramVal.dataLength) + paramVal.dataLength; /* 8 + payload size */

            /* Copy socketId parameter*/
            memcpy(&tpMessage->Payload[0], &socketId, sizeof(uint16));
            /* Copy paramId parameter*/
            memcpy(&tpMessage->Payload[2], &paramId, sizeof(uint16));
            /* Copy paramVal dataLength*/
            memcpy(&tpMessage->Payload[4], &paramVal.dataLength, sizeof(uint16));
            /* Copy paramVal Data */
            memcpy(&tpMessage->Payload[6], paramVal.Data, paramVal.dataLength);
            /* Result ID */
            tpMessage->Rid = RID_E_OK;

            /* Register the user callback */
            if (TP_UdpConfigureSocket_cb != NULL_PTR)
            {
                Udp_cb_func.UdpConfigureSocket_cb = TP_UdpConfigureSocket_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_UDP_CONFIGURE_SOCKET");
            }

            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_UdpConfigureSocket_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_UDP;
        tpMessage->Pid = PID_UDP_CONFIGURE_SOCKET;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;
        /* get the node containing the socketId */
        TP_socket_t *tempSocketNode = TP_FindSocket(socketId);
        if (tempSocketNode != NULL_PTR)
        {
            switch (paramId)
            {
            case SOCKET_PARAMETER_TTL_HOP:
                if (paramVal.dataLength > 1)
                {
                    tpMessage->Rid = RID_E_NOK;
                    break;
                }
                if (setsockopt(socketId, IPPROTO_IP, IP_TTL, &paramVal.Data[0], sizeof(uint8)) == ERROR_RETURN_CODE)
                {
                    TP_TRACE_ERROR("Failed to set socket option to SOCKET_PARAMETER_TTL_HOP. error code = %d", errno);
                    tpMessage->Rid = TP_DecodeReturnCode(errno);
                }
                else
                {
                    tpMessage->Rid = RID_E_OK;
                }
                break;
            case SOCKET_PARAMETER_PRIORITY:
                if (paramVal.dataLength > 1)
                {
                    tpMessage->Rid = RID_E_NOK;
                    break;
                }
                if (setsockopt(socketId, SOL_SOCKET, SO_PRIORITY, &paramVal.Data[0], sizeof(uint8)) == ERROR_RETURN_CODE)
                {
                    TP_TRACE_ERROR("Failed to set socket option to SOCKET_PARAMETER_PRIORITY. error code = %d", errno);
                    tpMessage->Rid = TP_DecodeReturnCode(errno);
                }
                else
                {
                    tpMessage->Rid = RID_E_OK;
                }
                break;
            case SOCKET_PARAMETER_IP_DF:
                if (paramVal.dataLength > 1)
                {
                    tpMessage->Rid = RID_E_NOK;
                    break;
                }
                if (setsockopt(socketId, IPPROTO_IP, IP_MTU_DISCOVER, &paramVal.Data[0], sizeof(uint8)) == ERROR_RETURN_CODE)
                {
                    TP_TRACE_ERROR("Failed to set socket option to SOCKET_PARAMETER_IP_DF. error code = %d", errno);
                    tpMessage->Rid = TP_DecodeReturnCode(errno);
                }
                else
                {
                    tpMessage->Rid = RID_E_OK;
                }
                break;
            case SOCKET_PARAMETER_IP_TIMESTAMP:
                /* To do */
                tpMessage->Rid = RID_E_OK;
                break;
            case SOCKET_PARAMETER_TOS:
                if (paramVal.dataLength > 1)
                {
                    tpMessage->Rid = RID_E_NOK;
                    break;
                }
                if (setsockopt(socketId, IPPROTO_IP, IP_TOS, &paramVal.Data[0], sizeof(uint8)) == ERROR_RETURN_CODE)
                {
                    TP_TRACE_ERROR("Failed to set socket option to SOCKET_PARAMETER_TOS. error code = %d", errno);
                    tpMessage->Rid = TP_DecodeReturnCode(errno);
                }
                else
                {
                    tpMessage->Rid = RID_E_OK;
                }
                break;
            case SOCKET_PARAMETER_MSS:
            {
                TP_TRACE_ERROR("Failed to set socket option to SOCKET_PARAMETER_MSS. Not support by UDP");
                /* MSS == MaxSegmentSize is a related to TCP */
                tpMessage->Rid = RID_E_NOK;
            }

            break;
            case SOCKET_PARAMETER_DISABLE_NAGLE:
            {
                TP_TRACE_ERROR("Failed to set socket option to SOCKET_PARAMETER_DISABLE_NAGLE. Not support by UDP");
                /* The NAGLE algorithm is part of TCP and not UDP */
                tpMessage->Rid = RID_E_NOK;
            }

            break;
            case SOCKET_PARAMETER_DISABLE_UDP_CHECKSUM:
                if (paramVal.dataLength > 1)
                {
                    tpMessage->Rid = RID_E_NOK;
                    break;
                }
                if (setsockopt(socketId, SOL_SOCKET, SO_NO_CHECK, &paramVal.Data[0], sizeof(uint8)) == ERROR_RETURN_CODE)
                {
                    TP_TRACE_ERROR("Failed to set socket option to SOCKET_PARAMETER_DISABLE_UDP_CHECKSUM. error code = %d", errno);
                    tpMessage->Rid = TP_DecodeReturnCode(errno);
                }
                else
                {
                    tpMessage->Rid = RID_E_OK;
                }
                break;
            default:
                tpMessage->Rid = RID_E_NOK;
                break;
            }
        }
        else
        {
            /* Socket not found */
            tpMessage->Rid = RID_E_ISD;
        }

        /* send_message_without_payload() */
        Result = TP_SendFrame(tpMessage);
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_UdpShutdown
*
***************************************************************************************************/
/**
* @brief
*  Shuts down a socket.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : Socket that should shutdown.
* @param[in] typeId : Type uint8 : Selects the way the socket is shutdown:
*                                        0x00: further reception will be disallowed
*                                        0x01: further transmission will be disallowed.
*                                        0x02: further transmission and reception will be disallowed.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_UdpShutdown(void (*TP_UdpShutdown_cb)(TP_ResultID_t), uint16 socketId, uint8 typeId)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_UDP;
        tpMessage->Pid = PID_UDP_SHUTDOWN;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((sizeof(socketId) + sizeof(typeId)), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + sizeof(socketId) + sizeof(typeId); /* 8 + payload size */

            /* Copy socket ID */
            memcpy(&tpMessage->Payload[0], &socketId, sizeof(uint16));
            /* Copy typeId parameter */
            tpMessage->Payload[2] = typeId;

            /* Result ID */
            tpMessage->Rid = RID_E_OK;
            /* Register the user callback */
            if (TP_UdpShutdown_cb != NULL_PTR)
            {
                Udp_cb_func.UdpShutdown_cb = TP_UdpShutdown_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_UDP_SHUTDOWN");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_UdpShutdown_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_UDP;
        tpMessage->Pid = PID_UDP_SHUTDOWN;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size = 8 + 0 */
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        /* Shutdown the requested Socket */
        if (shutdown(socketId, typeId) == ERROR_RETURN_CODE)
        {
            TP_TRACE_ERROR("Failed to shutdown socket, error code= %d", errno);
            tpMessage->Rid = TP_DecodeReturnCode(errno);
        }
        else
        {
            /* Socket Shuts down successfully */
            tpMessage->Rid = RID_E_OK;
        }

        Result = TP_SendFrame(tpMessage);
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpCloseSocket
*
***************************************************************************************************/
/**
* @brief
*  Closes a socket.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : Socket that should be closed.
* @param[in] abort : Type boolean : When true: closes the socket immediately, the stack is not waiting for
*                                   outstanding transmissions and acknowledgements
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpCloseSocket(void (*TP_TcpCloseSocket_cb)(TP_ResultID_t), uint16 socketId, boolean abort)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_CLOSE_SOCKET;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((sizeof(socketId) + sizeof(abort)), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + sizeof(socketId) + sizeof(abort); /* 8 + payload size */

            /* Copy socket ID */
            memcpy(&tpMessage->Payload[0], &socketId, sizeof(uint16));
            /* Copy abort parameter */
            tpMessage->Payload[2] = abort;

            /* Result ID */
            tpMessage->Rid = RID_E_OK;
            /* Register the user callback */
            if (TP_TcpCloseSocket_cb != NULL_PTR)
            {
                Tcp_cb_func.TcpCloseSocket_cb = TP_TcpCloseSocket_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_TCP_CLOSE_SOCKET");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else  /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_TcpCloseSocket_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_CLOSE_SOCKET;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size = 8 + 0  */
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        TP_socket_t *tempSocketNode = TP_FindSocket(socketId);
        if (tempSocketNode != NULL_PTR)
        {

            /* Close the requested Socket */
            if (abort == FALSE)
            {

                if (close(socketId) != ZERO_VALUE)
                {
                    TP_TRACE_ERROR("Failed to close socket, error code= %d", errno);
                    tpMessage->Rid = TP_DecodeReturnCode(errno);
                }
                else
                {
                    /* Socket closed successfully */
                    /* Cancel its thread */

                    if (pthread_cancel(tempSocketNode->pthread_id) != ZERO_VALUE)
                    {
                        TP_TRACE_ERROR("Failed to cancel thread id = %lu, socket = %d, error code = %d", tempSocketNode->pthread_id, socketId, errno);
                    }
                    /* remove the socketId from the list */
                    TP_DeleteSocket(socketId);
                    tpMessage->Rid = RID_E_OK;
                }
            }
            else
            {
                struct linger l;
                l.l_onoff = 1;
                l.l_linger = 0;
                setsockopt(socketId, SOL_SOCKET, SO_LINGER, &l, sizeof(l));
                shutdown(socketId, SHUT_RDWR);
                if (close(socketId) != ZERO_VALUE)
                {
                    TP_TRACE_ERROR("Failed to close socket, error code= %d", errno);
                    tpMessage->Rid = TP_DecodeReturnCode(errno);
                }
                else
                {
                    /* Socket closed successfully */
                    /* Cancel its thread */
                    if (pthread_cancel(tempSocketNode->pthread_id) != ZERO_VALUE)
                    {
                        TP_TRACE_ERROR("Failed to cancel thread id = %lu, socket = %d, error code = %d", tempSocketNode->pthread_id, socketId, errno);
                    }
                    /* remove the socketId from the list */
                    TP_DeleteSocket(socketId);
                    tpMessage->Rid = RID_E_OK;
                }
            }
        }
        else
        {
            tpMessage->Rid = RID_E_ISD;
        }

        Result = TP_SendFrame(tpMessage);
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }
#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpCreateAndBind
*
***************************************************************************************************/
/**
* @brief
*  Creates a socket and optionally binds this socket to a port and a local IP address.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] doBind : Type boolean : true: bind will be performed | false: no bind will be performed.
* @param[in] localPort : Type uint16 : Local port to bind (0xFFFF: PORT_ANY)
* @param[in] localAddr : Type ipxaddr : Local address (n=4:IPv4 or n=16:IPv6)
*                        Any IP: If all address bytes are zero The domain is selected by the type of address
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpCreateAndBind(void (*TP_TcpCreateAndBind_cb)(TP_ResultID_t, uint16), boolean doBind, uint16 localPort, ip4addr localAddr)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_CREATE_AND_BIND;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = calloc((sizeof(doBind) + sizeof(localPort) + sizeof(localAddr.dataLength) + localAddr.dataLength), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (sizeof(doBind) + sizeof(localPort) + sizeof(localAddr.dataLength) + localAddr.dataLength); /* 8 + payload size */

            /* Copy doBind parameter */
            tpMessage->Payload[0] = doBind;
            /* Copy localPort parameter */
            memcpy(&tpMessage->Payload[1], &localPort, sizeof(uint16));
            /* Copy localAddr dataLength parameter */
            memcpy(&tpMessage->Payload[3], &localAddr.dataLength, sizeof(uint16));
            /* Copy localAddr Data parameter */
            memcpy(&tpMessage->Payload[5], localAddr.Data, localAddr.dataLength);

            /* Result ID */
            tpMessage->Rid = RID_E_OK;

            /* Register the user callback */
            if (TP_TcpCreateAndBind_cb != NULL_PTR)
            {
                Tcp_cb_func.TcpCreateAndBind_cb = TP_TcpCreateAndBind_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_TCP_CREATE_AND_BIND");
            }

            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }
#else  /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_TcpCreateAndBind_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_CREATE_AND_BIND;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        /* Create the Requested Socket */

        struct sockaddr_in serverAddr = {ZERO_VALUE};

        int socketId = ERROR_RETURN_CODE;

        if (localAddr.dataLength == IPV4_ADDRESS_SIZE)
        {
            socketId = socket(PF_INET, SOCK_STREAM, ZERO_VALUE);

            if (socketId == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = TP_DecodeReturnCode(errno);
            }
            else
            {
                TP_TRACE_DEBUG("New TCP Socket is created socketId= %d", socketId);
                /* save the created socketId */
                TP_AddSocket(socketId, FALSE, ZERO_VALUE, ZERO_VALUE, ZERO_VALUE);
                threadSocket.numOfThread++;
                threadSocket.pthread_id = (pthread_t *)realloc(threadSocket.pthread_id, threadSocket.numOfThread * sizeof(pthread_t) * sizeof(uint8));
                int *socketIdthread = malloc(sizeof(socketId));
                if ((socketIdthread != NULL_PTR) && (threadSocket.pthread_id != NULL_PTR))
                {
                    *socketIdthread = socketId;
                    TP_socket_t *tempSocketNode = TP_FindSocket(socketId);
                    if (tempSocketNode != NULL_PTR)
                    {
                        if (pthread_create(&threadSocket.pthread_id[threadSocket.numOfThread - 1], NULL, TP_Tcp_ReceiveAndForwardSocket_thread, socketIdthread) == ZERO_VALUE)
                        {
                            /* Thread created successfully */
                            TP_TRACE_DEBUG("thread TP_Tcp_ReceiveAndForwardSocket_thread created successfully, socketId = %d", socketId);
                            /* Store the threadId related to the created socket */

                            tempSocketNode->pthread_id = threadSocket.pthread_id[threadSocket.numOfThread -1];
                            tpMessage->Rid = RID_E_OK;
                        }
                        else
                        {
                            TP_TRACE_ERROR("cannot create thread TP_Tcp_ReceiveAndForwardSocket_thread, socketId = %d", socketId);
                            tpMessage->Rid = RID_E_NOK;
                        }
                        //free(socketIdthread);

                        /* Bind the socket to a given IP addr and port */
                        if (doBind == TRUE)
                        {
                            // assign IP, PORT
                            serverAddr.sin_family = AF_INET;
                            serverAddr.sin_addr.s_addr = localAddr.Data[0] + (localAddr.Data[1] << 8) + (localAddr.Data[2] << 16) + (localAddr.Data[3] << 24);
                            serverAddr.sin_port = htons(localPort);

                            // Binding newly created socket to given IP and verification
                            if ((bind(socketId, (struct sockaddr *)&serverAddr, sizeof(serverAddr))) == ERROR_RETURN_CODE)
                            {
                                tpMessage->Rid = TP_DecodeReturnCode(errno);
                                TP_TRACE_ERROR("Failed to bind socket, error code= %d", errno);
                            }
                            else
                            {
                                TP_TRACE_DEBUG("bind socket sucessfully, socketId= %d", socketId);
                                tpMessage->Rid = RID_E_OK;
                            }
                        }
                        else
                        {
                            /* Nothing to do */
                        }
                    }
                    else
                    {
                        TP_TRACE_ERROR("Cannot find the socketId in the global linked list, socketId = %d", socketId);
                        tpMessage->Rid = RID_E_ISD;
                    }
                }
                else
                {
                    TP_TRACE_ERROR("Cannot create a thread to handle the received data on the newly created socket");
                    tpMessage->Rid = RID_E_NOK;
                }
            }
        }
        else if (localAddr.dataLength == IPV6_ADDRESS_SIZE)
        {
            /* Convert the given localAddr
                    char ipAddr[64] = {ZERO_VALUE};
                    sprintf(ipAddr, "%d.%d.%d.%d", *localAddr.ipv4_Addr.Data, localAddr.ipv4_Addr.Data[1], localAddr.ipv4_Addr.Data[2], localAddr.ipv4_Addr.Data[3]);
                    // assign IP, PORT
                    serverAddr.sin_family = AF_INET;
                    serverAddr.sin_addr.s_addr = htonl(ipAddr);
                    serverAddr.sin_port = htons(localPort);

                    // Binding newly created socket to given IP and verification
                    if ((bind(socketId, (struct sockaddr *)&serverAddr, sizeof(serverAddr))) != 0)
                    {
                        tpMessage->Rid = TP_DecodeReturnCode(errno);
                    }
                    else
                    {
                        tpMessage->Rid = RID_E_OK;
                    } */
        }
        else
        {
            tpMessage->Rid = RID_E_NOK;
        }
        /* Send the SocketId to the tester */
        if (tpMessage->Rid == RID_E_OK)
        {
            /* Construct the payload */
            tpMessage->Payload = (uint8 *)calloc(sizeof(uint16), sizeof(uint8));

            if (tpMessage->Payload != NULL_PTR)
            {
                tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + sizeof(uint16); /* 8 + payload size */

                /* Copy socketId response */
                memcpy(&tpMessage->Payload[0], &socketId, sizeof(uint16));

                /* send_message_with_payload() */
                Result = TP_SendFrame(tpMessage);
                free(tpMessage->Payload);
            }
            else
            {
                /* Can't allocate memory */
                tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size */
                /* Result ID */
                tpMessage->Rid = RID_E_ISB;

                /* send_message_without_payload() */
                Result = TP_SendFrame(tpMessage);
            }
        }
        else
        {
            /* send_message_without_payload() */
            Result = TP_SendFrame(tpMessage);
        }
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }
#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpSendData
*
***************************************************************************************************/
/**
* @brief
*  Sends data to a target.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : Local socket used to perform the transmission.
* @param[in] totalLen : Type uint16 : Total length: repeat data up to that length (in bytes).
*                       In case the value of totalLen is smaller than the length of data,
*                       the full length of data will be transmitted.
* @param[in] flags : Type uint8 : Bit 7: reserved
*                                 Bit 6: reserved
*                                 Bit 5: URG
*                                 Bit 4: reserved
*                                 Bit 3: PSH
*                                 Bit 2: reserved
*                                 Bit 1: reserved
*                                 Bit 0: reserved
* @param[in] data : Type vint8 : Data to transmit.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpSendData(void (*TP_TcpSendData_cb)(TP_ResultID_t), uint16 socketId, uint16 totalLen,
                             uint8 flags, vint8 data)
{
    TP_ResultID_t Result = E_NOT_OK;
    (void)flags;
    // to do cover flags param
#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_SEND_DATA;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;
        /* Construct the payload */
        tpMessage->Payload = calloc((sizeof(socketId) + sizeof(totalLen) + sizeof(flags) +
                                     sizeof(data.dataLength) + data.dataLength),
                                    sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (sizeof(socketId) + sizeof(totalLen) + sizeof(flags) +
                                                        sizeof(data.dataLength) + data.dataLength); /* 8 + payload size */

            /* Copy SocketId parameter */
            memcpy(&tpMessage->Payload[0], &socketId, sizeof(uint16));
            /* Copy totalLen parameter */
            memcpy(&tpMessage->Payload[2], &totalLen, sizeof(uint16));
            /* Copy flags parameter */
            memcpy(&tpMessage->Payload[4], &flags, sizeof(uint8));
            /* Copy data dataLength parameter */
            memcpy(&tpMessage->Payload[5], &data.dataLength, sizeof(uint16));
            /* Copy data Data parameter */
            memcpy(&tpMessage->Payload[7], data.Data, data.dataLength);

            /* Result ID */
            tpMessage->Rid = RID_E_OK;

            /* Register the user callback */
            if (TP_TcpSendData_cb != NULL_PTR)
            {
                Tcp_cb_func.TcpSendData_cb = TP_TcpSendData_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_TCP_SEND_DATA");
            }

            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_TcpSendData_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_SEND_DATA;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        /* In case the value of totalLen is smaller than the length of data, the full length of data will be transmitted */
        int bytesSent = ZERO_VALUE;
        if (data.dataLength >= totalLen)
        {
            /* send the data */
            bytesSent = send(socketId, data.Data, data.dataLength, ZERO_VALUE);
            //bytesSent = write(socketId, data.Data, data.dataLength);
            /* Check if the Data is sent successfully */
            if (bytesSent != data.dataLength)
            {
                tpMessage->Rid = RID_E_NOK;
            }
            else
            {
                tpMessage->Rid = RID_E_OK;
            }
        }
        else
        {
            /* Repeat data up to that totalLen (in bytes) */
            vint8 payloadToTranmit;
            int dataTransmited = ZERO_VALUE;
            payloadToTranmit.dataLength = totalLen;
            payloadToTranmit.Data = calloc(totalLen, sizeof(uint8));

            if (payloadToTranmit.Data != NULL_PTR)
            {
                /* Construct the Payload */

                while (((dataTransmited + data.dataLength) < totalLen) && (CancelThreadStatus == FALSE))
                {
                    memcpy(payloadToTranmit.Data + dataTransmited, data.Data, data.dataLength);
                    dataTransmited += data.dataLength;
                }
                /* Copy the remaining data */
                memcpy(payloadToTranmit.Data + dataTransmited, data.Data, (totalLen - dataTransmited));
                /* send the data */
                bytesSent = send(socketId, payloadToTranmit.Data, totalLen, ZERO_VALUE);
                /* Check if the Data is sent successfully */
                if (bytesSent != totalLen)
                {
                    tpMessage->Rid = RID_E_NOK;
                    TP_TRACE_ERROR("Could not send data bytesSent = %d, data.dataLength = %d, socketId = %d", bytesSent, data.dataLength, socketId);
                }
                else
                {
                    tpMessage->Rid = RID_E_OK;
                    TP_TRACE_ERROR("send data bytesSent = %d, data.dataLength = %d, socketId = %d", bytesSent, data.dataLength, socketId);
                }
            }
            else
            {
                /* Can't allocate memory */
                tpMessage->Rid = RID_E_NOK;
            }
        }
        Result = TP_SendFrame(tpMessage);
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpReceiveAndForward
*
***************************************************************************************************/
/**
* @brief
*  Data that will be received after the call of this SP will be forwarded to the test system.
*  The amount of forwarded data per received datagram (UDP) or bulk of stream data (TCP) can be limited using maxFwd.
*  The original length of this data unit can be obtained by fullLen. The process will repeat itself (active phase)
*  until the maximum amount of data defined by maxLen was received or END_TEST was called (inactive phase).
*  UDP: No further requirements. (see 6.12.2 UDP Receive and Count)
*  TCP: In the inactive phase (e.g. prior the first call) all data received will be discarded or ignored.
*       When called all data that was received on the specified socked prior the call of this SP will be consumed
*  in order to open the TCP receive window. All data that is received during the active phase of this SP will be consumed
*  up to the maximum amount of data defined by maxLen. (see 6.12.4 TCP Client Receive and Forward).
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] Function pointer to user defined callback to receive an event.
* @param[in] socketId : Type uint16 : The Socket selected for forwarding.
* @param[in] maxFwd : Type uint16 : Maximum length of payload to be forwarded per event.
* @param[in] maxLen : Type uint16 : Maximum count of bytes to receive over all (0xFFFF: limitless)

*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpReceiveAndForward(void (*TP_TcpReceiveAndForward_cb)(TP_ResultID_t, uint16), void (*TP_TcpReceiveAndForward_event_cb)(TP_ResultID_t, uint16, vint8),
                                      uint16 socketId, uint16 maxFwd, uint16 maxLen)
{

    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_RECEIVE_AND_FORWARD;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;
        /* Construct the payload */
        tpMessage->Payload = calloc((sizeof(socketId) + sizeof(maxFwd) + sizeof(maxLen)), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (sizeof(socketId) + sizeof(maxFwd) + sizeof(maxLen)); /* 8 + payload size */

            /* Copy SocketId parameter */
            memcpy(&tpMessage->Payload[0], &socketId, sizeof(uint16));
            /* Copy maxFwd parameter */
            memcpy(&tpMessage->Payload[2], &maxFwd, sizeof(uint16));
            /* Copy maxLen parameter */
            memcpy(&tpMessage->Payload[4], &maxLen, sizeof(uint16));

            /* Result ID */
            tpMessage->Rid = RID_E_OK;

            /* Register the user callback */
            if (TP_TcpReceiveAndForward_cb != NULL_PTR)
            {
                Tcp_cb_func.TcpReceiveAndForward_cb = TP_TcpReceiveAndForward_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_TCP_RECEIVE_AND_FORWARD");
            }

            /* Register the user callback to receive an event*/
            if (TP_TcpReceiveAndForward_event_cb != NULL_PTR)
            {
                Tcp_cb_func.TcpReceiveAndForward_event_cb = TP_TcpReceiveAndForward_event_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_TCP_RECEIVE_AND_FORWARD event");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_TcpReceiveAndForward_cb;
    (void)TP_TcpReceiveAndForward_event_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_RECEIVE_AND_FORWARD;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        /* get the node containing the socketId */
        TP_socket_t *tempSocketNode = TP_FindSocket(socketId);

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc(sizeof(uint16), sizeof(uint8));

        if ((tpMessage->Payload != NULL_PTR) && (tempSocketNode != NULL_PTR))
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + sizeof(uint16); /* 8 + payload size */

            /* Copy the dropCnt */
            memcpy(&tpMessage->Payload[0], &tempSocketNode->dropCnt, sizeof(uint16));

            /* Set the socketId for forwarding */
            tempSocketNode->RecvFwdActive = TRUE;

            /* Set the maxFwd parameter */
            tempSocketNode->maxFwd = maxFwd;
            /* Set the maxLen parameter */
            tempSocketNode->maxLen = maxLen;

            /* Result ID */
            tpMessage->Rid = RID_E_OK;

            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
        }
        else if (tempSocketNode == NULL_PTR)
        {
            /* Can't allocate memory */
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size */
            /* Result ID */
            tpMessage->Rid = RID_E_ISD;

            /* send_message_without_payload() */
            Result = TP_SendFrame(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size */
            /* Result ID */
            tpMessage->Rid = RID_E_ISB;

            /* send_message_without_payload() */
            Result = TP_SendFrame(tpMessage);
        }
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpListenAndAccept
*
***************************************************************************************************/
/**
* @brief
*  Marks a socket as listen socket that will be used to accept incoming connections. Whenever a new
*  connection was established this SP provides the socket ID of the new connection together with the listen
*  socket, client port, and address in an event.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] Function pointer to user defined callback to receive an event.
* @param[in] listenSocketId : Type uint16 : Local socket that should listen.
* @param[in] maxCon : Type uint16 : Maximum number of connections allowed to establish.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpListenAndAccept(void (*TP_TcpListenAndAccept_cb)(TP_ResultID_t), void (*TP_TcpListenAndAccept_event_cb)(TP_ResultID_t, uint16, uint16, uint16, ip4addr),
                                    uint16 listenSocketId, uint16 maxCon)
{

    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_LISTEN_AND_ACCEPT;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* set the TP type ID to request */
        tpMessage->Tid = TID_REQUEST;
        /* Construct the payload */
        tpMessage->Payload = calloc((sizeof(listenSocketId) + sizeof(maxCon)), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (sizeof(listenSocketId) + sizeof(maxCon)); /* 8 + payload size */

            /* Copy listenSocketId parameter */
            memcpy(&tpMessage->Payload[0], &listenSocketId, sizeof(uint16));
            /* Copy maxCon parameter */
            memcpy(&tpMessage->Payload[2], &maxCon, sizeof(uint16));
            /* Result ID */
            tpMessage->Rid = RID_E_OK;

            /* Register the user callback */
            if (TP_TcpListenAndAccept_cb != NULL_PTR)
            {
                Tcp_cb_func.TcpListenAndAccept_cb = TP_TcpListenAndAccept_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_TCP_LISTEN_AND_ACCEPT");
            }
            /* Register the user callback to receive an event*/
            if (TP_TcpListenAndAccept_event_cb != NULL_PTR)
            {
                Tcp_cb_func.TcpListenAndAccept_event_cb = TP_TcpListenAndAccept_event_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_TCP_LISTEN_AND_ACCEPT");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_TcpListenAndAccept_cb;
    (void)TP_TcpListenAndAccept_event_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_LISTEN_AND_ACCEPT;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;
        TP_socket_t *tempSocketNode_test = TP_FindSocket(listenSocketId);
        if (tempSocketNode_test != NULL_PTR)
        {
            /* set the given socketId to the listen state */
            if ((listen(listenSocketId, maxCon)) != 0)
            {
                TP_TRACE_ERROR("Input socket failed to listen.. ");
                tpMessage->Rid = TP_DecodeReturnCode(errno);
            }
            else
            {
                TP_TRACE_DEBUG("Input socket is listening.. ");
                /* create a thread to accept any incoming connection */
                Tcp_ListenAndAcceptParam_t *argus = calloc(sizeof(Tcp_ListenAndAcceptParam_t), sizeof(uint8));
                argus->maxCon = maxCon;
                argus->socketId = listenSocketId;
                threadSocket.numOfThread++;
                threadSocket.pthread_id = (pthread_t *)realloc(threadSocket.pthread_id, threadSocket.numOfThread * sizeof(pthread_t)* sizeof(uint8));
                if ((pthread_create(&threadSocket.pthread_id[threadSocket.numOfThread -1], NULL, TP_TcpListenAndAccept_event_thread, argus) == ZERO_VALUE) && (threadSocket.pthread_id != NULL_PTR))
                {
                    /* Thread created successfully */
                    TP_TRACE_DEBUG("thread TP_TcpListenAndAccept_event_thread created successfully");
                    tpMessage->Rid = RID_E_OK;
                }
                else
                {
                    TP_TRACE_ERROR("cannot create thread TP_TcpListenAndAccept_event_thread");
                    tpMessage->Rid = RID_E_NOK;
                }
            }
        }
        else
        {
            tpMessage->Rid = RID_E_ISD;
        }
        Result = TP_SendFrame(tpMessage);
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpConnect
*
***************************************************************************************************/
/**
* @brief
*  Triggers a TCP connection to a remote destination.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : TCP socket that should connect to a remote destination.
* @param[in] destPort : Type uint16 : Port of the remote destination.
* @param[in] destAddr : Type ip4addr : IP address of the remote destination.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpConnect(void (*TP_TcpConnect_cb)(TP_ResultID_t), uint16 socketId, uint16 destPort, ip4addr destAddr)
{

    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_CONNECT;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* set the TP type ID to request */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = calloc((sizeof(socketId) + sizeof(destPort) + sizeof(destAddr.dataLength) + destAddr.dataLength), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (sizeof(socketId) + sizeof(destPort) + sizeof(destAddr.dataLength) + destAddr.dataLength); /* 8 + payload size */

            /* Copy socketId parameter */
            memcpy(&tpMessage->Payload[0], &socketId, sizeof(uint16));
            /* Copy destPort parameter */
            memcpy(&tpMessage->Payload[2], &destPort, sizeof(uint16));
            /* Copy destAddr dataLength parameter */
            memcpy(&tpMessage->Payload[4], &destAddr.dataLength, sizeof(uint16));
            /* Copy destAddr Data parameter */
            memcpy(&tpMessage->Payload[6], destAddr.Data, destAddr.dataLength);
            /* Result ID */
            tpMessage->Rid = RID_E_OK;

            /* Register the user callback */
            if (TP_TcpConnect_cb != NULL_PTR)
            {
                Tcp_cb_func.TcpConnect_cb = TP_TcpConnect_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_TCP_CONNECT");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_TcpConnect_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_CONNECT;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;
        /* Connect to the given Addr */
        struct sockaddr_in servAddr;
        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = destAddr.Data[0] + (destAddr.Data[1] << 8) + (destAddr.Data[2] << 16) + (destAddr.Data[3] << 24);
        servAddr.sin_port = htons(destPort);

        /* connect the client socket to server socket */
        struct timeval tv = {ZERO_VALUE};
        tv.tv_sec = 1;
        tv.tv_usec = ZERO_VALUE;
        setsockopt(socketId, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);
        int iteration = ZERO_VALUE;
        int status = fcntl(socketId, F_SETFL, fcntl(socketId, F_GETFL, ZERO_VALUE) | O_NONBLOCK);
        if (status != ERROR_RETURN_CODE)
        {
            int res = connect(socketId, (struct sockaddr *)&servAddr, sizeof(servAddr));
            tpMessage->Rid = TP_DecodeReturnCode(errno);
            /* in case of the socket is not blocking .. */
            while ((res == ERROR_RETURN_CODE) && (CancelThreadStatus == FALSE))
            {
                res = connect(socketId, (struct sockaddr *)&servAddr, sizeof(servAddr));
                iteration++;
                TP_TRACE_ERROR("connection with the server failed...try number = %d , error code = %d", iteration, errno);
                tpMessage->Rid = TP_DecodeReturnCode(errno);
                if (iteration > 5)
                {
                    break;
                }

                sleep(1);
            }
            if (res != ERROR_RETURN_CODE)
            {
                tpMessage->Rid = RID_E_OK;
            }
        }
        else
        {
            tpMessage->Rid = TP_DecodeReturnCode(errno);
        }

        Result = TP_SendFrame(tpMessage);
        free(tpMessage->Payload);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpConfigureSocket
*
***************************************************************************************************/
/**
* @brief
*  This SP is used to select and set parameters that can be configured on a socket basis. More parameters may be
*  supported in following versions of this document or by non-standard extensions (Parameter IDs starting
*  with 0xFFFF, 0xFFFE… and so forth)
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : socket that should be configured.
* @param[in] paramId : Type uint16 : Selects the parameter to be configured:
*                      0x0000  (1 Byte): TTL/Hop Limit
*                      0x0001  (1 Byte): Priority (traffic class/DSCP & ECN)
*                      0x0002  (1 Byte): IP DF DontFragment
*                      0x0003  (N Bytes): IP Timestamp Option data as stored in the IP header option 4 as described by RFC 791 page 22
*                      0x0004  (1 Byte): IP Type of Service TOS encoded as defined by RFC 791 page 29 (Delay, Throughout, Reliability, Cost, MBZ)
*                      0x0005  (2 Byte): Set MSS MaxSegmentSize (valid values 5001460)
*                      0x0006  (1 Byte): Enable/disable Nagle Algorithm parameter (enabled=1)
*                      0x0007  (1 Byte): Enable/disable the transmission of the UDP checksum (enabled=1)
* @param[in] paramVal  : Type vint8 : The value of the selected parameter that must match the corresponding length.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpConfigureSocket(void (*TP_TcpConfigureSocket_cb)(TP_ResultID_t), uint16 socketId, uint16 paramId, vint8 paramVal)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_CONFIGURE_SOCKET;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((sizeof(socketId) + sizeof(paramId) + sizeof(paramVal.dataLength) + paramVal.dataLength), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + sizeof(socketId) + sizeof(paramId) + sizeof(paramVal.dataLength) + paramVal.dataLength; /* 8 + payload size */

            /* Copy socketId parameter*/
            memcpy(&tpMessage->Payload[0], &socketId, sizeof(uint16));
            /* Copy paramId parameter*/
            memcpy(&tpMessage->Payload[2], &paramId, sizeof(uint16));
            /* Copy paramVal dataLength*/
            memcpy(&tpMessage->Payload[4], &paramVal.dataLength, sizeof(uint16));
            /* Copy paramVal Data */
            memcpy(&tpMessage->Payload[6], paramVal.Data, paramVal.dataLength);
            /* Result ID */
            tpMessage->Rid = RID_E_OK;

            /* Register the user callback */
            if (TP_TcpConfigureSocket_cb != NULL_PTR)
            {
                Tcp_cb_func.TcpConfigureSocket_cb = TP_TcpConfigureSocket_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_TCP_CONFIGURE_SOCKET");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_TcpConfigureSocket_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_CONFIGURE_SOCKET;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;
        /* get the node containing the socketId */
        TP_socket_t *tempSocketNode = TP_FindSocket(socketId);
        if (tempSocketNode != NULL_PTR)
        {
            switch (paramId)
            {
            case SOCKET_PARAMETER_TTL_HOP:
                if (paramVal.dataLength > 1)
                {
                    tpMessage->Rid = RID_E_NOK;
                    break;
                }
                if (setsockopt(socketId, IPPROTO_IP, IP_TTL, &paramVal.Data[0], sizeof(uint8)) == ERROR_RETURN_CODE)
                {
                    TP_TRACE_ERROR("Failed to set socket option to SOCKET_PARAMETER_TTL_HOP. error code = %d", errno);
                    tpMessage->Rid = TP_DecodeReturnCode(errno);
                }
                else
                {
                    tpMessage->Rid = RID_E_OK;
                }
                break;
            case SOCKET_PARAMETER_PRIORITY:
                if (paramVal.dataLength > 1)
                {
                    tpMessage->Rid = RID_E_NOK;
                    break;
                }
                if (setsockopt(socketId, SOL_SOCKET, SO_PRIORITY, &paramVal.Data[0], sizeof(uint8)) == ERROR_RETURN_CODE)
                {
                    TP_TRACE_ERROR("Failed to set socket option to SOCKET_PARAMETER_PRIORITY. error code = %d", errno);
                    tpMessage->Rid = TP_DecodeReturnCode(errno);
                }
                else
                {
                    tpMessage->Rid = RID_E_OK;
                }
                break;
            case SOCKET_PARAMETER_IP_DF:
                if (paramVal.dataLength > 1)
                {
                    tpMessage->Rid = RID_E_NOK;
                    break;
                }
                if (setsockopt(socketId, IPPROTO_IP, IP_MTU_DISCOVER, &paramVal.Data[0], sizeof(uint8)) == ERROR_RETURN_CODE)
                {
                    TP_TRACE_ERROR("Failed to set socket option to SOCKET_PARAMETER_IP_DF. error code = %d", errno);
                    tpMessage->Rid = TP_DecodeReturnCode(errno);
                }
                else
                {
                    tpMessage->Rid = RID_E_OK;
                }
                break;
            case SOCKET_PARAMETER_IP_TIMESTAMP:
                /* To do */
                tpMessage->Rid = RID_E_OK;
                break;
            case SOCKET_PARAMETER_TOS:
                if (paramVal.dataLength > 1)
                {
                    tpMessage->Rid = RID_E_NOK;
                    break;
                }
                if (setsockopt(socketId, IPPROTO_IP, IP_TOS, &paramVal.Data[0], sizeof(uint8)) == ERROR_RETURN_CODE)
                {
                    TP_TRACE_ERROR("Failed to set socket option to SOCKET_PARAMETER_TOS. error code = %d", errno);
                    tpMessage->Rid = TP_DecodeReturnCode(errno);
                }
                else
                {
                    tpMessage->Rid = RID_E_OK;
                }
                break;
            case SOCKET_PARAMETER_MSS:
                if (paramVal.dataLength > 2)
                {
                    tpMessage->Rid = RID_E_NOK;
                    break;
                }
                if (setsockopt(socketId, IPPROTO_TCP, TCP_MAXSEG, &paramVal.Data[0], sizeof(uint16)) == ERROR_RETURN_CODE)
                {
                    TP_TRACE_ERROR("Failed to set socket option to SOCKET_PARAMETER_MSS. error code = %d", errno);
                    tpMessage->Rid = TP_DecodeReturnCode(errno);
                }
                else
                {
                    tpMessage->Rid = RID_E_OK;
                }
                break;
            case SOCKET_PARAMETER_DISABLE_NAGLE:
                if (paramVal.dataLength > 1)
                {
                    tpMessage->Rid = RID_E_NOK;
                    break;
                }
                if (setsockopt(socketId, IPPROTO_TCP, TCP_NODELAY, &paramVal.Data[0], sizeof(uint8)) == ERROR_RETURN_CODE)
                {
                    TP_TRACE_ERROR("Failed to set socket option to SOCKET_PARAMETER_DISABLE_NAGLE. error code = %d", errno);
                    tpMessage->Rid = TP_DecodeReturnCode(errno);
                }
                else
                {
                    tpMessage->Rid = RID_E_OK;
                }
                break;
            case SOCKET_PARAMETER_DISABLE_UDP_CHECKSUM:
                if (paramVal.dataLength > 1)
                {
                    tpMessage->Rid = RID_E_NOK;
                    break;
                }
                if (setsockopt(socketId, SOL_SOCKET, SO_NO_CHECK, &paramVal.Data[0], sizeof(uint8)) == ERROR_RETURN_CODE)
                {
                    TP_TRACE_ERROR("Failed to set socket option to SOCKET_PARAMETER_DISABLE_UDP_CHECKSUM. error code = %d", errno);
                    tpMessage->Rid = TP_DecodeReturnCode(errno);
                }
                else
                {
                    tpMessage->Rid = RID_E_OK;
                }
                break;
            default:
                tpMessage->Rid = RID_E_NOK;
                break;
            }
        }
        else
        {
            /* Socket not found */
            tpMessage->Rid = RID_E_ISD;
        }

        /* send_message_without_payload() */
        Result = TP_SendFrame(tpMessage);
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpShutdown
*
***************************************************************************************************/
/**
* @brief
*  Shuts down a socket.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] socketId : Type uint16 : Socket that should shutdown.
* @param[in] typeId : Type uint8 : Selects the way the socket is shutdown:
*                                        0x00: further reception will be disallowed
*                                        0x01: further transmission will be disallowed.
*                                        0x02: further transmission and reception will be disallowed.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpShutdown(void (*TP_TcpShutdown_cb)(TP_ResultID_t), uint16 socketId, uint8 typeId)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_SHUTDOWN;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((sizeof(socketId) + sizeof(typeId)), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + sizeof(socketId) + sizeof(typeId); /* 8 + payload size */

            /* Copy socket ID */
            memcpy(&tpMessage->Payload[0], &socketId, sizeof(uint16));
            /* Copy typeId parameter */
            tpMessage->Payload[2] = typeId;

            /* Result ID */
            tpMessage->Rid = RID_E_OK;
            /* Register the user callback */
            if (TP_TcpShutdown_cb != NULL_PTR)
            {
                Tcp_cb_func.TcpShutdown_cb = TP_TcpShutdown_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_Tcp_SHUTDOWN");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_TcpShutdown_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_SHUTDOWN;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size = 8 + 0  */
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        /* Shutdown the requested Socket */
        if (shutdown(socketId, typeId) == ERROR_RETURN_CODE)
        {
            TP_TRACE_ERROR("Failed to shutdown socket, error code = %d", errno);
            tpMessage->Rid = TP_DecodeReturnCode(errno);
        }
        else
        {
            /* Socket Shuts down successfully */
            tpMessage->Rid = RID_E_OK;
        }

        Result = TP_SendFrame(tpMessage);
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpSocketState
*
***************************************************************************************************/
/**
* @brief
*  Return the state of a socket.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] localPort : Type uint16 : local port.
* @param[in] localAddr : Type ip4addr : local IP address.
* @param[in] destPort : Type uint16 : local port.
* @param[in] destAddr : Type ip4addr : local IP address.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_TcpSocketState(void (*TP_TcpSocketState_cb)(TP_ResultID_t, uint8), uint16 localPort, ip4addr localAddr,
                                uint16 destPort, ip4addr destAddr)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_SOCKET_STATE;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((sizeof(localPort) + sizeof(localAddr.dataLength) + localAddr.dataLength +
                                              sizeof(destPort) + sizeof(destAddr.dataLength) + destAddr.dataLength),
                                             sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + sizeof(localPort) + sizeof(localAddr.dataLength) + localAddr.dataLength +
                                sizeof(destPort) + sizeof(destAddr.dataLength) + destAddr.dataLength; /* 8 + payload size */

            /* Copy localPort */
            memcpy(&tpMessage->Payload[0], &localPort, sizeof(uint16));
            /* Copy localAddr.dataLength parameter */
            memcpy(&tpMessage->Payload[2], &localAddr.dataLength, sizeof(uint16));
            /* Copy localAddr.Data parameter */
            memcpy(&tpMessage->Payload[4], localAddr.Data, localAddr.dataLength);
            /* Copy destPort */
            memcpy(&tpMessage->Payload[4 + localAddr.dataLength], &destPort, sizeof(uint16));
            /* Copy destAddr.dataLength parameter */
            memcpy(&tpMessage->Payload[6 + localAddr.dataLength], &destAddr.dataLength, sizeof(uint16));
            /* Copy destAddr.Data parameter */
            memcpy(&tpMessage->Payload[8 + localAddr.dataLength], destAddr.Data, destAddr.dataLength);

            /* Result ID */
            tpMessage->Rid = RID_E_OK;
            /* Register the user callback */
            if (TP_TcpSocketState_cb != NULL_PTR)
            {
                Tcp_cb_func.TcpSocketState_cb = TP_TcpSocketState_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_TCP_SOCKET_STATE");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_TcpSocketState_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_TCP;
        tpMessage->Pid = PID_TCP_SOCKET_STATE;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        FILE *fp;
        char path[512];
        uint8 socketState = ZERO_VALUE;
        fp = fopen("/proc/net/tcp", "r");
        do
        {
            if (fp == NULL)
            {
                tpMessage->Rid = E_NOT_OK;
                break;
            }
            else
            {
                uint32 local_addr_in, rem_addr = ZERO_VALUE; /* 32 + 1 for NUL */
                struct inet_params param;

                uint32 localipaddress = ZERO_VALUE;
                uint32 destipaddress = ZERO_VALUE;
                memset(&param, 0, sizeof(param));
                /* Read the output of the command */
                while (fgets(path, sizeof(path), fp) != NULL)
                {
                    sscanf(path,
                           "%*d: %X:%X "
                           "%X:%X %X "
                           "%lX:%lX %*X:%*X "
                           "%*X %d %*d %lu ",
                           &local_addr_in, &param.local_port,
                           &rem_addr, &param.rem_port, &param.state,
                           &param.txq, &param.rxq,
                           &param.uid, &param.inode);

                    /* find the requested socket state */
                    localipaddress = localAddr.Data[0] + (localAddr.Data[1] << 8) + (localAddr.Data[2] << 16) + (localAddr.Data[3] << 24);
                    destipaddress = destAddr.Data[0] + (destAddr.Data[1] << 8) + (destAddr.Data[2] << 16) + (destAddr.Data[3] << 24);
                    if ((localPort == param.local_port) && (localipaddress == local_addr_in) && (destPort == param.rem_port) && (destipaddress == rem_addr))
                    {
                        socketState = param.state;
                        break;
                    }
                    else
                    {
                        /* nothing to do */
                    }
                }

                pclose(fp);
            }
        } while (0);
        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((sizeof(uint8)), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + sizeof(uint8); /* 8 + payload size */

            /* Copy socket state */
            tpMessage->Payload[0] = socketState;
            /* Result ID */
            tpMessage->Rid = RID_E_OK;
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
        }
        else
        {
            /* Can't allocate memory */
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size */
            /* Result ID */
            tpMessage->Rid = RID_E_ISB;

            /* send_message_without_payload() */
            Result = TP_SendFrame(tpMessage);
        }
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_Ipv4StaticAddress
*
***************************************************************************************************/
/**
* @brief
*  Assigns a static IP address and Netmask to the given network interface.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS)
* @param[in] addr : Type ip4addr The subnet for the route.
* @param[in] netMask  : Type uint8 The subnet mask for the route in CIDR-notation.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_Ipv4StaticAddress(void (*TP_Ipv4StaticAddress_cb)(TP_ResultID_t), text ifName, ip4addr addr, uint8 netmask)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_IPv4;
        tpMessage->Pid = PID_IPv4_STATIC_ADDRESS;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((ifName.dataLength + sizeof(ifName.dataLength) + sizeof(addr.dataLength) + addr.dataLength + sizeof(netmask)), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (ifName.dataLength + sizeof(ifName.dataLength) + sizeof(addr.dataLength) + addr.dataLength + sizeof(netmask)); /* 8 + payload size */

            /* Copy ifName.dataLength */
            memcpy(&tpMessage->Payload[0], &ifName.dataLength, sizeof(uint16));
            /* Copy ifName.Data */
            memcpy(&tpMessage->Payload[2], ifName.Data, ifName.dataLength);
            /* Copy addr.dataLength */
            memcpy(&tpMessage->Payload[2 + ifName.dataLength], &addr.dataLength, sizeof(uint16));
            /* Copy addr.Data */
            memcpy(&tpMessage->Payload[4 + ifName.dataLength], addr.Data, addr.dataLength);
            /* Copy netmask parameter */
            tpMessage->Payload[4 + ifName.dataLength + addr.dataLength] = netmask;

            /* Result ID */
            tpMessage->Rid = RID_E_OK;
            /* Register the user callback */
            if (TP_Ipv4StaticAddress_cb != NULL_PTR)
            {
                IPv4_cb_func.Ipv4StaticAddress = TP_Ipv4StaticAddress_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_IPv4_STATIC_ADDRESS");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else  /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_Ipv4StaticAddress_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_IPv4;
        tpMessage->Pid = PID_IPv4_STATIC_ADDRESS;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        int fd;
        struct ifreq ifr;
        struct sockaddr_in *addr_ip;
        do
        {
            /* AF_INET - to define network interface IPv4 */
            /* Creating soket for it */
            fd = socket(AF_INET, SOCK_DGRAM, 0);

            if (fd == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = RID_E_NOK;
                TP_TRACE_ERROR("Cannot create socket, error code = %d", errno);
                break;
            }
            /* AF_INET - to define IPv4 Address type. */
            ifr.ifr_addr.sa_family = AF_INET;

            /* Copy Interface name */
            memcpy(ifr.ifr_name, ifName.Data, ifName.dataLength);

            /* defining the sockaddr_in */
            addr_ip = (struct sockaddr_in *)&ifr.ifr_addr;

            /* convert ip address in correct format to write */
            addr_ip->sin_addr.s_addr = addr.Data[0] + (addr.Data[1] << 8) + (addr.Data[2] << 16) + (addr.Data[3] << 24);
            /* Setting the Ip Address using ioctl */
            if (ioctl(fd, SIOCSIFADDR, &ifr) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = RID_E_NOK;
                TP_TRACE_ERROR("Cannot set ip address, error code = %d", errno);
                break;
            }
            /* Copy netmask */

            addr_ip->sin_addr.s_addr = htonl(0xFFFFFFFFU << (32 - netmask));
            if (ioctl(fd, SIOCSIFNETMASK, &ifr) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = RID_E_NOK;
                TP_TRACE_ERROR("Cannot set netmask, error code = %d", errno);
                break;
            }

            if (close(fd) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = RID_E_NOK;
                TP_TRACE_ERROR("Cannot close socket, error code = %d", errno);
                break;
            }
            tpMessage->Rid = RID_E_OK;
        } while (0);
    }

    else
    {
        /* Can't allocate memory */
        tpMessage->Rid = RID_E_NOK;
    }
    Result = TP_SendFrame(tpMessage);
    free(tpMessage);
#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_Ipv4StaticRoute
*
***************************************************************************************************/
/**
* @brief
*  Adds a static route for the network. This SP is not affecting the persistent configuration.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS)
* @param[in] subNeT : Type ip4addr The subnet for the route.
* @param[in] netMask  : Type uint8 The subnet mask for the route in CIDR-notation.
* @param[in] gatway : Type ip4addr The gateway IP address for the route.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_Ipv4StaticRoute(void (*TP_Ipv4StaticRoute_cb)(TP_ResultID_t), text ifName, ip4addr subNet,
                                 uint8 netMask, ip4addr gateway)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_IPv4;
        tpMessage->Pid = PID_IPv4_STATIC_ROUTE;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((ifName.dataLength + sizeof(ifName.dataLength) + sizeof(subNet.dataLength) + subNet.dataLength + sizeof(netMask) + gateway.dataLength + sizeof(gateway.dataLength)), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (ifName.dataLength + sizeof(ifName.dataLength) + sizeof(subNet.dataLength) + subNet.dataLength + sizeof(netMask) + gateway.dataLength + sizeof(gateway.dataLength)); /* 8 + payload size */

            /* Copy ifName.dataLength */
            memcpy(&tpMessage->Payload[0], &ifName.dataLength, sizeof(uint16));
            /* Copy ifName.Data */
            memcpy(&tpMessage->Payload[2], ifName.Data, ifName.dataLength);
            /* Copy subNet.dataLength */
            memcpy(&tpMessage->Payload[2 + ifName.dataLength], &subNet.dataLength, sizeof(uint16));
            /* Copy subNet.Data */
            memcpy(&tpMessage->Payload[4 + ifName.dataLength], subNet.Data, subNet.dataLength);
            /* Copy netmask parameter */
            tpMessage->Payload[4 + ifName.dataLength + subNet.dataLength] = netMask;
            /* Copy gateway.dataLength */
            memcpy(&tpMessage->Payload[5 + ifName.dataLength + subNet.dataLength], &gateway.dataLength, sizeof(uint16));
            /* Copy gateway.Data */
            memcpy(&tpMessage->Payload[7 + ifName.dataLength + subNet.dataLength], gateway.Data, gateway.dataLength);
            /* Result ID */
            tpMessage->Rid = RID_E_OK;
            /* Register the user callback */
            if (TP_Ipv4StaticRoute_cb != NULL_PTR)
            {
                IPv4_cb_func.Ipv4StaticRoute = TP_Ipv4StaticRoute_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_IPv4_STATIC_ROUTE");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else  /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_Ipv4StaticRoute_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_IPv4;
        tpMessage->Pid = PID_IPv4_STATIC_ROUTE;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        int sockfd;

        do
        {
            struct
            {
                /* Routing structure */
                struct nlmsghdr messageHeader;
                struct rtmsg routeMessage;
                char buf[1024];
            } nl_request;
            /* create netlink socket */
            sockfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
            if (sockfd == ERROR_RETURN_CODE)
            {
                TP_TRACE_ERROR("Cannot create socket, error code = %d", errno);
                tpMessage->Rid = RID_E_NOK;
                break;
            }

            /* Initialize request structure */
            nl_request.messageHeader.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
            nl_request.messageHeader.nlmsg_flags = NLM_F_REQUEST | NLM_F_CREATE;
            nl_request.messageHeader.nlmsg_type = RTM_NEWROUTE;
            nl_request.messageHeader.nlmsg_pid = 0;
            nl_request.routeMessage.rtm_family = AF_INET;
            nl_request.routeMessage.rtm_table = RT_TABLE_MAIN;
            nl_request.routeMessage.rtm_scope = RT_SCOPE_UNIVERSE;
            nl_request.routeMessage.rtm_type = RTN_UNICAST;
            nl_request.routeMessage.rtm_dst_len = netMask;
            nl_request.routeMessage.rtm_protocol = RTPROT_STATIC;
            nl_request.routeMessage.rtm_flags = ZERO_VALUE;

            uint32 addr = ZERO_VALUE;
            /* Set gateway */
            addr = gateway.Data[0] + (gateway.Data[1] << 8) + (gateway.Data[2] << 16) + (gateway.Data[3] << 24);
            rtattr_add(&nl_request.messageHeader, sizeof(nl_request), RTA_GATEWAY, &addr, 4);
            //addr = 100;
            //rtattr_add(&nl_request.messageHeader, sizeof(nl_request), RTA_PRIORITY, &addr, 4);
            nl_request.routeMessage.rtm_scope = 0;
            nl_request.routeMessage.rtm_family = AF_INET;
            /* Set destination network */
            addr = subNet.Data[0] + (subNet.Data[1] << 8) + (subNet.Data[2] << 16) + (subNet.Data[3] << 24);
            rtattr_add(&nl_request.messageHeader, sizeof(nl_request), RTA_DST, &addr, 4);

            /* Copy Interface name */
            char ifNameTemp[16];
            memcpy(ifNameTemp, ifName.Data, ifName.dataLength);
            addr = if_nametoindex(ifNameTemp);
            /* Set interface */
            rtattr_add(&nl_request.messageHeader, sizeof(nl_request), RTA_OIF, &addr, sizeof(int));

            /* Send message to the netlink */
            int tx = send(sockfd, &nl_request, sizeof(nl_request), 0);
            TP_TRACE_ERROR("tx = %d err %d", tx, errno);
            if (close(sockfd) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = RID_E_NOK;
                TP_TRACE_ERROR("Cannot close socket, error code = %d", errno);
                break;
            }
            tpMessage->Rid = RID_E_OK;
        } while (0);
    }

    else
    {
        /* Can't allocate memory */
        tpMessage->Rid = RID_E_NOK;
    }
    Result = TP_SendFrame(tpMessage);
    free(tpMessage);
#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DhcpInitClient
*
***************************************************************************************************/
/**
* @brief
*  Initialize the DHCP Client by use of network interface and port.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS)
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DhcpInitClient(void (*TP_DhcpInitClient_cb)(TP_ResultID_t), text ifName)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_DHCPv4;
        tpMessage->Pid = PID_DHCPv4_INIT_CLIENT;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((ifName.dataLength + sizeof(ifName.dataLength)), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (ifName.dataLength + sizeof(ifName.dataLength)); /* 8 + payload size */

            /* Copy ifName.dataLength */
            memcpy(&tpMessage->Payload[0], &ifName.dataLength, sizeof(uint16));
            /* Copy ifName.Data */
            memcpy(&tpMessage->Payload[2], ifName.Data, ifName.dataLength);

            /* Result ID */
            tpMessage->Rid = RID_E_OK;
            /* Register the user callback */
            if (TP_DhcpInitClient_cb != NULL_PTR)
            {
                Dhcpv4_cb_func.DhcpInitClient = TP_DhcpInitClient_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_DHCPv4_INIT_CLIENT");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else  /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_DhcpInitClient_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_DHCPv4;
        tpMessage->Pid = PID_DHCPv4_INIT_CLIENT;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;
        do
        {
            if ((ifName.Data == NULL_PTR) || (ifName.dataLength == ZERO_VALUE))
            {
                tpMessage->Rid = RID_E_INV;
                break;
            }
            char *release = " -r ";
            char *command = "dhclient -4 ";
            char releaseCommand [128];
            (void)memset(releaseCommand, ZERO_VALUE, sizeof(releaseCommand)/sizeof(releaseCommand[0]));
            /* Construct the releaseCommand */
            (void)strcat(releaseCommand, (char *)command);
            (void)strcat(releaseCommand, (char *)release);
            (void)strcat(releaseCommand, (char *)ifName.Data);

            TP_TRACE_DEBUG("command %s", releaseCommand);
            /*  Release  the  current  lease  and  stop  the  running DHCP client */
            if (system(releaseCommand) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = TP_DecodeReturnCode(errno);
                break;
            }
            (void)memset(releaseCommand, ZERO_VALUE, sizeof(releaseCommand)/sizeof(releaseCommand[0]));
            /* Construct the releaseCommand */
            (void)strcat(releaseCommand, (char *)command);
            (void)strcat(releaseCommand, (char *)ifName.Data);
            TP_TRACE_DEBUG("command %s", releaseCommand);
            /* Start the DHCP client using UDP default port 68 */ 
            if (system(releaseCommand) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = TP_DecodeReturnCode(errno);
                break;
            }
            tpMessage->Rid = RID_E_OK;

        } while (0);
    }

    else
    {
        /* Can't allocate memory */
        tpMessage->Rid = RID_E_NOK;
    }
    Result = TP_SendFrame(tpMessage);
    free(tpMessage);
#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DhcpStopClient
*
***************************************************************************************************/
/**
* @brief
*  Shutdown the DHCP Client by use of network interface and port.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS)
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DhcpStopClient(void (*TP_DhcpStopClient_cb)(TP_ResultID_t), text ifName)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_DHCPv4;
        tpMessage->Pid = PID_DHCPv4_STOP_CLIENT;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((ifName.dataLength + sizeof(ifName.dataLength)), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (ifName.dataLength + sizeof(ifName.dataLength)); /* 8 + payload size */

            /* Copy ifName.dataLength */
            memcpy(&tpMessage->Payload[0], &ifName.dataLength, sizeof(uint16));
            /* Copy ifName.Data */
            memcpy(&tpMessage->Payload[2], ifName.Data, ifName.dataLength);

            /* Result ID */
            tpMessage->Rid = RID_E_OK;
            /* Register the user callback */
            if (TP_DhcpStopClient_cb != NULL_PTR)
            {
                Dhcpv4_cb_func.DhcpStopClient = TP_DhcpStopClient_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_DHCPv4_STOP_CLIENT");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else  /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_DhcpStopClient_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_DHCPv4;
        tpMessage->Pid = PID_DHCPv4_STOP_CLIENT;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;
        do
        {
            if ((ifName.Data == NULL_PTR) || (ifName.dataLength == ZERO_VALUE))
            {
                tpMessage->Rid = RID_E_INV;
                break;
            }
            char *release = " -r ";
            char *command = "dhclient -4 ";
            char releaseCommand [128];
            (void)memset(releaseCommand, ZERO_VALUE, sizeof(releaseCommand)/sizeof(releaseCommand[0]));
            /* Construct the releaseCommand */
            (void)strcat(releaseCommand, (char *)command);
            (void)strcat(releaseCommand, (char *)release);
            (void)strcat(releaseCommand, (char *)ifName.Data);

            TP_TRACE_DEBUG("command %s", releaseCommand);
            /*  Release  the  current  lease  and  stop  the  running DHCP client */
            if (system(releaseCommand) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = TP_DecodeReturnCode(errno);
                break;
            }
            (void)memset(releaseCommand, ZERO_VALUE, sizeof(releaseCommand)/sizeof(releaseCommand[0]));
            /* Construct the releaseCommand */
            (void)strcat(releaseCommand, (char *)command);
            (void)strcat(releaseCommand, (char *)release);
            if(sprintf(releaseCommand + strlen(releaseCommand), " -pf /var/run/dhclient-%s.pid ", (char *)ifName.Data) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = TP_DecodeReturnCode(errno);
                break;  
            }
            (void)strcat(releaseCommand, (char *)ifName.Data);
            TP_TRACE_DEBUG("command %s", releaseCommand);
            /*  Release  the  current  lease  and  stop  the  running DHCP client */
            if (system(releaseCommand) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = TP_DecodeReturnCode(errno);
                break;
            }
            tpMessage->Rid = RID_E_OK;

        } while (0);
    }

    else
    {
        /* Can't allocate memory */
        tpMessage->Rid = RID_E_NOK;
    }
    Result = TP_SendFrame(tpMessage);
    free(tpMessage);
#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DhcpSetClientOptions
*
***************************************************************************************************/
/**
* @brief
*  Sets DHCP Client options.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS).
* @param[in] code : DHCP option code:
*                                       51d  [4  Byte]:   IP address lease time.
*                                       57d  [2  Byte]:   Maximum message size.
*                                       61d  [1…*  Byte]: Client identifier by name.
*                                       161d [6  Byte]:   Client identifier by hardware address.
* @param[in] value : DHCP option value selected by the code parameter using the corresponding byte size.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DhcpSetClientOptions(void (*TP_DhcpSetClientOptions_cb)(TP_ResultID_t), text ifName, uint8 code, vint8 value)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_DHCPv4;
        tpMessage->Pid = PID_DHCPv4_SET_OPTION;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((ifName.dataLength + sizeof(ifName.dataLength) + sizeof(uint8) + value.dataLength + sizeof(value.dataLength)), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (ifName.dataLength + sizeof(ifName.dataLength) + sizeof(uint8) + value.dataLength + sizeof(value.dataLength)); /* 8 + payload size */

            /* Copy ifName.dataLength */
            memcpy(&tpMessage->Payload[0], &ifName.dataLength, sizeof(uint16));
            /* Copy ifName.Data */
            memcpy(&tpMessage->Payload[2], ifName.Data, ifName.dataLength);
            /* Copy code */
            memcpy(&tpMessage->Payload[2 + ifName.dataLength], &code, sizeof(uint8));
            /* Copy value.dataLength */
            memcpy(&tpMessage->Payload[3 + ifName.dataLength], value.Data, sizeof(uint16));
            /* Copy value.Data */
            memcpy(&tpMessage->Payload[5 + ifName.dataLength], &value.dataLength, value.dataLength);

            /* Result ID */
            tpMessage->Rid = RID_E_OK;
            /* Register the user callback */
            if (TP_DhcpSetClientOptions_cb != NULL_PTR)
            {
                Dhcpv4_cb_func.DhcpSetClientOptions = TP_DhcpSetClientOptions_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_DHCPv4_SET_OPTION");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else  /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_DhcpSetClientOptions_cb;
    (void) ifName;
    (void)code;
    (void) value;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_DHCPv4;
        tpMessage->Pid = PID_DHCPv4_SET_OPTION;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        /* to do */
        tpMessage->Rid = RID_E_OK;

    }

    else
    {
        /* Can't allocate memory */
        tpMessage->Rid = RID_E_NOK;
    }
    Result = TP_SendFrame(tpMessage);
    free(tpMessage);
#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_Dhcpv6InitClient
*
***************************************************************************************************/
/**
* @brief
*  Initialize the DHCPv6 Client by use of network interface and port.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS)
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_Dhcpv6InitClient(void (*TP_Dhcpv6InitClient_cb)(TP_ResultID_t), text ifName)
{
      TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_DHCPv6;
        tpMessage->Pid = PID_DHCPv6_INIT_CLIENT;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((ifName.dataLength + sizeof(ifName.dataLength)), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (ifName.dataLength + sizeof(ifName.dataLength)); /* 8 + payload size */

            /* Copy ifName.dataLength */
            memcpy(&tpMessage->Payload[0], &ifName.dataLength, sizeof(uint16));
            /* Copy ifName.Data */
            memcpy(&tpMessage->Payload[2], ifName.Data, ifName.dataLength);

            /* Result ID */
            tpMessage->Rid = RID_E_OK;
            /* Register the user callback */
            if (TP_Dhcpv6InitClient_cb != NULL_PTR)
            {
                Dhcpv6_cb_func.Dhcpv6InitClient = TP_Dhcpv6InitClient_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_DHCPv6_INIT_CLIENT");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else  /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_Dhcpv6InitClient_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_DHCPv6;
        tpMessage->Pid = PID_DHCPv6_INIT_CLIENT;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;
        do
        {
            if ((ifName.Data == NULL_PTR) || (ifName.dataLength == ZERO_VALUE))
            {
                tpMessage->Rid = RID_E_INV;
                break;
            }
            char *release = " -r ";
            char *command = "dhclient -6 ";
            char releaseCommand [128];
            (void)memset(releaseCommand, ZERO_VALUE, sizeof(releaseCommand)/sizeof(releaseCommand[0]));
            /* Construct the releaseCommand */
            (void)strcat(releaseCommand, (char *)command);
            (void)strcat(releaseCommand, (char *)release);
            (void)strcat(releaseCommand, (char *)ifName.Data);

            TP_TRACE_DEBUG("command %s", releaseCommand);
            /*  Release  the  current  lease  and  stop  the  running DHCP client */
            if (system(releaseCommand) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = TP_DecodeReturnCode(errno);
                break;
            }
            (void)memset(releaseCommand, ZERO_VALUE, sizeof(releaseCommand)/sizeof(releaseCommand[0]));
            /* Construct the releaseCommand */
            (void)strcat(releaseCommand, (char *)command);
            (void)strcat(releaseCommand, (char *)ifName.Data);
            TP_TRACE_DEBUG("command %s", releaseCommand);
            /* Start the DHCP client using UDP default port 68 */ 
            if (system(releaseCommand) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = TP_DecodeReturnCode(errno);
                break;
            }
            tpMessage->Rid = RID_E_OK;

        } while (0);
    }

    else
    {
        /* Can't allocate memory */
        tpMessage->Rid = RID_E_NOK;
    }
    Result = TP_SendFrame(tpMessage);
    free(tpMessage);
#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_Dhcpv6StopClient
*
***************************************************************************************************/
/**
* @brief
*  Shutdown the DHCPv6 Client by use of network interface and port.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS)
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_Dhcpv6StopClient(void (*TP_Dhcpv6StopClient_cb)(TP_ResultID_t), text ifName)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_DHCPv6;
        tpMessage->Pid = PID_DHCPv6_STOP_CLIENT;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((ifName.dataLength + sizeof(ifName.dataLength)), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (ifName.dataLength + sizeof(ifName.dataLength)); /* 8 + payload size */

            /* Copy ifName.dataLength */
            memcpy(&tpMessage->Payload[0], &ifName.dataLength, sizeof(uint16));
            /* Copy ifName.Data */
            memcpy(&tpMessage->Payload[2], ifName.Data, ifName.dataLength);

            /* Result ID */
            tpMessage->Rid = RID_E_OK;
            /* Register the user callback */
            if (TP_Dhcpv6StopClient_cb != NULL_PTR)
            {
                Dhcpv6_cb_func.Dhcpv6StopClient = TP_Dhcpv6StopClient_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_DHCPv6_STOP_CLIENT");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else  /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_Dhcpv6StopClient_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_DHCPv6;
        tpMessage->Pid = PID_DHCPv6_STOP_CLIENT;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;
        do
        {
            if ((ifName.Data == NULL_PTR) || (ifName.dataLength == ZERO_VALUE))
            {
                tpMessage->Rid = RID_E_INV;
                break;
            }
            char *release = " -r ";
            char *command = "dhclient -6 ";
            char releaseCommand [128];
            (void)memset(releaseCommand, ZERO_VALUE, sizeof(releaseCommand)/sizeof(releaseCommand[0]));
            /* Construct the releaseCommand */
            (void)strcat(releaseCommand, (char *)command);
            (void)strcat(releaseCommand, (char *)release);
            (void)strcat(releaseCommand, (char *)ifName.Data);

            TP_TRACE_DEBUG("command %s", releaseCommand);
            /*  Release  the  current  lease  and  stop  the  running DHCP client */
            if (system(releaseCommand) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = TP_DecodeReturnCode(errno);
                break;
            }
            (void)memset(releaseCommand, ZERO_VALUE, sizeof(releaseCommand)/sizeof(releaseCommand[0]));
            /* Construct the releaseCommand */
            (void)strcat(releaseCommand, (char *)command);
            (void)strcat(releaseCommand, (char *)release);
            if(sprintf(releaseCommand + strlen(releaseCommand), " -pf /var/run/dhclient-%s.pid ", (char *)ifName.Data) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = TP_DecodeReturnCode(errno);
                break;  
            }
            (void)strcat(releaseCommand, (char *)ifName.Data);
            TP_TRACE_DEBUG("command %s", releaseCommand);
            /*  Release  the  current  lease  and  stop  the  running DHCP client */
            if (system(releaseCommand) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = TP_DecodeReturnCode(errno);
                break;
            }
            tpMessage->Rid = RID_E_OK;

        } while (0);
    }

    else
    {
        /* Can't allocate memory */
        tpMessage->Rid = RID_E_NOK;
    }
    Result = TP_SendFrame(tpMessage);
    free(tpMessage);
#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_EthInterfaceUp
*
***************************************************************************************************/
/**
* @brief
*  Enables an Ethernet interface or virtual interface. This SP is not affecting the persistent configuration.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS)
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_EthInterfaceUp(void (*TP_EthInterfaceUp_cb)(TP_ResultID_t), text ifName)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_ETH;
        tpMessage->Pid = PID_ETH_INTERFACE_UP;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((ifName.dataLength + sizeof(ifName.dataLength)), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (ifName.dataLength + sizeof(ifName.dataLength)); /* 8 + payload size */

            /* Copy ifName.dataLength */
            memcpy(&tpMessage->Payload[0], &ifName.dataLength, sizeof(uint16));
            /* Copy ifName.Data */
            memcpy(&tpMessage->Payload[2], ifName.Data, ifName.dataLength);
            /* Result ID */
            tpMessage->Rid = RID_E_OK;
            /* Register the user callback */
            if (TP_EthInterfaceUp_cb != NULL_PTR)
            {
                Eth_cb_func.EthInterfaceUp = TP_EthInterfaceUp_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_ETH_INTERFACE_UP");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else  /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_EthInterfaceUp_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_ETH;
        tpMessage->Pid = PID_ETH_INTERFACE_UP;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        int sockfd;
        struct ifreq ifr;

        do
        {
            /* create socket */
            sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (sockfd == ERROR_RETURN_CODE)
            {
                TP_TRACE_ERROR("Cannot create socket, error code = %d", errno);
                tpMessage->Rid = RID_E_NOK;
                break;
            }
            memset(&ifr, 0, sizeof ifr);
            /* AF_INET - to define IPv4 Address type. */
            ifr.ifr_addr.sa_family = AF_INET;

            /* Copy Interface name */
            memcpy(ifr.ifr_name, ifName.Data, ifName.dataLength);

            ifr.ifr_flags |= IFF_UP;
            if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = RID_E_NOK;
                TP_TRACE_ERROR("Cannot enable interface , error code = %d", errno);
                break;
            }
            if (close(sockfd) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = RID_E_NOK;
                TP_TRACE_ERROR("Cannot close socket, error code = %d", errno);
                break;
            }
            tpMessage->Rid = RID_E_OK;
        } while (0);
    }

    else
    {
        /* Can't allocate memory */
        tpMessage->Rid = RID_E_NOK;
    }
    Result = TP_SendFrame(tpMessage);
    free(tpMessage);
#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_EthInterfaceDown
*
***************************************************************************************************/
/**
* @brief
*  Disables an Ethernet interface or virtual interface. This SP is not affecting the persistent configuration.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] ifName : The name of the network interface (e.g. “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-BE6E101110033111}”
*                       or “0” depending of the OS)
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_EthInterfaceDown(void (*TP_EthInterfaceDown_cb)(TP_ResultID_t), text ifName)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_ETH;
        tpMessage->Pid = PID_ETH_INTERFACE_DOWN;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((ifName.dataLength + sizeof(ifName.dataLength)), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (ifName.dataLength + sizeof(ifName.dataLength)); /* 8 + payload size */

            /* Copy ifName.dataLength */
            memcpy(&tpMessage->Payload[0], &ifName.dataLength, sizeof(uint16));
            /* Copy ifName.Data */
            memcpy(&tpMessage->Payload[2], ifName.Data, ifName.dataLength);
            /* Result ID */
            tpMessage->Rid = RID_E_OK;
            /* Register the user callback */
            if (TP_EthInterfaceDown_cb != NULL_PTR)
            {
                Eth_cb_func.EthInterfaceDown = TP_EthInterfaceDown_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_ETH_INTERFACE_DOWN");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else  /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_EthInterfaceDown_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_ETH;
        tpMessage->Pid = PID_ETH_INTERFACE_DOWN;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        int sockfd;
        struct ifreq ifr;

        do
        {
            /* create socket */
            sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (sockfd == ERROR_RETURN_CODE)
            {
                TP_TRACE_ERROR("Cannot create socket, error code = %d", errno);
                tpMessage->Rid = RID_E_NOK;
                break;
            }
            memset(&ifr, 0, sizeof ifr);
            /* AF_INET - to define IPv4 Address type. */
            ifr.ifr_addr.sa_family = AF_INET;

            /* Copy Interface name */
            memcpy(ifr.ifr_name, ifName.Data, ifName.dataLength);

            ifr.ifr_flags &= ~IFF_UP;
            if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = RID_E_NOK;
                TP_TRACE_ERROR("Cannot disable interface , error code = %d", errno);
                break;
            }
            if (close(sockfd) == ERROR_RETURN_CODE)
            {
                tpMessage->Rid = RID_E_NOK;
                TP_TRACE_ERROR("Cannot close socket, error code = %d", errno);
                break;
            }
            tpMessage->Rid = RID_E_OK;
        } while (0);
    }

    else
    {
        /* Can't allocate memory */
        tpMessage->Rid = RID_E_NOK;
    }
    Result = TP_SendFrame(tpMessage);
    free(tpMessage);
#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_ClearArpCache
*
***************************************************************************************************/
/**
* @brief
*  Clear the dynamic entries in the ARP Cache of DUT.
*
* @par
* @param[in] Function pointer to user defined callback.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_ClearArpCache(void (*TP_ClearArpCache_cb)(TP_ResultID_t))
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_ARP;
        tpMessage->Pid = PID_ARP_CLEAR_CACHE;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Result ID */
        tpMessage->Rid = RID_E_OK;
        if (TP_ClearArpCache_cb != NULL_PTR)
        {
            Arp_cb_func.ClearArpCache = TP_ClearArpCache_cb;
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_ARP_CLAER_CACHE");
        }
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else  /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_ClearArpCache_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_ARP;
        tpMessage->Pid = PID_ARP_CLEAR_CACHE;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        system("ip -s -s neigh flush all");
        /* Result ID */
        tpMessage->Rid = RID_E_OK;

        /* send_message_with_payload() */
        Result = TP_SendFrame(tpMessage);
        free(tpMessage->Payload);
    }
    else
    {
        /* Can't allocate memory */
        tpMessage->Rid = RID_E_NOK;
    }
#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_AddStaticEntry
*
***************************************************************************************************/
/**
* @brief
*  Add a static entry in DUT ARP Cache.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] addip the Internet address of the entry to add.
* @param[in] addmac the hardware address of the entry to add.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_AddStaticEntry(void (*TP_AddStaticEntry_cb)(TP_ResultID_t), vint8 addip, vint8 addmac)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_ARP;
        tpMessage->Pid = PID_ARP_ADD_STATIC_ENTRY;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;
        /* Construct the payload */
        tpMessage->Payload = calloc((sizeof(addip.dataLength) + addip.dataLength + sizeof(addmac.dataLength) + addmac.dataLength), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (sizeof(addip.dataLength) + addip.dataLength + sizeof(addmac.dataLength) + addmac.dataLength); /* 8 + payload size */

            /* Copy addip dataLength parameter */
            memcpy(&tpMessage->Payload[0], &addip.dataLength, sizeof(uint16));
            /* Copy addip Data parameter */
            memcpy(&tpMessage->Payload[2], addip.Data, addip.dataLength);
            /* Copy addmac dataLength parameter */
            memcpy(&tpMessage->Payload[2 + addip.dataLength], &addmac.dataLength, sizeof(uint16));
            /* Copy addmac Data parameter */
            memcpy(&tpMessage->Payload[2 + addip.dataLength + 2], addmac.Data, addmac.dataLength);

            /* Result ID */
            tpMessage->Rid = RID_E_OK;

            /* Register the user callback */
            if (TP_AddStaticEntry_cb != NULL_PTR)
            {
                Arp_cb_func.AddStaticEntry = TP_AddStaticEntry_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_ADD_STATIC_ENTRY");
            }

            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else  /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_AddStaticEntry_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_ARP;
        tpMessage->Pid = PID_ARP_ADD_STATIC_ENTRY;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        /* construct the command to add the new entry in the arp table */
        char command[42] = "arp -s ";
        char *ip = addip.Data;
        char *mac = addmac.Data;

        strcat(command, ip);
        strcat(command, " ");
        strcat(command, mac);

        system(command);

        /* Result ID */
        tpMessage->Rid = RID_E_OK;

        /* send_message_with_payload() */
        Result = TP_SendFrame(tpMessage);
        free(tpMessage->Payload);
    }
    else
    {
        /* Can't allocate memory */
        tpMessage->Rid = RID_E_NOK;
    }
#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_DeleteStaticEntry
*
***************************************************************************************************/
/**
* @brief
*  Delete a static entry in DUT ARP Cache.
*
* @par
* @param[in] Function pointer to user defined callback.
* @param[in] addip the Internet address of the entry to delete.
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TP_ResultID_t TP_DeleteStaticEntry(void (*TP_DeleteStaticEntry_cb)(TP_ResultID_t), vint8 addip)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {

        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_ARP;
        tpMessage->Pid = PID_ARP_DELETE_STATIC_ENTRY;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;
        /* Construct the payload */
        tpMessage->Payload = calloc((sizeof(addip.dataLength) + addip.dataLength), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (sizeof(addip.dataLength) + addip.dataLength); /* 8 + payload size */

            /* Copy addip dataLength parameter */
            memcpy(&tpMessage->Payload[0], &addip.dataLength, sizeof(uint16));
            /* Copy addip Data parameter */
            memcpy(&tpMessage->Payload[2], addip.Data, addip.dataLength);

            /* Result ID */
            tpMessage->Rid = RID_E_OK;

            /* Register the user callback */
            if (TP_DeleteStaticEntry_cb != NULL_PTR)
            {
                Arp_cb_func.DeleteStaticEntry = TP_DeleteStaticEntry_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_ARP_DELETE_STATIC_ENTRY");
            }

            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }

#else  /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_DeleteStaticEntry_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_ARP;
        tpMessage->Pid = PID_ARP_DELETE_STATIC_ENTRY;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;

        /* construct the command to add the new entry in the arp table */
        char command[27] = "arp -d ";
        char *ip = addip.Data;

        strcat(command, ip);

        system(command);

        /* Result ID */
        tpMessage->Rid = RID_E_OK;

        /* send_message_with_payload() */
        Result = TP_SendFrame(tpMessage);
        free(tpMessage->Payload);
    }
    else
    {
        /* Can't allocate memory */
        tpMessage->Rid = RID_E_NOK;
    }
#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_IcmpEchoRequest
*
***************************************************************************************************/
/**
* @brief
*  Issues the transmission of an ICMP Echo Request
*
* @par
* @param[in] ifName     : Type text     : Optional: The name of the network interface (e.g.
*                                               “eth1.5” or “\Device\NPF_{6F111E2E-41B6-4147-
*                                               BE6E-101110033111}” or “0” depending of the
*                                               OS)
* @param[in] destAddr   : Type ipxaddr  : The destination address.
* @param[in] data       : Type vint8    : Payload to transmit
*
*
* @return TP_ResultID_t : Termination status of the executed function.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/

TP_ResultID_t TP_IcmpEchoRequest(void (*TP_IcmpEchoRequest_cb)(TP_ResultID_t), text ifName, ip4addr destAddr, vint8 data)
{
    TP_ResultID_t Result = E_NOT_OK;

#ifdef FEATURE_TESTER_SIDE

    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_ICMPv4;
        tpMessage->Pid = PID_ICMPv4_ECHO_REQUEST;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* Construct the payload */
        tpMessage->Payload = (uint8 *)calloc((ifName.dataLength + destAddr.dataLength + data.dataLength + 6), sizeof(uint8));

        if (tpMessage->Payload != NULL_PTR)
        {
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + ifName.dataLength + destAddr.dataLength + data.dataLength + 6; /* 8 + payload size */

            /* Copy ifName length*/
            memcpy(&tpMessage->Payload[0], &ifName.dataLength, sizeof(uint16));
            /* Copy ifName */
            memcpy(&tpMessage->Payload[2], ifName.Data, ifName.dataLength);
            /* Copy destAddr length */
            memcpy(&tpMessage->Payload[ifName.dataLength + 2], &destAddr.dataLength, sizeof(uint16));
            /* Copy destAddr  */
            memcpy(&tpMessage->Payload[ifName.dataLength + 4], destAddr.Data, destAddr.dataLength);
            /* Copy data length */
            memcpy(&tpMessage->Payload[ifName.dataLength + destAddr.dataLength + 4], &data.dataLength, sizeof(uint16));
            /* Copy data */
            memcpy(&tpMessage->Payload[ifName.dataLength + destAddr.dataLength + 6], data.Data, data.dataLength);
            /* Result ID */
            tpMessage->Rid = RID_E_OK;
            /* Register the user callback */
            if (TP_IcmpEchoRequest_cb != NULL_PTR)
            {
                Icmp_cb_func.IcmpEchoRequest_cb = TP_IcmpEchoRequest_cb;
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for IcmpEchoRequest_cb");
            }
            /* send_message_with_payload() */
            Result = TP_SendFrame(tpMessage);
            free(tpMessage->Payload);
            free(tpMessage);
        }
        else
        {
            /* Can't allocate memory */
            Result = RID_E_NOK;
        }
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }
#else  /* FEATURE_DUT_SIDE */

    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    (void)TP_IcmpEchoRequest_cb;
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_ICMPv4;
        tpMessage->Pid = PID_ICMPv4_ECHO_REQUEST;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_RESPONSE;
        if (sendIcmpEchoRequest(ifName, destAddr, data) == ERROR_RETURN_CODE)
        {
            TP_TRACE_ERROR("failed to send & receive ICMP Echo Request/reply  = %d", errno);
            tpMessage->Rid = RID_E_IIF;
        }
        else
        {
            TP_TRACE_INFO("succeeded to send & receive ICMP Echo Request/reply");
            tpMessage->Rid = RID_E_OK;
        }
        Result = TP_SendFrame(tpMessage);
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }
#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

#ifndef FEATURE_TESTER_SIDE

int main()
{

    TP_OpenControlChannel((ip4addr){ZERO_VALUE}, SEREVR_PORT);
    int returnValue = atexit(TP_cleanDependencies);
    if (returnValue != ZERO_VALUE)
    {
        TP_TRACE_ERROR("cannot set exit function = %d", returnValue);
        exit(EXIT_FAILURE);
    }
    TP_CloseControlChannel();

    return 0;
}

#endif

/** \}    end of addtogroup */
