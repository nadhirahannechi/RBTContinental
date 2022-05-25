#ifndef ICOMMODULE_H
#define ICOMMODULE_H

#include <functional>
#include <list>

//#define CONF_FILE_PATH "\\..\\IComModule\\conf\\testcase.conf"
#define CONF_FILE_PATH "\\..\\..\\com-module\\IComModule\\conf\\testcase.conf"

using namespace std::placeholders;

//start frame
typedef struct
{
    uint8_t OpCode=0x01;
    uint16_t Checksum=0;
}com_initializer_t;


//internal_protocol_frame_struct
typedef struct
{
    uint8_t Opcode=0x03;
    uint8_t TC8ID=0x01;
    uint16_t Test=0;
    uint16_t Api=0;
    uint16_t Reserved=0x0000;
    uint16_t Checksum=0;
}internal_protocol_frame_t;

//test_result_frame_struct
typedef struct
{
    uint8_t Opcode;
    uint8_t TC8ID;
    uint16_t Test;
    uint16_t Api;
    uint16_t ErrorCode;
    uint16_t Reserved;
    uint16_t Checksum;
}test_result_frame_t;
//Heartbeat struct
typedef struct
{
    uint8_t Opcode=0x04;
    uint16_t seq_num=0x0000;
    uint16_t Reserved1=0x0000;
    uint16_t Checksum=0;
}Heartbeat_t;

typedef struct
{
  int Test_ID;
  int API_ID;
}testID_APIID_conf_t;
typedef struct
{
    char DUT_IP[16]="169.254.159.220";                                            /* IP address of the DUT */
    char TESTER_IP[16]="169.254.159.221";                                         /* IP address of the Tester */
    unsigned int ICMP_IDENTIFIER=0x69db;                               /* Identifier used in the ICMP Messages to identify an ICMP Message */
    unsigned int ICMP_SEQUENCE_NUMBER=0x0001;                          /* Sequence Numbers used in ICMP Messages to identify an ICMP Message */
    unsigned int INVALID_CHECKSUM=0x0000;                              /* This is the checksum which is different from the calculated checksum */
    unsigned int FRAGMENT_REASSEMBLY_TIMEOUT=15;                   /* The fragment reassembly timeout. This defaults to 15 seconds. */
    unsigned int LISTEN_TIME=3;                                   /* This is the maximum time interval for which TESTER waits for an ICMP Reply packet.
                                                                   NOTE: This defaults to 3 seconds unless DUT configuration specifies otherwise. */
    unsigned int UNSUPPORTED_PROTOCOL=0;                          /* This is an IP protocol number that is not supported by the DUT. */
    unsigned int INVALID_ICMP_TYPE=44;                             /* This defines an invalid ICMP Type */
    char BROADCAST_ADDRESS[16];                                 /* IP broadcast address */
    unsigned int ORINGIN_TIMESTAMP_VALUE;
} ICMPv4Config_t;

typedef struct
{
        uint16_t DYNAMIC_ARP_CACHETIMEOUT;
        uint8_t HOST_1_IP[16];
        uint8_t DIface_0_IP[16];
        uint8_t ParamListenTime;
        uint8_t MAC_ADDR1[18];
        uint8_t MAC_ADDR2[18];
        uint8_t MAC_ADDR3[18];
        uint8_t DIFACE_O_MAC_ADDR[18];
        uint8_t ARBIT_MAC_ADDR[18];
        uint16_t ARP_HARDWARE_TYPE_UNKNOWN;
        uint8_t UNKNOWN_HW_ADDR_LEN;
        uint16_t ARP_PROTOCOL_UNKNOWN;
        uint8_t UNKNOWN_PROTCOL_ADDR_LEN;
        uint8_t ARP_TOLERANCE_TIME;
        uint8_t ETHERNET_ADDR_LEN;
        uint16_t ARP_HARDWARE_ETHERNET;
        uint8_t IP_ADDR_LEN;
        uint16_t ARP_PROTOCOL_IP;
        uint8_t OPERATION_REQUEST;
        uint8_t all_zeroes[18];
        uint8_t ETHERNET_BROADCAST_ADDR[18];
        uint8_t IP_FIRST_UNUSED_ADDR_INTERFACE_1[16];
        uint8_t OPERATION_RESPONSE;
}ARPConfig_t;
typedef struct
{
    uint16_t testerUDPPort;
    uint16_t testerUDPPort2;
    uint16_t unusedUDPDstPort1;
    uint16_t unusedUDPSrcPort;
    uint16_t DUTUDPPort;

    uint16_t udpUserDataSize;
    uint16_t calculatedUDPChecksum;
    uint16_t incorrectUDPChecksum;
    uint8_t UDPDefaultData[1024];
    uint16_t UDPDefaultDataLen;
    uint8_t UDPData[1024];
    uint16_t UDPDataLen;

    uint8_t Dst_IP_addr[16];
    uint8_t Host_1_IP[16];
    uint8_t Host_2_IP[16];
    uint8_t AIface_0_BcastIP[16];
    uint8_t allSystemMCastAddr[16];
}UDPConfig_t;
typedef struct
{
    uint8_t DUT_IP[16];                                                /* IP address of the DUT */
    uint8_t TESTER_IP[16];                                             /* IP address of the Tester */
    uint16_t DUT_PORT;                                          /* Port of the DUT */
    uint16_t TESTER_PORT;                                       /* Port of the Tester */
    uint16_t INVALID_CHECKSUM;                                  /* This is the checksum which is different from the calculated checksum */
    uint16_t TTL;                                               /* Time To Live field value which must be less than 2 */
    uint16_t LARGE_TTL_VALUE;                                   /* Time To Live value in a fragmented packet.
                                                                           NOTE: This value is greater than the initial reassemble timer setting <ipIniReassembleTimeout> */
    uint16_t LOW_TTL_VALUE;                                     /* Time To Live value in a fragmented packet.
                                                                           NOTE: This value is less than the initial reassemble timer setting <ipIniReassembleTimeout> */
    uint8_t DIRECTED_BROADCAST_ADDRESS[16];                            /* The directed broadcast address addresses a specific group in the network.
                                                                           {<Network-number>, -1}, e.g. 192.168.255.255 */
    uint8_t LIMITED_BROADCAST_ADDRESS[16];                             /* The limited broadcast address addresses every host on the connected physical network.
                                                                           {-1, -1} -> 255.255.255.255
                                                                           NOTE: It is not applicable for class D and class E */
    uint16_t LISTEN_TIME;                                       /* This is the maximum time interval for which TESTER waits for an ICMP Reply packet.
                                                                           This defaults to 3 seconds unless DUT configuration specifies otherwise. */
    uint16_t IP_INI_REASSEMBLE_TIMEOUT;                         /* Default value: depends on used controller. Time to wait for cleaning of the buffer
                                                                           with fragmented and malfored messages. */
    uint16_t FRAGMENT_REASSEMBLY_TIMEOUT;                       /* The fragment reassembly timeout. This defaults to 15 seconds */
    uint16_t IP_VERSION;                                        /* Indicates the version used in the current message */
    uint16_t IP_TYPE_ICMP;                                      /* Indicates the following protocol type, in this case ICMP */
    uint16_t IP_TYPE_TCP;                                       /* Indicates the following protocol type, in this case TCP */
    uint16_t MTU;                                                /* The Maximum Transmission Unit size */
} IPv4_config_t;


typedef struct
{
std::string S_interface="enp0s8";
std::string S_DUT_HW_Address="b8:27:eb:9c:25:5d";
std::string S_DUT_IP_Address="169.254.159.220";
std::string S_Tester_HW_Address="08:00:27:38:bd:1d";
std::string S_Tester_IP_Address="169.254.159.221";
}NetAPIConf_t;

typedef struct
{
    unsigned int DUT_Port=5001;
    unsigned int Tester_Port=6000;
    char DUT_IP[16]="169.254.159.220";
    char TESTER_IP[16]="169.254.159.221";
    unsigned int Maxcon=50;
    unsigned int ISN;
    unsigned int mss;
    unsigned int window;
    unsigned int nss;
    unsigned int ssz;
    unsigned int tmx;
    unsigned int tfn;
    unsigned int wnp;
    unsigned int uopt;
    unsigned int msl;
    unsigned int PORT1;
    unsigned int SEQ1;
} TCP_config_t;


typedef struct{

    uint8_t ParamLeaseTime;
    uint8_t ParamToleranceTime;
    uint8_t ParamProcessTime;
    uint8_t ParamListenTime;
    uint8_t ParamFirstRetransmissionInterval;
    uint8_t DHCP_MAGIC_COOKIE[4];
    uint8_t  IP_BROADCAST_ADDRESS[16];
    uint8_t  SERVERn_IP_ADDRESS[16];
    uint8_t  SERVERn_IP_POOL_L_M[16];
    uint8_t  SERVERn_IP_POOL_NETMASK[16];
    uint8_t  MAC_UNUSED_ADDRESS[18];
    uint8_t  IP_UNUSED_ADDRESS[16];
    uint8_t  IP_UNUSED_NET_MASK[16];
    uint16_t HIGH_LEASE_TIME;
    uint16_t VERY_HIGH_LEASE_TIME;
    uint8_t REMOTE_CLIENTn_LEASE_TIME;
    uint8_t REMOTE_CLIENTn_T1;
    uint8_t REMOTE_CLIENTn_T2;
    uint32_t extractedXID;
    uint8_t extractedSeconds;
    uint8_t SERVER2_IP_ADDRESS[16];
    uint8_t extractedSrcHwAddr[18];
}Dhcp_config_t;

typedef struct
{
    uint8_t opcode=05;
    TCP_config_t TCPConf;
    uint16_t Checksum=0;
} ComTCPConfig_t;







class IComModule
{
public:
    //_________________________________________
    // ::: Configuration and initialization :::

    /**********************************************
     * **************Uart config*******************
     * ********************************************
     */


    /*!
    \brief Open the serial port
    \param Device : Port name (COM1, COM2, ... for Windows )
    \param Bauds : Baud rate of the serial port.
    \n Supported baud rate for Windows :
            - 56000
            - 57600
            - 115200
            - 128000
            - 256000
    \return 0 success
    \return -1 device not found
    \return -2 error while opening the device
    \return -3 error while getting port parameters
    \return -4 Speed (Bauds) not recognized
    \return -5 error while writing port parameters
    */
    virtual int openPort(std::string &Device, const unsigned int &Bauds);

    /*!
    \brief Close the connection with the current device
    */
    virtual int closePort();

     /*********************************************
     * ************Ethernet config*****************
     * *******************************************/

    /*!
    \brief Establish Connection with Server
    \param Adress : "192.168.1.1"
    \param Port : 5001
    \return 0 Success
    \return -1 Error WSAStartup failed
    \return -2 Error create socket failed
    \return -3 Error connection failed
    */
    virtual int ConnectToTester(const std::string &Address, const int &Port);


    /*!
    \brief Start Test
    \param protocolID
    \param TestID
    \param API_ID
    \return 1 Success
    \return -1 Error Send Failed
    */
    virtual uint16_t StartTest(uint16_t &TestID, uint16_t &API_ID);



    /*!
     * \brief SetGlobalConfig
     * \param GlobalConfig {Tester_address ; tester_port, DUT_adress, DUT_port, MaxTestTimeout,... }
     * \return 0 success
     * \return -1 fail
     */
    virtual int SetNetAPIConfig(NetAPIConf_t NetAPIConf);

    virtual int SetIPV4Config(IPv4_config_t IPV4Conf);

    virtual int SetARPConfig(ARPConfig_t ARPConf);

    virtual int SetUDPConfig(UDPConfig_t UDPconfig);

    virtual int SetTCPConfig(TCP_config_t TCPConfig);

    virtual int SetICMPConfig(ICMPv4Config_t ICMPConf);





    /*!
    \brief Disconnect from Server
    \return 0 Success
    \return -1 Error Socket Shutdown failed
    */
    virtual int DisConnect();

    //Connection Register
    /*!
     \brief Register Tester Connection
     \param pointer to function
     \return 0 : status connected
     \return -1  status not connected
     */
    virtual void RegisterTesterConnection(std::function<void(bool)> on_connection_update);

    /*!
     \brief Register DUT Connection
     \param pointer to function
     \return 0 : status connected
     \return -1  status not connected
     */
    virtual void RegisterDutConnection(std::function<void(bool)> on_connection_update);


    virtual void IsBusyState(bool State);

private:

    /*!
     * \brief gen_crc16  function to calculate checksum f a given data
     * \param data : data to calculate its checksum
     * \param size : size of data
     * \return checksum
    */
    uint16_t gen_crc16(const uint8_t *data, uint16_t size);

    /*!
    \brief Recieve Data
    \return 0 Success
    \return -1 Error Failing while Recieving
    */
    virtual void Recieve();

    /*!
     * \brief HeartBeat
     * \param Address
     * \param Port
     * \return
     */
    //virtual int HeartBeat();

    virtual void Send();

    virtual uint16_t GetTestResult();

   // virtual int load_config_file(std::string filepath, std::vector<testID_APIID_conf_t> &testID_APIID_conf_vector);

    //virtual int get_API_ID(int TestID, std::vector<testID_APIID_conf_t> &_testID_APIID_conf_vector);

    virtual void HeartBeat();

};

#endif // ICOMMODULE_H
