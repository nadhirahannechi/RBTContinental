#include "TCPconfig.h"
#include "TCPcommon.h"

void Set_TCP_Config(TCP_config_t conf)
{
    printf("Set TCP config");
    //set TCP configutation values
    TCPConfig.DUT_Port = conf.DUT_Port;
    TCPConfig.Tester_Port = conf.Tester_Port;
    strcpy(TCPConfig.DUT_IP, conf.DUT_IP);  
    strcpy(TCPConfig.TESTER_IP, conf.TESTER_IP);
    TCPConfig.Maxcon = conf.Maxcon;
    TCPConfig.msl = conf.msl;
    TCPConfig.ISN=conf.ISN;
    TCPConfig.mss=conf.mss;
    TCPConfig.window=conf.window;
    TCPConfig.nss=conf.nss;
    TCPConfig.ssz=conf.ssz;
    TCPConfig.tmx=conf.tmx;
    TCPConfig.tfn=conf.tfn;
    TCPConfig.wnp=conf.wnp;
    TCPConfig.uopt=conf.uopt;
    TCPConfig.PORT1=conf.PORT1;
    TCPConfig.SEQ1=conf.SEQ1;
}

TCP_config_t Get_TCP_Config()
{
    printf("Get TCP config");
    TCP_config_t conf;
    conf.DUT_Port = TCPConfig.DUT_Port;
    conf.Tester_Port = TCPConfig.Tester_Port;
    strcpy(conf.DUT_IP, TCPConfig.DUT_IP);  
    strcpy(conf.TESTER_IP, TCPConfig.TESTER_IP);
    conf.Maxcon = TCPConfig.Maxcon;
    conf.msl = TCPConfig.msl;
    conf.Maxcon = TCPConfig.Maxcon;
    conf.msl = TCPConfig.msl;
    conf.ISN=TCPConfig.ISN;
    conf.mss=TCPConfig.mss;
    conf.window=TCPConfig.window;
    conf.nss=TCPConfig.nss;
    conf.ssz=TCPConfig.ssz;
    conf.tmx=TCPConfig.tmx;
    conf.tfn=TCPConfig.tfn;
    conf.wnp=TCPConfig.wnp;
    conf.uopt=TCPConfig.uopt;
    conf.PORT1=TCPConfig.PORT1;
    conf.SEQ1=TCPConfig.SEQ1;
    return conf;
}