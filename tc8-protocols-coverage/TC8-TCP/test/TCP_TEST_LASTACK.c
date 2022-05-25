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
    conf.Tester_Port = 59256;
    conf.msl = 60;
    conf.mss = 1460;
    conf.nss = 5;
    Set_TCP_Config(conf);

    // Controle channel
    stoiIP(conf.DUT_IP, ipv4.Data);
    TP_OpenControlChannel(ipv4, 56000);
    TP_GenaralStartTest(NULL);
    printf("\n Starting TCP_UNACCEPTABLE_02 ...\n");
    int socketID;
    uint32 rSEQNumber = 0;
    uint32 rACKNumber = 0;
    moveDUTToLASTACK(&socketID, NULL,TCPConfig.DUT_Port, 5, &rSEQNumber, &rACKNumber);
    printf("\nrSEQNumber = %d\n", rSEQNumber);
    printf("\nrACKNumber = %d\n", rACKNumber);
   
    
    TP_CloseControlChannel();
    return 0;
}
