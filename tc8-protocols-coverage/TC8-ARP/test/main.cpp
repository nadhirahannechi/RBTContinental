#include "packet_reception.h"
#include "packet_generation.h"
#include"config.h"
#include "common.h"
#include "AbstractionAPI.h"
#include "TestabilityProtocol_Intern.h"
#include "TestabilityProtocol_api.h"
extern Arp_config_t ARP_config;

int main()
{      
    ip4addr ipv4Dut;
    ipv4Dut.dataLength = 4;
    stoiIP((char*)ARP_config.DIface_0_IP,ipv4Dut.Data);        
    //TP_OpenControlChannel(ipv4Dut,56000);
    Net_API_config_t abst_config;
    strcpy(abst_config.S_interface,"enp0s3");
    strcpy(abst_config.S_DUT_HW_Address,"08:00:27:68:41:d5");
    strcpy(abst_config.S_DUT_IP_Address,"192.168.20.248");
    strcpy(abst_config.S_Tester_HW_Address,"08:00:27:cd:64:f1");
    strcpy(abst_config.S_Tester_IP_Address,"192.168.20.83");
    Set_Network_Abstration_API_Config(abst_config);
    init_ARP_config();
    ARP_16();
    ARP_17();
    ARP_18();
    ARP_19();
    ARP_41();
    ARP_42();
    ARP_43();
    ARP_44();
    ARP_46();
    ARP_47();

    //TP_CloseControlChannel();
}