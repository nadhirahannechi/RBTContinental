/**
 *  Copyright (C) [year] Focus Corporation
 *
 *  \file     TCP_FLAGS_PROCESSING.c
 *  \brief    This file contains the tests of TCP_FLAGS_PROCESSING
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
#include "TCP_FLAGS_PROCESSING.h"
#include <sys/time.h>
#include "TestabilityProtocol_api.h"
#include "TestabilityProtocol_Intern.h"
/******************************************************************************
* DEFINES AND MACROS
******************************************************************************/

TP_ResultID_t TP_TcpListenAndAccept_cb_result;
/******************************************************************************
* LOCAL VARIABLES AND CONSTANTS
******************************************************************************/
void TP_TcpListenAndAccept_cb(TP_ResultID_t result);



/******************************************************************************
* STATIC FUNCTION PROTOTYPES
******************************************************************************/
void TP_TcpCloseSocket_cb_call_abort(TP_ResultID_t result);

/******************************************************************************
* Local FUNCTIONS
******************************************************************************/

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpListenAndAccept_cb
*
***************************************************************************************************/
/**
* @brief
*  the callback result of close socket.
*  \param[in]    TP_ResultID_t result :  the callback result of close
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/


void TP_TcpListenAndAccept_cb(TP_ResultID_t result)
{
    TP_TcpListenAndAccept_cb_result = result;
}
/******************************************************************************
* GLOBAL FUNCTIONS
******************************************************************************/
/** \brief Description of the static function
 *  \param None
 *  \return None
 *****************************************************************************/
 
/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_01
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_01 test.
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

int TCP_FLAGS_PROCESSING_01()
{


    // start test
    StartTest();

    //1. TESTER: Cause DUT to move on to SYN-RCVD state initiated by active OPEN call
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
    //2. TESTER: Send a RESET segment
    TP_TcpListenAndAccept(TP_TcpListenAndAccept_cb, NULL, socketID, 5);
    //send rst,ack to the dut
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN+1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. TESTER: Cause DUT side app to issue a receive call
    int i=0 ;
    while ((TP_TcpListenAndAccept_cb_result==RID_E_NOK) && (i<20))
    {
        sleep(1);
        i++;
    }
    


    //4. DUT: Returns error
    //5. TESTER: Verify that the DUT informs \"connection refused\" to the receiving application
    if(TP_TcpListenAndAccept_cb_result == RID_E_TCP_COR)
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest passed\n");
        return 0;

    }
    else 
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        
        printf("\nTest failed --> DUT didn't inform \"connection refused\ %d\n");
        return 1;
    }


}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_02_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_02_IT1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  1. CASE: <wst> = SYN-RCVD 
*
* @warning
* -
*
***************************************************************************************************/

int TCP_FLAGS_PROCESSING_02_IT1()
{


    // start test
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
    
    //2. TESTER: Send a RST segment
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);


    //3. DUT: Moves to CLOSED state
    //4. TESTER: Verify that the DUT moves on to CLOSED state
    if(checkState(CLOSED))
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
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_02_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_02_IT2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  1. CASE: <wst> = ESTABLISHED 
*
* @warning
* -
*
***************************************************************************************************/
//2. CASE: <wst> = ESTABLISHED
int TCP_FLAGS_PROCESSING_02_IT2()
{


    // start test
    StartTest();

    //1. TESTER: Cause the DUT to move on to <wst> state 
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
    //2. TESTER: Send a RST segment
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);


    //3. DUT: Moves to CLOSED state
    //4. TESTER: Verify that the DUT moves on to CLOSED state
    if(checkState(CLOSED))
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
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_02_IT3
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_02_IT3 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  3. CASE: <wst> = FINWAIT-1
*
* @warning
* -
*
***************************************************************************************************/

int TCP_FLAGS_PROCESSING_02_IT3()
{


    // start test
    StartTest();

    //1. TESTER: Cause the DUT to move on to <wst> state 
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

    //2. TESTER: Send a RST segment
   TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Moves to CLOSED state
    //4. TESTER: Verify that the DUT moves on to CLOSED state
    if(checkState(CLOSED))
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
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_02_IT4
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_02_IT4 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  3. CASE: <wst> = FINWAIT-2
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_PROCESSING_02_IT4()
{


    // start test
    StartTest();

    //1. TESTER: Cause the DUT to move on to <wst> state
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

    //2. TESTER: Send a RST segment
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);


    //3. DUT: Moves to CLOSED state
    //4. TESTER: Verify that the DUT moves on to CLOSED state
    if(checkState(CLOSED))
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
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_02_IT4
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_02_IT4 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  3. CASE: <wst> = close wait
*
* @warning
* -
*
***************************************************************************************************/
//5. CASE: <wst> = CLOSE-WAIT
int TCP_FLAGS_PROCESSING_02_IT5()
{


    
    // start test
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
    //2. TESTER: Send a RST segment
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);


    //3. DUT: Moves to CLOSED state
    //4. TESTER: Verify that the DUT moves on to CLOSED state
    if(checkState(CLOSED))
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
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_03_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_03_IT1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  3. CASE: <wst> = closing
*
* @warning
* -
*
***************************************************************************************************/
//1. CASE: <wst> = CLOSING 
int TCP_FLAGS_PROCESSING_03_IT1()
{


    // start test
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

    //2. TESTER: Send a RST segment
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Goes to CLOSED state
    //4. TESTER: Verify that the DUT moves on to CLOSED state
    
    if(checkState(CLOSED))
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
         
        printf("\nTest passed\n");
		return 0;
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
         
        printf("\nTest failed\n");
		return 1;  

    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_03_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_03_IT2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  3. CASE: <wst> = lastack
*
* @warning
* -
*
***************************************************************************************************/
//2. CASE: <wst> = LAST-ACK
int TCP_FLAGS_PROCESSING_03_IT2()
{


    // start test
    StartTest();
    
    //1. TESTER: Cause the DUT to move on to <wst> state 
    uint32_t socketID, socketTester;
    uint32 seqN, ackN;
    int moveDUTToLASTACKResult = moveDUTToLASTACK(&socketID,&socketTester, TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveDUTToLASTACKResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> DUT response  is not Last ACK\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
    //2. TESTER: Send a RST segment
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Goes to CLOSED state
    //4. TESTER: Verify that the DUT moves on to CLOSED state
    if(checkState(CLOSED))
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
         
        printf("\nTest passed\n");
		return 0;
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
         
        printf("\nTest failed\n");
		return 1;  

    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_04
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_04 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  3. CASE: <wst> = closing
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_PROCESSING_04()
{


    // start test
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


    //2. TESTER: Send a RST segment
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    //3. DUT: Goes to CLOSED state
    //4. TESTER: Verify that the DUT moves on to CLOSED state
    if(checkState(CLOSED))
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed\n");
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_05_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_05 iteration 1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: <stp> = SYN in window  
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_PROCESSING_05_IT1()
{

    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause the DUT to move on to SYN-RCVD state
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
    //2. TESTER: Send a segment of type Stp
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send a RST segment 
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    { 
         int RSTFlag  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if (RSTFlag == 1)
        {
            // result ok 
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
         
        printf("\nTest failed --> DUT do not send responding\n");
        return 1;

    }


    //4.TESTER: Verify that the receiving application receives the signal that \"connection reset\" has occurred from the remote site and the DUT moves on to CLOSED state
   if(checkState(CLOSED))
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed\n");
        return 1;
    }

}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_05_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_05 iteration 2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
*  -CASE: <stp> = SYN,ACK in window  
*
* @warning
* -
*
***************************************************************************************************/
int TCP_FLAGS_PROCESSING_05_IT2()
{

    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause the DUT to move on to SYN-RCVD state
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

    //2. TESTER: Send a segment of type Stp
     TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 1));
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Send a RST segment 
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    { 
         int RSTFlag  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if (RSTFlag == 1)
        {
            // result ok 
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
         
        printf("\nTest failed --> DUT do not send responding\n");
        return 1;

    }


    //4.TESTER: Verify that the receiving application receives the signal that \"connection reset\" has occurred from the remote site and the DUT moves on to CLOSED state
     if(checkState(CLOSED))
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed\n");
        return 1;
    }

}
/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_06
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_06 test.
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
int TCP_FLAGS_PROCESSING_06()
{
    

    // start test
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


    //2. TESTER: Send the last FIN once more
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN - 1));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)0);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    
    //3. DUT: Send ACK for the retransmitted FIN
     Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) 
    { 
        int rACK  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
              // Resukt OK
        } 
        else
        {
            printf ("\nTest failed --> DUT response is not ACK\n");
        }
    }
    else 
    {
	End_Test_Mode_2(socketID,  seqN,  ackN);
     
    printf("\nTest failed --> DUT not responding\n");
    return 1;
    }
    //4. TESTER: Wait for 1.5*MSL time
    sleep(1.5*TCPConfig.msl);

    //5. TESTER: Verify that the DUT remains in TIME-WAIT state
    
    if(checkState(TIMEWAIT))
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
         
        printf("\nTest passed\n");
		return 0;
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
         
        printf("\nTest failed\n");
		return 1;  

    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_07_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_07_IT1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  3. CASE: <wst> = CLOSE-WAIT
*
* @warning
* -
*
***************************************************************************************************/
//1. CASE: <wst> = CLOSE-WAIT 
int TCP_FLAGS_PROCESSING_07_IT1()
{


    // start test
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

    //2. TESTER: Send a data segment with only URG flag set
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagURG, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN ));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)seqN);
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
        End_Test_Mode_2(socketID,seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    } 
    else 
    {
        End_Test_Mode_2(socketID,seqN, ackN);
         
        printf("\nTest failed --> DUT send responding\n");
        return 1;

    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_07_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_07_IT2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  3. CASE: <wst> = CLOSING
*
* @warning
* -
*
***************************************************************************************************/
//2. CASE: <wst> = CLOSING
int TCP_FLAGS_PROCESSING_07_IT2()
{


    // start test
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
    //2. TESTER: Send a data segment with only URG flag set
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagURG, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN ));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)seqN);
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
    if (TCP_RP.length == 0) { 
        End_Test_Mode_2(socketID,seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    } 
    else 
    {
        End_Test_Mode_2(socketID,seqN, ackN);
        printf("\nTest failed --> DUT send responding\n");
        return 1;

    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_07_IT3
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_07_IT3 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  3. CASE: <wst> = LASTACK
*
* @warning
* -
*
***************************************************************************************************/
//3. CASE: <wst> = LAST-ACK
int TCP_FLAGS_PROCESSING_07_IT3()
{


    // start test
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

    //2. TESTER: Send a data segment with only URG flag set
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagURG, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN ));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)seqN);
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
    if (TCP_RP.length == 0) { 
        End_Test_Mode_2(socketID,seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    } 
    else 
    {
        End_Test_Mode_2(socketID,seqN, ackN);
        printf("\nTest failed --> DUT send responding\n");
        return 1;

    }
}
/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_07_IT4
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_07_IT4 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  3. CASE: <wst> = TIME-WAIT
*
* @warning
* -
*
***************************************************************************************************/
//4. CASE: <wst> = TIME-WAIT
int TCP_FLAGS_PROCESSING_07_IT4()
{


    // start test
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

    //2. TESTER: Send a data segment with only URG flag set
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagURG, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN ));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)seqN);
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
    if (TCP_RP.length == 0) { 
        End_Test_Mode_2(socketID,seqN, ackN);
        printf("\nTest passed\n");
        return 0;
    } 
    else 
    {
        End_Test_Mode_2(socketID,seqN, ackN);
        printf("\nTest failed --> DUT send responding\n");
        return 1;

    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_08_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_08_IT1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  3. CASE: <wst> = CLOSED
*
* @warning
* -
*
***************************************************************************************************/
//1. CASE: <wst> = CLOSED
int TCP_FLAGS_PROCESSING_08_IT1()
{


    // start test
    StartTest();

    //1. TESTER: Cause the DUT to move on to <wst> state

    //2. TESTER: Send a FIN
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0x01);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);    

    //3. DUT: in SYN-SENT or LISTEN state : Do not send any response or retransmit SYN In CLOSED state : send a RST segment
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    { 
        int rRST  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if (rRST == 1)
        {
            // result ok
        } 
        else
        {
            printf ("\nTest failed --> DUT response is not SYN ACK\n");
        }
    } 
    else 
    {
	End_Test_Mode_2(-1,  0,  0);
    printf("\nTest failed --> DUT not responding\n");
    return 1;

    }
    //4. TESTER: Verify that the DUT remains in <wst> state
    if(checkState(CLOSED))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(-1,  0,  0);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in CLOSED state\n");
        End_Test_Mode_2(-1,  0,  0);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_08_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_08_IT2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  3. CASE: <wst> = LISTEN
*
* @warning
* -
*
***************************************************************************************************/
//2. CASE: <wst> = LISTEN
int TCP_FLAGS_PROCESSING_08_IT2()
{

    // start test
    StartTest();

    //1. TESTER: Cause the DUT to move on to <wst> state
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
    //2. TESTER: Send a FIN
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0x01);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);   

    //3. DUT: in SYN-SENT or LISTEN state : Do not send any response or retransmit SYN In CLOSED state : send a RST segment
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    { 
        // DUT do not send any response
    } 
    else 
    {
	    End_Test_Mode_2(-1,  0,  0);
        printf("\nTest failed --> DUT send responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in <wst> state
    if(checkState(LISTEN))
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
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_08_IT3
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_08_IT3 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  3. CASE: <wst> = SYN-SENT 
*
* @warning
* -
*
***************************************************************************************************/
//3. CASE: <wst> = SYN-SENT 
int TCP_FLAGS_PROCESSING_08_IT3()
{


    // start test
    StartTest();

    //1. TESTER: Cause the DUT to move on to <wst> state
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
	
    //2. TESTER: Send a FIN
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0x01);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);    
    //3. DUT: in SYN-SENT or LISTEN state : Do not send any response or retransmit SYN In CLOSED state : send a RST segment
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0)
    { 
        // DUT do not send any response
    } 
    else 
    {
	    End_Test_Mode_2(socketID,  seqN,  ackN);
         
        printf("\nTest failed --> DUT responded while it shouldn't\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT remains in <wst> state
    if(checkState(SYNSENT))
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
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_09_IT1
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_09_IT1 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  3. CASE: <wst> = CLOSE-WAIT 
*
* @warning
* -
*
***************************************************************************************************/
//1. CASE: <wst> = CLOSE-WAIT
int TCP_FLAGS_PROCESSING_09_IT1()
{


    // start test
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
    //2. TESTER: Send a FIN
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)seqN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)ackN);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);    

    //3. DUT: Do not change state
    //4. TESTER: Verify that the DUT remains in <wst> state
   if(checkState(CLOSEWAIT))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in CLOSED state\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_09_IT2
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_09_IT2 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  3. CASE: <wst> = CLOSING 
*
* @warning
* -
*
***************************************************************************************************/
//2. CASE: <wst> = CLOSING
int TCP_FLAGS_PROCESSING_09_IT2()
{


    // start test
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
    //2. TESTER: Send a FIN
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)seqN);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);    

    //3. DUT: Do not change state
    //4. TESTER: Verify that the DUT remains in <wst> state
    if(checkState(CLOSING))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in CLOSING state\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_09_IT3
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_09_IT3 test.
*
* @return int : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -  3. CASE: <wst> = LAST-ACK 
*
* @warning
* -
*
***************************************************************************************************/
//3. CASE: <wst> = LAST-ACK
int TCP_FLAGS_PROCESSING_09_IT3()
{


    // start test
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
    //2. TESTER: Send a FIN
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)seqN);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);    

    //3. DUT: Do not change state
    //4. TESTER: Verify that the DUT remains in <wst> state
   if(checkState(LASTACK))
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in LASTACK state\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }
}
/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_10
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_10 test.
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
int TCP_FLAGS_PROCESSING_10()
{
   
    StartTest();

    // 1. TESTER: Cause DUT to move on to ESTABLISHED state 
    uint32_t socketID, socketTester;
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
    //2. TESTER: Cause the application on the DUT-sIDe to issue a SEND call for data of size equal to the receive window of TESTER 
    //window of TESTER 
    vint8 data;
    data.Data =(uint8*) calloc(155,sizeof(uint8));
    memset(data.Data,1,155);
    data.dataLength=155;
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
        printf("\nResult ok : DUT: Send the data segment\n");
        // get seq number and ack number
        seqN=(int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        ackN=(int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rPSHF=(int)GetTCPField(&TCP_RP, TCP, TCP_FlagPSH);
        int rACKF=(int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rACKF ==1) && (rPSHF==1))
        {
            //result ok 
        }
        else
        {
            // No response sent
            printf("\nTest failed --> DUT  respond not a data segment\n");
            End_Test_Mode_2(socketTester,  seqN,  ackN);
            return 1;
        }
 
    }
	else
	{
        // No response sent
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketTester,  seqN,  ackN);
        return 1;
    }


    //4. TESTER: Cause the application to issue onemore SEND call for data
    memset(data.Data,2,155);
    TP_TcpSendData(NULL, socketTester, data.dataLength,  0, data);

    //5. DUT: Do not send this data segment
    TCP_RP = ListenTCP(f);
    if (TCP_RP.length == 0) 
	{
        printf("\nResult ok : DUT: Do not send this data segment\n");
 
    }
	else
	{
        int rseqN=(int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        if(rseqN==ackN+155)
        {   // No response sent
            printf("\nTest failed --> DUT  send this data segment\n");
            End_Test_Mode_2(socketTester,  seqN,  ackN);
            return 1;
        }
        else
        {
         //result ok   
        }
    }

    //6. TESTER: Send a data segment with ACK for the previous received segment
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN + 155));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);    

    //7. DUT: Send the pending data segment piggybacking the acknowledgement within 0.5 sec 
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    TCP_Packet TCP_RP2 = ListenTCP(f);
    if (TCP_RP2.length != 0) 
    {   
        char * payload = (char*)GetTCPField(&TCP_RP2,PAYLAOD, PAYLOAD_data);
        if(strcmp(payload,(char*)data.Data) == 0)
        {
            gettimeofday(&stop, NULL);
            double deltaT_us = time_diff(stop,start);
            //500000 us = 0.5s
            if (deltaT_us<500000)
            {
                printf("\nTest passed\n");
                End_Test_Mode_2(socketTester,  seqN,  ackN);
                return 0;
            }
            else
            {
                End_Test_Mode_2(socketTester,  seqN,  ackN);
                printf("\nTest failed --> the delay in receiving the ACK > 0.5s\n");
                return 1;
            }
        }
        else 
        {
            End_Test_Mode_2(socketTester,  seqN,  ackN);
            printf("\nTest failed --> response not ok\n");
            return 1; 
        }
    }
    else 
    {
        End_Test_Mode_2(socketTester,  seqN,  ackN);
        printf("\nTest failed --> response not ok\n");
        return 1;  
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_FLAGS_PROCESSING_10
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_FLAGS_PROCESSING_10 test.
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
int TCP_FLAGS_PROCESSING_11()
{


    //start test    
    StartTest();

    //1. TESTER: Cause DUT to move to ESTABLISHED state
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
    //2. TESTER: Send the last ACK once more
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)seqN-1);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN ));
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
    if (TCP_RP.length == 0) { 
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest passed\n");
        return 0;    } 
    else 
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
         
        printf("\nTest failed --> DUT send responding\n");
        return 1;

    }

    //4. TESTER: Verify that the DUT remains in ESTABLISHED state
    if(checkState(ESTABLISHED))
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest passed\n");
        return 0;
    }
    else
    {
        End_Test_Mode_2(socketID,  seqN,  ackN); 
        printf("\nTest failed\n");
        return 1;
    }

}
