/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file     TCPcommon.c
 *  \brief    This file is an internal TCPmodule lib functions
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
#include "TestabilityProtocolTypes.h"
#include "TestabilityProtocol_api.h"
#include <cstdio>

/******************************************************************************
* DEFINES AND MACROS
******************************************************************************/

/******************************************************************************
* GLOBAL VARIABLES AND CONSTANTS
******************************************************************************/
TCP_config_t TCPConfig;
tcp_state TP_TcpGetState_cb_State_common_file = CLOSED;                   /** < tcp state >*/
TP_ResultID_t TP_TcpGetState_cb_result_common_file = RID_E_NOK;           /** <the callback result of check state > **/
TP_ResultID_t TP_CreateAndBind_result_common_file = RID_E_NOK;            /** <the callback result of create and bind > **/
uint16 TP_CreateAndBind_socketID_common_file = -1;                           /**< Socket file description ID created*/
TP_ResultID_t TP_TcpListenAndAccept_result_common_file = RID_E_NOK;       /** <the callback result of listen and accept > **/
TP_ResultID_t TP_TcpListenAndAccept_event_result_common_file = RID_E_NOK; /** <the callback result of listen and accept event > **/
uint16 TP_TcpListenAndAccept_event_Server_SocketID_common_file = -1;      /** < Socket server file description ID > **/
uint16 TP_TcpListenAndAccept_event_Client_SocketID_common_file = 6;      /** <Socket client file description ID > **/
uint16 TP_TcpListenAndAccept_event_Client_Port_common_file = -1;          /** <Port client > **/
ip4addr TP_TcpListenAndAccept_event_Client_Address_common_file;           /** <Address client > **/
TP_ResultID_t TP_TcpConnect_cb_result_common_file = RID_E_NOK;            /** <the callback result of close socket > **/
TP_ResultID_t TP_TcpCloseSocket_cb_result_common_file = RID_E_NOK;        /** <the callback result of close socket > **/
TP_ResultID_t TP_TcpCloseSocket_cb_result_common_file_2 = RID_E_NOK;        /** <the callback result of close socket > **/
TP_ResultID_t TP_GenaralStartTest_cb_result_common_file = RID_E_NOK;      /** <the callback result of start test > **/
TP_ResultID_t TP_GenaralEndTest_cb_result_common_file = RID_E_NOK;        /** <the callback result of end test > **/

/******************************************************************************
* Local FUNCTION PROTOTYPES
******************************************************************************/
void TP_TcpGetState_cb_common_file(TP_ResultID_t result, uint8 State);
void TP_CreateAndBind_cb_common_file(TP_ResultID_t result, uint16 sock);
void TP_TcpListenAndAccept_cb_common_file(TP_ResultID_t result);
void TP_TcpListenAndAccept_event_cb_common_file(TP_ResultID_t result, uint16 Server_SocketID, uint16 Client_SocketID, uint16 Client_Port, ip4addr Client_Address);
void TP_TcpConnect_cb_common_file(TP_ResultID_t result);
void TP_TcpCloseSocket_cb_common_file(TP_ResultID_t result);
void TP_TcpCloseSocket_cb_common_file_2(TP_ResultID_t result);
void TP_GenaralStartTest_cb_common_file(TP_ResultID_t result);
void TP_GenaralEndTest_cb_common_file(TP_ResultID_t result);

/******************************************************************************
* local FUNCTIONS
******************************************************************************/

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpGetState_cb_common_file
*
***************************************************************************************************/
/**
* @brief
*  the callback result of TCP get state.
*  \param[in]     result      - callback result
*  \param[in]     State       - State of tcp socket
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void TP_TcpGetState_cb_common_file(TP_ResultID_t result, uint8 State)
{
    TP_TcpGetState_cb_result_common_file = result;
    TP_TcpGetState_cb_State_common_file = State;
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
void TP_CreateAndBind_cb_common_file(TP_ResultID_t result, uint16 sock)
{
    TP_CreateAndBind_result_common_file = result;
    TP_CreateAndBind_socketID_common_file = sock;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpListenAndAccept_cb_common_file
*
***************************************************************************************************/
/**
* @brief
*  the callback result of listen and accept.
*  \param[in]    TP_ResultID_t  : result      - callback result
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void TP_TcpListenAndAccept_cb_common_file(TP_ResultID_t result)
{
    TP_TcpListenAndAccept_result_common_file = result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpListenAndAccept_cb_common_file
*
***************************************************************************************************/
/**
* @brief
*  the callback result of listen and accept.
*  \param[in]    TP_ResultID_t TP_TcpListenAndAccept_event_result_common_file :  the callback result of listen and accept event 
*  \param[in]    uint16 TP_TcpListenAndAccept_event_Server_SocketID_common_file   Socket server file description ID
*  \param[in]    uint16 TP_TcpListenAndAccept_event_Client_SocketID_common_file   Socket client file description ID 
*  \param[in]    uint16 TP_TcpListenAndAccept_event_Client_Port_common_file   Port client 
*  \param[in]    ip4addr TP_TcpListenAndAccept_event_Client_Address_common_file;  Address client 
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void TP_TcpListenAndAccept_event_cb_common_file(TP_ResultID_t result, uint16 Server_SocketID, uint16 Client_SocketID, uint16 Client_Port, ip4addr Client_Address)
{
    TP_TcpListenAndAccept_event_result_common_file = result;
    TP_TcpListenAndAccept_event_Server_SocketID_common_file = Server_SocketID;
    TP_TcpListenAndAccept_event_Client_SocketID_common_file = Client_SocketID;
    TP_TcpListenAndAccept_event_Client_Port_common_file = Client_Port;
    //&TP_TcpListenAndAccept_event_Client_Address_common_file = &Client_Address;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_GenaralStartTest_cb_common_file
*
***************************************************************************************************/
/**
* @brief
*  the callback result of start test.
*  \param[in]    TP_ResultID_t TP_TcpCloseSocket_cb_result_common_file :  the callback result of close socket
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void TP_GenaralStartTest_cb_common_file(TP_ResultID_t result)
{
    TP_GenaralStartTest_cb_result_common_file = result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_GenaralEndTest_cb_common_file
*
***************************************************************************************************/
/**
* @brief
*  the callback result of close socket.
*  \param[in]    TP_ResultID_t result :  the callback result of end test
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void TP_GenaralEndTest_cb_common_file(TP_ResultID_t result)
{
    TP_GenaralEndTest_cb_result_common_file = result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpConnect_cb_common_file
*
***************************************************************************************************/
/**
* @brief
*  the callback result of connect socket.
*  \param[in]    TP_ResultID_t result :  the callback result of connect
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void TP_TcpConnect_cb_common_file(TP_ResultID_t result)
{
    TP_TcpConnect_cb_result_common_file = result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpCloseSocket_cb_common_file
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
void TP_TcpCloseSocket_cb_common_file(TP_ResultID_t result)
{
    TP_TcpCloseSocket_cb_result_common_file = result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpCloseSocket_cb_common_file_2
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
void TP_TcpCloseSocket_cb_common_file_2(TP_ResultID_t result)
{
    TP_TcpCloseSocket_cb_result_common_file_2 = result;
}



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
void stoiIP(const char *in_s, uint8 *out_ip)
{
    char temp_c;
    uint8 octet_i = 0;
    uint8 octet_p = 0;
    ;
    for (uint8 i = 0; i < strlen(in_s); i++)
    {
        temp_c = in_s[i];
        if (temp_c == '.')
        {
            out_ip[octet_p] = octet_i;
            octet_i = 0;
            octet_p += 1;
        }
        else
        {
            octet_i *= 10;
            octet_i += temp_c - '0';
        }
    }
    out_ip[octet_p] = octet_i;
}

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
bool checkState(tcp_state st)
{
    int i=0;
    ip4addr ipDUT, ipTESTER;
    ipDUT.dataLength = 4;
    ipTESTER.dataLength = 4;
    stoiIP(TCPConfig.DUT_IP, ipDUT.Data);
    if (st != LISTEN)
    {
        stoiIP(TCPConfig.TESTER_IP, ipTESTER.Data);
        TP_TcpSocketState(TP_TcpGetState_cb_common_file, TCPConfig.DUT_Port, ipDUT, TCPConfig.Tester_Port, ipTESTER);
    }
    else
    {
        stoiIP("0.0.0.0", ipTESTER.Data);
        TP_TcpSocketState(TP_TcpGetState_cb_common_file, TCPConfig.DUT_Port, ipDUT, 0, ipTESTER);
    }
    
    while ((TP_TcpGetState_cb_result_common_file==RID_E_NOK) && (i<20))
    {
        sleep(1);
        i++;
    }

    if ((TP_TcpGetState_cb_result_common_file ==RID_E_OK ) && (TP_TcpGetState_cb_State_common_file == st))
    {
        printf("\n state ok \n");
        TP_TcpGetState_cb_result_common_file = RID_E_NOK;
        TP_TcpGetState_cb_State_common_file =CLOSED;
        return true;
    }
    else
    {
        printf("\n state not ok %d %d %d\n",st,TP_TcpGetState_cb_State_common_file,TP_TcpGetState_cb_result_common_file);
        TP_TcpGetState_cb_result_common_file = RID_E_NOK;   
        TP_TcpGetState_cb_State_common_file =CLOSED;     
        return false;
    }
}

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
bool moveDUTToListen(int *socketDUT, int *socketTester, uint16 portDUT, uint16 maxCon)
{
    ip4addr ipv4;
    ipv4.dataLength = 4;
    stoiIP(TCPConfig.DUT_IP, ipv4.Data);
    // Move DUT to listen state
    TP_TcpCreateAndBind(TP_CreateAndBind_cb_common_file, true, TCPConfig.DUT_Port, ipv4);
    sleep(3);
    // check the callback result
    if (TP_CreateAndBind_result_common_file == RID_E_OK)
    {
        TP_CreateAndBind_result_common_file = RID_E_NOK;
        //get the socket dut ID
        *socketDUT = TP_CreateAndBind_socketID_common_file;

        //start listen and accept service
        TP_TcpListenAndAccept(TP_TcpListenAndAccept_cb_common_file, TP_TcpListenAndAccept_event_cb_common_file, TP_CreateAndBind_socketID_common_file, maxCon);
        int i=0 ;
        while ((TP_TcpListenAndAccept_result_common_file==RID_E_NOK) && (i<20))
        {
            sleep(1);
            i++;
        }
        if (TP_TcpListenAndAccept_result_common_file == RID_E_NOK)
        {
             return Result_NOK;
        }
        else 
        {
            TP_TcpListenAndAccept_result_common_file = RID_E_NOK;
        }
        // Verify that the DUT  in LISTEN state
        if (checkState(LISTEN))
        {
            return Result_OK;
        }
        else
        {
            return Result_NOK;
        }
        
    }
    else
    {
        return Result_NOK;
    }
}

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
bool moveDUTToSYNRCVD(int *socketDUT, int *socketTester, uint16 portDUT, uint16 maxCon, uint32 *seqN, uint32 *ackN)
{
    // Move DUT to Listen state
    int moveDUTToListenResult = moveDUTToListen(socketDUT, socketTester, portDUT, maxCon);
    if (moveDUTToListenResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }
    // Sending SYN to DUT
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)0x05);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    /*int i=0;
    while(((TP_TcpListenAndAccept_result_common_file== RID_E_NOK) || (TP_TcpListenAndAccept_event_result_common_file == RID_E_NOK)) && (i<10))
    {
        sleep(1);
        i++;
    } */
    
    // check the callback result
    
    /*if ((TP_TcpListenAndAccept_result_common_file == RID_E_OK) && (TP_TcpListenAndAccept_event_result_common_file == RID_E_OK))
    {
   
        printf("\n *****TP_TcpListenAndAccept_event_Client_SocketID_common_file = %d \n",TP_TcpListenAndAccept_event_Client_SocketID_common_file);
        *socketTester = TP_TcpListenAndAccept_event_Client_SocketID_common_file;
        
    }
    else
    {
        return Result_NOK;
    }*/
    
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        uint8 rSYN = (uint8)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        uint8 rACK = (uint8)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((rSYN == 1) && (rACK == 1))
        {
            if (seqN != NULL)
                *seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            if (ackN != NULL)
                *ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            
        }
        else
        {
            printf("\n DUT response does not SYN,ACK :  cannot move to  SYNRCVD \n");
            return Result_NOK;
        }
    }
    else 
    {
        printf("\n DUT not responding : cannot move to  SYNRCVD \n");
        return Result_NOK; 
    }

    // Verify that the DUT in SYN-RCVD state
    if (checkState(SYNRCVD))
    {
        printf("\n DUT state = SYNRCVD \n");
        if (socketTester != NULL)
    *socketTester = TP_TcpListenAndAccept_event_Client_SocketID_common_file;
        return Result_OK;
    }
    else
    {
        printf("\n DUT state not ok: cannot move to  SYNRCVD \n");
        return Result_NOK;
    }
}

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
bool moveServDUTToEstablished(int *socketDUT, int *socketTester, uint16 portDUT, uint16 maxCon, uint32 *seqN, uint32 *ackN)
{
    int moveToSynRCV = moveDUTToSYNRCVD(socketDUT, socketTester, portDUT, maxCon, seqN, ackN);
    if (moveToSynRCV == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }
    //send an ACK
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)*ackN );
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)*seqN+1 );
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // get the last sequence and ack
    if (seqN != NULL) *seqN = (uint32)GetTCPField(&TCP_P, TCP, TCP_SeqNumber);
    if (ackN != NULL) *ackN = (uint32)GetTCPField(&TCP_P, TCP, TCP_AckNumber);     
    
    
    // Verify that the DUT in Established state
    if (checkState(ESTABLISHED))
    {
        printf("\n DUT state = ESTABLISHED \n");
        return Result_OK;
    }
    else
    {
        
        printf("\n DUT state not ok :  cannot move to  Established \n");
        return Result_NOK;
    }
}

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
bool moveDUTToCloseWait(int *socketDUT, int *socketTester, uint16 portDUT, uint16 maxCon, uint32 *seqN, uint32 *ackN)
{
    //Move to established state
    int MoveToEstablished = moveServDUTToEstablished(socketDUT, socketTester, portDUT, maxCon, seqN, ackN);
    if (MoveToEstablished == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    //send FIN,ACK
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)*seqN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)*ackN);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //Receive ACK
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            printf("\n DUT response does not ACK :  cannot move to  CLOSEWAIT \n");

            if (seqN != NULL)
                *seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            if (ackN != NULL)
                *ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
        else
        {
            printf("\n DUT not responding : cannot move to  CLOSEWAIT \n");
            return Result_NOK;
        }
    }
    else
    {
        return Result_NOK;
    }

    // Verify that the DUT  in CLOSEWAIT state
    if (checkState(CLOSEWAIT))
    {
        printf("\n DUT state = CLOSEWAIT \n");
        return Result_OK;
    }
    else
    {
        printf("\n DUT state not ok :  cannot move to  CLOSEWAIT \n");
        return Result_NOK;
    }
}

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
bool moveDUTToLASTACK(int *socketDUT, int *socketTester, uint16 portDUT, uint16 maxCon, uint32 *seqN, uint32 *ackN)
{
    int MoveToCloseWait = moveDUTToCloseWait(socketDUT, socketTester, portDUT, maxCon, seqN, ackN);
    if (MoveToCloseWait == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }
    
    
    // close sockets
    TP_TcpCloseSocket(TP_TcpCloseSocket_cb_common_file, (uint16_t)*socketDUT, TRUE);
    TP_TcpCloseSocket(TP_TcpCloseSocket_cb_common_file_2, (uint16_t)*socketTester, FALSE);
    int i=0 ;
    while ((TP_TcpCloseSocket_cb_result_common_file_2==RID_E_NOK) && (i<20))
    {
        sleep(1);
        i++;
    }
    
    //check close socket result
    if (TP_TcpCloseSocket_cb_result_common_file_2 == RID_E_OK) 
    {
        TP_TcpCloseSocket_cb_result_common_file_2 = RID_E_NOK;
        TP_TcpCloseSocket_cb_result_common_file  = RID_E_NOK;
        //misra
    }
    else
    {
        TP_TcpCloseSocket_cb_result_common_file_2 = RID_E_NOK;
        TP_TcpCloseSocket_cb_result_common_file  = RID_E_NOK;
        printf(" TP_TcpCloseSocket_cb_result_common_file = %d",TP_TcpCloseSocket_cb_result_common_file);
        return Result_NOK;
    }

    //check state
    if (checkState(LASTACK))
    {
        return Result_OK;
    }
    else
    {
        return Result_NOK;
    }
}

// TCP State machine for DUT as Client

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
bool moveDUTToSYNSENT(int *socketDUT, uint16 portDUT, uint16 portTESTER, uint16 maxCon, uint32 *seqN, uint32 *ackN)
{
    ip4addr ipv4;
    ipv4.dataLength = 4;
    stoiIP(TCPConfig.DUT_IP, ipv4.Data);
    // Cause DUT to issue an active open call
    TP_TcpCreateAndBind(TP_CreateAndBind_cb_common_file, TRUE, TCPConfig.DUT_Port, ipv4);
    sleep(1);
    if (TP_CreateAndBind_result_common_file == RID_E_OK)
    {
        *socketDUT = TP_CreateAndBind_socketID_common_file;
        TP_CreateAndBind_result_common_file = RID_E_NOK;
        printf("\n socket created with success : socketID= %d\n",TP_CreateAndBind_socketID_common_file);
    }
    else
    {
        return Result_NOK;
    }

    stoiIP(TCPConfig.TESTER_IP, ipv4.Data);
    TP_TcpConnect(TP_TcpConnect_cb_common_file, *socketDUT, TCPConfig.Tester_Port, ipv4);

    //sleep(1);

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
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        if (rSYN == 1)
        {
            if (seqN != NULL)
                *seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            if (ackN != NULL)
                *ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
        else
        {
            printf("\n DUT response is not SYN \n");
            return Result_NOK;
        }
    }
    else
    {
        printf("\n DUT response not responding \n");
        return Result_NOK;
    }

    // Verify that the DUT  in SYNSENT state
    if (checkState(SYNSENT))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

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
bool moveClientDUTToEstablished(int *socketDUT, uint16 portDUT, uint16 portTESTER, uint16 maxCon, uint32 *seqN, uint32 *ackN)
{
    // Move DUT to SYN-SENT
    int moveToSYNSENT = moveDUTToSYNSENT(socketDUT, portDUT, portTESTER, maxCon, seqN, ackN);
    if (moveToSYNSENT == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    //send SYN,ACK
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(*ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(*seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //Receive ACK
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            if (seqN != NULL)
                *seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            if (ackN != NULL)
                *ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
        else
        {
            return Result_NOK;
        }
    }
    else
    {
        printf("\n No packet received %s : %d \n", __FILE__, __LINE__);
        return 1;
    }

    // Verify that the DUT in Established state
    if (checkState(ESTABLISHED))
    {
        return Result_OK;
    }
    else
    {
        return Result_NOK;
    }
}

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
bool moveDUTToFINWAIT1(int *socketDUT, uint16 portDUT, uint16 portTESTER, uint16 maxCon, uint32 *seqN, uint32 *ackN)
{
    // Move DUT to ESTABLISHED state
    int moveToClientEstablished = moveClientDUTToEstablished(socketDUT, portDUT, portTESTER, maxCon, seqN, ackN);
    if (moveToClientEstablished == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    // Cause DUT to issue a close() call
    TP_TcpCloseSocket(TP_TcpCloseSocket_cb_common_file, (uint16_t)*socketDUT, FALSE);
    
    int i = 0 ;
    while ((TP_TcpCloseSocket_cb_result_common_file==RID_E_NOK) && (i<20))
    {
        sleep(1);
        i++;
    }

    if (TP_TcpCloseSocket_cb_result_common_file == RID_E_NOK)
    {
        return Result_NOK;
    }
    else
    {
        TP_TcpCloseSocket_cb_result_common_file = RID_E_NOK;
    }
    
    // Receive FIN
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;

    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    i = 0 ;
    while ((TCP_RP.length == 0) && (i<20))
    {
        i++;
        TCP_RP = ListenTCP(f);
    }

    if (TCP_RP.length != 0)
    {
        int rFIN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagFIN);
        if (rFIN == 1)
        {
            if (seqN != NULL)
                *seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            if (ackN != NULL)
                *ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
        else
        {
            return Result_NOK;
        }
    }
    else
    {
        return Result_NOK;
    }

    // Verify that the DUT  in FINWAIT1 state
    if (checkState(FINWAIT1))
    {
        return Result_OK;
    }
    else
    {
        return Result_NOK;
    }
}

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
bool moveDUTToFINWAIT2(int *socketDUT, uint16 portDUT, uint16 portTESTER, uint16 maxCon, uint32 *seqN, uint32 *ackN)
{
    // Move DUT to ESTABLISHED state
    int moveToFINWAIT1 = moveDUTToFINWAIT1(socketDUT, portDUT, portTESTER, maxCon, seqN, ackN);
    if (moveToFINWAIT1 == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    // Send ACK
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(*ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(*seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    
    *ackN = (int)GetTCPField(&TCP_P, TCP, TCP_AckNumber);
    *seqN = (int)GetTCPField(&TCP_P, TCP, TCP_SeqNumber);
    // Verify that the DUT  in FINWAIT2 state
    if (checkState(FINWAIT2))
    {
        return Result_OK;
    }
    else
    {
        return Result_NOK;
    }
}

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
bool moveDUTToClosing(int *socketDUT, uint16 portDUT, uint16 portTESTER, uint16 maxCon, uint32 *seqN, uint32 *ackN)
{
    // Move DUT to fin wait 1 state
    int moveToFINWAIT1 = moveDUTToFINWAIT1(socketDUT, portDUT, portTESTER, maxCon, seqN, ackN);
    if (moveToFINWAIT1 == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\n cannot move top FINWAIT1\n");
        return Result_NOK;
    }

    //send FIN,ACK
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(*ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(*seqN));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //recv ACK
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        // Verify that DUT sends a ACK
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        int rFIN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagFIN);
        if ((rACK == 1) && (rFIN == 0))
        {
            *seqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            *ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
        else
        {
            // Unexpected response
            return Result_NOK;
        }
    }
    else
    {
        // No response sent
        return Result_NOK;
    }
   
    // Verify that the DUT  in CLOSING state
    if (checkState(CLOSING))
    {
        return Result_OK;
    }
    else
    {
        return Result_NOK;
    }
}

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
bool moveDUTToTimeWait(int *socketDUT, uint16 portDUT, uint16 portTESTER, uint16 maxCon, uint32 *seqN, uint32 *ackN)
{
    // Move DUT to fin wait 1 state
    int moveToFINWAIT1 = moveDUTToFINWAIT1(socketDUT, portDUT, portTESTER, maxCon, seqN, ackN);
    if (moveToFINWAIT1 == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    // Send FIN,ACK
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(*ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(*seqN + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // Receive ACK
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;

    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            if (seqN != NULL)
                *seqN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            if (ackN != NULL)
                *ackN = (uint32)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        }
        else
        {
            return Result_NOK;
        }
    }
    else
    {
        return Result_NOK;
    }

    // Verify that the DUT  in TIMEWAIT state
    if (checkState(TIMEWAIT))
    {
        return Result_OK;
    }
    else
    {
        return Result_NOK;
    }
}

int StartTest()
{
    ip4addr ipv4;
    ipv4.dataLength = 4;
    stoiIP(TCPConfig.DUT_IP, ipv4.Data);
    //TP_OpenControlChannel(ipv4, 56000);
    TP_GenaralStartTest(NULL);
    TP_TcpGetState_cb_State_common_file = CLOSED;                   /** < tcp state >*/
    TP_TcpGetState_cb_result_common_file = RID_E_NOK;           /** <the callback result of check state > **/
    TP_CreateAndBind_result_common_file = RID_E_NOK;            /** <the callback result of create and bind > **/
    TP_CreateAndBind_socketID_common_file = -1;                           /**< Socket file description ID created*/
    TP_TcpListenAndAccept_result_common_file = RID_E_NOK;       /** <the callback result of listen and accept > **/
    TP_TcpListenAndAccept_event_result_common_file = RID_E_NOK; /** <the callback result of listen and accept event > **/
    TP_TcpListenAndAccept_event_Server_SocketID_common_file = -1;      /** < Socket server file description ID > **/
    TP_TcpListenAndAccept_event_Client_SocketID_common_file = 6;      /** <Socket client file description ID > **/
    TP_TcpListenAndAccept_event_Client_Port_common_file = -1;          /** <Port client > **/
    TP_TcpConnect_cb_result_common_file = RID_E_NOK;            /** <the callback result of close socket > **/
    TP_TcpCloseSocket_cb_result_common_file = RID_E_NOK;        /** <the callback result of close socket > **/
    TP_TcpCloseSocket_cb_result_common_file_2 = RID_E_NOK;        /** <the callback result of close socket > **/
    TP_GenaralStartTest_cb_result_common_file = RID_E_NOK;      /** <the callback result of start test > **/
    TP_GenaralEndTest_cb_result_common_file = RID_E_NOK;
     
    if (checkState(CLOSED))
    {
        return Result_OK;
    }
    else
    {
        sleep(2*TCPConfig.msl);
    }
    
    if (checkState(CLOSED))
    {
        return Result_OK;
    }
    else
    {
        return Result_NOK;
    } 
}

int EndTest(int SocketID)
{
    if (SocketID != -1)
    {
        TP_TcpCloseSocket(NULL, (uint16_t)SocketID, TRUE);
    }
    sleep(3);
    TP_GenaralEndTest(NULL, 0, (text){0});
    return Result_OK;
}

    //this function use if DUT TCP state ==  Closing, FINWAIT1, FINWAIT2, CLOSEWAIT
int End_Test_Mode_2(int SocketID, int seqN, int ackN)
{
    if (SocketID != -1)
    {
        TP_TcpCloseSocket(NULL, (uint16_t)SocketID, TRUE);
    }
    sleep(3);
    TP_GenaralEndTest(NULL, 0, (text){0});

    //send rst,ack to the dut
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
}

double time_diff(struct timeval x, struct timeval y)
{

    struct timeval time_res;
    time_res.tv_sec = x.tv_sec - y.tv_sec;
    time_res.tv_usec = x.tv_usec - y.tv_usec;
    if (time_res.tv_usec < 0)
    {
        time_res.tv_usec += 1000000;
        time_res.tv_sec -= 1;
    }

    double diff = time_res.tv_sec * 1000000 + time_res.tv_usec;
    return diff;
}

/** \}    end of addtogroup */
