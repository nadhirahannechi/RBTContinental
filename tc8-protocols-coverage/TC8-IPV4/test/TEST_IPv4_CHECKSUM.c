#include "AbstractionAPI.h"
#include "IPv4_CHECKSUM.h"
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
    conf.LISTEN_TIME = 3;
    conf.INVALID_CHECKSUM = 0x0000;
    Set_IPv4_Config(conf);

    IPv4_CHECKSUM_02();
    IPv4_CHECKSUM_05();

	return 0;
}

