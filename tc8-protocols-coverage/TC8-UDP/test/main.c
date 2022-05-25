#include "AbstractionAPI.h"
#include "UDP_DatagramLength.h"
#include "UDP_FIELDS.h"
#include "UDP_USER_INTERFACE.h"
#include "UDP_INTRODUCTION.h"
#include "UDP_INVALID_ADDRESSES.h"
#include "UDP_MessageFormat.h"
#include "UDP_Padding.h"

#include "UDP_config.h"
#include "TestabilityProtocol_Intern.h"
#include "split.h"
int main() {
    Net_API_config_t abst_config;
    strcpy(abst_config.S_interface,"eth0");
    strcpy(abst_config.S_DUT_HW_Address,"08:00:27:e7:2d:e8");
    strcpy(abst_config.S_DUT_IP_Address,"192.168.1.18");
    strcpy(abst_config.S_Tester_HW_Address,"54:e1:ad:39:8b:16");
    strcpy(abst_config.S_Tester_IP_Address,"192.168.1.14");
    Set_Network_Abstration_API_Config(abst_config);

    UDP_config_t UDP_Parameters;

    strcpy(UDP_Parameters.Dst_IP_addr,"192.168.1.18");
    strcpy(UDP_Parameters.Host_1_IP,"192.168.1.14");
    strcpy(UDP_Parameters.Host_2_IP,"192.168.1.10");
    strcpy(UDP_Parameters.AIface_0_BcastIP,"192.168.1.255");
    strcpy(UDP_Parameters.allSystemMCastAddr,"224.0.0.1");

    strcpy(UDP_Parameters.UDPDefaultData,"UDP Test");
    strcpy(UDP_Parameters.UDPData,"TESTERTESTERTESTERTESTER");

    UDP_Parameters.testerUDPPort=13456;
    UDP_Parameters.testerUDPPort2=15632;
    UDP_Parameters.unusedUDPDstPort1=20000;
    UDP_Parameters.unusedUDPSrcPort=20001;
    UDP_Parameters.DUTUDPPort=36000;

    UDP_Parameters.udpUserDataSize=101;
    UDP_Parameters.incorrectUDPChecksum=0xffff;
    UDP_Parameters.UDPDefaultDataLen=8;
    UDP_Parameters.UDPDataLen=24;

    Set_UDP_Config(UDP_Parameters);
    Get_UDP_Config();
    ip4addr ipv4DUT;
    ipv4DUT.dataLength=4;
    extractIpAddress(UDP_Parameters.Dst_IP_addr,&ipv4DUT);

    TP_OpenControlChannel(ipv4DUT,56000);

    UDP_DatagramLength_01();
    sleep(1);

    UDP_MessageFormat_02();
    sleep(1);

    UDP_Padding_02();
    sleep(1);

    UDP_FIELDS_01();
    sleep(1);
    UDP_FIELDS_02();
    sleep(1);
    UDP_FIELDS_03();
    sleep(1);
    UDP_FIELDS_06();
    sleep(1);
    UDP_FIELDS_07();
    sleep(1);
    UDP_FIELDS_08();
    sleep(1);
    UDP_FIELDS_09();
    sleep(1);
    UDP_FIELDS_10();
    sleep(1);
    //UDP_FIELDS_12();

    UDP_FIELDS_13();
    sleep(1);
    UDP_FIELDS_14();
    sleep(1);
    UDP_FIELDS_15();
    sleep(1);
    UDP_FIELDS_16();
    sleep(1);

    UDP_USER_INTERFACE_01();
    sleep(1);
    UDP_USER_INTERFACE_02();
    sleep(1);
    UDP_USER_INTERFACE_03();
    sleep(1);
    UDP_USER_INTERFACE_04();
    sleep(1);
    UDP_USER_INTERFACE_05();
    sleep(1);
    UDP_USER_INTERFACE_06();
    sleep(1);
    UDP_USER_INTERFACE_07();
    sleep(1);
    UDP_USER_INTERFACE_08();
    sleep(1);

    UDP_INTRODUCTION_01();
    sleep(1);
    UDP_INTRODUCTION_02();
    sleep(1);
    UDP_INTRODUCTION_03();
    sleep(1);

    UDP_INVALID_ADDRESSES_01();
    sleep(1);
    UDP_INVALID_ADDRESSES_02();
    sleep(1);
    TP_CloseControlChannel();
}

