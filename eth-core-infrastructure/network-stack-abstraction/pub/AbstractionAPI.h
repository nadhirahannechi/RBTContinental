/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   Abstration_api.h
 *  @brief  Abstraction API.
 *  \author Aymen SALEM - Abderrahim JAMAOUI
 *
 *  \addtogroup SW_package
 *  \{
 */

/*==================================================================================================
*                                        Include Files
==================================================================================================*/

#ifndef API_H
#define API_H

#include <stdint.h>
/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/
const int MAX_SIZE_IP = 65535;
/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
//tcp options type code 
#define TCP_NO_OPERATION_OPTION 1
#define TCP_END_OF_OPTION 0
/*==================================================================================================
*                                             TYPES
==================================================================================================*/

enum protocol_code
{
    ETHERNET        =        143,
    IP              =        4,
    ICMP            =        1,
    UDP             =        17,
    TCP             =        6,
    PAYLAOD         =        108,
    ARP             =        92,
    DHCP            =        300,
    TCP_OPTIONS     =        400,
    IP_Options      =        500,
    ICMP_Timestamp  =        600
};

enum eth_field
{
    ETH_destinationMAC,
    ETH_sourceMac,
    ETH_type
};

enum ip_field
{
    IP_IHL,
    IP_Version,
    IP_Tos,
    IP_TotalLength,
    IP_Identification,
    IP_Offset,
    IP_Ttl,
    IP_Protocol,
    IP_HeaderChecksum,
    IP_SrcAddress,
    IP_DstAddress
};

typedef struct{
uint8_t data[MAX_SIZE_IP];
int length;
} IP_Packet ;

typedef struct{
uint8_t data[MAX_SIZE_IP];
int length;
} IP_OPTION_Packet ;

enum payload_field
{
    PAYLOAD_data,
    PAYLOAD_length
};

typedef struct{
uint8_t data[MAX_SIZE_IP];
int length;
} TCP_Packet ;

enum tcp_field
{
    TCP_SrcPort,
    TCP_DstPort,
    TCP_SeqNumber,
    TCP_AckNumber,
    TCP_Reserved,
    TCP_HeaderLength,
    TCP_FlagFIN,
    TCP_FlagSYN,
    TCP_FlagRST,
    TCP_FlagPSH,
    TCP_FlagACK,
    TCP_FlagURG,
    TCP_Flagece,
	TCP_Flagcwr,
    TCP_WindowsSize,
    TCP_Checksum,
    TCP_UrgPointer
};

typedef  struct{
uint8_t data[MAX_SIZE_IP];
int length;
} UDP_Packet;

enum udp_field
{
  UDP_SrcPort,
  UDP_DstPort,
  UDP_Length,
  UDP_Checksum
};

typedef  struct{
uint8_t data[MAX_SIZE_IP];
int length;
} ICMP_Packet;

enum icmp_field
{
    ICMP_type ,
    ICMP_code,
    ICMP_checksum,
    ICMP_identifier,
    ICMP_sequence,
    ICMP_pointer
};

typedef  struct{
uint8_t data[MAX_SIZE_IP];
int length;
} ICMP_Timestamp_Packet;

enum icmp_timestamp_field
{
    ICMP_Timestamp_type,
    ICMP_Timestamp_code,
    ICMP_Timestamp_checksum,
    ICMP_Timestamp_identifier,
    ICMP_Timestamp_sequence,
    ICMP_Timestamp_originate_timestamp,
    ICMP_Timestamp_receive_timestamp,
    ICMP_Timestamp_transmit_timestamp
};

typedef  struct{
uint8_t data[MAX_SIZE_IP];
int length;
} ICMP_Option_Packet;

enum ip_option_field
{
    ipt_code,
    ipt_len,
    ipt_ptr,
    ipt_flg,
    ipt_oflw,
    data
};

typedef  struct{
uint8_t data[MAX_SIZE_IP];
int length;
} DHCP_Packet;

enum dhcp_field
{
    packet_type,
    type_of_hardware_address,
    length_of_hardware_address,
    hops,
    random_transaction_id_number,
    seconds_used_in_timing,
    flags,
    ciaddr,
    yiaddr,
    siaddr,
    giaddr,
    chaddr,
    sname,
    file,
    options
};

typedef struct{
    unsigned char  data[100];
    int len;
}dhcp_value;

enum dhcp_message_type
{ DHCPDISCOVER = 1,
DHCPOFFER,
DHCPREQUEST,
DHCPDECLINE,
DHCPACK,
DHCPNAK,
DHCPRELEASE,
DHCPINFORM
};

typedef  struct{
uint8_t data[MAX_SIZE_IP];
int length;
} ARP_Packet;

enum arp_field
{
    hardware_address_format,
    protocol_address_format,
    Length_hardware_address,
    Length_protocol_address,
    opcode,
    sender_hardware_address,
    sender_protocol_address,
    target_hardware_address,
    target_protocol_address
};

typedef struct
    {
    uint16_t arp_operation;
    char  spa[16];
    char  tpa[16];
    
    char  SrcAdd[16];
    char  dstAdd[16];
    int Srcport;
    int Dstport;
    }Packet_filter;

typedef struct 
{
    const uint8_t type=2;
    uint8_t length;
    uint16_t value;
}TCP_MSS_OPTION;

typedef struct
{
    char S_interface [25];
    char S_DUT_HW_Address [18];
    char S_DUT_IP_Address[16];
    char S_Tester_HW_Address[19];  
    char S_Tester_IP_Address[16];
} Net_API_config_t;
/*==================================================================================================
*                                        Global Variables
==================================================================================================*/



/*==================================================================================================
*                                     local Functions prototypes
==================================================================================================*/

/*==================================================================================================
*                                     Global Functions prototypes
==================================================================================================*/

/***************************************************************************************************
*
*   FUNCTION NAME: Set_Network_Abstration_API_Config
*
***************************************************************************************************/
/**
* @brief
*  This function will configure the global values.
*  
* @parameters
* @param[in] configuration: The new configuration. 
*
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void Set_Network_Abstration_API_Config(Net_API_config_t configuration);

/***************************************************************************************************
*
*   FUNCTION NAME: Get_Network_Abstration_API_Config
*
***************************************************************************************************/
/**
* @brief
*  This function will return the configuration values.
*  
* @parameters
* @param[in] void 
*
* @return Current configutation
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
Net_API_config_t Get_Network_Abstration_API_Config(void);

/***************************************************************************************************
*
*   FUNCTION NAME: CreateTCP
*
***************************************************************************************************/
/**
* @brief
*  This function will return a pointer to the created packet.
*  This packet includes Ethernet header, IP_Header, TCPHeader and a payload initialized with default values.
*  
* @parameters
* @param[in] void
*
* @return TCP_Packet : a pointer to the created packet.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TCP_Packet CreateTCP();

/***************************************************************************************************
*
*   FUNCTION NAME: EditTCPField
*
***************************************************************************************************/
/**
* @brief
*  This function will change a specific field in certain layer with an input value
*  
* @par Parameters
* @param[in] TCP_Packet : a pointer to the packet TCP
* @param[in] Layer : the corresponding layer of the field to be change
* @param[in] Field : a number corresponds to the field position 
* @param[in] Value : The new value of the specific field
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int EditTCPField( TCP_Packet* packet , int Layer, int field, void* value);

/***************************************************************************************************
*
*   FUNCTION NAME: GetTCPField
*
***************************************************************************************************/
/**
* @brief
*  This function will return specific field in certain layer in specific packet.
*  
* @par Parameters
* @param[in] TCP_Packet : a pointer to the packet TCP
* @param[in] Layer : the layer position of the field to be return
* @param[in] Field : a number corresponds to the field position 
*
* @return  intptr_t : the value of the selected field
*         
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
intptr_t GetTCPField(TCP_Packet* packet , int layer , int field );

/***************************************************************************************************
*
*   FUNCTION NAME: SendTCP
*
***************************************************************************************************/
/**
* @brief
*  This function will send the packet.
*  
* @par Parameters
* @param[in] TCP_Packet : a pointer to the packet TCP
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int SendTCP(TCP_Packet packet);

/***************************************************************************************************
*
*   FUNCTION NAME: ListenTCP
*
***************************************************************************************************/
/**
* @brief
*  This function will return the first TCP packet received with the specific input Packet_filter
*  
* @par Parameters
* @param[in] Packet_filter : a Packet_filter 
*
* @return TCP_Packet : a pointer to the first received packet TCP
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TCP_Packet ListenTCP(Packet_filter f) ;

/***************************************************************************************************
*
*   FUNCTION NAME: Compute_checksum
*
***************************************************************************************************/
/**
* @brief
*  This function will compute the correct checksum and set it in the correct position of Data_packet
*  
* @par Parameters
* @param[in] TCP_Packet* : a pointer of tcp packet 
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_Compute_checksum(TCP_Packet*);

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_Verify_Correct_checksum
*
***************************************************************************************************/
/**
* @brief
*  This function will check if the correct checksum is present or not
*  
* @par Parameters
* @param[in] TCP_Packet : a tcp packet 
*
* @return int   0 if correct checksum is present
*               1 if incorrect checksum is present
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_Verify_Correct_checksum(TCP_Packet);

/***************************************************************************************************
*
*   FUNCTION NAME: TCP_Add_Option
*
***************************************************************************************************/
/**
* @brief
*  This function will add new tcp option to the packet
*  
* @par Parameters
* @param[in] TCP_Packet* : a pointer of tcp packet
* @param[in] uint8_t* : a pointer of data option 
* @param[in] uint8_t : the length of option data 
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int TCP_Add_Option(TCP_Packet* packetTCP,uint8_t* Data,uint8_t Length);
/***************************************************************************************************
*
*   FUNCTION NAME: Get_TCP_Option_Value
*
***************************************************************************************************/
/**
* @brief
*  This function will  get the tcp option value
*  
* @par Parameters
* @param[in] TCP_Packet :  tcp packet
* @param[in] uint8_t : the kind of the needed option 
* @param[in] uint8_t* : A pointer of uint8_t : return the tcp option value if exist
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int Get_TCP_Option_Value(TCP_Packet packetTCP, uint8_t optionKind, uint8_t* value);

/***************************************************************************************************
*
*   FUNCTION NAME: CreateUDP
*
***************************************************************************************************/
/**
* @brief
*  This function will return a pointer to the created packet.
*  This packet includes Ethernet header, IP_Header, UDPHeader and a payload initialized with default values
*  
* @parameters
* @param[in] void
*
* @return UDP_Packet  : a pointer to the created packet.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
UDP_Packet CreateUDP();

/***************************************************************************************************
*
*   FUNCTION NAME: EditUDPField
*
***************************************************************************************************/
/**
* @brief
*  This function will change a specific field in certain layer with an input value
*  
* @par Parameters
* @param[in] UDP_Packet : a pointer to the packet UDP
* @param[in] Layer : the corresponding layer of the field to be change
* @param[in] Field : a number corresponds to the field position 
* @param[in] Value : The new value of the specific field
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int EditUDPField( UDP_Packet* packet , int Layer, int field, void* value);

/***************************************************************************************************
*
*   FUNCTION NAME: GetUDPField
*
***************************************************************************************************/
/**
* @brief
*  This function will return specific field in certain layer in specific packet.
*  
* @par Parameters
* @param[in] UDP_Packet : a pointer to the packet UDP
* @param[in] Layer : the layer position of the field to be return
* @param[in] Field : a number corresponds to the field position 
*
* @return  void * : the value of the selected field
*         
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
intptr_t GetUDPField(UDP_Packet *packet , int layer , int field );

/***************************************************************************************************
*
*   FUNCTION NAME: SendUDP
*
***************************************************************************************************/
/**
* @brief
*  This function will send the packet.
*  
* @par Parameters
* @param[in] UDP_Packet : a pointer to the packet UDP
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int SendUDP(UDP_Packet packet);

/***************************************************************************************************
*
*   FUNCTION NAME: ListenUDP
*
***************************************************************************************************/
/**
* @brief
*  This function will return the first UDP packet received with the specific input Packet_filter
*  
* @par Parameters
* @param[in] Packet_filter : a Packet_filter 
*
* @return UDP_Packet : a pointer to the first received packet UDP
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
UDP_Packet ListenUDP(Packet_filter f, int timeout) ;


/***************************************************************************************************
*
*   FUNCTION NAME: UDP_Compute_checksum
*
***************************************************************************************************/
/**
* @brief
*  This function will compute the correct checksum and set it in the correct position of Data_packet
*  
* @par Parameters
* @param[in] UDP_Packet* : a pointer of udp packet 
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int UDP_Compute_checksum(UDP_Packet*);

/***************************************************************************************************
*
*   FUNCTION NAME: UDP_Verify_Correct_checksum
*
***************************************************************************************************/
/**
* @brief
*  This function will check if the correct checksum is present or not
*  
* @par Parameters
* @param[in] UDP_Packet : a udp packet 
*
* @return int   0 if correct checksum is present
*               1 if incorrect checksum is present
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int UDP_Verify_Correct_checksum(UDP_Packet);

/***************************************************************************************************
*
*   FUNCTION NAME: CreateICMP
*
***************************************************************************************************/
/**
* @brief
*  This function will return a pointer to the created packet.
*  This packet includes Ethernet header, IP_Header, ICMPHeader and a payload initialized with default values
*  
* @parameters
* @param[in] void
*
* @return ICMP_Packet  : a pointer to the created packet.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
ICMP_Packet CreateICMP();

/***************************************************************************************************
*
*   FUNCTION NAME: EditICMPField
*
***************************************************************************************************/
/**
* @brief
*  This function will change a specific field in certain layer with an input value
*  
* @par Parameters
* @param[in] ICMP_Packet : a pointer to the packet ICMP
* @param[in] Layer : the corresponding layer of the field to be change
* @param[in] Field : a number corresponds to the field position 
* @param[in] Value : The new value of the specific field
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int EditICMPField( ICMP_Packet* packet , int Layer, int field, void* value);

/***************************************************************************************************
*
*   FUNCTION NAME: GetICMPField
*
***************************************************************************************************/
/**
* @brief
*  This function will return specific field in certain layer in specific packet.
*  
* @par Parameters
* @param[in] ICMP_Packet : a pointer to the packet ICMP
* @param[in] Layer : the layer position of the field to be return
* @param[in] Field : a number corresponds to the field position 
*
* @return  intptr_t : the value of the selected field
*         
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
intptr_t GetICMPField(ICMP_Packet* packet , int layer , int field );

/***************************************************************************************************
*
*   FUNCTION NAME: SendICMP
*
***************************************************************************************************/
/**
* @brief
*  This function will send the packet.
*  
* @par Parameters
* @param[in] ICMP_Packet : ICMP packet
*
* @return int   0 if Done with success
*               1 if send failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int SendICMP(ICMP_Packet packetICMP);

/***************************************************************************************************
*
*   FUNCTION NAME: ListenICMP
*
***************************************************************************************************/
/**
* @brief
*  This function will return the first ICMP packet received with the specific input Packet_filter
*  
* @par Parameters
* @param[in] Packet_filter : a Packet_filter 
* @param[in] timeout : listen timeout
* @return ICMP_Packet : a pointer to the first received packet ICMP
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
ICMP_Packet ListenICMP(Packet_filter f, int timeout);


/***************************************************************************************************
*
*   FUNCTION NAME: ICMP_Compute_checksum
*
***************************************************************************************************/
/**
* @brief
*  This function will compute the correct checksum and set it in the correct position of Data_packet
*  
* @par Parameters
* @param[in] ICMP_Packet* : a pointer of icmp packet 
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ICMP_Compute_checksum(ICMP_Packet*);

/***************************************************************************************************
*
*   FUNCTION NAME: CreateARP
*
***************************************************************************************************/
/**
* @brief
*  This function will return a pointer to the created packet.
*  This packet includes Ethernet header and ARP header initialized with default values
*  
* @parameters
* @param[in] void
*
* @return ARP_Packet  : a pointer to the created packet.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
ARP_Packet CreateARP();

/***************************************************************************************************
*
*   FUNCTION NAME: EditARPField
*
***************************************************************************************************/
/**
* @brief
*  This function will change a specific field in certain layer with an input value
*  
* @par Parameters
* @param[in] ARP_Packet : a pointer to the packet ARP
* @param[in] Layer : the corresponding layer of the field to be change
* @param[in] Field : a number corresponds to the field position 
* @param[in] Value : The new value of the specific field
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int EditARPField( ARP_Packet* packet , int Layer, int field, void* value);

/***************************************************************************************************
*
*   FUNCTION NAME: GetARPField
*
***************************************************************************************************/
/**
* @brief
*  This function will return specific field in certain layer in specific packet.
*  
* @par Parameters
* @param[in] ARP_Packet : a pointer to the packet ARP
* @param[in] Layer : the layer position of the field to be return
* @param[in] Field : a number corresponds to the field position 
*
* @return  intptr_t : the value of the selected field
*         
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
intptr_t GetARPField(ARP_Packet *packet , int layer , int field );

/***************************************************************************************************
*
*   FUNCTION NAME: SendARP
*
***************************************************************************************************/
/**
* @brief
*  This function will send the packet.
*  
* @par Parameters
* @param[in] ARP_Packet : a pointer to the packet ARP
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int SendARP(ARP_Packet packet);

/***************************************************************************************************
*
*   FUNCTION NAME: ListenARP
*
***************************************************************************************************/
/**
* @brief
*  This function will return the first ARP packet received with the specific input Packet_filter
*  
* @par Parameters
* @param[in] Packet_filter : a Packet_filter 
*
* @return ARP_Packet : a pointer to the first received packet ARP
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
ARP_Packet ListenARP(Packet_filter f, int timeout);


/***************************************************************************************************
*
*   FUNCTION NAME: CreateDHCP
*
***************************************************************************************************/
/**
* @brief
*  This function will return a pointer to the created packet.
*  This packet includes Ethernet_header, IP_Header, UDP_Header and the DHCP_Header initialized with default values
*  
* @parameters
* @param[in] void
*
* @return DHCP_Packet  : a pointer to the created packet.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
DHCP_Packet CreateDHCP();

/***************************************************************************************************
*
*   FUNCTION NAME: EditDHCPField
*
***************************************************************************************************/
/**
* @brief
*  This function will change a specific field in certain layer with an input value
*  
* @par Parameters
* @param[in] DHCP_Packet : a pointer to the packet DHCP
* @param[in] Layer : the corresponding layer of the field to be change
* @param[in] Field : a number corresponds to the field position 
* @param[in] Value : The new value of the specific field
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int EditDHCPField( DHCP_Packet* packet , int Layer, int field, void* value);

/***************************************************************************************************
*
*   FUNCTION NAME: GetDHCPField
*
***************************************************************************************************/
/**
* @brief
*  This function will return specific field in certain layer in specific packet.
*  
* @par Parameters
* @param[in] DHCP_Packet : a pointer to the packet DHCP
* @param[in] Layer : the layer position of the field to be return
* @param[in] Field : a number corresponds to the field position 
*
* @return  intptr_t : the value of the selected field
*         
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
intptr_t GetDHCPField(DHCP_Packet *packet , int layer , int field );

/***************************************************************************************************
*
*   FUNCTION NAME: SendDHCP
*
***************************************************************************************************/
/**
* @brief
*  This function will send the packet.
*  
* @par Parameters
* @param[in] DHCP_Packet : a pointer to the packet DHCP
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int SendDHCP(DHCP_Packet packet);

/***************************************************************************************************
*
*   FUNCTION NAME: ListenDHCP
*
***************************************************************************************************/
/**
* @brief
*  This function will return the first DHCP packet received with the specific input Packet_filter
*  
* @par Parameters
* @param[in] Packet_filter : a Packet_filter 
*
* @return DHCP_Packet : a pointer to the first received packet DHCP
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
DHCP_Packet ListenDHCP(Packet_filter f, int timeout);


/***************************************************************************************************
*
*   FUNCTION NAME: CreateIP
*
***************************************************************************************************/
/**
* @brief
*  This function will return a pointer to the created packet.
*  This packet includes Ethernet header, IP_Header and a payload initialized with default values
*  
* @parameters
* @param[in] void
*
* @return IP_Packet  : a pointer to the created packet.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
IP_Packet CreateIP();

/***************************************************************************************************
*
*   FUNCTION NAME: EditIPField
*
***************************************************************************************************/
/**
* @brief
*  This function will change a specific field in certain layer with an input value
*  
* @par Parameters
* @param[in] IP_Packet : a pointer to the packet IP
* @param[in] Layer : the corresponding layer of the field to be change
* @param[in] Field : a number corresponds to the field position 
* @param[in] Value : The new value of the specific field
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int EditIPField( IP_Packet* packet , int Layer, int field, void* value);

/***************************************************************************************************
*
*   FUNCTION NAME: GetIPField
*
***************************************************************************************************/
/**
* @brief
*  This function will return specific field in certain layer in specific packet.
*  
* @par Parameters
* @param[in] IP_Packet : a pointer to the packet IP
* @param[in] Layer : the layer position of the field to be return
* @param[in] Field : a number corresponds to the field position 
*
* @return  intptr_t : the value of the selected field
*         
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
intptr_t GetIPField(IP_Packet* packet , int layer , int field );

/***************************************************************************************************
*
*   FUNCTION NAME: SendIP
*
***************************************************************************************************/
/**
* @brief
*  This function will send the packet.
*  
* @par Parameters
* @param[in] IP_Packet : a pointer to the packet IP
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int SendIP(IP_Packet packetIP);

/***************************************************************************************************
*
*   FUNCTION NAME: ListenIP
*
***************************************************************************************************/
/**
* @brief
*  This function will return the first IP packet received with the specific input Packet_filter
*  
* @par Parameters
* @param[in] Packet_filter : a Packet_filter 
* @param[in] timeout : listen timeout
* @return IP_Packet : a pointer to the first received packet IP
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
IP_Packet ListenIP(Packet_filter f, int timeout);

/***************************************************************************************************
*
*   FUNCTION NAME: IP_Compute_checksum
*
***************************************************************************************************/
/**
* @brief
*  This function will compute the correct checksum and set it in the correct position of Data_packet
*  
* @par Parameters
* @param[in] IP_Packet* : a pointer of ip packet 
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IP_Compute_checksum(IP_Packet*);

/***************************************************************************************************
*
*   FUNCTION NAME: CreateICMPOption
*
***************************************************************************************************/
/**
* @brief
*  This function will return a pointer to the created packet.
*  This packet includes Ethernet header, IP_Header + IP_Option, ICMPHeader and a payload initialized with default values 
* @par Parameters
* @param[in] IP_Packet* : a pointer of ip packet 
*
* @return ICMP_Packet  : a pointer to the created packet.
*              
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
ICMP_Option_Packet CreateICMPOption();

/***************************************************************************************************
*
*   FUNCTION NAME: SendICMPOption
*
***************************************************************************************************/
/**
* @brief
*  This function will send the packet.
* @par Parameters
* @param[in] ICMP_Option_Packet : ICPM packet
*
* @return int   0 if Done with success
*               1 if send failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int SendICMPOption(ICMP_Option_Packet packetICMPOption);


/***************************************************************************************************
*
*   FUNCTION NAME: EditICMPOPTIONField
*
***************************************************************************************************/
/**
* @brief
*  This function will change a specific field in certain layer with an input value
*  
* @par Parameters
* @param[in] ICMP_Packet : a pointer to the packet ICMP
* @param[in] Layer : the corresponding layer of the field to be change
* @param[in] Field : a number corresponds to the field position 
* @param[in] Value : The new value of the specific field
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int EditICMPOptionField( ICMP_Option_Packet *packetICMPoption, int proto_code, int field, void * value);

/***************************************************************************************************
*
*   FUNCTION NAME: GetICMPField
*
***************************************************************************************************/
/**
* @brief
*  This function will return specific field in certain layer in specific packet.
*  
* @par Parameters
* @param[in] ICMP_Packet : a pointer to the packet ICMP
* @param[in] Layer : the layer position of the field to be return
* @param[in] Field : a number corresponds to the field position 
*
* @return  intptr_t : the value of the selected field
*         
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
intptr_t GetICMPOptionField(ICMP_Option_Packet* packetICMPoption, int proto_code, int field);


/***************************************************************************************************
*
*   FUNCTION NAME: cleanUpSocket
*
***************************************************************************************************/
/**
* @brief
*  This function will close socket
*  
* @par Parameters
* @param[in] void 
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int cleanUpSocket(void);
/***************************************************************************************************
*
*   FUNCTION NAME: CreateIPoption
*
***************************************************************************************************/
/**
* @brief
*  This function will return a pointer to the created packet.
*  This packet includes Ethernet header, IP_Header + IP_Option and a payload initialized with default values 
* @par Parameters
* @param[in] IP_Packet* : a pointer of ip packet 
*
* @return ICMP_Packet  : a pointer to the created packet.
*              
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
IP_OPTION_Packet CreateIPoption();
/***************************************************************************************************
*
*   FUNCTION NAME: SendIPOption
*
***************************************************************************************************/
/**
* @brief
*  This function will send the packet.
* @par Parameters
* @param[in] ICMP_Option_Packet : IPv4 packet
*
* @return int   0 if Done with success
*               1 if send failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int SendIPOption(IP_OPTION_Packet packetIP);

/***************************************************************************************************
*
*   FUNCTION NAME: ICMP_Option_Compute_checksum
*
***************************************************************************************************/
/**
* @brief
*  This function will compute the correct checksum and set it in the correct position of Data_packet
*  
* @par Parameters
* @param[in] TCP_Packet* : a pointer of icmp packet 
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ICMP_Option_Compute_checksum(ICMP_Option_Packet* icmp_op_p);
/***************************************************************************************************
*
*   FUNCTION NAME: CreateICMPTimestamp
*
***************************************************************************************************/
/**
* @brief
*  This function will return a pointer to the created packet.
*  This packet includes Ethernet header, IP_Header, ICMP_Timestamp_Header and a payload initialized with default values
*  
* @parameters
* @param[in] void
*
* @return ICMP_Timestamp_Packet  : a pointer to the created packet.
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
ICMP_Timestamp_Packet CreateICMPTimestamp();

/***************************************************************************************************
*
*   FUNCTION NAME: SendICMPTimestamp
*
***************************************************************************************************/
/**
* @brief
*  This function will send the packet.
*  
* @par Parameters
* @param[in] ICMP_Timestamp_Packet : ICMP packet
*
* @return int   0 if Done with success
*               1 if send failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int SendICMPTimestamp(ICMP_Timestamp_Packet packetICMPTimestamp);

/***************************************************************************************************
*
*   FUNCTION NAME: EditICMPField
*
***************************************************************************************************/
/**
* @brief
*  This function will change a specific field in certain layer with an input value
*  
* @par Parameters
* @param[in] ICMP_Packet : a pointer to the packet ICMP
* @param[in] Layer : the corresponding layer of the field to be change
* @param[in] Field : a number corresponds to the field position 
* @param[in] Value : The new value of the specific field
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int EditICMPTimestampField( ICMP_Timestamp_Packet *packetICMPTimestamp, int proto_code, int field, void * value);
/***************************************************************************************************
*
*   FUNCTION NAME: GetICMPTimestampField
*
***************************************************************************************************/
/**
* @brief
*  This function will return specific field in certain layer in specific packet.
*  
* @par Parameters
* @param[in] ICMP_Timestamp_Packet : a pointer to the packet ICMP Timestamp
* @param[in] proto_code : the layer position of the field to be return
* @param[in] Field : a number corresponds to the field position 
*
* @return  intptr_t : the value of the selected field
*         
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
intptr_t GetICMPTimestampField(ICMP_Timestamp_Packet* packet , int proto_code , int field);


/***************************************************************************************************
*
*   FUNCTION NAME: ListenICMPTimestamp
*
***************************************************************************************************/
/**
* @brief
*  This function will return the first ICMP Timestamp packet received with the specific input Packet_filter
*  
* @par Parameters
* @param[in] Packet_filter : a Packet_filter 
* @param[in] timeout : listen timeout
* @return ICMP_Packet : a pointer to the first received packet ICMP timestamp
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
ICMP_Timestamp_Packet ListenICMPTimestamp(Packet_filter f, int timeout);
/***************************************************************************************************
*
*   FUNCTION NAME: ICMP_Timestamp_Compute_checksum
*
***************************************************************************************************/
/**
* @brief
*  This function will compute the correct checksum and set it in the correct position of Data_packet
*  
* @par Parameters
* @param[in] ICMP_Packet* : a pointer of icmp timestamp packet 
*
* @return int   0 if Done with success
*               1 if edit failed
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int ICMP_Timestamp_Compute_checksum(ICMP_Timestamp_Packet* icmp_p);
#endif
