#include "AbstractionAPI.h"
#include "TCPcommon.h"
#include "TCPconfig.h"

#include "TestabilityProtocol_api.h"
#include "TestabilityProtocol_Intern.h"

int main() {
    ip4addr ipv4;
    ipv4.dataLength = 4;

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
    stoiIP(TCPConfig.DUT_IP, ipv4.Data);
    TP_OpenControlChannel(ipv4, 56000);
    TP_GenaralStartTest(NULL);
    int socketID;
    uint32 rSEQNumber = 0;
    uint32 rACKNumber = 0;
    moveDUTToCloseWait(&socketID, NULL, TCPConfig.DUT_Port, 5, &rSEQNumber, &rACKNumber);
    //TP_GenaralEndTest(NULL,0,(text){0});
    //TP_CloseControlChannel();
    return 0;
}