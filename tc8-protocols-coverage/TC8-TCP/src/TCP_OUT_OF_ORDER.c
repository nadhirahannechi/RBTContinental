/**
 *  Copyright (C) [year] Focus Corporation
 *
 *  \file     TCP_OUT_OF_ORDER.c
 *  \brief    This file contains the tests of TCP OUT OF ORDER
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
#include "TestabilityProtocol_api.h"
#include <sys/time.h>
#include "TCP_OUT_OF_ORDER.h"

/******************************************************************************
* DEFINES AND MACROS
******************************************************************************/

/******************************************************************************
* GLOBAL VARIABLES AND CONSTANTS
******************************************************************************/
uint32 seqN_OUT_OF_ORDER_02, ackN_OUT_OF_ORDER_02;
int syncThread1=0,syncThread2=0;


/******************************************************************************
* Local FUNCTIONS
******************************************************************************/
void * Send_TCP1 (void *)
{
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN_OUT_OF_ORDER_02));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN_OUT_OF_ORDER_02));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "data to send 1 ");
    TCP_Compute_checksum(&TCP_P);
    while(syncThread1==0);
    syncThread2=1;
    SendTCP(TCP_P);
}

void * Send_TCP2 (void *)
{
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN_OUT_OF_ORDER_02+sizeof("data to send 1 ") - 1));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN_OUT_OF_ORDER_02));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "data to send 2");
    TCP_Compute_checksum(&TCP_P);
    syncThread1=1;
    while(syncThread2==0);
    SendTCP(TCP_P);
}
/***************************************************************************************************
*
*   FUNCTION NAME: TCP_OUT_OF_ORDER_01
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_OUT_OF_ORDER_01 test.
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
int TCP_OUT_OF_ORDER_01()
{
    //start test
    //TP_OpenControlChannel();
    StartTest();

    //1. TESTER: Cause the DUT to move on to ESTABLISHED state at a <wnp> 
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
	
    //2. TESTER: Send a full-sized segment
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    uint8_t * MAX_Size_Payload= (uint8_t*)malloc((TCPConfig.mss) * sizeof(uint8_t));
    memset(MAX_Size_Payload,0,(TCPConfig.mss));
    memset(MAX_Size_Payload,1,(TCPConfig.mss-1));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) MAX_Size_Payload);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    struct timeval stop, start;
    gettimeofday(&start, NULL);

    free(MAX_Size_Payload);

    //3. DUT: Send an ACK after some time
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) 
    {
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if  (rACK == 1)
        {
            //4. TESTER: Verify that the delay in receiving the ACK (subtracting the average round-trip-time) is less than 0.5 sec 
            gettimeofday(&stop, NULL);
            double deltaT_us = time_diff(stop,start);
            //500000 us = 0.5s
            if (deltaT_us<500000)
            {
                printf("\nTest passed \n",deltaT_us);
                End_Test_Mode_2(socketID,  seqN,  ackN);
                return 0;
            }
            else
            {
                End_Test_Mode_2(socketID,  seqN,  ackN);
                printf("\nTest failed --> the delay in receiving the ACK > 0.5s\n");
                return 1;
            }

        }
        else 
        {
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest failed --> response not ok\n");
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
*   FUNCTION NAME: TCP_OUT_OF_ORDER_02
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_OUT_OF_ORDER_02 test.
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

int TCP_OUT_OF_ORDER_02()
{
    //start test
    StartTest();

    //1. TESTER: Cause the DUT to move on to ESTABLISHED state at a <wnp> 
    int socketID, socketTester;
    uint32 seqN, ackN;
    int moveServDUTToEstablishedResult = moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, &seqN_OUT_OF_ORDER_02, &ackN_OUT_OF_ORDER_02);
    if (moveServDUTToEstablishedResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }
	
    //2. TESTER: Send two data segments consecutively without any delay
    
    //prepare two data segments
    pthread_t th1, th2;
    syncThread1=0;
    syncThread2=0;
    pthread_create(&th1, NULL, Send_TCP1, NULL);
    pthread_create(&th2, NULL, Send_TCP2, NULL);
    
    struct timeval stop, start;
    gettimeofday(&start, NULL);


    //3. DUT: Send an ACK after some time
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP_1 = ListenTCP(f);
    TCP_Packet TCP_RP_2 = ListenTCP(f);
    gettimeofday(&stop, NULL);

    if ( (TCP_RP_1.length != 0) && (TCP_RP_2.length ==0) )
    {
        int rACK;

        //check ACK flag
        rACK = (int)GetTCPField(&TCP_RP_1, TCP, TCP_FlagACK);

        if  (rACK == 1)
        {
            //4. TESTER: Verify that the delay in receiving the ACK (subtracting the average round-trip-time) is less than 0.5 sec 
            double deltaT_us = time_diff(stop, start);
            //500000 us = 0.5s
            if (deltaT_us<500000)
            {
                printf("\nTest passed\n");
                End_Test_Mode_2(socketID,  seqN,  ackN);
                return 0;
            }
            else
            {
                TP_GenaralEndTest(NULL,0,(text){0});
                End_Test_Mode_2(socketID,  seqN,  ackN);
                return 1;
            }

        }
        else 
        {
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest failed --> response not ok\n");
            return 1;  
        }
    }
    
    else if ( (TCP_RP_1.length != 0) && (TCP_RP_2.length !=0) )
    {
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest failed --> DUT: Send two ACK \n");
            return 1; 
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
*   FUNCTION NAME: TCP_OUT_OF_ORDER_03
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_OUT_OF_ORDER_03 test.
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
int TCP_OUT_OF_ORDER_03()
{
    //TP_OpenControlChannel();
    StartTest();

    //socket ID of DUT
    int socketID,socketTester;

    // list of sequence number and ack number of all segments 
    uint32 *seqN, ackN,ackN1;
    seqN = (uint32 *)malloc(TCPConfig.nss+1*sizeof(uint32));

    //1. TESTER: Cause the DUT to move on to ESTABLISHED state at a <wnp> 
    int moveServDUTToEstablishedResult = moveServDUTToEstablished(&socketID, &socketTester, TCPConfig.DUT_Port, TCPConfig.Maxcon, seqN, &ackN);
    if (moveServDUTToEstablishedResult == Result_OK)
    {
        //misra
    }
    else
    {
        return Result_NOK;
    }

    //2. TESTER: Send a data segment
    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN[0]));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) "Data to send");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    

    //3. DUT: Send the ACK for the data segment
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_Packet TCP_RP = ListenTCP(f);
    if (TCP_RP.length != 0) 
    {
        int RackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
        int RSeqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
        int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
        if ((RSeqN== ackN) && (rACK==1))
        {
            printf("\nresult ok : DUT sent the ACK segment 1 with correct sequence number \n");
            seqN[0]=RackN;
        }
        else
        {
            End_Test_Mode_2(socketID,  seqN,  ackN);
            printf("\nTest failed : DUT dID not send the ACK segment \n");
            return 1;
        }
    }
    else 
    {
        End_Test_Mode_2(socketID,  seqN,  ackN);
        printf("\nTest failed --> DUT not responding\n");
        return 1;
    }

    //4. TESTER: Send <nss> number of data segments leaving a gap in SEQ numbers (as if a segment is skipped) at the beginning
    for (int i=1;i<TCPConfig.nss-1;i++)
    {
        seqN[i]=seqN[i-1]+19;
        //2. TESTER: Send a full-sized segment
        TCP_P = CreateTCP();
        EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
        EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
        EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
        EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
        EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN[i]));
        EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
        uint8_t* P_DATA= (uint8_t*)malloc(20 * sizeof(uint8_t));
        memset(P_DATA,0,20);
        memset(P_DATA,i+1,19);
        EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) P_DATA);
        TCP_Compute_checksum(&TCP_P);
        SendTCP(TCP_P);
        free(P_DATA);

        //3. DUT: Send an ACK after some time

        TCP_RP = ListenTCP(f);
        if (TCP_RP.length == 0) 
        {
           //result ok 
            printf("\nresult ok : DUT dID not send the ACK segment %d \n",i+1);

        }
        else 
        {
            
            int RSeqN = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            int RackN = (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            if ((ackN == RSeqN) && ( RackN == seqN[i]+19))
            {
                End_Test_Mode_2(socketID,  seqN,  ackN);
                printf("\nTest failed : DUT sent the ACK segment % \n",i+1);
                return 1;
                 
            }
            else
            {
               printf("\nresult ok : DUT dID not send the ACK segment %d \n",i+1);
            }
        }
    }


    TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN[0]));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN));
    uint8_t* Payload_DATA= (uint8_t*)malloc((20) * sizeof(uint8_t));
    memset(Payload_DATA,0,20);
    memset(Payload_DATA,1,19);
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) Payload_DATA);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    free(Payload_DATA);

    //7. DUT: Send ACK for all queued data seg(s) including the latest one 
     for(time_t start=time(NULL);time(NULL)-start<1;)
    {  
        TCP_Packet TCP_RP_ACK = ListenTCP(f);
        if (TCP_RP_ACK.length != 0) 
        {
            int ackNum = (int)GetTCPField(&TCP_RP_ACK, TCP, TCP_AckNumber);
            int SeqNum = (int)GetTCPField(&TCP_RP_ACK, TCP, TCP_SeqNumber);
            int AckF= (int)GetTCPField(&TCP_RP_ACK, TCP, TCP_FlagACK);

            
            if((seqN[TCPConfig.nss-2]+19== ackNum) && (SeqNum == ackN) && (AckF ==1))
            {
                printf("\nTest passed\n");
                End_Test_Mode_2(socketID,  seqN,  ackN);
                return 0;
            }
            else
            {
                End_Test_Mode_2(socketID,  seqN,  ackN);
                printf("\nTest failed : DUT dID not send the ACK segment  \n");
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




   


}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_OUT_OF_ORDER_05
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP_OUT_OF_ORDER_05 test.
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
int TCP_OUT_OF_ORDER_05()
{
	//TP_OpenControlChannel();
    StartTest();



    //1. TESTER: Cause the DUT to move on to ESTABLISHED state at a <wnp> 
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
    

    for (int i=0;i<TCPConfig.nss;i++)
    {
        //2. TESTER: Send a full-sized segment
        TCP_Packet TCP_P = CreateTCP();
        EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
        EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
        EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
        EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
        EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(seqN ));
        EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(ackN ));
        uint8_t * MAX_Size_Payload= (uint8_t*)malloc((TCPConfig.mss) * sizeof(uint8_t));
        memset(MAX_Size_Payload,0,(TCPConfig.mss));
        memset(MAX_Size_Payload,i+1,(TCPConfig.mss-1));
        EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t) MAX_Size_Payload);
        TCP_Compute_checksum(&TCP_P);
        SendTCP(TCP_P);



        free(MAX_Size_Payload);

        //3. DUT: Send ACKs for all the data segments
        //4. TESTER: Verify that for every even number of data segments sent the number of ACKs received is at least half of that number 
        Packet_filter f;
        strcpy(f.SrcAdd, TCPConfig.DUT_IP);
        strcpy(f.dstAdd, TCPConfig.TESTER_IP);
        f.Srcport = TCPConfig.DUT_Port;
        f.Dstport = TCPConfig.Tester_Port;
        TCP_Packet TCP_RP = ListenTCP(f);
        if (TCP_RP.length != 0) 
        {
            int rACK = (int)GetTCPField(&TCP_RP, TCP, TCP_FlagACK);
            int rAckN= (int)GetTCPField(&TCP_RP, TCP, TCP_AckNumber);
            int rSeqNum = (int)GetTCPField(&TCP_RP, TCP, TCP_SeqNumber);
            
            if  ((rACK == 1) && (rSeqNum = ackN) && (rAckN>((seqN+TCPConfig.mss-1)/2)))
            {
                printf("\nresult ok : DUT sent the ACK segment %d \n",i+1);
                seqN=rAckN;

            }
            else 
            {
                End_Test_Mode_2(socketID,  seqN,  ackN);
                printf("\nTest failed --> response not ok\n");
                return 1;  
            }
        }
        else 
        {
            printf("\nTest failed --> DUT  does not sent the segment\n");
            End_Test_Mode_2(socketID,  seqN,  ackN);
            return 1;
        }

    }
    
    //test passed
    printf("\nTest passed\n");
    End_Test_Mode_2(socketID,  seqN,  ackN);
    return 0;
}
