#include "AbstractionAPI.h"
#include "IPv4_REASSEMBLY.h"
#include "IPv4config.h"
#include <string.h>

int main()
{
    //Configure Network API
    Net_API_config_t NetAPIConfig; 
    strcpy(NetAPIConfig.S_interface, "enp0s3");
    strcpy(NetAPIConfig.S_DUT_HW_Address, "50:9a:4c:34:2e:a6");
    strcpy(NetAPIConfig.S_DUT_IP_Address, "192.168.20.178");
    strcpy(NetAPIConfig.S_Tester_HW_Address, "08:00:27:fd:55:27");
    strcpy(NetAPIConfig.S_Tester_IP_Address, "192.168.20.243");
    Set_Network_Abstration_API_Config(NetAPIConfig);
	
    IPv4_config_t conf;
    strcpy(conf.DUT_IP, "192.168.20.178");
    strcpy(conf.TESTER_IP, "192.168.20.243");
    conf.LARGE_TTL_VALUE = 25;
    conf.LOW_TTL_VALUE = 5;
    conf.LISTEN_TIME = 3;
    conf.IP_INI_REASSEMBLE_TIMEOUT = 30;
    conf.FRAGMENT_REASSEMBLY_TIMEOUT = 15;
    conf.IP_TYPE_ICMP = 0x01;
    Set_IPv4_Config(conf);

    IPv4_REASSEMBLY_04();
    IPv4_REASSEMBLY_06();
    IPv4_REASSEMBLY_07();
    IPv4_REASSEMBLY_09();
    IPv4_REASSEMBLY_10();
    IPv4_REASSEMBLY_11();
    IPv4_REASSEMBLY_12();
    //IPv4_REASSEMBLY_13();

	return 0;
}
