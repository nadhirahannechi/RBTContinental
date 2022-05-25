#include "DHCPv4_CLIENT_PROTOCOL.h"
#include "DHCP_config.h"
#include <stdio.h>
#include <string.h>
#include "TestabilityProtocolTypes.h"
#include "TestabilityProtocol_api.h"
#include "AbstractionAPI.h"
#include "DHCP_state_machine.h"
#define N_EXECUTE 2
uint8_t genaralStartTest5, dhcpInitClient5, genaralEndTest5;

extern Dhcp_config_t DHCP_config;

/***************************************************************************************************
*
*   FUNCTION NAME: DhcpInitClient_cb5
*
***************************************************************************************************/
/**
* @brief
*  Initialize the DHCP Client callback
*
* @par
* @param[in] Function pointer to user defined callback.
*
* @return
* -
*
* @note
* -
*
* @warning
* -
*
**/
void DhcpInitClient_cb5 (TP_ResultID_t r)
{
    dhcpInitClient5 = r;
}




/***************************************************************************************************
*
*   FUNCTION NAME: GenaralStartTest_cb5
*
***************************************************************************************************/
/**
* @brief
*  Initialize the General Start Test callback
*
* @par
* @param[in] Function pointer to user defined callback.
*
* @return
* -
*
* @note
* -
*
* @warning
* -
*
**/
void GenaralStartTest_cb5(TP_ResultID_t r)
{
    genaralStartTest5 = r;
}




/***************************************************************************************************
*
*   FUNCTION NAME:GenaralEndTest_cb5
*
***************************************************************************************************/
/**
* @brief
*  Initialize the General End Test callback
*
* @par
* @param[in] Function pointer to user defined callback.
*
* @return
* -
*
* @note
* -
*
* @warning
* -
*
**/
void GenaralEndTest_cb5(TP_ResultID_t r)
{
    genaralEndTest5 = r;
}


/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_PROTOCOL_01
 *
 ***************************************************************************************************/
/**
* @brief
*  DHCPv4_CLIENT_PROTOCOL_01: First four octets of the 'options' field of the DHCP message
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_PROTOCOL_01 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_PROTOCOL_01()
{
    DHCP_Packet dhcp_packet;
    Packet_filter filter;
    char* Option;
    genaralStartTest5 = N_EXECUTE;
    dhcpInitClient5 = N_EXECUTE;
    genaralEndTest5 = N_EXECUTE;
    //start test
    TP_GenaralStartTest(GenaralStartTest_cb5);
    if (genaralStartTest5 == 0)
    {
        printf("\n Genaral Test will start with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_PROTOCOL_01 failed : Genaral Start Test  \n");
        return 1;
    }
    /* 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
       2. TESTER: Externally cause DUT to bring up <DIface-0>
          Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5" 
    */
    text interf;
    interf.dataLength = 20;
    interf.Data = DHCP_config.DIface_0;
    TP_DhcpInitClient(DhcpInitClient_cb5, interf);
    while (dhcpInitClient5 == N_EXECUTE) {usleep(100);}
    if (dhcpInitClient5 == 0)
    {
        printf("\n Configure DhcpClient with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_PROTOCOL_01 failed : Configure DhcpClient \n");
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }
    //Initialize the filter
    strcpy(filter.SrcAdd, "0.0.0.0");
    strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);                  
    // 3. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
    dhcp_packet = ListenDHCP(filter, DHCP_config.ParamListenTime);
    //length > 0 => a DHCP Packet is received to a server
    if(dhcp_packet.length > 0)
    {
        printf("\n A DHCP Packet is received with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_PROTOCOL_01 failed : Listen DHCP \n");
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }

    // Extract the options from DHCPDISCOVER
    Option = (char*)GetDHCPField(&dhcp_packet, DHCP, options);
    // 4. verify that the DUT: Sends "DHCPDISCOVER" Message  (packet_type of dhcp_field)
    if ((uint8_t)GetDHCPField(&dhcp_packet, DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPDISCOVER)
    {
        printf("\n A DHCPDISCOVER is received with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_PROTOCOL_01 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }

    //<DHCP-MAGIC-COOKIE>
    if (Option[0] == 99 && Option[1] == 130 && Option[2] == 83 && Option[3] == 99)
    {
        printf("\n Test DHCPv4_CLIENT_PROTOCOL_01 : The first four octets of DHCP Options is set to <DHCP-MAGIC-COOKIE> with success \n");
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 0;
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_PROTOCOL_01 failed : The first four octets of DHCP Options is NOT set to <DHCP-MAGIC-COOKIE> \n");
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }
}

/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_PROTOCOL_02
 *
 ***************************************************************************************************/
/**
* @brief
*  DHCPv4_CLIENT_PROTOCOL_02: "DHCP message type" option present in DHCPDISCOVER Message.
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_PROTOCOL_02 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_PROTOCOL_02()
{
    DHCP_Packet dhcp_packet;
    Packet_filter filter;
    char* Option;
    genaralStartTest5 = N_EXECUTE;
    dhcpInitClient5 = N_EXECUTE;
    genaralEndTest5 = N_EXECUTE;

    //start test
    TP_GenaralStartTest(GenaralStartTest_cb5);
    if (genaralStartTest5 == 0)
    {
        printf("\n Genaral Test will start with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_PROTOCOL_02 failed : Genaral Start Test  \n");
        return 1;
    }
    // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
    // 2. TESTER: Externally cause DUT to bring up <DIface-0>
    //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
    text interf;
    interf.dataLength = 20;
    interf.Data = DHCP_config.DIface_0;
    TP_DhcpInitClient(DhcpInitClient_cb5, interf);
    while (dhcpInitClient5 == N_EXECUTE) {usleep(100);}
    if (dhcpInitClient5 == 0)
    {
        printf("\n Configure DhcpClient with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_PROTOCOL_02 failed : Configure DhcpClient \n");
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }
    //Initialize the filter
    strcpy(filter.SrcAdd, "0.0.0.0");
    strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
    // 3. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
    dhcp_packet = ListenDHCP(filter, DHCP_config.ParamListenTime);
    //length > 0 => a DHCP Packet is received to a server
    if(dhcp_packet.length > 0)
    {
        printf("\n A DHCP Packet is received with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_PROTOCOL_02 failed : Listen DHCP \n");
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }
    // Extract the options from DHCPDISCOVER
    Option = (char*)GetDHCPField(&dhcp_packet, DHCP, options);
    /* packet_type : 1 for request , 2 for reply*/
    // 4. verify that the DUT: Sends "DHCPDISCOVER" Message  (packet_type of dhcp_field)
    if ((uint8_t)GetDHCPField(&dhcp_packet, DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPDISCOVER)
    {
        printf("\n Test DHCPv4_CLIENT_PROTOCOL_02 : A DHCPDISCOVER is received with success \n");
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 0;
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_PROTOCOL_02 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }
    //<Message Type Option> is set to 53
    /*if (Option[4] == 53)
    {
        printf("\n Test DHCPv4_CLIENT_PROTOCOL_02 : The Message Type Option of DHCP Options is set 53 with success \n");
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 0;
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_PROTOCOL_02 failed : The Message Type Option of DHCP Options is NOT set to 53 \n");
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }*/
}

/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_PROTOCOL_03
 *
 ***************************************************************************************************/
/**
* @brief
*  DHCPv4_CLIENT_PROTOCOL_03: "DHCP message type" option present in DHCPREQUEST Message
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_PROTOCOL_03 failed
* @note
* @warning
**/
int DHCPv4_CLIENT_PROTOCOL_03()
{
    DHCP_Packet dhcp_packet_1, dhcp_packet_2;
    Packet_filter filter;
    uint32_t extractedXID;
    char* Option;
    genaralStartTest5 = N_EXECUTE;
    dhcpInitClient5 = N_EXECUTE;
    genaralEndTest5 = N_EXECUTE;

    //start test
    TP_GenaralStartTest(GenaralStartTest_cb5);
    if (genaralStartTest5 == 0)
    {
        printf("\n Genaral Test will start with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_PROTOCOL_03 failed : Genaral Start Test  \n");
        return 1;
    }
    // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
    // 2. TESTER: Externally cause DUT to bring up <DIface-0>
    //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
    text interf;
    interf.dataLength = 20;
    interf.Data = DHCP_config.DIface_0;
    TP_DhcpInitClient(DhcpInitClient_cb5, interf);
    while (dhcpInitClient5 == N_EXECUTE) {usleep(100);}
    if (dhcpInitClient5 == 0)
    {
        printf("\n Configure DhcpClient with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_PROTOCOL_03 failed : Configure DhcpClient \n");
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }

    // 3. TESTER: DHCP Server <SERVER-1> Cause DUT to transit its state to DHCPCLIENT_STATE_REQUESTING
    // 4. DUT: Transit finite state to DHCPCLIENT_STATE_REQUESTING
    /***************************************************************************************************
    *
    *   FUNCTION NAME:  DHCPCLIENT_STATE_REQUESTING
    *
    ***************************************************************************************************/
    //Initialize the filter
    strcpy(filter.SrcAdd, "0.0.0.0");
    strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
    //  TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
    dhcp_packet_1 = ListenDHCP(filter, DHCP_config.ParamListenTime);
    //length > 0 => a DHCP Packet is received to a server
    if(dhcp_packet_1.length > 0)
    {
        printf("\n A DHCP Packet is received with success \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REQUESTING failed : Listen DHCP \n");
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }

    // Extract the options from DHCPDISCOVER
    Option = (char*)GetDHCPField(&dhcp_packet_1, DHCP, options);
    //  Verify that the DUT: Sends "DHCPDISCOVER" Message  (packet_type of dhcp_field)
    if ((uint8_t)GetDHCPField(&dhcp_packet_1 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPDISCOVER)
    {
        printf("\n A DHCPDISCOVER is received with success \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REQUESTING failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }

    // Extract the xid from DHCPDISCOVER : random_transaction_id_number
    extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number); 
    //  TESTER:DHCP Server <SERVER-1> Sends "DHCPOFFER" Message to DUT through <DIface-0>
    DHCP_Packet packet_created = CreateDHCP();
    dhcp_value *option_field;
    option_field = (dhcp_value*)malloc(sizeof(dhcp_value));
    option_field->data[0] = '\x63';
    option_field->data[1] = '\x82';
    option_field->data[2] = '\x53';
    option_field->data[3] = '\x63'; 
    option_field->data[4] = '\x35';
    option_field->data[5] = '\x01';
    option_field->data[6] = '\x02';
    option_field->data[7] = '\xff';
    option_field->len = 8;
    if (EditDHCPField(&packet_created  , DHCP, options, (void*)option_field) == 0)
    {
        printf("\n The field is edited with success, packet_type = DHCPOFFER \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REQUESTING failed : EditDHCPField, packet_type \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }

    // Edit the random_transaction_id_number of the packet_created "DHCPOFFER" with the extractedXID
    if (EditDHCPField(&packet_created  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
    {
        printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REQUESTING failed : EditDHCPField, xid \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }   
    if (EditDHCPField(&packet_created  , DHCP, yiaddr, (void*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
    {
        printf("\n The field is edited with success, yiaddr  \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REQUESTING failed : EditDHCPField, yiaddr \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }

    if (SendDHCP(packet_created) == 0 )
    {
        printf("\n The packet is sent with success \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REQUESTING failed : SendDHCP \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }


    //  TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
    dhcp_packet_2  = ListenDHCP(filter, DHCP_config.ParamListenTime);   
    //  verify that the DUT: Sends "DHCPREQUEST" Message
    if (dhcp_packet_2.length > 0 )
    {
        printf("\n The packet DHCP is received with success \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REQUESTING failed : The packet received isn't a DHCP packet \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }

    // Extract the options from DHCPREQUEST
    Option = (char*)GetDHCPField(&dhcp_packet_2, DHCP, options);
    if ((uint8_t)GetDHCPField(&dhcp_packet_2 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPREQUEST)
    {
        printf("\n DHCPCLIENT_STATE_REQUESTING : The DHCPREQUEST is received with success  \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REQUESTING failed : The field received (packet_type) isn't a DHCPREQUEST \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }
    //<Message Type Option> is set to 53
    if (Option[4] == 53)
    {
        printf("\n Test DHCPv4_CLIENT_PROTOCOL_03 : The Message Type Option of DHCP Options is set 53 with success \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 0;
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_PROTOCOL_03 failed : The Message Type Option of DHCP Options is NOT set to 53 \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb5, 0, (text){0});
        return 1;
    }
}