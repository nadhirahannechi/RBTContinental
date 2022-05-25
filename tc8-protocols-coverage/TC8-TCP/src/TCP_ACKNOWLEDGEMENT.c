/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file     TCP_ACKNOWLEDGEMENT.c
 *  \brief    This file contains the tests of TCP ACKNOWLEDGEMENT 
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
#include "TCP_ACKNOWLEDGEMENT.h"
#include"TestabilityProtocol_api.h"

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

int TCP_ACKNOWLEDGEMENT_02()
{

    int socketID;
    uint32 seqN, ackN;

    // start test
    StartTest();
    
    //1. TESTER: Bring DUT to ‘ESTABLISHED’ state.
    int MoveToEstablished = moveClientDUTToEstablished(&socketID, TCPConfig.DUT_Port ,TCPConfig.Tester_Port,  TCPConfig.Maxcon, &seqN, &ackN);

    if (MoveToEstablished == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    //2. TESTER: Send <sendTCPPacket> on the <udpPort> to request a TCP packet from the DUT.
    vint8 data;
    data.Data =(uint8*) calloc(4,sizeof(uint8));
    data.Data=(uint8*)"data";
    data.dataLength=sizeof(data.Data);
    TP_TcpSendData(NULL, socketID, data.dataLength,  0, data);

    //3. DUT: Sends a TCP packet with the expected Sequence Number
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) 
    { 
        int rSeqNum  = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rPSHF  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagPSH);
        int rACKF  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        char* payloadData  = (char *)GetTCPField(&TCP_RP, PAYLAOD, PAYLOAD_data);

        if ((rSeqNum == seqN) && (rPSHF == 1) && (rACKF==1) && (strcmp(payloadData,(char*)data.Data)==0))
        {
            seqN  = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        } 
        else
        {
            printf ("\nTest failed\n");
            //End_Test_Mode_2(socketID,  seqN,  ackN);
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 1;

        }
    }
    else 
    {
	//End_Test_Mode_2(socketID,  seqN,  ackN);
     End_Test_Mode_2(socketID,  seqN,  ackN);
    printf("\nTest failed --> DUT not responding\n");
    return 1;
    }


    //4. TESTER: Send an ACK with a <payload>.
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t) ackN );
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t) (seqN+data.dataLength));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"Data to send");

    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);




    //5. DUT: Accepts the TCP packet and sends an ACK with the expected Akc Number
    TCP_Packet TCP_RP2 = ListenTCP(f);
    if (TCP_RP2.length != 0) 
    { 
        int rACKNum  = (int)GetTCPField(&TCP_RP2, TCP, TCP_AckNumber);
        int rPSHF  = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagPSH);
        int rACKF  = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagACK);
        if ((rACKNum == ackN+sizeof("Data to send")-1)  && (rPSHF==0) && (rACKF==1))
        {
            End_Test_Mode_2(socketID,  seqN,  ackN);
            //End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest passed \n");
            return 0;
        } 
        else
        {
           End_Test_Mode_2(socketID,  seqN,  ackN);
            //End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest failed --> DUT responding not ok\n");
            return 1;
        }
    }
    else 
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
	//End_Test_Mode_2(socketID,  seqN,  ackN);
     
    printf("\nTest failed --> DUT not responding\n");
    return 1;
    }

}

int TCP_ACKNOWLEDGEMENT_03()
{

    int socketID;
    uint32 seqN, ackN;
    // start test
    StartTest();

    //1. TESTER: Bring DUT to ‘ESTABLISHED’ state using a passive socket call. 
    
    int MoveToEstablished = moveClientDUTToEstablished(&socketID, TCPConfig.DUT_Port , TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);

    if (MoveToEstablished == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }
    
    //2. TESTER: Send TCP packet
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagRST, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN + 1));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)0);
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"Data to send");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    //3. DUT: Sends an ACK with the expected Ack Number.

    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) 
    { 
        int rAckNum  = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        if (rAckNum == ackN+2)
        {
            printf ("\nTest passed\n");
            End_Test_Mode_2(socketID,  seqN,  ackN);

            return 0;  
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

 

}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_ACKNOWLEDGEMENT_04
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_ACKNOWLEDGEMENT_04 test.
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
int TCP_ACKNOWLEDGEMENT_04()
{

    int socketID;
    uint32 seqN, ackN;

    // start test
    StartTest();

    //1. TESTER: Bring DUT to ‘ESTABLISHED’ state. 
    
    int MoveToEstablished = moveClientDUTToEstablished(&socketID, TCPConfig.DUT_Port , TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);

    if (MoveToEstablished == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }
    //2. TESTER: Send <sendTCPPacket> to request a TCP packet from the DUT.
    vint8 data;
    data.Data =(uint8*) calloc(4,sizeof(uint8));
    memset(data.Data,0,sizeof("data"));
    data.Data = (uint8*) "data";
    data.dataLength = sizeof(data.Data);


    TP_TcpSendData(NULL, socketID, data.dataLength,  0, data);

    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    // Sniffing and verifying DUT response
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) 
    { 
        int rSeqNum  = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rPSHF  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagPSH);
        int rACKF  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        char* payloadData  = (char *)GetTCPField(&TCP_RP, PAYLAOD, PAYLOAD_data);
        if ((rSeqNum == seqN) && (rPSHF == 1) && (rACKF==1) && (strcmp(payloadData,(char*)data.Data)) == 0)
        {
            seqN  = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            ackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
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


    
    //4. TESTER: Send an ACK with the expected Ack Number, with Length equal to 0.
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t) ackN );
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t) (seqN+data.dataLength));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    
    //5. DUT: Sends no RST and ends the connection correctly when requested to.
    //DUT: Sends no RST  
    TCP_Packet TCP_RP2 = ListenTCP(f);
    if (TCP_RP2.length == 0) 
    { 
        //result ok  
    }
    else 
    {
        int rRST  = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagRST);
        if (rRST == 1)
        {
            printf("\nTest failed --> DUT  responding\n");
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 1;
        }
        else 
        {
            // result ok 
        }
    }
    
    
    //ends the connection correctly
    TCP_Packet TCP_P_1 = CreateTCP();
    EditTCPField(&TCP_P_1, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P_1, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P_1, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P_1, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P_1, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P_1, TCP, TCP_AckNumber, (void *)(intptr_t)seqN);
    TCP_Compute_checksum(&TCP_P_1);
    SendTCP(TCP_P_1);

    // 3. DUT: Send ACK packet
    // Receive DUT responses
    TCP_Packet TCP_RP_1 = ListenTCP(f);
    if (TCP_RP_1.length != 0)
    {
        // Verify that DUT sends a ACK
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            // Test OK
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not ACK \n");
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

    // Sends a FIN packet and moves to ‘Last ACK’ state.
    TP_TcpCloseSocket(NULL, (uint16)socketID, false);
    
    TCP_Packet TCP_RP_2 = ListenTCP(f);
    if (TCP_RP_2.length != 0)
    {
        // Verify that DUT sends a ACK
        int rACK = (int)GetTCPField(&TCP_RP_2, TCP, TCP_FlagACK);
        int rFIN= (int)GetTCPField(&TCP_RP_2, TCP, TCP_FlagFIN);
        if ((rACK == 1) && (rFIN == 1))
        {
            seqN = (int)GetTCPField(&TCP_RP_2, TCP, TCP_SeqNumber);
            ackN = (int)GetTCPField(&TCP_RP_2, TCP, TCP_AckNumber );
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not FIN  \n");
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 1;
        }
    }
    else
    {
        // No response sent
        printf("\nTest failed --> DUT not responding \n");
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }
    
    //send an ACK packet
    TCP_Packet TCP_P_2 = CreateTCP();
    EditTCPField(&TCP_P_2, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P_2, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P_2, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P_2, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P_2, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN+1));
    TCP_Compute_checksum(&TCP_P_2);
    SendTCP(TCP_P_2);
    
    // externally check the state of the DUT.
    if(checkState(CLOSED))
    {
        printf("\nTest passed in %s : %d\n", __FILE__, __LINE__);
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in closed state in %s : %d\n", __FILE__, __LINE__);
        End_Test_Mode_2(socketID,  seqN,  ackN);
        return 1;
    }

}

