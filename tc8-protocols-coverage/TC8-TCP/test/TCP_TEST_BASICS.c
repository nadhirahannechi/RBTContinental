#include "AbstractionAPI.h"
#include "TCPcommon.h"
#include "TCPconfig.h"
#include "TCP_BASICS.h"

#include "TestabilityProtocol_Intern.h"

int main() {
    // Configure Network API
    Net_API_config_t NetAPIConfig; 
    strcpy(NetAPIConfig.S_interface, "enp0s3");
    strcpy(NetAPIConfig.S_DUT_HW_Address, "b8:27:eb:4b:b2:24");
    strcpy(NetAPIConfig.S_DUT_IP_Address, "169.254.159.220");
    strcpy(NetAPIConfig.S_Tester_HW_Address, "08:00:27:0b:e3:32");
    strcpy(NetAPIConfig.S_Tester_IP_Address, "169.254.159.221");
    Set_Network_Abstration_API_Config(NetAPIConfig);

    // Configure TCP protocol
    TCP_config_t conf;
    strcpy(conf.DUT_IP, "169.254.159.220");
    strcpy(conf.TESTER_IP, "169.254.159.221");
    conf.DUT_Port = 50000;
    conf.Tester_Port = 59256;
    conf.msl = 60;
    Set_TCP_Config(conf);

    // Controle channel
    ip4addr ipv4;
    ipv4.dataLength = 4;
    stoiIP(TCPConfig.DUT_IP, ipv4.Data);
    TP_OpenControlChannel(ipv4, 56000);
    // Starting tests
    //int b01 = TCP_BASICS_01();
    //int b02 = TCP_BASICS_02();
    int b03 = TCP_BASICS_03();
    int b04 = TCP_BASICS_04_IT1() + TCP_BASICS_04_IT2() + TCP_BASICS_04_IT3();
    //int b05 = TCP_BASICS_05_IT1() + TCP_BASICS_05_IT2();
    //int b06 = TCP_BASICS_06();
    //int b07 = TCP_BASICS_07();
    //int b10 = TCP_BASICS_10_IT1();
    //printf("\n b01 = %d b02 = %d b03 = %d b04 = %d b05 = %d b06 = %d b07 = %d b10 = %d\n", b01, b02, b03, b04, b05, b06, b07, b10);
    //printf("\n b01 = %d b02 = %d b03 = %d b04 = %d b05 = %d\n", b01, b02, b03, b04, b05);
    //printf("\nb01 = %d b04 = %d b05 = %d\n", b01, b04, b05);
    printf("\nb03 = %d b04 = %d\n", b03, b04);
    //printf("\nb06 = %d\n", b06);
    // End tests
    //TP_CloseControlChannel();
    return 0;
}