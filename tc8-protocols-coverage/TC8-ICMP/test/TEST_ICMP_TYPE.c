#include "ICMPv4_TYPE.h"
#include "ICMPv4config.h"
#include "AbstractionAPI.h"
#include <stdio.h>
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
	
    ICMPv4_config_t conf;
    strcpy(conf.DUT_IP, "192.168.20.178");
    strcpy(conf.TESTER_IP, "192.168.20.243");
	conf.ICMP_IDENTIFIER = 0x69db;
	conf.ICMP_SEQUENCE_NUMBER = 0x0001;
	conf.INVALID_CHECKSUM = 0x0000;
	conf.FRAGMENT_REASSEMBLY_TIMEOUT = 15;
	conf.UNSUPPORTED_PROTOCOL = 0;
	conf.LISTEN_TIME = 3;
    Set_ICMPv4_Config(conf);

	ICMPv4_TYPE_04();
	ICMPv4_TYPE_05();
	ICMPv4_TYPE_08();
	ICMPv4_TYPE_09();
	ICMPv4_TYPE_10();
	//ICMPv4_TYPE_11();
	ICMPv4_TYPE_12();
	ICMPv4_TYPE_16();
	ICMPv4_TYPE_18();
	ICMPv4_TYPE_22();
	    
	return 0;
}
