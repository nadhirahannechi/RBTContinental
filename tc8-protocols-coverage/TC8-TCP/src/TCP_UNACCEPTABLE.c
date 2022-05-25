/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file     TCP_UNACCEPTABLE.c
 *  \brief    his file contains the tests of TCP UNACCEPTABLE
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
#include "TCP_UNACCEPTABLE.h"
#include "TestabilityProtocol_api.h"
#include "TestabilityProtocol_Intern.h"

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

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_UNACCEPTABLE_01
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_01.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* 
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_01() 
{
    int socketID,socketTester;
    uint32 seqN, ackN;
	
	// Start Test
    StartTest();
	
	//1. TESTER: Cause the DUT to move to SYN-RCVD state at a <wnp>initiated by passive  OPEN call

	
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

	//2. TESTER: Send RST segment (as if SYN,ACK just received was unexpected)
    // Send RST segment
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN+1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

	//3. DUT: Do not send response

    // Receive DUT response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {
        // misra
    }
    else
    {
        printf("\nTest failed --> DUT responded to RST segment\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
	
	// Check if DUT is in LISTEN state
    if(checkState(LISTEN))
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
*   FUNCTION NAME: TCP_UNACCEPTABLE_02
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_02.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_02() 
{
   int socketID,socketTester;
    uint32 seqN, ackN;
	
	// Start Test
    StartTest();
	
	//1. TESTER: Cause the DUT to move to SYN-RCVD state at a <wnp>initiated by passive  OPEN call
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

	//2. TESTER:  TESTER: Send a RST segment (as if SYN,ACK just received was unexpected) with a SEQ number outside the receive window of the DUT
    // Send RST segment
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0xff);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN+1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

	//3. DUT: Do not send response

    // Receive DUT response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    {
        // misra
    }
    else
    {
        printf("\nTest failed --> DUT responded to RST segment\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
	
	// Check if DUT is in SYNRCVD state
    if(checkState(SYNRCVD))
    {
		printf("\nTest passed\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
	}
    else
    {
        printf("\nTest failed --> DUT is not in SYNRCVD state\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_UNACCEPTABLE_03
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_03.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* 
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_03() 
{
    
	
	
	int socketID,socketTester;
    uint32 seqN, ackN;
	
	// Start Test
    StartTest();
	
	//1. TESTER: Cause the DUT to move to SYN-RCVD state at a <wnp>initiated by passive  OPEN call
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

    // Send segment with unacceptable ACK number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)seqN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(0xFFF));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // Receive DUT response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    // Check that DUT response is a RST segment
    if (TCP_RP.length != 0) 
    {
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if (rRST == 1)
        {
            printf("\nTest passed\n");
			End_Test_Mode_2(socketID,  seqN,  ackN);
            return 0;
        }
        else{
        printf("\nTest failed --> DUT response is not RST\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
        }
    } 
    else 
    {
        printf("\nTest failed --> DUT is not responding\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

  
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_UNACCEPTABLE_04_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_04 test iteration 1.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* - CASE 1: Data segment with out of window SEQ number
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_04_IT1()
{

	int socketID,socketTester;
    uint32 seqN, ackN;
    // Start Test
    StartTest();

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
	
    //2. TESTER: Send a data segment with out of window SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN + 50));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with current send SEQ number and ACK number indicating next SEQ number expected
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    // Check that DUT response is an ACK segment
    if (TCP_RP.length != 0) 
    {
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            int rSEQN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            int rACKN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);

            
            if ((rSEQN == ackN) && (rACKN == seqN))
            {
                //TEST OK
            }
            else
            {
                printf("\nTest failed --> DUT response is not ok\n");
                End_Test_Mode_2(socketID,  seqN,  ackN);
                return 1;
            }
        }
        else{
            printf("\nTest failed --> DUT response is not ACK\n");
			End_Test_Mode_2(socketID,  seqN,  ackN);
            return 1;
        }
    } 
    else 
    {
        printf("\nTest failed --> DUT is not responding\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in the ESTABLISHED state 
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
*   FUNCTION NAME: TCP_UNACCEPTABLE_04_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_04 test iteration 2.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  CASE 2: Data segment with an unacceptable ACK number
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_04_IT2()
{

	int socketID,socketTester;
    uint32 seqN, ackN;
    // Start Test
    StartTest();

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
	
    //2. TESTER: Send a data segment with out of window SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN ));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN + 50 ));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with current send SEQ number and ACK number indicating next SEQ number expected
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    // Check that DUT response is an ACK segment
    if (TCP_RP.length != 0) 
    {
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            int rSEQN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            int rACKN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            if ((rSEQN == ackN+1) && (rACKN == seqN))
            {
                //TEST OK
            }
            else
            {
               printf("\nTest failed --> DUT response is not ok\n");
                End_Test_Mode_2(socketID,  seqN,  ackN);
                return 1; 
            }
        }
        else{
            printf("\nTest failed --> DUT response is not ACK\n");
			End_Test_Mode_2(socketID,  seqN,  ackN);
            return 1;
        }
    } 
    else 
    {
        printf("\nTest failed --> DUT is not responding\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in the ESTABLISHED state 
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
*   FUNCTION NAME: TCP_UNACCEPTABLE_05_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_05_IT1.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  CASE 2: Data segment with an unacceptable ACK number
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_05_IT1() 
{
    int socketID,socketTester;
    uint32 seqN, ackN;
	
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

	//2. TESTER: Send a segment with a flag set and with unacceptable ACK number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(0x20));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

	//3. DUT: Send a RST segment
	Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
	if (TCP_RP.length != 0) 
	{
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if (rRST == 1) 
		{
            //misra
        }
        else
        {
            printf("\nTest failed --> DUT response is not ok\n");
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
	
	
	// Check if DUT is in LISTEN state
    if(checkState(LISTEN))
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
*   FUNCTION NAME: TCP_UNACCEPTABLE_14_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_14 test iteration 2.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  CASE 2: Data segment with an unacceptable ACK number
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_05_IT2() 
{
    int socketID,socketTester;
    uint32 seqN, ackN;
	
	// Start Test
    StartTest();
	
	//1. TESTER: Cause the DUT to move to listen state at a <wnp>initiated by passive  OPEN call
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

	//2. TESTER: Send a segment with a flag set and with unacceptable ACK number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0x20);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN+1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

	//3. DUT: Send a RST segment
	Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
	if (TCP_RP.length != 0) 
	{
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if (rRST == 1) 
		{
            //misra
        }
        else
        {
            printf("\nTest failed --> DUT response is not ok\n");
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
	
	
	// Check if DUT is in LISTEN state
    if(checkState(LISTEN))
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
*   FUNCTION NAME: TCP_UNACCEPTABLE_06
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_06.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* 
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_06() 
{
    int socketID,socketTester;
    uint32 seqN, ackN;
    // Start Test
    StartTest();

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
	
	//2. TESTER: Send a SYN with a sequence number outside the window
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0x50);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
	
	
	//3. DUT: Send an ACK indicating next expected SEQ number
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
            int rACKNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            if(rACKNumber == seqN) 
			{
                printf("\nTest passed\n");
				End_Test_Mode_2(socketID,  seqN,  ackN);
                return 0;
            } else 
			{
                printf("\nTest failed --> DUT responded with an unexpected ACK number\n");
				End_Test_Mode_2(socketID,  seqN,  ackN);
                return 1;
            }
        }
    } else 
	{
        printf("\nTest failed --> DUT not responding\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_UNACCEPTABLE_07
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_07.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* 
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_07() 
{
	int socketID,socketTester;
    uint32 seqN, ackN;
	
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

    // Send SYN,ACK segment
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //Receive DUT response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    //Verify that DUT response is a RST
    if (TCP_RP.length != 0) {
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if (rRST == 1) {
            printf("test passed");
			End_Test_Mode_2(socketID,  seqN,  ackN);
            return 0;
        }
    } else {
        printf("\nTest failed --> DUT not responding\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_UNACCEPTABLE_08_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE 08 test iteration 2.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  1. CASE: flag set = SYN,ACK 
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_08_IT1()
{
    int socketID;
    uint32 seqN, ackN;

    // Start Test
    StartTest();

    // 1. TESTER: Cause DUT to move on to SYN-SENT state 
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
	
    // Send SYN,ACK segment
    int sACK = 0xFF;
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)sACK);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //Receive DUT response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) {
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        int rSeqNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        if ((rRST == 1) && (rSeqNumber == sACK)) 
        {
            printf("\ntest passed\n");
			End_Test_Mode_2(socketID,  seqN,  ackN);
            return 0;
        }
        else
        {
            printf("\nTest failed --> DUT response is unexpected\n");
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
    //return 0;
	
  
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_UNACCEPTABLE_08_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_08 test iteration 2.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  2. CASE: flag set = ACK
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_08_IT2()
{
    int socketID;
    uint32 seqN, ackN;

    // Start Test
    StartTest();

    // 1. TESTER: Cause DUT to move on to SYN-SENT state 
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
    int sACK = 0xFF;
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)sACK);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //Receive DUT response
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) {
        int rRST = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        int rSeqNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        if ((rRST == 1) && (rSeqNumber == sACK)) 
        {
            printf("\ntest passed\n");
			End_Test_Mode_2(socketID,  seqN,  ackN);
            return 0;
        }
        else
        {
            printf("\nTest failed --> DUT response is unexpected\n");
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
*   FUNCTION NAME: TCP_UNACCEPTABLE_09_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_09 test iteration 1.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* - CASE 1: Data segment with out of window SEQ number
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_09_IT1()
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
	

    //2. TESTER: Send a data segment with out of window SEQ number
	int sSeq = 30000;
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with current send SEQ number and ACK number indicating next SEQ number expected
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0) 
    {
        int rSEQN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rACKN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
      
        if ((rSEQN == seqN + 1) && (rACKN == ackN) && (rACK == 1))
        {
            printf("\nTest passed\n");
			End_Test_Mode_2(socketID,  seqN,  ackN);
            return 0;
        }
        else
        {
        printf("\nTest failed --> DUT response is not ok \n");
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 2;
        }
    } 
    else 
    {
        printf("\nTest failed --> DUT is not responding\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 3;
    }

    //4. TESTER: Verify that the DUT remains in the FINWAIT-1 state 
    if(checkState(FINWAIT1))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in FINWAIT1 state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 4;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_UNACCEPTABLE_09_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_09 test iteration 2.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  CASE 2: Data segment with an unacceptable ACK number
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_09_IT2()
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

    //2. TESTER: Send a data segment satisfying one of the following cases
    //2. TESTER: Send a data segment with out of window SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN - 200));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with current send SEQ number and ACK number indicating next SEQ number expected
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    // Check that DUT response is a RST segment
    if (TCP_RP.length != 0) 
    {
        int rSEQN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rACKN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);

        
        if ((rSEQN == seqN + 1) && (rACKN == ackN) && (rACK == 1))
        {
            printf("\nTest passed\n");
			End_Test_Mode_2(socketID,  seqN,  ackN);
            return 0;
        }
        else{
        printf("\nTest failed --> DUT response is not ok \n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
        }
    }
    else 
    {
        printf("\nTest failed --> DUT is not responding\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in the FINWAIT-1 state 
    if(checkState(FINWAIT1))
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
*   FUNCTION NAME: TCP_UNACCEPTABLE_10_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_10 test iteration 1.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* - CASE 1: Data segment with out of window SEQ number
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_10_IT1()
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

    //2. TESTER: Send a data segment with out of window SEQ number
	int sSeq = 30000;
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with current send SEQ number and ACK number indicating next SEQ number expected
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0) 
    {
        int rSEQN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rACKN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
     
        if ((rSEQN == ackN ) && (rACKN == seqN + 1) && (rACK == 1))
        {
            printf("\nTest passed\n");
            return 0;
        }
        else
        {
        printf("\nTest failed --> DUT response is not ACK\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
        }
    } 
    else 
    {
        printf("\nTest failed --> DUT is not responding\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in the FINWAIT-2 state 
    if(checkState(FINWAIT2))
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
*   FUNCTION NAME: TCP_UNACCEPTABLE_10_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_10 test iteration 2.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  CASE 2: Data segment with an unacceptable ACK number
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_10_IT2()
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

    //2. TESTER: Send a data segment satisfying one of the following cases
    //2. TESTER: Send a data segment with out of window SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(0xFFFFFFF1));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with current send SEQ number and ACK number indicating next SEQ number expected
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    // Check that DUT response is a RST segment
    if (TCP_RP.length != 0) 
    {
        int rSEQN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rACKN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rSEQN == ackN ) && (rACKN == seqN + 1) && (rACK == 1))
        {
            printf("\nTest passed\n");
			End_Test_Mode_2(socketID,  seqN,  ackN);
            return 0;
        }
        else{
        printf("\nTest failed --> DUT response is not ACK\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
        }
    }
    else 
    {
        printf("\nTest failed --> DUT is not responding\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in the FINWAIT-2 state 
    if(checkState(FINWAIT2))
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
*   FUNCTION NAME: TCP_UNACCEPTABLE_11_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_11 test iteration 1.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* - CASE 1: Data segment with out of window SEQ number
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_11_IT1()
{
    // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to CLOSING state
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

    //2. TESTER: Send a data segment with out of window SEQ number
	int sSeq = 30000;
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(sSeq));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with current send SEQ number and ACK number indicating next SEQ number expected
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0) 
    {
        int rSEQN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rACKN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        printf("\n ****************** %d == %d  ***************\n ",rSEQN,seqN); 
        printf("\n **************** %d == %d ************** \n ",rACKN,ackN); 
        printf("\n **************** % %d == %d **************** % \n ",rACK,rACK);   
        if ((rSEQN == seqN ) && (rACKN == ackN) && (rACK == 1))
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
    } 
    else 
    {
        printf("\nTest failed --> DUT is not responding\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in the CLOSING state 
    if(checkState(CLOSING))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in Closing state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_UNACCEPTABLE_11_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_11 test iteration 2.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  CASE 2: Data segment with an unacceptable ACK number
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_11_IT2()
{
    // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to CLOSING state
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

    //2. TESTER: Send a data segment satisfying one of the following cases
    //2. TESTER: Send a data segment with out of window SEQ number
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(0xFFFFFFF1));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    EditTCPField(&TCP_P, IP, IP_TotalLength, (void *)(intptr_t)48);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with current send SEQ number and ACK number indicating next SEQ number expected
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    // Check that DUT response is a RST segment
    if (TCP_RP.length != 0) 
    {
        int rSEQN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rACKN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rSEQN == seqN ) && (rACKN == ackN) && (rACK == 1))
        {
            printf("\nTest passed\n");
			End_Test_Mode_2(socketID,  seqN,  ackN);
            return 0;
        }
        else{
        printf("\nTest failed --> DUT response is not ACK\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
        }
    }
    else 
    {
        printf("\nTest failed --> DUT is not responding\n");
		End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in the CLOSING state 
    if(checkState(CLOSING))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in CLOSING state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_UNACCEPTABLE_12_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_12 test iteration 1.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* - CASE 1: Data segment with out of window SEQ number
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_12_IT1()
{

	// Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to CLOSING state
    int socketID,socketTester;
    uint32 seqN, ackN;
    int moveDUTToLASTACKResult= moveDUTToLASTACK(&socketID, &socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToLASTACKResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to LASTACK state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }


    //2. TESTER: Send a data segment satisfying one of the following cases
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0x1000);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)ackN+1);
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with current send SEQ number and ACK number indicating next SEQ number expected
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    // Check that DUT response is a ACK segment
    if (TCP_RP.length != 0) 
    {
        int rSEQN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rACKN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);

        if ((rSEQN == seqN+1) && (rACKN == ackN) && (rACK==1))
        {
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN= (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
        else
        {
        printf("\nTest failed --> DUT response is not OK %d %d %d %d %d \n",rSEQN,seqN+1,rACKN,ackN,rACK);
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
        }
    } 
    else 
    {
        printf("\nTest failed --> DUT is not responding\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in the LAST-ACK  state 
    if(checkState(LASTACK))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in LAST-ACK state\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_UNACCEPTABLE_12_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_12 test iteration 2.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  CASE 2: Data segment with an unacceptable ACK number
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_12_IT2()
{

    
	// Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to CLOSING state
    int socketID,socketTester;
    uint32 seqN, ackN;
    int moveDUTToLASTACKResult= moveDUTToLASTACK(&socketID, &socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToLASTACKResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to LASTACK state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }

    //2. TESTER: Send a data segment satisfying one of the following cases
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)0xffff);
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with current send SEQ number and ACK number indicating next SEQ number expected
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    // Check that DUT response is a ACK segment
    if (TCP_RP.length != 0) 
    {
        int rSEQN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rACKN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);

        if ((rSEQN == seqN+1) && (rACKN == ackN) && (rACK==1))
        {
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN= (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
        else
        {
        printf("\nTest failed --> DUT response is not OK %d %d %d %d %d \n",rSEQN,seqN+1,rACKN,ackN,rACK);
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
        }
    } 
    else 
    {
        printf("\nTest failed --> DUT is not responding\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in the LAST-ACK  state 
    if(checkState(LASTACK))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in LAST-ACK state\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_UNACCEPTABLE_13_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_13 test iteration 1.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* - CASE 1: Data segment with out of window SEQ number
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_13_IT1()
{
    
	// Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to CLOSING state
    int socketID;
    uint32 seqN, ackN;
    int moveDUTToTimeWaitResult= moveDUTToTimeWait(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToTimeWaitResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to TIMEWAIT state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }


    //2. TESTER: Send a data segment satisfying one of the following cases
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0x1000);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)seqN+sizeof("TEST TCP"));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with current send SEQ number and ACK number indicating next SEQ number expected
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    // Check that DUT response is a ACK segment
    if (TCP_RP.length != 0) 
    {
        int rSEQN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rACKN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);

        if ((rSEQN == seqN) && (rACKN == ackN) && (rACK==1))
        {
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN= (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
        else
        {
        printf("\nTest failed --> DUT response is not OK\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
        }
    } 
    else 
    {
        printf("\nTest failed --> DUT is not responding\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }

   
    //4. TESTER: Verify that the DUT remains in the TIME-WAIT state 
    if(checkState(TIMEWAIT))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in TIME-WAIT state\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_UNACCEPTABLE_13_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_13 test iteration 2.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  CASE 2: Data segment with an unacceptable ACK number
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_13_IT2()
{

   // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to CLOSING state
    int socketID;
    uint32 seqN, ackN;
    int moveDUTToTimeWaitResult= moveDUTToTimeWait(&socketID, TCPConfig.DUT_Port, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToTimeWaitResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to TIMEWAIT state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }

    //2. TESTER: Send a data segment satisfying one of the following cases
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0xffff);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)seqN+sizeof("TEST TCP"));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with current send SEQ number and ACK number indicating next SEQ number expected
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    // Check that DUT response is a ACK segment
    if (TCP_RP.length != 0) 
    {
        int rSEQN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rACKN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);

        if ((rSEQN == seqN) && (rACKN == ackN) && (rACK==1))
        {
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN= (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
        else
        {
        printf("\nTest failed --> DUT response is not OK\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
        }
    } 
    else 
    {
        printf("\nTest failed --> DUT is not responding\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }

   
    //4. TESTER: Verify that the DUT remains in the TIME-WAIT state 
    if(checkState(TIMEWAIT))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in TIME-WAIT state\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_UNACCEPTABLE_14_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_14 test iteration 1.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* - CASE 1: Data segment with out of window SEQ number
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_14_IT1()
{
	uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to ESTABLISHED state at a <wnp>
    int moveDUTToCloseWaitResult =moveDUTToCloseWait(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToCloseWaitResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }
    
   

    //2. TESTER: Send a data segment satisfying one of the following cases
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0xffff);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)ackN+1);
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with current send SEQ number and ACK number indicating next SEQ number expected
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    // Check that DUT response is a ACK segment
    if (TCP_RP.length != 0) 
    {
        int rSEQN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rACKN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);

        if ((rSEQN == seqN) && (rACKN == ackN) && (rACK==1))
        {
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN= (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            printf("\nTest passed\n");
            End_Test_Mode_2(socketID,seqN, ackN);
            return 0;
        }
        else
        {
        printf("\nTest failed --> DUT response is not OK %d %d %d %d %d \n",rSEQN,seqN+1,rACKN,ackN,rACK);
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
        }
    } 
    else 
    {
        printf("\nTest failed --> DUT is not responding\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }
   
   
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_UNACCEPTABLE_14_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_UNACCEPTABLE_14 test iteration 2.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  CASE 2: Data segment with an unacceptable ACK number
*
* @warning
* -
*
***************************************************************************************************/
int TCP_UNACCEPTABLE_14_IT2()
{

	uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    // Start Test
    StartTest();

    // 1. TESTER: Cause the DUT to move on to ESTABLISHED state at a <wnp>
    int moveDUTToCloseWaitResult =moveDUTToCloseWait(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToCloseWaitResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    //2. TESTER: Send a data segment satisfying one of the following cases
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)0xffff);
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send an ACK with current send SEQ number and ACK number indicating next SEQ number expected
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    // Check that DUT response is a ACK segment
    if (TCP_RP.length != 0) 
    {
        int rSEQN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rACKN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);

        if ((rSEQN == seqN) && (rACKN == ackN) && (rACK==1))
        {
            seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN= (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            printf("\nTest passed\n");
            End_Test_Mode_2(socketID,seqN, ackN);
            return 0;
        }
        else
        {
                    printf("\nTest failed --> DUT response is not OK %d %d %d %d %d \n",rSEQN,seqN+1,rACKN,ackN,rACK);

        printf("\nTest failed --> DUT response is not OK\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
        }
    } 
    else 
    {
        printf("\nTest failed --> DUT is not responding\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }

}
