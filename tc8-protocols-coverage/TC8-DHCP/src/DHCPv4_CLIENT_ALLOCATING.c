#include "DHCPv4_CLIENT_ALLOCATING.h"
#include "DHCP_config.h"
#include <stdio.h>
#include <string.h>
#include "TestabilityProtocolTypes.h"
#include "TestabilityProtocol_api.h"
#include "AbstractionAPI.h"
#include "commons_functions.h"
#include "DHCP_state_machine.h"

#define N_EXECUTE 2

uint8_t genaralStartTest1, dhcpInitClient1, genaralEndTest1;
extern Dhcp_config_t DHCP_config;

/***************************************************************************************************
*
*   FUNCTION NAME: DhcpInitClient_cb1
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
void DhcpInitClient_cb1 (TP_ResultID_t r)
{
    dhcpInitClient1 = r;
}

/***************************************************************************************************
*
*   FUNCTION NAME: GenaralStartTest_cb1
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
void GenaralStartTest_cb1(TP_ResultID_t r)
{
    genaralStartTest1 = r;
}

/***************************************************************************************************
*
*   FUNCTION NAME: GenaralEndTest_cb1
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
void GenaralEndTest_cb1(TP_ResultID_t r)
{
    genaralEndTest1 = r;
}
/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_ALLOCATING_01
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_ALLOCATING_01: Broadcast DHCPDISCOVER message on its local physical subnet
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_ALLOCATING_01 failed
*
*
* @note
* -
*
* @warning
* -
*
**/

int DHCPv4_CLIENT_ALLOCATING_01()
{
      DHCP_Packet dhcp_packet_1, dhcp_packet_2, packet_created;
      Packet_filter filter;
      char* Option;
      genaralStartTest1 = N_EXECUTE;
      dhcpInitClient1 = N_EXECUTE;
      genaralEndTest1 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb1);
      if (genaralStartTest1 == 0)
      {
          printf("\n Genaral Test will start with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_ALLOCATING_01 failed : Genaral Start Test  \n");
      }
      /* 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
         2. TESTER: Externally cause DUT to bring up <DIface-0>
            Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      */
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb1, interf);
      while (dhcpInitClient1 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient1 == 0)
      {
           printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
           printf("\n Test DHCPv4_CLIENT_ALLOCATING_01 failed : Configure_DhcpClient \n");
           TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_01 failed : Listen DHCP \n");
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_01 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }     
      // 5. TESTER : Verify that received DHCPDISCOVER Message contains: - Destination IP Address field is set to <IP-BROADCAST-ADDRESS>
      if (strcmp((char*)GetDHCPField(&dhcp_packet_1 , IP, IP_DstAddress), (char*)DHCP_config.IP_BROADCAST_ADDRESS) == 0)
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_01 : The received DHCPDISCOVER Message contains IP_DstAddress = 255.255.255.255 with success \n");
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_01 failed : The received DHCPDISCOVER Message contains IP_DstAddress != 255.255.255.255 \n");
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }
}



/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_ALLOCATING_03
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_ALLOCATING_03: Send DHCPREQUEST - must include the 'server identifier'
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_ALLOCATING_03 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_ALLOCATING_03()
{
      DHCP_Packet dhcp_packet_1, dhcp_packet_2, packet_created;
      Packet_filter filter;
      uint32_t extractedXID;
      char* Option;
      int counter;
      genaralStartTest1 = N_EXECUTE;
      dhcpInitClient1 = N_EXECUTE;
      genaralEndTest1 = N_EXECUTE;

      //start test
      TP_GenaralStartTest(GenaralStartTest_cb1);
      if (genaralStartTest1 == 0)
      {
          printf("\n Genaral Test will start with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_ALLOCATING_03 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb1, interf);
      while (dhcpInitClient1 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient1 == 0)
      {
           printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
           printf("\n Test DHCPv4_CLIENT_ALLOCATING_03 failed : Configure_DhcpClient \n");
           TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_03 failed : Listen DHCP \n");
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_03 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_03 failed : EditDHCPField, packet_type = DHCPOFFER \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }

      // Edit the random_transaction_id_number of the packet_created "DHCPOFFER" with the extractedXID
      if (EditDHCPField(&packet_created  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
      {
            printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_03 failed : EditDHCPField, xid \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }

      if (EditDHCPField(&packet_created  , DHCP, yiaddr, (void*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
      {
           printf("\n The field is edited with success, yiaddr  \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_ALLOCATING_03 failed : EditDHCPField, yiaddr \n");
          free(option_field);
          TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
          return 1;
      }

      if (SendDHCP(packet_created) == 0 )
      {
            printf("\n The packet is sent with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_03 failed : SendDHCP \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_03 failed : The packet received isn't a DHCP packet \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_03 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }


      /*8. TESTER: Verify that received DHCPREQUEST Message contains:
            - Message Option containing:
                 - Type field set to 54 (Server Identifier Option)
                 - Length field set to 4
                 - Value set to <SERVER1-IP-ADDRESS>
      */

      counter=4;
      while (Option[counter] != 54 && Option[counter] != 255)
      {
        counter = counter + 2 + Option[counter+1];
      }               
      if (Option[counter] == 54)
      {
            if (Option[counter+1] == 4)
            {
                  printf("\n The octet of DHCP Options is set to Server Identifier Option with success \n");
                  if(strncmp((char*)DHCP_config.SERVERn_IP_ADDRESS, &Option[counter+2], 4) == 0)
                  {
                        printf("\n Test DHCPv4_CLIENT_ALLOCATING_03 : The four octets of DHCP Options is set to <SERVER1-IP-ADDRESS> with success \n");
                        free(option_field);
                        TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
                        return 0;
                  }
                  else
                  {
                        printf("\n Test DHCPv4_CLIENT_ALLOCATING_03 failed : The four octets of DHCP Options is NOT set to <SERVER1-IP-ADDRESS> \n");
                        free(option_field);
                        TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
                        return 1;
                  }
            }
            else
            {
                  printf("\n Test DHCPv4_CLIENT_ALLOCATING_03 failed : The  length of DHCP Options is NOT set to 4, <SERVER1-IP-ADDRESS> \n");
                  free(option_field);
                  TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
                  return 1;
            }
      }
      else
      {
         printf("\n Test DHCPv4_CLIENT_ALLOCATING_03 failed : Server Identifier Option is not found \n");
         free(option_field);
         TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
         return 1;
      }               
}

/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_ALLOCATING_04
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_ALLOCATING_04: Send DHCPREQUEST - header value 'secs' field
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_ALLOCATING_04 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_ALLOCATING_04()
{
      DHCP_Packet dhcp_packet_1, dhcp_packet_2, packet_created;
      Packet_filter filter;
      uint16_t extractedSeconds;
      uint32_t extractedXID;
      char* Option;
      genaralStartTest1 = N_EXECUTE;
      dhcpInitClient1 = N_EXECUTE;
      genaralEndTest1 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb1);
      if (genaralStartTest1 == 0)
      {
          printf("\n Genaral Test will start with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_ALLOCATING_04 failed : Genaral Start Test  \n");
      }
      /* 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
         2. TESTER: Externally cause DUT to bring up <DIface-0>
         Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      */
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb1, interf);
      while (dhcpInitClient1 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient1 == 0)
      {
           printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
           printf("\n Test DHCPv4_CLIENT_ALLOCATING_04 failed : Configure_DhcpClient \n");
           TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
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
            printf("\n Test failed : Listen DHCP \n");
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }

      // Extract the options from DHCPDISCOVER
      Option = (char*)GetDHCPField(&dhcp_packet_1, DHCP, options);
      // 4. verify that the DUT: Sends "DHCPDISCOVER" Message  (packet_type of dhcp_field)
      if ((uint8_t)GetDHCPField(&dhcp_packet_1, DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPDISCOVER)
      {
            printf("\n A DHCPDISCOVER is received with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_04 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }     
      // Extract the xid from DHCPDISCOVER : random_transaction_id_number
      extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number);     

      // 5. TESTER : Extracts the content of 'secs' field to <sextractedSeconds>
      extractedSeconds = (uint16_t)GetDHCPField(&dhcp_packet_1 , DHCP, seconds_used_in_timing);  
      // 6. TESTER:DHCP Server <SERVER-1> Sends "DHCPOFFER" Message to DUT through <DIface-0>
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
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_04 failed : EditDHCPField, packet_type = DHCPOFFER \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }     
      // Edit the random_transaction_id_number of the packet_created "DHCPOFFER" with the extractedXID
      if (EditDHCPField(&packet_created  , DHCP, random_transaction_id_number, (void *)(intptr_t)extractedXID) == 0)
      {
            printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_04 failed : EditDHCPField, xid \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }

      if (EditDHCPField(&packet_created  , DHCP, yiaddr, (void*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
      {
           printf("\n The field is edited with success, yiaddr  \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_ALLOCATING_04 failed : EditDHCPField, yiaddr \n");
          free(option_field);
          TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
          return 1;
      }

      if (SendDHCP(packet_created) == 0 )
      {
            printf("\n The packet is sent with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_04 failed : SendDHCP \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }


      // 7. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
      dhcp_packet_2  = ListenDHCP(filter, DHCP_config.ParamListenTime); 
      // 8. verify that the DUT: Sends "DHCPREQUEST" Message
      if (dhcp_packet_2.length > 0 )
      {
            printf("\n The packet DHCP is received with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_04 failed : The packet received isn't a DHCP packet \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_04 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }


      //9. TESTER: Verify that received DHCPREQUEST Message contains: - 'secs' field is set to extractedSeconds
      if ((uint16_t)GetDHCPField(&dhcp_packet_2 , DHCP, seconds_used_in_timing) == extractedSeconds)
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_04 : The received DHCPREQUEST Message contains : 'secs' field is set to extractedSeconds with success \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_04 failed : The received DHCPREQUEST Message contains : 'secs' field is NOT equal to seconds_extracted  \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }
}



/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_ALLOCATING_05
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_ALLOCATING_05: Send DHCPREQUEST to the same IP broadcast address
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_ALLOCATING_05 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_ALLOCATING_05()
{
      DHCP_Packet dhcp_packet_1, dhcp_packet_2;
      Packet_filter filter;
      uint32_t extractedXID;
      char* Option;
      genaralStartTest1 = N_EXECUTE;
      dhcpInitClient1 = N_EXECUTE;
      genaralEndTest1 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb1);
      if (genaralStartTest1 == 0)
      {
            printf("\n Genaral Test will start with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_05 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb1, interf);
      while (dhcpInitClient1 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient1 == 0)
      {
            printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_05 failed : Configure_DhcpClient \n");
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_05 failed : Listen DHCP \n");
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
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
                 printf("\n Test DHCPv4_CLIENT_ALLOCATING_05 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
                 TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
                 return 1;
      }

      // Extract the xid from DHCPDISCOVER : random_transaction_id_number
      extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number);     
      // 5. TESTER : Verify that received DHCPDISCOVER Message contains: - Destination IP Address field is set to <IP-BROADCAST-ADDRESS>
      if (strcmp((char*)GetDHCPField(&dhcp_packet_1 , IP, IP_DstAddress), (char*)DHCP_config.IP_BROADCAST_ADDRESS ) == 0)
      {
            printf("\n The received DHCPDISCOVER Message contains IP_DstAddress = 255.255.255.255  \n");     
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_05 failed : The received DHCPDISCOVER Message contains IP_DstAddress != 255.255.255.255 \n");
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }
      
      // 6. TESTER:DHCP Server <SERVER-1> Sends "DHCPOFFER" Message to DUT through <DIface-0>
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
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_05 failed : EditDHCPField \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }     
      // Edit the random_transaction_id_number of the packet_created "DHCPOFFER" with the extractedXID
      if (EditDHCPField(&packet_created  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
      {
            printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_05 failed : EditDHCPField, xid \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }

      if (EditDHCPField(&packet_created  , DHCP, yiaddr, (void*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
      {
           printf("\n The field is edited with success, yiaddr  \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_ALLOCATING_05 failed : EditDHCPField, yiaddr \n");
          free(option_field);
          TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
          return 1;
      }

      if (SendDHCP(packet_created) == 0 )
      {
            printf("\n The packet is sent with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_05 failed : SendDHCP \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }

      // 7. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
      dhcp_packet_2  = ListenDHCP(filter, DHCP_config.ParamListenTime); 
      // 8. verify that the DUT: Sends "DHCPREQUEST" Message
      if (dhcp_packet_2.length > 0 )
      {
            printf("\n The packet DHCP is received with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_05 failed : The packet received isn't a DHCP packet \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_05 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }

      //9. TESTER: Verify that received DHCPREQUEST Message contains: - Destination IP Address field is set to <IP-BROADCAST-ADDRESS>
      if (strcmp((char*)GetDHCPField(&dhcp_packet_2 , IP, IP_DstAddress), (char*)DHCP_config.IP_BROADCAST_ADDRESS) == 0)
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_05 : The received DHCPREQUEST Message contains IP_DstAddress = 255.255.255.255 with success \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_05 failed : The received DHCPREQUEST Message contains IP_DstAddress != 255.255.255.255 \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }
}

/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_ALLOCATING_06
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_ALLOCATING_06: Send DHCPDISCOVER message - timeout and resend on no DHCPOFFER messages
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_ALLOCATING_06 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_ALLOCATING_06()
{
    DHCP_Packet dhcp_packet_1, dhcp_packet_2;
    Packet_filter filter;
    char* Option;
    genaralStartTest1 = N_EXECUTE;
    dhcpInitClient1 = N_EXECUTE;
    genaralEndTest1 = N_EXECUTE;
    //start test
    TP_GenaralStartTest(GenaralStartTest_cb1);
    if (genaralStartTest1 == 0)
    {
        printf("\n Genaral Test will start with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_ALLOCATING_06 failed : Genaral Start Test  \n");
    }
   // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
   // 2. TESTER: Externally cause DUT to bring up <DIface-0>
   //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
   text interf;
   interf.dataLength = 20;
   interf.Data = DHCP_config.DIface_0;
   TP_DhcpInitClient(DhcpInitClient_cb1, interf);
   while (dhcpInitClient1 == N_EXECUTE) {usleep(100);}
   if (dhcpInitClient1 == 0)
   {
        printf("\n Configure_DhcpClient with success \n");
   }
   else
   {
        printf("\n Test DHCPv4_CLIENT_ALLOCATING_06 failed : Configure_DhcpClient \n");
        TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
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
        printf("\n Test DHCPv4_CLIENT_ALLOCATING_06 failed : Listen DHCP \n");
        TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
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
        printf("\n Test DHCPv4_CLIENT_ALLOCATING_06 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
        TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
        return 1;
  }

  // 5. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
  dhcp_packet_2 = ListenDHCP(filter, DHCP_config.ParamListenTime);
  //length > 0 => a DHCP Packet is received to a server
  if(dhcp_packet_2.length > 0)
  {
        printf("\n A DHCP Packet is received with success \n");
  }
  else
  {
        printf("\n Test DHCPv4_CLIENT_ALLOCATING_06 failed : Listen DHCP \n");
        TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
        return 1;
  }


  // Extract the options from DHCPDISCOVER
  Option = (char*)GetDHCPField(&dhcp_packet_2, DHCP, options);
  // 6. verify that the DUT: Sends "DHCPDISCOVER" Message  (packet_type of dhcp_field)
  if ((uint8_t)GetDHCPField(&dhcp_packet_2 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPDISCOVER)
  {
        printf("\n Test DHCPv4_CLIENT_ALLOCATING_06 : A DHCPDISCOVER is received with success \n");
        TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
        return 0;
  }
  else
  {
        printf("\n Test DHCPv4_CLIENT_ALLOCATING_06 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
        TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
        return 1;
  }


}

int DHCPv4_CLIENT_ALLOCATING_07()
{
	return 2;
}

int DHCPv4_CLIENT_ALLOCATING_08()
{
	return 2;
}


/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_ALLOCATING_09
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_ALLOCATING_09:  Receive DHCPNAK - restart the configuration process
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_ALLOCATING_09 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_ALLOCATING_09()
{
      DHCP_Packet dhcp_packet_1, dhcp_packet_2, dhcp_packet_3;
      Packet_filter filter;
      uint32_t extractedXID;
      char* Option;
      char* requested_add;
      int i;
      genaralStartTest1 = N_EXECUTE;
      dhcpInitClient1 = N_EXECUTE;
      genaralEndTest1 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb1);
      if (genaralStartTest1 == 0)
      {
            printf("\n Genaral Test will start with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_09 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb1, interf);
      while (dhcpInitClient1 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient1 == 0)
      {
            printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_09 failed : Configure_DhcpClient \n");
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }

      // 3. DHCP Server <SERVER-1> Cause DUT to transit its state to DHCPCLIENT_STATE_RENEWING
      // 4. DUT: Transit finite state to DHCPCLIENT_STATE_RENEWING
      if (DHCPCLIENT_STATE_RENEWING() == 0)
      {
           printf("\n Test DHCPv4_CLIENT_ALLOCATING_09 : DHCPCLIENT_STATE_RENEWING() with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_ALLOCATING_09 failed : DHCPCLIENT_STATE_RENEWING() \n");
          TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
          return 1;
      }

      //Initialize the filter
      strcpy(filter.SrcAdd, "0.0.0.0");
      strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
      // 5. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
      dhcp_packet_1 = ListenDHCP(filter, DHCP_config.ParamListenTime);
      //length > 0 => a DHCP Packet is received to a server
      if(dhcp_packet_1.length > 0)
      {
            printf("\n A DHCP Packet is received with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_09 failed : Listen DHCP \n");
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }

      // Extract the options from DHCPREQUEST
      Option = (char*)GetDHCPField(&dhcp_packet_1, DHCP, options);
      // 5. verify that the DUT: Sends "DHCPREQUEST" Message  (packet_type of dhcp_field)
      if ((uint8_t)GetDHCPField(&dhcp_packet_1 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPREQUEST)
      {
            printf("\n A DHCPREQUEST is received with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_09 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }

      // Extract the xid from DHCPREQUEST : random_transaction_id_number
      extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number);

      // 7. TESTER:DHCP Server <SERVER-1> Sends "DHCPNAK" Message to DUT through <DIface-0>
      DHCP_Packet packet_created = CreateDHCP();
      dhcp_value *option_field;
      option_field = (dhcp_value*)malloc(sizeof(dhcp_value));
      option_field->data[0] = '\x63';
      option_field->data[1] = '\x82';
      option_field->data[2] = '\x53';
      option_field->data[3] = '\x63';

      option_field->data[4] = '\x35';
      option_field->data[5] = '\x01';
      option_field->data[6] = '\x06';
      option_field->data[7] = '\xff';
      option_field->len = 8;
      if (EditDHCPField(&packet_created  , DHCP, options, (void*)option_field) == 0)
      {
            printf("\n The field is edited with success, packet_type = DHCPNAK \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_09 failed : EditDHCPField \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }
      // Edit the random_transaction_id_number of the packet_created "DHCPOFFER" with the extractedXID
      if (EditDHCPField(&packet_created  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
      {
            printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_09 failed : EditDHCPField, xid \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }

      if (SendDHCP(packet_created) == 0 )
      {
            printf("\n The packet is sent with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_09 failed : SendDHCP \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }

      // 8. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
      dhcp_packet_2  = ListenDHCP(filter, 10+DHCP_config.ParamToleranceTime);
      // 9. verify that the DUT: Sends "DHCPREQUEST" Message
      if (dhcp_packet_2.length > 0 )
      {
            printf("\n The packet DHCP is received with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_09 failed : The packet received isn't a DHCP packet \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }

      // Extract the options from DHCPDISCOVER
      Option = (char*)GetDHCPField(&dhcp_packet_2, DHCP, options);
      if ((uint8_t)GetDHCPField(&dhcp_packet_2 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPDISCOVER)
      {
            printf("\n The DHCPDISCOVER is received with success  \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_ALLOCATING_05 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb1, 0, (text){0});
            return 1;
      }
}


/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_ALLOCATING_10
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_ALLOCATING_10: Resend DHCPREQUEST message if timeout on no DHCPACK or a DHCPNAK message
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_ALLOCATING_10 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_ALLOCATING_10()
{
      return 2;
}

