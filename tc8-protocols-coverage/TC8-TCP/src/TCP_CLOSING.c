/**
 *  Copyright (C) [year] Focus Corporation
 *
 *  \file     TCP_CLOSING.c
 *  \brief    This file contains the tests of TCP CLOSING
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
#include "TCP_CLOSING.h"
#include "TestabilityProtocol_api.h"
/******************************************************************************
* DEFINES AND MACROS
******************************************************************************/


/******************************************************************************
* GLOBAL VARIABLES AND CONSTANTS
******************************************************************************/
TP_ResultID_t result_cb_closing;
uint16 Length_cb_closing;
vint8 Data_cb_closing;

/******************************************************************************
* LOCAL FUNCTION PROTOTYPES
******************************************************************************/
void TP_TcpReceiveAndForward_event_cb_closing(TP_ResultID_t result, uint16 Length , vint8 Data);
/******************************************************************************
* Local FUNCTIONS
******************************************************************************/
void TP_TcpReceiveAndForward_event_cb_closing(TP_ResultID_t result, uint16 Length , vint8 Data)
{
    result_cb_closing = result;
    Length_cb_closing = Length;
    Data_cb_closing.dataLength = Data.dataLength;
    Data_cb_closing.Data = (uint8*) calloc(Data.dataLength,sizeof(uint8)); 
    memcpy(Data_cb_closing.Data,Data.Data,Data.dataLength);
}
/******************************************************************************
* GLOBAL FUNCTIONS
******************************************************************************/


/***************************************************************************************************
*
*   FUNCTION NAME: TCP_CLOSING_03
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_CLOSING_03 test.
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
int TCP_CLOSING_03()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause the DUT to move on to ESTABLISHED state
    int socketID,socketTester;
    uint32 seqN, ackN;
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
	
    //2. TESTER: Send a RST segment containing some data
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //Receiving DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {
        //3. DUT: Do not send any response
    }
    else
    {
        printf("\nTest failed --> DUT responded while it shouldn't\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    //4. TESTER: Verify that the DUT is in CLOSED state 
    if(checkState(CLOSED))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed -> DUT is not in CLOSE state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_CLOSING_06
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_CLOSING_06 test.
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
int TCP_CLOSING_06()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause the DUT to move on to ESTABLISHED state
    int socketID,socketTester;
    uint32 seqN, ackN;
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
	
    //2. TESTER: Cause DUT side application to issue a CLOSE call 
	TP_TcpCloseSocket(NULL, (uint16_t)socketTester, FALSE);
	
    //3. DUT: Send a FIN 
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        int rFIN  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagFIN);
        if(rFIN == 1)
        {
            printf("\nTest passed\n");
			End_Test_Mode_2(socketID,  seqN,  ackN);
            return 0;
        }
        else
        {
            printf("\nTest failed --> DUT response is not FIN\n");
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
/***************************************************************************************************
*
*   FUNCTION NAME: TCP_CLOSING_07
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_CLOSING_07 test.
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
int TCP_CLOSING_07()
{
    int socketID;
    uint32 seqN, ackN;


    // start test
    StartTest();

    // Move DUT to ESTABLISHED state   
    int moveToFINWAIT1=moveDUTToFINWAIT1(&socketID, TCPConfig.DUT_Port ,TCPConfig.Tester_Port,  TCPConfig.Maxcon, &seqN, &ackN);
    if(moveToFINWAIT1==Result_OK)
    {
        //misra
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> DUT response not ok\n");
        return 1;
    }
    
    //6. TESTER: Send some data to DUT
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    TCP_Compute_checksum(&TCP_P);

    //prepare step 8 
    TP_TcpReceiveAndForward(NULL, TP_TcpReceiveAndForward_event_cb_closing, socketID, 0xFFFF, 0xFFFF);

    SendTCP(TCP_P);
  
    //7. DUT: Send ACK of the received data
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP2 = ListenTCP(f);
    if (TCP_RP2.length != 0) 
    { 
        int rFACK  = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagACK);
        int rFIN  = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagFIN);
        if ((rFACK == 1) && (rFIN==0))
        {
          //result ok
        } 
        else
        {
           
            End_Test_Mode_2(socketID,seqN, ackN);
            printf("\nTest failed --> DUT response not ok\n");
            return 1;
        }
    }
    else 
    {
	    End_Test_Mode_2(socketID,seqN, ackN);

        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //8. TESTER: Check that DUT receives proper data
    sleep(3);
    if((Length_cb_closing > 0) && (result_cb_closing== RID_E_OK))
    {
        if((Data_cb_closing.dataLength==8) &&( strcmp((char*)Data_cb_closing.Data,"TEST TCP")==0))
        {
            //result ok
        }
        else 
        {
            End_Test_Mode_2(socketID,seqN, ackN);
            printf("\nTest failed --> DATA sent was corrupted\n");
            return 1;  
        }
    }
    else 
    {
        End_Test_Mode_2(socketID,seqN, ackN);
        printf("\nTest failed --> DATA sent was corrupted\n");
        return 1; 
    }
    //9. TESTER: Check that DUT remains in FIN-WAIT-1 state 
    if(checkState(FINWAIT1))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed -> DUT is not in CLOSE state\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }
}



/***************************************************************************************************
*
*   FUNCTION NAME: TCP_CLOSING_08
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_CLOSING_08 test.
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
int TCP_CLOSING_08()
{
    StartTest();
    //1. TESTER: Cause DUT to move  on to FINWAIT-2 state 
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

    //2. TESTER: Cause DUT side application to issue a RECEIVE call

    //3. DUT: Issue a RECEIVE call
    TP_TcpReceiveAndForward(NULL, TP_TcpReceiveAndForward_event_cb_closing, socketID, 0xFFFF, 0xFFFF);
    sleep(3);

    //4. TESTER: Send some data to DUT
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

    // 5. DUT: Send ACK of the received data
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) 
    { 
        int rFACK  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if (rFACK == 1)
        {
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest passed \n");
            return 0;
        } 
        else
        {
           
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest failed --> DUT response not ok\n");
            return 1;
        }
    }
    else 
    {
	    End_Test_Mode_2(socketID,  seqN,  ackN);

        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //6. TESTER: Check that DUT receives proper data
    if((Length_cb_closing > 0) && (result_cb_closing== RID_E_OK))
    {
        if((Data_cb_closing.dataLength==8) &&( strcmp((char*)Data_cb_closing.Data,"TEST TCP")==0))
        {
            //result ok
        }
        else 
        {
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest failed --> DATA sent was corrupted\n");
            return 1;  
        }
    }
    else 
    {
        //misra
    }
    //7. TESTER: Check that DUT remains in FIN-WAIT-2 state 
    if(checkState(FINWAIT2))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed -> DUT is not in CLOSE state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}


/***************************************************************************************************
*
*   FUNCTION NAME: TCP_CLOSING_09

***************************************************************************************************/
/**
* @brief
*  Performs TCP_CLOSING_09 test.
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
int TCP_CLOSING_09()
{
    int socketID;

    // start test
    StartTest();

    //1. TESTER: Take DUT to ESTABLISHED state 
    uint32 seqN, ackN;
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
    
    //2. TESTER: Send a FIN
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
   
    //3. DUT: Send ACK for the received FIN
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) 
    { 
        int rFACK  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rFFIN  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagFIN);
        if ( (rFACK == 1)&& (rFFIN==0) )
        {
            //result ok
        } 
        else
        {
           
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest failed --> DUT response not ok\n");
            return 1;
        }
    }
    else 
    {
	    End_Test_Mode_2(socketID,  seqN,  ackN);

        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }
    //4. TESTER: Cause the DUT side application to send some data
    vint8 data;
    data.dataLength = 4;
    data.Data =(uint8*) calloc(data.dataLength,sizeof(uint8));
    data.Data = (uint8*) "data";
    TP_TcpSendData(NULL, socketID, data.dataLength,  0, data);

    //5. DUT: Send data
    //6. TESTER: Check that DUT sent proper data
    TCP_Packet TCP_RP2 = ListenTCP(f);
    if (TCP_RP2.length != 0) 
    { 
        int rPSHFlag  = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagPSH);
        int   rAckFlag = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagACK);
        char* payloadData  = (char *)GetTCPField(&TCP_RP2, PAYLAOD, PAYLOAD_data);
        if ((rPSHFlag == 1)  && (rAckFlag==1) && (strcmp(payloadData,(char*)data.Data)==0))
        {
            //result ok
        } 
        else
        {
           
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest failed --> DUT response not ok\n");
            return 1;
        }
    }
    else 
    {
	    End_Test_Mode_2(socketID,  seqN,  ackN);

        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //7. TESTER: Check that DUT remains in CLOSE_WAIT state 
    if(checkState(CLOSEWAIT))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed -> DUT is not in CLOSE state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_CLOSING_13
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_CLOSING_08 test.
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
int TCP_CLOSING_13()
{
    
    // start test
    StartTest();
    
    //1. TESTER: Send a RST control message 
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //Receiving DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {
        //2. DUT: Do not send any response
    }
    else
    {
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //3. TESTER: Verify that the DUT moves on to CLOSED state
    if(checkState(CLOSED))
    {
        printf("\nTest passed\n");
        return 0;
    }
    else
    {
        printf("\nTest failed -> DUT is not in CLOSED state\n");
        return 1;
    }
}
