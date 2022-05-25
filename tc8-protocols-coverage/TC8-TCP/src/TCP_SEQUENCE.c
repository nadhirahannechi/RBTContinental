/**
 *  Copyright (C) [year] Focus Corporation
 *
 *  \file     TCP_SEQUENCE.c
 *  \brief    This file contains the tests of TCP SEQUENCE
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
#include "TCP_SEQUENCE.h"
#include"TestabilityProtocol_api.h"
#include"TestabilityProtocol_Intern.h"

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


int TCP_SEQUENCE_01() 
{
    int socketID,socketTester;
    uint32 seqN, ackN;
    int ISN = 5;
    //open controle connection
    //TP_OpenControlChannel();
    StartTest();

    // Move DUT to established state
    
    int moveDUTToListenResult = moveDUTToListen(&socketID,  &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon);
    if (moveDUTToListenResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ISN);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) {
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rSYN == 1) && (rACK == 1)) {
            int rACKNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            if (rACKNumber == ISN + 1) {
                printf("\nTest passed\n");
                End_Test_Mode_2(socketID,  seqN,  ackN);
                return 0;
            } else {
                printf("\nTest failed --> DUT responded with unexpected SEQ number\n");
                End_Test_Mode_2(socketID,  seqN,  ackN);
                return 1;
            }
        }
    } else {
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

int TCP_SEQUENCE_02()
{
    //open controle connection
    //TP_OpenControlChannel();
    StartTest();

    // Move DUT to SYN-SENT state
    int socketID;
    uint32 seqN, ackN;
    int moveDUTToSYNSENTResult = moveDUTToSYNSENT(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToSYNSENTResult == Result_OK)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT response is not ACK\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    // Send SYN,ACK
    int ISN = 5;
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ISN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // Sniffing DUT response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) {
        // Verifying that DUT responses with a SYN,ACK
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rSYN == 1) && (rACK == 1)) {
            int rACKNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            if (rACKNumber == ISN + 1) {
                printf("\nTest passed\n");
                End_Test_Mode_2(socketID,  seqN,  ackN);
                return 0;
            } else {
                printf("\nTest failed --> DUT responded with unexpected SEQ number\n");
                End_Test_Mode_2(socketID,  seqN,  ackN);
                return 1;
            }
        }
    } else {
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

int TCP_SEQUENCE_03() {
    int socketID,socketTester;
    uint32 seqN, ackN;
    //open controle connection
    //TP_OpenControlChannel();
    StartTest();

    // Move DUT to established state
    
    int moveDUTToListenResult = moveDUTToListen(&socketID,  &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon);
    if (moveDUTToListenResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    // Send SYN
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // Sniffing DUT response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) {
        // Verifying that DUT responses with a SYN,ACK
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rSYN == 1) && (rACK == 1)) {
            int rACKNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            if (rACKNumber == 1) {
                printf("\nTest passed\n");
                End_Test_Mode_2(socketID,  seqN,  ackN);
                return 0;
            } else {
                printf("\nTest failed --> DUT responded with unexpected SEQ number\n");
                End_Test_Mode_2(socketID,  seqN,  ackN);
                return 1;
            }
        }
    } else {
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

int TCP_SEQUENCE_04() {
    //open controle connection
    //TP_OpenControlChannel();
    StartTest();

    // Move DUT to established state
    int socketID,socketTester;
    uint32 seqN, ackN;
    int MoveToEstablished = moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port , TCPConfig.Maxcon, &seqN, &ackN);
    if (MoveToEstablished == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    // Send SYN packet with maximum sequence value
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0xFFFFFFFF);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) {
        // Verify that DUT sends SYN,ACK
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rSYN == 1) && (rACK == 1)) {
            int rACKNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            if (rACKNumber == 0) {
                printf("\nTest passed\n");
                End_Test_Mode_2(socketID,  seqN,  ackN);
                return 0;
            } else {
                printf("\nTest failed --> DUT responded with unexpected SEQ number\n");
                End_Test_Mode_2(socketID,  seqN,  ackN);
                return 1;
            }
        }
    } else {
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

int TCP_SEQUENCE_05() {
    //open controle connection
    //TP_OpenControlChannel();
    StartTest();

    // Move DUT to established state
    int socketID,socketTester;
    uint32 seqN, ackN, expectedACKN;
    int MoveToEstablished = moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port , TCPConfig.Maxcon, &seqN, &ackN);
    if (MoveToEstablished == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    // Send a first TCP packet
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // Set expected acknowledgment number
    expectedACKN = seqN + 8;

    // Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        // Verify that DUT sends an ACK with correct acknowledgement number
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rAckNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        if ((rACK == 1) && (rAckNumber == expectedACKN))
        {
            // Test OK
            //End_Test_Mode_2(socketID,  seqN,  ackN);
        }
        else
        {
            printf("\nTest failed --> Response is not ACK or with unexpected Ack number\n");
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

    // Update sequence and acknowledgment numbers
    seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
    ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
    expectedACKN += 8; 

    // Send a second TCP segment
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // Receive DUT responses
    TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        // Verify that DUT sends an ACK with correct acknowledgement number
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rAckNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        if ((rACK == 1) && (rAckNumber == expectedACKN))
        {
             printf("\nTest passed\n");
             End_Test_Mode_2(socketID,  seqN,  ackN);
             return 0;
        }
        else
        {
            printf("\nTest failed --> Response is not ACK or with unexpected Ack number\n");
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

}
