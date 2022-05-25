/**
 *  Copyright (C) [year] Focus Corporation
 *
 *  \file     TCP_MSS_OPTIONS.c
 *  \brief    This file contains the tests of TCP MSS OPTIONS
 *  \author   Aymen SALEM && Omar NSIRI
 *  \platform [target platform]
 *
 *  \addtogroup SW_package
 *  \{
 */

/******************************************************************************
* INCLUDE FILES
******************************************************************************/
#include "TCP_MSS_OPTIONS.h"
#include "AbstractionAPI.h"
#include "TCPcommon.h"
#include "TestabilityProtocol_api.h"
/******************************************************************************
* DEFINES AND MACROS
******************************************************************************/

/******************************************************************************
* GLOBAL VARIABLES AND CONSTANTS
******************************************************************************/
TP_ResultID_t result_cb_mss_option;
uint16 Length_cb_mms_option;
vint8 Data_cb_mms_option;
int socketID_CB = 0;

/******************************************************************************
* LOCAL FUNCTION PROTOTYPES
******************************************************************************/
void TP_TcpReceiveAndForward_event_cb_mss_option(TP_ResultID_t result, uint16 Length , vint8 Data);
void CreateAndBind_callback_mss_options(TP_ResultID_t b, uint16 socket);



/******************************************************************************
* Local FUNCTIONS
******************************************************************************/

void TP_TcpReceiveAndForward_event_cb_mss_option(TP_ResultID_t result, uint16 Length , vint8 Data)
{
    result_cb_mss_option = result;
    Length_cb_mms_option = Length;
    Data_cb_mms_option.dataLength = Data.dataLength;
    Data_cb_mms_option.Data = (uint8*) calloc(Data.dataLength,sizeof(uint8)); 
    memcpy(Data_cb_mms_option.Data,Data.Data,Data.dataLength);
}


void CreateAndBind_callback_mss_options(TP_ResultID_t b, uint16 socket)
{
    socketID_CB = socket;
}



/******************************************************************************
* GLOBAL FUNCTIONS
******************************************************************************/

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_MSS_OPTIONS_01_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_MSS_OPTIONS_01 test iteration 1.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* 1. CASE: ilen = 0 (less than actual) 
*
* @warning
* -
*
***************************************************************************************************/
int TCP_MSS_OPTIONS_01_IT1()
{
    //TP_OpenControlChannel();
    StartTest();

    // 1. TESTER: Cause DUT to move on to LISTEN state at a <wnp>
	int socketID,socketTester;
    uint32 seqN, ackN;
	int moveDUTToListenResult = moveDUTToListen(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon);
    if (moveDUTToListenResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }    
	
	//2. TESTER: Send a SYN with MSS option having illegal length, ilen
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    //prepare mss option
    TCP_MSS_OPTION mss;
    mss.length=0;
    mss.value=htons(536);
    //prepare the other options
    uint8_t options[4];
    options[0]=TCP_NO_OPERATION_OPTION ;
    options[1]=TCP_NO_OPERATION_OPTION ;
    options[2]=TCP_NO_OPERATION_OPTION ;
    options[3]=TCP_END_OF_OPTION ;


    TCP_Add_Option(&TCP_P,(uint8_t*)&mss,4);
    TCP_Add_Option(&TCP_P,(uint8_t*)options,4);
    //compute checksum
    TCP_Compute_checksum(&TCP_P);

    TP_TcpReceiveAndForward(NULL,TP_TcpReceiveAndForward_event_cb_mss_option,socketTester,5,1500);
    SendTCP(TCP_P);
	int i=0 ;
    while ((result_cb_mss_option==RID_E_NOK) && (i<20))
    {
        sleep(1);
        i++;
    }
	
    //3. TESTER: Verify that the DUT has not crashed. 
    if(result_cb_mss_option== RID_E_OK)
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
		result_cb_mss_option= RID_E_NOK;
        printf("\nIT01 passed --> DUT was not crashed\n");
        return 0;
    }
    else 
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> DUT was crashed\n");
        return 1; 
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_MSS_OPTIONS_01_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_MSS_OPTIONS_01 test iteration 2.
*
* @return int : 0: The state progression was successful | 1: The state progression didn't go as expected.
*
* @note
* -2. CASE: ilen = 5 (more than actual) 
*
* @warning
* -
*
***************************************************************************************************/
int TCP_MSS_OPTIONS_01_IT2()
{
    //TP_OpenControlChannel();
    StartTest();

    // 1. TESTER: Cause DUT to move on to LISTEN state at a <wnp>
	int socketID,socketTester;
    uint32 seqN, ackN;
	int moveDUTToListenResult = moveDUTToListen(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon);
    if (moveDUTToListenResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }    
	//2. TESTER: Send a SYN with MSS option having illegal length, ilen
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    //prepare mss option
    TCP_MSS_OPTION mss;
    mss.length=5;
    mss.value=htons(536);
    //prepare the other options
    uint8_t options[4];
    options[0]=TCP_NO_OPERATION_OPTION ;
    options[1]=TCP_NO_OPERATION_OPTION ;
    options[2]=TCP_NO_OPERATION_OPTION ;
    options[3]=TCP_END_OF_OPTION ;


    TCP_Add_Option(&TCP_P,(uint8_t*)&mss,4);
    TCP_Add_Option(&TCP_P,(uint8_t*)options,4);
    //compute checksum
    TCP_Compute_checksum(&TCP_P);

    TP_TcpReceiveAndForward(NULL,TP_TcpReceiveAndForward_event_cb_mss_option,socketTester,5,1500);
    SendTCP(TCP_P);
	
	//wait for cb functions
	int i=0 ;
    while ((result_cb_mss_option==RID_E_NOK) && (i<20))
    {
        sleep(1);
        i++;
    }
	
    //3. TESTER: Verify that the DUT has not crashed. 
    if(result_cb_mss_option== RID_E_OK)
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
		result_cb_mss_option= RID_E_NOK;
        printf("\nIT01 passed --> DUT was not crashed\n");
        return 0;
    }
    else 
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> DUT was crashed\n");
        return 1; 
    }
	
}
/***************************************************************************************************
*
*   FUNCTION NAME: TCP_MSS_OPTIONS_02
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_MSS_OPTIONS_02 test.
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
int TCP_MSS_OPTIONS_02()
{
   	//TP_OpenControlChannel();
    StartTest();

    // 1. TESTER: Cause DUT to move on to LISTEN state at a <wnp>
    int socketID,socketTester;
    uint32 seqN, ackN;
	int moveDUTToListenResult = moveDUTToListen(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon);
    if (moveDUTToListenResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }  
	
	//2. TESTER: Send a SYN with three No Operation options followed by an End of Options list option
	TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
	
    //prepare  options
    uint8_t options[4];
    options[0]=TCP_NO_OPERATION_OPTION ;
    options[1]=TCP_NO_OPERATION_OPTION ;
    options[2]=TCP_NO_OPERATION_OPTION ;
    options[3]=TCP_END_OF_OPTION ;


    TCP_Add_Option(&TCP_P,(uint8_t*)options,4);
    //compute checksum
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    //3. DUT: Send SYN,ACK
	Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    int R_SEQN;
    int R_ACKN;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) 
    { 
        int R_FlagSYN  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        int R_FlagACK  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
		R_SEQN  = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        R_ACKN  = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        if ((R_FlagSYN == 1) && (R_FlagACK == 1))
        {
            // result ok 
        } 
        else
        {
            printf ("\nTest failed\n");
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 1;

        }
    }
    else 
    {
		End_Test_Mode_2(socketID,  seqN,  ackN);
		printf("\nTest failed --> DUT not responding\n");
		return 1;
    }
    //4. TESTER: Send ACK
	TCP_Packet TCP_P2 = CreateTCP();
    EditTCPField(&TCP_P2, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P2, TCP, TCP_SeqNumber, (void *)(intptr_t)R_ACKN);
    EditTCPField(&TCP_P2, TCP, TCP_AckNumber, (void *)(intptr_t)(R_SEQN+1));
    EditTCPField(&TCP_P2, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P2, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
	TCP_Compute_checksum(&TCP_P2);
    SendTCP(TCP_P2);
	
    //5. TESTER: Verify that the DUT is in ESTABLISHED state
	if(checkState(ESTABLISHED))
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
*   FUNCTION NAME: TCP_MSS_OPTIONS_03
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_MSS_OPTIONS_03 test.
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
int TCP_MSS_OPTIONS_03()
{
	//TP_OpenControlChannel();
    StartTest();

    // 1. TESTER: Cause DUT to move on to LISTEN state at a <wnp>
	int socketID,socketTester;
    uint32 seqN, ackN;
	int moveDUTToListenResult = moveDUTToListen(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon);
    if (moveDUTToListenResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }    
    
	//2. TESTER: Send a SYN with three No Operation options followed by an End of Options list option
	TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    //compute checksum
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    //3. DUT: Send SYN,ACK
	Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    int R_SEQN;
    int R_ACKN;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) 
    { 
        int R_FlagSYN  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        int R_FlagACK  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
		R_SEQN  = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        R_ACKN  = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        if ((R_FlagSYN == 1) && (R_FlagACK == 1))
        {
            // result ok 
        } 
        else
        {
            printf ("\nTest failed\n");
            End_Test_Mode_2(socketID,  seqN,  ackN);

            return 1;

        }
    }
    else 
    {
		End_Test_Mode_2(socketID,  seqN,  ackN);
		printf("\nTest failed --> DUT not responding\n");
		return 1;
    }
    //4. TESTER: Send ACK
	TCP_Packet TCP_P2 = CreateTCP();
    EditTCPField(&TCP_P2, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P2, TCP, TCP_SeqNumber, (void *)(intptr_t)R_ACKN);
    EditTCPField(&TCP_P2, TCP, TCP_AckNumber, (void *)(intptr_t)(R_SEQN+1));
    EditTCPField(&TCP_P2, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P2, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
	TCP_Compute_checksum(&TCP_P2);
    SendTCP(TCP_P2);
	
    //5. TESTER: Verify that the DUT is in ESTABLISHED state
	if(checkState(ESTABLISHED))
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
    }}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_MSS_OPTIONS_05_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_MSS_OPTIONS_05 test IT 1.
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
int TCP_MSS_OPTIONS_05_IT1()
{
	//TP_OpenControlChannel();
    StartTest();


    // 1. TESTER: Cause the DUT to move on to SYN-SENT state
    int socketID;
    uint32 seqN = 0, ackN = 0;
    int moveDUTToSYNSENTResult = moveDUTToSYNSENT(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
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
	
	TCP_MSS_OPTION mss;
    mss.length=0;
    mss.value=htons(536);
    //prepare the other options
    uint8_t options[4];
    options[0]=TCP_NO_OPERATION_OPTION ;
    options[1]=TCP_NO_OPERATION_OPTION ;
    options[2]=TCP_NO_OPERATION_OPTION ;
    options[3]=TCP_END_OF_OPTION ;


    TCP_Add_Option(&TCP_P,(uint8_t*)&mss,4);
    TCP_Add_Option(&TCP_P,(uint8_t*)options,4);
    //compute checksum
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
        printf("\nTest passed \n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
	else
	{
        // No response sent
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    }

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_MSS_OPTIONS_05_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_MSS_OPTIONS_05 test IT 2.
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
int TCP_MSS_OPTIONS_05_IT2()
{
	//TP_OpenControlChannel();
    StartTest();


    // 1. TESTER: Cause the DUT to move on to SYN-SENT state
    int socketID;
    uint32 seqN = 0, ackN = 0;
    int moveDUTToSYNSENTResult = moveDUTToSYNSENT(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
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
	
	TCP_MSS_OPTION mss;
    mss.length=5;
    mss.value=htons(536);
    //prepare the other options
    uint8_t options[4];
    options[0]=TCP_NO_OPERATION_OPTION ;
    options[1]=TCP_NO_OPERATION_OPTION ;
    options[2]=TCP_NO_OPERATION_OPTION ;
    options[3]=TCP_END_OF_OPTION ;


    TCP_Add_Option(&TCP_P,(uint8_t*)&mss,4);
    TCP_Add_Option(&TCP_P,(uint8_t*)options,4);
    //compute checksum
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
        printf("\nTest passed \n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
	else
	{
        // No response sent
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    }

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_MSS_OPTIONS_06_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_MSS_OPTIONS_06 test IT 1.
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
int TCP_MSS_OPTIONS_06_IT1()
{
    //TP_OpenControlChannel();
    StartTest();

    // 1. TESTER: Cause DUT to move on to LISTEN state at a <wnp>
    int socketID,socketTester;
    uint32 seqN, ackN;
	int moveDUTToListenResult = moveDUTToListen(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon);
    if (moveDUTToListenResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }
    
    //2. TESTER: Send a SYN with MSS option having a value Mv
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    //prepare mss option
    TCP_MSS_OPTION mss;
    mss.length=4;
    mss.value=htons((TCPConfig.mss)-1);
    //prepare the other options
    uint8_t options[4];
    options[0]=TCP_NO_OPERATION_OPTION ;
    options[1]=TCP_NO_OPERATION_OPTION ;
    options[2]=TCP_NO_OPERATION_OPTION ;
    options[3]=TCP_END_OF_OPTION ;


    TCP_Add_Option(&TCP_P,(uint8_t*)&mss,4);
    TCP_Add_Option(&TCP_P,(uint8_t*)options,4);
    //compute checksum
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);


    //3. DUT: Send SYN,ACK
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
        ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        if ((rSYN == 1) && (rACK == 1))
        {
            //result ok
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not SYN,ACK\n");
            EndTest(socketID);
            return 1;
        }
    }
    else
    {
        // No response sent
        printf("\nTest failed --> DUT not responding\n");
        EndTest(socketID);
        return 1;
    }


    //4. TESTER: Send ACK
    TCP_Packet TCP_P_2 = CreateTCP();
    EditTCPField(&TCP_P_2, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P_2, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P_2, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P_2, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P_2, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P_2);
    SendTCP(TCP_P_2);
    
    //5. TESTER: Cause the application to issue a SEND request for data with size at least max (MSS)
    vint8 data;
    data.Data =(uint8*) calloc(TCPConfig.mss,sizeof(uint8));
    memset(data.Data,1,TCPConfig.mss);
    data.dataLength=TCPConfig.mss;
    TP_TcpSendData(NULL, socketTester, data.dataLength,  0, data);

    //6. DUT: Send data segment(s)
    //7. TESTER: Verify that the first received segment has size that is min(MSS) 
    TCP_Packet TCP_RP_2 = ListenTCP(f);
    if (TCP_RP_2.length != 0) 
	{
        if(TCP_RP_2.length<=((TCPConfig.mss)-1))
        {

            ackN = (int)GetTCPField(&TCP_RP_2, TCP, TCP_AckNumber);
            seqN = (int)GetTCPField(&TCP_RP_2, TCP, TCP_SeqNumber);
            printf("\nTest passed \n");
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 0;
        }
        else 
        {
            printf("\nTest failed --> SEG LEN > MSS\n");
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



}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_MSS_OPTIONS_06_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_MSS_OPTIONS_06 test IT 2.
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
int TCP_MSS_OPTIONS_06_IT2()
{
    //TP_OpenControlChannel();
    StartTest();

    // 1. TESTER: Cause DUT to move on to LISTEN state at a <wnp>
	int socketID,socketTester;
    uint32 seqN, ackN;
	int moveDUTToListenResult = moveDUTToListen(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon);
    if (moveDUTToListenResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }    
    //2. TESTER: Send a SYN with MSS option having a value Mv
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    //prepare mss option
    TCP_MSS_OPTION mss;
    mss.length=4;
    mss.value=htons((TCPConfig.mss)+11);
    //prepare the other options
    uint8_t options[4];
    options[0]=TCP_NO_OPERATION_OPTION ;
    options[1]=TCP_NO_OPERATION_OPTION ;
    options[2]=TCP_NO_OPERATION_OPTION ;
    options[3]=TCP_END_OF_OPTION ;


    TCP_Add_Option(&TCP_P,(uint8_t*)&mss,4);
    TCP_Add_Option(&TCP_P,(uint8_t*)options,4);
    //compute checksum
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);


    //3. DUT: Send SYN,ACK
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
        ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        if ((rSYN == 1) && (rACK == 1))
        {
            //result ok
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not SYN,ACK\n");
            EndTest(socketID);
            return 1;
        }
    }
    else
    {
        // No response sent
        printf("\nTest failed --> DUT not responding\n");
        EndTest(socketID);
        return 1;
    }


    //4. TESTER: Send ACK
    TCP_Packet TCP_P_2 = CreateTCP();
    EditTCPField(&TCP_P_2, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P_2, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P_2, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P_2, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P_2, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    TCP_Compute_checksum(&TCP_P_2);
    SendTCP(TCP_P_2);
    
    //5. TESTER: Cause the application to issue a SEND request for data with size at least max (MSS)
    vint8 data;
    data.Data =(uint8*) calloc(TCPConfig.mss+1,sizeof(uint8));
    memset(data.Data,1,TCPConfig.mss+1);
    data.dataLength=TCPConfig.mss;
    TP_TcpSendData(NULL, socketTester, data.dataLength,  0, data);

    //6. DUT: Send data segment(s)
    //7. TESTER: Verify that the first received segment has size that is min(MSS) 
    TCP_Packet TCP_RP_2 = ListenTCP(f);
    if (TCP_RP_2.length != 0) 
	{
        if(TCP_RP_2.length<=((TCPConfig.mss)+1))
        {
            ackN = (int)GetTCPField(&TCP_RP_2, TCP, TCP_AckNumber);
            seqN = (int)GetTCPField(&TCP_RP_2, TCP, TCP_SeqNumber);
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest passed \n");

            return 0;
        }
        else 
        {
            ackN = (int)GetTCPField(&TCP_RP_2, TCP, TCP_AckNumber);
            seqN = (int)GetTCPField(&TCP_RP_2, TCP, TCP_SeqNumber);
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest failed --> SEG LEN > MSS\n");
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 1;
        }
    }
	else
	{
        // No response sent
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_MSS_OPTIONS_09_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_MSS_OPTIONS_09 test IT 1.
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
int TCP_MSS_OPTIONS_09_IT1()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause the application on the DUT-side to issue an OPEN call
    //2. DUT: Send a SYN
    int socketID;
    uint32 seqN = 0, ackN = 0;
    int moveDUTToSYNSENTResult = moveDUTToSYNSENT(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToSYNSENTResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }
	
    //3. TESTER: Send SYN,ACK with MSS option having a value Mv
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)seqN+1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    //prepare mss option
    TCP_MSS_OPTION mss;
    mss.length=4;
    mss.value=htons((TCPConfig.mss)-21);
    //prepare the other options
    uint8_t options[4];
    options[0]=TCP_NO_OPERATION_OPTION ;
    options[1]=TCP_NO_OPERATION_OPTION ;
    options[2]=TCP_NO_OPERATION_OPTION ;
    options[3]=TCP_END_OF_OPTION ;


    TCP_Add_Option(&TCP_P,(uint8_t*)&mss,4);
    TCP_Add_Option(&TCP_P,(uint8_t*)options,4);
    //compute checksum
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);


    //4. DUT: Send ACK
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) 
    {
        // Verify that DUT response is SYN,ACK
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if  (rACK == 1)
        {
            //result ok
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not SYN,ACK\n");
            EndTest(socketID);
            return 1;
        }
    }
    else
    {
        // No response sent
        printf("\nTest failed --> DUT not responding\n");
        EndTest(socketID);
        return 1;
    }

    
    //TESTER: Cause the application to issue a send request for data withsize at least max(transport_layer_max_send_msg_size-20, MSS)
    vint8 data;
    data.Data =(uint8*) calloc(TCPConfig.mss-22,sizeof(uint8));
    memset(data.Data,1,TCPConfig.mss-22);
    data.dataLength=TCPConfig.mss-22;
    TP_TcpSendData(NULL, socketID, data.dataLength,  0, data);

    //6. DUT: Send data segment(s)
    //7. TESTER: Verify that the first received segment has size that is min(transport_layer_max_send_msg_size-20, MSS)
    TCP_Packet TCP_RP_2 = ListenTCP(f);
    if (TCP_RP_2.length != 0) 
	{
        int rPSHFlag  = (int)GetTCPField(&TCP_RP_2, TCP, TCP_FlagPSH);
        int  rAckFlag = (int)GetTCPField(&TCP_RP_2, TCP, TCP_FlagACK);
        if ((rPSHFlag==1) && (rAckFlag==1) && (TCP_RP_2.length<=((TCPConfig.mss)-20)))
        {
            printf("\nTest passed \n");
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 0;
        }
        else 
        {
            printf("\nTest failed --> SEG LEN > MSS\n");
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

}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_MSS_OPTIONS_09_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_MSS_OPTIONS_09 test IT 2.
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
int TCP_MSS_OPTIONS_09_IT2()
{
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause the application on the DUT-side to issue an OPEN call
    //2. DUT: Send a SYN
    int socketID;
    uint32 seqN = 0, ackN = 0;
    int moveDUTToSYNSENTResult = moveDUTToSYNSENT(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToSYNSENTResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    //3. TESTER: Send SYN,ACK with MSS option having a value Mv
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)seqN+1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    //prepare mss option
    TCP_MSS_OPTION mss;
    mss.length=4;
    mss.value=htons((TCPConfig.mss)-21);
    //prepare the other options
    uint8_t options[4];
    options[0]=TCP_NO_OPERATION_OPTION ;
    options[1]=TCP_NO_OPERATION_OPTION ;
    options[2]=TCP_NO_OPERATION_OPTION ;
    options[3]=TCP_END_OF_OPTION ;


    TCP_Add_Option(&TCP_P,(uint8_t*)&mss,4);
    TCP_Add_Option(&TCP_P,(uint8_t*)options,4);
    //compute checksum
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);


    //4. DUT: Send ACK
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) 
    {
        // Verify that DUT response is SYN,ACK
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if  (rACK == 1)
        {
            //result ok
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not SYN,ACK\n");
            EndTest(socketID);
            return 1;
        }
    }
    else
    {
        // No response sent
        printf("\nTest failed --> DUT not responding\n");
        EndTest(socketID);
        return 1;
    }

    
    //TESTER: Cause the application to issue a send request for data withsize at least max(transport_layer_max_send_msg_size-20, MSS)
    vint8 data;
    data.Data =(uint8*) calloc(TCPConfig.mss-19,sizeof(uint8));
    memset(data.Data,0,TCPConfig.mss-19);
    data.dataLength=TCPConfig.mss-19;
    TP_TcpSendData(NULL, socketID, data.dataLength,  0, data);

    //6. DUT: Send data segment(s)
    //7. TESTER: Verify that the first received segment has size that is min(transport_layer_max_send_msg_size-20, MSS)
    TCP_Packet TCP_RP_2 = ListenTCP(f);
    if (TCP_RP_2.length != 0) 
	{
        int rPSHFlag  = (int)GetTCPField(&TCP_RP_2, TCP, TCP_FlagPSH);
        int  rAckFlag = (int)GetTCPField(&TCP_RP_2, TCP, TCP_FlagACK);
        if ((rPSHFlag==1) && (rAckFlag==1) && (TCP_RP_2.length<=((TCPConfig.mss)-20)))
        {
            printf("\nTest passed \n");
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 0;
        }
        else 
        {
            printf("\nTest failed --> SEG LEN > MSS\n");
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
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_MSS_OPTIONS_10
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_MSS_OPTIONS_10 test.
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
int TCP_MSS_OPTIONS_10()
{
	//TP_OpenControlChannel();
    StartTest();
    //1. TESTER: Send a SYN without MSS option
    //2. DUT: Send SYN,ACK
    //3. TESTER: Send ACK
	int socketID, socketTester;
    uint32 seqN, ackN;
    int moveServDUTToEstablishedResult = moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveServDUTToEstablishedResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

	//4. TESTER: Cause the application on the DUT-side to issue a send request for data of size more than 536 bytes (MSS)

	vint8 data;
    data.Data =(uint8*) calloc(TCPConfig.mss+5,sizeof(uint8));
    memset(data.Data,0x00,TCPConfig.mss+5);
    data.dataLength=TCPConfig.mss+5;
    TP_TcpSendData(NULL, socketTester, TCPConfig.mss+5,  0, data);
	
    
    // 6. TESTER: Verify that the first received data segment has 536 bytes   
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == TCPConfig.mss) 
	{
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else if (TCP_RP.length == 0) 
	{
        // No response sent
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
    else
    {
        // No response sent
        printf("\nTest failed -->  the first received data segment has not MSS bytes\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_MSS_OPTIONS_11
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_MSS_OPTIONS_11 test.
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
int TCP_MSS_OPTIONS_11()
{
    socketID_CB=0;
    uint32 seqN,ackN;
    //TP_OpenControlChannel();
    StartTest();
  

    //1. TESTER: Cause an application on the DUT-side to issue an OPEN call 
    ip4addr ipv4;
    ipv4.dataLength = 4;
    stoiIP(TCPConfig.DUT_IP, ipv4.Data);
    // Cause DUT to issue an active open call
    TP_TcpCreateAndBind(CreateAndBind_callback_mss_options, TRUE, TCPConfig.DUT_Port, ipv4);
    sleep(3);
    stoiIP(TCPConfig.TESTER_IP, ipv4.Data);
    TP_TcpConnect(NULL, socketID_CB, TCPConfig.Tester_Port, ipv4);


    //2. DUT: Send a SYN
    //3. TESTER: Verify that received SYN contains MSS option 
    // Receive SYN
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;

    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    TCP_MSS_OPTION R_TCP_Option;
    memset(&R_TCP_Option,0,sizeof(R_TCP_Option));
    if (TCP_RP.length != 0) 
    {
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        if (rSYN == 1)
        {
            seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            if(Get_TCP_Option_Value(TCP_RP, 2,(uint8_t*)&(R_TCP_Option.value))==0)
            {
                
                printf("\nTest passed\n");
                End_Test_Mode_2(socketID_CB,  seqN,  ackN);
                return 0;
            }
            else 
            {
                printf("\nTest failed --> SYN does not contains MSS option \n");
                End_Test_Mode_2(socketID_CB,  seqN,  ackN);
                return 1;  
            }
            


        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not SYN,ACK\n");
            End_Test_Mode_2(socketID_CB,  0,  0);
            return 1;
        }
     
    }

}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_MSS_OPTIONS_12
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_MSS_OPTIONS_12 test.
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
int TCP_MSS_OPTIONS_12()
{
    socketID_CB=0;
    uint32 seqN,ackN;
    //TP_OpenControlChannel();
    StartTest();
  

    //1. TESTER: Cause an application on the DUT-side to issue an OPEN call 
    ip4addr ipv4;
    ipv4.dataLength = 4;
    stoiIP(TCPConfig.DUT_IP, ipv4.Data);
    // Cause DUT to issue an active open call
    TP_TcpCreateAndBind(CreateAndBind_callback_mss_options, TRUE, TCPConfig.DUT_Port, ipv4);
    sleep(3);
    stoiIP(TCPConfig.TESTER_IP, ipv4.Data);
    TP_TcpConnect(NULL, socketID_CB, TCPConfig.Tester_Port, ipv4);


    //2. DUT: Send a SYN
    //3. 2. DUT: Send a SYN with an MSS different from the default 536
    // Receive SYN
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;

    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    TCP_MSS_OPTION R_TCP_Option;
    memset(&R_TCP_Option,0,sizeof(R_TCP_Option));
    if (TCP_RP.length != 0) 
    {
        seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        if (rSYN == 1)
        {
            if(Get_TCP_Option_Value(TCP_RP, 2,(uint8_t*)&(R_TCP_Option.value))==0)
            {
                if(R_TCP_Option.value==htons(536))
                {
                    printf("\nTest failed --> MSS value = 536 \n");
                    End_Test_Mode_2(socketID_CB,  seqN,  ackN);
                    return 1; 
                }
                else
                {
                    printf("\nTest passed\n");
                    End_Test_Mode_2(socketID_CB,  seqN,  ackN);
                    return 0;
                }
            }
            else 
            {
                printf("\nTest failed --> SYN does not contains MSS option \n");
                End_Test_Mode_2(socketID_CB,  seqN,  ackN);
                return 1;  
            }
            


        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not SYN,ACK\n");
            End_Test_Mode_2(socketID_CB,  0,  0);
            return 1;
        }
     
    }

}
