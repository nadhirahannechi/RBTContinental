#ifndef INTERCOM_TCPSERVER_H
#define INTERCOM_TCPSERVER_H
/**
 * This is a basic C++ TCP server class
 * This code was written to run on linux machines. 
 * The code is documented, so I hope you find it easy to change it 
 * to suit your needs if needed to.
 * The server class supports multiple clients.
 * 
 * written by Othmani Tarek
 **/

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <future>
#include <chrono>
#include <mutex>          // std::mutex
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <thread>
#include "AbstractionAPI.h"
#include "TCPconfig.h"
#include "config.h"
#include "UDP_config.h"
#include "ICMPv4config.h"
#include "IPv4config.h"


#define TRUE 1
#define FALSE 0
#define CRC16 0x8005
#define buffer_size 8192
#define Port 9876

uint16_t gen_crc16(const uint8_t *data, uint16_t size);

//Heartbeat struct
struct Heartbeat
{
    uint8_t Opcode = 04;
    uint16_t seq_num;
    uint16_t Reserved1;
    uint16_t Checksum;
};

typedef struct
{
    uint8_t opcode=02;
    char S_interface [25];
    char S_DUT_HW_Address [25];
    char S_DUT_IP_Address[25];
    char S_Tester_HW_Address[25];
    char S_Tester_IP_Address[25];
    uint16_t Checksum=0;
} ComNetAPIConfig_t;

typedef struct
{
    uint8_t opcode=05;
    TCP_config_t TCPConf;
    uint16_t Checksum=0;
} ComTCPConfig_t;

typedef struct
{
    uint8_t opcode=07;
    UDP_config_t UDPconf;
    uint16_t Checksum=0;
}ComUDPConfig;
typedef struct
{
    uint8_t opcode=06;
   Arp_config_t ARPconf;
    uint16_t Checksum=0;
}ComARPConfig;
typedef struct
{
    uint8_t opcode=8;
   ICMPv4_config_t ICMPconf;
    uint16_t Checksum=0;
}ComICMPConfig;
typedef struct
{
    uint8_t opcode=9;
   IPv4_config_t IPV4conf;
    uint16_t Checksum=0;
}ComIPV4Config;


class TCPServer
{
public:
    //constructor
    TCPServer();
    //Destructor
    ~TCPServer();
    //thread launch
    void start();

private:
    //start server
    void ServerStart();
};

#endif
