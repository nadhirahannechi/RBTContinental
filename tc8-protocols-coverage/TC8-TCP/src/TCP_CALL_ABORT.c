/**
 *  Copyright (C) [year] Focus Corporation
 *
 *  \file     TCP_CALL_ABORT.c
 *  \brief    This file contains the tests of TCP CALL ABORT 
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
#include "TCP_CALL_ABORT.h"
#include "TestabilityProtocol_api.h"

/******************************************************************************
* DEFINES AND MACROS
******************************************************************************/
TP_ResultID_t TP_TcpCloseSocket_cb_result_call_abort = RID_E_NOK; /** <the callback result of close socket > **/


/******************************************************************************
* GLOBAL VARIABLES AND CONSTANTS
******************************************************************************/



/******************************************************************************
* STATIC FUNCTION PROTOTYPES
******************************************************************************/
void TP_TcpCloseSocket_cb_call_abort(TP_ResultID_t result);

/******************************************************************************
* Local FUNCTIONS
******************************************************************************/


/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpCloseSocket_cb_call_abort
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
void TP_TcpCloseSocket_cb_call_abort(TP_ResultID_t result)
{
    TP_TcpCloseSocket_cb_result_call_abort = result;
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
*   FUNCTION NAME: TCP_CALL_ABORT_02
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_CALL_ABORT_02 test.
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
int TCP_CALL_ABORT_02()
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

    //2. TESTER: Cause the application to issue an ABORT call
    TP_TcpCloseSocket(NULL, (uint16_t)socketID, TRUE);

    //3. DUT: Send a RST control message
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;

    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) 
    { 
        int rRST  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if (rRST == 1)
        {
           //result ok
        } 
        else
        {
            printf ("\nTest failed --> DUT response is not RST\n");
        }
    }
    else 
    {
	    TP_GenaralEndTest(NULL,0,(text){0});
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Verify that the DUT moves on to CLOSED state
    if(checkState(CLOSED))
    {
        printf("\nTest passed\n");
        TP_GenaralEndTest(NULL,0,(text){0});
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in CLOSED state\n");
        TP_GenaralEndTest(NULL,0,(text){0});
        return 1;
    }
}


/***************************************************************************************************
*
*   FUNCTION NAME: TCP_CALL_ABORT_03_IT01
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_CALL_ABORT_03 test first iteration.
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
int TCP_CALL_ABORT_03_IT01()
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

    //1. TESTER: Cause the DUT to move on to the <wst> state
    uint32 seqN, ackN;    
    int moveToClosing=moveDUTToClosing(&socketID, TCPConfig.DUT_Port , TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    
    //2. TESTER: Cause the application on the DUT-sIDe to issue an ABORT call
    TP_TcpCloseSocket(TP_TcpCloseSocket_cb_call_abort, (uint16_t)socketID, true);
    sleep(5);
    //3. DUT: Issues an ABORT call
    //4. TESTER: Verify that the DUT responds with \"ok\" to the application
    if (TP_TcpCloseSocket_cb_result_call_abort == RID_E_NOK)
    {
        End_Test_Mode_2(socketID,seqN, ackN);
        printf("\nTest failed-> close abort cb = %d\n" ,TP_TcpCloseSocket_cb_result_call_abort);		
		return 1; 
    }
    else 
    {
        //result ok 
    }


    //5. TESTER: Verify that the DUT moves on to CLOSED state
    if(checkState(CLOSED))
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
*   FUNCTION NAME: TCP_CALL_ABORT_03_IT02
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_CALL_ABORT_03 test second iteration.
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
int TCP_CALL_ABORT_03_IT02()
{
    int socketID,socketTester;

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

    //1. TESTER: Cause the DUT to move on to the <wst> state
    uint32 seqN, ackN;    
    int moveToLastACKres=moveDUTToLASTACK(&socketID, &socketTester,TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN, &ackN);
    if (moveToLastACKres == Result_OK)
    {
        printf("\n move to LastACK with success \n");
    }
    else 
    {
        printf("\n Test failed :cannot move to LAST ACK \n");
        return Result_NOK;
    }

    //2. TESTER: Cause the application on the DUT-sIDe to issue an ABORT call
    TP_TcpCloseSocket(TP_TcpCloseSocket_cb_call_abort, (uint16_t)socketID, true);
    sleep(5);
    //3. DUT: Issues an ABORT call
    //4. TESTER: Verify that the DUT responds with \"ok\" to the application
    if (TP_TcpCloseSocket_cb_result_call_abort == RID_E_NOK)
    {
        End_Test_Mode_2(socketID,seqN, ackN);
        printf("\nTest failed-> close abort cb = %d\n" ,TP_TcpCloseSocket_cb_result_call_abort);		
		return 1; 
    }
    else 
    {
        //result ok 
    }


    //5. TESTER: Verify that the DUT moves on to CLOSED state
    if(checkState(CLOSED))
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
*   FUNCTION NAME: TCP_CALL_ABORT_03_IT03
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_CALL_ABORT_03 test third iteration.
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
int TCP_CALL_ABORT_03_IT03()
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

    //1. TESTER: Cause the DUT to move on to the <wst> state
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

    //2. TESTER: Cause the application on the DUT-sIDe to issue an ABORT call
    TP_TcpCloseSocket(TP_TcpCloseSocket_cb_call_abort, (uint16_t)socketID, true);
    sleep(5);
    //3. DUT: Issues an ABORT call
    //4. TESTER: Verify that the DUT responds with \"ok\" to the application
    if (TP_TcpCloseSocket_cb_result_call_abort == RID_E_NOK)
    {
        End_Test_Mode_2(socketID,seqN, ackN);
        printf("\nTest failed-> close abort cb = %d\n" ,TP_TcpCloseSocket_cb_result_call_abort);		
		return 1; 
    }
    else 
    {
        //result ok 
    }


    //5. TESTER: Verify that the DUT moves on to CLOSED state
    if(checkState(CLOSED))
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
