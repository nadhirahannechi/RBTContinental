#include "AbstractionAPI.h"
#include "TCPcommon.h"
#include "TCPconfig.h"
#include "TCP_FLAGS_INVALID.h"
#include "TCP_RETRANSMISSION_TO.h"
#include "TCP_MSS_OPTIONS.h"
#include "TCP_BASICS.h"
#include "TCP_CALL_ABORT.h"
#include "TCP_FLAGS_PROCESSING.h"
#include "TCP_OUT_OF_ORDER.h"

#include "TestabilityProtocol_Intern.h"

int main() {
    ip4addr ipv4;
    ipv4.dataLength = 4;

    // Configure Network API
    Net_API_config_t NetAPIConfig; 
    strcpy(NetAPIConfig.S_interface, "eth1");
    strcpy(NetAPIConfig.S_DUT_HW_Address, "b8:27:eb:00:55:0d");
    strcpy(NetAPIConfig.S_DUT_IP_Address, "192.168.200.2");
    strcpy(NetAPIConfig.S_Tester_HW_Address, "00:00:10:00:0b:68");
    strcpy(NetAPIConfig.S_Tester_IP_Address, "192.168.200.1");
    Set_Network_Abstration_API_Config(NetAPIConfig);

    // Configure TCP protocol
    TCP_config_t conf;
    strcpy(conf.DUT_IP, "192.168.200.2");
    strcpy(conf.TESTER_IP, "192.168.200.1");
    conf.DUT_Port = 50000;
    conf.Tester_Port = 41500;
    conf.msl = 60;
    conf.mss = 1460;
    conf.nss = 20;
    conf.ssz = 3;
    Set_TCP_Config(conf);

    // Controle channel
    stoiIP(conf.DUT_IP, ipv4.Data);
    TP_OpenControlChannel(ipv4, 56000);
    //int result=TCP_RETRANSMISSION_TO_03();
    //int result1=TCP_RETRANSMISSION_TO_08();
    //int result2=TCP_RETRANSMISSION_TO_09();
    //int result2=TCP_RETRANSMISSION_TO_05();
    //int result3=TCP_RETRANSMISSION_TO_08();
    //int result4=TCP_RETRANSMISSION_TO_09();
    //int r= TCP_MSS_OPTIONS_06_IT1();
    //int r1= TCP_MSS_OPTIONS_06_IT2();
    //int result =TCP_FLAGS_PROCESSING_01();
    /*int result1 =TCP_FLAGS_INVALID_13_IT2();
    int result2 =TCP_FLAGS_INVALID_13_IT3();
    int result3 =TCP_FLAGS_INVALID_13_IT4();
    int result4 =TCP_FLAGS_INVALID_13_IT5();
    int result=TCP_FLAGS_INVALID_13_IT2();*/
    /*int result2=TCP_FLAGS_PROCESSING_02_IT2();
    int result3=TCP_FLAGS_PROCESSING_02_IT3();
    int result4=TCP_FLAGS_PROCESSING_02_IT4();*/
    /*int result1=TCP_FLAGS_PROCESSING_07_IT1();
    int result2=TCP_FLAGS_PROCESSING_07_IT2();
    int result3=TCP_FLAGS_PROCESSING_07_IT3();*/
    //int result=TCP_CALL_RECEIVE_04_IT1();
    printf("\n Starting TCP_UNACCEPTABLE_02 ...\n");
    StartTest();
    int socketID,sock;
    uint32 rSEQNumber = 0;
    uint32 rACKNumber = 0;
    int result=TCP_OUT_OF_ORDER_02();
    //printf("\nrSEQNumber = %d\n", rSEQNumber);
    //printf("\nrACKNumber = %d\n", rACKNumber);
        //int result1=TCP_BASICS_08_IT1();

    /*int result=TCP_UNACCEPTABLE_14_IT2();
    int result1=TCP_UNACCEPTABLE_14_IT1();
    int result2=TCP_UNACCEPTABLE_12_IT2();
    int result3=TCP_UNACCEPTABLE_12_IT1();*/
    //int result1=TCP_UNACCEPTABLE_12_IT2();
    //int result2=TCP_UNACCEPTABLE_14_IT1();
    //int result3=TCP_UNACCEPTABLE_14_IT2();
    //int result1=TCP_BASICS_02();
    //sleep(10);
    //End_Test_Mode_2(socketID,rSEQNumber, rACKNumber);
    //TP_CloseControlChannel();
    //printf("\n************************result = %d %d %d %d %d\n",result,result1,result3,result4,result5);
    
   // printf(" *********************************** test result = %d , %d %d , %d  ",result,result1,result2,result3);
    printf(" *********************************** test result = %d , %d %d , %d  ",result,result,result,result);
    //printf(" *********************************** test result = %d %d %d %d %d ",result,result1,result2,result3,result4);
    return 0;
}
