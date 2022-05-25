/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   TCPcommon.h
 *  \brief  This is a sample C header file template.
 *  \author Aymen SALEM && Omar NSIRI
 *
 *  \addtogroup SW_package
 *  \{
 */
#ifndef TCPCOMMON_H
#define TCPCOMMON_H

/******************************************************************************
* INCLUDE FILES
******************************************************************************/
#include "AbstractionAPI.h"
#include <stdbool.h>
#include "TestabilityProtocolTypes.h"
#include "TCPconfig.h"

/******************************************************************************
* MACROS
******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif  /* #ifdef __cplusplus*/

/******************************************************************************
* DEFINES
******************************************************************************/
#define Result_OK 0
#define Result_NOK 1


    enum tcp_state
    {
        CLOSED,
        ESTABLISHED,
        SYNSENT,
        SYNRCVD,
        FINWAIT1,
        FINWAIT2,
        TIMEWAIT,
        CLOSE,
        CLOSEWAIT,
        LASTACK,
        LISTEN,
        CLOSING
    };


    /******************************************************************************
* GLOBAL VARIABLES AND CONSTANTS
******************************************************************************/


/******************************************************************************
* GLOBAL FUNCTIONS
******************************************************************************/

/***************************************************************************************************
*
*   FUNCTION NAME: stoiIP
*
***************************************************************************************************/
/**
* @brief
*  convert string to ipv4 format (uint8[4])
*
* @par Parameters
* @param[in] const char *in_s : ipv4 address string
* @param[in] uint8 *out_ip    : output ipv4 address uint8[4]
*
* @return bool : 0: The state progression was successful | 1: The state progression dIDn't go as expected
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void stoiIP (const char *in_s, uint8 *out_ip);

/***************************************************************************************************
*
*   FUNCTION NAME: checkState
*
***************************************************************************************************/
/**
* @brief
*  check if tcp state is the input state.
*
* @par Parameters
* @param[in] tcp_state st : state to check 
*
* @return bool : 0: The state progression was successful | 1: The state progression dIDn't go as expected
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
bool checkState(tcp_state st);


/*******************************************************************************************************************
 * ************************************ TCP State machine for DUT as Server ****************************************
 * *****************************************************************************************************************/

/***************************************************************************************************
*
*   FUNCTION NAME: moveDUTToListen
*
***************************************************************************************************/
/**
* @brief
*  Moves the DUT to LISTEN state.
*
* @par Parameters
* @param[in] socketDUT pointer to the DUT's socket.
* @param[in] socketTester pointer to the tester's socket.
* @param[in] portDUT : Type uint16 : DUT's port number.
* @param[in] maxCon : Type uint16 : Maximum number of connections allowed to establish.
*
* @return bool : 0: The state progression was successful | 1: The state progression dIDn't go as expected
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
bool moveDUTToListen(int* socketDUT,int* socketTester, uint16 portDUT, uint16 maxCon);

/***************************************************************************************************
*
*   FUNCTION NAME: moveDUTToSYNRCVD
*
***************************************************************************************************/
/**
* @brief
*  Moves the DUT to SYN-RCVD state.
*
* @par Parameters
* @param[in] socketDUT pointer to the DUT's socket.
* @param[in] socketTester pointer to the Tester's socket.
* @param[in] portDUT : Type uint16 : DUT's port number.
* @param[in] maxCon : Type uint16 : Maximum number of connections allowed to establish.
* @param[in] seqN : Type uint32 : Last sequence number.
* @param[in] ackN : Type uint32 : Last acknowledgment number.
*
* @return bool : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
bool moveDUTToSYNRCVD(int* socketDUT, int * socketTester, uint16 portDUT, uint16 maxCon, uint32* seqN, uint32* ackN);

/***************************************************************************************************
*
*   FUNCTION NAME: moveServDUTToEstablished
*
***************************************************************************************************/
/**
* @brief
*  Moves the DUT to to ESTABLISHED state as server.
*
* @par Parameters
* @param[in] socketDUT pointer to the DUT's socket.
* @param[in] socketTester pointer to the Tester's socket.
* @param[in] portDUT : Type uint16 : DUT's port number.
* @param[in] maxCon : Type uint16 : Maximum number of connections allowed to establish.
* @param[in] seqN : Type uint32 : Last sequence number.
* @param[in] ackN : Type uint32 : Last acknowledgment number.
*
* @return bool : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
bool moveServDUTToEstablished(int* socketDUT, int * socketTester, uint16 portDUT, uint16 maxCon, uint32* seqN, uint32* ackN);

/***************************************************************************************************
*
*   FUNCTION NAME: moveDUTToCloseWait
*
***************************************************************************************************/
/**
* @brief
*  Moves the DUT to CLOSE-WAIT state.
*
* @par Parameters
* @param[in] socketDUT pointer to the DUT's socket.
* @param[in] socketTester pointer to the Tester's socket.
* @param[in] portDUT : Type uint16 : DUT's port number.
* @param[in] maxCon : Type uint16 : Maximum number of connections allowed to establish.
* @param[in] seqN : Type uint32 : Last sequence number.
* @param[in] ackN : Type uint32 : Last acknowledgment number.
*
* @return bool : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
bool moveDUTToCloseWait(int* socketDUT,  int * socketTester, uint16 portDUT, uint16 maxCon, uint32* seqN, uint32* ackN);


/***************************************************************************************************
*
*   FUNCTION NAME: moveDUTToLASTACK
*
***************************************************************************************************/
/**
* @brief
*  Moves the DUT to LAST-ACK state.
*
* @par Parameters
* @param[in] socketDUT pointer to the DUT's socket.
* @param[in] portDUT : Type uint16 : DUT's port number.
* @param[in] maxCon : Type uint16 : Maximum number of connections allowed to establish.
* @param[in] seqN : Type uint32 : Last sequence number.
* @param[in] ackN : Type uint32 : Last acknowledgment number.
*
* @return bool : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
bool moveDUTToLASTACK(int* socketDUT, int * socketTester, uint16 portDUT, uint16 maxCon, uint32* seqN, uint32* ackN);


/*******************************************************************************************************************
 * ************************************ TCP State machine for DUT as Client ****************************************
 * *****************************************************************************************************************/

/***************************************************************************************************
*
*   FUNCTION NAME: moveDUTToSYNSENT
*
***************************************************************************************************/
/**
* @brief
*  Moves the DUT to SYN-SENT state.
*
* @par Parameters
* @param[in] socketDUT pointer to the DUT's socket.
* @param[in] socketTESTER pointer to the TESTER's socket.
* @param[in] portDUT : Type uint16 : DUT's port number.
* @param[in] maxCon : Type uint16 : Maximum number of connections allowed to establish.
* @param[in] seqN : Type uint32 : Last sequence number.
* @param[in] ackN : Type uint32 : Last acknowledgment number.
*
* @return bool : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
bool moveDUTToSYNSENT(int* socketDUT, uint16 portDUT, uint16 portTESTER, uint16 maxCon, uint32* seqN, uint32* ackN);

/***************************************************************************************************
*
*   FUNCTION NAME: moveClientDUTToEstablished
*
***************************************************************************************************/
/**
* @brief
*  Moves the DUT to ESTABLISHED state as client.
*
* @par Parameters
* @param[in] socketDUT pointer to the DUT's socket.
* @param[in] socketTESTER pointer to the TESTER's socket.
* @param[in] portDUT : Type uint16 : DUT's port number.
* @param[in] maxCon : Type uint16 : Maximum number of connections allowed to establish.
* @param[in] seqN : Type uint32 : Last sequence number.
* @param[in] ackN : Type uint32 : Last acknowledgment number.
*
* @return bool : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
bool moveClientDUTToEstablished(int* socketDUT, uint16 portDUT, uint16 portTESTER, uint16 maxCon, uint32* seqN, uint32* ackN);

/***************************************************************************************************
*
*   FUNCTION NAME: moveDUTToFINWAIT1
*
***************************************************************************************************/
/**
* @brief
*  Moves the DUT to FIN-WAIT1 state.
*
* @par Parameters
* @param[in] socketDUT pointer to the DUT's socket.
* @param[in] socketTESTER pointer to the TESTER's socket.
* @param[in] portDUT : Type uint16 : DUT's port number.
* @param[in] maxCon : Type uint16 : Maximum number of connections allowed to establish.
* @param[in] seqN : Type uint32 : Last sequence number.
* @param[in] ackN : Type uint32 : Last acknowledgment number.
*
* @return bool : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
bool moveDUTToFINWAIT1(int* socketDUT, uint16 portDUT, uint16 portTESTER, uint16 maxCon, uint32* seqN, uint32* ackN);

/***************************************************************************************************
*
*   FUNCTION NAME: moveDUTToFINWAIT2
*
***************************************************************************************************/
/**
* @brief
*  Moves the DUT to FIN-WAIT2 state.
*
* @par Parameters
* @param[in] socketDUT pointer to the DUT's socket.
* @param[in] socketTESTER pointer to the TESTER's socket.
* @param[in] portDUT : Type uint16 : DUT's port number.
* @param[in] maxCon : Type uint16 : Maximum number of connections allowed to establish.
* @param[in] seqN : Type uint32 : Last sequence number.
* @param[in] ackN : Type uint32 : Last acknowledgment number.
*
* @return bool : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
bool moveDUTToFINWAIT2(int* socketDUT, uint16 portDUT, uint16 portTESTER, uint16 maxCon, uint32* seqN, uint32* ackN);

/***************************************************************************************************
*
*   FUNCTION NAME: moveDUTToClosing
*
***************************************************************************************************/
/**
* @brief
*  Moves the DUT to CLOSING state.
*
* @par Parameters
* @param[in] socketDUT pointer to the DUT's socket.
* @param[in] socketTESTER pointer to the TESTER's socket.
* @param[in] portDUT : Type uint16 : DUT's port number.
* @param[in] maxCon : Type uint16 : Maximum number of connections allowed to establish.
* @param[in] seqN : Type uint32 : Last sequence number.
* @param[in] ackN : Type uint32 : Last acknowledgment number.
*
* @return bool : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
bool moveDUTToClosing(int* socketDUT, uint16 portDUT, uint16 portTESTER, uint16 maxCon, uint32* seqN, uint32* ackN);

/***************************************************************************************************
*
*   FUNCTION NAME: moveDUTToTimeWait
*
***************************************************************************************************/
/**
* @brief
*  Moves the DUT to TIME-WAIT state.
*
* @par Parameters
* @param[in] socketDUT pointer to the DUT's socket.
* @param[in] socketTESTER pointer to the TESTER's socket.
* @param[in] portDUT : Type uint16 : DUT's port number.
* @param[in] maxCon : Type uint16 : Maximum number of connections allowed to establish.
* @param[in] seqN : Type uint32 : Last sequence number.
* @param[in] ackN : Type uint32 : Last acknowledgment number.
*
* @return bool : 0: The state progression was successful | 1: The state progression dIDn't go as expected.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
bool moveDUTToTimeWait(int* socketDUT, uint16 portDUT, uint16 portTESTER, uint16 maxCon, uint32* seqN, uint32* ackN);

int StartTest();
int EndTest(int socketID);
int End_Test_Mode_2(int SocketID, int seqN, int ackN);
double time_diff(struct timeval x , struct timeval y);
#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus*/

#endif /* TCPCOMMON_H */
/** \}    end of addtogroup */
