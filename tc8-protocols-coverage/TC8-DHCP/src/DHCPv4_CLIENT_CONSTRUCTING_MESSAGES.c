#include "DHCPv4_CLIENT_CONSTRUCTING_MESSAGES.h"
#include "DHCP_config.h"
#include <stdio.h>
#include <string.h>
#include "TestabilityProtocolTypes.h"
#include "TestabilityProtocol_api.h"
#include "AbstractionAPI.h"
#include "commons_functions.h"
#include "DHCP_state_machine.h"
#define N_EXECUTE 2
uint8_t genaralStartTest2, dhcpInitClient2, genaralEndTest2, udpCreateAndBind2, udpSendData2;
uint16_t socketId, totalLen;
extern Dhcp_config_t DHCP_config;

/***************************************************************************************************
*
*   FUNCTION NAME: DhcpInitClient_cb2
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
void DhcpInitClient_cb2 (TP_ResultID_t r)
{
    dhcpInitClient2 = r;
}




/***************************************************************************************************
*
*   FUNCTION NAME: GenaralStartTest_cb2
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
void GenaralStartTest_cb2(TP_ResultID_t r)
{
    genaralStartTest2 = r;
}




/***************************************************************************************************
*
*   FUNCTION NAME: GenaralEndTest_cb2
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
void GenaralEndTest_cb2(TP_ResultID_t r)
{
    genaralEndTest2 = r;
}


/***************************************************************************************************
*
*   FUNCTION NAME: UdpCreateAndBind_cb2
*
***************************************************************************************************/
/**
* @brief
*  Initialize the Udp Create And Bind callback
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
void UdpCreateAndBind_cb2(TP_ResultID_t r, uint16_t s)
{
    udpCreateAndBind2 = r;
    socketId = s;
}


/***************************************************************************************************
*
*   FUNCTION NAME: UdpSendData_cb2
*
***************************************************************************************************/
/**
* @brief
*  Initialize the Udp Create And Bind callback
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
void UdpSendData_cb2(TP_ResultID_t r)
{
    udpSendData2 = r;
}




/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_01()
 *
 ***************************************************************************************************/
/**
* @brief
*  DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_01: The last option must always be the 'end' option
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_01 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_01()
{
    DHCP_Packet dhcp_packet;
    Packet_filter filter;
    char* Option;
    int counter;
    genaralStartTest2 = N_EXECUTE;
    dhcpInitClient2 = N_EXECUTE;
    genaralEndTest2 = N_EXECUTE;
    //start test
    TP_GenaralStartTest(GenaralStartTest_cb2);
    if (genaralStartTest2 == 0)
    {
        printf("\n Genaral Test will start with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_01 failed : Genaral Start Test  \n");
    }
    // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
    // 2. TESTER: Externally cause DUT to bring up <DIface-0>
    //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
    text interf;
    interf.dataLength = 20;
    interf.Data = DHCP_config.DIface_0;
    TP_DhcpInitClient(DhcpInitClient_cb2, interf);
    while (dhcpInitClient2 == N_EXECUTE) {usleep(100);}
    if (dhcpInitClient2 == 0)
    {
      printf("\n Configure_DhcpClient with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_01 failed : Configure_DhcpClient \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
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
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_01 failed : Listen DHCP \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 1;
    }

    // Extract the options from DHCPDISCOVER
    Option = (char*)GetDHCPField(&dhcp_packet, DHCP, options);
    // 4. verify that the DUT: Sends "DHCPDISCOVER" Message  (packet_type of dhcp_field)
    if ((uint8_t)GetDHCPField(&dhcp_packet , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPDISCOVER)
    {
        printf("\n A DHCPDISCOVER is received with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_01 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 1;
    }
    /* 5. TESTER : Verify that received DHCPDISCOVER Message contains:
    - Message Option containing:  - 255 (End Option) at position Last
    */

    counter=4;
    while ( counter < 312 && Option[counter] != 0xff)
    {
        counter = counter + 2 + Option[counter+1];
    }
    if(Option[counter] == 0xff)
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_01 : The message Option contain 255 (End Option) at position Last with success \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 0;
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_01 failed :The message Option doesn't contain 255 (End Option) at position Last\n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 1;
    }
}

/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_02()
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_02: Use the IP address provided in the 'server identifier' option for any unicast requests
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_02 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_02()
{
    DHCP_Packet dhcp_packet;
    Packet_filter filter;
    char* Option;
    int i;
    genaralStartTest2 = N_EXECUTE;
    dhcpInitClient2 = N_EXECUTE;
    genaralEndTest2 = N_EXECUTE;
    //start test
    TP_GenaralStartTest(GenaralStartTest_cb2);
    if (genaralStartTest2 == 0)
    {
        printf("\n Genaral Test will start with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_02 failed : Genaral Start Test  \n");
    }
    // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
    // 2. TESTER: Externally cause DUT to bring up <DIface-0>
    //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
    text interf;
    interf.dataLength = 20;
    interf.Data = DHCP_config.DIface_0;
    TP_DhcpInitClient(DhcpInitClient_cb2, interf);
    while (dhcpInitClient2 == N_EXECUTE) {usleep(100);}
    if (dhcpInitClient2 == 0)
    {
        printf("\n Configure_DhcpClient with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_02 failed : Configure_DhcpClient \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 1;
    }

    // 3. DHCP Server <SERVER-1> Cause DUT to transit its state to DHCPCLIENT_STATE_RENEWING
    // 4. DUT: Transit finite state to DHCPCLIENT_STATE_RENEWING

    if (DHCPCLIENT_STATE_RENEWING() == 0)
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_02 : DHCPCLIENT_STATE_RENEWING() with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_02 failed : DHCPCLIENT_STATE_RENEWING() \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 1;
    }

    //Initialize the filter
    strcpy(filter.SrcAdd, "0.0.0.0");
    strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
    // 5. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
    dhcp_packet  = ListenDHCP(filter, DHCP_config.ParamListenTime);
    // 6. verify that the DUT: Sends "DHCPREQUEST" Message
    if (dhcp_packet.length > 0 )
    {
        printf("\n The packet DHCP is received with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_02 failed : The packet received isn't a DHCP packet \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 1;
    }

    // Extract the options from DHCPREQUEST
    Option = (char*)GetDHCPField(&dhcp_packet, DHCP, options);
    if ((uint8_t)GetDHCPField(&dhcp_packet, DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPREQUEST)
    {
        printf("\n The DHCPREQUEST is received with success  \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_02 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 1;
    }

    // 7. TESTER: Verify that received DHCPREQUEST Message contains: - Destination IP Address field is set to <SERVER1-IP-ADDRESS>
    if (strcmp((char*)GetDHCPField(&dhcp_packet, IP, IP_DstAddress), (char*)DHCP_config.SERVERn_IP_ADDRESS) == 0)
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_02 : The received DHCPREQUEST Message contains IP_DstAddress = SERVERn_IP_ADDRESS with success \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 0;
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_02 failed : The received DHCPREQUEST Message contains IP_DstAddress != SERVERn_IP_ADDRESS \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 1;
    }
}





/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_03()
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_03:  Source IP address field of DHCPDISCOVER Message is 0
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_03 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_03()
{
    DHCP_Packet dhcp_packet;
    Packet_filter filter;
    char* Option;
    genaralStartTest2 = N_EXECUTE;
    dhcpInitClient2 = N_EXECUTE;
    genaralEndTest2 = N_EXECUTE;
    //start test
    TP_GenaralStartTest(GenaralStartTest_cb2);
    if (genaralStartTest2 == 0)
    {
        printf("\n Genaral Test will start with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_03 failed : Genaral Start Test  \n");
    }
    // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
    // 2. TESTER: Externally cause DUT to bring up <DIface-0>
    //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
    text interf;
    interf.dataLength = 20;
    interf.Data = DHCP_config.DIface_0;
    TP_DhcpInitClient(DhcpInitClient_cb2, interf);
    while (dhcpInitClient2 == N_EXECUTE) {usleep(100);}
    if (dhcpInitClient2 == 0)
    {
        printf("\n Configure_DhcpClient with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_03 failed : Configure_DhcpClient \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
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
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_03 failed : Listen DHCP \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
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
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_03 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 1;
    }

    // 5. TESTER : Verify that received DHCPDISCOVER Message contains:
    // - - Source IP Address field is set to 0
    if (strcmp((char*)GetDHCPField(&dhcp_packet, IP, IP_SrcAddress), "0.0.0.0") == 0)
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_03 : The received DHCPDISCOVER Message contains IP_SrcAddress = 0.0.0.0 with success \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 0;
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_03 failed : The received DHCPDISCOVER Message contains IP_SrcAddress != 255.255.255.255 \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 1;
    }
}


/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_04()
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_04: Source IP address field of DHCPREQUEST Message is 0
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_04 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_04()
{
  DHCP_Packet dhcp_packet_1, dhcp_packet_2;
  Packet_filter filter;
  uint32_t extractedXID;
  char* Option;
  int i;
  genaralStartTest2 = N_EXECUTE;
  dhcpInitClient2 = N_EXECUTE;
  genaralEndTest2 = N_EXECUTE;
  //start test
  TP_GenaralStartTest(GenaralStartTest_cb2);
  if (genaralStartTest2 == 0)
  {
      printf("\n Genaral Test will start with success \n");
  }
  else
  {
      printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_04 failed : Genaral Start Test  \n");
  }
  // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
  // 2. TESTER: Externally cause DUT to bring up <DIface-0>
  //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
  text interf;
  interf.dataLength = 20;
  interf.Data = DHCP_config.DIface_0;
  TP_DhcpInitClient(DhcpInitClient_cb2, interf);
  while (dhcpInitClient2 == N_EXECUTE) {usleep(100);}
  if (dhcpInitClient2 == 0)
  {
      printf("\n Configure_DhcpClient with success \n");
  }
  else
  {
      printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_04 failed : Configure_DhcpClient \n");
      TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
      return 1;
  }

  // 3. DHCP Server <SERVER-1> Cause DUT to transit its state to DHCPCLIENT_STATE_REQUESTING
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
      TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
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
      TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
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
      TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
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
      TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
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
      TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
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
      TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
      return 1;
  }


  //  TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
  dhcp_packet_2  = ListenDHCP(filter, DHCP_config.ParamListenTime);

  // verify that the DUT: Sends "DHCPREQUEST" Message
  if (dhcp_packet_2.length > 0 )
  {
      printf("\n The packet DHCP is received with success \n");
  }
  else
  {
      printf("\n DHCPCLIENT_STATE_REQUESTING failed : The packet received isn't a DHCP packet \n");
      free(option_field);
      TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
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
      TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
      return 1;
  }


  // 7. TESTER: Verify that received DHCPREQUEST Message contains: - Destination IP Address field is set to <SERVER1-IP-ADDRESS>
  if (strcmp((char*)GetDHCPField(&dhcp_packet_2, IP, IP_SrcAddress), "0.0.0.0") == 0)
  {
      printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_04 : The received DHCPREQUEST Message contains IP_DstAddress = 0.0.0.0 with success \n");
      TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
      return 0;
  }
  else
  {
      printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_04 failed : The received DHCPREQUEST Message contains IP_DstAddress != 0.0.0.0 \n");
      TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
      return 1;
  }

}



/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05()
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05:  Parse 'sname' field when Option Overload is present
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05()
{
    DHCP_Packet dhcp_packet_1, dhcp_packet_2;
    UDP_Packet udp_packet;
    Packet_filter filter1, filter2;
    uint32_t extractedXID;
    char* Option;
    int counter;
    genaralStartTest2 = N_EXECUTE;
    dhcpInitClient2 = N_EXECUTE;
    genaralEndTest2 = N_EXECUTE;

    //start test
    TP_GenaralStartTest(GenaralStartTest_cb2);
    if (genaralStartTest2 == 0)
    {
        printf("\n Genaral Test will start with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05 failed : Genaral Start Test  \n");
    }
   // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
   // 2. TESTER: Externally cause DUT to bring up <DIface-0>
   //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
   text interf;
   interf.dataLength = 20;
   interf.Data = DHCP_config.DIface_0;
   TP_DhcpInitClient(DhcpInitClient_cb2, interf);
   while (dhcpInitClient2 == N_EXECUTE) {usleep(100);}
   if (dhcpInitClient2 == 0)
   {
        printf("\n Configure_DhcpClient with success \n");
   }
   else
   {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05 failed : Configure_DhcpClient \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 1;
   }


  //Initialize the filter
  strcpy(filter1.SrcAdd, "0.0.0.0");
  strcpy(filter1.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
  // 3. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
  dhcp_packet_1 = ListenDHCP(filter1, DHCP_config.ParamListenTime);
  //length > 0 => a DHCP Packet is received to a server
  if(dhcp_packet_1.length > 0)
  {
        printf("\n A DHCP Packet is received with success \n");
  }
  else
  {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05 failed : Listen DHCP \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
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
             printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
             TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
             return 1;
  }

  // Extract the xid from DHCPDISCOVER : random_transaction_id_number
  extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number);

  /* 5. - Additional PAD Options to make packet length set to 576
        - Message Option containing:
          - Type field set to 52 (Overload Option)
          - Length field set to 1
          - Value set to the 2s
          -
  */
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

  option_field->data[7] = '\x34';
  option_field->data[8] = '\x01';
  option_field->data[9] = '\x02';
  option_field->data[10] = '\xff';
  option_field->len = 11;

  /*   - DHCP Message Option in 'sname' field containing:
              - Type field set to 3 (Router Option)
              - Length field set to 4
              - Value set to <SERVER1-IP-ADDRESS>
              - DHCP Message Option in 'sname' field containing:
              - Type field set to 255 (End Option)
  */
  dhcp_value *sname_field;
  sname_field = (dhcp_value*)malloc(sizeof(dhcp_value));
  sname_field->data[0] = '\x03';
  sname_field->data[1] = '\x04';
  //extractIpAddress(DHCP_config.SERVERn_IP_ADDRESS,sname_field->data[2]); // to be veriifed
  strcpy((char*)&sname_field->data[2], (char*)DHCP_config.SERVERn_IP_ADDRESS);

  sname_field->data[6] = '\xff';
  sname_field->len = 7;

  if (EditDHCPField(&packet_created  , DHCP, options, (void*)option_field) == 0)
  {
    printf("\n The field is edited with success, packet_type = DHCPOFFER \n");
  }
  else
  {
               printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05 failed : EditDHCPField \n");
               free(option_field);
               TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
               return 1;
  }

  // Edit the Sname of the packet_created "DHCPOFFER"
  if (EditDHCPField(&packet_created  , DHCP, sname, (void*)sname_field) == 0)
  {
               printf("\n The field is edited with success, Sname \n");
  }
  else
  {
               printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05 failed : EditDHCPField \n");
               free(option_field);
               TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
               return 1;
  }
  // Edit the random_transaction_id_number of the packet_created "DHCPOFFER" with the extractedXID
  if (EditDHCPField(&packet_created  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
  {
             printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
  }
  else
  {
             printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05 failed : EditDHCPField, xid \n");
             free(option_field);
             TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
             return 1;
  }

  if (EditDHCPField(&packet_created  , DHCP, yiaddr, (void*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
  {
            printf("\n The field is edited with success, yiaddr  \n");
  }
  else
  {
           printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05 failed : EditDHCPField, yiaddr \n");
           free(option_field);
           TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
           return 1;
  }


  if (SendDHCP(packet_created) == 0 )
  {
               printf("\n The packet is sent with success \n");
  }
  else
  {
               printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05 failed : SendDHCP \n");
               free(option_field);
               TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
               return 1;
  }


  // 6. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
  dhcp_packet_2  = ListenDHCP(filter1, DHCP_config.ParamListenTime);

  // 7. verify that the DUT: Sends "DHCPREQUEST" Message
  if (dhcp_packet_2.length > 0 )
  {
               printf("\n The packet DHCP is received with success \n");
  }
  else
  {
               printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05 failed : The packet received isn't a DHCP packet \n");
               free(option_field);
               TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
               return 1;
  }

  // Extract the options from DHCPREQUEST
  Option = (char*)GetDHCPField(&dhcp_packet_2, DHCP, options);
  if ((uint8_t)GetDHCPField(&dhcp_packet_2 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPREQUEST)
  {
               printf("\n The DHCPREQUEST is received with success  \n");

  }
  else
  {
               printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
               free(option_field);
               TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
               return 1;
  }

  // 9. TESTER: Externally cause DUT to send UDP message through <DIface-0> to IP address <IP-UNUSED-ADDRESS>
  // 9.1 We will use : TP_ResultID_t TP_UdpCreateAndBind(void (*UdpCreateAndBind_cb)(TP_ResultID_t, uint16),boolean doBind, uint16 localPort, ip4addr localAddr);
  // Initialise ip4addr destAddr
  ip4addr destAddr;
  destAddr.dataLength = 4;
  extractIpAddress(DHCP_config.IP_UNUSED_ADDRESS, &destAddr);
  TP_UdpCreateAndBind(UdpCreateAndBind_cb2, true, DHCP_config.PORT_UDP_SRC, destAddr);

  // 9.2 We will use : TP_UdpSendData(void (*TP_UdpSendData_cb)(TP_ResultID_t), uint16 socketId, uint16 totalLen, uint16 destPort, ip4addr destAddr, vint8 data);
  // Initialise vint8
  vint8 Data;
  uint8 data[] = "Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05";
  Data.dataLength = sizeof(data);
  Data.Data = (uint8 *) calloc(Data.dataLength, sizeof(uint8));
  memcpy(Data.Data, data, sizeof(data));
  TP_UdpSendData(UdpSendData_cb2, socketId, totalLen, DHCP_config.PORT_UDP_DST, destAddr, Data);

  // 10. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
  strcpy(filter2.SrcAdd, (char*)DHCP_config.SERVERn_IP_POOL_L_M);
  filter2.Srcport = DHCP_config.PORT_UDP_SRC;
  strcpy(filter2.dstAdd, (char*)DHCP_config.SERVERn_IP_ADDRESS);
  filter2.Dstport = DHCP_config.PORT_UDP_DST;
  udp_packet = ListenUDP(filter2, DHCP_config.ParamListenTime);
  if(udp_packet.length > 0)
  {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05 : A UDP Packet is received with success \n");
        free(option_field);
        return 0;
  }
  else
  {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05 failed : Listen UDP \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 1;
  }

}


/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06()
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06: : Parse 'file' field when Option Overload is present
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06()
{
    DHCP_Packet dhcp_packet_1, dhcp_packet_2;
    UDP_Packet udp_packet;
    Packet_filter filter1, filter2;
    uint32_t extractedXID;
    char* Option;
    char* requested_add;
    int i;
    genaralStartTest2 = N_EXECUTE;
    dhcpInitClient2 = N_EXECUTE;
    genaralEndTest2 = N_EXECUTE;

    //start test
    TP_GenaralStartTest(GenaralStartTest_cb2);
    if (genaralStartTest2 == 0)
    {
        printf("\n Genaral Test will start with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : Genaral Start Test  \n");
    }
   // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
   // 2. TESTER: Externally cause DUT to bring up <DIface-0>
   //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
   text interf;
   interf.dataLength = 20;
   interf.Data = DHCP_config.DIface_0;
   TP_DhcpInitClient(DhcpInitClient_cb2, interf);
   while (dhcpInitClient2 == N_EXECUTE) {usleep(100);}
   if (dhcpInitClient2 == 0)
   {
        printf("\n Configure_DhcpClient with success \n");
   }
   else
   {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : Configure_DhcpClient \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 1;
   }


  //Initialize the filter
  strcpy(filter1.SrcAdd, "0.0.0.0");
  strcpy(filter1.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
  // 3. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
  dhcp_packet_1 = ListenDHCP(filter1, DHCP_config.ParamListenTime);
  //length > 0 => a DHCP Packet is received to a server
  if(dhcp_packet_1.length > 0)
  {
        printf("\n A DHCP Packet is received with success \n");
  }
  else
  {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : Listen DHCP \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
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
             printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
             TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
             return 1;
  }


  // Extract the xid from DHCPDISCOVER : random_transaction_id_number
  extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number);


  /* 5. - Additional PAD Options to make packet length set to 576
        - Message Option containing:
          - Type field set to 52 (Overload Option)
          - Length field set to 1
          - Value set to the 'file' field is used to hold options
  */

  DHCP_Packet packet_created_1 = CreateDHCP();
  dhcp_value *option_field_1;
  option_field_1 = (dhcp_value*)malloc(sizeof(dhcp_value));
  option_field_1->data[0] = '\x63';
  option_field_1->data[1] = '\x82';
  option_field_1->data[2] = '\x53';
  option_field_1->data[3] = '\x63';

  option_field_1->data[4] = '\x35';
  option_field_1->data[5] = '\x01';
  option_field_1->data[6] = '\x02';

  option_field_1->data[7] = '\x34';
  option_field_1->data[8] = '\x01';
  option_field_1->data[9] = '\x01';
  option_field_1->data[10] = '\xff';
  option_field_1->len = 11;

  /* DHCP Message Option in 'file' field containing:
      - Type field set to 3 (Router Option)
      - Length field set to 4
      - Value set to <SERVER1-IP-ADDRESS>
      - DHCP Message Option in 'file' field containing:
      - Type field set to 255 (End Option)
  */
  dhcp_value *file_field_1;
  file_field_1 = (dhcp_value*)malloc(sizeof(dhcp_value));
  file_field_1->data[0] = '\x03';
  file_field_1->data[1] = '\x04';
  strcpy((char*)&file_field_1->data[2], (char*)DHCP_config.SERVERn_IP_ADDRESS); // must change this methode
  file_field_1->data[3] = '\xff';
  file_field_1->len = 4;

  if (EditDHCPField(&packet_created_1  , DHCP, options, (void*)option_field_1) == 0)
  {
               printf("\n The field is edited with success, packet_type = DHCPOFFER \n");
  }
  else
  {
               printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : EditDHCPField \n");
               free(option_field_1);
               TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
               return 1;
  }

  // Edit the file of the packet_created "DHCPOFFER"
  if (EditDHCPField(&packet_created_1  , DHCP, file, (void*)file_field_1) == 0)
  {
               printf("\n The field is edited with success, file \n");
  }
  else
  {
               printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : EditDHCPField \n");
               free(option_field_1);
               TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
               return 1;
  }

  // Edit the random_transaction_id_number of the packet_created "DHCPOFFER" with the extractedXID
  if (EditDHCPField(&packet_created_1, DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
  {
             printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
  }
  else
  {
             printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : EditDHCPField, xid \n");
             free(option_field_1);
             TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
             return 1;
  }

  if (EditDHCPField(&packet_created_1, DHCP, yiaddr, (void*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
  {
            printf("\n The field is edited with success, yiaddr  \n");
  }
  else
  {
           printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : EditDHCPField, yiaddr \n");
           free(option_field_1);
           TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
           return 1;
  }


  if (SendDHCP(packet_created_1) == 0 )
  {
               printf("\n The packet is sent with success \n");
  }
  else
  {
               printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : SendDHCP \n");
               free(option_field_1);
               TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
               return 1;
  }


  // 6. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
  dhcp_packet_2  = ListenDHCP(filter1, DHCP_config.ParamListenTime);

  // 7. verify that the DUT: Sends "DHCPREQUEST" Message
  if (dhcp_packet_2.length > 0 )
  {
               printf("\n The packet DHCP is received with success \n");
  }
  else
  {
               printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : The packet received isn't a DHCP packet \n");
               free(option_field_1);
               TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
               return 1;
  }

  // 7. DHCPREQUEST Message
  // Extract the options from DHCPREQUEST
  Option = (char*)GetDHCPField(&dhcp_packet_2, DHCP, options);
  if ((uint8_t)GetDHCPField(&dhcp_packet_2 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPREQUEST)
  {
               printf("\n The DHCPREQUEST is received with success  \n");

  }
  else
  {
               printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
               free(option_field_1);
               TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
               return 1;
  }

  // Extract the Requested Address from the options of DHCPREQUEST
  i = 4;
  while (Option[i] != 50 && Option[i] != 255)
  {
      i = i + 2 + Option[i+1];
  }

  if (Option[i] == 50 && Option[i+1] == 4)
  {
     printf("\n The Requested IP Address Option of DHCP Options is copy to requested_add \n");
     strncpy(requested_add, &Option[i+2], 4);
  }
  else
  {
     printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : The Requested IP Address Option of DHCP Options doesn't exist \n");
     free(option_field_1);
     return 1;
  }



  /* 8. TESTER: DHCP Server <SERVER-1> Sends DHCPACK Message to DUT through <DIface-0> containing :
        - Additional PAD Options to make packet length set to 576
        - Message Option containing:
        - Type field set to 52 (Overload Option)
        - Length field set to 1
        - Value set to the 'file' field is used to hold options
      - DHCP Message Option in 'file' field containing:
        - Type field set to 3 (Router Option)
        - Length field set to 4
        - Value set to <SERVER1-IP-ADDRESS>
      - DHCP Message Option in 'file' field containing:
        - Type field set to 255 (End Option)
  */
  DHCP_Packet packet_created_2 = CreateDHCP();
  dhcp_value *option_field_2;
  option_field_2 = (dhcp_value*)malloc(sizeof(dhcp_value));
  option_field_2->data[0] = '\x63';
  option_field_2->data[1] = '\x82';
  option_field_2->data[2] = '\x53';
  option_field_2->data[3] = '\x63';

  option_field_2->data[4] = '\x35';
  option_field_2->data[5] = '\x01';
  option_field_2->data[6] = '\x05';

  option_field_2->data[7] = '\x34';
  option_field_2->data[8] = '\x01';
  option_field_2->data[9] = '\x01';
  option_field_2->data[9] = '\xff';
  option_field_2->len = 10;

  /* DHCP Message Option in 'file' field containing:
      - Type field set to 3 (Router Option)
      - Length field set to 4
      - Value set to <SERVER1-IP-ADDRESS>
      - DHCP Message Option in 'file' field containing:
      - Type field set to 255 (End Option)
  */
  dhcp_value *file_field_2;
  file_field_2 = (dhcp_value*)malloc(sizeof(dhcp_value));
  file_field_2->data[0] = '\x03';
  file_field_2->data[1] = '\x04';
  strcpy((char*)&file_field_2->data[2], (char*)DHCP_config.SERVERn_IP_ADDRESS); // must change
  file_field_2->data[3] = '\xff';
  file_field_2->len = 4;


  if (EditDHCPField(&packet_created_2  , DHCP, options, (void*)option_field_2) == 0)
  {
      printf("\n The field is edited with success, packet_type = DHCPACK \n");
  }
  else
  {
      printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : EditDHCPField, packet_type \n");
      free(option_field_1);
      free(option_field_2);
      TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
      return 1;
  }

  // Edit the file of the packet_created "DHCPACK"
  if (EditDHCPField(&packet_created_2  , DHCP, file, (void*)file_field_2) == 0)
  {
               printf("\n The field is edited with success, file \n");
  }
  else
  {
               printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : EditDHCPField \n");
               free(option_field_1);
               free(option_field_2);
               TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
               return 1;
  }

  // Edit the random_transaction_id_number of the packet_created "DHCPACK" with the extractedXID
  if (EditDHCPField(&packet_created_2  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
  {
      printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
  }
  else
  {
      printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : EditDHCPField, xid \n");
      free(option_field_1);
      free(option_field_2);
      TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
      return 1;
  }

  if (EditDHCPField(&packet_created_2  , DHCP, yiaddr, (void*)requested_add) == 0)
  {
      printf("\n The field is edited with success, yiaddr  \n");
  }
  else
  {
      printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : EditDHCPField, yiaddr \n");
      free(option_field_1);
      free(option_field_2);
      TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
      return 1;
  }

  if (SendDHCP(packet_created_2) == 0 )
  {
      printf("\n The packet is sent with success \n");
  }
  else
  {
      printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : SendDHCP \n");
      free(option_field_1);
      free(option_field_2);
      TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
      return 1;
  }

  // 9. TESTER: Externally cause DUT to send UDP message through <DIface-0> to IP address <IP-UNUSED-ADDRESS>
  // 9.1 We will use : TP_ResultID_t TP_UdpCreateAndBind(void (*UdpCreateAndBind_cb)(TP_ResultID_t, uint16),boolean doBind, uint16 localPort, ip4addr localAddr);
  // Initialise ip4addr destAddr
  ip4addr destAddr;
  destAddr.dataLength = 4;
  extractIpAddress(DHCP_config.IP_UNUSED_ADDRESS, &destAddr);
  TP_UdpCreateAndBind(UdpCreateAndBind_cb2, true, DHCP_config.PORT_UDP_SRC, destAddr);

  // 9.2 We will use : TP_UdpSendData(void (*TP_UdpSendData_cb)(TP_ResultID_t), uint16 socketId, uint16 totalLen, uint16 destPort, ip4addr destAddr, vint8 data);
  // Initialise vint8
  vint8 Data;
  uint8 data[] = "Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05";
  Data.dataLength = sizeof(data);
  Data.Data = (uint8 *) calloc(Data.dataLength, sizeof(uint8));
  memcpy(Data.Data, data, sizeof(data));
  TP_UdpSendData(UdpSendData_cb2, socketId, totalLen, DHCP_config.PORT_UDP_DST, destAddr, Data);

  // 10. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
  strcpy(filter2.SrcAdd, (char*)DHCP_config.SERVERn_IP_POOL_L_M);
  filter2.Srcport = DHCP_config.PORT_UDP_SRC;
  strcpy(filter2.dstAdd, (char*)DHCP_config.SERVERn_IP_ADDRESS);
  filter2.Dstport = DHCP_config.PORT_UDP_DST;
  udp_packet = ListenUDP(filter2, DHCP_config.ParamListenTime);
  if(udp_packet.length > 0)
  {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 : A UDP Packet is received with success \n");
        free(option_field_1);
        free(option_field_2);
        return 0;
  }
  else
  {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 failed : Listen UDP \n");
        free(option_field_1);
        free(option_field_2);
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 1;
  }


}


/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_12()
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_12: : The retransmission delay should be doubled with subsequent retransmissions
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_12 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_12()
{
    DHCP_Packet dhcp_packet_1, dhcp_packet_2, packet_created;
    Packet_filter filter;
    char* Option;
    genaralStartTest2 = N_EXECUTE;
    dhcpInitClient2 = N_EXECUTE;
    genaralEndTest2 = N_EXECUTE;

    //start test
    TP_GenaralStartTest(GenaralStartTest_cb2);
    if (genaralStartTest2 == 0)
    {
        printf("\n Genaral Test will start with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_12 failed : Genaral Start Test  \n");
        return 1;
    }
    // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
    // 2. TESTER: Externally cause DUT to bring up <DIface-0>
    //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
    text interf;
    interf.dataLength = 20;
    interf.Data = DHCP_config.DIface_0;
    TP_DhcpInitClient(DhcpInitClient_cb2, interf);
    while (dhcpInitClient2 == N_EXECUTE) {usleep(100);}
    if (dhcpInitClient2 == 0)
    {
        printf("\n Configure DhcpClient with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_12 failed : Configure DhcpClient \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
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
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_12 failed : Listen DHCP \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 1;
    }

    // Extract the options from DHCPDISCOVER
    Option = (char*)GetDHCPField(&dhcp_packet_1, DHCP, options);
    // 4. verify that the DUT: Sends "DHCPDISCOVER" Message  (packet_type of dhcp_field)
    if ((uint8_t)GetDHCPField(&dhcp_packet_1 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPDISCOVER)
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_12 : A DHCPDISCOVER is received with success \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 0;
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_12 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
        TP_GenaralEndTest(GenaralEndTest_cb2, 0, (text){0});
        return 1;
    }
}

int DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_13()
{
	return 2;
}

int DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_14()
{
	return 2;
}

