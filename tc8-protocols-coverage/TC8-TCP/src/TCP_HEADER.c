/**
 *  Copyright (C) [year] Focus Corporation
 *
 *  \file     TCP_HEADER.c
 *  \brief    This file contains the tests of TCP HEADER
 *  \author   Xxxxx
 *  \platform [target platform]
 *
 *  \addtogroup SW_package
 *  \{
 */

/******************************************************************************
* INCLUDE FILES
******************************************************************************/
#include "AbstractionAPI.h"
#include "TCPcommon.h"
#include "TCP_HEADER.h"

#include "TestabilityProtocol_api.h"

/******************************************************************************
* DEFINES AND MACROS
******************************************************************************/

/******************************************************************************
* GLOBAL VARIABLES AND CONSTANTS
******************************************************************************/

/******************************************************************************
* STATIC FUNCTION PROTOTYPES
******************************************************************************/

/******************************************************************************
* Local FUNCTIONS
******************************************************************************/

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_HEADER_01
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_HEADER_01 test .
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_HEADER_01()
{
    int socketID,socketTester;

    // start test
    StartTest();

    //1. TESTER: Bring DUT to ‘ESTABLISHED’ state.
    uint32 seqN, ackN;
    int MoveToEstablished = moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (MoveToEstablished == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    //2. TESTER: Send <generateTCPSegment> request.
    vint8 data;
    data.Data = (uint8 *)calloc(8, sizeof(uint8));
    data.Data = (uint8 *)"TCP DATA";
    data.dataLength = sizeof("TCP DATA");
    TP_TcpSendData(NULL, socketTester, data.dataLength, 0, data);

    //3. DUT: Sends a data segment containing valID header values
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port + 1;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        int rPSHFlag = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagPSH);
        int rHeaderLength = (int)GetTCPField(&TCP_RP, TCP, TCP_HeaderLength);
        int VerifyCheksum = (int)TCP_Verify_Correct_checksum(TCP_RP);
        int rSeqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rAckN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        char *payloadData = (char *)GetTCPField(&TCP_RP, PAYLAOD, PAYLOAD_data);
        if ((rPSHFlag == 1) && (rHeaderLength == htons(5)) && (VerifyCheksum == 0) && (rSeqN = seqN) && (rAckN == ackN) && (strcmp(payloadData, (char *)data.Data) == 0))
        {
            printf("\nTest passed \n");
            End_Test_Mode_2(socketTester, seqN, ackN);
            return 0;
        }
        else
        {

            End_Test_Mode_2(socketTester, seqN, ackN);
            printf("\nTest failed --> DUT response not ok\n");
            return 1;
        }
    }
    else
    {
        printf("\nTest failed\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 0;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_HEADER_02
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_HEADER_02 test .
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_HEADER_02()
{
    int socketID,socketTester;

    // start test
    StartTest();

    //1. TESTER: Bring DUT to ‘ESTABLISHED’ state.
    uint32 seqN, ackN;
    int MoveToEstablished = moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (MoveToEstablished == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    //2. TESTER: Send a data packet
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Sends an ACK packet with the expected Ack Number.

    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        int rACKNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rFACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rACKNum == ackN) && (rFACK == 1))
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest passed \n");
            return 0;
        }
        else
        {

            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> DUT response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);

        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_HEADER_04
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_HEADER_04 test .
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_HEADER_04()
{

    int socketID,socketTester;

    // start test
    StartTest();

    //1. TESTER: Bring DUT to ‘ESTABLISHED’ state on <PORT1>.
    uint32 seqN, ackN;
    int MoveToEstablished = moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (MoveToEstablished == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    //2. TESTER: Send a TCP packet on a <PORT2> with Sequence Number set to <SEQ1>
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)(TCPConfig.Tester_Port + 1));
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)(TCPConfig.DUT_Port));
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: DUT discards the TCP packet and optionally DUT sends a RST packet
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port + 1;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        int rRSTFlag = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if (rRSTFlag == 1)
        {
            printf("\nTest passed \n");
            End_Test_Mode_2(socketID, seqN, ackN);
            return 0;
        }
        else
        {

            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> DUT response not ok\n");
            return 1;
        }
    }
    else
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 0;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_HEADER_05
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_HEADER_05 test .
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_HEADER_05()
{
    int socketID,socketTester;

    // start test
    StartTest();

    //1. TESTER: Bring DUT to ‘ESTABLISHED’ state.
    uint32 seqN, ackN;
    int MoveToEstablished = moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (MoveToEstablished == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    //2. TESTER: Send a data packet
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Sends an ACK packet with the expected Ack Number.

    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        int rACKNum = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        if (rACKNum == ackN)
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest passed \n");
            return 0;
        }
        else
        {

            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> DUT response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);

        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_HEADER_06
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_HEADER_06 test .
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_HEADER_06()
{
    int socketID,socketTester;

    // start test
    StartTest();

    //1. TESTER: Bring DUT to ‘ESTABLISHED’ state.
    uint32 seqN, ackN;
    int MoveToEstablished = moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (MoveToEstablished == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    //2. TESTER: Send a data packet
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_Reserved, (void *)(intptr_t)0b110);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Sends an ACK packet with the expected Ack Number.

    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        int rACKNum = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        if (rACKNum == ackN)
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest passed \n");
            return 0;
        }
        else
        {

            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> DUT response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);

        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_HEADER_07
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_HEADER_07 test .
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_HEADER_07()
{
    int socketID,socketTester;

    // start test
    StartTest();

    //1. TESTER: Bring DUT to ‘ESTABLISHED’ state.
    uint32 seqN, ackN;
    int MoveToEstablished = moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (MoveToEstablished == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    //2. TESTER: Send a TCP packet with Data Offset value less than 5
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_HeaderLength, (void *)(intptr_t)4);
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Discards the TCP packet.
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed \n");
        return 0;
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT send respond\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_HEADER_08
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_HEADER_08 test .
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_HEADER_08()
{
    int socketID,socketTester;

    // start test
    StartTest();

    //1. TESTER: Bring DUT to ‘ESTABLISHED’ state.
    uint32 seqN, ackN;
    int MoveToEstablished = moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (MoveToEstablished == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    //2. TESTER: Send a TCP packet with Data Offset value less than 5
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_HeaderLength, (void *)(intptr_t)8);
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Discards the TCP packet.
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed \n");
        return 0;
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT send respond\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_HEADER_09
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_HEADER_09 test .
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_HEADER_09()
{
    int socketID,socketTester;

    // start test
    StartTest();

    //1. TESTER: Bring DUT to ‘ESTABLISHED’ state.
    uint32 seqN, ackN;
    int MoveToEstablished = moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (MoveToEstablished == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    //2. TESTER: Send a TCP packet with a Checksum = 0 and Sequence Number = <SEQ>
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    SendTCP(TCP_P);
    EditTCPField(&TCP_P, IP, TCP_Checksum, (void *)(intptr_t)0);

    //3. DUT: Discards the TCP packet.
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed \n");
        return 0;
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT send respond\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_HEADER_11
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_HEADER_11 test .
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_HEADER_11()
{

    int socketID,socketTester;

    // start test
    StartTest();

    //1. TESTER: Bring DUT to ‘ESTABLISHED’ state.
    uint32 seqN, ackN;
    int MoveToEstablished = moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (MoveToEstablished == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    // 2. TESTER: Send a SYN with IP Destination Address = <multicastAddress>
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, IP, IP_DstAddress, (void *)(intptr_t) "239.10.10.10");
    EditTCPField(&TCP_P, ETHERNET, ETH_destinationMAC, (void *)(intptr_t) "01:00:5e:0a:0a:0a");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Discards the TCP packet.
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed \n");
        return 0;
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT send respond\n");
        return 1;
    }
}
