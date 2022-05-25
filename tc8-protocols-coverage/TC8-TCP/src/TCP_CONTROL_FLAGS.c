/**
 *  Copyright (C) [year] Focus Corporation
 *
 *  \file     TCP_CONTROL_FLAGS.c
 *  \brief    This file contains the tests of TCP CONTROL FLAGS
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
#include "TCP_CONTROL_FLAGS.h"
#include "TestabilityProtocol_api.h"

/******************************************************************************
* DEFINES AND MACROS
******************************************************************************/

/******************************************************************************
* GLOBAL VARIABLES AND CONSTANTS
******************************************************************************/

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_CONTROL_FLAGS_05
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_CONTROL_FLAGS_05 test.
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
int TCP_CONTROL_FLAGS_05()
{
    int socketID,socketTester;

    // start test
    //TP_GenaralStartTest(NULL);

    //1. TESTER: Bring DUT to ‘ESTABLISHED’ state. 
    uint32 seqN, ackN;
    StartTest();

    int MoveToEstablished = moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port , TCPConfig.Maxcon, &seqN, &ackN);
    if (MoveToEstablished == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    //2. TESTER: Send a TCP packet with URG being set to DUT.
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagURG, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_UrgPointer, (void *)(intptr_t)2);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Sends an ACK with expected Ack Number.
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) 
    {
        int rACKFlag, rAckNum;
        //get ack flag and ack number
        rACKFlag = (int) GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        rAckNum = (int) GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        // check result 
        if( ( rACKFlag == 1 ) && ( rAckNum == seqN + 8) )
        {
            printf("\nTest passed\n");
            //TP_GenaralEndTest(NULL,0,(text){0});
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 0;
        }
        else
        {
            printf("\nTest failed --> response is not ACK or with unacceptable ACK number\n");
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 1;
        }
    }
    else 
    {
        printf("\nTest failed --> DUT not responding\n");
        //TP_GenaralEndTest(NULL,0,(text){0});
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_CONTROL_FLAGS_08
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP CONTROL FLAGS 08 test.
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
int TCP_CONTROL_FLAGS_08()
{
    StartTest();

    //1. TESTER: Bring DUT to ‘LISTEN’ state.
    
    int socketID,socketTester;
    uint32 seqN1=5,seqN2=1, ackN=0 , rseqN;
    int moveDUTToListenResult = moveDUTToListen(&socketID,&socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon);

    if (moveDUTToListenResult == Result_OK)

    {
        //misra
    }
    else
    {
        printf("\nTest failed --> cannot move to Listen\n");
        End_Test_Mode_2(socketID,  seqN1,  ackN);
        return 1;
    }
    //2. TESTER: Send SYN with Sequence Number equal to <SEQ1>
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)seqN1);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    sleep(3);
    //3. DUT: Sends a SYN,ACK with Ack Number equal to <SEQ1>+1 and moves to ‘SYNRECEIVED’ state.
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        uint8 rSYN = (uint8)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        uint8 rACK = (uint8)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        rseqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        ackN= (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        if ((rSYN == 1) && (rACK == 1) && (ackN == seqN1 + 1 ))
        {
           //result ok
            
        }
        else
        {
            printf("\n DUT response does not SYN,ACK :  cannot move to  SYNRCVD \n");
            End_Test_Mode_2(socketID,  seqN1++,  ackN);
            return 1;
        }
    }
    else 
    {
        printf("\n DUT not responding : cannot move to  SYNRCVD \n");
        End_Test_Mode_2(socketID,  seqN1++,  ackN);
        return 1; 
    }

    // Verify that the DUT in SYN-RCVD state
    if (checkState(SYNRCVD))
    {
        printf("\n DUT state = SYNRCVD \n");
        
    }
    else
    {
        printf("\n DUT state not ok: cannot move to  SYNRCVD \n");
        return 1;
    }
    
    //4. TESTER: Send SYN with RST flag set with Sequence Number equal to <SEQ1>
    TCP_Packet TCP_P2 = CreateTCP();
    EditTCPField(&TCP_P2, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P2, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P2, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P2, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P2, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P2, TCP, TCP_AckNumber, (void *)(intptr_t)rseqN);
    TCP_Compute_checksum(&TCP_P2);
    SendTCP(TCP_P2);
    
    //5. TESTER: Send SYN with Sequence Number equal to <SEQ2>
     TCP_Packet TCP_P3 = CreateTCP();
    EditTCPField(&TCP_P3, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P3, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P3, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P3, TCP, TCP_SeqNumber, (void *)(intptr_t)seqN2);
    TCP_Compute_checksum(&TCP_P3);
    SendTCP(TCP_P3);
    
    //6 DUT: Sends a SYN,ACK with Ack Number equal to <SEQ2>+1 
    TCP_Packet TCP_RP3 = ListenTCP(f);
    if (TCP_RP3.length != 0)
    {
        uint8 rSYN = (uint8)GetTCPField(&TCP_RP3, TCP, TCP_FlagSYN);
        uint8 rACK = (uint8)GetTCPField(&TCP_RP3, TCP, TCP_FlagACK);
        uint32 rseqN = (uint32)GetTCPField(&TCP_RP3, TCP, TCP_SeqNumber);
        ackN= (uint32)GetTCPField(&TCP_RP3, TCP, TCP_AckNumber);
        if ((rSYN == 1) && (rACK == 1) && (ackN == seqN2 + 1 ))
        {
            printf("\n Test passed \n");
            End_Test_Mode_2(socketID,  seqN2++,  ackN);
            return 0;
            
        }
        else
        {
            printf("\n DUT response does not SYN,ACK :  cannot move to  SYNRCVD \n");
            End_Test_Mode_2(socketID,  seqN2++,  ackN);
            return 1;
        }
    }
    else 
    {
        printf("\n DUT not responding : cannot move to  SYNRCVD \n");
        End_Test_Mode_2(socketID,  seqN2++,  ackN);
        return 1; 
    }
}
