#include "AbstractionAPI.h"
#include "TCPcommon.h"
#include "TCP_URGENT_PTR.h"

#include"TestabilityProtocol_api.h"

TP_ResultID_t result_ReceiveAndForward_event_TCP_URGENT_PTR = 1 ,result_ReceiveAndForward_cb_TCP_URGENT_PTR = 1;
vint8 Data_TCP_URGENT_PTR;
void TP_TcpReceiveAndForward_cb_Urg_Pointer(TP_ResultID_t a, uint16 b)
{
    result_ReceiveAndForward_cb_TCP_URGENT_PTR=a;
    
}

void TP_TcpReceiveAndForward_event_cb_Urg_Pointer(TP_ResultID_t Result_ReceiveAndForward_event, uint16 c , vint8 data)
{
   printf("\n ************************** %d ***************\n",data.Data);
   Data_TCP_URGENT_PTR.Data= (uint8*)malloc(data.dataLength*sizeof(uint8));
   Data_TCP_URGENT_PTR.dataLength=data.dataLength;
   memcpy(Data_TCP_URGENT_PTR.Data, data.Data,data.dataLength);
   result_ReceiveAndForward_event_TCP_URGENT_PTR=Result_ReceiveAndForward_event;
}

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_URGENT_PTR_04
*
***************************************************************************************************/
/**
* @brief
*  Performs TCP URGENT PTR 04 test.
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
int TCP_URGENT_PTR_04()
{
     TP_GenaralStartTest(NULL);

    // 1. TESTER: Cause DUT to move on to ESTABLISHED state 
    int socketID;
    uint32 seqN, ackN;
    moveClientDUTToEstablished(&socketID, TCPConfig.DUT_Port,TCPConfig.Tester_Port, TCPConfig.Maxcon, &seqN, &ackN);
    

    
    //2. TESTER: Send a data segment with URG flag set and urgent pointer containing the offset for the SEQ number of a byte that is not the last byte of this data segment
    //TESTER: Cause the application on the DUT-sIDe to issue a RECEIVE call
    TP_TcpReceiveAndForward(TP_TcpReceiveAndForward_cb_Urg_Pointer, TP_TcpReceiveAndForward_event_cb_Urg_Pointer, socketID, 0xFFFF, 0xFFFF);

    TCP_Packet TCP_P = CreateTCP();
    EditTCPField(&TCP_P, TCP, TCP_FlagPSH, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagACK, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_FlagURG, (void *)(intptr_t)1);
    EditTCPField(&TCP_P, TCP, TCP_SrcPort, (void *)(intptr_t)TCPConfig.Tester_Port);
    EditTCPField(&TCP_P, TCP, TCP_DstPort, (void *)(intptr_t)TCPConfig.DUT_Port);
    EditTCPField(&TCP_P, TCP, TCP_SeqNumber, (void *)(intptr_t)(ackN));
    EditTCPField(&TCP_P, TCP, TCP_AckNumber, (void *)(intptr_t)(seqN));
    EditTCPField(&TCP_P, TCP, TCP_UrgPointer, (void *)(intptr_t)(3));
    EditTCPField(&TCP_P, PAYLAOD, PAYLOAD_data, (void *)(intptr_t)"TEST TCP");
    TCP_Compute_checksum(&TCP_P);
    SendTCP(TCP_P);
    sleep(2);

   

    //3. TESTER: Cause the application on the DUT-sIDe to issue a RECEIVE call
    //4. DUT: Issues a RECEIVE call
    //5. TESTER: Verify that the DUT returns the data to the receiving application
    if  (result_ReceiveAndForward_event_TCP_URGENT_PTR==0)
    {
        if (!strcmp(Data_TCP_URGENT_PTR.Data,"T TCP"))
        {
            printf("\n Test passed\n");
            EndTest(socketID);
            return 0;
        }
        else 
        {
            printf("\nTest failed --> data received is not the correct data\n");
            EndTest(socketID);
            return 1;
        }
    }
    else 
    {
        printf("\nTest failed --> DUT does not issues a RECEIVE call\n");
        EndTest(socketID);
        return 1;
    }
}
