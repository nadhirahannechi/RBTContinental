/**
 *  Copyright (C) [year] Focus Corporation
 *
 *  \file     TCP_CALL_RECEIVE.c
 *  \brief    This file contains the tests of TCP CALL RECEIVE
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
#include "TCP_CHECKSUM.h"
#include <sys/time.h>
#include "TestabilityProtocol_api.h"
#include "TestabilityProtocol_Intern.h"

/******************************************************************************
* DEFINES AND MACROS
******************************************************************************/


/******************************************************************************
* GLOBAL VARIABLES AND CONSTANTS
******************************************************************************/

TP_ResultID_t TP_CreateAndBind_result_checksum = RID_E_NOK;            /** <the callback result of create and bind > **/
uint16 TP_CreateAndBind_socketID_checksum = -1;                           /**< Socket file description ID created*/
/******************************************************************************
* STATIC FUNCTION PROTOTYPES
******************************************************************************/
void TP_CreateAndBind_cb_checksum(TP_ResultID_t result, uint16 sock);
/******************************************************************************
* Local FUNCTIONS
******************************************************************************/
void TP_CreateAndBind_cb_checksum(TP_ResultID_t result, uint16 sock)
{
    TP_CreateAndBind_result_checksum= result;
    TP_CreateAndBind_socketID_checksum = sock;
}


/***************************************************************************************************
*
*   FUNCTION NAME: TP_CreateAndBind_cb_common_file
*
***************************************************************************************************/
/**
* @brief
*  the callback result of TCP create and bind.
*  \param[in]    TP_ResultID_t  : result      - callback result
*  \param[in]    uint16         : sock        - socket ID created
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/


int TCP_CHECKSUM_01()
{
    // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to ESTABLISHED state at a <wnp>
    int socketId,socketTester;
    uint32 seqN, ackN;

    int moveServDUTToEstablishedResult = moveServDUTToEstablished(&socketId,&socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveServDUTToEstablishedResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> cannot move to Established\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
    
    // 2. TESTER: Send a data segment with correct checksum
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
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
        // verify that DUT sends an ACK segment
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
            return 0;
        }
    }
    else
    {
        // DUT not responding
        printf("\nTest failed --> DUT is not responding\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 0;
    }
	
}

int TCP_CHECKSUM_02()
{
    // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to ESTABLISHED state at a <wnp>
    int socketId,socketTester;
    uint32 seqN, ackN;

    int moveServDUTToEstablishedResult = moveServDUTToEstablished(&socketId,&socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveServDUTToEstablishedResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> cannot move to Established\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
    

    // 2. TESTER: Send a data segment with incorrect checksum
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    SendTCP(TCP_P);

    // 3. DUT: Do not send ACK
    // Recieve DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 0;
    }
    else
    {
        // Unexpected response
        printf("\nTest failed --> DUT responded while it shouldn't\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
}


/***************************************************************************************************
*
*   FUNCTION NAME: TCP_CHECKSUM_03
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP CHECKSUM 03 test.
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
int TCP_CHECKSUM_03()
{
     // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to ESTABLISHED state at a <wnp>
    int socketId,socketTester;
    uint32 seqN, ackN;

    int moveServDUTToEstablishedResult = moveServDUTToEstablished(&socketId,&socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveServDUTToEstablishedResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> cannot move to Established\n");
        End_Test_Mode_2(socketId,  seqN,  ackN);
        return 1;
    }
    
    //2. TESTER: Cause the application on the DUT-side to issue a SEND request for a data segment
    vint8 data;
    data.Data =(uint8*) calloc(sizeof("data"),sizeof(uint8));
    data.Data = (uint8*)"data";
    data.dataLength = sizeof("data")-1;
    TP_TcpSendData(NULL, socketTester, data.dataLength,  0, data);
    
    
    //3. DUT: Send the data segment
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) 
    {
        //4. TESTER: Verify that correct checksum is present in the incoming segment 
        
        int RseqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int RackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rPSHFlag  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagPSH);
        int  rAckFlag = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        char* payloadData  = (char *)GetTCPField(&TCP_RP, PAYLAOD, PAYLOAD_data);
        if ((rPSHFlag==1) && (rAckFlag==1) && ( seqN ==RackN) && (ackN == RseqN) && (strcmp(payloadData,"data") == 0))
        
        {
            printf("\nDUT: Send the data segment\n");
            seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
        else 
        {
            printf("\nTest failed --> DUT  does not send the segment\n");
            End_Test_Mode_2(socketId,seqN, ackN);
            return 1;
        }
        
        if (TCP_Verify_Correct_checksum(TCP_RP) == 0) 
        {
            printf("\nTest passed\n");
            End_Test_Mode_2(socketId,seqN, ackN);
            return 0;
        } 
        else 
        {
            // Unexpected response
            printf("\nTest failed --> checksum incorrect\n");
            End_Test_Mode_2(socketId,seqN, ackN);
            return 1;
        }
    }
    else
    {
        // DUT not responding
        printf("\nTest failed --> DUT is not responding\n");
        End_Test_Mode_2(socketId,seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_CHECKSUM_04
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP CHECKSUM 04 test.
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
int TCP_CHECKSUM_04()
{
        
    int socketId;
    uint32 rSeqNumber1,rSeqNumber2,ackN,seqN;
    struct timeval stop, start;
    // Start Test
    StartTest();
    
    // 1. TESTER: Cause the DUT-side application to issue an active OPEN call
    ip4addr ipv4;
    ipv4.dataLength = 4;
    stoiIP(TCPConfig.DUT_IP, ipv4.Data);
    // Cause DUT to issue an active open call
    TP_TcpCreateAndBind(TP_CreateAndBind_cb_checksum, TRUE, TCPConfig.DUT_Port, ipv4);
    
    int i = 0;
    while((TP_CreateAndBind_result_checksum == RID_E_NOK)&&(i<10))
    {
        sleep(1);
        i++;
    }
    
    if (TP_CreateAndBind_result_checksum == RID_E_OK)
    {
        socketId = TP_CreateAndBind_socketID_checksum;
        TP_CreateAndBind_result_checksum = RID_E_NOK;
        printf("\n socket created with success : socketID= %d\n",TP_CreateAndBind_socketID_checksum);
    }
    else
    {
        printf("\n Test failed : socket cannot create socket \n");
        End_Test_Mode_2(socketId,seqN, ackN);
        return 1;
    }

    stoiIP(TCPConfig.TESTER_IP, ipv4.Data);
    TP_TcpConnect(NULL, socketId, TCPConfig.Tester_Port, ipv4);



    // 2. DUT: Send a SYN
    // Receive SYN
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        
        gettimeofday(&start, NULL);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        if (rSYN == 1)
        {
            seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            rSeqNumber1 = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        }
        else
        {
            printf("\n DUT response is not SYN \n");
            End_Test_Mode_2(socketId,seqN, ackN);
            return 2;
        }
    }
    else
    {
        printf("\n DUT response not responding \n");
        End_Test_Mode_2(socketId,seqN, ackN);
        return 3;
    }
    
    


    // 3. TESTER: Send RST,ACK to take DUT to CLOSED state
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(rSeqNumber1 + 1));
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 4. TESTER: Cause the DUT-side application to issue another active OPEN call
    /*TP_TcpCreateAndBind(TP_CreateAndBind_cb_checksum, TRUE, TCPConfig.DUT_Port, ipv4);
    
    i = 0;
    while((TP_CreateAndBind_result_checksum == RID_E_NOK)&&(i<10))
    {
        sleep(1);
        i++;
    }
    
    if (TP_CreateAndBind_result_checksum == RID_E_OK)
    {
        socketId = TP_CreateAndBind_socketID_checksum;
        TP_CreateAndBind_result_checksum = RID_E_NOK;
        printf("\n socket created with success : socketID= %d\n",TP_CreateAndBind_socketID_checksum);
    }
    else
    {
        End_Test_Mode_2(socketId,seqN, ackN);
        return 4;
    }*/

    stoiIP(TCPConfig.TESTER_IP, ipv4.Data);
    TP_TcpConnect(NULL, socketId, TCPConfig.Tester_Port, ipv4);



    // 5. DUT: Send SYN
    // Receive SYN

    // Sniffing and verifying DUT response
    i=0;
    TCP_RP = ListenTCP(f);
    while ((TCP_RP.length == 0) && (i<10))
    i++;
    
    if (TCP_RP.length != 0)
    {
        
        gettimeofday(&stop, NULL);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        if (rSYN == 1)
        {
            seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            rSeqNumber2 = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        }
        else
        {
            printf("\n DUT response is not SYN \n");
            End_Test_Mode_2(socketId,seqN, ackN);
            return 5;
        }
    }
    else
    {
        printf("\n DUT response not responding \n");
        End_Test_Mode_2(socketId,seqN, ackN);
        return 6;
    }


    int seqdiff = rSeqNumber2 - rSeqNumber1;
    double deltaT_us = time_diff(stop, start);

    // 6. TESTER: Verify that sequence number of the recent SYN is more than the sequence number of the previous SYN 
    // (with the appropriate checking of wrap around of counter) at least by the 
    // (difference between the times of reception of two SYNs)x250000
    if (seqdiff > (deltaT_us * 25000)) 
    {
        printf("\nTest Passed\n");
        End_Test_Mode_2(socketId,seqN, ackN);
        return 7;
    }
    else
    {
        printf("\nTest failed --> The difference between sequence numbers in not consistent\n");
        End_Test_Mode_2(socketId,seqN, ackN);
        return 8; 
    }

}
