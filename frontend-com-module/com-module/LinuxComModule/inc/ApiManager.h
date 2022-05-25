#ifndef INTERCOM_APIMANAGER_H
#define INTERCOM_APIMANAGER_H
#include <stdint.h>
#include <string>
#include <iostream>
#include "TCP_BASICS.h"
#include "TCP_CHECKSUM.h"
#include "TCP_UNACCEPTABLE.h"
#include "TCP_FLAGS_INVALID.h"
#include "TCP_FLAGS_PROCESSING.h"
#include  "TCP_MSS_OPTIONS.h"
#include "TCP_OUT_OF_ORDER.h"
#include "TCP_RETRANSMISSION_TO.h"
#include "UDP_MessageFormat.h"
#include "UDP_DatagramLength.h"
#include "UDP_Padding.h"
#include "UDP_FIELDS.h"
#include "UDP_USER_INTERFACE.h"
#include "UDP_INTRODUCTION.h"
#include "UDP_INVALID_ADDRESSES.h"
#include "packet_generation.h"
#include "packet_reception.h"
#include "ICMPv4_ERROR.h"
#include "ICMPv4_TYPE.h"
#include "IPv4_HEADER.h"
#include "IPv4_CHECKSUM.h"
#include "IPv4_TTL.h"
#include "IPv4_VERSION.h"
#include "IPv4_ADDRESSING.h"
#include "IPv4_FRAGMENTS.h"
#include "IPv4_REASSEMBLY.h"
#include "IPv4_AUTOCONF_ADDRESS_SELECTION.h"
#include "IPv4_AUTOCONF_ANNOUNCING.h"
#include "IPv4_AUTOCONF_CONFLICT.h"
#include "IPv4_AUTOCONF_LINKLOCAL_PACKETS.h"
#include "IPv4_AUTOCONF_NETWORK_PARTITIONS.h"
#include "DHCPv4_CLIENT_SUMMARY.h"
#include "DHCPv4_CLIENT_PROTOCOL.h"
#include "DHCPv4_CLIENT_ALLOCATING.h"
#include "DHCPv4_CLIENT_PARAMETERS.h"
#include "DHCPv4_CLIENT_USAGE.h"
#include "DHCPv4_CLIENT_CONSTRUCTING_MESSAGES.h"
#include "DHCPv4_CLIENT_REQUEST.h"
#include "DHCPv4_CLIENT_INITIALIZATION_ALLOCATION.h"
#include "DHCPv4_CLIENT_REACQUISITION.h"
#include "SOMEIPSRV_FORMAT.hpp"
#include "SOMEIPSRV_OPTIONS.hpp"
#include "SOMEIPSRV_SD_MESSAGE.hpp"
#include "SOMEIPSRV_SD_BEHAVIOR.hpp"
#include "SOMEIPSRV_BASIC.hpp"
#include "SOMEIPSRV_ONWIRE.hpp"
#include "SOMEIPSRV_RPC.hpp"
#include "TCP_SEQUENCE.h"
#include "TCP_CONTROL_FLAGS.h"
#include "TCP_CLOSING.h"
#include "TCP_PROBING_WINDOWS.h"
#include "TCP_NAGLE.h"
#include "TCP_CONNECTION_ESTAB.h"
#include "TCP_HEADER.h"
#include "TCP_ACKNOWLEDGEMENT.h"
#include "TCP_CALL_RECEIVE.h"
#include "TCP_CALL_ABORT.h"




#define CRC16 0x8005

//internal_protocol_frame_struct
struct internal_protocol_frame_t
{
    uint8_t Opcode;
    uint8_t TC8ID;
    uint16_t Test;
    uint16_t Api;
    uint16_t Reserved;
    uint16_t Checksum;
};

//test_result_frame_struct
struct test_result_frame_t
{
    uint8_t Opcode;
    uint8_t TC8ID;
    uint16_t Test;
    uint16_t Api;
    uint16_t ErrorCode;
    uint16_t Reserved;
    uint16_t Checksum;
};



struct TestConfig
{
    uint8_t Opcode=0x04;
    std::string Tester_adress;
    int Tester_port;
    std::string Dut_adress;
    int Dut_Port;
    uint16_t Checksum;
    //other parameter to be specified
};

/*!
* \brief gen_crc16  function to calculate checksum f a given data
* \param data : data to calculate its checksum
* \param size : size of data
* \return checksum
*/
uint16_t gen_crc16(const uint8_t *data, uint16_t size);

test_result_frame_t APIrunner(internal_protocol_frame_t test_result);

#endif
