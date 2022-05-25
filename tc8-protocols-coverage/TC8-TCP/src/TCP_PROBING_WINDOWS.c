/**
 *  Copyright (C) [year] Focus Corporation
 *
 *  \file     TCP_PROBING_WINDOWS.c
 *  \brief    This file contains the tests of TCP PROBING WINDOWS
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
#include "TestabilityProtocol_api.h"
#include "TCP_PROBING_WINDOWS.h"
#include <sys/time.h>

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
*   FUNCTION NAME: TCP_PROBING_WINDOWS_02
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP PROBING WINDOWS 02 test.
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
int TCP_PROBING_WINDOWS_02()
{
    int socketID,socketTester;
    uint32 seqN = 0 , ackN = 0;
	
	// Start Test
    StartTest();
	
	//1. TESTER: Cause the DUT to move to SYN-RCVD state at a <wnp>initiated by passive  OPEN call
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
	

    // Sending SYN to DUT
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0x05);
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
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rSYN == 1) && (rACK == 1))
        {
            seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber); 
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not SYN,ACK\n");
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 1;
        }
            
    }
    else
    {
        // No response sent
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
    
    
    //2. TESTER: Send ACK segment with a window size having the MSB set
    TCP_Packet TCP_P2 = CreateTCP();
    EditTCPField(&TCP_P2, TCP, TCP_FlagSYN, (void *)(intptr_t)0);
    EditTCPField(&TCP_P2, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P2, TCP, TCP_WindowsSize, (void *)(intptr_t)0xffff);
    EditTCPField(&TCP_P2, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P2, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P2, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P2, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P2);
    SendTCP(TCP_P2);
	
	//3. TESTER: Cause the application on the DUT-side to issue a SEND request for a data segment
	vint8 data;
    data.Data =(uint8*) calloc(4,sizeof(uint8));
    data.Data = (uint8*)"data";
    data.dataLength = sizeof("data")-1;
    TP_TcpSendData(NULL, socketTester, data.dataLength,  0, data);
    
    
    //4. DUT: Send the data segment
    TCP_Packet TCP_RP2 = ListenTCP(f);
    if (TCP_RP2.length != 0) 
    {
        int rPSHFlag  = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagPSH);
        int   rAckFlag = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagACK);
        char* payloadData  = (char *)GetTCPField(&TCP_RP2, PAYLAOD, PAYLOAD_data);
        if ((rPSHFlag == 1)  && (rAckFlag==1) && (strcmp(payloadData,(char*)data.Data)==0))
        {
            printf("\nTest passed\n");
            End_Test_Mode_2(socketID,seqN, ackN); 
            return 0;
        } 
        else
        {

            TP_GenaralEndTest(NULL,0,(text){0});
            printf("\nTest failed --> DUT response not ok\n");
            return 1;
        }
        
    }
     else 
    {
        // DUT not responding
        printf("\nTest failed --> DUT is not responding\n");
        End_Test_Mode_2(socketID,seqN, ackN); 
        return 1;
    }
}



/***************************************************************************************************
*
*   FUNCTION NAME: TCP_PROBING_WINDOWS_03
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP PROBING WINDOWS 02 test.
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
int TCP_PROBING_WINDOWS_03()
{
    //start test
    StartTest();

    //1. TESTER: Cause the DUT to move on to ESTABLISHED state at a <wnp> 
    uint32 seqN, ackN,FirstAckN,FirstSeqN;
	int socketID,socketTester;


    // 1. TESTER: Cause the DUT to move on to ESTABLISHED state
    int moveServDUTToEstablishedResult = moveServDUTToEstablished(&socketID,&socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveServDUTToEstablishedResult == Result_OK)
	{
        //misra
    }
    else
    {
        printf("\nTest failed --> cannot move to Established\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
	
    //2. TESTER: Cause the application on the DUT-side to issue one data SEND request
    vint8 data;
    data.Data =(uint8*) malloc(sizeof("First segment")*sizeof(uint8));
    data.Data = (uint8*) "First segment";
    data.dataLength = sizeof("First segment")-1;
    TP_TcpSendData(NULL, 6, data.dataLength,  0, data);

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
        
        if ((rPSHFlag==1) && (rAckFlag==1) && (ackN ==RseqN) && (seqN== RackN) && (strcmp(payloadData,"First segment") == 0))
        
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


    //4. TESTER: Send the ACK
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN ));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN+sizeof("First segment")-1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    seqN=seqN+sizeof("First segment")-1 ;
    //5. TESTER: Cause the application on the DUT-side to issue two more SEND calls for data segments
    vint8 data2;
    data2.Data =(uint8*) calloc(sizeof("Second segment"),sizeof(uint8));
    data2.Data = (uint8*) "Second segment";
    data2.dataLength = sizeof("Second segment")-1;
    TP_TcpSendData(NULL, socketTester, data2.dataLength,  0, data2);
    TP_TcpSendData(NULL, socketTester, data2.dataLength,  0, data2);
    //free(data2.Data);

    //6. DUT: Send the data segments
    TCP_Packet TCP_RP2 = ListenTCP(f);
    if (TCP_RP2.length != 0) 
    { 
        int RseqN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_SeqNumber);
        int RackN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_AckNumber);
        int rPSHFlag  = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagPSH);
        int  rAckFlag = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagACK);
        char* payloadData  = (char *)GetTCPField(&TCP_RP2, PAYLAOD, PAYLOAD_data);
        if ((rPSHFlag==1) && (rAckFlag==1) && (ackN ==RackN) && (seqN== RseqN) && (strcmp(payloadData,"Second segment") == 0))
        
        {
            printf("\nDUT: Send the data segment\n");
            seqN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_SeqNumber);
            ackN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_AckNumber);
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
        printf("\nTest failed --> DUT  does not sent the 2nd segment\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }


    //7. TESTER: Send ACK for the first segment with an updated window value of zero
    TCP_Packet TCP_P_2 = CreateTCP();
    EditTCPField(&TCP_P_2, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P_2, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P_2, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P_2, TCP, TCP_SeqNumber, (void *)(intptr_t) ackN );
    EditTCPField(&TCP_P_2, TCP, TCP_AckNumber, (void *)(intptr_t)( seqN+sizeof("Second segment")-1));
    TCP_Compute_checksum(&TCP_P_2);
    SendTCP(TCP_P_2);
    
    seqN=seqN+sizeof("Second segment")-1;
    TCP_RP2 = ListenTCP(f);
    if (TCP_RP2.length != 0) 
    { 
        int RseqN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_SeqNumber);
        int RackN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_AckNumber);
        int rPSHFlag  = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagPSH);
        int  rAckFlag = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagACK);
        char* payloadData  = (char *)GetTCPField(&TCP_RP2, PAYLAOD, PAYLOAD_data);
        if ((rPSHFlag==1) && (rAckFlag==1) && (ackN == RackN) && (seqN== RseqN ) && (strcmp(payloadData,"Second segment") == 0))
        
        {
            printf("\nDUT: Send the data segment\n");
            seqN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_SeqNumber);
            ackN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_AckNumber);
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
        printf("\nTest failed --> DUT  does not sent the 2nd segment\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }
    TCP_P_2 = CreateTCP();
    EditTCPField(&TCP_P_2, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P_2, TCP, TCP_WindowsSize, (void *)(intptr_t)0x00);
    EditTCPField(&TCP_P_2, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P_2, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P_2, TCP, TCP_SeqNumber, (void *)(intptr_t) ackN );
    EditTCPField(&TCP_P_2, TCP, TCP_AckNumber, (void *)(intptr_t)( seqN+sizeof("Second segment")-1));
    TCP_Compute_checksum(&TCP_P_2);
    SendTCP(TCP_P_2);
    seqN=seqN+sizeof("Second segment")-1;    

    //8. TESTER: Cause the application on the DUT-side to issue a SEND request for a data segment
    vint8 data3;
    data3.Data =(uint8*) malloc(sizeof("Third segment")*sizeof(uint8));
    data3.Data = (uint8*) "Third segment";
    data3.dataLength = sizeof("Third segment")-1;
    TP_TcpSendData(NULL, socketTester, data3.dataLength,  0, data3);
    

    //9. DUT: Do not send the segment as the \"useable window\" is negative
    TCP_Packet TCP_RP3 = ListenTCP(f);
    if (TCP_RP3.length == 0) 
    { 
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 0;    
    }
    else 
    {
        char* payloadData  = (char *)GetTCPField(&TCP_RP3, PAYLAOD, PAYLOAD_data);
        
        if (strcmp(payloadData,"Third segment"))
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


}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_PROBING_WINDOWS_04
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP PROBING WINDOWS 02 test.
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

int TCP_PROBING_WINDOWS_04()
{
    StartTest();

    // 1. TESTER: Cause DUT to move on to ESTABLISHED state 
    int socketID;
    uint32 seqN, ackN,FirstAckN,FirstSeqN;
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
	
    //2. TESTER: Cause the application on the DUT-side to issue a SEND request for data segment
    vint8 data;
    data.Data =(uint8*) calloc(15,sizeof(uint8));
    data.Data = (uint8*) "First segment";
    data.dataLength = sizeof("First segment")-1;
    TP_TcpSendData(NULL, socketID, data.dataLength,  0, data);
    //free(data.Data);
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
        if ((rPSHFlag==1) && (rAckFlag==1) && (ackN ==RackN) && (seqN== RseqN) && (strcmp(payloadData,"First segment") == 0))
        
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
    
    //4. TESTER: Send ACK declaring a zero receive window
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_WindowsSize, (void *)(intptr_t)0x00);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)( ackN ));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)( seqN+sizeof("First segment")-1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    
    //getting new seq and ack
    seqN = (uint32)GetTCPField(&TCP_P, TCP, TCP_SeqNumber);
    ackN = (uint32)GetTCPField(&TCP_P, TCP, TCP_AckNumber);
    
    //5. TESTER: Cause the application on the DUT-side to issue another SEND request for data segment
    vint8 data2;
    data2.Data =(uint8*) calloc(sizeof("Second segment"),sizeof(uint8));
    data2.Data = (uint8*) "Second segment";
    data2.dataLength = sizeof("Second segment")-1;
    TP_TcpSendData(NULL, socketID, data2.dataLength,  0, data2);
    
    
    //8. DUT: Keep on sending the zero window probes, staying in the ESTABLISHED state, as long as the tester acknowledges each of them at every reception 
    TCP_Packet TCP_RP2 = ListenTCP(f);
    if (TCP_RP2.length != 0) 
    { 
        int RseqN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_SeqNumber);
        int RackN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_AckNumber);
        int rPSHFlag  = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagPSH);
        int  rAckFlag = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagACK);

        if ((rPSHFlag==0) && (rAckFlag==1) && (RseqN ==ackN-1) && (RackN== seqN))
        
        {
            printf("\nDUT: Send the keep alive segment\n");
            seqN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_SeqNumber);
            ackN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_AckNumber);
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
    
    if(checkState(ESTABLISHED))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in ESTABLISHED state\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_PROBING_WINDOWS_05
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP PROBING WINDOWS 02 test.
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
int TCP_PROBING_WINDOWS_05()
{
    StartTest();

    // 1. TESTER: Cause DUT to move on to ESTABLISHED state 
    double deltaT_us_1;
	int socketID,socketTester;
    uint32 seqN, ackN;

    // 1. TESTER: Cause the DUT to move on to ESTABLISHED state
    int moveServDUTToEstablishedResult = moveServDUTToEstablished(&socketID,&socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveServDUTToEstablishedResult == Result_OK)
	{
        //misra
    }
    else
    {
        printf("\nTest failed --> cannot move to Established\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
	
    //2. TESTER: Cause the application on the DUT-side to issue a SEND request for data segments
    vint8 data;
    data.Data =(uint8*) calloc(sizeof("First segment"),sizeof(uint8));
    data.Data = (uint8*) "First segment";
    data.dataLength = sizeof("First segment")-1;
    TP_TcpSendData(NULL, socketTester, data.dataLength,  0, data);
    //free(data.Data);

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

        int rPSHFlag  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagPSH);
        int  rAckFlag = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        char* payloadData  = (char *)GetTCPField(&TCP_RP, PAYLAOD, PAYLOAD_data);
        if ((rPSHFlag==1) && (rAckFlag==1)  && (strcmp(payloadData,"First segment") == 0))
        
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
    
    //4. TESTER: Send ACK declaring a zero window
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_WindowsSize, (void *)(intptr_t)0x00);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN ));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)( seqN+sizeof("First segment")-1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    
    
    //getting last seq and ack numbers
    seqN = (uint32)GetTCPField(&TCP_P, TCP, TCP_SeqNumber);
    ackN = (uint32)GetTCPField(&TCP_P, TCP, TCP_AckNumber);
    
    //5. TESTER: Cause the application on the DUT-side to issue another SEND request for data segment
    vint8 data2;
    data2.Data =(uint8*) calloc(sizeof("Second segment"),sizeof(uint8));
    data2.Data = (uint8*) "Second segment";
    data2.dataLength = sizeof("Second segment")-1;
    TP_TcpSendData(NULL, socketTester, data2.dataLength,  0, data2);

    struct timeval stop, start;
    gettimeofday(&start, NULL);

    //6. DUT: Send a zero window probe after some delay
    TCP_Packet TCP_RP2 = ListenTCP(f);
    if (TCP_RP2.length != 0) 
    { 
        int RseqN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_SeqNumber);
        int RackN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_AckNumber);
        int rPSHFlag  = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagPSH);
        int  rAckFlag = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagACK);

        if ((rPSHFlag==0) && (rAckFlag==1) && (RseqN ==ackN-1) && (RackN== seqN))
        
        {
            printf("\nDUT: Send the keep alive segment\n");
            seqN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_SeqNumber);
            ackN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_AckNumber);
            gettimeofday(&stop, NULL);
            deltaT_us_1 = time_diff(stop,start);
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
    

    //7. TESTER: Verify that the probe has been sent by the DUT after waiting for at least the retransmission timeout 
    if ((deltaT_us_1 > 900000000) && (deltaT_us_1 < 1100000000))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> the delays between successive probes does not increases continually\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }
    
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_PROBING_WINDOWS_06
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP PROBING WINDOWS 02 test.
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
int TCP_PROBING_WINDOWS_06()
{
    StartTest();

    // 1. TESTER: Cause DUT to move on to ESTABLISHED state 
    uint32 seqN, ackN;
    double deltaT_us_1,deltaT_us_2;
	int socketID,socketTester;

    // 1. TESTER: Cause the DUT to move on to ESTABLISHED state
    int moveServDUTToEstablishedResult = moveServDUTToEstablished(&socketID,&socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveServDUTToEstablishedResult == Result_OK)
	{
        //misra
    }
    else
    {
        printf("\nTest failed --> cannot move to Established\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
	
    //2. TESTER: Cause the application on the DUT-side to issue a SEND request for data segments
    vint8 data;
    data.Data =(uint8*) calloc(sizeof("First segment"),sizeof(uint8));
    data.Data = (uint8*) "First segment";
    data.dataLength = sizeof("First segment")-1;
    TP_TcpSendData(NULL, socketTester, data.dataLength,  0, data);
    //free(data.Data);

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

        int rPSHFlag  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagPSH);
        int  rAckFlag = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        char* payloadData  = (char *)GetTCPField(&TCP_RP, PAYLAOD, PAYLOAD_data);
        if ((rPSHFlag==1) && (rAckFlag==1)  && (strcmp(payloadData,"First segment") == 0))
        
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
    
    //4. TESTER: Send ACK declaring a zero window
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_WindowsSize, (void *)(intptr_t)0x00);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN ));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)( seqN+sizeof("First segment")-1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    
    
    //getting last seq and ack numbers
    seqN = (uint32)GetTCPField(&TCP_P, TCP, TCP_SeqNumber);
    ackN = (uint32)GetTCPField(&TCP_P, TCP, TCP_AckNumber);
    
    //5. TESTER: Cause the application on the DUT-side to issue another SEND request for data segment
    vint8 data2;
    data2.Data =(uint8*) calloc(sizeof("Second segment"),sizeof(uint8));
    data2.Data = (uint8*) "Second segment";
    data2.dataLength = sizeof("Second segment")-1;
    TP_TcpSendData(NULL, socketTester, data2.dataLength,  0, data2);
    //free(data2.Data);
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    //6. DUT: Send a zero window probe after some delay
    TCP_Packet TCP_RP2 = ListenTCP(f);
    if (TCP_RP2.length != 0) 
    { 
        int RseqN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_SeqNumber);
        int RackN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_AckNumber);
        int rPSHFlag  = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagPSH);
        int  rAckFlag = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagACK);

        if ((rPSHFlag==0) && (rAckFlag==1) && (RseqN ==ackN-1) && (RackN== seqN))
        
        {
            printf("\nDUT: Send the keep alive segment\n");
            seqN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_SeqNumber);
            ackN = (uint32)GetTCPField(&TCP_RP2, TCP, TCP_AckNumber);
            gettimeofday(&stop, NULL);
            deltaT_us_1 = time_diff(stop,start);
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
    
    // send the retresmission segment
    gettimeofday(&start, NULL);
    TCP_Packet TCP_RP3 ;
    int i=0;
    while ((TCP_RP3.length == 0) && (i<10))
    {
        TCP_RP3 = ListenTCP(f);
        i++;
    }
    
    if (TCP_RP3.length != 0) 
    { 
        int RseqN = (uint32)GetTCPField(&TCP_RP3, TCP, TCP_SeqNumber);
        int RackN = (uint32)GetTCPField(&TCP_RP3, TCP, TCP_AckNumber);
        int rPSHFlag  = (int)GetTCPField(&TCP_RP3, TCP, TCP_FlagPSH);
        int  rAckFlag = (int)GetTCPField(&TCP_RP3, TCP, TCP_FlagACK);
        
        if ((rPSHFlag==0) && (rAckFlag==1) && (RseqN ==seqN) && (RackN== ackN))
        {    
            printf("\nDUT: Send the keep alive segment\n");
            gettimeofday(&stop, NULL);
            deltaT_us_2 = time_diff(stop,start);
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
    
    //7. TESTER: Verify that the delays between successive probes increases continually
    if (deltaT_us_2> deltaT_us_1)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> the delays between successive probes does not increases continually\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }
    
}
