#include "DHCPv4_CLIENT_INITIALIZATION_ALLOCATION.h"
#include "DHCP_config.h"
#include "DHCP_config.h"
#include <stdio.h>
#include <string.h>
#include "TestabilityProtocolTypes.h"
#include "TestabilityProtocol_api.h"
#include "AbstractionAPI.h"
#include "commons_functions.h"
#include "DHCP_state_machine.h"

#define N_EXECUTE 2

uint8_t genaralStartTest3, dhcpInitClient3, genaralEndTest3;
extern Dhcp_config_t DHCP_config;

/***************************************************************************************************
*
*   FUNCTION NAME: DhcpInitClient_cb3
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
void DhcpInitClient_cb3 (TP_ResultID_t r)
{
    dhcpInitClient3 = r;
}

/***************************************************************************************************
*
*   FUNCTION NAME: GenaralStartTest_cb3
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
void GenaralStartTest_cb3(TP_ResultID_t r)
{
    genaralStartTest3 = r;
}

/***************************************************************************************************
*
*   FUNCTION NAME: GenaralEndTest_cb3
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
void GenaralEndTest_cb3(TP_ResultID_t r)
{
    genaralEndTest3 = r;
}

int DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_01()
{
	return 2;
}

/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_02
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_02:  DHCPREQUEST message - the 'ciaddr' option
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_02 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_02()
{
      DHCP_Packet dhcp_packet;
      Packet_filter filter;
      char* Option;
      int i;
      genaralStartTest3 = N_EXECUTE;
      dhcpInitClient3 = N_EXECUTE;
      genaralEndTest3 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb3);
      if (genaralStartTest3 == 0)
      {
          printf("\n Genaral Test will start with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_02 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb3, interf);
      while (dhcpInitClient3 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient3 == 0)
      {
           printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
           printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_02 failed : Configure_DhcpClient \n");
           TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
           return 1;
      }


      //Initialize the filter
      strcpy(filter.SrcAdd, "0.0.0.0");
      strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
      // 3. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
      dhcp_packet  = ListenDHCP(filter, DHCP_config.ParamListenTime);
      // 4. verify that the DUT: Sends "DHCPDISCOVER" Message
      if (dhcp_packet.length > 0 )
      {
            printf("\n The packet DHCP is received with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_02 failed : The packet received isn't a DHCP packet \n");
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }

      // Extract the options from DHCPDISCOVER
      Option = (char*)GetDHCPField(&dhcp_packet, DHCP, options);
      if ((uint8_t)GetDHCPField(&dhcp_packet, DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPDISCOVER)
      {
            printf("\n The DHCPDISCOVER is received with success  \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_02 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }

      // 5. TESTER: Verify that received DHCPDISCOVER Message contains: - Destination IP Address field is set to 0.0.0.0
      if (strcmp((char*)GetDHCPField(&dhcp_packet, DHCP, ciaddr), "0.0.0.0") == 0)
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_02 : The received DHCPDISCOVER Message contains ciaddr = 0.0.0.0 with success \n");
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_02 failed : The received DHCPDISCOVER Message contains ciaddr != 0.0.0.0 \n");
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }
}

/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_03
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_03: INIT state and forms a DHCPDISCOVER message - the 'chaddr' field
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_03 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_03()
{
      DHCP_Packet dhcp_packet;
      Packet_filter filter;
      char* Option;
      genaralStartTest3 = N_EXECUTE;
      dhcpInitClient3 = N_EXECUTE;
      genaralEndTest3 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb3);
      if (genaralStartTest3 == 0)
      {
          printf("\n Genaral Test will start with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_03 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb3, interf);
      while (dhcpInitClient3 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient3 == 0)
      {
           printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
           printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_03 failed : Configure_DhcpClient \n");
           TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
           return 1;
      }


      //Initialize the filter
      strcpy(filter.SrcAdd, "0.0.0.0");
      strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
      // 3. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
      dhcp_packet  = ListenDHCP(filter, DHCP_config.ParamListenTime);
      // 4. verify that the DUT: Sends "DHCPDISCOVER" Message
      if (dhcp_packet.length > 0 )
      {
            printf("\n The packet DHCP is received with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_03 failed : The packet received isn't a DHCP packet \n");
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }

      // Extract the options from DHCPDISCOVER
      Option = (char*)GetDHCPField(&dhcp_packet, DHCP, options);
      if ((uint8_t)GetDHCPField(&dhcp_packet, DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPDISCOVER)
      {
            printf("\n The DHCPDISCOVER is received with success  \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_03 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }
      /* 5. TESTER: Extracts the content of Source Harware Address field to <extractedSrcHwAddr> */
      strcpy((char*)DHCP_config.extractedSrcHwAddr, (char*)GetDHCPField(&dhcp_packet,ETHERNET,ETH_sourceMac));
      /* TESTER: Verify that received DHCPDISCOVER Message contains: - 'chaddr' field is set to extractedSrcHwAddr */
      if (strcmp((char*)GetDHCPField(&dhcp_packet, DHCP, chaddr), (char*)DHCP_config.extractedSrcHwAddr) == 0)
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_03 : The received DHCPDISCOVER Message contains 'chaddr' = extractedSrcHwAddr with success \n");
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_03 failed : The received DHCPDISCOVER Message contains 'chaddr' != extractedSrcHwAddr \n");
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }
}

/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04: Verify 'xid' of an arriving DHCPOFFER message
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04()
{
      DHCP_Packet dhcp_packet_1, dhcp_packet_2;
      Packet_filter filter;
      uint32_t extractedXID;
      char* Option;
      genaralStartTest3 = N_EXECUTE;
      dhcpInitClient3 = N_EXECUTE;
      genaralEndTest3 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb3);
      if (genaralStartTest3 == 0)
      {
            printf("\n Genaral Test will start with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb3, interf);
      while (dhcpInitClient3 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient3 == 0)
      {
            printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
      printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04 failed : Configure_DhcpClient \n");
      TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04 failed : Listen DHCP \n");
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }


      // 5. Extract the xid from DHCPDISCOVER : random_transaction_id_number
      extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number);

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
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04 failed : EditDHCPField \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }

      // Edit the random_transaction_id_number of the packet_created "DHCPOFFER" with the extractedXID
      if (EditDHCPField(&packet_created  , DHCP, random_transaction_id_number, (void*)(intptr_t)(extractedXID+1)) == 0)
      {
            printf("\n The field is edited with success, random_transaction_id_number = extractedXID + 1 \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04 failed : EditDHCPField, xid \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }

      if (EditDHCPField(&packet_created  , DHCP, yiaddr, (void*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
      {
            printf("\n The field is edited with success, yiaddr  \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04 failed : EditDHCPField, yiaddr \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }
      if (SendDHCP(packet_created) == 0 )
      {
            printf("\n The packet is sent with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04 failed : SendDHCP \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }


      // 7. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
      dhcp_packet_2  = ListenDHCP(filter, DHCP_config.ParamListenTime);

      // 8. verify that the DUT: Does not send "DHCPREQUEST" Message
      if (dhcp_packet_2.length > 0 )
      {
            printf("\n The packet DHCP is received with success \n");
            Option = (char*)GetDHCPField(&dhcp_packet_2, DHCP, options);
            if ((uint8_t)GetDHCPField(&dhcp_packet_2 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] != DHCPREQUEST)
            {
                  printf("\n DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04 : The field received (packet_type) isn't a DHCPREQUEST : success  \n");
                  free(option_field);
                  TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
                  return 0;
            }
            else
            {
                  printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04 failed : The field received (packet_type) is a DHCPREQUEST \n");
                  free(option_field);
                  TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
                  return 1;
            }
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04 PASSED : The packet received isn't a DHCP packet \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 0;
      }

      /* Extract the options from DHCP Message
      Option = (char*)GetDHCPField(&dhcp_packet_2, DHCP, options);
      if ((uint8_t)GetDHCPField(&dhcp_packet_2 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] != DHCPREQUEST)
      {
            printf("\n DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04 : The field received (packet_type) isn't a DHCPREQUEST : success  \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04 failed : The field received (packet_type) is a DHCPREQUEST \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }*/

}


/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05: Verify 'xid' of an arriving DHCPOFFER message
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05()
{
      DHCP_Packet dhcp_packet_1, dhcp_packet_2;
      Packet_filter filter;
      uint32_t extractedXID;
      char* Option;
      genaralStartTest3 = N_EXECUTE;
      dhcpInitClient3 = N_EXECUTE;
      genaralEndTest3 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb3);
      if (genaralStartTest3 == 0)
      {
            printf("\n Genaral Test will start with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb3, interf);
      while (dhcpInitClient3 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient3 == 0)
      {
            printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05 failed : Configure_DhcpClient \n");
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05 failed : Listen DHCP \n");
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }


      // Extract the xid from DHCPDISCOVER : random_transaction_id_number
      extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number);

      // 5. TESTER:DHCP Server <SERVER-1> Sends "DHCPACK" Message to DUT through <DIface-0>
      DHCP_Packet packet_created = CreateDHCP();
      dhcp_value *option_field;
      option_field = (dhcp_value*)malloc(sizeof(dhcp_value));
      option_field->data[0] = '\x63';
      option_field->data[1] = '\x82';
      option_field->data[2] = '\x53';
      option_field->data[3] = '\x63';

      option_field->data[4] = '\x35';
      option_field->data[5] = '\x01';
      option_field->data[6] = '\x05';
      option_field->data[7] = '\xff';
      option_field->len = 8;
      if (EditDHCPField(&packet_created  , DHCP, options, (void*)option_field) == 0)
      {
            printf("\n The field is edited with success, packet_type = DHCPACK \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05 failed : EditDHCPField \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }

      // Edit the random_transaction_id_number of the packet_created "DHCPACK" with the extractedXID
      if (EditDHCPField(&packet_created  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
      {
            printf("\n The field is edited with success, random_transaction_id_number = extractedXID  \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05 failed : EditDHCPField, xid \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }

      if (EditDHCPField(&packet_created  , DHCP, yiaddr, (void*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
      {
            printf("\n The field is edited with success, yiaddr \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05 failed : EditDHCPField, yiaddr \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }
      if (SendDHCP(packet_created) == 0 )
      {
            printf("\n The packet is sent with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05 failed : SendDHCP \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }


      // 6. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
      dhcp_packet_2  = ListenDHCP(filter, DHCP_config.ParamListenTime);

      // 7. verify that the DUT: Does not send "DHCPREQUEST" Message
      if (dhcp_packet_2.length > 0 )
      {
      printf("\n The packet DHCP is received with success \n");
      // Extract the options from DHCP Message
      Option = (char*)GetDHCPField(&dhcp_packet_2, DHCP, options);
      if ((uint8_t)GetDHCPField(&dhcp_packet_2 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] != DHCPREQUEST)
      {
            printf("\n DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05 : The field received (packet_type) isn't a DHCPREQUEST : success  \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05 failed : The field received (packet_type) is a DHCPREQUEST \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }
      }
      else
      {
      printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05 passed : No DHCP packet received\n");
      free(option_field);
      TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
      return 0;
      }

      // Extract the options from DHCP Message
      Option = (char*)GetDHCPField(&dhcp_packet_2, DHCP, options);
      if ((uint8_t)GetDHCPField(&dhcp_packet_2 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] != DHCPREQUEST)
      {
            printf("\n DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05 : The field received (packet_type) isn't a DHCPREQUEST : success  \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05 failed : The field received (packet_type) is a DHCPREQUEST \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }
}



/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06: : The DHCPREQUEST message contains the same 'xid' as the DHCPOFFER message
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06()
{
      DHCP_Packet dhcp_packet_1, dhcp_packet_2;
      Packet_filter filter;
      uint32_t extractedXID;
      char* Option;
      genaralStartTest3 = N_EXECUTE;
      dhcpInitClient3 = N_EXECUTE;
      genaralEndTest3 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb3);
      if (genaralStartTest3 == 0)
      {
            printf("\n Genaral Test will start with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb3, interf);
      while (dhcpInitClient3 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient3 == 0)
      {
            printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06 failed : Configure_DhcpClient \n");
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06 failed : Listen DHCP \n");
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }


      //  Extract the xid from DHCPDISCOVER : random_transaction_id_number
      extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number);

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
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06 failed : EditDHCPField \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }

      // Edit the random_transaction_id_number of the packet_created "DHCPOFFER" with the extractedXID
      if (EditDHCPField(&packet_created  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
      {
            printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06 failed : EditDHCPField, xid \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }

      if (EditDHCPField(&packet_created  , DHCP, yiaddr, (void*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
      {
            printf("\n The field is edited with success, yiaddr  \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06 failed : EditDHCPField, yiaddr \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }
      if (SendDHCP(packet_created) == 0 )
      {
            printf("\n The packet is sent with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06 failed : SendDHCP \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }


      // 7. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
      dhcp_packet_2  = ListenDHCP(filter, DHCP_config.ParamListenTime);

      // 8. verify that the DUT send "DHCPREQUEST" Message
      if (dhcp_packet_2.length > 0 )
      {
            printf("\n The packet DHCP is received with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06 failed : The packet received isn't a DHCP packet \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }

      // Extract the options from DHCP Message
      Option = (char*)GetDHCPField(&dhcp_packet_2, DHCP, options);
      if ((uint8_t)GetDHCPField(&dhcp_packet_2 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPREQUEST)
      {
            printf("\n  The field received (packet_type) is a DHCPREQUEST : success  \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06 failed : The field received (packet_type) isn't a DHCPREQUEST  \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }

      if ((uint32_t)GetDHCPField(&dhcp_packet_2 , DHCP, random_transaction_id_number) == extractedXID)
      {
            printf("\n DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06 : The DHCPREQUEST message contains the same 'xid' as the DHCPDISCOVER message with success  \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06 failed : The DHCPREQUEST message doesn't contain the same 'xid' as the DHCPDISCOVER message \n");
            free(option_field);
            TP_GenaralEndTest(GenaralEndTest_cb3, 0, (text){0});
            return 1;
      }
}

int DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_08()
{
	return 2;
}

int DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_09()
{
	return 2;
}

int DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_10()
{
	return 2;
}


