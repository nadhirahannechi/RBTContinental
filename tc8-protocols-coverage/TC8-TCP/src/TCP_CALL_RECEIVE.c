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
#include "TCP_CALL_RECEIVE.h"
#include "TestabilityProtocol_api.h"

/******************************************************************************
* DEFINES AND MACROS
******************************************************************************/


/******************************************************************************
* GLOBAL VARIABLES AND CONSTANTS
******************************************************************************/
TP_ResultID_t result_ReceiveAndForward_event = 1 ,result_ReceiveAndForward_cb = 1;
uint16 C,B;
vint8 Data;


/******************************************************************************
* LOCAL FUNCTION PROTOTYPES
******************************************************************************/
void TP_TcpReceiveAndForward_cb_Call_Receive(TP_ResultID_t a, uint16 b);
void TP_TcpReceiveAndForward_event_cb_Call_Receive(TP_ResultID_t Result_ReceiveAndForward_event, uint16 c , vint8 data);

/******************************************************************************
* Local FUNCTIONS
******************************************************************************/



void TP_TcpReceiveAndForward_cb_Call_Receive(TP_ResultID_t a, uint16 b)
{
    result_ReceiveAndForward_cb=a;
    B=b;
    
}

void TP_TcpReceiveAndForward_event_cb_Call_Receive(TP_ResultID_t Result_ReceiveAndForward_event, uint16 c , vint8 data)
{
   printf("\n ************************** %d ***************\n",data.Data);
   Data.Data= (uint8*)malloc(data.dataLength*sizeof(uint8));
   Data.dataLength=data.dataLength;
   memcpy(Data.Data, data.Data,Data.dataLength);
   result_ReceiveAndForward_event=Result_ReceiveAndForward_event;
}
/******************************************************************************
* GLOBAL FUNCTIONS
******************************************************************************/



/***************************************************************************************************
*
*   FUNCTION NAME: TCP_CALL_RECEIVE_04_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP CALL RECEIVE 04  test IT 1.
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
int TCP_CALL_RECEIVE_04_IT1()
{
    int socketID;

    //start test
    int StartTestRes = StartTest();
    if (StartTestRes == Result_OK)
    {
        printf("\n start test \n");
    }
    else 
    {
        printf("\n Test failed :cannot start test \n");
        return Result_NOK;
    }
	
    //1. TESTER: Cause the DUT to move on to ESTABLISHED state 
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
	
    char sszData[TCPConfig.ssz];
    char fullData[TCPConfig.nss * TCPConfig.ssz];
    memset(sszData, 1, TCPConfig.ssz);
    memset(fullData, 1, TCPConfig.nss * TCPConfig.ssz);


    //Length_cb = 0;
    //Data_cb.dataLength = TCPConfig.nss * TCPConfig.ssz;

    TP_TcpReceiveAndForward(TP_TcpReceiveAndForward_cb_Call_Receive, TP_TcpReceiveAndForward_event_cb_Call_Receive, socketID, 0xFFFF, 0xFFFF);
    
    
    // 2. TESTER: Send <nss> number of small sized <ssz> data segments
    for(int i = 0; i < TCPConfig.nss; i++)
    {
        TCP_Packet TCP_P = CreateTCP();
        EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
        EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
        EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
        EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
        EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
        EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
        EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)sszData);
        TCP_Compute_checksum(&TCP_P);
        SendTCP(TCP_P);
        //sleep(1);
        ackN=  ackN + TCPConfig.ssz;
    
    }

    // 3. TESTER: Cause the application on the DUT-sIDe to issue a RECEIVE
    // call having more than <nss>*<ssz> buffer size

	int i = 0;
	while((result_ReceiveAndForward_event == RID_E_NOK)&&(i<20))
    {
        sleep(1);
        i++;
    }
    // 4. DUT: Issues a RECEIVE call
    // 5. TESTER: Verify that the DUT returns the reassembled data to the receiving application 
    if  (result_ReceiveAndForward_event==0)
    {
	result_ReceiveAndForward_event=0;
        if (!strcmp(Data.Data,"TEST TCP"))
        {
        printf("\n Test passed\n");
        EndTest(socketID);
        return 0;
        }
        else 
        {
            printf("\nTest failed --> data received is not full data\n");
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 1;
        }
    }
    else 
    {
        printf("\nTest failed --> DUT does not issues a RECEIVE call\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }


    
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_CALL_RECEIVE_04_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP CALL RECEIVE 04  test IT 2.
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
int TCP_CALL_RECEIVE_04_IT2()
{
   // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to FINWAIT-1 state
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
	

     char sszData[TCPConfig.ssz];
    char fullData[TCPConfig.nss * TCPConfig.ssz];
    memset(sszData, 1, TCPConfig.ssz);
    memset(fullData, 1, TCPConfig.nss * TCPConfig.ssz);


    //Length_cb = 0;
    //Data_cb.dataLength = TCPConfig.nss * TCPConfig.ssz;

    TP_TcpReceiveAndForward(TP_TcpReceiveAndForward_cb_Call_Receive, TP_TcpReceiveAndForward_event_cb_Call_Receive, socketID, 0xFFFF, 0xFFFF);
    
    
    // 2. TESTER: Send <nss> number of small sized <ssz> data segments
    for(int i = 0; i < TCPConfig.nss; i++)
    {
        TCP_Packet TCP_P = CreateTCP();
        EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
        EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
        EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
        EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
        EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
        EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
        EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)sszData);
        TCP_Compute_checksum(&TCP_P);
        SendTCP(TCP_P);
        //sleep(1);
        ackN=  ackN + TCPConfig.ssz;
    
    }

    // 3. TESTER: Cause the application on the DUT-sIDe to issue a RECEIVE
    // call having more than <nss>*<ssz> buffer size



    // 4. DUT: Issues a RECEIVE call
    // 5. TESTER: Verify that the DUT returns the reassembled data to the receiving application 
	int i = 0;
	while((result_ReceiveAndForward_event == RID_E_NOK)&&(i<20))
    {
        sleep(1);
        i++;
    }
    if  (result_ReceiveAndForward_event==0)
    {
	result_ReceiveAndForward_event=0;
        if (!strcmp(Data.Data,"TEST TCP"))
        {
        printf("\n Test passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
        }
        else 
        {
            printf("\nTest failed --> data received is not full data\n");
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 1;
        }
    }
    else 
    {
        printf("\nTest failed --> DUT does not issues a RECEIVE call\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_CALL_RECEIVE_04_IT3
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP CALL RECEIVE 04  test IT 3.
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
int TCP_CALL_RECEIVE_04_IT3()
{
   // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to FINWAIT-2 state
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


    char sszData[TCPConfig.ssz];
    char fullData[TCPConfig.nss * TCPConfig.ssz];
    memset(sszData, 1, TCPConfig.ssz);
    memset(fullData, 1, TCPConfig.nss * TCPConfig.ssz);


    //Length_cb = 0;
    //Data_cb.dataLength = TCPConfig.nss * TCPConfig.ssz;

    TP_TcpReceiveAndForward(TP_TcpReceiveAndForward_cb_Call_Receive, TP_TcpReceiveAndForward_event_cb_Call_Receive, socketID, 0xFFFF, 0xFFFF);
    
    
    // 2. TESTER: Send <nss> number of small sized <ssz> data segments
    for(int i = 0; i < TCPConfig.nss; i++)
    {
        TCP_Packet TCP_P = CreateTCP();
        EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
        EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
        EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
        EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
        EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
        EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
        EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)sszData);
        TCP_Compute_checksum(&TCP_P);
        SendTCP(TCP_P);
        sleep(1);
        ackN=  ackN + TCPConfig.ssz;
    
    }

    // 3. TESTER: Cause the application on the DUT-sIDe to issue a RECEIVE
    // call having more than <nss>*<ssz> buffer size



    // 4. DUT: Issues a RECEIVE call
    // 5. TESTER: Verify that the DUT returns the reassembled data to the receiving application 
		int i = 0;
	while((result_ReceiveAndForward_event == RID_E_NOK)&&(i<20))
    {
        sleep(1);
        i++;
    }
    if  (result_ReceiveAndForward_event==0)
    {
	result_ReceiveAndForward_event=0;
        if (!strcmp(Data.Data,"TEST TCP"))
        {
        printf("\n Test passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
        }
        else 
        {
            printf("\nTest failed --> data received is not full data\n");
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 1;
        }
    }
    else 
    {
        printf("\nTest failed --> DUT does not issues a RECEIVE call\n");
       End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_CALL_RECEIVE_05
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP CALL RECEIVE 05  test.
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

int TCP_CALL_RECEIVE_05()
{
   int socketID;

    //start test
    int StartTestRes = StartTest();
    if (StartTestRes == Result_OK)
    {
        printf("\n start test \n");
    }
    else 
    {
        printf("\n Test failed :cannot start test \n");
        return Result_NOK;
    }
	
    //1. TESTER: Cause the DUT to move on to ESTABLISHED state 
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
    
    //TESTER: Cause the application on the DUT-sIDe to issue a RECEIVE call
    
    //2. TESTER: Send a data segment with FIN flag set
    TP_TcpReceiveAndForward(TP_TcpReceiveAndForward_cb_Call_Receive, TP_TcpReceiveAndForward_event_cb_Call_Receive, socketID, 0xFFFF, 0xFFFF);

    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    sleep(2);

   

    //3. TESTER: Cause the application on the DUT-sIDe to issue a RECEIVE call
    //4. DUT: Issues a RECEIVE call
    //5. TESTER: Verify that the DUT returns the data to the receiving application
		int i = 0;
	while((result_ReceiveAndForward_event == RID_E_NOK)&&(i<20))
    {
        sleep(1);
        i++;
    }
    if  (result_ReceiveAndForward_event==0)
    {	
	result_ReceiveAndForward_event=0;
        if (!strcmp(Data.Data,"TEST TCP"))
        {
        printf("\n Test passed\n");
        EndTest(socketID);
        return 0;
        }
        else 
        {
            printf("\nTest failed --> data received is not data\n");
            EndTest(socketID);
            return 1;
        }
    }
    else 
    {
        printf("\nTest failed --> DUT does not issues a RECEIVE call\n");
        EndTest(socketID);
        return 1;
    }

     
}
