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
#include "TCP_RETRANSMISSION_TO.h"
#include <sys/time.h>
#include <time.h>
#include "TestabilityProtocol_api.h"
/******************************************************************************
* DEFINES AND MACROS
******************************************************************************/

/******************************************************************************
* GLOBAL VARIABLES AND CONSTANTS
******************************************************************************/
TP_ResultID_t TP_CreateAndBind_result_RTO = RID_E_NOK;            /** <the callback result of create and bind > **/
uint16 TP_CreateAndBind_socketID_RTO = -1;                           /**< Socket file description ID created*/
TP_ResultID_t TP_TcpConnect_cb_result_RTO = RID_E_NOK;            /** <the callback result of close socket > **/

/******************************************************************************
* LOCAL FUNCTION PROTOTYPES
******************************************************************************/
void TP_CreateAndBind_cb_RTO(TP_ResultID_t result, uint16 sock);
void TP_TcpConnect_cb_RTO(TP_ResultID_t result);
/******************************************************************************
* Local FUNCTIONS
******************************************************************************/

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

void TP_CreateAndBind_cb_RTO(TP_ResultID_t result, uint16 sock)
{
    TP_CreateAndBind_result_RTO = result;
    TP_CreateAndBind_socketID_RTO = sock;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TP_TcpConnect_cb_RTO
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
void TP_TcpConnect_cb_RTO(TP_ResultID_t result)
{
    TP_TcpConnect_cb_result_RTO = result;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_RETRANSMISSION_TO_03
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_RETRANSMISSION_TO_03 test.
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
int TCP_RETRANSMISSION_TO_03()
{
    //start test
    StartTest();
	// 1. TESTER: Cause the DUT to move on to ESTABLISHED state
    int socketID;
    uint32 seqN, ackN;
    int moveClientDUTToEstablishedResult = moveClientDUTToEstablished(&socketID, TCPConfig.DUT_Port ,TCPConfig.Tester_Port,  TCPConfig.Maxcon, &seqN, &ackN);
     if (moveClientDUTToEstablishedResult == Result_OK)
    {
        //misra
    }
    else
    {
        printf("\nTest failed --> Cannot move to Client Established state\n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return Result_NOK;
    }
    // 2. TESTER: Cause an application on the DUT-side to issue a data SEND request
    vint8 data;
    data.Data =(uint8*) calloc(4,sizeof(uint8));
    data.Data=(uint8*)"data";
    data.dataLength = 4;
    TP_TcpSendData(NULL, socketID, data.dataLength, 0, data);

    // 3. DUT: Send the data segment
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    uint32 rSeqNumber, rAckNumber;
    struct timeval transTime1;
    gettimeofday(&transTime1, NULL);
    if (TCP_RP.length != 0)
    {
        // Verify that DUT response is PSH
        rSeqNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        rAckNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int rPSH = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagPSH);
        if (rPSH == 1)
        {
            // Test OK
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not a data segment\n");
            End_Test_Mode_2(socketID,seqN, ackN);  
            return 1;
        }
    }
    else
    {
        // No response sent
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketID,seqN, ackN);  
        return 1;
    }

    // 4. TESTER: Do not send the ACK and cause the retransmission timer on the DUT side to expire
    // 5. DUT: Send the data segment once more
    
    TCP_Packet TCP_RP_RTO;
    int PendingListen = 0;
    // pending 100 times
    while((TCP_RP_RTO.length == 0) && (PendingListen<100))
    {
        TCP_RP_RTO = ListenTCP(f);
        PendingListen++;
    }
    
    struct timeval retransTime1;
    gettimeofday(&retransTime1, NULL);
    if (TCP_RP_RTO.length != 0)
    {
        // Verify that DUT response is PSH
        uint32 rrSeqNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rPSH = (int)GetTCPField(&TCP_RP_RTO, TCP, TCP_FlagPSH);
        if ((rPSH == 1) && (rSeqNumber == rrSeqNumber))
        {
            gettimeofday(&retransTime1, NULL);
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not a data segment\n");
            End_Test_Mode_2(socketID,seqN, ackN);  
            return 1;
        }
    }
    else
    {
        // No response sent
        printf("\nTest failed --> DUT not retransmitting\n");
        End_Test_Mode_2(socketID,seqN, ackN);  
        return 1;
    }

    // 6. TESTER: Note the retransmission time (RTO)
    double RTO1 = time_diff(retransTime1, transTime1);

    // 7. TESTER: Send an ACK just before the RTO occurs on DUT
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)rAckNumber);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(rSeqNumber + 4));
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 8. TESTER: Cause the application to issue SEND request for one more data segment
    TP_TcpSendData(NULL, socketID, data.dataLength, 0, data);

    // 9. DUT: Send the data segment
    TCP_RP = ListenTCP(f);
    struct timeval transTime2;
    gettimeofday(&transTime2, NULL);
    if (TCP_RP.length != 0)
    {
        // Verify that DUT response is PSH
        rSeqNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rPSH = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagPSH);
        if (rPSH == 1)
        {
            // Test OK
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not a data segment\n");
            End_Test_Mode_2(socketID,seqN, ackN);  

            return 1;
        }
    }
    else
    {
        // No response sent
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketID,seqN, ackN);  
        return 1;
    }

    // 10. TESTER: Do not send ACK
    // 11. DUT: Retransmit the data segment

    TCP_Packet TCP_RP_RTO_2;
    TCP_RP_RTO_2 = ListenTCP(f);
    int PendingListen2 = 0;
    // pending 100 times
    while((TCP_RP_RTO_2.length == 0) && (PendingListen2<100))
    {
        TCP_RP_RTO_2 = ListenTCP(f);
        PendingListen2++;
    }
    
    struct timeval retransTime2;


    if (TCP_RP_RTO_2.length != 0)
    {
        // Verify that DUT response is PSH
        uint32 rrSeqNumber = (int)GetTCPField(&TCP_RP_RTO_2, TCP, TCP_SeqNumber);
        int rPSH = (int)GetTCPField(&TCP_RP_RTO_2, TCP, TCP_FlagPSH);
        if ((rPSH == 1) && (rSeqNumber == rrSeqNumber))
        {
            gettimeofday(&retransTime2, NULL);

        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not a data segment\n");
            End_Test_Mode_2(socketID,seqN, ackN);  
            return 1;
        }
    }
    else
    {
        // No response sent
        printf("\nTest failed --> DUT not retransmitting 2 \n");
        End_Test_Mode_2(socketID,seqN, ackN);  
        return 1;
    }

    // 12. TESTER: Verify that the retransmission timer value is twice the value of RTO noted earlier(exponential backoff only)

    double RTO2 = time_diff(retransTime2, transTime2);

    if(RTO2 > 2*RTO1)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,seqN, ackN);  
        return 0;
    }
    else
    {
        printf("\nTest failed --> RTO values are not consistent\n");
        End_Test_Mode_2(socketID,seqN, ackN);  
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_RETRANSMISSION_TO_04
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_RETRANSMISSION_TO_04 test.
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
int TCP_RETRANSMISSION_TO_04()
{
    //start test
    StartTest();
	// 1. TESTER: Cause the DUT to move on to ESTABLISHED state
	int socketID;
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
    // 2. TESTER: Cause an application on the DUT-side to issue a data SEND request
    vint8 data;
    data.Data = (uint8*) calloc(4,sizeof(uint8));
    data.Data = (uint8*)"data";
    data.dataLength = 4;
    TP_TcpSendData(NULL, socketID, data.dataLength, 0, data);

    // 3. DUT: Send the data segment
    double RTO1, RTO2, RTO3;
    for (int i=1; i<=3; i++)
    {
        Packet_filter f;
        strcpy(f.SrcAdd, TCPConfig.DUT_IP);
        strcpy(f.dstAdd, TCPConfig.TESTER_IP);
        f.Srcport = TCPConfig.DUT_Port;
        f.Dstport = TCPConfig.Tester_Port;
        uint32 rSeqNumber;
        struct timeval transTime;
        gettimeofday(&transTime, NULL);

        TCP_Packet TCP_RP ;
        TCP_RP.length = 0;
        int PendingListen = 0;
        // pending 100 times
        while((TCP_RP.length == 0)  && (PendingListen<100))
        {
            TCP_RP = ListenTCP(f);
            PendingListen++;
        }
    
        if (TCP_RP.length != 0)
        {
            // Verify that DUT response is PSH
            rSeqNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            int rPSH = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagPSH);
            if (rPSH == 1)
            {
                // Test OK
            }
            else
            {
                // Unexpected response
                printf("\nTest failed --> DUT response is not a data segment\n");
                End_Test_Mode_2(socketID,seqN, ackN);  
                return 1;
            }
        }
        else
        {
            // No response sent
            printf("\nTest failed --> DUT not responding\n");
            End_Test_Mode_2(socketID,seqN, ackN);  
            return 1;
        }

        // 4. TESTER: Do not send ACK
        // 5. DUT: Retransmit the data segment after a timeout
        TCP_RP.length = 0;
        //TCP_RP = ListenTCP(f);
        PendingListen=0;
        while((TCP_RP.length == 0) && (PendingListen<100))
        {
            TCP_RP = ListenTCP(f);
            PendingListen++;
            
        }
        
        struct timeval retransTime;
        if (TCP_RP.length != 0)
        {
            // Verify that DUT response is PSH
            uint32 rrSeqNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            int rPSH = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagPSH);
            if ((rPSH == 1) && (rSeqNumber == rrSeqNumber))
            {
                gettimeofday(&retransTime, NULL);
            }
            else
            {
                // Unexpected response
                printf("\nTest failed --> DUT response is not a data segment\n");
                End_Test_Mode_2(socketID,seqN, ackN);
                return 1;
            }
        }
        else
        {
            // No response sent
            printf("\nTest failed --> DUT not retransmitting\n");
            End_Test_Mode_2(socketID,seqN, ackN);End_Test_Mode_2(socketID,seqN, ackN);
            return 1;
        }

        if (i == 1) RTO1 = time_diff(retransTime,transTime);
        if (i == 2) RTO2 = time_diff(retransTime,transTime);
        if (i == 3) RTO3 = time_diff(retransTime,transTime);
        transTime = retransTime;
    }

    //6. TESTER: Do not send ACK and verify that RTO retransmission interval is increasing at a fast (at least more than linear) rate

    // estimation for linear RTO from the first two RTO values
    uint32 slope = RTO2 - RTO1;
    uint32 linearRTO = (2 * slope) + RTO1;

    // Compare between RTO3 value and estimated RTO by linear approximation
    if(RTO3 > linearRTO)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,seqN, ackN);  
        return 0;
    }
    else
    {
        printf("\nTest failed --> RTO values does not follow an exponential backoff\n");
        End_Test_Mode_2(socketID,seqN, ackN);  
        return 1;
    }

}
/***************************************************************************************************
*
*   FUNCTION NAME: TCP_RETRANSMISSION_TO_05
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_RETRANSMISSION_TO_05 test.
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
int TCP_RETRANSMISSION_TO_05()
{
	int socketID;
	uint32 seqN,ackN;
    // Start Test
    StartTest();

	// 1. TESTER: Cause the application on the DUT-side to issue an active OPEN call
	ip4addr ipv4;
    ipv4.dataLength = 4;
    stoiIP(TCPConfig.DUT_IP, ipv4.Data);
    // Cause DUT to issue an active open call
    TP_TcpCreateAndBind(TP_CreateAndBind_cb_RTO, TRUE, TCPConfig.DUT_Port, ipv4);
    sleep(1);
    if (TP_CreateAndBind_result_RTO == RID_E_OK)
    {
        socketID = TP_CreateAndBind_socketID_RTO;
        TP_CreateAndBind_result_RTO = RID_E_NOK;
        printf("\n socket created with success : socketID= %d\n",TP_CreateAndBind_socketID_RTO);
    }
    else
    {
        return Result_NOK;
    }

    stoiIP(TCPConfig.TESTER_IP, ipv4.Data);
    TP_TcpConnect(TP_TcpConnect_cb_RTO, socketID, TCPConfig.Tester_Port, ipv4);

    // 2. DUT: Send a SYN
    // Receive DUT responses
    struct timeval transTime;
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0) 
    {
        
        // Verify that DUT sends a SYN
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        seqN=(int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        ackN=(int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        if (rSYN == 1)
        {
            gettimeofday(&transTime, NULL);

        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not SYN\n");
            End_Test_Mode_2(socketID,seqN, ackN);
            return 1;
        }
    }
    else
    {
        // DUT nor reponding
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }

    // 3. TESTER: Do not send SYN,ACK
    // 4. DUT: Retransmit the SYN segment after a timeout

    double RTO1, RTO2, RTO3;
    for(int i = 1; i <= 3; i++)
    {
        Packet_filter f;
        strcpy(f.SrcAdd, TCPConfig.DUT_IP);
        strcpy(f.dstAdd, TCPConfig.TESTER_IP);
        f.Srcport = TCPConfig.DUT_Port;
        f.Dstport = TCPConfig.Tester_Port;
        TCP_Packet TCP_RP = ListenTCP(f);

        int PendingListen = 0;
        // pending 100 times
        while((TCP_RP.length == 0) && (PendingListen<100))
        {
            TCP_RP = ListenTCP(f);
            PendingListen++;
        }
        
        struct timeval retransTime;
        gettimeofday(&retransTime, NULL);
        
        if (TCP_RP.length != 0)
        {
            // Verify that DUT sends a SYN
            int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
            seqN=(int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN=(int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            if (rSYN == 1)
            {
                //Test ok
            }
            else
            {
                // Unexpected response
                printf("\nTest failed --> DUT response is not SYN\n");
                End_Test_Mode_2(socketID,seqN, ackN);
                return 1;
            }
        }
        else
        {
            // DUT nor reponding
            printf("\nTest failed --> DUT not responding\n");
            End_Test_Mode_2(socketID,seqN, ackN);
            return 1;
        }
        // Compute RTO for each retransmission
        if (i == 1) RTO1 = time_diff(retransTime,transTime);
        if (i == 2) RTO2 = time_diff(retransTime,transTime);
        if (i == 3) RTO3 = time_diff(retransTime,transTime);
        transTime = retransTime;
    }

    // 5. TESTER: Do not send SYN,ACK and verify that RTO retransmission interval is increasing at a fast (at least more than linear) rate

    // estimation for linear RTO from the first two RTO values
    uint32 slope = RTO2 - RTO1;
    uint32 linearRTO = (2 * slope) + RTO1;

    // Compare between RTO3 value and estimated RTO by linear approximation
    if(RTO3 > linearRTO)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,seqN, ackN);        
        return 0;
    }
    else
    {
        printf("\nTest failed --> RTO values does not follow an exponential backoff\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }


}
/***************************************************************************************************
*
*   FUNCTION NAME: TCP_RETRANSMISSION_TO_06
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_RETRANSMISSION_TO_06 test.
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
int TCP_RETRANSMISSION_TO_06()
{
   
	int socketID;
	uint32 seqN,ackN;
    // Start Test
    StartTest();

	// 1. TESTER: Cause the application on the DUT-side to issue an active OPEN call
	ip4addr ipv4;
    ipv4.dataLength = 4;
    stoiIP(TCPConfig.DUT_IP, ipv4.Data);
    // Cause DUT to issue an active open call
    TP_TcpCreateAndBind(TP_CreateAndBind_cb_RTO, TRUE, TCPConfig.DUT_Port, ipv4);
    sleep(1);
    if (TP_CreateAndBind_result_RTO == RID_E_OK)
    {
        socketID = TP_CreateAndBind_socketID_RTO;
        TP_CreateAndBind_result_RTO = RID_E_NOK;
        printf("\n socket created with success : socketID= %d\n",TP_CreateAndBind_socketID_RTO);
    }
    else
    {
        return Result_NOK;
    }

    stoiIP(TCPConfig.TESTER_IP, ipv4.Data);
    TP_TcpConnect(TP_TcpConnect_cb_RTO, socketID, TCPConfig.Tester_Port, ipv4);


    // 2. DUT: Send a SYN
    // Receive DUT responses
    struct timeval transTime,retransTime;
    
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0)
    {
        gettimeofday(&transTime, NULL);
        // Verify that DUT sends a SYN
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        seqN=(int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        ackN=(int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        if (rSYN == 1)
        {
            //Test OK
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not SYN\n");
            End_Test_Mode_2(socketID,seqN, ackN);
            return 1;
        }
    }
    else
    {
        // DUT nor reponding
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }

    // 3. TESTER: Do not send SYN,ACK
    // 4. DUT: Retransmit the SYN segment after a timeout

    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_RP = ListenTCP(f);
    int PendingListen = 0;
    // pending 100 times
    while((TCP_RP.length == 0) && (PendingListen<100))
    {
        TCP_RP = ListenTCP(f);
        PendingListen++;
    }
        
    
    if (TCP_RP.length != 0)
    {
        gettimeofday(&retransTime, NULL);
        // Verify that DUT sends a SYN
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        seqN=(int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        ackN=(int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        if (rSYN == 1)
        {
            //Test ok
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not SYN\n");
            End_Test_Mode_2(socketID,seqN, ackN);
            return 1;
        }
    }
    else
    {
        // DUT nor reponding
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }
    // Compute RTO
    double RTO = time_diff(retransTime, transTime);

    if ((RTO > 900000000) && (RTO < 1100000000))
    {
        printf("\nTest passed %d \n",RTO);
        End_Test_Mode_2(socketID,seqN, ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> RTO does not follow RFC 6298 %d\n");
        
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }


}
/***************************************************************************************************
*
*   FUNCTION NAME: TCP_RETRANSMISSION_TO_08
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_RETRANSMISSION_TO_08 test.
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
int TCP_RETRANSMISSION_TO_08()
{
   //start test
    StartTest();
	
   // 1. TESTER: Cause the DUT to move on to ESTABLISHED state
   int socketID;
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
    // 2. TESTER: Cause an application on the DUT-side to issue a data SEND request
    vint8 data;
    data.Data = (uint8*) calloc(4,sizeof(uint8));
    data.Data = (uint8*)"data";
    data.dataLength = 4;
    TP_TcpSendData(NULL, socketID, data.dataLength, 0, data);
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0)
    {
        // Verify that DUT response is PSH
        int rPSH = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagPSH);
        if (rPSH == 1)
        {
            // Test OK
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not a data segment\n");
            End_Test_Mode_2(socketID,seqN, ackN);  
            return 1;
        }
    }
    else
    {
        // No response sent
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketID,seqN, ackN);  
        return 1;
    }
    
    //sleep 2* MSL
    printf("\n sleep for 2*MSL= %d s \n" ,2*TCPConfig.msl);
    sleep(2*TCPConfig.msl);
    // 3. DUT: Send the data segment
    double RTO1, RTO2;
    for (int i=1; i<=2; i++)
    {
        
        uint32 rSeqNumber;
        struct timeval transTime;
        gettimeofday(&transTime, NULL);

        TCP_Packet TCP_RP ;
        TCP_RP.length = 0;
        int PendingListen = 0;
        // pending 100 times
        while((TCP_RP.length == 0)  && (PendingListen<500))
        {
            TCP_RP = ListenTCP(f);
            PendingListen++;
        }
    
        if (TCP_RP.length != 0)
        {
            // Verify that DUT response is PSH
            rSeqNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            int rPSH = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagPSH);
            if (rPSH == 1)
            {
                // Test OK
            }
            else
            {
                // Unexpected response
                printf("\nTest failed --> DUT response is not a data segment\n");
                End_Test_Mode_2(socketID,seqN, ackN);  
                return 1;
            }
        }
        else
        {
            // No response sent
            printf("\nTest failed --> DUT not responding\n");
            End_Test_Mode_2(socketID,seqN, ackN);  
            return 1;
        }

        // 4. TESTER: Do not send ACK
        // 5. DUT: Retransmit the data segment after a timeout
        TCP_RP.length = 0;
        //TCP_RP = ListenTCP(f);
        PendingListen=0;
        while((TCP_RP.length == 0) && (PendingListen<500))
        {
            TCP_RP = ListenTCP(f);
            PendingListen++;
            
        }
        
        struct timeval retransTime;
        if (TCP_RP.length != 0)
        {
            // Verify that DUT response is PSH
            uint32 rrSeqNumber = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            int rPSH = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagPSH);
            if ((rPSH == 1) && (rSeqNumber == rrSeqNumber))
            {
                gettimeofday(&retransTime, NULL);
            }
            else
            {
                // Unexpected response
                printf("\nTest failed --> DUT response is not a data segment\n");
                End_Test_Mode_2(socketID,seqN, ackN);
                return 1;
            }
        }
        else
        {
            // No response sent
            printf("\nTest failed --> DUT not retransmitting\n");
            End_Test_Mode_2(socketID,seqN, ackN);End_Test_Mode_2(socketID,seqN, ackN);
            return 1;
        }

        if (i == 1) RTO1 = time_diff(retransTime,transTime);
        if (i == 2) RTO2 = time_diff(retransTime,transTime);
        transTime = retransTime;
    }

    //6. TESTER: Do not send ACK and verify that RTO retransmission interval is increasing at a fast (at least more than linear) rate



    // Compare between RTO3 value and estimated RTO by linear approximation
    if(RTO2 = RTO1)
    {
        printf("\nTest passed\n");
        End_Test_Mode_2(socketID,seqN, ackN);  
        printf(" \n %d =!= %d \n",RTO1,RTO2);

        return 0;
    }
    else
    {
        printf("\nTest failed --> RTO values does not follow an exponential backoff\n");
        End_Test_Mode_2(socketID,seqN, ackN);  
        printf(" \n %d =!= %d \n",RTO1,RTO2);

        return 1;
    }
}
/***************************************************************************************************
*
*   FUNCTION NAME: TCP_RETRANSMISSION_TO_09
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_RETRANSMISSION_TO_09 test.
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
int TCP_RETRANSMISSION_TO_09()
{
   int socketID;
	uint32 seqN,ackN;
    // Start Test
    StartTest();

	// 1. TESTER: Cause the application on the DUT-side to issue an active OPEN call
	ip4addr ipv4;
    ipv4.dataLength = 4;
    stoiIP(TCPConfig.DUT_IP, ipv4.Data);
    // Cause DUT to issue an active open call
    TP_TcpCreateAndBind(TP_CreateAndBind_cb_RTO, TRUE, TCPConfig.DUT_Port, ipv4);
    sleep(1);
    if (TP_CreateAndBind_result_RTO == RID_E_OK)
    {
        socketID = TP_CreateAndBind_socketID_RTO;
        TP_CreateAndBind_result_RTO = RID_E_NOK;
        printf("\n socket created with success : socketID= %d\n",TP_CreateAndBind_socketID_RTO);
    }
    else
    {
        return Result_NOK;
    }

    stoiIP(TCPConfig.TESTER_IP, ipv4.Data);
    TP_TcpConnect(TP_TcpConnect_cb_RTO, socketID, TCPConfig.Tester_Port, ipv4);

    // 2. DUT: Send a SYN
    // Receive DUT responses
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);

    if (TCP_RP.length != 0) 
    {
        
        // Verify that DUT sends a SYN
        int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
        seqN=(int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        ackN=(int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        if (rSYN == 1)
        {

        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not SYN\n");
            End_Test_Mode_2(socketID,seqN, ackN);
            return 1;
        }
    }
    else
    {
        // DUT nor reponding
        printf("\nTest failed --> DUT not responding\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        return 1;
    }

    // 3. TESTER: Do not send SYN,ACK
    // 4. DUT: Retransmit the SYN segment after a timeout
    
    //sleep 2* MSL
    printf("\n sleep for 2*MSL= %d s \n" ,2*TCPConfig.msl);
    sleep(2*TCPConfig.msl);
    double RTO1, RTO2;
    for(int i = 1; i <= 3; i++)
    {
        
        struct timeval transTime;
        gettimeofday(&transTime, NULL);
        TCP_RP.length = 0;
        int PendingListen = 0;
        // pending 100 times
        while((TCP_RP.length == 0) && (PendingListen<500))
        {
            TCP_RP = ListenTCP(f);
            PendingListen++;
        }


        
        if (TCP_RP.length != 0)
        {
            // Verify that DUT sends a SYN
            int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
            seqN=(int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN=(int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            if (rSYN == 1)
            {
                //Test ok
            }
            else
            {
                // Unexpected response
                printf("\nTest failed --> DUT response is not SYN\n");
                End_Test_Mode_2(socketID,seqN, ackN);
                return 1;
            }
        }
        else
        {
            // DUT nor reponding
            printf("\nTest failed --> DUT not responding\n");
            End_Test_Mode_2(socketID,seqN, ackN);
            return 1;
        }
        
        //second frame
        PendingListen = 0;
        TCP_RP.length = 0;
        // pending 100 times
        while((TCP_RP.length == 0) && (PendingListen<500))
        {
            TCP_RP = ListenTCP(f);
            PendingListen++;
        }

        struct timeval retransTime;
        gettimeofday(&retransTime, NULL);
         
        if (TCP_RP.length != 0)
        {
            // Verify that DUT sends a SYN
            int rSYN = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagSYN);
            seqN=(int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN=(int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            if (rSYN == 1)
            {
                //Test ok
            }
            else
            {
                // Unexpected response
                printf("\nTest failed --> DUT response is not SYN\n");
                End_Test_Mode_2(socketID,seqN, ackN);
                return 1;
            }
        }
        else
        {
            // DUT nor reponding
            printf("\nTest failed --> DUT not responding\n");
            End_Test_Mode_2(socketID,seqN, ackN);
            return 1;
        }
        
        // Compute RTO for each retransmission
        if (i == 1) RTO1 = time_diff(retransTime,transTime);
        if (i == 2) RTO2 = time_diff(retransTime,transTime);
        transTime = retransTime;
    }

    // 5. TESTER: Do not send SYN,ACK and verify that RTO retransmission interval is increasing at a fast (at least more than linear) rate


    // Compare between RTO3 value and estimated RTO by linear approximation
    if(RTO2 == RTO1)
    {
        printf("\nTest passed\n"); 
        End_Test_Mode_2(socketID,seqN, ackN);
        printf(" \n %d =!= %d \n",RTO1,RTO2);
     
        return 0;
    }
    else
    {
        printf("\nTest failed --> RTO values does not follow an exponential backoff\n");
        End_Test_Mode_2(socketID,seqN, ackN);
        printf(" \n %d =!= %d \n",RTO1,RTO2);

        return 1;
    }

}
