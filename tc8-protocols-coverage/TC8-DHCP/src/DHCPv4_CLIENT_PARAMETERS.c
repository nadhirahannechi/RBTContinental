#include "DHCPv4_CLIENT_PARAMETERS.h"
#include "DHCP_config.h"
#include <stdio.h>
#include <string.h>
#include "TestabilityProtocolTypes.h"
#include "TestabilityProtocol_api.h"
#include "AbstractionAPI.h"

#define N_EXECUTE 2

uint8_t genaralStartTest4, dhcpInitClient4, genaralEndTest4;
extern Dhcp_config_t DHCP_config;

/***************************************************************************************************
*
*   FUNCTION NAME: DhcpInitClient_cb4
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
void DhcpInitClient_cb4 (TP_ResultID_t r)
{
    dhcpInitClient4 = r;
}




/***************************************************************************************************
*
*   FUNCTION NAME: GenaralStartTest_cb4
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
void GenaralStartTest_cb4(TP_ResultID_t r)
{
    genaralStartTest4 = r;
}




/***************************************************************************************************
*
*   FUNCTION NAME: GenaralEndTest_cb4
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
void GenaralEndTest_cb4(TP_ResultID_t r)
{
    genaralEndTest4 = r;
}


/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_PARAMETERS_03
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_PARAMETERS_03: Override value for TTL
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_PARAMETERS_03 failed
*
*
* @note
* -
*
* @warning
* -
*
**/


int DHCPv4_CLIENT_PARAMETERS_03()
{
    DHCP_Packet dhcp_packet_1, dhcp_packet_2;
    Packet_filter filter;
    uint32_t extractedXID;
    char* Option;
    int i;
    genaralStartTest4 = N_EXECUTE;
    dhcpInitClient4 = N_EXECUTE;
    genaralEndTest4 = N_EXECUTE;
    //start test
    TP_GenaralStartTest(GenaralStartTest_cb4);
    if (genaralStartTest4 == 0)
    {
      printf("\n Genaral Test will start with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_PARAMETERS_03 failed : Genaral Start Test  \n");
    }
    // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
    // 2. TESTER: Externally cause DUT to bring up <DIface-0>
    //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
    text interf;
    interf.dataLength = 20;
    interf.Data = DHCP_config.DIface_0;
    TP_DhcpInitClient(DhcpInitClient_cb4, interf);
    while (dhcpInitClient4 == N_EXECUTE) {usleep(100);}
    if (dhcpInitClient4 == 0)
    {
      printf("\n Configure_DhcpClient with success \n");
    }
    else
    {
      printf("\n Test DHCPv4_CLIENT_PARAMETERS_03 failed : Configure_DhcpClient \n");
      TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
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
      printf("\n Test DHCPv4_CLIENT_PARAMETERS_03 failed : Listen DHCP \n");
      TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
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
      printf("\n Test DHCPv4_CLIENT_PARAMETERS_03 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
      TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
      return 1;
    }

    // Extract the xid from DHCPDISCOVER : random_transaction_id_number
    extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number);

    // 5. TESTER:DHCP Server <SERVER-1> Sends "DHCPOFFER" Message to DUT through <DIface-0>
    DHCP_Packet packet_created = CreateDHCP();
    dhcp_value *option_field;
    option_field = (dhcp_value*)malloc(sizeof(dhcp_value));
    //Magic_cookie
    option_field->data[0] = '\x63';
    option_field->data[1] = '\x82';
    option_field->data[2] = '\x53';
    option_field->data[3] = '\x63';

    //Message Type Option ==> DHCPOFFER
    option_field->data[4] = '\x35';
    option_field->data[5] = '\x01';
    option_field->data[6] = '\x02';

  /* Edit the options of the packet_created "DHCPOFFER"
            - Message Option containing:
            - Type field set to 51 (IP Address Lease Time Option)
            - Length field set to 4
            - Value set to <ParamLeaseTime>
  */
 
  option_field->data[7] = '\x33';
  option_field->data[8] = '\x04';
  option_field->data[9] = (DHCP_config.ParamLeaseTime >> 24) & 0xff;
  option_field->data[10] = (DHCP_config.ParamLeaseTime >> 16) & 0xff;
  option_field->data[11] = (DHCP_config.ParamLeaseTime >> 8) & 0xff;
  option_field->data[12] = DHCP_config.ParamLeaseTime & 0xff;
  option_field->data[13] = '\xff';
  option_field->len = 14;
  if (EditDHCPField(&packet_created  , DHCP, options, (void*)option_field) == 0)
  {
    printf("\n The field is edited with success, options = Option \n");
  }
  else
  {
    printf("\n Test DHCPv4_CLIENT_PARAMETERS_03 failed : EditDHCPField, options \n");
    free(option_field);
    TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
    return 1;
  }

  // Edit the random_transaction_id_number of the packet_created "DHCPOFFER" with the extractedXID
  if (EditDHCPField(&packet_created  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
  {
    printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
  }
  else
  {
    printf("\n Test DHCPv4_CLIENT_PARAMETERS_03 failed : EditDHCPField, xid \n");
    free(option_field);
    TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
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
    TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
    return 1;
  }

  if (SendDHCP(packet_created) == 0 )
  {
    printf("\n The packet is sent with success \n");
  }
  else
  {
    printf("\n Test DHCPv4_CLIENT_PARAMETERS_03 failed : SendDHCP \n");
    free(option_field);
    TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
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
    printf("\n Test DHCPv4_CLIENT_PARAMETERS_03 failed : The packet received isn't a DHCP packet \n");
    free(option_field);
    TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
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
    printf("\n Test DHCPv4_CLIENT_PARAMETERS_03 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
    free(option_field);
    TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
    return 1;
  }

  /* 8. TESTER: Verify that received DHCPREQUEST Message contains
       - Message Option containing:
          - Type field set to 51 (IP Address Lease Time Option)
          - Length field set to 4
          - Value set to <ParamLeaseTime>
  */

  i=4;
  while (Option[i] != 51 && Option[i] != 255)
  {
     i = i + 2 + Option[i+1];
  }
  if (Option[i] == 51)
  {
    if(Option[i+1] == 4 && (Option[i+2] == (DHCP_config.ParamLeaseTime >> 24) & 0xff) && (Option[i+3] == (DHCP_config.ParamLeaseTime >> 16) & 0xff) && (Option[i+4] == (DHCP_config.ParamLeaseTime >> 8) & 0xff) && (Option[i+2] == DHCP_config.ParamLeaseTime & 0xff))
    {
      printf("\n Test DHCPv4_CLIENT_PARAMETERS_03 : The octet of DHCP Options is set to IP Address Lease Time Option with success \n");
      free(option_field);
      TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
      return 0;
    }
    else
    {
      printf("\n Test DHCPv4_CLIENT_PARAMETERS_03 failed : The four octets of DHCP Options is NOT set to IP Address Lease Time Option \n");
      free(option_field);
      TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
      return 1;
    }

  }
  else
  {
    printf("\n Test DHCPv4_CLIENT_PARAMETERS_03 failed : The IP Address Lease Time Option of DHCP Options doesn't exist \n");
    free(option_field);
    TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
    return 1;
  }

}


/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPv4_CLIENT_PARAMETERS_04
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPv4_CLIENT_PARAMETERS_04: Use same parameters in DHCPREQUEST message as in DHCPDISCOVER
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPv4_CLIENT_PARAMETERS_04 failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPv4_CLIENT_PARAMETERS_04()
{
    DHCP_Packet dhcp_packet_1, dhcp_packet_2;
    Packet_filter filter;
    uint32_t extractedXID;
    char* Option;
    char* extractedParamReq;
    char* extractedParamReqTest;
    int i, j, length;
    genaralStartTest4 = N_EXECUTE;
    dhcpInitClient4 = N_EXECUTE;
    genaralEndTest4 = N_EXECUTE;
    //start test
    TP_GenaralStartTest(GenaralStartTest_cb4);
    if (genaralStartTest4 == 0)
    {
        printf("\n Genaral Test will start with success \n");
    }
    else
    {
        printf("\n Test DHCPv4_CLIENT_PARAMETERS_04 failed : Genaral Start Test  \n");
    }
   // 1. DUT CONFIGURE: Externally configure DHCP Client on <DIface-0>
   // 2. TESTER: Externally cause DUT to bring up <DIface-0>
   //    Initialize the DHCP Client by use of t : interface  <DIface-0> example "eth1.5"
   text interf;
   interf.dataLength = 20;
   interf.Data = DHCP_config.DIface_0;
   TP_DhcpInitClient(DhcpInitClient_cb4, interf);
   while (dhcpInitClient4 == N_EXECUTE) {usleep(100);}
   if (dhcpInitClient4 == 0)
   {
        printf("\n Configure_DhcpClient with success \n");
   }
   else
   {
        printf("\n Test DHCPv4_CLIENT_PARAMETERS_04 failed : Configure_DhcpClient \n");
        TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
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
        printf("\n Test DHCPv4_CLIENT_PARAMETERS_04 failed : Listen DHCP \n");
        TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
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
             printf("\n Test DHCPv4_CLIENT_PARAMETERS_04 failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
             TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
             return 1;
  }


  // Extract the xid from DHCPDISCOVER : random_transaction_id_number
  extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number);

  // 5. TESTER: Extracts the content of 55 (Parameter Request List Option) to <extractedParamReq>
  i = 4;
  length = 0;
  while (Option[i] != 55 && Option[i] != 255)
  {
    i = i + 2 + Option[i+1];
  }
  if (Option[i] == 55)
  { j = i+2;
    //length => the length of the value of (Parameter Request List Option
    while(length<i+1)
    {
        strcat(extractedParamReq, &Option[j]);
        j++;
        length++;
    }
    printf("\n The Parameter Request List Option is set to extractedParamReq with success \n");

  }
  else
  {
    printf("\n Test DHCPv4_CLIENT_PARAMETERS_04 failed : TheParameter Request List Option of DHCP Options doesn't exist) \n");

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
               printf("\n Test DHCPv4_CLIENT_PARAMETERS_04 failed : EditDHCPField \n");
               free(option_field);
               TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
               return 1;
  }

  // Edit the random_transaction_id_number of the packet_created "DHCPOFFER" with the extractedXID
  if (EditDHCPField(&packet_created  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
  {
             printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
  }
  else
  {
             printf("\n Test DHCPv4_CLIENT_PARAMETERS_04 failed : EditDHCPField, xid \n");
             free(option_field);
             TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
             return 1;
  }

  if (EditDHCPField(&packet_created  , DHCP, yiaddr, (void*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
  {
            printf("\n The field is edited with success, yiaddr  \n");
  }
  else
  {
           printf("\n Test DHCPv4_CLIENT_PARAMETERS_04 failed : EditDHCPField, yiaddr \n");
           free(option_field);
           TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
           return 1;
  }
  if (SendDHCP(packet_created) == 0 )
  {
               printf("\n The packet is sent with success \n");
  }
  else
  {
    printf("\n Test DHCPv4_CLIENT_PARAMETERS_04 failed : SendDHCP \n");
    free(option_field);
    TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
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
    printf("\n Test DHCPv4_CLIENT_PARAMETERS_04 failed : The packet received isn't a DHCP packet \n");
    free(option_field);
    TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
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
               printf("\n Test DHCPv4_CLIENT_PARAMETERS_04 failed : The field received (packet_type) isn't a DHCPREQUEST \n");
               free(option_field);
               TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
               return 1;
  }

  /* 9. TESTER: Verify that received DHCPREQUEST Message contains
         - Message Option containing:
         - extractedParamReq
  */

  i=4;
  length = 0;
  while (Option[i] != 55 && Option[i] != 255)
  {
    i = i + 2 + Option[i+1];
  }
  if (Option[i] == 55)
  {
    //length => the length of the value of (Parameter Request List Option
    while(length<i+1)
    {
        strcat(extractedParamReqTest, &Option[j]);
        j++;
        length++;
    }
    printf("\n The Parameter Request List Option is set to extractedParamReq with success \n");

  }
  else
  {
    printf("\n Test DHCPv4_CLIENT_PARAMETERS_04 failed : The Parameter Request List Option of DHCP Options doesn't exist \n");
    free(option_field);
    TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
    return 1;
  }


  if (strcmp(extractedParamReq, extractedParamReqTest) == 0)
  {
        printf("\n Test DHCPv4_CLIENT_PARAMETERS_04 : The received DHCPREQUEST Message contains extractedParamReqTest = extractedParamReq  \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
        return 0;
  }
  else
  {
        printf("\n Test DHCPv4_CLIENT_PARAMETERS_04 failed : The received DHCPREQUEST Message contains extractedParamReqTest != extractedParamReq \n");
        free(option_field);
        TP_GenaralEndTest(GenaralEndTest_cb4, 0, (text){0});
        return 1;
  }
}


