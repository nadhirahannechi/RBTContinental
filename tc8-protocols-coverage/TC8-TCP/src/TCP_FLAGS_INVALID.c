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
#include "TCP_FLAGS_INVALID.h"
#include "TestabilityProtocol_api.h"
#include "TestabilityProtocol_Intern.h"

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_01
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_01  test.
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

int TCP_FLAGS_INVALID_01()
{
    //open controle connection
    //TP_OpenControlChannel();
    StartTest();

    // Move DUT to listen state
    int socketID,socketTester;
    uint32 seqN, ackN;
    int moveDUTToListenResult = moveDUTToListen(&socketID,&socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon);

    if (moveDUTToListenResult == Result_OK)

    {
        //misra
    }
    else
    {
       printf("\nTest failed --> cannot move to Listen\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // Receive DUT response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        
        printf("\nTest failed --> DUT responded while it shoudn't\n");
        return 1;
    }
    else 
    {
        seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        ackN= (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
    }
    
    // Verify that DUT is in LISTEN state
    if (checkState(LISTEN))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in LISTEN state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_02
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_02  test.
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
int TCP_FLAGS_INVALID_02()
{
    //open controle connection
    //TP_OpenControlChannel();
    StartTest();

    // Move DUT to established state
    int socketID,socketTester;
    uint32 seqN, ackN;
    int moveDUTToListenResult = moveDUTToListen(&socketID,&socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon);

    if (moveDUTToListenResult == Result_OK)

    {
        //misra
    }
    else
    {
       printf("\nTest failed --> cannot move to Listen\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    // Send SYN,ACK segment
    TCP_Packet TCP_P = CreateTCP();
    int ACKNumber = 55;
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)ACKNumber);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    // Receive DUT response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        int rSEQNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if ((rSEQNumber == ACKNumber) && (rRST == 1))
        {
            //TEST OK
        }
        else
        {
            printf("\nTest failed --> DUT response not RST or with unexpected SEQ number\n");
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 1;
        }
    }
    else
    {
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    // Verify that DUT is in LISTEN state
    if (checkState(LISTEN))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in LISTEN state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_03
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_03  test.
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
int TCP_FLAGS_INVALID_03()
{
    //open controle connection
    //TP_OpenControlChannel();
    StartTest();

    // Move DUT to established state
	int socketID;
    uint32 seqN, ackN;

    int MoveToSYNSENTResult = moveDUTToSYNSENT(&socketID, TCPConfig.DUT_Port, TCPConfig.TESTER_IP, 5, &seqN, &ackN);
    if (MoveToSYNSENTResult == 0)
    {
        //result ok
    }
    else
    {
        // Unexpected response
        printf("\nTest failed --> Cannot move to SYNSYNT state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
    // Send SYN,RST segment
    TCP_Packet TCP_P = CreateTCP();
    int ACKNumber = 55;
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)ACKNumber);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    // Receive DUT response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {
        seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        ackN= (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
    }
    else
    {
        printf("\nTest failed --> DUT responded to a RST segment\n");
        return 1;
    }
    //4. TESTER: Verify that the DUT remains in SYN-SENT state
    if (checkState(SYNSENT))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in SYN-SENT state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_04
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_04  test.
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
int TCP_FLAGS_INVALID_04()
{
    //open controle connection
    //TP_OpenControlChannel();
    StartTest();

    // Move DUT to established state
    int socketID;
    uint32 seqN, ackN;

    int MoveToSYNSENTResult = moveDUTToSYNSENT(&socketID, TCPConfig.DUT_Port, TCPConfig.TESTER_IP, 5, &seqN, &ackN);
    if (MoveToSYNSENTResult == 0)
    {
        //result ok
    }
    else
    {
        // Unexpected response
        printf("\nTest failed --> Cannot move to SYNSYNT state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    // Send a RST segment
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // Receive DUT response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    // Verify that DUT ignored the RST segment
    if (TCP_RP.length == 0)
    {
        seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        ackN= (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
    }
    else
    {
        printf("\nTest failed --> DUT responded to a RST segment\n");
        return 1;
    }
    // Verify that the DUT remains in SYN-SENT state
    if (checkState(SYNSENT))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in SYN-SENT state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_05_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_05_IT1  test.
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
int TCP_FLAGS_INVALID_05_IT1()
{
    //open controle connection
    //TP_OpenControlChannel();
    StartTest();

    // Move DUT to established state
    int socketID;
    uint32 seqN, ackN;

    int MoveToSYNSENTResult = moveDUTToSYNSENT(&socketID, TCPConfig.DUT_Port, TCPConfig.TESTER_IP, 5, &seqN, &ackN);
    if (MoveToSYNSENTResult == 0)
    {
        //result ok
    }
    else
    {
        // Unexpected response
        printf("\nTest failed --> Cannot move to SYNSYNT state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
	
    // Send segment with SYN,ACK and RST flags set
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    seqN = (int)GetTCPField(&TCP_P, TCP, TCP_SeqNumber);
    ackN= (int)GetTCPField(&TCP_P, TCP, TCP_AckNumber);
    
    // Verify that the DUT move to CLOSED state
    if (checkState(CLOSED))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in CLOSED state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_05_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_05_IT2  test.
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
int TCP_FLAGS_INVALID_05_IT2()
{
    //open controle connection
    //TP_OpenControlChannel();
    StartTest();

    // Move DUT to established state
    int socketID;
    uint32 seqN, ackN;

    int MoveToSYNSENTResult = moveDUTToSYNSENT(&socketID, TCPConfig.DUT_Port, TCPConfig.TESTER_IP, 5, &seqN, &ackN);
    if (MoveToSYNSENTResult == 0)
    {
        //result ok
    }
    else
    {
        // Unexpected response
        printf("\nTest failed --> Cannot move to SYNSYNT state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    // Send segment with ACK and RST flags set
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // Verify that the DUT move to CLOSED state
    if (checkState(CLOSED))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in CLOSED state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_06_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_06_IT1  test.
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
int TCP_FLAGS_INVALID_06_IT1()
{
    //open controle connection
    //TP_OpenControlChannel();
    StartTest();

    // Move DUT to established state
    int socketID;
    uint32 seqN, ackN;

    int MoveToSYNSENTResult = moveDUTToSYNSENT(&socketID, TCPConfig.DUT_Port, TCPConfig.TESTER_IP, 5, &seqN, &ackN);
    if (MoveToSYNSENTResult == 0)
    {
        //result ok
    }
    else
    {
        // Unexpected response
        printf("\nTest failed --> Cannot move to SYNSYNT state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
    // Send segment with PSH and ACK flags set
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // Receive DUT response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    // Verify that DUT does not respond
    if (TCP_RP.length != 0)
    {
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rSYN == 1) && (rACK == 1))
        {
            printf("\nTest failed --> DUT responded with a SYN,ACK\n");
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN= (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 1;
        }
        else 
        {
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN= (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
    }
    else
    {
        //Test OK
    }

    // Verify that the DUT remain in SYN-SENT state
    if (checkState(SYNSENT))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in SYN-SENT state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_06_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_06_IT2  test.
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
int TCP_FLAGS_INVALID_06_IT2()
{
    //open controle connection
    //TP_OpenControlChannel();
    StartTest();

    // Move DUT to established state
    int socketID;
    uint32 seqN, ackN;

    int MoveToSYNSENTResult = moveDUTToSYNSENT(&socketID, TCPConfig.DUT_Port, TCPConfig.TESTER_IP, 5, &seqN, &ackN);
    if (MoveToSYNSENTResult == 0)
    {
        //result ok
    }
    else
    {
        // Unexpected response
        printf("\nTest failed --> Cannot move to SYNSYNT state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    // Send ACK segment
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // Receive DUT response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    // Verify that DUT does not respond
    if (TCP_RP.length != 0)
    {
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rSYN == 1) && (rACK == 1))
        {
            printf("\nTest failed --> DUT responded with a SYN,ACK\n");
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 1;
        }
        else
        {
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN= (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
    }
    else
    {
        
    }

    // Verify that the DUT remain in SYN-SENT state
    if (checkState(SYNSENT))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in SYN-SENT state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_07_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_07 iteration 1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = SYN
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_07_IT1()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move on to SYN-RCVD state
    int socketID,socketTester;
    uint32 seqN, ackN;
    int moveDUTToSYNRCVDResult = moveDUTToSYNRCVD(&socketID,&socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);

    if (moveDUTToSYNRCVDResult == Result_OK)

    {
        //misra
    }
    else
    {
       printf("\nTest failed --> cannot move to moveDUTToSYNRCVD\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(0xffff));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        if ((rAckNum == ackN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        End_Test_Mode_2(socketID, seqN, ackN);
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in SYN-RCVD state
    if (checkState(SYNRCVD))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in SYN-RCVD state\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_07_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_07 iteration 2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = SYN,ACK
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_07_IT2()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move on to SYN-RCVD state
    int socketID,socketTester;
    uint32 seqN, ackN;
    int moveDUTToSYNRCVDResult = moveDUTToSYNRCVD(&socketID,&socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);

    if (moveDUTToSYNRCVDResult == Result_OK)

    {
        //misra
    }
    else
    {
       printf("\nTest failed --> cannot move to moveDUTToSYNRCVD\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(0xffff));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        if ((rAckNum == ackN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in SYN-RCVD state
    if (checkState(SYNRCVD))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in SYN-RCVD state\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_07_IT3
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_07 iteration 3 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = ACK
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_07_IT3()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move on to SYN-RCVD state
	int socketID,socketTester;
    uint32 seqN, ackN;
    int moveDUTToSYNRCVDResult = moveDUTToSYNRCVD(&socketID,&socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);

    if (moveDUTToSYNRCVDResult == Result_OK)

    {
        //misra
    }
    else
    {
       printf("\nTest failed --> cannot move to moveDUTToSYNRCVD\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    int sSeq = 0x7530;

    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(0xffff));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);

        if ((rAckNum == ackN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            printf("\nTest failed --> response not ok\n");
            End_Test_Mode_2(socketID, seqN, ackN);
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in SYN-RCVD state
    if (checkState(SYNRCVD))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in SYN-RCVD state\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_07_IT4
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_07 iteration 4 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set FIN
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_07_IT4()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move on to SYN-RCVD state
	int socketID,socketTester;
    uint32 seqN, ackN;
    int moveDUTToSYNRCVDResult = moveDUTToSYNRCVD(&socketID,&socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);

    if (moveDUTToSYNRCVDResult == Result_OK)

    {
        //misra
    }
    else
    {
       printf("\nTest failed --> cannot move to moveDUTToSYNRCVD\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    int sSeq = 0x7530;

    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(0xffff));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        if ((rAckNum == ackN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in SYN-RCVD state
    if (checkState(SYNRCVD))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in SYN-RCVD state\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_07_IT5
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_07 iteration 5 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set Data segment 
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_07_IT5()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move on to SYN-RCVD state
	int socketID,socketTester;
    uint32 seqN, ackN;
    int moveDUTToSYNRCVDResult = moveDUTToSYNRCVD(&socketID,&socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);

    if (moveDUTToSYNRCVDResult == Result_OK)

    {
        //misra
    }
    else
    {
       printf("\nTest failed --> cannot move to moveDUTToSYNRCVD\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    int sSeq = 0x7530;

    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(0xffff));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        if ((rAckNum == ackN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in SYN-RCVD state
    if (checkState(SYNRCVD))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in SYN-RCVD state\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_08_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_08 iteration 1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = SYN
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_08_IT1()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move on to ESTABLISHED state
	uint32 seqN, ackN,socketID;
    int moveClientDUTToEstablishedResult = moveClientDUTToEstablished(&socketID, TCPConfig.DUT_Port ,TCPConfig.Tester_Port,  TCPConfig.Maxcon, &seqN, &ackN);
     if (moveClientDUTToEstablishedResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT2 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
	
    int sSeq = 0x7530;

    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        uint32 rAckNum = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == 1) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in ESTABLISHED state
    if (checkState(ESTABLISHED))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in ESTABLISHED state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_08_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_08 iteration 2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = SYN,ACK
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_08_IT2()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move on to ESTABLISHED state
    int sSeq = 0x7530;
	uint32 seqN, ackN,socketID;
    int moveClientDUTToEstablishedResult = moveClientDUTToEstablished(&socketID, TCPConfig.DUT_Port ,TCPConfig.Tester_Port,  TCPConfig.Maxcon, &seqN, &ackN);
     if (moveClientDUTToEstablishedResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT2 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == 1) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in ESTABLISHED state
    if (checkState(ESTABLISHED))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in ESTABLISHED state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_08_IT3
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_08 iteration 3 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = ACK
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_08_IT3()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move on to ESTABLISHED state
    int sSeq = 0x7530;
	uint32 seqN, ackN,socketID;
    int moveClientDUTToEstablishedResult = moveClientDUTToEstablished(&socketID, TCPConfig.DUT_Port ,TCPConfig.Tester_Port,  TCPConfig.Maxcon, &seqN, &ackN);
     if (moveClientDUTToEstablishedResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT2 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == 1) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in ESTABLISHED state
    if (checkState(ESTABLISHED))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in ESTABLISHED state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_08_IT4
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_08 iteration 4 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set FIN
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_08_IT4()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move on to ESTABLISHED state
    int sSeq = 0x7530;
	uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    int moveServDUTToEstablishedResult =moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveServDUTToEstablishedResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        if ((rAckNum == 1) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in ESTABLISHED state
    if (checkState(ESTABLISHED))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in ESTABLISHED state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_08_IT5
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_08 iteration 5 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set Data segment 
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_08_IT5()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move on to ESTABLISHED state
    int sSeq = 0x7530;
	uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    int moveServDUTToEstablishedResult =moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveServDUTToEstablishedResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        if ((rAckNum == seqN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in ESTABLISHED state
    if (checkState(ESTABLISHED))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in ESTABLISHED state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_09_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_09 iteration 1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = SYN
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_09_IT1()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move on to FINWAIT1 state
    int sSeq = 0x7530;
	int socketID;
    uint32 seqN, ackN;
    int moveDUTToFINWAIT1Result = moveDUTToFINWAIT1(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToFINWAIT1Result == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT1 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
	
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));

    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        uint32 rAckNum = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == 1) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in FINWAIT1 state
    if (checkState(FINWAIT1))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in FINWAIT1 state\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_09_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_09 iteration 2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = SYN,ACK
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_09_IT2()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move on to FINWAIT1 state
    int sSeq = 0x7530;
	int socketID;
    uint32 seqN, ackN;
    int moveDUTToFINWAIT1Result = moveDUTToFINWAIT1(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToFINWAIT1Result == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT1 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
	
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == 1) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in FINWAIT1 state
    if (checkState(FINWAIT1))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in FINWAIT1 state\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_09_IT3
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_09 iteration 3 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = ACK
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_09_IT3()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move on to FINWAIT1 state
    int sSeq = 0x7530;
	int socketID;
    uint32 seqN, ackN;
    int moveDUTToFINWAIT1Result = moveDUTToFINWAIT1(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToFINWAIT1Result == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT1 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
	
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == 1) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in FINWAIT1 state
    if (checkState(FINWAIT1))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in FINWAIT1 state\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_09_IT4
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_09 iteration 4 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set FIN
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_09_IT4()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move on to FINWAIT1 state
    int sSeq = 0x7530;
	int socketID;
    uint32 seqN, ackN;
    int moveDUTToFINWAIT1Result = moveDUTToFINWAIT1(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToFINWAIT1Result == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT1 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
	
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    //EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        if ((rAckNum == 1) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in FINWAIT1 state
    if (checkState(FINWAIT1))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in FINWAIT1 state\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_09_IT5
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_09 iteration 5 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set Data segment 
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_09_IT5()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move on to FINWAIT1 state
    int sSeq = 0x7530;
	int socketID;
    uint32 seqN, ackN;
    int moveDUTToFINWAIT1Result = moveDUTToFINWAIT1(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToFINWAIT1Result == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT1 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
	
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        if ((rAckNum == 1) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in FINWAIT1 state
    if (checkState(FINWAIT1))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in FINWAIT1 state\n");
        End_Test_Mode_2(socketID, seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_10_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_10 iteration 1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = SYN
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_10_IT1()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on to FINWAIT-2 state
    int sSeq = 0x7530;
	int socketID;
    uint32 seqN, ackN;
    int moveDUTToFINWAIT2Result = moveDUTToFINWAIT2(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToFINWAIT2Result == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT2 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
	
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        uint32 rAckNum = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == seqN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remainsin FINWAIT-2 state
     

    if (checkState(FINWAIT2))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_10_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_10 iteration 2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = SYN,ACK
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_10_IT2()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on to FINWAIT-2 state
    int sSeq = 0x7530;
	int socketID;
    uint32 seqN, ackN;
    int moveDUTToFINWAIT2Result = moveDUTToFINWAIT2(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToFINWAIT2Result == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT2 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
	
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == seqN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            //End_Test_Mode_2(socketID,seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        //End_Test_Mode_2(socketID,seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remainsin FINWAIT-2 state
     

    if (checkState(FINWAIT2))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_10_IT3
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_10 iteration 3 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = ACK
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_10_IT3()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on to FINWAIT-2 state
    int sSeq = 0x7530;
	int socketID;
    uint32 seqN, ackN;
    int moveDUTToFINWAIT2Result = moveDUTToFINWAIT2(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToFINWAIT2Result == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT2 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
	
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == seqN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remainsin FINWAIT-2 state
     

    if (checkState(FINWAIT2))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_10_IT4
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_10 iteration 4 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set FIN
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_10_IT4()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on to FINWAIT-2 state
    int sSeq = 0x7530;
	int socketID;
    uint32 seqN, ackN;
    int moveDUTToFINWAIT2Result = moveDUTToFINWAIT2(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToFINWAIT2Result == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT2 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
	
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        if ((rAckNum == seqN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remainsin FINWAIT-2 state
     

    if (checkState(FINWAIT2))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_10_IT5
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_10 iteration 5 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set Data segment 
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_10_IT5()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on to FINWAIT-2 state
    int sSeq = 0x7530;
	int socketID;
    uint32 seqN, ackN;
    int moveDUTToFINWAIT2Result = moveDUTToFINWAIT2(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToFINWAIT2Result == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT2 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
	
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        if ((rAckNum == seqN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remainsin FINWAIT-2 state
     

    if (checkState(FINWAIT2))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_11_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_11 iteration 1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = SYN
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_11_IT1()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on to CLOSE-WAIT state
    int sSeq = 0x7530;
	uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    int moveDUTToCloseWaitResult = moveDUTToCloseWait(&socketID, &socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToCloseWaitResult == Result_OK)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT state is not CloseWait \n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

	
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        uint32 rAckNum = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == 1) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in CLOSE-WAIT state
     

    if (checkState(CLOSEWAIT))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_11_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_11 iteration 2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = SYN,ACK
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_11_IT2()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on to CLOSE-WAIT state
    int sSeq = 0x7530;
    uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    int moveDUTToCloseWaitResult = moveDUTToCloseWait(&socketID, &socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToCloseWaitResult == Result_OK)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT state is not CloseWait \n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == 1) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in CLOSE-WAIT state
     

    if (checkState(CLOSEWAIT))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_11_IT3
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_11 iteration 3 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = ACK
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_11_IT3()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on to CLOSE-WAIT state
    int sSeq = 0x7530;
	uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    int moveDUTToCloseWaitResult = moveDUTToCloseWait(&socketID, &socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToCloseWaitResult == Result_OK)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT state is not CloseWait \n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
	
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == 1) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in CLOSE-WAIT state
     

    if (checkState(CLOSEWAIT))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_11_IT4
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_11 iteration 4 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set FIN
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_11_IT4()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on to CLOSE-WAIT state
    int sSeq = 0x7530;
	uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    int moveDUTToCloseWaitResult = moveDUTToCloseWait(&socketID, &socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToCloseWaitResult == Result_OK)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT state is not CloseWait \n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
	
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        if ((rAckNum == 1) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in CLOSE-WAIT state
     

    if (checkState(CLOSEWAIT))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_11_IT5
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_11 iteration 5 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set Data segment 
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_11_IT5()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on to CLOSE-WAIT state
    int sSeq = 0x7530;
	uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    int moveDUTToCloseWaitResult = moveDUTToCloseWait(&socketID, &socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToCloseWaitResult == Result_OK)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT state is not CloseWait \n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
	
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0);
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == seqN + 1) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in CLOSE-WAIT state
     

    if (checkState(CLOSEWAIT))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_12_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_12 iteration 1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = SYN
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_12_IT1()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on  to CLOSING state
    int sSeq = 0x7530;
	int socketID;
    uint32 seqN, ackN;
    int moveDUTToClosingResult= moveDUTToClosing(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToClosingResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to CLOSING state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
    
	
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if ((rAckNum == 2) && (rACK == 1) && (rRST == 0))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in CLOSING state
     

    if (checkState(CLOSING))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_12_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_12 iteration 2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = SYN,ACK
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_12_IT2()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on  to CLOSING state
    int sSeq = 0x7530;
	int socketID;
    uint32 seqN, ackN;
    int moveDUTToClosingResult= moveDUTToClosing(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToClosingResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to CLOSING state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
    
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if ((rAckNum == 2) && (rACK == 1) && (rRST == 0))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in CLOSING state
     

    if (checkState(CLOSING))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_12_IT3
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_12 iteration 3 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = ACK
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_12_IT3()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on  to CLOSING state
    int sSeq = 0x7530;
	int socketID;
    uint32 seqN, ackN;
    int moveDUTToClosingResult= moveDUTToClosing(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToClosingResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to CLOSING state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
    
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if ((rAckNum == 2) && (rACK == 1) && (rRST == 0))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in CLOSING state
     

    if (checkState(CLOSING))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_12_IT4
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_12 iteration 4 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set FIN
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_12_IT4()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on  to CLOSING state
    int sSeq = 0x7530;
	int socketID;
    uint32 seqN, ackN;
    int moveDUTToClosingResult= moveDUTToClosing(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToClosingResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to CLOSING state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
    
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if ((rAckNum == 2) && (rACK == 1) && (rRST == 0))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in CLOSING state
     

    if (checkState(CLOSING))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_12_IT5
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_12 iteration 5 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set Data segment 
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_12_IT5()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on  to CLOSING state
    int sSeq = 0x7530;
	int socketID;
    uint32 seqN, ackN;
    int moveDUTToClosingResult= moveDUTToClosing(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToClosingResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to CLOSING state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
    
    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if ((rAckNum == 2) && (rACK == 1) && (rRST == 0))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in CLOSING state
     

    if (checkState(CLOSING))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_13_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_13 iteration 1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = SYN
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_13_IT1()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on to LAST-ACK state
    int sSeq = 0x7530;
    uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    int moveDUTToLASTACKResult = moveDUTToLASTACK(&socketID,&socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToLASTACKResult == Result_OK)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT response  is not Last ACK\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
  

    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        uint32 rAckNum = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == ackN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remainsin LAST-ACK state
     

    if (checkState(LASTACK))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_13_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_13 iteration 2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = SYN,ACK
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_13_IT2()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on to LAST-ACK state
    int sSeq = 0x7530;
    uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    int moveDUTToLASTACKResult = moveDUTToLASTACK(&socketID,&socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToLASTACKResult == Result_OK)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT response  is not Last ACK\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
  

    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == ackN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remainsin LAST-ACK state
     

    if (checkState(LASTACK))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_13_IT3
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_13 iteration 3 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = ACK
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_13_IT3()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on to LAST-ACK state
    int sSeq = 0x7530;
    uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    int moveDUTToLASTACKResult = moveDUTToLASTACK(&socketID,&socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToLASTACKResult == Result_OK)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT response  is not Last ACK\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
  

    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == ackN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remainsin LAST-ACK state
     

    if (checkState(LASTACK))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_13_IT4
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_13 iteration 4 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set FIN
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_13_IT4()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on to LAST-ACK state
    int sSeq = 0x7530;
    uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    int moveDUTToLASTACKResult = moveDUTToLASTACK(&socketID,&socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToLASTACKResult == Result_OK)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT response  is not Last ACK\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
  

    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        if ((rAckNum == ackN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remainsin LAST-ACK state
     

    if (checkState(LASTACK))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_13_IT5
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_13 iteration 5 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set Data segment 
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_13_IT5()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause DUT to move  on to LAST-ACK state
    int sSeq = 0x7530;
    uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    int moveDUTToLASTACKResult = moveDUTToLASTACK(&socketID,&socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToLASTACKResult == Result_OK)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT response  is not Last ACK\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
  

    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        if ((rAckNum == ackN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID, seqN, ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remainsin LAST-ACK state
     

    if (checkState(LASTACK))
    {
        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID, seqN, ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_14_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_14 iteration 1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = FIN
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_14_IT1()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause the DUT to move on to TIME-WAIT state
    int socketID;
    uint32 seqN, ackN;
    int moveToTimeWaitres=moveDUTToTimeWait(&socketID, TCPConfig.DUT_Port , TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveToTimeWaitres == Result_OK)
    {
        printf("\n move to Timewaait with success \n");
    }
    else 
    {
        printf("\n Test failed :cannot move to time wait \n");
        return Result_NOK;
    }

    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)0);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN + 30000));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == ackN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify DUT remains in TIME-WAIT state for other cases
    if (checkState(TIMEWAIT))
    {
        printf("\nTest passed\n");
        EndTest(socketID);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in TIMEWAIT state\n");
        EndTest(socketID);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_14_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_14 iteration 2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: flag set = DATA Segment
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_14_IT2()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause the DUT to move on to TIME-WAIT state
    int socketID;
    uint32 seqN, ackN;
    int moveToTimeWaitres=moveDUTToTimeWait(&socketID, TCPConfig.DUT_Port , TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveToTimeWaitres == Result_OK)
    {
        printf("\n move to Timewaait with success \n");
    }
    else 
    {
        printf("\n Test failed :cannot move to time wait \n");
        return Result_NOK;
    }

    //2. TESTER: Send a segment with a flag set, RST=0 and with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)0);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN + 30000));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with ACK number indicating the correct expected next SEQ number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        int rAckNum = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rAckNum == ackN) && (rACK == 1))
        {
            // result ok
        }
        else
        {
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;
        }
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify DUT remains in TIME-WAIT state for other cases
    if (checkState(TIMEWAIT))
    {
        printf("\nTest passed\n");
        EndTest(socketID);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in TIMEWAIT state\n");
        EndTest(socketID);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_15_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_15 iteration 1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: <wst> = SYN-RECEIVED
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_15_IT1()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause the DUT to move on to <wst> state
	int socketID,socketTester;
    uint32 seqN, ackN;
    int moveDUTToSYNRCVDResult = moveDUTToSYNRCVD(&socketID,&socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);

    if (moveDUTToSYNRCVDResult == Result_OK)

    {
        //misra
    }
    else
    {
       printf("\nTest failed --> cannot move to moveDUTToSYNRCVD\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    int sSeq = 30000;

    //2. TESTER: Send a RST segment with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)sSeq);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Do not send any response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {
        //result ok
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        
        printf("\nTest failed --> DUT send responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in <wst> state
    if (checkState(SYNRCVD))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in SYN-RCVD state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_15_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_15 iteration 2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: <wst> = ESTABLISHED
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_15_IT2()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause the DUT to move on to <wst> state
    int sSeq = 30000;
	uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    int moveServDUTToEstablishedResult =moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveServDUTToEstablishedResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }
	
    //2. TESTER: Send a RST segment with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)sSeq);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Do not send any response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {
        //result ok
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        
        printf("\nTest failed --> DUT send responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in <wst> state
    if (checkState(ESTABLISHED))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in ESTABLISHED state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_15_IT3
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_15 iteration 3 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: <wst> = FINWAIT-1
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_15_IT3()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause the DUT to move on to <wst> state
    int sSeq = 30000;
	int socketID;
    uint32 seqN, ackN;
    int moveDUTToFINWAIT1Result = moveDUTToFINWAIT1(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToFINWAIT1Result == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT1 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
	
    //2. TESTER: Send a RST segment with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)sSeq);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Do not send any response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {
        //result ok
    }
    else
    {
        int rSeqNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        if (rSeqNumber != seqN)
        {
            printf("\nTest failed --> DUT responded while it shouldn't\n");
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 1;
        }
    }

    //4. TESTER: Verify that the DUT remains in <wst> state
    if (checkState(FINWAIT1))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in FINWAIT1 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_15_IT4
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_15 iteration 4 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: <wst> = FINWAIT-2
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_15_IT4()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause the DUT to move on to <wst> state
    int sSeq = 30000;
	int socketID;
    uint32 seqN, ackN;
    int moveDUTToFINWAIT2Result = moveDUTToFINWAIT2(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToFINWAIT2Result == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to FINWAIT2 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
	
    //2. TESTER: Send a RST segment with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)sSeq);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Do not send any response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {
        //result ok
    }
    else
    {
        printf("\nTest failed --> DUT responded while it shouldn't\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in <wst> state
    if (checkState(FINWAIT2))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in FINWAIT2 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_15_IT5
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_15 iteration 5 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: <wst> = CLOSE-WAIT
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_15_IT5()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause the DUT to move on to <wst> state
    uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    int moveDUTToCloseWaitResult = moveDUTToCloseWait(&socketID, &socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToCloseWaitResult == Result_OK)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT state is not CloseWait \n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
	
    //2. TESTER: Send a RST segment with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0);
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Do not send any response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {
        //result ok
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        
        printf("\nTest failed --> DUT send responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in <wst> state
     

    if (checkState(CLOSEWAIT))
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_15_IT6
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_15 iteration 6 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: <wst> = CLOSING
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_15_IT6()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause the DUT to move on to <wst> state
    int socketID;
    uint32 seqN, ackN;
    int moveDUTToClosingResult= moveDUTToClosing(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToClosingResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to CLOSING state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
    
    //2. TESTER: Send a RST segment with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0);
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Do not send any response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {
        //result ok
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        
        printf("\nTest failed --> DUT send responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in <wst> state
     

    if (checkState(CLOSING))
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_15_IT7
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_15 iteration 7 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: <wst> = LAST-ACK
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_15_IT7()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause the DUT to move on to <wst> state
    uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    int moveDUTToLASTACKResult = moveDUTToLASTACK(&socketID,&socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToLASTACKResult == Result_OK)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT response  is not Last ACK\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
  

    //2. TESTER: Send a RST segment with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0);
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Do not send any response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {
        //result ok
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        
        printf("\nTest failed --> DUT send responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in <wst> state
     

    if (checkState(LASTACK))
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_INVALID_15_IT8
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_INVALID_15 iteration 8 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: <wst> = TIME-WAIT 
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_INVALID_15_IT8()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause the DUT to move on to <wst> state
    int socketID;
    uint32 seqN, ackN;
   int moveToTimeWaitres=moveDUTToTimeWait(&socketID, TCPConfig.DUT_Port , TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveToTimeWaitres == Result_OK)
    {
        printf("\n move to Timewaait with success \n");
    }
    else 
    {
        printf("\n Test failed :cannot move to time wait \n");
        return Result_NOK;
    }
    //2. TESTER: Send a RST segment with an unacceptable SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0);
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Do not send any response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {
        //result ok
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> DUT send responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in <wst> state

    if (checkState(TIMEWAIT))
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {

        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> state not ok\n");
        return 1;
    }
}
