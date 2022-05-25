#include "DHCPv4_CLIENT_REQUEST.h"
#include "DHCP_config.h"
#include <stdio.h>
#include <string.h>
#include "TestabilityProtocolTypes.h"
#include "TestabilityProtocol_api.h"
#include "AbstractionAPI.h"
#include "commons_functions.h"
#include "DHCP_state_machine.h"
#define N_EXECUTE 2
uint8_t genaralStartTest7, dhcpInitClient7, genaralEndTest7;
extern Dhcp_config_t DHCP_config;

/***************************************************************************************************
*
*   FUNCTION NAME: DhcpInitClient_cb7
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
void DhcpInitClient_cb7 (TP_ResultID_t r)
{
    dhcpInitClient7 = r;
}




/***************************************************************************************************
*
*   FUNCTION NAME: GenaralStartTest_cb7
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
void GenaralStartTest_cb7(TP_ResultID_t r)
{
    genaralStartTest7 = r;
}




/***************************************************************************************************
*
*   FUNCTION NAME: GenaralEndTest_cb7
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
void GenaralEndTest_cb7(TP_ResultID_t r)
{
    genaralEndTest7 = r;
}

/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_REQUEST_01
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_REQUEST_01:  DHCPREQUEST message - the 'ciaddr' option
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_REQUEST_02 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_REQUEST_01()
{
      DHCP_Packet dhcp_packet_1, dhcp_packet_2;
      Packet_filter filter;
      uint32_t extractedXID;
      char* Option;
      int i;
      genaralStartTest7 = N_EXECUTE;
      dhcpInitClient7 = N_EXECUTE;
      genaralEndTest7 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb7);
      if (genaralStartTest7 == 0)
      {
          printf("\n Genaral Test will start with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_REQUEST_01 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb7, interf);
      while (dhcpInitClient7 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient7 == 0)
      {
           printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
           printf("\n Test DHCPv4_CLIENT_REQUEST_01 failed : Configure_DhcpClient \n");
           TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
          TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
          TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
          TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
          TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
          TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
          TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
          TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
          TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
          return 1;
      }

      // 7. TESTER: Verify that received DHCPREQUEST Message contains: - Destination IP Address field is set to 0.0.0.0
      if (strcmp((char*)GetDHCPField(&dhcp_packet_2, DHCP, ciaddr), "0.0.0.0") == 0)
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_01 : The received DHCPREQUEST Message contains ciaddr = 0.0.0.0 with success \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_01 failed : The received DHCPREQUEST Message contains ciaddr != 0.0.0.0 \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }
}




/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_REQUEST_02
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_REQUEST_02: DHCPREQUEST message - requested IP address
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_REQUEST_02 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_REQUEST_02()
{
      DHCP_Packet dhcp_packet_1, dhcp_packet_2;
      Packet_filter filter;
      uint32_t extractedXID;
      char* Option;
      char* extractedParamReq;
      char* extractedParamReqTest;
      int i, j, length;
      genaralStartTest7 = N_EXECUTE;
      dhcpInitClient7 = N_EXECUTE;
      genaralEndTest7 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb7);
      if (genaralStartTest7 == 0)
      {
            printf("\n Genaral Test will start with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_02 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb7, interf);
      while (dhcpInitClient7 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient7 == 0)
      {
            printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_02 failed : Configure_DhcpClient \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_REQUEST_02 failed : Listen DHCP \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_REQUEST_02 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }


      // Extract the xid from DHCPDISCOVER : random_transaction_id_number
      extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number);



      // 5. TESTER:DHCP Server <SERVER-1> Sends "DHCPOFFER" Message to DUT through <DIface-0> containing - 'yiaddr' field set to <SERVER1-IP-POOL-0-1>

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
            printf("\n Test DHCPv4_CLIENT_REQUEST_02 failed : EditDHCPField \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }

      // Edit the random_transaction_id_number of the packet_created "DHCPOFFER" with the extractedXID
      if (EditDHCPField(&packet_created  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
      {
            printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_02 failed : EditDHCPField, xid \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }

      // Edit the 'yiaddr' field of the packet_created "DHCPOFFER" with <SERVER1-IP-POOL-0-1>
      // n=1; L=0; M=1
      if (EditDHCPField(&packet_created  , DHCP, yiaddr, (void*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
      {
                  printf("\n The field is edited with success, yiaddr = SERVER1-IP-POOL-0-1 \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_02 failed : EditDHCPField, yiaddr \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }

      if (SendDHCP(packet_created) == 0 )
      {
            printf("\n The packet is sent with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_02 failed : SendDHCP \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_REQUEST_02 failed : The packet received isn't a DHCP packet \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_REQUEST_02 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }

      /* 8. TESTER: Verify that received DHCPREQUEST Message contains
            - Message Option containing:
            - Type field set to 50 (Requested IP Address Option)
            - Length field set to 4
            - Value set to <SERVER1-IP-POOL-0-1>
      */

      i = 4;
      while (Option[i] != 50 && Option[i] != 255)
      {
            i = i + 2 + Option[i+1];
      }

      if (Option[i] == 50)
      {
            if(Option[i+1] == 4 && strncmp((char*)DHCP_config.SERVERn_IP_POOL_L_M, &Option[i+2], 4) == 0)
            {
                  printf("\n Test DHCPv4_CLIENT_REQUEST_02 : The Requested IP Address Option is set to SERVER1-IP-POOL-0-1 with success \n");
                  free(option_field);
                  TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
                  return 0;
            }
            else
            {
                  printf("\n Test DHCPv4_CLIENT_REQUEST_02 failed : The Requested IP Address Option of DHCP Options != SERVERn_IP_POOL_L_M \n");
                  free(option_field);
                  TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
                  return 1;
            }
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_02 failed : The Requested IP Address Option of DHCP Options doesn't exist \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }
}


/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_REQUEST_06
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_REQUEST_06: DHCPREQUEST generated during RENEWING state: 'server identifier' option
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_REQUEST_06 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_REQUEST_06()
{
      DHCP_Packet dhcp_packet;
      Packet_filter filter;
      char* Option;
      int counter;
      genaralStartTest7 = N_EXECUTE;
      dhcpInitClient7 = N_EXECUTE;
      genaralEndTest7 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb7);
      if (genaralStartTest7 == 0)
      {
          printf("\n Genaral Test will start with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_REQUEST_06 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb7, interf);
      while (dhcpInitClient7 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient7 == 0)
      {
           printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
           printf("\n Test DHCPv4_CLIENT_REQUEST_06 failed : Configure_DhcpClient \n");
           TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
           return 1;
      }

      // 3. DHCP Server <SERVER-1> Cause DUT to transit its state to DHCPCLIENT_STATE_RENEWING
      // 4. DUT: Transit finite state to DHCPCLIENT_STATE_RENEWING
      if (DHCPCLIENT_STATE_RENEWING() == 0)
      {
           printf("\n Test DHCPv4_CLIENT_REQUEST_06 : DHCPCLIENT_STATE_RENEWING() with success \n");
      }
      else
      {
          printf("\n Test DDHCPv4_CLIENT_REQUEST_06 failed : DHCPCLIENT_STATE_RENEWING() \n");
          TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
          return 1;
      }

      //Initialize the filter
      strcpy(filter.SrcAdd, "0.0.0.0");
      strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
      // 5. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
      dhcp_packet  = ListenDHCP(filter, DHCP_config.ParamListenTime);
      // 6. verify that the DUT: Sends "DHCPREQUEST" Message
      if (dhcp_packet.length > 0)
      {
            printf("\n The packet DHCP is received with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_06 failed : The packet received isn't a DHCP packet \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_REQUEST_06 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }

      /* 7. TESTER: Verify that received DHCPREQUEST Message contains
         - Message Option containing:
          - Type field set to 54 (Server Identifier Option)
      */
      counter = 4;
      while (Option[counter] != 54 && Option[counter] != 255)
      {
            counter = counter + 2 + Option[counter+1];
      }
      if (Option[counter] == 54)
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_06 : The octet of DHCP Options is set to Server Identifier Option with success \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_06 failed : The Server Identifier Option of DHCP Options doesn't exist \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }
}


/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_REQUEST_07
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_REQUEST_07: DHCPREQUEST generated during RENEWING state: 'requested IP address' option
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_REQUEST_07 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_REQUEST_07()
{
      DHCP_Packet dhcp_packet;
      Packet_filter filter;
      char* Option;
      int counter;
      genaralStartTest7 = N_EXECUTE;
      dhcpInitClient7 = N_EXECUTE;
      genaralEndTest7 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb7);
      if (genaralStartTest7 == 0)
      {
          printf("\n Genaral Test will start with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_REQUEST_07 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb7, interf);
      while (dhcpInitClient7 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient7 == 0)
      {
           printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
           printf("\n Test DHCPv4_CLIENT_REQUEST_07 failed : Configure_DhcpClient \n");
           TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
           return 1;
      }

      // 3. DHCP Server <SERVER-1> Cause DUT to transit its state to DHCPCLIENT_STATE_RENEWING
      // 4. DUT: Transit finite state to DHCPCLIENT_STATE_RENEWING
      if (DHCPCLIENT_STATE_RENEWING() == 0)
      {
           printf("\n Test DHCPv4_CLIENT_REQUEST_07 : DHCPCLIENT_STATE_RENEWING() with success \n");
      }
      else
      {
          printf("\n Test DDHCPv4_CLIENT_REQUEST_07 failed : DHCPCLIENT_STATE_RENEWING() \n");
          TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_REQUEST_07 failed : The packet received isn't a DHCP packet \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_REQUEST_07 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }

      /* 7. TESTER: Verify that received DHCPREQUEST Message contains
         - Message Option containing:
          - Type field is not set to 50 (Requested IP Address Option)
      */
      counter=4;
      while (Option[counter] != 50 && Option[counter] != 255)
      {
            counter = counter + 2 + Option[counter+1];
      }
      if (Option[counter] == 50)
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_07 : The octet of DHCP Options is set to Requested IP Address Option with success \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_07 failed : The Requested IP Address Option of DHCP Options doesn't exist \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }

}

/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_REQUEST_08
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_REQUEST_08: DHCPREQUEST generated during RENEWING state: 'ciaddr'  option
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_REQUEST_08 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_REQUEST_08()
{
      DHCP_Packet dhcp_packet;
      Packet_filter filter;
      char* Option;
      genaralStartTest7 = N_EXECUTE;
      dhcpInitClient7 = N_EXECUTE;
      genaralEndTest7 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb7);
      if (genaralStartTest7 == 0)
      {
            printf("\n Genaral Test will start with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_08 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb7, interf);
      while (dhcpInitClient7 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient7 == 0)
      {
            printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_08 failed : Configure_DhcpClient \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }

      // 3. DHCP Server <SERVER-1> Cause DUT to transit its state to DHCPCLIENT_STATE_BOUND
      /* 4. DUT: Transit finite state to DHCPCLIENT_STATE_BOUND
      if (DHCPCLIENT_STATE_BOUND() == 0)
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_08 : DHCPCLIENT_STATE_BOUND() with success \n");
      }
      else
      {
            printf("\n Test DDHCPv4_CLIENT_REQUEST_08 failed : DHCPCLIENT_STATE_BOUND() \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }*/

      // 5. DHCP Server <SERVER-1> Cause DUT to transit its state to DHCPCLIENT_STATE_RENEWING
      // 6. DUT: Transit finite state to DHCPCLIENT_STATE_RENEWING
      if (DHCPCLIENT_STATE_RENEWING() == 0)
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_08 : DHCPCLIENT_STATE_RENEWING() with success \n");
      }
      else
      {
            printf("\n Test DDHCPv4_CLIENT_REQUEST_08 failed : DHCPCLIENT_STATE_RENEWING() \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }

      //Initialize the filter
      strcpy(filter.SrcAdd, "0.0.0.0");
      strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
      // 7. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
      dhcp_packet  = ListenDHCP(filter, DHCP_config.ParamListenTime);
      // 6. verify that the DUT: Sends "DHCPREQUEST" Message
      if (dhcp_packet.length > 0 )
      {
            printf("\n The packet DHCP is received with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_08 failed : The packet received isn't a DHCP packet \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_REQUEST_08 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }
      // 7. TESTER: Verify that received DHCPREQUEST Message contains: - Destination IP Address field is set to <SERVER1-IP-POOL-0-0>
      if (strcmp((char*)GetDHCPField(&dhcp_packet, DHCP, ciaddr), (char*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_08 : The received DHCPREQUEST Message contains ciaddr = <SERVER1-IP-POOL-0-0> with success \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_08 failed : The received DHCPREQUEST Message contains ciaddr != <SERVER1-IP-POOL-0-0> \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }
}

/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_REQUEST_09
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_REQUEST_09: DHCPREQUEST generated during REBINDING state: 'server identifier' option
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_REQUEST_09 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_REQUEST_09()
{
      DHCP_Packet dhcp_packet;
      Packet_filter filter;
      char* Option;
      int counter;
      genaralStartTest7 = N_EXECUTE;
      dhcpInitClient7 = N_EXECUTE;
      genaralEndTest7 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb7);
      if (genaralStartTest7 == 0)
      {
          printf("\n Genaral Test will start with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_REQUEST_09 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb7, interf);
      while (dhcpInitClient7 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient7 == 0)
      {
           printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
           printf("\n Test DHCPv4_CLIENT_REQUEST_09 failed : Configure_DhcpClient \n");
           TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
           return 1;
      }

      // 3. DHCP Server <SERVER-1> Cause DUT to transit its state to DHCPCLIENT_STATE_REBINDING
      // 4. DUT: Transit finite state to DHCPCLIENT_STATE_REBINDING
      if (DHCPCLIENT_STATE_REBINDING() == 0)
      {
           printf("\n Test DHCPv4_CLIENT_REQUEST_09 : DHCPCLIENT_STATE_REBINDING() with success \n");
      }
      else
      {
          printf("\n Test DDHCPv4_CLIENT_REQUEST_09 failed : DHCPCLIENT_STATE_REBINDING() \n");
          TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_REQUEST_09 failed : The packet received isn't a DHCP packet \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_REQUEST_09 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }
      /* 7. TESTER: Verify that received DHCPREQUEST Message contains
         - Message Option containing:
          - Type field set to 54 (Server Identifier Option)
      */
      counter = 4;
      while (Option[counter] != 54 && Option[counter] != 255)
      {
            counter = counter + 2 + Option[counter+1];
      }
      if (Option[counter] == 54)
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_09 : The octet of DHCP Options is set to Server Identifier Option with success \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_09 failed : The Server Identifier Option of DHCP Options doesn't exist \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }

}


/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_REQUEST_10
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_REQUEST_10: : DHCPREQUEST generated during REBINDING state: 'requested IP address' option
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_REQUEST_10 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_REQUEST_10()
{
      DHCP_Packet dhcp_packet;
      Packet_filter filter;
      char* Option;
      int counter;
      genaralStartTest7 = N_EXECUTE;
      dhcpInitClient7 = N_EXECUTE;
      genaralEndTest7 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb7);
      if (genaralStartTest7 == 0)
      {
          printf("\n Genaral Test will start with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_REQUEST_10 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb7, interf);
      while (dhcpInitClient7 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient7 == 0)
      {
           printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
           printf("\n Test DHCPv4_CLIENT_REQUEST_10 failed : Configure_DhcpClient \n");
           TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
           return 1;
      }

      // 3. DHCP Server <SERVER-1> Cause DUT to transit its state to DHCPCLIENT_STATE_REBINDING
      // 4. DUT: Transit finite state to DHCPCLIENT_STATE_REBINDING
      if (DHCPCLIENT_STATE_REBINDING() == 0)
      {
           printf("\n Test DHCPv4_CLIENT_REQUEST_10 : DHCPCLIENT_STATE_REBINDING() with success \n");
      }
      else
      {
          printf("\n Test DDHCPv4_CLIENT_REQUEST_10 failed : DHCPCLIENT_STATE_REBINDING() \n");
          TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_REQUEST_10 failed : The packet received isn't a DHCP packet \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_REQUEST_10 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }

      /* 7. TESTER: Verify that received DHCPREQUEST Message contains
         - Message Option containing:
          - Type field is not set to 50 (Requested IP Address Option)
      */
      counter = 4;
      while (Option[counter] != 50 && Option[counter] != 255)
      {
            counter = counter + 2 + Option[counter+1];
      }
      if (Option[counter] == 50)
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_10 : The octet of DHCP Options is set to Requested IP Address Option with success \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_10 failed : The Requested IP Address Option of DHCP Options doesn't exist \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }
}

/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_REQUEST_11
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_REQUEST_11: : DHCPREQUEST generated during REBINDING state: 'requested IP address' option
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_REQUEST_11 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_REQUEST_11()
{
      DHCP_Packet dhcp_packet;
      Packet_filter filter;
      char* Option;
      int counter;
      genaralStartTest7 = N_EXECUTE;
      dhcpInitClient7 = N_EXECUTE;
      genaralEndTest7 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb7);
      if (genaralStartTest7 == 0)
      {
          printf("\n Genaral Test will start with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_REQUEST_11 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb7, interf);
      while (dhcpInitClient7 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient7 == 0)
      {
           printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
           printf("\n Test DHCPv4_CLIENT_REQUEST_11 failed : Configure_DhcpClient \n");
           TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
           return 1;
      }

      // 3. DHCP Server <SERVER-1> Cause DUT to transit its state to DHCPCLIENT_STATE_BOUND
      /* 4. DUT: Transit finite state to DHCPCLIENT_STATE_BOUND
      if (DHCPCLIENT_STATE_BOUND() == 0)
      {
           printf("\n Test DHCPv4_CLIENT_REQUEST_11 : DHCPCLIENT_STATE_BOUND() with success \n");
      }
      else
      {
          printf("\n Test DDHCPv4_CLIENT_REQUEST_11 failed : DHCPCLIENT_STATE_BOUND() \n");
          TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
          return 1;
      }*/

      // 5. DHCP Server <SERVER-1> Cause DUT to transit its state to DHCPCLIENT_STATE_REBINDING
      // 6. DUT: Transit finite state to DHCPCLIENT_STATE_REBINDING
      if (DHCPCLIENT_STATE_REBINDING() == 0)
      {
           printf("\n Test DHCPv4_CLIENT_REQUEST_11 : DHCPCLIENT_STATE_REBINDING() with success \n");
      }
      else
      {
          printf("\n Test DDHCPv4_CLIENT_REQUEST_11 failed : DHCPCLIENT_STATE_REBINDING() \n");
          TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
          return 1;
      }

      //Initialize the filter
      strcpy(filter.SrcAdd, "0.0.0.0");
      strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
      // 7. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
      dhcp_packet  = ListenDHCP(filter, DHCP_config.ParamListenTime);
      // 6. verify that the DUT: Sends "DHCPREQUEST" Message
      if (dhcp_packet.length > 0 )
      {
            printf("\n The packet DHCP is received with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_11 failed : The packet received isn't a DHCP packet \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_REQUEST_11 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }

      // 7. TESTER: Verify that received DHCPREQUEST Message contains: - Destination IP Address field is set to <SERVER1-IP-POOL-0-0>
      if (strcmp((char*)GetDHCPField(&dhcp_packet, DHCP, ciaddr), (char*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_11 : The received DHCPREQUEST Message contains ciaddr = <SERVER1-IP-POOL-0-0> with success \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_11 failed : The received DHCPREQUEST Message contains ciaddr != <SERVER1-IP-POOL-0-0> \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }
}

/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_REQUEST_12
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_REQUEST_12: : DHCPREQUEST generated during REBINDING state: use IP broadcast address
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_REQUEST_12 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_REQUEST_12()
{
      DHCP_Packet dhcp_packet;
      Packet_filter filter;
      char* Option;
      int i;
      genaralStartTest7 = N_EXECUTE;
      dhcpInitClient7 = N_EXECUTE;
      genaralEndTest7 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb7);
      if (genaralStartTest7 == 0)
      {
          printf("\n Genaral Test will start with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_REQUEST_12 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb7, interf);
      while (dhcpInitClient7 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient7 == 0)
      {
           printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
           printf("\n Test DHCPv4_CLIENT_REQUEST_12 failed : Configure_DhcpClient \n");
           TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
           return 1;
      }

      // 3. DHCP Server <SERVER-1> Cause DUT to transit its state to DHCPCLIENT_STATE_BOUND
      /* 4. DUT: Transit finite state to DHCPCLIENT_STATE_BOUND
      if (DHCPCLIENT_STATE_BOUND() == 0)
      {
           printf("\n Test DHCPv4_CLIENT_REQUEST_12 : DHCPCLIENT_STATE_BOUND() with success \n");
      }
      else
      {
          printf("\n Test DDHCPv4_CLIENT_REQUEST_12 failed : DHCPCLIENT_STATE_BOUND() \n");
          TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
          return 1;
      }*/

      // 5. DHCP Server <SERVER-1> Cause DUT to transit its state to DHCPCLIENT_STATE_REBINDING
      // 6. DUT: Transit finite state to DHCPCLIENT_STATE_REBINDING
      if (DHCPCLIENT_STATE_REBINDING() == 0)
      {
           printf("\n Test DHCPv4_CLIENT_REQUEST_12 : DHCPCLIENT_STATE_REBINDING() with success \n");
      }
      else
      {
          printf("\n Test DDHCPv4_CLIENT_REQUEST_12 failed : DHCPCLIENT_STATE_REBINDING() \n");
          TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
          return 1;
      }

      //Initialize the filter
      strcpy(filter.SrcAdd, "0.0.0.0");
      strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
      // 7. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
      dhcp_packet  = ListenDHCP(filter, DHCP_config.ParamListenTime);
      // 6. verify that the DUT: Sends "DHCPREQUEST" Message
      if (dhcp_packet.length > 0 )
      {
            printf("\n The packet DHCP is received with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_12 failed : The packet received isn't a DHCP packet \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_REQUEST_12 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }

      // 7. TESTER: Verify that received DHCPREQUEST Message contains: - Destination IP Address field is set to <SERVER1-IP-POOL-0-0>
      if (strcmp((char*)GetDHCPField(&dhcp_packet, IP, IP_DstAddress), (char*)DHCP_config.IP_BROADCAST_ADDRESS) == 0)
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_12 : The received DHCPREQUEST Message contains IP_DstAddress = IP_BROADCAST_ADDRESS with success \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REQUEST_12 failed : The received DHCPREQUEST Message contains IP_DstAddress != IP_BROADCAST_ADDRESS \n");
            TP_GenaralEndTest(GenaralEndTest_cb7, 0, (text){0});
            return 1;
      }
}