#include "AbstractionAPI.h"
#include "TCPcommon.h"
#include "TCPconfig.h"
#include "TCP_CONNECTION_ESTAB.h"
#include"TestabilityProtocol_Intern.h"

int main() {
   
    // Configure TCP protocol
    TCP_config_t conf;
    strcpy(conf.DUT_IP, "169.254.159.220");
    strcpy(conf.TESTER_IP, "169.254.159.221");
    conf.DUT_Port = 50000;
    conf.Tester_Port = 59256;
    conf.msl = 60;
    conf.Maxcon=3;
    conf.ISN=3;
    conf.mss=3;
    conf.window=3;
    conf.nss=3;
    conf.ssz=3;
    conf.tmx=3;
    conf.tfn=3;
    conf.wnp=3;
    conf.uopt=3;
    conf.PORT1=3;
    conf.SEQ1=3;
    Set_TCP_Config(conf);
    TCP_config_t confG= Get_TCP_Config();
    printf("DUT IP = %s \n",confG.DUT_IP);
    printf("TESTER IP = %s \n",confG.TESTER_IP);
    printf("DUT_Port  = %d \n",confG.DUT_Port);
    printf("Tester_Port  = %d \n",confG.Tester_Port);
    printf("msl  = %d \n",confG.msl);
    printf("Maxcon  = %d \n",confG.Maxcon);
    printf("ISN  = %d \n",confG.ISN);
    printf("mss  = %d \n",confG.mss);
    printf("window  = %d \n",confG.window);
    printf("nss  = %d \n",confG.nss);
    printf("ssz  = %d \n",confG.ssz);
    printf("tmx  = %d \n",confG.tmx);
    printf("tfn  = %d \n",confG.tfn);
    printf("wnp  = %d \n",confG.wnp);
    printf("uopt  = %d \n",confG.uopt);
    printf("PORT1  = %d \n",confG.PORT1);
    printf("SEQ1  = %d \n",confG.SEQ1);

   
    return 0;
}