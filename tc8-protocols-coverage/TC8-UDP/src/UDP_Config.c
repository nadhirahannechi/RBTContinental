#include "UDP_config.h"
#include <string.h>


UDP_config_t UDPConfiguration;

void Set_UDP_Config(UDP_config_t configuration){

    strcpy(UDPConfiguration.Dst_IP_addr,configuration.Dst_IP_addr);
    strcpy(UDPConfiguration.Host_1_IP,configuration.Host_1_IP);
    strcpy(UDPConfiguration.Host_2_IP,configuration.Host_2_IP);
    strcpy(UDPConfiguration.AIface_0_BcastIP,configuration.AIface_0_BcastIP);
    strcpy(UDPConfiguration.allSystemMCastAddr,configuration.allSystemMCastAddr);

    strcpy(UDPConfiguration.UDPDefaultData,configuration.UDPDefaultData);
    strcpy(UDPConfiguration.UDPData,configuration.UDPData);
  
    UDPConfiguration.testerUDPPort=configuration.testerUDPPort;
    UDPConfiguration.testerUDPPort2=configuration.testerUDPPort2;
    UDPConfiguration.unusedUDPDstPort1=configuration.unusedUDPDstPort1;
    UDPConfiguration.unusedUDPSrcPort=configuration.unusedUDPSrcPort;
    UDPConfiguration.DUTUDPPort=configuration.DUTUDPPort;
    
    UDPConfiguration.udpUserDataSize=configuration.udpUserDataSize;
    UDPConfiguration.incorrectUDPChecksum=configuration.incorrectUDPChecksum;
    UDPConfiguration.UDPDefaultDataLen=configuration.UDPDefaultDataLen;
    UDPConfiguration.UDPDataLen=configuration.UDPDataLen;
    

}

UDP_config_t Get_UDP_Config(void){
  return (UDPConfiguration);
}