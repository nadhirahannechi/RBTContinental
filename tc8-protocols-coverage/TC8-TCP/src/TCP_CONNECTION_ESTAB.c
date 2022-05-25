#include "AbstractionAPI.h"
#include "TCPcommon.h"
#include "TCP_CONNECTION_ESTAB.h"


#include "TestabilityProtocol_api.h"
#include "TestabilityProtocol_Intern.h"

int socketID1 = 0;
int socketID2 = 0;
int socketID3 = 0;

void CreateAndBind_callback1(TP_ResultID_t b, uint16 socket)
{
    socketID1 = socket;
}

void CreateAndBind_callback2(TP_ResultID_t b, uint16 socket)
{
    socketID2 = socket;
}

void CreateAndBind_callback3(TP_ResultID_t b, uint16 socket)
{
    socketID3 = socket;
}

TCP_Packet TCP_RP_1, TCP_RP_2, TCP_RP_3;

void* recieveTCP1_01(void *)
{
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_RP_1 = ListenTCP(f);
}

void* recieveTCP2_01(void *)
{
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port + 1;
    TCP_RP_2 = ListenTCP(f);
}

void* recieveTCP3_01(void *)
{
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port + 2;
    TCP_RP_3 = ListenTCP(f);
}

void* recieveTCP1_02(void *)
{
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port;
    f.Dstport = TCPConfig.Tester_Port;
    TCP_RP_1 = ListenTCP(f);
}

void* recieveTCP2_02(void *)
{
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port + 1;
    f.Dstport = TCPConfig.Tester_Port + 1;
    TCP_RP_2 = ListenTCP(f);
}

void* recieveTCP3_02(void *)
{
    Packet_filter f;
    strcpy(f.SrcAdd, TCPConfig.DUT_IP);
    strcpy(f.dstAdd, TCPConfig.TESTER_IP);
    f.Srcport = TCPConfig.DUT_Port + 2;
    f.Dstport = TCPConfig.Tester_Port + 2;
    TCP_RP_3 = ListenTCP(f);
}

int TCP_CONNECTION_ESTAB_01() {
    //TP_OpenControlChannel();
    TP_GenaralStartTest(NULL);

    // Move DUT to established state
    int socketID;
    moveDUTToListen(&socketID,  NULL, TCPConfig.DUT_Port, 5);

    pthread_t th1, th2, th3;
    pthread_create(&th1, NULL, recieveTCP1_01, NULL);
    pthread_create(&th2, NULL, recieveTCP2_01, NULL);
    pthread_create(&th3, NULL, recieveTCP3_01, NULL);

    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)(TCPConfig.Tester_Port + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)(TCPConfig.Tester_Port + 2));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    if (TCP_RP_1.length != 0) { 
        int rSYN = (int)GetTCPField(&TCP_RP_1, TCP, TCP_FlagSYN);
        int rACK = (int)GetTCPField(&TCP_RP_1, TCP, TCP_FlagACK);
        int rPort = (int)GetTCPField(&TCP_RP_1, TCP, TCP_DstPort);
        if ((rSYN == 1) && (rACK == 1)) {
            printf("\nTest passed on port %d\n", ntohs(rPort));
        } else {
            printf ("\nTest failed --> DUT response is not SYN ACK in %s : %d\n", __FILE__, __LINE__);
            return 1;
        }
    } else {
        printf("\nTest failed --> DUT not responding in %s : %d\n", __FILE__, __LINE__);
        return 1;
    }
    if (TCP_RP_2.length != 0) {
        int rSYN = (int)GetTCPField(&TCP_RP_2, TCP, TCP_FlagSYN);
        int rACK = (int)GetTCPField(&TCP_RP_2, TCP, TCP_FlagACK);
        int rPort = (int)GetTCPField(&TCP_RP_2, TCP, TCP_DstPort);
        if ((rSYN == 1) && (rACK == 1)) {
            printf("\nTest passed on port %d\n", ntohs(rPort));
            return 1;
        } else {
            printf ("\nTest failed --> DUT response is not SYN ACK in %s : %d\n", __FILE__, __LINE__);
            return 1;
        }
    } else {
        printf("\nTest failed --> DUT not responding in %s : %d\n", __FILE__, __LINE__);
        return 1;
    }
    if (TCP_RP_3.length != 0) { 
        int rSYN = (int)GetTCPField(&TCP_RP_3, TCP, TCP_FlagSYN);
        int rACK = (int)GetTCPField(&TCP_RP_3, TCP, TCP_FlagACK);
        int rPort = (int)GetTCPField(&TCP_RP_3, TCP, TCP_DstPort);
        if ((rSYN == 1) && (rACK == 1)) {
            printf("\nTest passed on port %d\n", ntohs(rPort));
        } else {
            printf ("\nTest failed --> DUT response is not SYN ACK in %s : %d\n", __FILE__, __LINE__);
            return 1;
        }
    } else {
        printf("\nTest failed --> DUT not responding in %s : %d\n", __FILE__, __LINE__);
        return 1;
    }
	
    TP_GenaralEndTest(NULL,0,(text){0});
    TP_CloseControlChannel();
    return 0;
}

int TCP_CONNECTION_ESTAB_02() {
    //TP_OpenControlChannel();
    TP_GenaralStartTest(NULL);
    int socketID;
    moveDUTToListen(&socketID, NULL, TCPConfig.DUT_Port, 5);
    TCPConfig.DUT_Port++;
    moveDUTToListen(&socketID, NULL,  TCPConfig.DUT_Port, 5);
    TCPConfig.DUT_Port++;
    moveDUTToListen(&socketID, NULL, TCPConfig.DUT_Port, 5);

    pthread_t th1, th2, th3;
    pthread_create(&th1, NULL, recieveTCP1_02, NULL);
    pthread_create(&th2, NULL, recieveTCP2_02, NULL);
    pthread_create(&th3, NULL, recieveTCP3_02, NULL);

    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)(TCPConfig.Tester_Port));
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)(TCPConfig.DUT_Port));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)(TCPConfig.Tester_Port + 1));
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)(TCPConfig.DUT_Port + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)(TCPConfig.Tester_Port + 2));
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)(TCPConfig.DUT_Port + 2));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    if (TCP_RP_1.length != 0) { 
        int rSYN = (int)GetTCPField(&TCP_RP_1, TCP, TCP_FlagSYN);
        int rACK = (int)GetTCPField(&TCP_RP_1, TCP, TCP_FlagACK);
        if ((rSYN == 1) && (rACK == 1)) {
            printf("\nTest passed in %s : %d\n", __FILE__, __LINE__);
        } else {
            printf ("\nTest failed --> DUT response is not SYN ACK in %s : %d\n", __FILE__, __LINE__);
            return 1;
        }
        
    } else {
        printf("\nTest failed --> DUT not responding in %s : %d\n", __FILE__, __LINE__);
        return 1;
    }
        if (TCP_RP_2.length != 0) { 
        int rSYN = (int)GetTCPField(&TCP_RP_2, TCP, TCP_FlagSYN);
        int rACK = (int)GetTCPField(&TCP_RP_2, TCP, TCP_FlagACK);
        if ((rSYN == 1) && (rACK == 1)) {
            printf("\nTest passed in %s : %d\n", __FILE__, __LINE__);
        } else {
            printf ("\nTest failed --> DUT response is not SYN ACK in %s : %d\n", __FILE__, __LINE__);
            return 1;
        }
    } else {
        printf("\nTest failed --> DUT not responding in %s : %d\n", __FILE__, __LINE__);
        return 1;
    }
    
    if (TCP_RP_3.length != 0) { 
        int rSYN = (int)GetTCPField(&TCP_RP_3, TCP, TCP_FlagSYN);
        int rACK = (int)GetTCPField(&TCP_RP_3, TCP, TCP_FlagACK);
        if ((rSYN == 1) && (rACK == 1)) {
            printf("\nTest passed in %s : %d\n", __FILE__, __LINE__);
        } else {
            printf ("\nTest failed --> DUT response is not SYN ACK in %s : %d\n", __FILE__, __LINE__);
            return 1;
        }
    } else {
        printf("\nTest failed --> DUT not responding in %s : %d\n", __FILE__, __LINE__);
        return 1;
    }
	
    TP_GenaralEndTest(NULL,0,(text){0});
    TP_CloseControlChannel();
    return 0;
}


/***************************************************************************************************
*
*   FUNCTION NAME: TCP_CONNECTION_ESTAB_03
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP CONNECTION ESTAB 03 test.
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
int TCP_CONNECTION_ESTAB_03()
{
    ip4addr ipv4;
    ipv4.dataLength = 4;
    stoiIP(TCPConfig.DUT_IP, ipv4.Data);
    // Start Test
    StartTest();

    // 1. TESTER: Send an UpperTester <OpenMultipleTCPSocket (active,3)> request 
    TP_TcpCreateAndBind(CreateAndBind_callback1, TRUE, TCPConfig.DUT_Port, ipv4);
    sleep(3);
    TP_TcpCreateAndBind(CreateAndBind_callback2, TRUE, TCPConfig.DUT_Port + 1, ipv4);
    sleep(3);
    TP_TcpCreateAndBind(CreateAndBind_callback3, TRUE, TCPConfig.DUT_Port + 2, ipv4);
    sleep(3);

    stoiIP(TCPConfig.TESTER_IP, ipv4.Data);
    pthread_t th1, th2, th3;
    TP_TcpConnect(NULL, socketID1, TCPConfig.Tester_Port, ipv4);
    sleep(3);
    pthread_create(&th1, NULL, recieveTCP1_02, NULL);
    TP_TcpConnect(NULL, socketID2, TCPConfig.Tester_Port + 1, ipv4);
    sleep(3);
    pthread_create(&th2, NULL, recieveTCP2_02, NULL);
    TP_TcpConnect(NULL, socketID3, TCPConfig.Tester_Port + 2, ipv4);
    sleep(5);
    pthread_create(&th3, NULL, recieveTCP3_02, NULL);


    // 2. DUT: Opens 3 TCP active sockets and send 3 SYNs from each socket.
    pthread_join(th1, NULL);
    int rSeqNumber1;
    if (TCP_RP_1.length != 0)
    {
        int rSYN = (int)GetTCPField(&TCP_RP_1, TCP, TCP_FlagSYN);
        rSeqNumber1 = (int)GetTCPField(&TCP_RP_1, TCP, TCP_SeqNumber);
        if (rSYN == 1)
        {
            // Test OK
        }
        else
        {
            printf ("\nTest failed --> DUT response is not SYN in %s : %d\n", __FILE__, __LINE__);
            return 1;
        }
    }
    else
    {
        printf("\nTest failed --> DUT not responding in %s : %d\n", __FILE__, __LINE__);
        return 1;
    }

    sleep(3);
    pthread_join(th2, NULL);
    int rSeqNumber2;
    if (TCP_RP_2.length != 0)
    { 
        int rSYN = (int)GetTCPField(&TCP_RP_2, TCP, TCP_FlagSYN);
        rSeqNumber2 = (int)GetTCPField(&TCP_RP_2, TCP, TCP_SeqNumber);
        if (rSYN == 1)
        {
            // Test OK
        }
        else
        {
            printf ("\nTest failed --> DUT response is not SYN in %s : %d\n", __FILE__, __LINE__);
            return 1;
        }
    }
    else
    {
        printf("\nTest failed --> DUT not responding in %s : %d\n", __FILE__, __LINE__);
        return 1;
    }
    
    sleep(3);
    pthread_join(th3, NULL);
    int rSeqNumber3;
    if (TCP_RP_3.length != 0)
    {
        int rSYN = (int)GetTCPField(&TCP_RP_3, TCP, TCP_FlagSYN);
        rSeqNumber3 = (int)GetTCPField(&TCP_RP_3, TCP, TCP_SeqNumber);
        if (rSYN == 1)
        {
            // Test OK
        }
        else
        {
            printf ("\nTest failed --> DUT response is not SYN in %s : %d\n", __FILE__, __LINE__);
            return 1;
        }
    }
    else
    {
        printf("\nTest failed --> DUT not responding in %s : %d\n", __FILE__, __LINE__);
        return 1;
    }

    // 3. TESTER: Send 3 TCP Segments with SYN, ACK set to 1 for each socket.
    sleep(4);
    pthread_create(&th1, NULL, recieveTCP1_02, NULL);
    pthread_create(&th2, NULL, recieveTCP2_02, NULL);
    pthread_create(&th3, NULL, recieveTCP3_02, NULL);

    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagSYN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(rSeqNumber1 + 1));
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)(TCPConfig.Tester_Port));
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)(TCPConfig.DUT_Port));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(rSeqNumber2 + 1));
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)(TCPConfig.Tester_Port + 1));
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)(TCPConfig.DUT_Port + 1));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(rSeqNumber3 + 1));
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)(TCPConfig.Tester_Port + 2));
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)(TCPConfig.DUT_Port + 2));
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 4. DUT: Send 3 TCP Segments with ACK set to 1 from each socket. 
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    if (TCP_RP_1.length != 0) { 
        int rACK = (int)GetTCPField(&TCP_RP_2, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            // Test OK
        }
        else
        {
            printf ("\nTest failed --> DUT response is not ACK in %s : %d\n", __FILE__, __LINE__);
            return 1;
        }
    }
    else
    {
        printf("\nTest failed --> DUT not responding in %s : %d\n", __FILE__, __LINE__);
        return 1;
    }

    pthread_join(th2, NULL);
    if (TCP_RP_2.length != 0)
    { 
        int rACK = (int)GetTCPField(&TCP_RP_2, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            // Test OK
        }
        else
        {
            printf ("\nTest failed --> DUT response is not ACK in %s : %d\n", __FILE__, __LINE__);
            return 1;
        }
    }
    else
    {
        printf("\nTest failed --> DUT not responding in %s : %d\n", __FILE__, __LINE__);
        return 1;
    }
    
    pthread_join(th3, NULL);
    if (TCP_RP_3.length != 0)
    {
        int rACK = (int)GetTCPField(&TCP_RP_2, TCP, TCP_FlagACK);
        if (rACK == 1)
        {
            printf ("\nTest passed");
            return 0;
        }
        else
        {
            printf ("\nTest failed --> DUT response is not ACK in %s : %d\n", __FILE__, __LINE__);
            return 1;
        }
    }
    else
    {
        printf("\nTest failed --> DUT not responding in %s : %d\n", __FILE__, __LINE__);
        return 1;
    }
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_CONNECTION_ESTAB_07
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP CONNECTION ESTAB 07 test.
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
int TCP_CONNECTION_ESTAB_07()
{
    // Start Test
    StartTest();

    // 1. TESTER: Bring DUT to state ‘ESTABLISHED’.
    int socketID;
    uint32 seqN = 0, ackN = 0;
    moveClientDUTToEstablished(&socketID, TCPConfig.DUT_Port,TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    sleep(3);

    // 2. TESTER: Send a FIN packet

    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagFIN, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)seqN);
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);

    // 3. DUT: Send ACK packet
    // Receive DUT responses
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
        if (rACK == 1)
        {
            // Test OK
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not ACK \n");
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

    // 4. DUT: Sends a FIN packet and moves to ‘CLOSED’ state.
    TP_TcpCloseSocket(NULL, (uint16)socketID, false);
    
    TCP_Packet TCP_RP2 = ListenTCP(f);
    if (TCP_RP2.length != 0)
    {
        // Verify that DUT sends a ACK
        int rACK = (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagACK);
        int rFIN= (int)GetTCPField(&TCP_RP2, TCP, TCP_FlagFIN);
        if ((rACK == 1) && (rFIN == 1))
        {
            seqN = (int)GetTCPField(&TCP_RP2, TCP, TCP_SeqNumber);
            ackN = (int)GetTCPField(&TCP_RP2, TCP, TCP_AckNumber );
        }
        else
        {
            // Unexpected response
            printf("\nTest failed --> DUT response is not FIN  \n");
            EndTest(socketID);
            return 1;
        }
    }
    else
    {
        // No response sent
        printf("\nTest failed --> DUT not responding \n");
        EndTest(socketID);
        return 1;
    }
    
    //5- Tester : send an ACK packet
    TCP_Packet TCP_P_2 = CreateTCP();
    EditTCPField(&TCP_P_2, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P_2, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P_2, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P_2, TCP, TCP_SeqNumber, (void *)(intptr_t)ackN);
    EditTCPField(&TCP_P_2, TCP, TCP_AckNumber, (void *)(intptr_t)seqN+1);
    TCP_Compute_checksum(&TCP_P_2);
    SendTCP(TCP_P_2);
    
    // 5. TESTER: externally check the state of the DUT.
    if(checkState(CLOSED))
    {
        printf("\nTest passed in %s : %d\n", __FILE__, __LINE__);
        EndTest(socketID);
        return 0;
    }
    else
    {
        printf("\nTest failed --> DUT is not in closed state in %s : %d\n", __FILE__, __LINE__);
        EndTest(socketID);
        return 1;
    }
}
