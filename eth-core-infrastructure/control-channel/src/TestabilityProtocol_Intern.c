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
#include "TestabilityProtocol_Intern.h"
#include "TestabilityProtocol_api.h"
#include "TestabilityProtocol_cbTypes.h"
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
#define BYTE_ORDER_MARK_B 0xEFU
#define BYTE_ORDER_MARK_O 0xBBU
#define BYTE_ORDER_MARK_M 0xBFU

#define ERROR_RETURN_CODE (-1)
#define SEREVR_PORT 56000
#define PORT_ANY 0xFFFFU

#define LIMITLESS_FORWARD_DATA 0xFFFFU
/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/*==================================================================================================
*                                        Global Variables
==================================================================================================*/
pthread_t thread_id;
/* Create a TCP scoket */
int sockfd1, connfd;
uint32 len;
int sockfd;
struct sockaddr_in cli;
/* Genaral Group cb function */
General_cb_t General_cb_func;
/* UDP Group cb function */
Udp_cb_t Udp_cb_func;
/* TCP Group cb function */
Tcp_cb_t Tcp_cb_func;
/* IPv4 Group cb function */
Ipv4_cb_t IPv4_cb_func;
/* Dhcpv4 Group cb function */
Dhcpv4_cb_t Dhcpv4_cb_func;
/* Dhcpv6 Group cb function */
Dhcpv6_cb_t Dhcpv6_cb_func;
/* ARP Group cb function */
Arp_cb_t Arp_cb_func;
/* ETH Group cb function */
Eth_cb_t Eth_cb_func;
/* ICMP Group cb function */
Icmp_cb_t Icmp_cb_func;

struct TP_socket_t *headSocketId_t = NULL_PTR;

/* Dynamic array for the created threads */
TP_threadSocket_t threadSocket = {ZERO_VALUE, NULL_PTR};
/* Global variable used to exit all threads */
boolean CancelThreadStatus = FALSE;

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
TP_ResultID_t TP_OpenControlChannel(ip4addr ipv4Address, uint16 socketPort)
{

    TP_ResultID_t Result = E_OK;

#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_INFO("enter FEATURE_TESTER_SIDE ");
    struct sockaddr_in servAddr;

    /* socket create and verification */
    sockfd = socket(PF_INET, SOCK_STREAM, ZERO_VALUE);
    if (sockfd == ERROR_RETURN_CODE)
    {
        TP_TRACE_ERROR("socket creation failed, errno = %d", errno);
        Result = TP_DecodeReturnCode(errno);
        exit(EXIT_FAILURE);
    }
    else
    {
        /* Close all opened sockets */
        struct linger l;
        l.l_onoff = 1;
        l.l_linger = 0;
        int true = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &l, sizeof(l));
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(int));
        TP_TRACE_DEBUG("Socket successfully created.. ");
        /* assign IP, PORT */
        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = ipv4Address.Data[0] + (ipv4Address.Data[1] << 8) + (ipv4Address.Data[2] << 16) + (ipv4Address.Data[3] << 24);
        servAddr.sin_port = htons(socketPort);

        /* connect the client socket to server socket */
        while (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == ERROR_RETURN_CODE)
        {
            TP_TRACE_ERROR("connection with the server failed... %d ", errno);
            sleep(1);
        }
        TP_TRACE_INFO("connected to the server.. ");
    }

#else /* FEATURE_DUT_SIDE */
    TP_TRACE_INFO("enter FEATURE_DUT_SIDE ");
    struct sockaddr_in serverAddr;

    /* socket create and verification */
    sockfd1 = socket(PF_INET, SOCK_STREAM, ZERO_VALUE);
    if (sockfd1 == -1)
    {
        TP_TRACE_ERROR("socket creation failed, error = %d", errno);
        Result = TP_DecodeReturnCode(errno);
        exit(EXIT_FAILURE);
    }
    else
    {
        int true = 1;
        setsockopt(sockfd1, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(int));
        TP_TRACE_DEBUG("Socket successfully created.. ");
        bzero(&serverAddr, sizeof(serverAddr));

        /* assign IP, PORT */
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = ipv4Address.Data[0] + (ipv4Address.Data[1] << 8) + (ipv4Address.Data[2] << 16) + (ipv4Address.Data[3] << 24);
        serverAddr.sin_port = htons(socketPort);

        /* Binding newly created socket to given IP and verification */
        if ((bind(sockfd1, (struct sockaddr *)&serverAddr, sizeof(serverAddr))) != 0)
        {
            TP_TRACE_ERROR("socket bind failed, error = %d", errno);
            Result = TP_DecodeReturnCode(errno);
            exit(EXIT_FAILURE);
        }
        else
        {
            TP_TRACE_DEBUG("Socket successfully binded.. ");

            /* Now server is ready to listen and verification */
            if ((listen(sockfd1, 1)) != 0)
            {
                TP_TRACE_ERROR("Listen failed, error = %d", errno);
                Result = TP_DecodeReturnCode(errno);
                exit(EXIT_FAILURE);
            }
            else
            {
                TP_TRACE_DEBUG("Server listening.. ");
                len = sizeof(cli);

                /* Accept the data packet from client and verification */
                connfd = accept(sockfd1, (struct sockaddr *)&cli, &len);
                /* Close all opened sockets */
                struct linger l;
                l.l_onoff = 1;
                l.l_linger = 0;
                int true = 1;
                setsockopt(connfd, SOL_SOCKET, SO_LINGER, &l, sizeof(l));
                setsockopt(connfd, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(int));
                if (connfd < 0)
                {
                    TP_TRACE_ERROR("server acccept failed, error = %d", errno);
                    Result = TP_DecodeReturnCode(errno);
                    exit(EXIT_FAILURE);
                }
                else
                {
                    TP_TRACE_DEBUG("server acccept the client... ");
                }
            }
        }
    }

#endif /* FEATURE_TESTER_SIDE */

    if (pthread_create(&thread_id, NULL, TP_ReceiveFrameCb, NULL) == ZERO_VALUE)
    {
        /* Thread created successfully */
        TP_TRACE_DEBUG("thread TP_ReceiveFrameCb created successfully");
        Result = RID_E_OK;
    }
    else
    {
        TP_TRACE_ERROR("cannot create thread TP_ReceiveFrameCb");
        Result = TP_DecodeReturnCode(errno);
        exit(EXIT_FAILURE);
    }
    TP_TRACE_INFO("exit Result = %d ", Result);
    return Result;
}

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
TP_ResultID_t TP_CloseControlChannel(void)
{
    TP_ResultID_t Result = E_NOT_OK;
    TP_TRACE_INFO("enter");
#ifdef FEATURE_TESTER_SIDE
    TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
    if (tpMessage != NULL_PTR)
    {
        /* Construct the TP message */
        TP_TRACE_DEBUG("Construct the TP message");
        tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
        tpMessage->EVB = ZERO_VALUE;
        tpMessage->Gid = GID_CC;
        tpMessage->Pid = PID_CC_CLOSE_CHANNEL;
        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
        tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
        tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
        /* change the TP type ID to response */
        tpMessage->Tid = TID_REQUEST;

        /* send_message_with_payload() */
        Result = TP_SendFrame(tpMessage);
        free(tpMessage);
    }
    else
    {
        /* Can't allocate memory */
        Result = RID_E_NOK;
    }
#else  /* FEATURE_DUT_SIDE */
    if (pthread_join(thread_id, NULL) == ZERO_VALUE)
    {
        /* close the opened sockets */
        if (close(sockfd) == ERROR_RETURN_CODE)
        {
            Result = TP_DecodeReturnCode(errno);
        }
        else
        {
            if (close(sockfd1) == ERROR_RETURN_CODE)
            {
                Result = TP_DecodeReturnCode(errno);
            }
            else
            {

                if (close(connfd) == ERROR_RETURN_CODE)
                {
                    Result = TP_DecodeReturnCode(errno);
                }
                else
                {
                    Result = TP_DecodeReturnCode(errno);
                }
            }
        }
    }
#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_INFO("exit Result = %d \n", Result);
    return Result;
}

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
void *TP_ReceiveFrameCb(void *args)
{
    uint32 numberOfBytesRead = ZERO_VALUE;
    uint32 bufferSize = TP_MESSAGE_LENGTH_WITHOUT_PAYLOAD;
    (void)args;
    while (TRUE)
    {
        TestabilityProtocolMessage_t *tempBufferMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
#ifdef FEATURE_TESTER_SIDE
        //TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
        numberOfBytesRead = recv(sockfd, tempBufferMessage, bufferSize, ZERO_VALUE);
        if (numberOfBytesRead == bufferSize)
        {
            TP_TRACE_DEBUG("Received TP message Header: ");
            TP_PrintArray((uint8 *)tempBufferMessage, numberOfBytesRead);
            /* We received data */
            /* Check the payload size of the received TP message */
            if (tempBufferMessage->Length > TP_PAYLOAD_SIZE_BRUT)
            {
                /* Receive the payload */
                tempBufferMessage->Payload = (uint8 *)calloc((tempBufferMessage->Length - TP_PAYLOAD_SIZE_BRUT), sizeof(uint8));
                numberOfBytesRead = recv(sockfd, tempBufferMessage->Payload, (tempBufferMessage->Length - TP_PAYLOAD_SIZE_BRUT), ZERO_VALUE);
                if (numberOfBytesRead == (tempBufferMessage->Length - TP_PAYLOAD_SIZE_BRUT))
                {
                    TP_TRACE_DEBUG("Received TP message Payload: ");
                    TP_PrintArray((uint8 *)tempBufferMessage->Payload, numberOfBytesRead);
                    TP_DecodeReceivedMessage(tempBufferMessage);

                    free(tempBufferMessage->Payload);
                }
                else
                {
                    TP_TRACE_ERROR("Received buffer length = %d Expected = %d", numberOfBytesRead, (tempBufferMessage->Length - TP_PAYLOAD_SIZE_BRUT));
                    free(tempBufferMessage->Payload);
                }
            }
            else
            {
                TP_DecodeReceivedMessage(tempBufferMessage);
                /* Received TP message without payload */
            }
        }
        else
        {
            //TP_TRACE_ERROR("Received buffer length = %d Expected = %d", numberOfBytesRead, bufferSize);
        }

#else /* FEATURE_DUT_SIDE */
        //TP_TRACE_DEBUG("start FEATURE_DUT_SIDE ");
        numberOfBytesRead = recv(connfd, tempBufferMessage, bufferSize, ZERO_VALUE);
        if (numberOfBytesRead == bufferSize)
        {
            TP_TRACE_DEBUG("Received TP message Header: ");
            TP_PrintArray((uint8 *)tempBufferMessage, numberOfBytesRead);
            /* We received data */
            /* Check the payload size of the received TP message */
            if (tempBufferMessage->Length > TP_PAYLOAD_SIZE_BRUT)
            {

                /* Receive the payload */
                tempBufferMessage->Payload = (uint8 *)calloc((tempBufferMessage->Length - TP_PAYLOAD_SIZE_BRUT), sizeof(uint8));
                numberOfBytesRead = recv(connfd, tempBufferMessage->Payload, (tempBufferMessage->Length - TP_PAYLOAD_SIZE_BRUT), ZERO_VALUE);
                if (numberOfBytesRead == (tempBufferMessage->Length - TP_PAYLOAD_SIZE_BRUT))
                {
                    TP_TRACE_DEBUG("Received TP message Payload: ");
                    TP_PrintArray((uint8 *)tempBufferMessage->Payload, numberOfBytesRead);
                    TP_DecodeReceivedMessage(tempBufferMessage);

                    free(tempBufferMessage->Payload);
                }
                else
                {

                    TP_TRACE_ERROR("Received buffer length = %d Expected = %d", numberOfBytesRead, (tempBufferMessage->Length - TP_PAYLOAD_SIZE_BRUT));
                    free(tempBufferMessage->Payload);
                }
            }
            else
            {
                /* Received TP message without payload */
                TP_DecodeReceivedMessage(tempBufferMessage);
            }
        }
        else
        {
            //TP_TRACE_ERROR("Received buffer length = %d Expected = %d", numberOfBytesRead, bufferSize);
        }

#endif /* FEATURE_TESTER_SIDE */

        free(tempBufferMessage);
    }
    return NULL_PTR;
}

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
TP_ResultID_t TP_SendFrame(TestabilityProtocolMessage_t *tpMessage)
{
    TP_ResultID_t Result = E_NOT_OK;

    uint32 bufferSize = TP_MESSAGE_LENGTH_WITHOUT_PAYLOAD;
    uint32 bufferTemp = ZERO_VALUE;
#ifdef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    bufferTemp = send(sockfd, tpMessage, bufferSize, ZERO_VALUE);
    if (bufferTemp != bufferSize)
    {
        TP_TRACE_ERROR("Send buffer length = %d Expected = %d", bufferTemp, bufferSize);
        Result = RID_E_NOK;
    }
    else
    {
        TP_TRACE_DEBUG("Send TP message header: ");
        TP_PrintArray((uint8 *)tpMessage, bufferTemp);
        if (tpMessage->Length > TP_PAYLOAD_SIZE_BRUT)
        {
            bufferTemp = send(sockfd, tpMessage->Payload, (tpMessage->Length - TP_PAYLOAD_SIZE_BRUT), ZERO_VALUE);
            if (bufferTemp != (tpMessage->Length - TP_PAYLOAD_SIZE_BRUT))
            {
                TP_TRACE_ERROR("Send buffer length = %d Expected = %d", bufferTemp, (tpMessage->Length - TP_PAYLOAD_SIZE_BRUT));
                Result = RID_E_NOK;
            }
            else
            {
                TP_TRACE_DEBUG("Send TP message Payload: ");
                TP_PrintArray((uint8 *)tpMessage->Payload, bufferTemp);
                Result = RID_E_OK;
            }
        }
        else
        {
            Result = RID_E_OK;
        }
    }

#else /* FEATURE_DUT_SIDE */
    TP_TRACE_DEBUG("start FEATURE_TESTER_SIDE");
    bufferTemp = send(connfd, tpMessage, bufferSize, ZERO_VALUE);
    if (bufferTemp != bufferSize)
    {
        TP_TRACE_ERROR("Send buffer length = %d Expected = %d", bufferTemp, bufferSize);
        Result = RID_E_NOK;
    }
    else
    {
        TP_TRACE_DEBUG("Send TP message header: ");
        TP_PrintArray((uint8 *)tpMessage, bufferTemp);
        if (tpMessage->Length > TP_PAYLOAD_SIZE_BRUT)
        {
            bufferTemp = send(connfd, tpMessage->Payload, (tpMessage->Length - TP_PAYLOAD_SIZE_BRUT), ZERO_VALUE);
            if (bufferTemp != (tpMessage->Length - TP_PAYLOAD_SIZE_BRUT))
            {
                TP_TRACE_ERROR("Send buffer length = %d Expected = %d", bufferTemp, (tpMessage->Length - TP_PAYLOAD_SIZE_BRUT));
                Result = RID_E_NOK;
            }
            else
            {
                TP_TRACE_DEBUG("Send TP message Payload: ");
                TP_PrintArray((uint8 *)tpMessage->Payload, bufferTemp);
                Result = RID_E_OK;
            }
        }
        else
        {
            Result = RID_E_OK;
        }
    }

#endif /* FEATURE_TESTER_SIDE */
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

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
TP_ResultID_t TP_DecodeReceivedMessage(TestabilityProtocolMessage_t *tpMessage)
{
    TP_ResultID_t Result = E_NOT_OK;
    TP_TRACE_DEBUG("start");
    /* Check if the Service ID is supported */
    if (tpMessage->Sid == TESTABILITY_PROTOCOL_SERVICE_ID)
    {
        /* Check if the Protocol Version is supported */
        if (tpMessage->ProtocolVersion == TESTABILITY_PROTOCOL_VERSION)
        {
            /* Check if the Protocol Interface is supported */
            if (tpMessage->InterfaceVersion == TESTABILITY_PROTOCOL_VERSION)
            {

                /* add another check about TID later */
                /* Check if the Group ID is supported */
                if ((tpMessage->Gid <= 0x0CU) || (tpMessage->Gid == 0x7FU))
                {
                    TP_TRACE_INFO("Received GID = %d", tpMessage->Gid);
                    Result = TP_DecodeReceivedGroupID(tpMessage);
                }
                else
                {
                    Result = E_NOT_OK;
                    TP_TRACE_ERROR("Received GID = %d", tpMessage->Gid);
                }
            }
            else
            {
                Result = E_NOT_OK;
                TP_TRACE_ERROR("Received Interface Version = %d", tpMessage->InterfaceVersion);
            }
        }
        else
        {
            Result = E_NOT_OK;
            TP_TRACE_ERROR("Received Protocol Version = %d", tpMessage->ProtocolVersion);
        }
    }
    else
    {
        Result = E_NOT_OK;
        TP_TRACE_ERROR("Received Service ID = %d", tpMessage->Sid);
    }
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

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
TP_ResultID_t TP_DecodeReceivedGroupID(TestabilityProtocolMessage_t *tpMessage)
{
    TP_ResultID_t Result = E_NOT_OK;
    TP_TRACE_DEBUG("start");
    switch (tpMessage->Gid)
    {
    case GID_GENERAL:
    {
        Result = TP_DecodeReceivedGroupGeanral(tpMessage);
    }
    break;
    case GID_UDP:
    {
        Result = TP_DecodeReceivedGroupUDP(tpMessage);
    }
    break;
    case GID_TCP:
    {
        Result = TP_DecodeReceivedGroupTCP(tpMessage);
    }
    break;
    case GID_ICMPv4:
    {
        Result = TP_DecodeReceivedGroupICMPv4(tpMessage);
    }
    break;
    case GID_ICMPv6:
    {
        Result = TP_DecodeReceivedGroupICMPv6(tpMessage);
    }
    break;
    case GID_IPv4:
    {
        Result = TP_DecodeReceivedGroupIPv4(tpMessage);
    }
    case GID_IPv6:
    {
        Result = TP_DecodeReceivedGroupIPv6(tpMessage);
    }
    break;
    case GID_DHCPv4:
    {
        Result = TP_DecodeReceivedGroupDHCPv4(tpMessage);
    }
    break;
    case GID_DHCPv6:
    {
        Result = TP_DecodeReceivedGroupDHCPv6(tpMessage);
    }
    break;
    case GID_ARP:
    {
        Result = TP_DecodeReceivedGroupARP(tpMessage);
    }
    break;
    case GID_NDP:
    {
#ifdef FEATURE_DUT_SIDE
        /* Not yet supported */
        tpMessage->Tid = TID_RESPONSE;
        /* Result ID */
        tpMessage->Rid = RID_E_NTF;
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
#endif
    }
    break;
    case GID_ETH:
    {
        Result = TP_DecodeReceivedGroupEth(tpMessage);
    }
    break;
    case GID_PHY:
    {
        Result = TP_DecodeReceivedGroupPhy(tpMessage);
    }
    break;
    case GID_CC:
    {
        Result = TP_DecodeReceivedGroupCC(tpMessage);
    }
    break;
    default:
    {
        tpMessage->Tid = TID_RESPONSE;
        /* Result ID */
        tpMessage->Rid = RID_E_NTF;
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
    }
    break;
    }
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

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
TP_ResultID_t TP_DecodeReceivedGroupGeanral(TestabilityProtocolMessage_t *tpMessage)
{
    TP_ResultID_t Result = E_NOT_OK;
    TP_TRACE_DEBUG("start");
    switch (tpMessage->Pid)
    {
    case PID_GENERAL_GET_VERSION:
#ifdef FEATURE_TESTER_SIDE
        if (General_cb_func.generalGetVersion_cb != NULL_PTR)
        {
            uint16 majorVer = ZERO_VALUE;
            uint16 minorVer = ZERO_VALUE;
            uint16 patchVer = ZERO_VALUE;
            if (tpMessage->Rid == RID_E_OK)
            {

                majorVer = tpMessage->Payload[0] << 8 | tpMessage->Payload[1];
                minorVer = tpMessage->Payload[2] << 8 | tpMessage->Payload[3];
                patchVer = tpMessage->Payload[4] << 8 | tpMessage->Payload[5];

                General_cb_func.generalGetVersion_cb(tpMessage->Rid, majorVer, minorVer, patchVer);
            }
            else
            {
                General_cb_func.generalGetVersion_cb(tpMessage->Rid, majorVer, minorVer, patchVer);
            }
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_GENERAL_GET_VERSION");
        }
        Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
        Result = TP_GenaralGetVersion(NULL_PTR);
#endif /* FEATURE_TESTER_SIDE */
        break;
    case PID_GENERAL_START_TEST:
#ifdef FEATURE_TESTER_SIDE
        if (General_cb_func.GenaralStartTest_cb != NULL_PTR)
        {
            General_cb_func.GenaralStartTest_cb(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_GENERAL_START_TEST");
        }
        Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
        Result = TP_GenaralStartTest(NULL_PTR);
#endif /* FEATURE_TESTER_SIDE */
        break;
    case PID_GENERAL_END_TEST:
    {
#ifdef FEATURE_TESTER_SIDE
        if (General_cb_func.GenaralEndTest_cb != NULL_PTR)
        {
            General_cb_func.GenaralEndTest_cb(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_GENERAL_END_TEST");
        }
        /* Cancel all threads */
        CancelThreadStatus = TRUE;
        /* Remove all registred CBs */
        General_cb_func = (General_cb_t){NULL_PTR};
        Udp_cb_func = (Udp_cb_t){NULL_PTR};
        Tcp_cb_func = (Tcp_cb_t){NULL_PTR};
        IPv4_cb_func = (Ipv4_cb_t){NULL_PTR};
        Arp_cb_func = (Arp_cb_t){NULL_PTR};
        Eth_cb_func = (Eth_cb_t){NULL_PTR};
        Icmp_cb_func = (Icmp_cb_t){NULL_PTR};
        Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
        uint16 tcId = ZERO_VALUE;
        text tsName = (text){0};
        /* Copy tcId */
        memcpy(&tcId, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy tsName dataLength */
        memcpy(&tsName.dataLength, &tpMessage->Payload[2], sizeof(uint16));
        /*  Copy tsName Data */
        tsName.Data = (uint8 *)calloc(tsName.dataLength, sizeof(uint8));
        if (tsName.Data != NULL_PTR)
        {
            memcpy(tsName.Data, &tpMessage->Payload[4], tsName.dataLength);
            Result = TP_GenaralEndTest(NULL_PTR, tcId, tsName);
            free(tsName.Data);
        }
        else
        {
            Result = RID_E_NOK;
            TP_TRACE_ERROR("Failed to allocate memory for PID_GENERAL_END_TEST");
        }
#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    default:
        tpMessage->Tid = TID_RESPONSE;
        /* Result ID */
        tpMessage->Rid = RID_E_NTF;
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
        break;
    }
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

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
TP_ResultID_t TP_DecodeReceivedGroupUDP(TestabilityProtocolMessage_t *tpMessage)
{
    TP_ResultID_t Result = E_NOT_OK;
    TP_TRACE_DEBUG("start");
    switch (tpMessage->Pid)
    {
    case PID_UDP_CLOSE_SOCKET:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Udp_cb_func.UdpCloseSocket_cb != NULL_PTR)
        {
            Udp_cb_func.UdpCloseSocket_cb(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_UDP_CLOSE_SOCKET");
        }
        Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
        uint16 socketId = ZERO_VALUE;
        /* Copy socket ID */
        memcpy(&socketId, &tpMessage->Payload[0], sizeof(uint16));
        Result = TP_UdpCloseSocket(NULL_PTR, socketId);
#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_UDP_CREATE_AND_BIND:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Udp_cb_func.UdpCreateAndBind_cb != NULL_PTR)
        {
            uint16 socketId = ZERO_VALUE;
            if (tpMessage->Rid == RID_E_OK)
            {
                /* Copy the socketId */
                memcpy(&socketId, &tpMessage->Payload[0], sizeof(uint16));
                Udp_cb_func.UdpCreateAndBind_cb(tpMessage->Rid, socketId);
            }
            else
            {

                Udp_cb_func.UdpCreateAndBind_cb(tpMessage->Rid, ERROR_RETURN_CODE);
            }
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_UDP_CREATE_AND_BIND");
        }
        Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
        boolean tempdoBind = FALSE;
        uint16 templocalPort = ZERO_VALUE;
        ip4addr templocalAddr = {ZERO_VALUE};
        /* Copy doBind parameter */
        tempdoBind = tpMessage->Payload[0];
        /* Copy localPort parameter */
        memcpy(&templocalPort, &tpMessage->Payload[1], sizeof(uint16));
        /* Copy localAddr dataLength parameter */
        memcpy(&templocalAddr.dataLength, &tpMessage->Payload[3], sizeof(uint16));
        /* Copy localAddr Data parameter */
        memcpy(templocalAddr.Data, &tpMessage->Payload[5], templocalAddr.dataLength);
        Result = TP_UdpCreateAndBind(NULL_PTR, tempdoBind, templocalPort, templocalAddr);
#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_UDP_SEND_DATA:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Udp_cb_func.UdpSendData_cb != NULL_PTR)
        {
            Udp_cb_func.UdpSendData_cb(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_UDP_SEND_DATA");
        }
        Result = RID_E_OK;
#else /* FEATURE_DUT_SIDE */
        uint16 socketId = ZERO_VALUE;
        uint16 totalLen = ZERO_VALUE;
        uint16 destPort = ZERO_VALUE;
        ip4addr destAddr = {IPV4_ADDRESS_SIZE, {ZERO_VALUE}};
        vint8 data = {ZERO_VALUE};
        /* Copy SocketId parameter */
        memcpy(&socketId, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy totalLen parameter */
        memcpy(&totalLen, &tpMessage->Payload[2], sizeof(uint16));
        /* Copy destPort parameter */
        memcpy(&destPort, &tpMessage->Payload[4], sizeof(uint16));
        /* Copy destAddr dataLength parameter */
        memcpy(&destAddr.dataLength, &tpMessage->Payload[6], sizeof(uint16));
        /* Copy destAddr Data parameter */
        memcpy(destAddr.Data, &tpMessage->Payload[8], destAddr.dataLength);
        /* Copy data dataLength parameter */
        memcpy(&data.dataLength, &tpMessage->Payload[8 + destAddr.dataLength], sizeof(uint16));
        /* Copy data Data parameter */
        data.Data = (uint8 *)calloc(data.dataLength, sizeof(uint8));
        if (data.Data != NULL_PTR)
        {
            memcpy(data.Data, &tpMessage->Payload[10 + destAddr.dataLength], data.dataLength);
            Result = TP_UdpSendData(NULL_PTR, socketId, totalLen, destPort, destAddr, data);
            free(data.Data);
        }
        else
        {
            Result = RID_E_NOK;
            TP_TRACE_ERROR("Failed to allocate memory for PID_UDP_SEND_DATA");
        }

#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_UDP_RECEIVE_AND_FORWARD:
    {
        if (tpMessage->EVB == ZERO_VALUE)
        {
#ifdef FEATURE_TESTER_SIDE
            if (Udp_cb_func.UdpReceiveAndForward_cb != NULL_PTR)
            {
                uint16 dropCnt = ZERO_VALUE;
                if (tpMessage->Rid == RID_E_OK)
                {
                    /* Copy the dropCnt */
                    memcpy(&dropCnt, &tpMessage->Payload[0], sizeof(uint16));
                    Udp_cb_func.UdpReceiveAndForward_cb(tpMessage->Rid, dropCnt);
                }
                else
                {
                    Udp_cb_func.UdpReceiveAndForward_cb(tpMessage->Rid, ERROR_RETURN_CODE);
                }
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_UDP_RECEIVE_AND_FORWARD");
            }
            Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
            uint16 socketId = ZERO_VALUE;
            uint16 maxFwd = ZERO_VALUE;
            uint16 maxLen = ZERO_VALUE;
            /* Copy SocketId parameter */
            memcpy(&socketId, &tpMessage->Payload[0], sizeof(uint16));
            /* Copy maxFwd parameter */
            memcpy(&maxFwd, &tpMessage->Payload[2], sizeof(uint16));
            /* Copy maxLen parameter */
            memcpy(&maxLen, &tpMessage->Payload[4], sizeof(uint16));
            Result = TP_UdpReceiveAndForward(NULL_PTR, NULL_PTR, socketId, maxFwd, maxLen);
#endif /* FEATURE_TESTER_SIDE */
        }
        else
        {
#ifdef FEATURE_TESTER_SIDE
            if (Udp_cb_func.UdpReceiveAndForward_event_cb != NULL_PTR)
            {
                uint16 fullLen = ZERO_VALUE;
                uint16 srcPort = ZERO_VALUE;
                ip4addr srcAddr = {4, {ZERO_VALUE}};
                vint8 payload = {ZERO_VALUE, ZERO_VALUE};
                if (tpMessage->Rid == RID_E_OK)
                {
                    /* Copy fullLen parametr */
                    memcpy(&fullLen, &tpMessage->Payload[0], sizeof(uint16));
                    /* Copy srcPort parametr */
                    memcpy(&srcPort, &tpMessage->Payload[2], sizeof(uint16));
                    /* Copy srcAddr.dataLength parametr */
                    memcpy(&srcAddr.dataLength, &tpMessage->Payload[4], sizeof(uint16));
                    /* Copy srcAddr.Data parametr */
                    memcpy(srcAddr.Data, &tpMessage->Payload[6], srcAddr.dataLength);
                    /* Copy payload.dataLength parametr */
                    memcpy(&payload.dataLength, &tpMessage->Payload[6 + srcAddr.dataLength], sizeof(uint16));

                    payload.Data = (uint8 *)calloc(payload.dataLength, sizeof(uint8));
                    if (payload.Data != NULL_PTR)
                    {
                        /* Copy payload.Data parametr */
                        memcpy(payload.Data, &tpMessage->Payload[8 + srcAddr.dataLength], payload.dataLength);
                        Udp_cb_func.UdpReceiveAndForward_event_cb(tpMessage->Rid, fullLen, srcPort, srcAddr, payload);
                        free(payload.Data);
                        Result = RID_E_OK;
                    }
                    else
                    {
                        Result = RID_E_NOK;
                        TP_TRACE_ERROR("Failed to allocate memory forUdpReceiveAndForward event");
                    }
                }
                else
                {
                    Result = RID_E_OK;
                    Udp_cb_func.UdpReceiveAndForward_event_cb(tpMessage->Rid, fullLen, srcPort, srcAddr, payload);
                }
            }
            else
            {
                Result = RID_E_OK;
                TP_TRACE_WARNING("User did not define a callback function for UdpReceiveAndForward event");
            }
#endif /* FEATURE_TESTER_SIDE */
        }
    }
    break;
    case PID_UDP_CONFIGURE_SOCKET:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Udp_cb_func.UdpConfigureSocket_cb != NULL_PTR)
        {
            Udp_cb_func.UdpConfigureSocket_cb(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_UDP_CONFIGURE_SOCKET");
        }
        Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
        uint16 socketId = ZERO_VALUE;
        uint16 paramId = ZERO_VALUE;
        vint8 paramVal = {ZERO_VALUE};

        /* Copy socketId parameter*/
        memcpy(&socketId, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy paramId parameter*/
        memcpy(&paramId, &tpMessage->Payload[2], sizeof(uint16));
        /* Copy paramVal dataLength*/
        memcpy(&paramVal.dataLength, &tpMessage->Payload[4], sizeof(uint16));
        /* Copy paramVal Data */
        paramVal.Data = (uint8 *)calloc(paramVal.dataLength, sizeof(uint8));
        if (paramVal.Data != NULL_PTR)
        {
            memcpy(paramVal.Data, &tpMessage->Payload[6], paramVal.dataLength);
            Result = TP_UdpConfigureSocket(NULL_PTR, socketId, paramId, paramVal);
            free(paramVal.Data);
        }
        else
        {
            Result = RID_E_NOK;
            TP_TRACE_ERROR("Failed to allocate memory for PID_UDP_CONFIGURE_SOCKET");
        }
#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_UDP_SHUTDOWN:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Udp_cb_func.UdpShutdown_cb != NULL_PTR)
        {
            Udp_cb_func.UdpShutdown_cb(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_UDP_SHUTDOWN");
        }
        Result = RID_E_OK;
#else /* FEATURE_DUT_SIDE */
        uint16 socketId = ZERO_VALUE;
        uint8 typeId = ZERO_VALUE;
        /* Copy socket ID */
        memcpy(&socketId, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy typeId parameter */
        typeId = tpMessage->Payload[2];
        Result = TP_UdpShutdown(NULL_PTR, socketId, typeId);

#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    default:
    {
        tpMessage->Tid = TID_RESPONSE;
        /* Result ID */
        tpMessage->Rid = RID_E_NTF;
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
    }
    break;
    }
    TP_TRACE_DEBUG("exit Result = %d", Result);
    return Result;
}

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
TP_ResultID_t TP_DecodeReceivedGroupTCP(TestabilityProtocolMessage_t *tpMessage)
{
    TP_ResultID_t Result = E_NOT_OK;
    TP_TRACE_DEBUG("start");

    switch (tpMessage->Pid)
    {
    case PID_TCP_CLOSE_SOCKET:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Tcp_cb_func.TcpCloseSocket_cb != NULL_PTR)
        {
            Tcp_cb_func.TcpCloseSocket_cb(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_TCP_CLOSE_SOCKET");
        }
        Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
        uint16 socketId = ZERO_VALUE;
        boolean abort = FALSE;
        /* Copy socket ID */
        memcpy(&socketId, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy abort parameter */
        abort = tpMessage->Payload[2];
        Result = TP_TcpCloseSocket(NULL_PTR, socketId, abort);
#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_TCP_CREATE_AND_BIND:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Tcp_cb_func.TcpCreateAndBind_cb != NULL_PTR)
        {
            uint16 socketId = ZERO_VALUE;
            if (tpMessage->Rid == ZERO_VALUE)
            {
                /* Copy the socketId */
                memcpy(&socketId, &tpMessage->Payload[0], sizeof(uint16));
                Tcp_cb_func.TcpCreateAndBind_cb(tpMessage->Rid, socketId);
            }
            else
            {
                Tcp_cb_func.TcpCreateAndBind_cb(tpMessage->Rid, socketId);
            }
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_TCP_CREATE_AND_BIND");
        }
        Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
        boolean tempdoBind = FALSE;
        uint16 templocalPort = ZERO_VALUE;
        ip4addr templocalAddr = {ZERO_VALUE};
        /* Copy doBind parameter */
        tempdoBind = tpMessage->Payload[0];
        /* Copy localPort parameter */
        memcpy(&templocalPort, &tpMessage->Payload[1], sizeof(uint16));
        /* Copy localAddr dataLength parameter */
        memcpy(&templocalAddr.dataLength, &tpMessage->Payload[3], sizeof(uint16));
        /* Copy localAddr Data parameter */
        memcpy(templocalAddr.Data, &tpMessage->Payload[5], templocalAddr.dataLength);
        TP_PrintArray(tpMessage->Payload, (sizeof(boolean) + sizeof(uint16) + sizeof(uint16) + templocalAddr.dataLength));
        Result = TP_TcpCreateAndBind(NULL_PTR, tempdoBind, templocalPort, templocalAddr);
#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_TCP_SEND_DATA:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Tcp_cb_func.TcpSendData_cb != NULL_PTR)
        {
            Tcp_cb_func.TcpSendData_cb(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_TCP_SEND_DATA");
        }
        Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
        uint16 socketId = ZERO_VALUE;
        uint16 totalLen = ZERO_VALUE;
        uint8 flags = ZERO_VALUE;
        vint8 data = {ZERO_VALUE};
        /* Copy SocketId parameter */
        memcpy(&socketId, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy totalLen parameter */
        memcpy(&totalLen, &tpMessage->Payload[2], sizeof(uint16));
        /* Copy flags parameter */
        memcpy(&flags, &tpMessage->Payload[4], sizeof(uint8));
        /* Copy data dataLength parameter */
        memcpy(&data.dataLength, &tpMessage->Payload[5], sizeof(uint16));
        /* Copy data Data parameter */
        data.Data = (uint8 *)calloc(data.dataLength, sizeof(uint8));
        if (data.Data != NULL_PTR)
        {
            memcpy(data.Data, &tpMessage->Payload[7], data.dataLength);
            Result = TP_TcpSendData(NULL_PTR, socketId, totalLen, flags, data);
            free(data.Data);
        }
        else
        {
            Result = RID_E_NOK;
            TP_TRACE_ERROR("Failed to allocate memory for PID_UDP_SEND_DATA");
        }
#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_TCP_RECEIVE_AND_FORWARD:
    {
        if (tpMessage->EVB == ZERO_VALUE)
        {
#ifdef FEATURE_TESTER_SIDE
            if (Tcp_cb_func.TcpReceiveAndForward_cb != NULL_PTR)
            {
                uint16 dropCnt = ZERO_VALUE;
                if (tpMessage->Rid == RID_E_OK)
                {
                    /* Copy the dropCnt */
                    memcpy(&dropCnt, &tpMessage->Payload[0], sizeof(uint16));
                    Tcp_cb_func.TcpReceiveAndForward_cb(tpMessage->Rid, dropCnt);
                }
                else
                {
                    Tcp_cb_func.TcpReceiveAndForward_cb(tpMessage->Rid, dropCnt);
                }
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_TCP_RECEIVE_AND_FORWARD");
            }
            Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
            uint16 socketId = ZERO_VALUE;
            uint16 maxFwd = ZERO_VALUE;
            uint16 maxLen = ZERO_VALUE;
            /* Copy SocketId parameter */
            memcpy(&socketId, &tpMessage->Payload[0], sizeof(uint16));
            /* Copy maxFwd parameter */
            memcpy(&maxFwd, &tpMessage->Payload[2], sizeof(uint16));
            /* Copy maxLen parameter */
            memcpy(&maxLen, &tpMessage->Payload[4], sizeof(uint16));
            Result = TP_TcpReceiveAndForward(NULL_PTR, NULL_PTR, socketId, maxFwd, maxLen);
#endif /* FEATURE_TESTER_SIDE */
        }
        else
        {
#ifdef FEATURE_TESTER_SIDE
            if (Tcp_cb_func.TcpReceiveAndForward_event_cb != NULL_PTR)
            {
                uint16 fullLen = ZERO_VALUE;
                vint8 payload = {ZERO_VALUE, ZERO_VALUE};
                if (tpMessage->Rid == RID_E_OK)
                {

                    /* Copy fullLen parametr */
                    memcpy(&fullLen, &tpMessage->Payload[0], sizeof(uint16));
                    /* Copy payload.dataLength parametr */
                    memcpy(&payload.dataLength, &tpMessage->Payload[2], sizeof(uint16));

                    payload.Data = (uint8 *)calloc(payload.dataLength, sizeof(uint8));
                    if (payload.Data != NULL_PTR)
                    {
                        /* Copy payload.Data parametr */
                        memcpy(payload.Data, &tpMessage->Payload[4], payload.dataLength);
                        Tcp_cb_func.TcpReceiveAndForward_event_cb(tpMessage->Rid, fullLen, payload);
                        free(payload.Data);
                        Result = RID_E_OK;
                    }
                    else
                    {
                        Result = RID_E_NOK;
                        TP_TRACE_ERROR("Failed to allocate memory for TcpReceiveAndForward event");
                    }
                }
                else
                {
                    Tcp_cb_func.TcpReceiveAndForward_event_cb(tpMessage->Rid, fullLen, payload);
                    Result = RID_E_OK;
                }
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for TcpReceiveAndForward event");
                Result = RID_E_OK;
            }
#endif /* FEATURE_TESTER_SIDE */
        }
    }
    break;
    case PID_TCP_LISTEN_AND_ACCEPT:
    {
        if (tpMessage->EVB == ZERO_VALUE)
        {

#ifdef FEATURE_TESTER_SIDE
            if (Tcp_cb_func.TcpListenAndAccept_cb != NULL_PTR)
            {
                Tcp_cb_func.TcpListenAndAccept_cb(tpMessage->Rid);
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_TCP_LISTEN_AND_ACCEPT");
            }
            Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
            uint16 listenSocketId = ZERO_VALUE;
            uint16 maxCon = ZERO_VALUE;
            /* Copy listenSocketId parameter */
            memcpy(&listenSocketId, &tpMessage->Payload[0], sizeof(uint16));
            /* Copy maxCon parameter */
            memcpy(&maxCon, &tpMessage->Payload[2], sizeof(uint16));
            Result = TP_TcpListenAndAccept(NULL_PTR, NULL_PTR, listenSocketId, maxCon);
#endif /* FEATURE_TESTER_SIDE */
        }
        else
        {
#ifdef FEATURE_TESTER_SIDE
            if (Tcp_cb_func.TcpListenAndAccept_event_cb != NULL_PTR)
            {
                uint16 listenSocketId = ZERO_VALUE;
                uint16 newSocketId = ZERO_VALUE;
                uint16 port = ZERO_VALUE;
                ip4addr address = {ZERO_VALUE};
                if (tpMessage->Rid)
                {
                    /* Copy listenSocketId parameter */
                    memcpy(&listenSocketId, &tpMessage->Payload[0], sizeof(uint16));
                    /* Copy localPort parameter */
                    memcpy(&newSocketId, &tpMessage->Payload[2], sizeof(uint16));
                    /* Copy port parameter */
                    memcpy(&port, &tpMessage->Payload[4], sizeof(uint16));
                    /* Copy localAddr dataLength parameter */
                    memcpy(&address.dataLength, &tpMessage->Payload[6], sizeof(uint16));
                    /* Copy localAddr Data parameter */
                    memcpy(address.Data, &tpMessage->Payload[8], address.dataLength);
                    Tcp_cb_func.TcpListenAndAccept_event_cb(tpMessage->Rid, listenSocketId, newSocketId, port, address);
                }
                else
                {
                    Tcp_cb_func.TcpListenAndAccept_event_cb(tpMessage->Rid, listenSocketId, newSocketId, port, address);
                }
            }
            else
            {
                TP_TRACE_WARNING("User did not define a callback function for PID_TCP_LISTEN_AND_ACCEPT");
            }
            Result = RID_E_OK;
#endif /* FEATURE_TESTER_SIDE */
        }
    }
    break;
    case PID_TCP_CONNECT:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Tcp_cb_func.TcpConnect_cb != NULL_PTR)
        {
            Tcp_cb_func.TcpConnect_cb(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_TCP_CONNECT");
        }
        Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
        uint16 socketId = ZERO_VALUE;
        uint16 destPort = ZERO_VALUE;
        ip4addr destAddr = (ip4addr){ZERO_VALUE};
        /* Copy socketId parameter */
        memcpy(&socketId, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy destPort parameter */
        memcpy(&destPort, &tpMessage->Payload[2], sizeof(uint16));
        /* Copy destAddr dataLength parameter */
        memcpy(&destAddr.dataLength, &tpMessage->Payload[4], sizeof(uint16));
        /* Copy destAddr Data parameter */
        memcpy(destAddr.Data, &tpMessage->Payload[6], destAddr.dataLength);
        /* Result ID */
        tpMessage->Rid = RID_E_OK;
        Result = TP_TcpConnect(NULL_PTR, socketId, destPort, destAddr);
#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_TCP_CONFIGURE_SOCKET:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Tcp_cb_func.TcpConfigureSocket_cb != NULL_PTR)
        {
            Tcp_cb_func.TcpConfigureSocket_cb(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_TCP_CONFIGURE_SOCKET");
        }
        Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
        uint16 socketId = ZERO_VALUE;
        uint16 paramId = ZERO_VALUE;
        vint8 paramVal = {ZERO_VALUE};

        /* Copy socketId parameter*/
        memcpy(&socketId, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy paramId parameter*/
        memcpy(&paramId, &tpMessage->Payload[2], sizeof(uint16));
        /* Copy paramVal dataLength*/
        memcpy(&paramVal.dataLength, &tpMessage->Payload[4], sizeof(uint16));
        /* Copy paramVal Data */
        paramVal.Data = (uint8 *)calloc(paramVal.dataLength, sizeof(uint8));
        if (paramVal.Data != NULL_PTR)
        {
            memcpy(paramVal.Data, &tpMessage->Payload[6], paramVal.dataLength);
            Result = TP_TcpConfigureSocket(NULL_PTR, socketId, paramId, paramVal);
            free(paramVal.Data);
        }
        else
        {
            Result = RID_E_NOK;
            TP_TRACE_ERROR("Failed to allocate memory for PID_TCP_CONFIGURE_SOCKET");
        }
#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_TCP_SHUTDOWN:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Tcp_cb_func.TcpShutdown_cb != NULL_PTR)
        {
            Tcp_cb_func.TcpShutdown_cb(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_TCP_SHUTDOWN");
        }
        Result = RID_E_OK;
#else /* FEATURE_DUT_SIDE */
        uint16 socketId = ZERO_VALUE;
        uint8 typeId = ZERO_VALUE;
        /* Copy socket ID */
        memcpy(&socketId, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy typeId parameter */
        typeId = tpMessage->Payload[2];
        Result = TP_TcpShutdown(NULL_PTR, socketId, typeId);

#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_TCP_SOCKET_STATE:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Tcp_cb_func.TcpSocketState_cb != NULL_PTR)
        {
            uint8 socketState = ZERO_VALUE;
            if (tpMessage->Rid == RID_E_OK)
            {
                /* Copy the socket state */
                memcpy(&socketState, &tpMessage->Payload[0], sizeof(uint8));
                Tcp_cb_func.TcpSocketState_cb(tpMessage->Rid, socketState);
            }
            else
            {
                Tcp_cb_func.TcpSocketState_cb(tpMessage->Rid, ERROR_RETURN_CODE);
            }
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_TCP_SOCKET_STATE");
        }
        Result = RID_E_OK;
#else /* FEATURE_DUT_SIDE */
        uint16 localPort = ZERO_VALUE;
        uint16 destPort = ZERO_VALUE;
        ip4addr localAddr = {ZERO_VALUE};
        ip4addr destAddr = {ZERO_VALUE};
        /* Copy localPort */
        memcpy(&localPort, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy localAddr.dataLength parameter */
        memcpy(&localAddr.dataLength, &tpMessage->Payload[2], sizeof(uint16));
        /* Copy localAddr.Data parameter */
        memcpy(localAddr.Data, &tpMessage->Payload[4], localAddr.dataLength);
        /* Copy destPort */
        memcpy(&destPort, &tpMessage->Payload[4 + localAddr.dataLength], sizeof(uint16));
        /* Copy destAddr.dataLength parameter */
        memcpy(&destAddr.dataLength, &tpMessage->Payload[6 + localAddr.dataLength], sizeof(uint16));
        /* Copy destAddr.Data parameter */
        memcpy(destAddr.Data, &tpMessage->Payload[8 + localAddr.dataLength], destAddr.dataLength);
        Result = TP_TcpSocketState(NULL_PTR, localPort, localAddr, destPort, destAddr);

#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    default:
        tpMessage->Tid = TID_RESPONSE;
        /* Result ID */
        tpMessage->Rid = RID_E_NTF;
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
        break;
    }
    return Result;
}

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
TP_ResultID_t TP_DecodeReceivedGroupICMPv4(TestabilityProtocolMessage_t *tpMessage)
{
    TP_ResultID_t Result = E_NOT_OK;
    TP_TRACE_DEBUG("start");
    switch (tpMessage->Pid)
    {
    case PID_ICMPv4_ECHO_REQUEST:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Icmp_cb_func.IcmpEchoRequest_cb != NULL_PTR)
        {
            Icmp_cb_func.IcmpEchoRequest_cb(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for IcmpEchoRequest_cb");
        }
        Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
        text ifName = {ZERO_VALUE};
        ip4addr destAddr = {ZERO_VALUE};
        vint8 data = {ZERO_VALUE};
        /* Copy ifName.dataLength */
        memcpy(&ifName.dataLength, &tpMessage->Payload[0], sizeof(uint16));
        if (ifName.dataLength != 0)
        {

            ifName.Data = (uint8 *)calloc(ifName.dataLength, sizeof(uint8));
            if (ifName.Data == NULL_PTR)
            {
                Result = RID_E_NOK;
                TP_TRACE_ERROR("Failed to allocate memory for  IcmpEchoRequest");
                break;
            }
            else
            {
                /* Copy ifName.Data */
                memcpy(ifName.Data, &tpMessage->Payload[2], ifName.dataLength);
            }
        }

        memcpy(&destAddr.dataLength, &tpMessage->Payload[ifName.dataLength + 2], sizeof(uint16));
        memcpy(destAddr.Data, &tpMessage->Payload[ifName.dataLength + 4], destAddr.dataLength);
        memcpy(&data.dataLength, &tpMessage->Payload[ifName.dataLength + destAddr.dataLength + 4], sizeof(uint16));
        data.Data = (uint8 *)calloc(data.dataLength, sizeof(uint8));
        if (data.Data == NULL_PTR)
        {
            Result = RID_E_NOK;
            TP_TRACE_ERROR("Failed to allocate memory for IcmpEchoRequest");
            free(ifName.Data);
            break;
        }
        else
        {
            memcpy(data.Data, &tpMessage->Payload[ifName.dataLength + 6 + destAddr.dataLength], data.dataLength);
        }
        Result = TP_IcmpEchoRequest(NULL_PTR, ifName, destAddr, data);
        if (ifName.dataLength != 0)
        {
            free(ifName.Data);
        }
        free(data.Data);
#endif /* FEATURE_DUT_SIDE */
        break;
    }
    default:
        tpMessage->Tid = TID_RESPONSE;
        /* Result ID */
        tpMessage->Rid = RID_E_NTF;
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
        break;
    }
    return Result;
}

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
TP_ResultID_t TP_DecodeReceivedGroupICMPv6(TestabilityProtocolMessage_t *tpMessage)
{
    TP_ResultID_t Result = E_NOT_OK;
    TP_TRACE_DEBUG("start");
    switch (tpMessage->Pid)
    {
    case PID_ICMPv6_ECHO_REQUEST:
        Result = E_NOT_OK;
        break;
    default:
        tpMessage->Tid = TID_RESPONSE;
        /* Result ID */
        tpMessage->Rid = RID_E_NTF;
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
        break;
    }
    return Result;
}

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
TP_ResultID_t TP_DecodeReceivedGroupIPv4(TestabilityProtocolMessage_t *tpMessage)
{
    TP_ResultID_t Result = E_NOT_OK;
    TP_TRACE_DEBUG("start");
    switch (tpMessage->Pid)
    {
    case PID_IPv4_STATIC_ADDRESS:
    {

#ifdef FEATURE_TESTER_SIDE
        if (IPv4_cb_func.Ipv4StaticAddress != NULL_PTR)
        {
            IPv4_cb_func.Ipv4StaticAddress(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_IPv4_STATIC_ADDRESS");
        }
        Result = RID_E_OK;
#else /* FEATURE_DUT_SIDE */
        uint8 netmask = ZERO_VALUE;
        text ifName = {ZERO_VALUE};
        ip4addr addr = {ZERO_VALUE};
        /* Copy ifName.dataLength */
        memcpy(&ifName.dataLength, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy ifName.Data */
        ifName.Data = (uint8 *)calloc(ifName.dataLength, sizeof(uint8));
        if (ifName.Data != NULL_PTR)
        {
            memcpy(ifName.Data, &tpMessage->Payload[2], ifName.dataLength);
            /* Copy addr.dataLength */
            memcpy(&addr.dataLength, &tpMessage->Payload[2 + ifName.dataLength], sizeof(uint16));
            /* Copy addr.Data */
            memcpy(addr.Data, &tpMessage->Payload[4 + ifName.dataLength], addr.dataLength);
            /* Copy netmask parameter */
            netmask = tpMessage->Payload[4 + ifName.dataLength + addr.dataLength];
            Result = TP_Ipv4StaticAddress(NULL_PTR, ifName, addr, netmask);
            free(ifName.Data);
        }
        else
        {
            Result = RID_E_NOK;
            TP_TRACE_ERROR("Failed to allocate memory for PID_IPv4_STATIC_ADDRESS");
        }

#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_IPv4_STATIC_ROUTE:
    {

#ifdef FEATURE_TESTER_SIDE
        if (IPv4_cb_func.Ipv4StaticRoute != NULL_PTR)
        {
            IPv4_cb_func.Ipv4StaticRoute(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_IPv4_STATIC_ADDRESS");
        }
        Result = RID_E_OK;
#else /* FEATURE_DUT_SIDE */
        uint8 netMask = ZERO_VALUE;
        text ifName = {ZERO_VALUE};
        ip4addr gateway = {ZERO_VALUE};
        ip4addr subNet = {ZERO_VALUE};
        /* Copy ifName.dataLength */
        memcpy(&ifName.dataLength, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy ifName.Data */
        ifName.Data = (uint8 *)calloc(ifName.dataLength, sizeof(uint8));
        if (ifName.Data != NULL_PTR)
        {
            memcpy(ifName.Data, &tpMessage->Payload[2], ifName.dataLength);
            /* Copy subNet.dataLength */
            memcpy(&subNet.dataLength, &tpMessage->Payload[2 + ifName.dataLength], sizeof(uint16));
            /* Copy subNet.Data */
            memcpy(subNet.Data, &tpMessage->Payload[4 + ifName.dataLength], subNet.dataLength);
            /* Copy netmask parameter */
            netMask = tpMessage->Payload[4 + ifName.dataLength + subNet.dataLength];
            /* Copy gateway.dataLength */
            memcpy(&gateway.dataLength, &tpMessage->Payload[5 + ifName.dataLength + subNet.dataLength], sizeof(uint16));
            /* Copy gateway.Data */
            memcpy(gateway.Data, &tpMessage->Payload[7 + ifName.dataLength + subNet.dataLength], gateway.dataLength);
            Result = TP_Ipv4StaticRoute(NULL_PTR, ifName, subNet, netMask, gateway);
            free(ifName.Data);
        }
        else
        {
            Result = RID_E_NOK;
            TP_TRACE_ERROR("Failed to allocate memory for PID_IPv4_STATIC_ROUTE");
        }

#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    default:
        tpMessage->Tid = TID_RESPONSE;
        /* Result ID */
        tpMessage->Rid = RID_E_NTF;
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
        break;
    }
    return Result;
}

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
TP_ResultID_t TP_DecodeReceivedGroupIPv6(TestabilityProtocolMessage_t *tpMessage)
{
    TP_ResultID_t Result = E_NOT_OK;
    TP_TRACE_DEBUG("start");
    switch (tpMessage->Pid)
    {
    case PID_IPv6_STATIC_ADDRESS:
        Result = E_NOT_OK;
        break;
    case PID_IPv6_STATIC_ROUTE:
        Result = E_NOT_OK;
        break;
    default:
        tpMessage->Tid = TID_RESPONSE;
        /* Result ID */
        tpMessage->Rid = RID_E_NTF;
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
        break;
    }
    return Result;
}

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
TP_ResultID_t TP_DecodeReceivedGroupDHCPv4(TestabilityProtocolMessage_t *tpMessage)
{
    TP_ResultID_t Result = E_NOT_OK;
    TP_TRACE_DEBUG("start");
    switch (tpMessage->Pid)
    {
    case PID_DHCPv4_INIT_CLIENT:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Dhcpv4_cb_func.DhcpInitClient != NULL_PTR)
        {
            Dhcpv4_cb_func.DhcpInitClient(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_DHCPv4_INIT_CLIENT");
        }
        Result = RID_E_OK;
#else /* FEATURE_DUT_SIDE */
        text ifName = {ZERO_VALUE};
        /* Copy ifName.dataLength */
        memcpy(&ifName.dataLength, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy ifName.Data */
        ifName.Data = (uint8 *)calloc(ifName.dataLength, sizeof(uint8));
        if (ifName.Data != NULL_PTR)
        {
            memcpy(ifName.Data, &tpMessage->Payload[2], ifName.dataLength);
            Result = TP_DhcpInitClient(NULL_PTR, ifName);
            free(ifName.Data);
        }
        else
        {
            Result = RID_E_NOK;
            TP_TRACE_ERROR("Failed to allocate memory for PID_DHCPv4_INIT_CLIENT");
        }

#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_DHCPv4_STOP_CLIENT:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Dhcpv4_cb_func.DhcpStopClient != NULL_PTR)
        {
            Dhcpv4_cb_func.DhcpStopClient(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_DHCPv4_STOP_CLIENT");
        }
        Result = RID_E_OK;
#else /* FEATURE_DUT_SIDE */
        text ifName = {ZERO_VALUE};
        /* Copy ifName.dataLength */
        memcpy(&ifName.dataLength, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy ifName.Data */
        ifName.Data = (uint8 *)calloc(ifName.dataLength, sizeof(uint8));
        if (ifName.Data != NULL_PTR)
        {
            memcpy(ifName.Data, &tpMessage->Payload[2], ifName.dataLength);
            Result = TP_DhcpStopClient(NULL_PTR, ifName);
            free(ifName.Data);
        }
        else
        {
            Result = RID_E_NOK;
            TP_TRACE_ERROR("Failed to allocate memory for PID_DHCPv4_STOP_CLIENT");
        }

#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_DHCPv4_SET_OPTION:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Dhcpv4_cb_func.DhcpSetClientOptions != NULL_PTR)
        {
            Dhcpv4_cb_func.DhcpSetClientOptions(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_DHCPv4_SET_OPTION");
        }
        Result = RID_E_OK;
#else /* FEATURE_DUT_SIDE */
        text ifName = {ZERO_VALUE};
        uint8 code = ZERO_VALUE;
        vint8 value = {ZERO_VALUE};
        /* Copy ifName.dataLength */
        memcpy(&ifName.dataLength, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy ifName.Data */
        ifName.Data = (uint8 *)calloc(ifName.dataLength, sizeof(uint8));
        if (ifName.Data != NULL_PTR)
        {
            memcpy(ifName.Data, &tpMessage->Payload[2], ifName.dataLength);
            memcpy(&code, &tpMessage->Payload[2 + ifName.dataLength], sizeof(uint8));
            /* Copy value.dataLength */
            memcpy(&value.dataLength, &tpMessage->Payload[3 + ifName.dataLength], sizeof(uint16));
            /* Copy value.Data */
            value.Data = (uint8 *)calloc(value.dataLength, sizeof(uint8));
            if (value.Data != NULL_PTR)
            {
                memcpy(value.Data, &tpMessage->Payload[5 + ifName.dataLength], value.dataLength);
                Result = TP_DhcpSetClientOptions(NULL_PTR, ifName, code, value);
                free(value.Data);
            }
            else
            {
                Result = RID_E_NOK;
                TP_TRACE_ERROR("Failed to allocate memory for PID_DHCPv4_STOP_CLIENT");
            }
            free(ifName.Data);
        }
        else
        {
            Result = RID_E_NOK;
            TP_TRACE_ERROR("Failed to allocate memory for PID_DHCPv4_SET_OPTION");
        }

#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    default:
        tpMessage->Tid = TID_RESPONSE;
        /* Result ID */
        tpMessage->Rid = RID_E_NTF;
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
        break;
    }
    return Result;
}

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
TP_ResultID_t TP_DecodeReceivedGroupDHCPv6(TestabilityProtocolMessage_t *tpMessage)
{
    TP_ResultID_t Result = E_NOT_OK;
    TP_TRACE_DEBUG("start");
    switch (tpMessage->Pid)
    {
    case PID_DHCPv6_INIT_CLIENT:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Dhcpv6_cb_func.Dhcpv6InitClient != NULL_PTR)
        {
            Dhcpv6_cb_func.Dhcpv6InitClient(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_DHCPv6_INIT_CLIENT");
        }
        Result = RID_E_OK;
#else /* FEATURE_DUT_SIDE */
        text ifName = {ZERO_VALUE};
        /* Copy ifName.dataLength */
        memcpy(&ifName.dataLength, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy ifName.Data */
        ifName.Data = (uint8 *)calloc(ifName.dataLength, sizeof(uint8));
        if (ifName.Data != NULL_PTR)
        {
            memcpy(ifName.Data, &tpMessage->Payload[2], ifName.dataLength);
            Result = TP_Dhcpv6InitClient(NULL_PTR, ifName);
            free(ifName.Data);
        }
        else
        {
            Result = RID_E_NOK;
            TP_TRACE_ERROR("Failed to allocate memory for PID_DHCPv6_INIT_CLIENT");
        }

#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_DHCPv6_STOP_CLIENT:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Dhcpv6_cb_func.Dhcpv6StopClient != NULL_PTR)
        {
            Dhcpv6_cb_func.Dhcpv6StopClient(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_DHCPv6_STOP_CLIENT");
        }
        Result = RID_E_OK;
#else /* FEATURE_DUT_SIDE */
        text ifName = {ZERO_VALUE};
        /* Copy ifName.dataLength */
        memcpy(&ifName.dataLength, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy ifName.Data */
        ifName.Data = (uint8 *)calloc(ifName.dataLength, sizeof(uint8));
        if (ifName.Data != NULL_PTR)
        {
            memcpy(ifName.Data, &tpMessage->Payload[2], ifName.dataLength);
            Result = TP_Dhcpv6StopClient(NULL_PTR, ifName);
            free(ifName.Data);
        }
        else
        {
            Result = RID_E_NOK;
            TP_TRACE_ERROR("Failed to allocate memory for PID_DHCPv6_STOP_CLIENT");
        }

#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_DHCPv4_SET_OPTION:
    {
        Result = RID_E_NOK;
    }
    break;
    default:
        tpMessage->Tid = TID_RESPONSE;
        /* Result ID */
        tpMessage->Rid = RID_E_NTF;
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
        break;
    }
    return Result;
}

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
TP_ResultID_t TP_DecodeReceivedGroupARP(TestabilityProtocolMessage_t *tpMessage)
{
    TP_ResultID_t Result = E_NOT_OK;
    TP_TRACE_DEBUG("start");
    switch (tpMessage->Pid)
    {
    case PID_ARP_CLEAR_CACHE:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Arp_cb_func.ClearArpCache != NULL_PTR)
        {
            Arp_cb_func.ClearArpCache(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_ARP_CLEAR_CACHE");
        }
        Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
        Result = TP_ClearArpCache(NULL_PTR);
#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_ARP_ADD_STATIC_ENTRY:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Arp_cb_func.AddStaticEntry != NULL_PTR)
        {

            Arp_cb_func.AddStaticEntry(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_ARP_ADD_STATIC_ENTRY");
        }
        Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
        vint8 addip = {ZERO_VALUE};
        vint8 addmac = {ZERO_VALUE};
        /* Copy addip dataLength parameter */
        memcpy(&addip.dataLength, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy addip Data parameter */
        addip.Data = (uint8 *)calloc(addip.dataLength, sizeof(uint8));
        if (addip.Data != NULL_PTR)
        {
            /* Copy addip Data parameter */
            memcpy(addip.Data, &tpMessage->Payload[2], addip.dataLength);
            /* Copy addmac dataLength parameter */
            memcpy(&addmac.dataLength, &tpMessage->Payload[2 + addip.dataLength], sizeof(uint16));
            addmac.Data = (uint8 *)calloc(addmac.dataLength, sizeof(uint8));
            if (addmac.Data != NULL_PTR)
            {
                /* Copy addmac Data parameter */
                memcpy(addmac.Data, &tpMessage->Payload[2 + addip.dataLength + 2], addmac.dataLength);
                Result = TP_AddStaticEntry(NULL_PTR, addip, addmac);
                free(addip.Data);
                free(addmac.Data);
            }
            else
            {
                Result = RID_E_NOK;
                TP_TRACE_ERROR("Failed to allocate memory for PID_ARP_ADD_STATIC_ENTRY");
            }
        }
        else
        {
            Result = RID_E_NOK;
            TP_TRACE_ERROR("Failed to allocate memory for PID_ARP_ADD_STATIC_ENTRY");
        }
#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_ARP_DELETE_STATIC_ENTRY:
    {
#ifdef FEATURE_TESTER_SIDE
        if (Arp_cb_func.DeleteStaticEntry != NULL_PTR)
        {

            Arp_cb_func.DeleteStaticEntry(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_ARP_DELETE_STATIC_ENTRY");
        }
        Result = RID_E_OK;
#else  /* FEATURE_DUT_SIDE */
        vint8 addip = {ZERO_VALUE};
        /* Copy addip dataLength parameter */
        memcpy(&addip.dataLength, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy addip Data parameter */
        addip.Data = (uint8 *)calloc(addip.dataLength, sizeof(uint8));
        if (addip.Data != NULL_PTR)
        {
            memcpy(addip.Data, &tpMessage->Payload[2], addip.dataLength);
            Result = TP_DeleteStaticEntry(NULL_PTR, addip);
            free(addip.Data);
        }
        else
        {
            Result = RID_E_NOK;
            TP_TRACE_ERROR("Failed to allocate memory for PID_ARP_ADD_STATIC_ENTRY");
        }
#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_ARP_SET_TIMEOUT:
        Result = E_OK;
        break;
    case PID_ARP_CLEAR_TIMEOUT:
        Result = E_OK;
        break;
    default:
        tpMessage->Tid = TID_RESPONSE;
        /* Result ID */
        tpMessage->Rid = RID_E_NTF;
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
        break;
    }

    return Result;
}

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
TP_ResultID_t TP_DecodeReceivedGroupEth(TestabilityProtocolMessage_t *tpMessage)
{
    TP_ResultID_t Result = E_NOT_OK;
    TP_TRACE_DEBUG("start");
    switch (tpMessage->Pid)
    {
    case PID_ETH_INTERFACE_UP:
    {

#ifdef FEATURE_TESTER_SIDE
        if (Eth_cb_func.EthInterfaceUp != NULL_PTR)
        {
            Eth_cb_func.EthInterfaceUp(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_ETH_INTERFACE_UP");
        }
        Result = RID_E_OK;
#else /* FEATURE_DUT_SIDE */
        text ifName = {ZERO_VALUE};
        /* Copy ifName.dataLength */
        memcpy(&ifName.dataLength, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy ifName.Data */
        ifName.Data = (uint8 *)calloc(ifName.dataLength, sizeof(uint8));
        if (ifName.Data != NULL_PTR)
        {
            memcpy(ifName.Data, &tpMessage->Payload[2], ifName.dataLength);
            Result = TP_EthInterfaceUp(NULL_PTR, ifName);
            free(ifName.Data);
        }
        else
        {
            Result = RID_E_NOK;
            TP_TRACE_ERROR("Failed to allocate memory for PID_ETH_INTERFACE_UP");
        }

#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    case PID_ETH_INTERFACE_DOWN:
    {

#ifdef FEATURE_TESTER_SIDE
        if (Eth_cb_func.EthInterfaceDown != NULL_PTR)
        {
            Eth_cb_func.EthInterfaceDown(tpMessage->Rid);
        }
        else
        {
            TP_TRACE_WARNING("User did not define a callback function for PID_ETH_INTERFACE_DOWN");
        }
        Result = RID_E_OK;
#else /* FEATURE_DUT_SIDE */
        text ifName = {ZERO_VALUE};
        /* Copy ifName.dataLength */
        memcpy(&ifName.dataLength, &tpMessage->Payload[0], sizeof(uint16));
        /* Copy ifName.Data */
        ifName.Data = (uint8 *)calloc(ifName.dataLength, sizeof(uint8));
        if (ifName.Data != NULL_PTR)
        {
            memcpy(ifName.Data, &tpMessage->Payload[2], ifName.dataLength);
            Result = TP_EthInterfaceDown(NULL_PTR, ifName);
            free(ifName.Data);
        }
        else
        {
            Result = RID_E_NOK;
            TP_TRACE_ERROR("Failed to allocate memory for PID_ETH_INTERFACE_DOWN");
        }

#endif /* FEATURE_TESTER_SIDE */
    }
    break;
    default:
        tpMessage->Tid = TID_RESPONSE;
        /* Result ID */
        tpMessage->Rid = RID_E_NTF;
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
        break;
    }
    return Result;
}

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
TP_ResultID_t TP_DecodeReceivedGroupPhy(TestabilityProtocolMessage_t *tpMessage)
{
    TP_ResultID_t Result = E_NOT_OK;
    TP_TRACE_DEBUG("start");
    switch (tpMessage->Pid)
    {
    case PID_PHY_READ_SIGNAL_QUALITY:
        Result = E_NOT_OK;
        break;
    case PID_PHY_READ_DIAG_RESULT:
        Result = E_NOT_OK;
        break;
    case PID_PHY_ACTIVATE_TEST_MODE:
        Result = E_NOT_OK;
        break;
    case PID_PHY_SET_PHY_TX_MODE:
        Result = E_NOT_OK;
        break;
    default:
        tpMessage->Tid = TID_RESPONSE;
        /* Result ID */
        tpMessage->Rid = RID_E_NTF;
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
        break;
    }
    return Result;
}

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
TP_ResultID_t TP_DecodeReceivedGroupCC(TestabilityProtocolMessage_t *tpMessage)
{
    TP_ResultID_t Result = E_NOT_OK;
    TP_TRACE_DEBUG("start");
    switch (tpMessage->Pid)
    {
    case PID_CC_CLOSE_CHANNEL:
    {
        TP_cleanDependencies();
    }
    break;

    default:
        tpMessage->Tid = TID_RESPONSE;
        /* Result ID */
        tpMessage->Rid = RID_E_NTF;
        /* TP message payload is empty*/
        Result = TP_SendFrame(tpMessage);
        break;
    }
    return Result;
}

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
void *TP_Udp_ReceiveAndForwardSocket_thread(void *inputParam)
{
#ifndef FEATURE_TESTER_SIDE
    int socketId = *((int *)inputParam);
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE, socketId = %d", socketId);
    if (socketId != ERROR_RETURN_CODE)
    {
        TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
        if (tpMessage != NULL_PTR)
        {
            /* Construct the TP message */
            TP_TRACE_DEBUG("Construct the TP message");
            tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
            tpMessage->EVB = 1;
            tpMessage->Gid = GID_UDP;
            tpMessage->Pid = PID_UDP_RECEIVE_AND_FORWARD;
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
            tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
            tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
            /* change the TP type ID to response */
            tpMessage->Tid = TID_EVENT;

            int bytesReceived = ZERO_VALUE;
            struct sockaddr_in client_addr;
            boolean firstEntry = FALSE;
            /* get the node containing the socketId */
            TP_socket_t *tempSocketNode = TP_FindSocket(socketId);

            uint32 tempBufferSize = 64;
            uint32 client_struct_length = sizeof(client_addr);
            uint16 fullLen = ZERO_VALUE;
            uint16 srcPort = ZERO_VALUE;
            ip4addr srcAddr = {ZERO_VALUE};
            uint16 dataToTransmit = ZERO_VALUE;
            while ((CancelThreadStatus == FALSE) && (tempSocketNode != NULL_PTR))
            {
                if (tempSocketNode->RecvFwdActive == TRUE)
                {
                    if (firstEntry == TRUE)
                    {
                        tempSocketNode->recvBuffer = (uint8 *)realloc(tempSocketNode->recvBuffer, tempSocketNode->maxFwd);
                        tempBufferSize = tempSocketNode->maxFwd;
                        firstEntry = FALSE;
                    }
                    else
                    {
                        /* MISRA C */
                    }
                }
                else
                {
                    if (firstEntry == FALSE)
                    {
                        tempSocketNode->recvBuffer = (uint8 *)realloc(NULL, tempBufferSize);
                        firstEntry = TRUE;
                    }
                    else
                    {
                        /* MISRA C */
                    }
                }
                bytesReceived = ZERO_VALUE;
                bytesReceived = recvfrom(socketId, tempSocketNode->recvBuffer, tempBufferSize, MSG_TRUNC, (struct sockaddr *)&client_addr, &client_struct_length);

                if (bytesReceived >= (sint32)ZERO_VALUE)
                {

                    if (tempSocketNode->RecvFwdActive == TRUE)
                    {
                        TP_TRACE_DEBUG("bytesReceived = %d socketId %d", bytesReceived, socketId);
                        fullLen = bytesReceived;
                        srcPort = ntohs(client_addr.sin_port);
                        srcAddr.dataLength = 4;
                        memcpy(srcAddr.Data, &client_addr.sin_addr.s_addr, IPV4_ADDRESS_SIZE);

                        if (tempSocketNode->maxLen < LIMITLESS_FORWARD_DATA)
                        {
                            if (tempSocketNode->maxLen >= bytesReceived)
                            {
                                dataToTransmit = bytesReceived;
                                tempSocketNode->maxLen -= bytesReceived;
                            }
                            else
                            {
                                dataToTransmit = tempSocketNode->maxLen;
                                if (tempSocketNode->maxLen >= tempSocketNode->maxFwd)
                                {
                                    tempSocketNode->maxLen -= tempSocketNode->maxFwd;
                                }
                                else
                                {
                                    tempBufferSize = tempSocketNode->maxLen;
                                    tempSocketNode->maxLen = ZERO_VALUE;
                                }
                            }
                        }
                        else
                        {
                            if (tempSocketNode->maxLen >= bytesReceived)
                            {
                                dataToTransmit = bytesReceived;
                            }
                            else
                            {
                                dataToTransmit = tempSocketNode->maxLen;
                            }
                        }

                        vint8 payload = {dataToTransmit, ZERO_VALUE};
                        payload.Data = (uint8 *)calloc(dataToTransmit, sizeof(uint8));
                        memcpy(payload.Data, tempSocketNode->recvBuffer, dataToTransmit);

                        /* Construct the payload */
                        tpMessage->Payload = (uint8 *)calloc(sizeof(fullLen) + sizeof(srcPort) + sizeof(srcAddr.dataLength) + srcAddr.dataLength + sizeof(payload.dataLength) + payload.dataLength, sizeof(uint8));

                        if (tpMessage->Payload != NULL_PTR)
                        {
                            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + sizeof(fullLen) + sizeof(srcPort) + sizeof(srcAddr.dataLength) + srcAddr.dataLength + sizeof(payload.dataLength) + payload.dataLength; /* 8 + payload size */

                            /* Result ID */
                            tpMessage->Rid = RID_E_OK;
                            /* Copy fullLen parametr */
                            memcpy(&tpMessage->Payload[0], &fullLen, sizeof(uint16));
                            /* Copy srcPort parametr */
                            memcpy(&tpMessage->Payload[2], &srcPort, sizeof(uint16));
                            /* Copy srcAddr.dataLength parametr */
                            memcpy(&tpMessage->Payload[4], &srcAddr.dataLength, sizeof(uint16));
                            /* Copy srcAddr.Data parametr */
                            memcpy(&tpMessage->Payload[6], srcAddr.Data, srcAddr.dataLength);
                            /* Copy payload.dataLength parametr */
                            memcpy(&tpMessage->Payload[6 + srcAddr.dataLength], &payload.dataLength, sizeof(uint16));
                            /* Copy payload.Data parametr */
                            memcpy(&tpMessage->Payload[8 + srcAddr.dataLength], payload.Data, payload.dataLength);
                            /* send_message_with_payload() */
                            TP_SendFrame(tpMessage);
                            free(tpMessage->Payload);
                        }
                        else
                        {
                            /* Can't allocate memory */
                            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size */
                            /* Result ID */
                            tpMessage->Rid = RID_E_ISB;

                            /* send_message_without_payload() */
                            TP_SendFrame(tpMessage);
                        }
                        if (tempSocketNode->maxLen == ZERO_VALUE)
                        {

                            break;
                        }
                    }
                    else
                    {

                        tempSocketNode->dropCnt += bytesReceived;
                        TP_TRACE_DEBUG("Receive and Forward disabled for this socketId %d, dropCnt %d", socketId, tempSocketNode->dropCnt);
                    }
                }
                else
                {
                    //TP_TRACE_WARNING("Couldn't receive data");
                }
            }
        }
        else
        {
            /* Can't allocate memory */
        }

        free(tpMessage);
    }
#endif /* FEATURE_TESTER_SIDE */
    (void)inputParam;
    return NULL_PTR;
}

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
void *TP_Tcp_ReceiveAndForwardSocket_thread(void *inputParam)
{

#ifndef FEATURE_TESTER_SIDE
    int socketId = *((int *)inputParam);
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE, socketId = %d", socketId);
    if (socketId != ERROR_RETURN_CODE)
    {
        TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
        if (tpMessage != NULL_PTR)
        {
            /* Construct the TP message */
            TP_TRACE_DEBUG("Construct the TP message");
            tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
            tpMessage->EVB = 1;
            tpMessage->Gid = GID_TCP;
            tpMessage->Pid = PID_TCP_RECEIVE_AND_FORWARD;
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
            tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
            tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
            /* change the TP type ID to response */
            tpMessage->Tid = TID_EVENT;

            int bytesReceived = ZERO_VALUE;
            boolean firstEntry = FALSE;
            /* get the node containing the socketId */
            TP_socket_t *tempSocketNode = TP_FindSocket(socketId);

            uint32 tempBufferSize = 64;
            uint16 fullLen = ZERO_VALUE;
            uint16 dataToTransmit = ZERO_VALUE;
            while ((CancelThreadStatus == FALSE) && (tempSocketNode != NULL_PTR))
            {
                if (tempSocketNode->RecvFwdActive == TRUE)
                {
                    if (firstEntry == TRUE)
                    {
                        tempSocketNode->recvBuffer = (uint8 *)realloc(tempSocketNode->recvBuffer, tempSocketNode->maxFwd);
                        tempBufferSize = tempSocketNode->maxFwd;
                        firstEntry = FALSE;
                    }
                    else
                    {
                        /* MISRA C */
                    }
                }
                else
                {
                    if (firstEntry == FALSE)
                    {
                        tempSocketNode->recvBuffer = (uint8 *)realloc(NULL, tempBufferSize);
                        firstEntry = TRUE;
                    }
                    else
                    {
                        /* MISRA C */
                    }
                }
                bytesReceived = ZERO_VALUE;
                bytesReceived = recv(socketId, tempSocketNode->recvBuffer, tempBufferSize, ZERO_VALUE);
                if (bytesReceived > (sint32)ZERO_VALUE)
                {

                    if (tempSocketNode->RecvFwdActive == TRUE)
                    {
                        fullLen = bytesReceived;

                        if (tempSocketNode->maxLen < LIMITLESS_FORWARD_DATA)
                        {
                            if (tempSocketNode->maxLen >= bytesReceived)
                            {
                                dataToTransmit = bytesReceived;
                                tempSocketNode->maxLen -= bytesReceived;
                            }
                            else
                            {
                                dataToTransmit = tempSocketNode->maxLen;
                                if (tempSocketNode->maxLen >= tempSocketNode->maxFwd)
                                {
                                    tempSocketNode->maxLen -= tempSocketNode->maxFwd;
                                }
                                else
                                {
                                    tempBufferSize = tempSocketNode->maxLen;
                                    tempSocketNode->maxLen = ZERO_VALUE;
                                }
                            }
                        }
                        else
                        {
                            if (tempSocketNode->maxLen >= bytesReceived)
                            {
                                dataToTransmit = bytesReceived;
                            }
                            else
                            {
                                dataToTransmit = tempSocketNode->maxLen;
                            }
                        }
                        vint8 payload = {dataToTransmit, ZERO_VALUE};
                        payload.Data = (uint8 *)calloc(dataToTransmit, sizeof(uint8));
                        memcpy(payload.Data, &tempSocketNode->recvBuffer[0], dataToTransmit);

                        /* Construct the payload */
                        tpMessage->Payload = (uint8 *)calloc(sizeof(fullLen) + sizeof(payload.dataLength) + payload.dataLength, sizeof(uint8));

                        if (tpMessage->Payload != NULL_PTR)
                        {
                            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + sizeof(fullLen) + sizeof(payload.dataLength) + payload.dataLength; /* 8 + payload size */

                            /* Result ID */
                            tpMessage->Rid = RID_E_OK;
                            /* Copy fullLen parametr */
                            memcpy(&tpMessage->Payload[0], &fullLen, sizeof(uint16));
                            /* Copy payload.dataLength parametr */
                            memcpy(&tpMessage->Payload[2], &payload.dataLength, sizeof(uint16));
                            /* Copy payload.Data parametr */
                            memcpy(&tpMessage->Payload[4], payload.Data, payload.dataLength);
                            /* send_message_with_payload() */
                            TP_SendFrame(tpMessage);
                            free(tpMessage->Payload);
                        }
                        else
                        {
                            /* Can't allocate memory */
                            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size */
                            /* Result ID */
                            tpMessage->Rid = RID_E_ISB;

                            /* send_message_without_payload() */
                            TP_SendFrame(tpMessage);
                        }
                        if (tempSocketNode->maxLen == ZERO_VALUE)
                        {
                            break;
                        }
                    }
                    else
                    {
                        tempSocketNode->dropCnt += bytesReceived;
                        //TP_TRACE_DEBUG("Receive and Forward disabled for this socketId %d, dropCnt %d", socketId, tempSocketNode->dropCnt);
                    }
                }
                else
                {
                    //TP_TRACE_WARNING("Couldn't receive data");
                }
            }
        }
        else
        {
            /* Can't allocate memory */
        }
        free(tpMessage);
    }
#endif /* FEATURE_TESTER_SIDE */
    (void)inputParam;
    return NULL_PTR;
}

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
void *TP_TcpListenAndAccept_event_thread(void *inputParam)
{
#ifndef FEATURE_TESTER_SIDE
    TP_TRACE_DEBUG("start FEATURE_DUT_SIDE");
    Tcp_ListenAndAcceptParam_t *argus = inputParam;
    TP_socket_t *tempSocketNode_test = TP_FindSocket(argus->socketId);
    if ((tempSocketNode_test != NULL_PTR) && (argus->socketId != ERROR_RETURN_CODE))
    {
        TestabilityProtocolMessage_t *tpMessage = calloc(sizeof(TestabilityProtocolMessage_t), sizeof(uint8));
        if (tpMessage != NULL_PTR)
        {
            /* Construct the TP message */
            TP_TRACE_DEBUG("Construct the TP message");
            tpMessage->Sid = TESTABILITY_PROTOCOL_SERVICE_ID;
            tpMessage->EVB = 1;
            tpMessage->Gid = GID_TCP;
            tpMessage->Pid = PID_TCP_LISTEN_AND_ACCEPT;
            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT;
            tpMessage->ProtocolVersion = TESTABILITY_PROTOCOL_VERSION;
            tpMessage->InterfaceVersion = TESTABILITY_PROTOCOL_INTERFACE;
            /* change the TP type ID to event */
            tpMessage->Tid = TID_EVENT;

            uint8 connectedSocket = ZERO_VALUE;
            while ((connectedSocket < argus->maxCon) && (CancelThreadStatus == FALSE))
            {
                struct sockaddr_in remote_sin;
                socklen_t remote_sinlen = sizeof(remote_sin);
                /* wait for a connection */
                int nsd = accept(argus->socketId, (struct sockaddr *)&remote_sin, &remote_sinlen);
                if (nsd != ERROR_RETURN_CODE)
                {
                    /* save the created socketId */
                    TP_AddSocket(nsd, FALSE, ZERO_VALUE, ZERO_VALUE, ZERO_VALUE);

                    TP_socket_t *tempSocketNode = TP_FindSocket(nsd);
                    if (tempSocketNode != NULL_PTR)
                    {
                        threadSocket.numOfThread++;
                        threadSocket.pthread_id = (pthread_t *)realloc(threadSocket.pthread_id, threadSocket.numOfThread * sizeof(pthread_t) * sizeof(uint8));
                        int *socketIdthread = malloc(sizeof(nsd));
                        if ((socketIdthread != NULL_PTR) && (threadSocket.pthread_id != NULL_PTR))
                        {
                            *socketIdthread = nsd;
                            if (pthread_create(&threadSocket.pthread_id[threadSocket.numOfThread -1], NULL, TP_Tcp_ReceiveAndForwardSocket_thread, socketIdthread) == ZERO_VALUE)
                            {
                                /* Thread created successfully */
                                TP_TRACE_DEBUG("thread TP_Tcp_ReceiveAndForwardSocket_thread created successfully, socketId = %d", nsd);
                                /* Store the threadId related to the created socket */
                                tempSocketNode->pthread_id = threadSocket.pthread_id[threadSocket.numOfThread -1];
                            }
                            else
                            {
                                TP_TRACE_ERROR("cannot create thread TP_Tcp_ReceiveAndForwardSocket_thread, socketId = %d", nsd);
                            }
                            /* get the remote address of the connected socket */
                            getpeername(nsd, (struct sockaddr *)&remote_sin, &remote_sinlen);

                            /* Construct the payload */
                            uint16 listenSocketId = argus->socketId;
                            uint16 newSocketId = nsd;
                            uint16 port = ntohs(remote_sin.sin_port);
                            ip4addr address = {4, {ZERO_VALUE}};
                            memcpy(address.Data, &remote_sin.sin_addr.s_addr, IPV4_ADDRESS_SIZE);

                            tpMessage->Payload = calloc((sizeof(uint16) + sizeof(uint16) + sizeof(uint16) + sizeof(uint16) + address.dataLength), sizeof(uint8));

                            if (tpMessage->Payload != NULL_PTR)
                            {
                                tpMessage->Length = TP_PAYLOAD_SIZE_BRUT + (sizeof(uint16) + sizeof(uint16) + sizeof(uint16) + sizeof(uint16) + address.dataLength); /* 8 + payload size */

                                /* Copy listenSocketId parameter */
                                memcpy(&tpMessage->Payload[0], &listenSocketId, sizeof(uint16));
                                /* Copy newSocketId parameter */
                                memcpy(&tpMessage->Payload[2], &newSocketId, sizeof(uint16));
                                /* Copy port parameter */
                                memcpy(&tpMessage->Payload[4], &port, sizeof(uint16));
                                /* Copy localAddr dataLength parameter */
                                memcpy(&tpMessage->Payload[6], &address.dataLength, sizeof(uint16));
                                /* Copy localAddr Data parameter */
                                memcpy(&tpMessage->Payload[8], address.Data, address.dataLength);
                                /* send_message_with_payload() */
                                TP_SendFrame(tpMessage);
                                free(tpMessage->Payload);
                            }
                            else
                            {
                                /* Can't allocate memory */
                                tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size */
                                /* Result ID */
                                tpMessage->Rid = RID_E_ISB;

                                /* send_message_without_payload() */
                                TP_SendFrame(tpMessage);
                            }
                        }
                        else
                        {
                            TP_TRACE_ERROR("Cannot create a thread to handle the received data on the newly created socket");
                            tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size */
                            /* Result ID */
                            tpMessage->Rid = RID_E_NOK;
                            /* send_message_without_payload() */
                            TP_SendFrame(tpMessage);
                        }
                    }
                    else
                    {
                        TP_TRACE_ERROR("Cannot find the socketId in the global linked list, socketId = %d", nsd);
                        tpMessage->Length = TP_PAYLOAD_SIZE_BRUT; /* 8 + payload size */
                        /* Result ID */
                        tpMessage->Rid = RID_E_NOK;
                        /* send_message_without_payload() */
                        TP_SendFrame(tpMessage);
                    }

                    /* close the connection */
                    connectedSocket++;
                }
                else
                {
                    //TP_TRACE_ERROR("error while accepting a new connection, error = %d, continue listning ..", errno);
                }
                usleep(100000);
            }
        }
        else
        {
            /* Can't allocate memory */
        }
    }
#endif /* FEATURE_DUT_SIDE */
    (void)inputParam;
    return NULL_PTR;
}

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
TP_ResultID_t TP_DecodeReturnCode(int errorCode)
{

    TP_ResultID_t Result = E_NOT_OK;
    switch (errorCode)
    {
    case ZERO_VALUE:
    case ESRCH:
        Result = RID_E_OK;
        break;
    case EAGAIN:
        Result = RID_E_ISB;
        break;
    case EBADFD:
    case EBADF:
    case ENOTSOCK:
        Result = RID_E_ISD;
        break;
    case ENFILE:
    case ENOBUFS:
    case ENOMEM:
    case EMFILE:
        Result = RID_E_UCS;
        break;
    case EADDRINUSE:
    case EOPNOTSUPP:
        Result = RID_E_UBS;
        break;
    case EALREADY:
    case EINPROGRESS:
        Result = RID_E_OAP;
        break;
    case ENODEV:
    case ENXIO:
        Result = RID_E_IIF;
        break;
    case EINVAL:
    case EAFNOSUPPORT:
    case EPROTONOSUPPORT:
        Result = RID_E_INV;
        break;
    case ENOTCONN:
        Result = RID_E_TCP_CNE;
        break;
    case EACCES:
        Result = RID_E_TCP_ILP;
        break;
    default:
        Result = RID_E_NOK;
        break;
    }
    return Result;
}

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
void TP_PrintArray(uint8 *buffer, uint32 bufferLength)
{
    if (buffer != NULL_PTR)
    {
        uint32 counter = 0;
        for (counter = 0; counter < bufferLength; counter++)
        {
            printf("%d ", buffer[counter]);
        }
    }
    else
    {
        TP_TRACE_ERROR("Received Null ptr");
    }
}

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
void TP_AddSocket(uint16 socketId, boolean RecvFwdActive, uint16 dropCnt, uint16 maxFwd, uint16 maxLen)
{
    /* Insert a new socketId at first location */

    TP_socket_t *newSocketId = (struct TP_socket_t *)calloc(sizeof(TP_socket_t), sizeof(uint8));
    if (newSocketId != NULL_PTR)
    {
        newSocketId->socketId = socketId;
        newSocketId->RecvFwdActive = RecvFwdActive;
        newSocketId->maxFwd = maxFwd;
        newSocketId->maxLen = maxLen;
        newSocketId->dropCnt = dropCnt;
        /* point it to old first node */
        newSocketId->next = headSocketId_t;

        /* point first to new first node */
        headSocketId_t = newSocketId;
    }
    else
    {
        TP_TRACE_ERROR("Cannot create a new node");
    }
}

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
void TP_DeleteSocket(uint16 socketId)
{

    /* start from the first TP_socket_t */
    TP_socket_t *current = headSocketId_t;
    TP_socket_t *previous = NULL_PTR;

    /* check if list is empty */
    if (headSocketId_t == NULL_PTR)
    {
        TP_TRACE_WARNING("Cannot delete a new node: list is empty");
        return;
    }
    else
    {
        /* navigate through list */
        while (current->socketId != socketId)
        {

            /* if it is last node */
            if (current->next == NULL_PTR)
            {
                TP_TRACE_WARNING("Cannot delete a new node: socketId not found in list");
                return;
            }
            else
            {
                /* store reference to current TP_socket_t */
                previous = current;
                /* move to next TP_socket_t */
                current = current->next;
            }
        }
        /* free any allocated buffer */
        free(current->recvBuffer);
        /* found a match, update the TP_socket_t */
        if (current == headSocketId_t)
        {
            /* change first to point to next TP_socket_t */
            headSocketId_t = headSocketId_t->next;
        }
        else
        {
            /* bypass the current TP_socket_t */
            previous->next = current->next;
        }
    }
}

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
struct TP_socket_t *TP_FindSocket(uint16 socketId)
{
    /* start from the first node */
    struct TP_socket_t *current = headSocketId_t;

    /* if list is empty */
    if (headSocketId_t == NULL)
    {
        TP_TRACE_WARNING("Cannot find the element: list is empty");
        return NULL;
    }

    /* navigate through list */
    while (current->socketId != socketId)
    {

        /* if it is last node */
        if (current->next == NULL)
        {
            TP_TRACE_WARNING("Cannot find the element: socketId not found in list");
            return NULL;
        }
        else
        {
            /* go to next link */
            current = current->next;
        }
    }

    /* if data found, return the current Link */
    return current;
}

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
int rtattr_add(struct nlmsghdr *n, int maxlen, int type, const void *data, int alen)
{
    (void)maxlen;
    int len = RTA_LENGTH(alen);
    struct rtattr *rta;

    rta = NLMSG_TAIL(n);
    rta->rta_type = type;
    rta->rta_len = len;

    if (alen)
    {
        memcpy(RTA_DATA(rta), data, alen);
    }

    n->nlmsg_len = NLMSG_ALIGN(n->nlmsg_len) + RTA_ALIGN(len);

    return 0;
}
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
int sendIcmpEchoRequest(text ifName, ip4addr destAddr, vint8 sdata)
{
    struct icmphdr icmp_hdr = {0};
    struct sockaddr_in addr = {0};
    int rc = 0;
    uint8 *data;
    int sequence = 0;
    int sock = 0;
    int retval = 0;

    do
    {
        /* create ICMP socket */
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
        if (sock < 0)
        {
            perror("socket");
            retval = ERROR_RETURN_CODE;
            TP_TRACE_ERROR("ICMP SOCKET CREATION ERROR");
            break;
        }
        else
        {

            /* MISRA */
        }
        if (ifName.Data != NULL_PTR)
        {
            struct ifreq interface;
            memset(&interface, 0, sizeof(interface));
            memcpy(interface.ifr_name, ifName.Data, ifName.dataLength);
            if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE,
                           (char *)&interface, sizeof(interface)) < 0)
            {
                perror("sendpacket: setting SO_BINDTODEVICE");
                retval = ERROR_RETURN_CODE;
                TP_TRACE_INFO("ICMP ETH INTERFACE ERROR");
                break;
            }
            else
            {
                /* MISRA */
            }
        }
        else
        {
            /* MSIRA */
        }

        /* Set ICMP Header */
        memset(&icmp_hdr, 0, sizeof(icmp_hdr));
        icmp_hdr.type = ICMP_ECHO;
        icmp_hdr.un.echo.id = 1234; //arbitrary id
        icmp_hdr.un.echo.sequence = sequence;

        /* Fill  Payload : ICMP Header + data */
        data = calloc(sizeof(icmp_hdr) + sdata.dataLength, sizeof(uint8));
        if (data == NULL_PTR)
        {
            retval = ERROR_RETURN_CODE;
            TP_TRACE_ERROR("ICMP CAN NOT ALLOCATE MEMORY FOR DATA");
            break;
        }
        else
        {
            /*MISRA*/
        }
        memcpy(data, &icmp_hdr, sizeof(icmp_hdr));
        memcpy(data + sizeof(icmp_hdr), sdata.Data, sdata.dataLength);

        /* Set Adrr Struct */
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = destAddr.Data[0] | (destAddr.Data[1] << 8) | (destAddr.Data[2] << 16) | (destAddr.Data[3] << 24);
        /* Send ICMP Echo Request */
        rc = sendto(sock, data, sizeof sizeof(icmp_hdr) + sdata.dataLength, 0, (struct sockaddr *)&addr, sizeof(addr));
        if (rc <= 0)
        {
            TP_TRACE_INFO("SEND ICMP ECHO REQUEST ERROR");
            perror("Sendto");
            free(data);
            retval = ERROR_RETURN_CODE;
            break;
        }
        else
        {
            /* MISRA*/
        }
        free(data);
    } while (0);

    return retval;
}

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
void TP_cleanDependencies(void)
{
    /* Cancel all threads */
    CancelThreadStatus = TRUE;
    pthread_cancel(thread_id);
    /* Remove all registred CBs */
    General_cb_func = (General_cb_t){NULL_PTR};
    Udp_cb_func = (Udp_cb_t){NULL_PTR};
    Tcp_cb_func = (Tcp_cb_t){NULL_PTR};
    IPv4_cb_func = (Ipv4_cb_t){NULL_PTR};
    Arp_cb_func = (Arp_cb_t){NULL_PTR};
    Eth_cb_func = (Eth_cb_t){NULL_PTR};
    Icmp_cb_func = (Icmp_cb_t){NULL_PTR};
#ifdef FEATURE_TESTER_SIDE

    /* close the opened sockets */
    if (close(sockfd) == ERROR_RETURN_CODE)
    {
        TP_TRACE_ERROR("can not close Control Channel main socket, error = %d", errno);
    }
    else
    {
        /* MISRA C */
    }
#else  /*FEATURE_DUT_SIDE */

    /* Close all opened threads */
    while (threadSocket.numOfThread > 0)
    {
        pthread_cancel(threadSocket.pthread_id[threadSocket.numOfThread - 1]);
        threadSocket.numOfThread--;
    }
    /* Close all opened sockets */
    while (headSocketId_t != NULL_PTR)
    {
        close(headSocketId_t->socketId);
        pthread_cancel(headSocketId_t->pthread_id);
        TP_DeleteSocket(headSocketId_t->socketId);
    }

    if (close(sockfd1) == ERROR_RETURN_CODE)
    {
        TP_DecodeReturnCode(errno);
    }
    else
    {
        if (close(connfd) == ERROR_RETURN_CODE)
        {
            TP_DecodeReturnCode(errno);
        }
        else
        {
            TP_DecodeReturnCode(errno);
        }
    }
#endif /* FEATURE_DUT_SIDE */
}
/** \}    end of addtogroup */
