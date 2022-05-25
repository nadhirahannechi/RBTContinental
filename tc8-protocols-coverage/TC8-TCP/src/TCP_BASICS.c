/**
 *  Copyright (C) [year] Focus Corporation
 *
 *  \file     TCP_CALL_ABORT.c
 *  \brief    This file contains the tests of TCP CALL ABORT 
 *  \author   Aymen SALEM && Omar NSIRI
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
#include "TCP_BASICS.h"
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
*******************
/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_01
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP BASICS 1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_01()
{
    uint32_t socketId, socketTester;
    uint32 seqN, ackN;
    // Start Test
    StartTest();

    // 1. TESTER: Cause DUT to move on to LISTEN state at a <wnp>
    int moveDUTToListenResult = moveDUTToListen(&socketId, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon);
    if (moveDUTToListenResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }
    
    // 2. TESTER: Send a SYN to DUT at <wnp>
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 3. DUT: Send SYN,ACK
    // Receive DUT response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        // Verify that DUT response is SYN,ACK
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rSYN == 1) && (rACK == 1))
        {
            seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            printf("\nTest passed\n");
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 0;
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not SYN,ACK\n");
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 1;
        }
    }
    else
    {
        // No response sent
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_02
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP BASICS 2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_02()
{
    uint32_t socketId, socketTester;
    uint32 seqN, ackN;
    // Start Test
    StartTest();

    // Move DUT to Listen state
    int moveDUTToListenResult = moveDUTToListen(&socketId, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon);
    if (moveDUTToListenResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }
    
    // 1. TESTER: Send a SYN to DUT
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 2. DUT: Send a SYN,ACK(this will take DUT to the state SYN-RCVD)
    // Receive DUT response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    int rAckNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
    int rSeqNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
    if (TCP_RP.length != 0)
    {
        
        // Verify that DUT response is SYN,ACK
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rSYN == 1) && (rACK == 1))
        {
            ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not SYN,ACK\n");
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 1;
        }
    }
    else
    {
        // No response sent
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }

    // 3. TESTER: Send an ACK
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)0);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)rAckNumber);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(rSeqNumber + 1));
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    sleep(3);

    ackN = (int)GetTCPField(&TCP_P, TCP, TCP_AckNumber);
    seqN = (int)GetTCPField(&TCP_P, TCP, TCP_SeqNumber);
    // 4. TESTER: Verify that the DUT moves on to ESTABLISHED state
    if (checkState(ESTABLISHED))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in ESTABLISHED state\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_03
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP BASICS 3 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_03()
{

    uint32_t socketId, socketTester;
    uint32 seqN, ackN;
    // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to ESTABLISHED state at a <wnp>
    
    int moveServDUTToEstablishedResult =moveServDUTToEstablished(&socketId, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveServDUTToEstablishedResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }
    
    // 2. TESTER: Send a FIN ,ACK
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)seqN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 3. DUT: Send ACK
    // Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            printf("test passed");
            ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 0;
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not ACK\n");
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 1;
        }
    }
    else
    {
        // No response sent
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_04_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP BASICS 4 iteration 1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_04_IT1()
{
    
    // 1. TESTER: Send a TCP segment with a flag set
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 2. DUT: Send a RST control message with zero SEQ number
    // Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        int rSEQ = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        if ((rRST == 1) && (rSEQ == 0))
        {
            printf("\nTest passed\n");
            return 0;
        }
        else
        {
            printf("\nTest failed --> RST field is not set or sequence number != 0\n");
            //EndTest(-1);
            return 1;
        }
    }
    else
    {
        printf("\nTest failed --> DUT not responding\n");
        //EndTest(-1);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_04_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP BASICS 4 iteration 2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_04_IT2()
{

    // 1. TESTER: Send a TCP segment with a flag set
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 2. DUT: Send a RST control message with zero SEQ number
    // Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        int rSEQ = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        if ((rRST == 1) && (rSEQ == 0))
        {
            printf("\nTest passed\n");
            return 0;
        }
        else
        {
            printf("\nTest failed --> RST field is not set or sequence number # 0\n");
            return 1;
        }
    }
    else
    {
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_04_IT3
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP BASICS 4 iteration 3 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_04_IT3()
{

    uint32 seqN, ackN;
    // 1. TESTER: Send a TCP segment with a flag set
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 2. DUT: Send a RST control message with zero SEQ number
    // Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        int rSEQ = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        if ((rRST == 1) && (rSEQ == 0))
        {
            
            printf("\nTest passed\n");
            return 0;
        }
        else
        {
            printf("\nTest failed --> RST field is not set or sequence number # 0\n");
            return 1;
        }
    }
    else
    {
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_05_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP BASICS 5 iteration 1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_05_IT1()
{

    // 1. TESTER: Send a segment with a flag set
    TCP_Packet TCP_P = CreateTCP();
    int ackNum = 222;
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)ackNum);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 2. DUT: Send a RST segment with SEQ number same as the ACK  number of the incoming segment
    // Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        int seqNum = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        if ((rRST == 1) && (seqNum == ackNum))
        {
            printf("\nTest passed\n");
            return 0;
        }
        else
        {
            printf("\nTest failed --> RST field is not set or #SEQ != #ACK\n");
            return 1;
        }
    }
    else
    {
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_05_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP BASICS 5 iteration 2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_05_IT2()
{

    uint32 seqN, ackN;
    // 1. TESTER: Send a segment with a flag set
    TCP_Packet TCP_P = CreateTCP();
    int ackNum = 222;
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)ackNum);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 2. DUT: Send a RST segment with SEQ number same as the ACK  number of the incoming segment
    // Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        int seqNum = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        if ((rRST == 1) && (seqNum == ackNum))
        {
            printf("\nTest passed\n");
            return 0;
        }
        else
        {
            printf("\nTest failed --> RST field is not set or #SEQ != #ACK\n");
            return 1;
        }
    }
    else
    {
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_06
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP BASICS 6 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_06()
{

    int socketId;
    uint32 seqN = 0, ackN = 0;
    
    // Start Test
    StartTest();

    // 1. TESTER: Cause the application on the DUT-side to issue an active OPEN call
    // 2. DUT: Send a SYN
    // Receive DUT responses
    
    int moveDUTToSYNSENTResult = moveDUTToSYNSENT(&socketId, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToSYNSENTResult == Result_OK)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT response is not ACK\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
    
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_07
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP BASICS 7 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_07()
{
    // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to SYN-SENT state
    int socketId;
    uint32 seqN = 0, ackN = 0;
    int moveDUTToSYNSENTResult = moveDUTToSYNSENT(&socketId, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToSYNSENTResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }
    
    // 2. TESTER: Send a SYN,ACK
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 3. DUT: Send ACK
    // Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        // Verify that DUT sends a ACK
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not ACK\n");
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 1;
        }
    }
    else
    {
        // No response sent
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }

    // 4. TESTER: Verify that the DUT moves to ESTABLISHED state
    if (checkState(ESTABLISHED))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in ESTABLISHED state\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_08_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs iteration 1 of TCP BASICS 8 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_08_IT1()
{
    uint32_t socketId, socketTester;
    uint32 seqN, ackN;
    // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to <wst> state
    // 2. TESTER: Cause the application on the DUT-side to issue a CLOSE call
    // 3. DUT: Send a FIN
    // Receive DUT responses

    int moveDUTToCloseWaitResult = moveDUTToCloseWait(&socketId, &socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToCloseWaitResult == Result_OK)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT state is not CloseWait \n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_08_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs iteration 2 of TCP BASICS 8 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_08_IT2()
{
    uint32_t socketId, socketTester;
    uint32 seqN, ackN;
    
    // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to <wst> state
    // 2. TESTER: Cause the application on the DUT-side to issue a CLOSE call
    // 3. DUT: Send a FIN
    // Receive DUT responses
    int moveDUTToLASTACKResult = moveDUTToLASTACK(&socketId,&socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToLASTACKResult == Result_OK)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT response  is not Last ACK\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
  
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_09
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP BASICS 9 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_09()
{
    uint32_t socketId, socketTester;
    uint32 seqN, ackN;
    
    // Start Test
    StartTest();
    // 1. TESTER: Cause the DUT to move on to LAST-ACK state
    // 2. DUT: Send FIN when moving into LAST-ACK state
    int moveDUTToLASTACKResult = moveDUTToLASTACK(&socketId,&socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToLASTACKResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> DUT response  is not Last ACK\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
    
    // 3. TESTER: Send ACK for the FIN just received from the DUT
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)seqN + 1);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 4. TESTER: Send a segment without the RST flag set
    TCP_Packet TCP_P_2 = CreateTCP();
    EditTCPField(&TCP_P_2, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P_2, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P_2, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P_2, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P_2, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P_2, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 6));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "DATA");

    TCP_Compute_checksum(&TCP_P_2);
    SendTCP(TCP_P_2);

    // 5. DUT: Send a segment with the RST flag set (this will verify the the DUT has moved on to the CLOSED state)
    // Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        // Verify that DUT sends a RST
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if (rRST == 1)
        {
            printf("\nTest passed\n");
            ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 0;
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not RST\n");
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 1;
        }
    }
    else
    {
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_10_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs iteration 1 of TCP BASICS 10 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_10_IT1()
{
    // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to FINWAIT-1 state
    int socketId;
    uint32 seqN, ackN;
    int moveDUTToFINWAIT1Result = moveDUTToFINWAIT1(&socketId, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToFINWAIT1Result == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT1 state\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return Result_NOK;
    }
    
    // 2. TESTER: Send a FIN
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 3. DUT: Send ACK
    // Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        // Verify that DUT sends an ACK
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            End_Test_Mode_2(socketId,  seqN,  ackN);
            printf("\nTest passed\n");
            return 0;
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not ACK\n");
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 1;
        }
    }
    else
    {
        // DUT not responding
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_10_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs iteration 2 of TCP BASICS 10 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_10_IT2()
{
    // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to FINWAIT-2 state
    int socketId;
    uint32 seqN, ackN;
    int moveDUTToFINWAIT2Result = moveDUTToFINWAIT2(&socketId, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToFINWAIT2Result == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT2 state\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return Result_NOK;
    }
    
    // 2. TESTER: Send a FIN
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 3. DUT: Send ACK
    // Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        // Verify that DUT sends an ACK
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            printf("\nTest passed\n");
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 0;
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not ACK\n");
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 1;
        }
    }
    else
    {
        // DUT not responding
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_11
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP BASICS 11 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_11()
{
    // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to FINWAIT-2 state
    int socketId;
    uint32 seqN, ackN;
    int moveDUTToFINWAIT2Result = moveDUTToFINWAIT2(&socketId, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToFINWAIT2Result == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT2 state\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return Result_NOK;
    }
    
    // 2. TESTER: Send a FIN
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 3. DUT: Send ACK
    // Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        // Verify that DUT sends an ACK
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        }
        else
        {
            printf("\nTest failed --> DUT response is not ACK\n");
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 1;
        }
    }
    else
    {
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }

    // 4. TESTER: Send a FIN after 2*MSL + 20%
    printf("\n sleep for 2.2*TCPConfig.msl = %d ", 2.2 * TCPConfig.msl);
    sleep(2.2 * TCPConfig.msl);

    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 5. DUT: Send a RST segment(this will indicate DUT is in CLOSED state
    //Receive DUT responses
    TCP_Packet TCP_RP_2 = ListenTCP(f);
    if (TCP_RP_2.length != 0)
    {
        // Verify that DUT sends an RST
        int rRST = (int)GetTCPField(&TCP_RP_2, TCP, TCP_FlagRST);
        if (rRST == 1)
        {
            printf("\nTest passed\n");
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 0;
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT did not move to closed state\n");
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 1;
        }
    }
    else
    {
        // DUT not responding
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_12
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP BASICS 12 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_12()
{
    // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to CLOSING state
    int socketId;
    uint32 seqN, ackN;
    int moveDUTToClosingResult= moveDUTToClosing(&socketId, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToClosingResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to CLOSING state\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return Result_NOK;
    }
    
    // 2. TESTER: Send ACK for the FIN just received from the DUT
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // DUT moves to TIME-WAIT

    // 3. TESTER: Send a FIN after 2*MSL + 20%
    printf("\n sleep for 2.2*TCPConfig.msl = %d \n", 2.2 * TCPConfig.msl);
    sleep(2.2 * TCPConfig.msl);
    // Send FIN segment
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 4. DUT: Send a RST segment(this will indicate DUT is in CLOSED state)
    //Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        // Verify that DUT sends an RST
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if (rRST == 1)
        {
            printf("\nTest passed\n");
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 0;
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT did not move to closed state\n");
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 1;
        }
    }
    else
    {
        // DUT not responding
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_13
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP BASICS 13 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_13()
{
    int socketId;
    uint32 seqN, ackN;
    // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to FINWAIT-2 state

    int moveDUTToFINWAIT2Result = moveDUTToFINWAIT2(&socketId, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToFINWAIT2Result == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT2 state\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return Result_NOK;
    }
    // 2. TESTER: Send a FIN
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 3. DUT: Send ACK (DUT moves to TIME-WAIT state)
    // Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        // Verify that DUT sends an ACK
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            //Test ok
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT did not move to closed state\n");
            End_Test_Mode_2(socketId, seqN, ackN);
            return 1;
        }
    }
    else
    {
        // DUT not responding
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketId, seqN, ackN);
        return 1;
    }

    // 4. TESTER: Send a FIN within 2*MSL time
    int percentage = 1.6;
    //sleep(percentage*TCPConfig.msl);

    // 5. DUT: Send FIN,ACK
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //Receive DUT responses
    TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        // Verify that DUT sends an ACK
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            printf("\nTest passed\n");
            End_Test_Mode_2(socketId, seqN, ackN);
            return 0;
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT did not move to closed state\n");
            End_Test_Mode_2(socketId, seqN, ackN);
            return 1;
        }
    }
    else
    {
        // DUT not responding
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketId, seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_14
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP BASICS 14 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_14()
{
    int socketId;
    uint32 seqN, ackN;
    // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to CLOSING state

    int moveDUTToClosingResult = moveDUTToClosing(&socketId, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToClosingResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to CLOSING state\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return Result_NOK;
    }
    // 2. TESTER: Send ACK for the FIN just received from the DUT (DUT moves to TIME-WAIT state)
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // DUT moves to TIME-WAIT
    if (checkState(TIMEWAIT))
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to TIMEWAIT state\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return Result_NOK;
    }
    
    // 3. TESTER: Send a FIN within 2*MSL time
    int percentage = 1.6;
    printf("\n sleep for %d\n",percentage * TCPConfig.msl);

    sleep(percentage * TCPConfig.msl);
    // Send FIN segment
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    
    
    // 4. DUT: Send ACK
    // Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        // Verify that DUT sends an ACK
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            printf("\nTest passed\n");
            End_Test_Mode_2(socketId, seqN, ackN);
            return 0;
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT did not move to closed state\n");
            End_Test_Mode_2(socketId, seqN, ackN);
            return 1;
        }
    }
    else
    {
        // DUT not responding
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketId, seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_BASICS_17
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP BASICS 17 test.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_BASICS_17()
{
    int socketId;
    uint32 seqN, ackN;

    // Start Test
    StartTest();

    // 1. TESTER: Cause the application on the DUT-side to issue an active OPEN call
    // 2. DUT: Send a SYN
    int MoveToSYNSENTResult = moveDUTToSYNSENT(&socketId, TCPConfig.DUT_Port, TCPConfig.TESTER_IP, 5, &seqN, &ackN);
    if (MoveToSYNSENTResult == 0)
    {
        //result ok
    }
    else
    {
        // Unexpected response
        printf("\nTest failed --> Cannot move to SYNSYNT state\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }

    // 3. TESTER: Send a SYN simultaneously
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN + 1));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 4. DUT: Send SYN,ACK
    // Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        // Verify that DUT sends an SYN,ACK
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        if ((rACK == 1) && (rSYN == 1))
        {
            seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
        else
        {

            // Unexpected response
            printf("\nTest failed --> Unexpected response\n");
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 1;
        }
    }
    else
    {

        // DUT not responding
        printf("\nTest failed --> DUT not responding in %d\n", __LINE__);
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }

    // 5. TESTER: Send SYN,ACK in reply to the SYN received from the DUT
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    seqN = (uint32)GetTCPField(&TCP_P, TCP, TCP_SeqNumber);
    ackN = (uint32)GetTCPField(&TCP_P, TCP, TCP_AckNumber);
    SendTCP(TCP_P);

    // 6. DUT: Send ACK
    // Receive DUT responses

    TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        // Verify that DUT sends an ACK
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> Unexpected response\n");
            End_Test_Mode_2(socketId,  seqN,  ackN);
            return 1;
        }
    }
    else
    {
        // DUT not responding
        printf("\nTest failed --> DUT not responding in %d\n", __LINE__);
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }

    // 7. TESTER: Verify that the DUT moves on to ESTABLISHED state
    if (checkState(ESTABLISHED))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in ESTABLISHED state\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
}
