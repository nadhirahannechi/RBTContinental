#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <iterator>
#include <thread>
#include <queue>
#include <future> // std::async, std::future
#include <chrono> // std::chrono::milliseconds
#include <string.h>
#include "ETLComModule_global.h"
#include "IComModule.h"
#include "time.h"

#define WIN32_LEAN_AND_MEAN
#define CRC16 0x8005
#define DEFAULT_BYTE_NUMBER 8
#define MAX_RETRY_ATTEMPTS 3
#define WAIT_BEFORE_RETRY 500
std::vector<testID_APIID_conf_t> global_config_vector;
// Handle on serial device
HANDLE hSerial;

//Socket for Tcp connection
SOCKET ConnectSocket;

//io parameters
int iResult;
const int recvbuflen = 256;
char recvbuf[recvbuflen];
bool IsConnected = false, IsBusy = false;
int api_array[512];

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
    uint8_t opcode=7;
    UDPConfig_t UDPconf;
    uint16_t Checksum=0;
}ComUDPConfig;
typedef struct
{
    uint8_t opcode=6;
   ARPConfig_t ARPconf;
    uint16_t Checksum=0;
}ComARPConfig;
typedef struct
{
    uint8_t opcode=8;
   ICMPv4Config_t ICMPconf;
    uint16_t Checksum=0;
}ComICMPConfig;
typedef struct
{
    uint8_t opcode=9;
   IPv4_config_t IPV4conf;
    uint16_t Checksum=0;
}ComIPV4Config;

std::queue<ComNetAPIConfig_t> NetAPI_config;
std::queue<Heartbeat_t> HbSent, HbRecieved;
std::queue<internal_protocol_frame_t> protocol_frame;
std::queue<test_result_frame_t> TestResult;
std::queue<ComTCPConfig_t> TCP_config;
std::queue<ComARPConfig> ARPConfig;
std::queue<ComUDPConfig> UDPConfig;
std::queue<ComICMPConfig> ICMPConfig;
std::queue<ComIPV4Config> IPV4Config;

std::function<void(bool)> on_connection_update1;
void myfunction()
{
    while (1)
    {
        on_connection_update1(IsConnected);
        Sleep(1000);
    }
}

//*********************public**************************//

//_________________________________________
// ::: Configuration and initialization :::

/**********************************************
                 * **************Uart config*******************
                 * ********************************************/

//Open Device
int IComModule::openPort(std::string &Device, const unsigned int &Bauds)
{
    // Open serial port
    std::cout << "opening Serial port\n";
    hSerial = CreateFileA(Device.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            std::cout << "Device not found\n";
            return -1; // Device not found
        }

        // Error while opening the device
        std::cout << "can not open device\n";
        return -2;
    }

    // Set parameters

    // Structure for the port parameters
    DCB dcbSerialParams;
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    std::cout << "getting port parameters\n";
    // Get the port parameters
    if (!GetCommState(hSerial, &dcbSerialParams))
        return -3;
    std::cout << "setting baud rate\n";
    // Set the speed (Bauds)
    switch (Bauds)
    {
    case 56000:
        dcbSerialParams.BaudRate = CBR_56000;
        break;
    case 57600:
        dcbSerialParams.BaudRate = CBR_57600;
        break;
    case 115200:
        dcbSerialParams.BaudRate = CBR_115200;
        break;
    case 128000:
        dcbSerialParams.BaudRate = CBR_128000;
        break;
    case 256000:
        dcbSerialParams.BaudRate = CBR_256000;
        break;
    default:
        return -4;
    }
    // 8 bit data
    std::cout << "setting byte size\n";
    dcbSerialParams.ByteSize = DEFAULT_BYTE_NUMBER;
    // One stop bit
    std::cout << "setting stop bits\n";
    dcbSerialParams.StopBits = ONESTOPBIT;
    // No parity
    std::cout << "setting parity\n";
    dcbSerialParams.Parity = NOPARITY;

    // Write the parameters
    std::cout << "writing parameters\n";
    if (!SetCommState(hSerial, &dcbSerialParams))
        return -5;

    // Opening successfull
    std::cout << "device opened successfully\n";
    return 0;
}

int IComModule::closePort()
{
    std::cout << "closing Serial Port\n";
    CloseHandle(hSerial);
    return 0;
}

/**********************************************
                             * ************Ethernet config*****************
                             * ********************************************/


int IComModule::ConnectToTester(const std::string &Address, const int &Port)
{
    int ret_value = -1;
    if (IsConnected == false)
    {
        WSADATA wsa;
        bool winsock_init_flag = false;
        int try_counter = 0;
            while (winsock_init_flag == false)
            {
                std::cout << "Initialising Winsock...\n";
                if (int wsa_ret = WSAStartup(MAKEWORD(2, 2), &wsa) != 0) //initiates use of the Winsock DLL by a process.
                {
                    try_counter++;
                    std::cout << "error while intializing!! error code = " << wsa_ret << "\n";
                    std::cout << "Fail to initialize Winsock \n Error Code : " << WSAGetLastError() << "\n";
                    if (try_counter >= MAX_RETRY_ATTEMPTS)
                    {
                        std::cout << "connection can't be established!!\n error while initializing socket\n";
                        ret_value = -2; // error while intializing winsock
                        break;
                    }
                    Sleep(WAIT_BEFORE_RETRY); //wait and try again after 500 ms
                }
                else
                {
                    std::cout << "Winsock Initialised.\n";
                    winsock_init_flag = true;
                    try_counter = 0;
                    bool winsock_creation_flag = false;
                    //Create a socket
                    while (winsock_creation_flag == false)
                    {
                        std::cout << "Trying to create socket\n";
                        if ((ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
                        {
                            try_counter++;
                            std::cout << "Could not create socket \n Error Code: " << WSAGetLastError() << "\n";
                            if (try_counter == MAX_RETRY_ATTEMPTS)
                            {
                                std::cout << "connection can't be established!!\n error while creating socket\n";
                                ret_value = -3; //error while creation socket
                                break;
                            }
                            Sleep(WAIT_BEFORE_RETRY);
                        }
                        else
                        {
                            u_long NonBlock = 1;
                            iResult = ioctlsocket(ConnectSocket, FIONBIO, &NonBlock);
                            if (iResult != NO_ERROR)
                            {
                                std::cout<<"ioctlsocket failed with error: "<<iResult<<"\n";
                            }
                            else
                            {
                                std::cout << "Socket created successfully.\n";
                                winsock_creation_flag = true;
                                try_counter = 0;
                                struct sockaddr_in server;
                                std::cout << "setting parameters: Adress,Port\n";
                                server.sin_addr.s_addr = inet_addr(Address.c_str());
                                server.sin_family = AF_INET;
                                server.sin_port = htons(Port);
                                std::cout << "Tester Adress: " << Address << "\n";
                                std::cout << "Tester Port: " << Port << "\n";
                                bool connection_established_flag = false;
                                while (connection_established_flag == false)
                                {
                                    std::cout << "Connecting to " << Address << " Port " << Port << "\n";
                                    //Connect to remote server
                                    connect(ConnectSocket, (struct sockaddr *)&server, sizeof(server));
                                    fd_set fdsread, fdswrite;
                                    FD_ZERO(&fdsread);
                                    FD_ZERO(&fdswrite);
                                    FD_SET(ConnectSocket, &fdsread);
                                    FD_SET(ConnectSocket, &fdswrite);
                                    timeval tv;
                                    tv.tv_sec = 1;
                                    tv.tv_usec = 0;

                                    if (select(ConnectSocket + 1, &fdsread, &fdswrite, 0, &tv) != 1)
                                    {
                                        try_counter++;
                                        std::cout << "Cannot Connect to tester \n Error Code: " << WSAGetLastError() << "\n";
                                        if (try_counter == MAX_RETRY_ATTEMPTS)
                                        {
                                            std::cout << "connection can't be established!!\n error while connecting socket\n";
                                            ret_value = -4; //error while connecting to server
                                            break;
                                        }
                                        Sleep(WAIT_BEFORE_RETRY);
                                    }
                                    else
                                    {
                                        std::cout << "connection established successfully\n";
                                        connection_established_flag = true;
                                        std::cout << "initializing  start frame \n";
                                        com_initializer_t start_frame;
                                        start_frame.Checksum = gen_crc16((uint8_t *)&start_frame, sizeof(com_initializer_t) - 2);
                                        std::cout << "sending start frame\n";
                                        bool send_flag = false;
                                        while (send_flag == false)
                                        {

                                            if (send(ConnectSocket, (char *)&start_frame, sizeof(com_initializer_t), 0) < 0)
                                            {
                                                try_counter++;
                                                std::cout << "Start frame Send failed \n Error Code: " << WSAGetLastError() << "\n";
                                                if (try_counter == MAX_RETRY_ATTEMPTS)
                                                {
                                                    std::cout << "error sending start frame!!!\n";
                                                    ret_value = -5; //error while sending start frame
                                                    break;
                                                }
                                                Sleep(WAIT_BEFORE_RETRY);
                                            }
                                            else
                                            {
                                                std::cout << "start frame sent\n";
                                                send_flag = true;
                                                recv(ConnectSocket, recvbuf, recvbuflen, 0);
                                                if (select(ConnectSocket + 1, &fdsread, &fdswrite, 0, &tv) != 1)
                                                {
                                                    std::cout << "recv failed with error: " << WSAGetLastError() << "\n";
                                                    ret_value = -6; //exit error reciving start frame from tester
                                                }
                                                else
                                                {
                                                    std::cout << "start frame recieved\n";
                                                    /*Heartbeat_t Hb;
                                                    if (HbSent.empty() == true)
                                                    {
                                                        HbSent.push(Hb);
                                                    }
                                                    else
                                                    {
                                                        HbSent.pop();
                                                        HbSent.push(Hb);
                                                    }*/
                                                    IsConnected = true;
                                                    Sleep(500);
                                                    std::thread t1(&IComModule::Send, this);
                                                    t1.detach();
                                                    std::cout << "Send thread started\n";
                                                    std::thread t2(&IComModule::Recieve, this);
                                                    t2.detach();
                                                    //std::thread t3(&IComModule::HeartBeat, this);
                                                    //t3.detach();
                                                    std::cout << "recieve thread started\n";
                                                    //NetAPIConf_t NetAPIConf;
                                                    //SetNetAPIConfig(NetAPIConf);

                                                    ret_value = 0; //success!!! connected and ready to send and recieve data
                                                }

                                                break; //exit sending start frame loop
                                            }
                                        }
                                        break; //exit connecting to serverloop
                                    }
                                }
                            }
                            break; //exit creation socket loop
                        }
                    }
                    break; //exit initializing socket loop
                }
            }
    }
    else
    {
        std::cout << "disconnecting\n";
        if (int disconnect_ret = DisConnect() == 0)
        {
            Sleep(500);
            std::cout << "reconnecting to Address: " << Address << " on Port" << Port << "\n";
            ConnectToTester(Address, Port);
            ret_value = 0;
        }
        else
        {
            ret_value = disconnect_ret;
        }
    }

    return ret_value;
}

int IComModule::DisConnect()
{
    int ret_value = -1;
    if (IsConnected == true)
    {

        // shutdown the connection since no more data will be sent
        std::cout << "shutdowning Socket\n";
        iResult = shutdown(ConnectSocket, SD_SEND);
        if (iResult == SOCKET_ERROR)
        {
            std::cout << "shutdown failed with error: " << WSAGetLastError() << "\n";
            closesocket(ConnectSocket);
            WSACleanup();
            ret_value = -2; //error while shutdowning socket
        }
        else
        {
            std::cout << "closing socket\n";
            closesocket(ConnectSocket);
            IsConnected = false;
            WSACleanup();
            std::cout << "socket closed successfully\n";
            ret_value = 0; //success
        }
    }
    else
    {
        std::cout << "connection is already closed\n";
        ret_value = -1; // error while closing socket
    }
    return ret_value;
}

uint16_t IComModule::StartTest(uint16_t &TestID, uint16_t &API_ID)
{
    uint16_t test_result = 1;
    if (IsConnected == true)
    {
        internal_protocol_frame_t frame;
        frame.Test = TestID;
        std::cout << "test started: Test" << TestID << "\n";
        std::cout << "Api to be executed : Api" << API_ID << "\n";
        frame.Api = API_ID;
        frame.Checksum = gen_crc16((uint8_t *)&frame, sizeof(internal_protocol_frame_t) - 2);
        std::cout<<"Adding frame to queue...\n";
        protocol_frame.push(frame);
        std::cout<<"the queue contains"<<protocol_frame.size()<<" item\n";
        Sleep(500);
        test_result = GetTestResult();
    }
    else{
        std::cout<<"HMI not connected to Tester \n";
    }
    return test_result;
}

int IComModule::SetNetAPIConfig(NetAPIConf_t NetAPIConf)
{
    int ret = -1;
    if (IsConnected == true)
    {
        std::cout << "setting NetAPI config\n";
        ComNetAPIConfig_t ComNetAPIConf;
        strcpy(ComNetAPIConf.S_interface,NetAPIConf.S_interface.c_str());
        strcpy(ComNetAPIConf.S_DUT_HW_Address,NetAPIConf.S_DUT_HW_Address.c_str());
        strcpy(ComNetAPIConf.S_DUT_IP_Address,NetAPIConf.S_DUT_IP_Address.c_str());
        strcpy(ComNetAPIConf.S_Tester_HW_Address,NetAPIConf.S_Tester_HW_Address.c_str());
        strcpy(ComNetAPIConf.S_Tester_IP_Address,NetAPIConf.S_Tester_IP_Address.c_str());
        std::cout<<"yyyyyyyyyyyyyyyy\n";
        std::cout<<ComNetAPIConf.S_interface;
        std::cout<<ComNetAPIConf.S_DUT_HW_Address;
        std::cout<<ComNetAPIConf.S_DUT_IP_Address;
        std::cout<<ComNetAPIConf.S_Tester_HW_Address;
        std::cout<<ComNetAPIConf.S_DUT_IP_Address;
        ComNetAPIConf.Checksum = gen_crc16((uint8_t *)&ComNetAPIConf, sizeof(ComNetAPIConfig_t) - 2);
        NetAPI_config.push(ComNetAPIConf);
        ret = 0;
    }
    else
        ret = -1;
    return ret;
}

int IComModule::SetTCPConfig(TCP_config_t TCPConfig)
{
    int ret = -1;
    if (IsConnected == true)
    {
        std::cout << "setting TCP config\n";
        ComTCPConfig_t ComTCPConf;
        ComTCPConf.TCPConf = TCPConfig;
        ComTCPConf.Checksum = gen_crc16((uint8_t *)&ComTCPConf, sizeof(ComTCPConfig_t) - 2);
        TCP_config.push(ComTCPConf);
        ret = 0;
    }
    else
        ret = -1;
    return ret;
}
int IComModule::SetICMPConfig(ICMPv4Config_t ICMPconfig)
{
    int ret = -1;
    if (IsConnected == true)
    {   //std::cout<<"ggggggggggggg\n"<<ICMPconfig.HOST_1_IP<<"\n";
        std::cout << "setting ICMP config\n";
        ComICMPConfig ComICMPConf;
        ComICMPConf.ICMPconf = ICMPconfig;
        ComICMPConf.Checksum = gen_crc16((uint8_t *)&ComICMPConf, sizeof(ComICMPConfig) - 2);
        ICMPConfig.push(ComICMPConf);
        ret = 0;
    }
    else
        ret = -1;
    return ret;
}
int IComModule::SetIPV4Config(IPv4_config_t IPV4config)
{
    int ret = -1;
    if (IsConnected == true)
    {   //std::cout<<"ggggggggggggg\n"<<ICMPconfig.HOST_1_IP<<"\n";
        std::cout << "setting IPV4 config\n";
        ComIPV4Config ComIPV4Conf;
        ComIPV4Conf.IPV4conf = IPV4config;
        ComIPV4Conf.Checksum = gen_crc16((uint8_t *)&ComIPV4Conf, sizeof(ComIPV4Config) - 2);
        IPV4Config.push(ComIPV4Conf);
        ret = 0;
    }
    else
        ret = -1;
    return ret;
}
int IComModule::SetARPConfig(ARPConfig_t ARPconfig)
{
    int ret = -1;
    if (IsConnected == true)
    {   std::cout<<"ggggggggggggg\n"<<ARPconfig.HOST_1_IP<<"\n";
        std::cout << "setting TCP config\n";
        ComARPConfig ComARPConf;
        ComARPConf.ARPconf = ARPconfig;
        ComARPConf.Checksum = gen_crc16((uint8_t *)&ComARPConf, sizeof(ComARPConfig) - 2);
        ARPConfig.push(ComARPConf);
        ret = 0;
    }
    else
        ret = -1;
    return ret;
}
int IComModule::SetUDPConfig(UDPConfig_t UDPconfig)
{
    int ret = -1;
    if (IsConnected == true)
    {
        std::cout << "setting TCP config\n";
        ComUDPConfig ComUDPConf;
        ComUDPConf.UDPconf = UDPconfig;
        ComUDPConf.Checksum = gen_crc16((uint8_t *)&ComUDPConf, sizeof(ComUDPConfig) - 2);
        UDPConfig.push(ComUDPConf);
        ret = 0;
    }
    else
        ret = -1;
    return ret;
}

void IComModule::RegisterTesterConnection(std::function<void(bool)> on_connection_update)
{
    //to be defined
    on_connection_update1 = on_connection_update;
    std::thread thread(&myfunction);
    thread.detach();
}

void IComModule::HeartBeat()
{
    std::cout << "Heartbeat thread started\n";
    int HbQueueCheck = 0;
    while (IsConnected == true)
    {
        while (HbRecieved.empty() == true)
        {
            if (IsBusy == false)
            {
                std::cout << "checking for recieved Heartbeat\n";
                HbQueueCheck++;
                if (HbQueueCheck > 20)
                {
                    std::cout << "attempting to close socket...server not responding!!!\n please check your server!!\n";
                    IsConnected = false;
                    DisConnect();

                    break;
                }
                Sleep(100);
            }
            if (HbRecieved.empty() == false)
            {
                HbQueueCheck = 0;
                Heartbeat_t RecievedHb = HbRecieved.front();
                HbRecieved.pop();
                RecievedHb.seq_num++;
                HbSent.push(RecievedHb);
                Sleep(100);
            }
        }
    }
    std::cout << "Closing Heartbeat Thread\n";
}

void IComModule::RegisterDutConnection(std::function<void(bool)> on_connection_update)
{
    //to be defined
    if (IsConnected == true)
        on_connection_update(true);
    else
        on_connection_update(false);
}

void IComModule::IsBusyState(bool State)
{
    if (State == true)
    {
        std::cout << "Com is busy\n";
        IsBusy = true;
    }
    else if (State == false)
    {
        std::cout << "Com is free\n";
        IsBusy = false;
    }
}

/********************************************
            * ***********Private Functions**************
            ********************************************/

void IComModule::Send()
{
    while (IsConnected == true)
    {
        while (!NetAPI_config.empty())
        {
            std::cout << "the NetAPI config queue contains" << NetAPI_config.size() << "\n";
            ComNetAPIConfig_t NetAPIConf = NetAPI_config.front();
            if (send(ConnectSocket, (char *)&NetAPIConf, sizeof(ComNetAPIConfig_t), 0) < 0)
            {
                std::cout << "NetAPI Config Send failed \n Error Code: " << WSAGetLastError() << "\n";
            }
            else
            {
                std::cout << "NetAPI Config sent successfully\n";
                NetAPI_config.pop();
                break;
            }
        }
        while (!ARPConfig.empty())
        {
            std::cout << "the ARPconf config queue contains" << ARPConfig.size() << "\n";
            ComARPConfig ComARPconf = ARPConfig.front();
            if (send(ConnectSocket, (char *)&ComARPconf, sizeof(ComARPConfig), 0) < 0)
            {
                std::cout << "ARPconf Config Send failed \n Error Code: " << WSAGetLastError() << "\n";
            }
            else
            {
                std::cout << "ARPconf Config sent successfully\n";
                ARPConfig.pop();
                break;
            }
        }
        while (!IPV4Config.empty())
        {
            std::cout << "the IPV4conf config queue contains" << IPV4Config.size() << "\n";
            ComIPV4Config ComIPV4conf = IPV4Config.front();
            if (send(ConnectSocket, (char *)&ComIPV4conf, sizeof(ComIPV4Config), 0) < 0)
            {
                std::cout << "IPV4conf Config Send failed \n Error Code: " << WSAGetLastError() << "\n";
            }
            else
            {
                std::cout << "IPV4conf Config sent successfully\n";
                IPV4Config.pop();
                break;
            }
        }
        while (!UDPConfig.empty())
        {
            std::cout << "the UDPconf config queue contains" << UDPConfig.size() << "\n";
            ComUDPConfig ComUDPconf = UDPConfig.front();
            if (send(ConnectSocket, (char *)&ComUDPconf, sizeof(ComUDPConfig), 0) < 0)
            {
                std::cout << "UDPconf Config Send failed \n Error Code: " << WSAGetLastError() << "\n";
            }
            else
            {
                std::cout << "UDPconf Config sent successfully\n";
                UDPConfig.pop();
                break;
            }
        }
        while (!ICMPConfig.empty())
        {
            std::cout << "the ICMPconf config queue contains" << ICMPConfig.size() << "\n";
            ComICMPConfig ComICMPconf = ICMPConfig.front();
            if (send(ConnectSocket, (char *)&ComICMPconf, sizeof(ComICMPConfig), 0) < 0)
            {
                std::cout << "ICMPconf Config Send failed \n Error Code: " << WSAGetLastError() << "\n";
            }
            else
            {
                std::cout << "ICMPconf Config sent successfully\n";
                ICMPConfig.pop();
                break;
            }
        }
        while (!TCP_config.empty())
        {
            std::cout << "the TCP config queue contains" << TCP_config.size() << "\n";
            ComTCPConfig_t TCPConf = TCP_config.front();
            if (send(ConnectSocket, (char *)&TCPConf, sizeof(ComTCPConfig_t), 0) < 0)
            {
                std::cout << "TCP Config Send failed \n Error Code: " << WSAGetLastError() << "\n";
            }
            else
            {
                std::cout << "TCP Config sent successfully\n";
                TCP_config.pop();
                break;
            }
        }
        while (!protocol_frame.empty())
        {
            std::cout << "the protocol frame queue contains" << protocol_frame.size() << "\n";
            internal_protocol_frame_t prot_frame = protocol_frame.front();

            if (send(ConnectSocket, (char *)&prot_frame, sizeof(internal_protocol_frame_t), 0) < 0)
            {
                std::cout << "protocol frame Send failed \n Error Code: " << WSAGetLastError() << "\n";
            }
            else
            {
                std::cout << "protocol frame sent successfully\n";
                protocol_frame.pop();
                break;
            }
        }
        while (!HbSent.empty() && protocol_frame.empty())
        {
            if (IsBusy == false)
            {
                std::cout << "the heartbeat queue contains" << HbSent.size() << "\n";
                Heartbeat_t Hb = HbSent.front();

                if (send(ConnectSocket, (char *)&Hb, sizeof(Heartbeat_t), 0) < 0)
                {
                    std::cout << "heartbeat Send failed \n Error Code: " << WSAGetLastError() << "\n";
                    IsConnected = false;
                }
                else
                {
                    std::cout << "heartbeat frame sent \n";
                    HbSent.pop();
                    break;
                }
            }
        }
        Sleep(500);
    }
    std::cout << "closing send thread...\n";
}

void IComModule::Recieve()
{
    while (IsConnected == true)
    {
        memset(recvbuf, 0xff, sizeof(recvbuf));
        while (((iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0)) == SOCKET_ERROR) && (IsConnected == true))
        {
            int ierr= WSAGetLastError();
                     if (ierr==WSAEWOULDBLOCK) // currently no data available
                     {
                         Sleep(500);  // wait and try again
                         continue;
                     }
                     break;

        }

        if ((iResult > 0) && (recvbuf[0] != 0x00))
        {
            std::cout << "number of bytes recieved : " << iResult << "\n";
            enum OpCode
            {
                config_opcode = 0x02,
                protocol_opcode = 0x03,
                heartbeat_opcode = 0x04
            };
            OpCode recv_frame_opcode = (OpCode)recvbuf[0];
            switch (recv_frame_opcode)
            {
            case 0x02:
            {
                std::cout << "global config hqs been set successfullyn";
                break;
            }
            case 0x03:
            {

                test_result_frame_t *result = (test_result_frame_t *)&recvbuf[0];
                if (gen_crc16((uint8_t *)result, sizeof(test_result_frame_t) - 2) == result->Checksum)
                {
                    std::cout << "test result frame recieved \n";
                    TestResult.push(*result);
                }
                else
                {
                    std::cout << "wrong CRC!! \n recieved frame:" << result->Opcode << ", " << result->TC8ID << ", " << result->Test << ", " << result->Api << ", " << result->Checksum << "\n";
                }
                break;
            }
            case 0x04:
            {

                Heartbeat_t *Hb = (Heartbeat_t *)&recvbuf[0];
                if (gen_crc16((uint8_t *)Hb, sizeof(Heartbeat_t) - 2) == Hb->Checksum)
                {
                    std::cout << "HeartBeat frame recieved \n";
                    HbRecieved.push(*Hb);
                }
                else
                {
                    std::cout << "wrong CRC!! \n recieved frame:" << Hb->Opcode << ", " << Hb->seq_num << ", " << Hb->Checksum << "\n";
                }
                break;
            }
            default:
                break;
            }
        }
        else
        {
            std::cout << "unrecognized frame recieved\n";
        }
        Sleep(500);
    }
    std::cout << "closing recieve thread...\n";
}

uint16_t IComModule::GetTestResult()
{
    int TestResultCheck = 0;
    uint16_t ret = 0x0001;
    if (IsConnected == true)
    {
            while (TestResult.empty()==true)
            {
            Sleep(1000);
            //std::cout << "Cheking for result...\n";
            time_t now;

            time(&now);
            char *tm = ctime(&now);
            std::cout << "Cheking for result...\n"<<tm<<"\n";
            TestResultCheck++;
            /*if (TestResultCheck > 20)
            {
                ret = 0x0003;
                IsBusy=false;
                std::cout << "error getting result from server!!! check server state...\n";
                return ret;
            }
            if (TestResult.empty()==false)
                break;*/
            }

        TestResultCheck = 0;
        test_result_frame_t test_result = TestResult.front();
        if (test_result.Opcode == 03)
        {
            ret = test_result.ErrorCode;
        }
        else
        {
            ret = 0x0004;
            std::cout<<"False frame recieved\n";
            std::cout<<ret<<"\n";
        }
        TestResult.pop();
    }
    return ret;
}

uint16_t IComModule::gen_crc16(const uint8_t *data, uint16_t size)
{
    uint16_t out = 0;
    int bits_read = 0, bit_flag;

    /* Sanity check: */
    if (data == NULL)
        return 0;

    while (size > 0)
    {
        bit_flag = out >> 15;

        /* Get next bit: */
        out <<= 1;
        out |= (*data >> bits_read) & 1; // item a) work from the least significant bits

        /* Increment bit counter: */
        bits_read++;
        if (bits_read > 7)
        {
            bits_read = 0;
            data++;
            size--;
        }

        /* Cycle check: */
        if (bit_flag)
            out ^= CRC16;
    }

    // item b) "push out" the last 16 bits
    int i;
    for (i = 0; i < 16; ++i)
    {
        bit_flag = out >> 15;
        out <<= 1;
        if (bit_flag)
            out ^= CRC16;
    }

    // item c) reverse the bits
    uint16_t crc = 0;
    i = 0x8000;
    int j = 0x0001;
    for (; i != 0; i >>= 1, j <<= 1)
    {
        if (i & out)
            crc |= j;
    }

    return crc;
}

/*int IComModule::load_config_file(std::string filepath, std::vector<testID_APIID_conf_t> &testID_APIID_conf_vector)
{
    uint16_t counter = 1;
    std::cout << "loading config file\n";
    std::ifstream file(filepath);
    if (file.is_open())
    {
        std::string str;

        while (std::getline(file, str))
        {
            //std::cout <<"case 0x"<<counter<<":\n    {\n        test_result.ErrorCode = "<<str<<"();\n        test_result.Checksum =gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);\n        break;\n    } \n";
            if (str.find('#') == std::string::npos && str.find('=') != std::string::npos)
            {
                //std::cout<< str<<"\n";
                std::string TestID = str.substr(0, str.find('='));
                std::string APIID = str.substr(str.find('=') + 1, str.length());
                if ((TestID.find_first_not_of("0123456789") == std::string::npos) && (APIID.find_first_not_of("0123456789") == std::string::npos))
                {
                    std::cout << "test ID=" << stoi(TestID) << "\n";
                    std::cout << "API ID=" << stoi(APIID) << "\n";
                    std::cout << "***************************\n";
                    testID_APIID_conf_vector.push_back(testID_APIID_conf_t{(stoi(str.substr(0, str.find('=')))), (stoi(str.substr(str.find('=') + 1, str.length())))});
                }
                else
                {
                    std::cout << "invalid line found at line " << counter << ": " << str << "\n";
                }
            }

            counter++;
        }
        if (testID_APIID_conf_vector.empty())
        {
            std::cout << "file existing but does not contain any valid data\n";
            return -2;
        }
    }
    else
    {
        std::cout << filepath << " not found!!\n";
        return -1;
    }
    return 0;
}

int IComModule::get_API_ID(int TestID, std::vector<testID_APIID_conf_t> &_testID_APIID_conf_vector)
{
    if (!_testID_APIID_conf_vector.empty())
    {
        for (unsigned int counter = 0; counter < _testID_APIID_conf_vector.size(); counter++)
        {
            if (_testID_APIID_conf_vector[counter].Test_ID == TestID)
            {
                std::cout << "API ID=" << _testID_APIID_conf_vector[counter].API_ID << "\n";
                return _testID_APIID_conf_vector[counter].API_ID;
            }
        }
        std::cout << "Test_ID not found!!!\n";
        return -1;
    }
    else
    {
        std::cout << "vector is empty!!\n";
        return -2;
    }
} */
