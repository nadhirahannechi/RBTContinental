/**
 *  Copyright (C) [year] Focus Corporation
 *
 *  \file     TCP_NAGLE.c
 *  \brief    This file contains the tests of TCP NAGLE
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
#include "TCP_NAGLE.h"
#include "TestabilityProtocol_api.h"
#include <string.h>
/******************************************************************************
* DEFINES AND MACROS
******************************************************************************/

/******************************************************************************
* GLOBAL VARIABLES AND CONSTANTS
******************************************************************************/


/******************************************************************************
* LOCAL FUNCTION PROTOTYPES
******************************************************************************/


/******************************************************************************
* Local FUNCTIONS
******************************************************************************/



/******************************************************************************
* GLOBAL FUNCTIONS
******************************************************************************/


/***************************************************************************************************
*
*   FUNCTION NAME: TCP_NAGLE_02
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP NAGLE 02 test.
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
int TCP_NAGLE_02()
{
    StartTest();

    // 1. TESTER: Cause DUT to move on to ESTABLISHED state 
    int socketId,socketClient;
    uint32 seqN, ackN;
    moveServDUTToEstablished(&socketId,&socketClient, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);

    //2. TESTER: Cause the application on the DUT-side to issue a SEND request for a segment
    vint8 data;
    data.Data =(uint8*) calloc(sizeof("TCP From DUT"),sizeof(uint8));
    data.Data = (uint8*) "TCP From DUT";
    data.dataLength = sizeof("TCP From DUT") -1;


    TP_TcpSendData(NULL, socketClient, data.dataLength,  0, data);
    
    //3. DUT: Send the data segment
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) 
    { 
        int RseqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int RackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rPSHFlag  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagPSH);
        int  rAckFlag = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        char* payloadData  = (char *)GetTCPField(&TCP_RP, PAYLAOD, PAYLOAD_data);
       
        if ((rPSHFlag==1) && (rAckFlag==1) && (ackN ==RseqN) && (seqN==RackN ) && (strcmp(payloadData,"TCP From DUT") == 0))
        
        {
            printf("\nDUT: Send the data segment\n");
            seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
        else 
        {
            printf("\nTest failed --> DUT does not send the segment\n");
            End_Test_Mode_2(socketId,seqN, ackN);
            return 1;
        }
        
    }
    else 
    {
        printf("\nTest failed --> DUT  does not send the segment\n");
        End_Test_Mode_2(socketId,seqN, ackN);
        return 1;
    }

    //4. TESTER: Do not acknowledge the data
    //5. TESTER: Cause the application to issue another SEND request with the PSH bit (if possible to specify)
    vint8 data1;
    data1.Data =(uint8*) calloc(sizeof("TCP From DUT 2"),sizeof(uint8));
    data1.Data = (uint8*) "TCP From DUT 2";
    data1.dataLength = sizeof("TCP From DUT 2")-1;
    TP_TcpSendData(NULL, socketClient, data1.dataLength,  1<<3, data1);
    
    //6. DUT: Do not send the data segment
    TCP_Packet TCP_RP2 = ListenTCP(f);
    int acknumber,sequenceNumber;
    if (TCP_RP2.length == 0) 
    { 
        //misra 
        
    }
    else 
    {
        int RseqN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_SeqNumber);
        int RackN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_AckNumber);
        int rPSHFlag  = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagPSH);
        int  rAckFlag = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagACK);
        char* payloadData  = (char *)GetTCPField(&TCP_RP2, PAYLAOD, PAYLOAD_data);
        if ((rPSHFlag==1) && (rAckFlag==1) && (ackN ==RackN) && (seqN== RseqN) && (strcmp(payloadData,"TCP From DUT") == 0))
        
        {
            printf("\nDUT: Send the data segment\n");
            seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
        else if ((rPSHFlag==1) && (rAckFlag==1) && (ackN ==RackN) && (seqN== RseqN) && (strcmp(payloadData,"TCP From DUT 2") == 0))
        {
            printf("\nTest failed --> DUT sent the segment\n");
            End_Test_Mode_2(socketId,seqN, ackN);
            return 1;
        }
        else 
        {
            //misra
        }
        
    }
    //7. TESTER: Acknowledge the previous data
    TCP_Packet TCP_P1 = CreateTCP();
    EditTCPField(&TCP_P1, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P1, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P1, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P1, TCP, TCP_SeqNumber, (void *)(intptr_t)( ackN ));
    EditTCPField(&TCP_P1, TCP, TCP_AckNumber, (void *)(intptr_t)( seqN+sizeof("TCP From DUT")-1));
    TCP_Compute_checksum(&TCP_P1);
    SendTCP(TCP_P1);
    
    
    seqN = (uint32)GetTCPField(&TCP_P1, TCP, TCP_SeqNumber);
    ackN = (uint32)GetTCPField(&TCP_P1, TCP, TCP_AckNumber);
    
    //8. DUT: Send the latter data segment 
    TCP_Packet TCP_RP3 = ListenTCP(f);
    if (TCP_RP3.length != 0) 
    { 
        int RseqN = (uint32)GetTCPField(&TCP_RP3, TCP, TCP_SeqNumber);
        int RackN = (uint32)GetTCPField(&TCP_RP3, TCP, TCP_AckNumber);
        int rPSHFlag  = (int)GetTCPField(&TCP_RP3, TCP, TCP_FlagPSH);
        int  rAckFlag = (int)GetTCPField(&TCP_RP3, TCP, TCP_FlagACK);
        char* payloadData  = (char *)GetTCPField(&TCP_RP3, PAYLAOD, PAYLOAD_data);
    
        if ((rPSHFlag==1) && (rAckFlag==1) && (ackN ==RseqN) && (seqN== RackN) && (strcmp(payloadData,"TCP From DUT 2") == 0))
        
        {
            printf("\nTest passed\n");
            End_Test_Mode_2(socketId,seqN, ackN);
            return 0;
        }
        else 
        {
            printf("\nTest failed --> DUT  does not send the segment\n");
            End_Test_Mode_2(socketId,seqN, ackN);
            return 1;
        }
        
    }
    else 
    {
        printf("\nTest failed --> DUT  does  send the segment\n");
        End_Test_Mode_2(socketId,seqN, ackN);
        return 1;
    }
}


/***************************************************************************************************
*
*   FUNCTION NAME: TCP_NAGLE_03
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP NAGLE 03 test.
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
int TCP_NAGLE_03()
{

    StartTest();
    
    // 1. TESTER: Cause DUT to move on to ESTABLISHED state 
    int socketID,socketClient;
    uint32 seqN, ackN;
    moveServDUTToEstablished(&socketID, &socketClient, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);

    vint8 data;
    data.Data =(uint8*) calloc(sizeof("TCP From DUT"),sizeof(uint8));
    data.Data = (uint8*) "TCP From DUT";
    data.dataLength = sizeof("TCP From DUT") -1;


    TP_TcpSendData(NULL, socketClient, data.dataLength,  0, data);
    
    //3. DUT: Send the data segment
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) 
    { 
        int RseqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int RackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rPSHFlag  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagPSH);
        int  rAckFlag = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        char* payloadData  = (char *)GetTCPField(&TCP_RP, PAYLAOD, PAYLOAD_data);
        if ((rPSHFlag==1) && (rAckFlag==1) && (ackN ==RseqN) && (seqN==RackN ) && (strcmp(payloadData,"TCP From DUT") == 0))
        
        {
            printf("\nDUT: Send the data segment\n");
            seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
        else 
        {
            printf("\nTest failed --> DUT  does not sent the segment\n");
            End_Test_Mode_2(socketID,seqN, ackN);
            return 1;
        }
        
    }
    else 
    {
        printf("\nTest failed --> DUT  does not sent the segment\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }



    //4. TESTER: Do not acknowledge the data
    //5. TESTER: Cause the application to issue another send request of small size <ssz> with the PSH bit (if possible to specify)
    vint8 data1;
    data1.Data =(uint8*) calloc(TCPConfig.mss,sizeof(uint8));
    memset(data1.Data, 0, TCPConfig.mss);
    memset(data1.Data, 1, TCPConfig.ssz);
    data1.dataLength = TCPConfig.ssz;
    TP_TcpSendData(NULL, socketClient, data1.dataLength,  1<<3, data1);
    
    //6. DUT: Do not send the data segment
    TCP_Packet TCP_RP2 = ListenTCP(f);
    int acknumber,sequenceNumber;
    if (TCP_RP2.length == 0) 
    { 
        //misra 
        
    }
    else 
    {
        int RseqN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_SeqNumber);
        int RackN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_AckNumber);
        int rPSHFlag  = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagPSH);
        int  rAckFlag = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagACK);
        char* payloadData  = (char *)GetTCPField(&TCP_RP2, PAYLAOD, PAYLOAD_data);
        if ((rPSHFlag==1) && (rAckFlag==1) && (ackN ==RackN) && (seqN== RseqN) && (strcmp(payloadData,data1.Data) == 0))
        
        {
            printf("\nDUT: Send the data segment\n");
            seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
        else if ((rPSHFlag==1) && (rAckFlag==1) && (ackN ==RackN) && (seqN== RseqN) && (strcmp(payloadData,data1.Data) == 0))
        {
            printf("\nTest failed --> DUT sent the segment\n");
            End_Test_Mode_2(socketID,seqN, ackN);
            return 1;
        }
        else 
        {
            //misra
        }
        
    }
    

    //7. TESTER: Cause the application to issue one more SEND request with the aggregate data size equal to effective send MSS
    memset(data1.Data+TCPConfig.ssz, 2, TCPConfig.mss-TCPConfig.ssz);
    data1.dataLength = TCPConfig.mss-TCPConfig.ssz;
    TP_TcpSendData(NULL, socketClient, data1.dataLength,  1<<3, data1);

    

    //8. DUT: Send the aggregate data segment 
    TCP_Packet TCP_RP3 = ListenTCP(f);
    if (TCP_RP3.length != 0) 
    { 
        int RseqN = (uint32)GetTCPField(&TCP_RP3, TCP, TCP_SeqNumber);
        int RackN = (uint32)GetTCPField(&TCP_RP3, TCP, TCP_AckNumber);
        int rPSHFlag  = (int)GetTCPField(&TCP_RP3, TCP, TCP_FlagPSH);
        int  rAckFlag = (int)GetTCPField(&TCP_RP3, TCP, TCP_FlagACK);
        char* payloadData  = (char *)GetTCPField(&TCP_RP3, PAYLAOD, PAYLOAD_data);
        if ((rPSHFlag==1) && (rAckFlag==1) && (ackN ==RseqN) && (seqN== RackN) && (strcmp(payloadData,data1.Data) == 0))
        
        {
            printf("\nTest passed\n");
            End_Test_Mode_2(socketID,seqN, ackN);
            return 0;
        }
        else 
        {
            printf("\nTest failed --> DUT  does not sent the segment\n");
            End_Test_Mode_2(socketID,seqN, ackN);
            return 1;
        }
        
    }
    else 
    {
        printf("\nTest failed --> DUT  does  sent the segment\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }

}


