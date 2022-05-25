#include "DHCPv4_CLIENT_SUMMARY.h"
#include "DHCP_config.h"
#include <stdio.h>
#include <string.h>
#include "TestabilityProtocolTypes.h"
#include "TestabilityProtocol_api.h"
#include "AbstractionAPI.h"

#define N_EXECUTE 2

uint8_t genaralStartTest8, dhcpInitClient8, genaralEndTest8;

extern Dhcp_config_t DHCP_config;


/**************************************callbacks***************************************************/

/***************************************************************************************************
*
*   FUNCTION NAME: DhcpInitClient_cb8
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
void DhcpInitClient_cb8 (TP_ResultID_t r)
{
    dhcpInitClient8 = r;
}




/***************************************************************************************************
*
*   FUNCTION NAME: GenaralStartTest_cb8
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
void GenaralStartTest_cb8(TP_ResultID_t r)
{
    genaralStartTest8 = r;
}




/***************************************************************************************************
*
*   FUNCTION NAME: GenaralEndTest_cb8
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
void GenaralEndTest_cb8(TP_ResultID_t r)
{
    genaralEndTest8 = r;
}

/*=========================================Test cases==============================================*/

/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_SUMMARY_01()
 *
 ***************************************************************************************************/
/**
* @brief
*  DHCPv4_CLIENT_SUMMARY_01: Setup Verification (DHCP Client Listens on UDP port 68)
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if setup-verification-01 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_SUMMARY_01()
{
    DHCP_Packet dhcp_packet_1, dhcp_packet_2, packet_created;
    Packet_filter filter;
    uint32_t extractedXID;
    char* Option;
    genaralStartTest8 = N_EXECUTE;
    dhcpInitClient8 = N_EXECUTE;
    genaralEndTest8 = N_EXECUTE;

    //start test
    TP_GenaralStartTest(GenaralStartTest_cb8);
    if (genaralStartTest8 == 0)
    {
        printf("\n Genaral Test will start with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_01 failed : Genaral Start Test  \n");
        return 1;
    }
    // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
    // 2. TESTER: Externally cause DUT to bring up <DIface-0>
    //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
    text interf;
    interf.dataLength = 20;
    interf.Data = DHCP_config.DIface_0;
    TP_DhcpInitClient(DhcpInitClient_cb8, interf);
    while (dhcpInitClient8 == N_EXECUTE) {usleep(100);}
    if (dhcpInitClient8 == 0)
    {
        printf("\n Configure DhcpClient with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_01 failed : Configure DhcpClient \n");
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }

    //Initialize the filter
    strcpy(filter.SrcAdd, "0.0.0.0");
    strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
    // 3. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
    dhcp_packet_1 = ListenDHCP(filter, DHCP_config.ParamListenTime);
    //length > 0 => a DHCP Packet is received to a server
    if(dhcp_packet_1.length > 0)
    {
        printf("\n A DHCP Packet is received with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_01 failed : Listen DHCP \n");
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }

    // Extract the options from DHCPDISCOVER
    Option = (char*)GetDHCPField(&dhcp_packet_1, DHCP, options);
    // 4. verify that the DUT: Sends "DHCPDISCOVER" Message  (packet_type of dhcp_field)
    if ((uint8_t)GetDHCPField(&dhcp_packet_1 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPDISCOVER)
    {
        printf("\n A DHCPDISCOVER is received with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_01 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }

    // Extract the xid from DHCPDISCOVER : random_transaction_id_number
    extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number);

    // 5. TESTER:DHCP Server <SERVER-1> Sends "DHCPOFFER" Message to DUT through <DIface-0>
    packet_created = CreateDHCP();

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
        printf("\n Test DHCPv4_CLIENT_SUMMARY_01 failed : EditDHCPField, packet_type \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }

    // Edit the random_transaction_id_number of the packet_created "DHCPOFFER" with the extractedXID
    if (EditDHCPField(&packet_created  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
    {
        printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_01 failed : EditDHCPField, xid \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }

    if (EditDHCPField(&packet_created  , DHCP, yiaddr, (void*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
    {
        printf("\n The field is edited with success, yiaddr  \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_01 failed : EditDHCPField, yiaddr \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }

    if (SendDHCP(packet_created) == 0)
    {
        printf("\n The packet is sent with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_01 failed : SendDHCP \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }

    // 6. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
    dhcp_packet_2  = ListenDHCP(filter, DHCP_config.ParamListenTime);

    // 7. verify that the DUT: Sends "DHCPREQUEST" Message
    if (dhcp_packet_2.length > 0 )
    {
        printf("\n The packet DHCP is received with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_01 failed : The packet received isn't a DHCP packet \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }
    // Extract the options from DHCPREQUEST
    Option = (char*)GetDHCPField(&dhcp_packet_2, DHCP, options);
    if ((uint8_t)GetDHCPField(&dhcp_packet_2 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPREQUEST)
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_01 passed: The DHCPREQUEST is received with success  \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 0;
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_01 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }
}

int DHCPv4_CLIENT_SUMMARY_02()
{
	return 2;
}




/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_SUMMARY_03()
 *
 ***************************************************************************************************/
/**
* @brief
*  DHCPv4_CLIENT_SUMMARY_03: Receive DHCP messages with an 'options' field of at least length 312 octets
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if setup-verification-03 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_SUMMARY_03()
{
    DHCP_Packet dhcp_packet_1, dhcp_packet_2, packet_created;
    Packet_filter filter;
    uint32_t extractedXID;
    char* Option;
    genaralStartTest8 = N_EXECUTE;
    dhcpInitClient8 = N_EXECUTE;
    genaralEndTest8 = N_EXECUTE;

    //start test
    TP_GenaralStartTest(GenaralStartTest_cb8);
    if (genaralStartTest8 == 0)
    {
        printf("\n Genaral Test will start with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_03 failed : Genaral Start Test  \n");
        return 1;
    }
    // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
    // 2. TESTER: Externally cause DUT to bring up <DIface-0>
    //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
    text interf;
    interf.dataLength = 20;
    interf.Data = DHCP_config.DIface_0;
    TP_DhcpInitClient(DhcpInitClient_cb8, interf);
    while (dhcpInitClient8 == N_EXECUTE) {usleep(100);}
    if (dhcpInitClient8 == 0)
    {
        printf("\n Configure DhcpClient with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_03 failed : Configure DhcpClient \n");
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }

    //Initialize the filter
    strcpy(filter.SrcAdd, "0.0.0.0");     
    strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
    // 3. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
    dhcp_packet_1 = ListenDHCP(filter, DHCP_config.ParamListenTime);
    //length > 0 => a DHCP Packet is received to a server
    if(dhcp_packet_1.length > 0)
    {
        printf("\n A DHCP Packet is received with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_03 failed : Listen DHCP \n");
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }

    // Extract the options from DHCPDISCOVER
    Option = (char*)GetDHCPField(&dhcp_packet_1, DHCP, options);
    // 4. verify that the DUT: Sends "DHCPDISCOVER" Message  (packet_type of dhcp_field)
    if ((uint8_t)GetDHCPField(&dhcp_packet_1 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPDISCOVER)
    {
        printf("\n A DHCPDISCOVER is received with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_03 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }

    // Extract the xid from DHCPDISCOVER : random_transaction_id_number
    extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number);

    // 5. TESTER:DHCP Server <SERVER-1> Sends "DHCPOFFER" Message to DUT through <DIface-0>
    packet_created = CreateDHCP();
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
    // Additional PAD Options to make packet length set to 576
    option_field->len = 8;
    if (EditDHCPField(&packet_created  , DHCP, options, (void*)option_field) == 0)
    {
        printf("\n The field is edited with success, packet_type = DHCPOFFER \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_03 failed : EditDHCPField, packet_type \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }

    // Edit the random_transaction_id_number of the packet_created "DHCPOFFER" with the extractedXID
    if (EditDHCPField(&packet_created  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
    {
        printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_03 failed : EditDHCPField, xid \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }

    if (EditDHCPField(&packet_created  , DHCP, yiaddr, (void*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
    {
        printf("\n The field is edited with success, yiaddr  \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_03 failed : EditDHCPField, yiaddr \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }
    if (SendDHCP(packet_created) == 0 )
    {
        printf("\n The packet is sent with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_03 failed : SendDHCP \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }

    // 6. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
    dhcp_packet_2  = ListenDHCP(filter, DHCP_config.ParamListenTime);

    // 7. verify that the DUT: Sends "DHCPREQUEST" Message
    if (dhcp_packet_2.length > 0 )
    {
        printf("\n The packet DHCP is received with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_03 failed : The packet received isn't a DHCP packet \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }
    // Extract the options from DHCPREQUEST
    Option = (char*)GetDHCPField(&dhcp_packet_2, DHCP, options);
    if ((uint8_t)GetDHCPField(&dhcp_packet_2 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPREQUEST)
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_03 : The DHCPREQUEST is received with success  \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 0;
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_SUMMARY_03 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb8, 0, (text){0});
        return 1;
    }
}

