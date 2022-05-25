#include "DHCPv4_CLIENT_REACQUISITION.h"
#include "DHCP_config.h"
#include <stdio.h>
#include <string.h>
#include "TestabilityProtocolTypes.h"
#include "TestabilityProtocol_api.h"
#include "AbstractionAPI.h"
#include "DHCP_state_machine.h"

#define N_EXECUTE 2

uint8_t genaralStartTest6, dhcpInitClient6, genaralEndTest6;

extern Dhcp_config_t DHCP_config;

/***************************************************************************************************
*
*   FUNCTION NAME: DhcpInitClient_cb6
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
void DhcpInitClient_cb6 (TP_ResultID_t r)
{
    dhcpInitClient6 = r;
}




/***************************************************************************************************
*
*   FUNCTION NAME: GenaralStartTest_cb6
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
void GenaralStartTest_cb6(TP_ResultID_t r)
{
    genaralStartTest6 = r;
}




/***************************************************************************************************
*
*   FUNCTION NAME:GenaralEndTest_cb6
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
void GenaralEndTest_cb6(TP_ResultID_t r)
{
    genaralEndTest6 = r;
}


/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_REACQUISITION_01
 *
 ***************************************************************************************************/
/**
* @brief
*  DHCPv4_CLIENT_REACQUISITION_01: RENEWING state - send unicast DHCPREQUEST message
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_REACQUISITION_01 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_REACQUISITION_01()
{
      DHCP_Packet dhcp_packet;
      Packet_filter filter;
      char* Option;
      int i;
      genaralStartTest6 = N_EXECUTE;
      dhcpInitClient6 = N_EXECUTE;
      genaralEndTest6 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb6);
      if (genaralStartTest6 == 0)
      {
          printf("\n Genaral Test will start with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_REACQUISITION_01 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb6, interf);
      while (dhcpInitClient6 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient6 == 0)
      {
           printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
           printf("\n Test DHCPv4_CLIENT_REACQUISITION_01 failed : Configure_DhcpClient \n");
           TP_GenaralEndTest(GenaralEndTest_cb6, 0, (text){0});
           return 1;
      }

      // 3. DHCP Server <SERVER-1> Cause DUT to transit its state to DHCPCLIENT_STATE_RENEWING
      // 4. DUT: Transit finite state to DHCPCLIENT_STATE_RENEWING
      if (DHCPCLIENT_STATE_RENEWING() == 0)
      {
           printf("\n Test DHCPv4_CLIENT_REACQUISITION_01 : DHCPCLIENT_STATE_RENEWING() with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_REACQUISITION_01 failed : DHCPCLIENT_STATE_RENEWING() \n");
          TP_GenaralEndTest(GenaralEndTest_cb6, 0, (text){0});
          return 1;
      }

      //Initialize the filter
      strcpy(filter.SrcAdd, "0.0.0.0");
      strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
      // 5. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
      dhcp_packet = ListenDHCP(filter, DHCP_config.ParamListenTime);
      // 6. verify that the DUT: Sends "DHCPREQUEST" Message
      if (dhcp_packet.length > 0 )
      {
            printf("\n The packet DHCP is received with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REACQUISITION_01 failed : The packet received isn't a DHCP packet \n");
            TP_GenaralEndTest(GenaralEndTest_cb6, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_REACQUISITION_01 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
            TP_GenaralEndTest(GenaralEndTest_cb6, 0, (text){0});
            return 1;
      }

      // 7. TESTER: Verify that received DHCPREQUEST Message contains: - Destination IP Address field is set to <SERVER1-IP-ADDRESS>
      if (strcmp((char*)GetDHCPField(&dhcp_packet, IP, IP_DstAddress), (char*)DHCP_config.SERVERn_IP_ADDRESS) == 0)
      {
            printf("\n Test DHCPv4_CLIENT_REACQUISITION_01 : The received DHCPREQUEST Message contains IP_DstAddress = SERVERn_IP_ADDRESS with success \n");
            TP_GenaralEndTest(GenaralEndTest_cb6, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REACQUISITION_01 failed : The received DHCPREQUEST Message contains IP_DstAddress != SERVERn_IP_ADDRESS \n");
            TP_GenaralEndTest(GenaralEndTest_cb6, 0, (text){0});
            return 1;
      }
}

/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_REACQUISITION_02
 *
 ***************************************************************************************************/
/**
* @brief
*  DHCPv4_CLIENT_REACQUISITION_02: : On DHCPACK timeout move to REBINDING state and send DHCPREQUEST broadcast
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_REACQUISITION_02 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_REACQUISITION_02()
{
      DHCP_Packet dhcp_packet;
      Packet_filter filter;
      char* Option;
      int i;
      genaralStartTest6 = N_EXECUTE;
      dhcpInitClient6 = N_EXECUTE;
      genaralEndTest6 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb6);
      if (genaralStartTest6 == 0)
      {
          printf("\n Genaral Test will start with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_REACQUISITION_02 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb6, interf);
      while (dhcpInitClient6 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient6 == 0)
      {
           printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
           printf("\n Test DHCPv4_CLIENT_REACQUISITION_02 failed : Configure_DhcpClient \n");
           TP_GenaralEndTest(GenaralEndTest_cb6, 0, (text){0});
           return 1;
      }

      // 3. DHCP Server <SERVER-1> Cause DUT to transit its state to DHCPCLIENT_STATE_BOUND
      // 4. DUT: Transit finite state to DHCPCLIENT_STATE_BOUND
      if (DHCPCLIENT_STATE_BOUND() == 0)
      {
           printf("\n Test DHCPv4_CLIENT_REACQUISITION_02 : DHCPCLIENT_STATE_BOUND() with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_REACQUISITION_02 failed : DHCPCLIENT_STATE_BOUND() \n");
          TP_GenaralEndTest(GenaralEndTest_cb6, 0, (text){0});
          return 1;
      }

      // 5. Wait till (<REMOTE-CLIENT1-T2>-<ParamToleranceTime>) for DUT to go to a state just before T2 expires
      usleep(DHCP_config.REMOTE_CLIENTn_T2 - DHCP_config.ParamToleranceTime);

      //Initialize the filter
      strcpy(filter.SrcAdd, "0.0.0.0");
      strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
      // 6. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
      dhcp_packet  = ListenDHCP(filter, DHCP_config.REMOTE_CLIENTn_T2 - DHCP_config.ParamToleranceTime);
      // 7. verify that the DUT: Sends "DHCPREQUEST" Message
      if (dhcp_packet.length > 0 )
      {
            printf("\n The packet DHCP is received with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REACQUISITION_02 failed : The packet received isn't a DHCP packet \n");
            TP_GenaralEndTest(GenaralEndTest_cb6, 0, (text){0});
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
            printf("\n Test DHCPv4_CLIENT_REACQUISITION_02 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
            TP_GenaralEndTest(GenaralEndTest_cb6, 0, (text){0});
            return 1;
      }

      // 8. TESTER: Verify that received DHCPREQUEST Message contains: - Destination IP Address field is set to <SERVER1-IP-ADDRESS>
      if (strcmp((char*)GetDHCPField(&dhcp_packet, IP, IP_DstAddress), (char*)DHCP_config.IP_BROADCAST_ADDRESS) == 0)
      {
            printf("\n Test DHCPv4_CLIENT_REACQUISITION_02 : The received DHCPREQUEST Message contains IP_DstAddress = IP_BROADCAST_ADDRESS with success \n");
            TP_GenaralEndTest(GenaralEndTest_cb6, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REACQUISITION_02 failed : The received DHCPREQUEST Message contains IP_DstAddress != IP_BROADCAST_ADDRESS \n");
            TP_GenaralEndTest(GenaralEndTest_cb6, 0, (text){0});
            return 1;
      }
}

int DHCPv4_CLIENT_REACQUISITION_03()
{
	return 2;
}

int DHCPv4_CLIENT_REACQUISITION_04()
{
	return 2;
}

int DHCPv4_CLIENT_REACQUISITION_05()
{
	return 2;
}

int DHCPv4_CLIENT_REACQUISITION_06()
{
	return 2;
}

int DHCPv4_CLIENT_REACQUISITION_07()
{
	return 2;
}


/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_REACQUISITION_08
 *
 ***************************************************************************************************/
/**
* @brief
*  DHCPv4_CLIENT_REACQUISITION_08: : Request network initialization parameters after lease time expires
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_REACQUISITION_08 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_REACQUISITION_08()
{
      DHCP_Packet dhcp_packet;
      Packet_filter filter;
      char* Option;
      int i;
      genaralStartTest6 = N_EXECUTE;
      dhcpInitClient6 = N_EXECUTE;
      genaralEndTest6 = N_EXECUTE;
      //start test
      TP_GenaralStartTest(GenaralStartTest_cb6);
      if (genaralStartTest6 == 0)
      {
          printf("\n Genaral Test will start with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_REACQUISITION_08 failed : Genaral Start Test  \n");
      }
      // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
      // 2. TESTER: Externally cause DUT to bring up <DIface-0>
      //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
      text interf;
      interf.dataLength = 20;
      interf.Data = DHCP_config.DIface_0;
      TP_DhcpInitClient(DhcpInitClient_cb6, interf);
      while (dhcpInitClient6 == N_EXECUTE) {usleep(100);}
      if (dhcpInitClient6 == 0)
      {
           printf("\n Configure_DhcpClient with success \n");
      }
      else
      {
           printf("\n Test DHCPv4_CLIENT_REACQUISITION_08 failed : Configure_DhcpClient \n");
           TP_GenaralEndTest(GenaralEndTest_cb6, 0, (text){0});
           return 1;
      }

      // 3. DHCP Server <SERVER-1> Cause DUT to transit its state to DHCPCLIENT_STATE_BOUND
      // 4. DUT: Transit finite state to DHCPCLIENT_STATE_BOUND
      if (DHCPCLIENT_STATE_BOUND() == 0)
      {
           printf("\n Test DHCPv4_CLIENT_REACQUISITION_08 : DHCPCLIENT_STATE_BOUND() with success \n");
      }
      else
      {
          printf("\n Test DHCPv4_CLIENT_REACQUISITION_08 failed : DHCPCLIENT_STATE_BOUND() \n");
          TP_GenaralEndTest(GenaralEndTest_cb6, 0, (text){0});
          return 1;
      }

      // 5. : Wait till (<ParamLeaseTime>+<ParamToleranceTime>) for the lease on the DUT to expire
      sleep(DHCP_config.ParamLeaseTime + DHCP_config.ParamToleranceTime);

      //Initialize the filter
      strcpy(filter.SrcAdd, "0.0.0.0");
      strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
      // 6. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
      dhcp_packet  = ListenDHCP(filter, DHCP_config.ParamListenTime);
      // 7. verify that the DUT: Sends "DHCPDISCOVER" Message
      if (dhcp_packet.length > 0 )
      {
            printf("\n The packet DHCP is received with success \n");
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REACQUISITION_08 failed : The packet received isn't a DHCP packet \n");
            TP_GenaralEndTest(GenaralEndTest_cb6, 0, (text){0});
            return 1;
      }

      // Extract the options from DHCPDISCOVER
      Option = (char*)GetDHCPField(&dhcp_packet, DHCP, options);
      if ((uint8_t)GetDHCPField(&dhcp_packet, DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPDISCOVER)
      {
            printf("\n The DHCPDISCOVER is received with success  \n");
            TP_GenaralEndTest(GenaralEndTest_cb6, 0, (text){0});
            return 0;
      }
      else
      {
            printf("\n Test DHCPv4_CLIENT_REACQUISITION_08 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
            TP_GenaralEndTest(GenaralEndTest_cb6, 0, (text){0});
            return 1;
      }
}

