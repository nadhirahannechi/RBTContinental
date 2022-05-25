#include "DHCP_state_machine.h"
#include "DHCP_config.h"
#include <stdio.h>
#include <string.h>

extern Dhcp_config_t DHCP_config;

/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPCLIENT_STATE_REQUESTING
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPCLIENT_STATE_REQUESTING: transit the state to DHCPCLIENT_STATE_REQUESTING
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPCLIENT_STATE_REQUESTING failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPCLIENT_STATE_REQUESTING()
{
   DHCP_Packet dhcp_packet_1, dhcp_packet_2;
   Packet_filter filter;
   uint32_t extractedXID;
   char* Option;

  //Initialize the filter
  strcpy(filter.SrcAdd, "0.0.0.0");
  strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
  // 1. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
  dhcp_packet_1 = ListenDHCP(filter, DHCP_config.ParamListenTime);
  //length > 0 => a DHCP Packet is received to a server
  if(dhcp_packet_1.length > 0)
  {
      printf("\n A DHCP Packet is received with success \n");
  }
  else
  {
      printf("\n DHCPCLIENT_STATE_REQUESTING failed : Listen DHCP \n");
      return 1;
  }


  // Extract the options from DHCPDISCOVER
  Option = (char*)GetDHCPField(&dhcp_packet_1, DHCP, options);
  // 2. verify that the DUT: Sends "DHCPDISCOVER" Message  (packet_type of dhcp_field)
  if ((uint8_t)GetDHCPField(&dhcp_packet_1 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPDISCOVER)
  {
      printf("\n A DHCPDISCOVER is received with success \n");
  }
  else
  {
      printf("\n DHCPCLIENT_STATE_REQUESTING failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
      return 1;
  }

  // Extract the xid from DHCPDISCOVER : random_transaction_id_number
  extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number);

  // 3. TESTER:DHCP Server <SERVER-1> Sends "DHCPOFFER" Message to DUT through <DIface-0>
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
      return 1;
  }


  // 4. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
  dhcp_packet_2  = ListenDHCP(filter, DHCP_config.ParamListenTime);

  // 5. verify that the DUT: Sends "DHCPREQUEST" Message
  if (dhcp_packet_2.length > 0 )
  {
      printf("\n The packet DHCP is received with success \n");
  }
  else
  {
      printf("\n DHCPCLIENT_STATE_REQUESTING failed : The packet received isn't a DHCP packet \n");
      free(option_field);
      return 1;
  }

  // Extract the options from DHCPREQUEST
  Option = (char*)GetDHCPField(&dhcp_packet_2, DHCP, options);
  if ((uint8_t)GetDHCPField(&dhcp_packet_2 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPREQUEST)
  {
      printf("\n DHCPCLIENT_STATE_REQUESTING : The DHCPREQUEST is received with success  \n");
      free(option_field);
      return 0;
  }
  else
  {
      printf("\n DHCPCLIENT_STATE_REQUESTING failed : The field received (packet_type) isn't a DHCPREQUEST \n");
      free(option_field);
      return 1;
  }
}


/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPCLIENT_STATE_BOUND
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPCLIENT_STATE_BOUND: transit the state to DHCPCLIENT_STATE_BOUND
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPCLIENT_STATE_BOUND failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPCLIENT_STATE_BOUND()
{
    DHCP_Packet dhcp_packet_1, dhcp_packet_2;
    Packet_filter filter;
    uint32_t extractedXID;
    char* Option;
    char* requested_add;
    int i;

    //Initialize the filter
    strcpy(filter.SrcAdd, "0.0.0.0");
    strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
    // 1. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
    dhcp_packet_1 = ListenDHCP(filter, DHCP_config.ParamListenTime);
    //length > 0 => a DHCP Packet is received to a server
    if(dhcp_packet_1.length > 0)
    {
        printf("\n A DHCP Packet is received with success \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_BOUND failed : Listen DHCP \n");
        return 1;
    }


    // Extract the options from DHCPDISCOVER
    Option = (char*)GetDHCPField(&dhcp_packet_1, DHCP, options);
    // 2. verify that the DUT: Sends "DHCPDISCOVER" Message  (packet_type of dhcp_field)
    if ((uint8_t)GetDHCPField(&dhcp_packet_1 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPDISCOVER)
    {
        printf("\n A DHCPDISCOVER is received with success \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_BOUND failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
        return 1;
    }

    // Extract the xid from DHCPDISCOVER : random_transaction_id_number
    extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number);

    // 3. TESTER:DHCP Server <SERVER-1> Sends "DHCPOFFER" Message to DUT through <DIface-0>
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
    option_field_1->data[7] = '\xff';
    option_field_1->len = 8;
    if (EditDHCPField(&packet_created_1  , DHCP, options, (void*)option_field_1) == 0)
    {
        printf("\n The field is edited with success, packet_type = DHCPOFFER \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_BOUND failed : EditDHCPField, packet_type \n");
        free(option_field_1);
        return 1;
    }

    // Edit the random_transaction_id_number of the packet_created "DHCPOFFER" with the extractedXID
    if (EditDHCPField(&packet_created_1  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
    {
        printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_BOUND failed : EditDHCPField, xid \n");
        free(option_field_1);
        return 1;
    }

    if (EditDHCPField(&packet_created_1  , DHCP, yiaddr, (void*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
    {
        printf("\n The field is edited with success, yiaddr  \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_BOUND failed : EditDHCPField, yiaddr \n");
        free(option_field_1);
        return 1;
    }

    if (SendDHCP(packet_created_1) == 0 )
    {
        printf("\n The packet is sent with success \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_BOUND failed : SendDHCP \n");
        free(option_field_1);
        return 1;
    }


    // 4. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
    dhcp_packet_2  = ListenDHCP(filter, DHCP_config.ParamListenTime);

    // 5. verify that the DUT: Sends "DHCPREQUEST" Message
    if (dhcp_packet_2.length > 0 )
    {
        printf("\n The packet DHCP is received with success \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_BOUND failed : The packet received isn't a DHCP packet \n");
        free(option_field_1);
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
        printf("\n DHCPCLIENT_STATE_BOUND failed : The field received (packet_type) isn't a DHCPREQUEST \n");
        free(option_field_1);
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
        printf("\n DHCPCLIENT_STATE_BOUND failed : The Requested IP Address Option of DHCP Options doesn't exist \n");
        free(option_field_1);
        return 1;
    }


    // 6. TESTER:DHCP Server <SERVER-1> Sends "DHCPACK" Message to DUT through <DIface-0>
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
    option_field_2->data[7] = '\xff';
    option_field_2->len = 8;
    if (EditDHCPField(&packet_created_2  , DHCP, options, (void*)option_field_2) == 0)
    {
        printf("\n The field is edited with success, packet_type = DHCPACK \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_BOUND failed : EditDHCPField, packet_type \n");
        free(option_field_1);
        free(option_field_2);
        return 1;
    }

    // Edit the random_transaction_id_number of the packet_created "DHCPACK" with the extractedXID
    if (EditDHCPField(&packet_created_2  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
    {
        printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_BOUND failed : EditDHCPField, xid \n");
        free(option_field_1);
        free(option_field_2);
        return 1;
    }

    if (EditDHCPField(&packet_created_2  , DHCP, yiaddr, (void*)requested_add) == 0)
    {
        printf("\n The field is edited with success, yiaddr  \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_BOUND failed : EditDHCPField, yiaddr \n");
        free(option_field_1);
        free(option_field_2);
        return 1;
    }

    if (SendDHCP(packet_created_2) == 0 )
    {
        printf("\n DHCPCLIENT_STATE_BOUND : The packet is sent with success \n");
        free(option_field_1);
        free(option_field_2);
        return 0;
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_BOUND failed : SendDHCP \n");
        free(option_field_1);
        free(option_field_2);
        return 1;
    }
}

/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPCLIENT_STATE_RENEWING
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPCLIENT_STATE_RENEWING: transit the state to DHCPCLIENT_STATE_RENEWING
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPCLIENT_STATE_RENEWING failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPCLIENT_STATE_RENEWING()
{
   DHCP_Packet dhcp_packet_1, dhcp_packet_2, dhcp_packet_3;
   Packet_filter filter;
   uint32_t extractedXID;
   char* Option;
   char* requested_add;
   int i;

  //Initialize the filter
  strcpy(filter.SrcAdd, "0.0.0.0");
  strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
  // 1. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
  dhcp_packet_1 = ListenDHCP(filter, DHCP_config.ParamListenTime);
  //length > 0 => a DHCP Packet is received to a server
  if(dhcp_packet_1.length > 0)
  {
      printf("\n A DHCP Packet is received with success \n");
  }
  else
  {
      printf("\n DHCPCLIENT_STATE_RENEWING failed : Listen DHCP \n");
      return 1;
  }


  // Extract the options from DHCPDISCOVER
  Option = (char*)GetDHCPField(&dhcp_packet_1, DHCP, options);
  // 2. verify that the DUT: Sends "DHCPDISCOVER" Message  (packet_type of dhcp_field)
  if ((uint8_t)GetDHCPField(&dhcp_packet_1 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPDISCOVER)
  {
      printf("\n A DHCPDISCOVER is received with success \n");
  }
  else
  {
      printf("\n DHCPCLIENT_STATE_RENEWING failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
      return 1;
    }

  // Extract the xid from DHCPDISCOVER : random_transaction_id_number
  extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number);

  // 3. TESTER:DHCP Server <SERVER-1> Sends "DHCPOFFER" Message to DUT through <DIface-0>
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
  option_field_1->data[7] = '\xff';
  option_field_1->len = 8;
  if (EditDHCPField(&packet_created_1  , DHCP, options, (void*)option_field_1) == 0)
  {
      printf("\n The field is edited with success, packet_type = DHCPOFFER \n");
  }
  else
  {
      printf("\n DHCPCLIENT_STATE_RENEWING failed : EditDHCPField, packet_type \n");
      free(option_field_1);
      return 1;
  }

  // Edit the random_transaction_id_number of the packet_created "DHCPOFFER" with the extractedXID
  if (EditDHCPField(&packet_created_1  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
  {
      printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
  }
  else
  {
      printf("\n DHCPCLIENT_STATE_RENEWING failed : EditDHCPField, xid \n");
      free(option_field_1);
      return 1;
  }

  if (EditDHCPField(&packet_created_1  , DHCP, yiaddr, (void*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
  {
      printf("\n The field is edited with success, yiaddr  \n");
  }
  else
  {
      printf("\n DHCPCLIENT_STATE_RENEWING failed : EditDHCPField, yiaddr \n");
      free(option_field_1);
      return 1;
  }

  if (SendDHCP(packet_created_1) == 0 )
  {
      printf("\n The packet is sent with success \n");
  }
  else
  {
      printf("\n DHCPCLIENT_STATE_RENEWING failed : SendDHCP \n");
      free(option_field_1);
      return 1;
  }


  // 4. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
  dhcp_packet_2  = ListenDHCP(filter, DHCP_config.ParamListenTime);

  // 5. verify that the DUT: Sends "DHCPREQUEST" Message
  if (dhcp_packet_2.length > 0 )
  {
      printf("\n The packet DHCP is received with success \n");
  }
  else
  {
      printf("\n DHCPCLIENT_STATE_RENEWING failed : The packet received isn't a DHCP packet \n");
      free(option_field_1);
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
      printf("\n DHCPCLIENT_STATE_RENEWING failed : The field received (packet_type) isn't a DHCPREQUEST \n");
      free(option_field_1);
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
      printf("\n DHCPCLIENT_STATE_RENEWING failed : The Requested IP Address Option of DHCP Options doesn't exist \n");
      free(option_field_1);
      return 1;
  }


  // 6. TESTER:DHCP Server <SERVER-1> Sends "DHCPACK" Message to DUT through <DIface-0>
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
  option_field_2->data[7] = '\xff';
  option_field_2->len = 8;
  if (EditDHCPField(&packet_created_2  , DHCP, options, (void*)option_field_2) == 0)
  {
      printf("\n The field is edited with success, packet_type = DHCPACK \n");
  }
  else
  {
      printf("\n DHCPCLIENT_STATE_RENEWING failed : EditDHCPField, packet_type \n");
      free(option_field_1);
      free(option_field_2);
      return 1;
  }

  // Edit the random_transaction_id_number of the packet_created "DHCPACK" with the extractedXID
  if (EditDHCPField(&packet_created_2  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
  {
      printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
  }
  else
  {
      printf("\n DHCPCLIENT_STATE_RENEWING failed : EditDHCPField, xid \n");
      free(option_field_1);
      free(option_field_2);
      return 1;
  }

  if (EditDHCPField(&packet_created_2  , DHCP, yiaddr, (void*)requested_add) == 0)
  {
      printf("\n The field is edited with success, yiaddr  \n");
  }
  else
  {
      printf("\n DHCPCLIENT_STATE_RENEWING failed : EditDHCPField, yiaddr \n");
      free(option_field_1);
      free(option_field_2);
      return 1;
  }

  if (SendDHCP(packet_created_2) == 0 )
  {
      printf("\n The packet is sent with success \n");
  }
  else
  {
      printf("\n DHCPCLIENT_STATE_RENEWING failed : SendDHCP \n");
      free(option_field_1);
      free(option_field_2);
      return 1;
  }

  // 7. TESTER:DHCP Server <SERVER-1> Listens (upto 50% of <ParamLeaseTime>) on <DIface-0>
  dhcp_packet_3  = ListenDHCP(filter, DHCP_config.ParamLeaseTime/2 + DHCP_config.ParamListenTime);

  // 8. verify that the DUT: Sends "DHCPREQUEST" Message
  if (dhcp_packet_3.length > 0 )
  {
      printf("\n The packet DHCP is received with success \n");
  }
  else
  {
      printf("\n DHCPCLIENT_STATE_RENEWING failed : The packet received isn't a DHCP packet \n");
      free(option_field_1);
      free(option_field_2);
      return 1;
  }
  // Extract the options from DHCPREQUEST
  Option = (char*)GetDHCPField(&dhcp_packet_3, DHCP, options);
  if ((uint8_t)GetDHCPField(&dhcp_packet_3 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPREQUEST)
  {
      printf("\n DHCPCLIENT_STATE_RENEWING : The DHCPREQUEST is received with success  \n");
      free(option_field_1);
      free(option_field_2);
      return 0;

  }
  else
  {
      printf("\n DHCPCLIENT_STATE_RENEWING failed : The field received (packet_type) isn't a DHCPREQUEST \n");
      free(option_field_1);
      free(option_field_2);
      return 1;
  }
}




/***************************************************************************************************
 *
 *   FUNCTION NAME: DHCPCLIENT_STATE_REBINDING
 *
 ***************************************************************************************************/
/**
* @brief
* DHCPCLIENT_STATE_REBINDING: transit the state to DHCPCLIENT_STATE_REBINDING
*
* @par
* -
*
* @return int   0 if Done with success
*               1 if DHCPCLIENT_STATE_REBINDING failed
*
*
* @note
* -
*
* @warning
* -
*
**/
int DHCPCLIENT_STATE_REBINDING()
{
    DHCP_Packet dhcp_packet_1, dhcp_packet_2, dhcp_packet_3, dhcp_packet_4;
    Packet_filter filter;
    uint32_t extractedXID;
    char* Option;
    char* requested_add;
    int i;

    //Initialize the filter
    strcpy(filter.SrcAdd, "0.0.0.0");
    strcpy(filter.dstAdd, (char*)DHCP_config.IP_BROADCAST_ADDRESS);
    // 1. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
    dhcp_packet_1 = ListenDHCP(filter, DHCP_config.ParamListenTime);
    //length > 0 => a DHCP Packet is received to a server
    if(dhcp_packet_1.length > 0)
    {
        printf("\n A DHCP Packet is received with success \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REBINDING failed : Listen DHCP \n");
        return 1;
    }


    // Extract the options from DHCPDISCOVER
    Option = (char*)GetDHCPField(&dhcp_packet_1, DHCP, options);
    // 2. verify that the DUT: Sends "DHCPDISCOVER" Message  (packet_type of dhcp_field)
    if ((uint8_t)GetDHCPField(&dhcp_packet_1 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPDISCOVER)
    {
        printf("\n A DHCPDISCOVER is received with success \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REBINDING failed : The field received (packet_type) isn't a DHCPDISCOVER \n");
        return 1;
    }

    // Extract the xid from DHCPDISCOVER : random_transaction_id_number
    extractedXID = (uint32_t)GetDHCPField(&dhcp_packet_1 , DHCP, random_transaction_id_number);

    // 3. TESTER:DHCP Server <SERVER-1> Sends "DHCPOFFER" Message to DUT through <DIface-0>
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
    option_field_1->data[7] = '\xff';
    option_field_1->len = 8;
    if (EditDHCPField(&packet_created_1  , DHCP, options, (void*)option_field_1) == 0)
    {
        printf("\n The field is edited with success, packet_type = DHCPOFFER \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REBINDING failed : EditDHCPField, packet_type \n");
        free(option_field_1);
        return 1;
    }

    // Edit the random_transaction_id_number of the packet_created "DHCPOFFER" with the extractedXID
    if (EditDHCPField(&packet_created_1  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
    {
        printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REBINDING failed : EditDHCPField, xid \n");
        free(option_field_1);
        return 1;
    }

    if (EditDHCPField(&packet_created_1  , DHCP, yiaddr, (void*)DHCP_config.SERVERn_IP_POOL_L_M) == 0)
    {
        printf("\n The field is edited with success, yiaddr  \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REBINDING failed : EditDHCPField, yiaddr \n");
        free(option_field_1);
        return 1;
    }

    if (SendDHCP(packet_created_1) == 0 )
    {
        printf("\n The packet is sent with success \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REBINDING failed : SendDHCP \n");
        free(option_field_1);
        return 1;
    }


    // 4. TESTER:DHCP Server <SERVER-1> Listens (upto <ParamListenTime>) on <DIface-0>
    dhcp_packet_2  = ListenDHCP(filter, DHCP_config.ParamListenTime);

    // 5. verify that the DUT: Sends "DHCPREQUEST" Message
    if (dhcp_packet_2.length > 0 )
    {
        printf("\n The packet DHCP is received with success \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REBINDING failed : The packet received isn't a DHCP packet \n");
        free(option_field_1);
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
        printf("\n DHCPCLIENT_STATE_REBINDING failed : The field received (packet_type) isn't a DHCPREQUEST \n");
        free(option_field_1);
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
        printf("\n DHCPCLIENT_STATE_REBINDING failed : The Requested IP Address Option of DHCP Options doesn't exist \n");
        free(option_field_1);
        return 1;
    }


    // 6. TESTER:DHCP Server <SERVER-1> Sends "DHCPACK" Message to DUT through <DIface-0>
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
    option_field_2->data[7] = '\xff';
    option_field_2->len = 8;
    if (EditDHCPField(&packet_created_2  , DHCP, options, (void*)option_field_2) == 0)
    {
        printf("\n The field is edited with success, packet_type = DHCPACK \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REBINDING failed : EditDHCPField, packet_type \n");
        free(option_field_1);
        free(option_field_2);
        return 1;
    }

    // Edit the random_transaction_id_number of the packet_created "DHCPACK" with the extractedXID
    if (EditDHCPField(&packet_created_2  , DHCP, random_transaction_id_number, (void*)(intptr_t)extractedXID) == 0)
    {
        printf("\n The field is edited with success, random_transaction_id_number = extractedXID \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REBINDING failed : EditDHCPField, xid \n");
        free(option_field_1);
        free(option_field_2);
        return 1;
    }

    if (EditDHCPField(&packet_created_2  , DHCP, yiaddr, (void*)requested_add) == 0)
    {
        printf("\n The field is edited with success, yiaddr  \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REBINDING failed : EditDHCPField, yiaddr \n");
        free(option_field_1);
        free(option_field_2);
        return 1;
    }

    if (SendDHCP(packet_created_2) == 0 )
    {
        printf("\n The packet is sent with success \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REBINDING failed : SendDHCP \n");
        free(option_field_1);
        free(option_field_2);
        return 1;
    }

    // 7. TESTER:DHCP Server <SERVER-1> Listens (upto 50% of <ParamLeaseTime>) on <DIface-0>
    dhcp_packet_3  = ListenDHCP(filter, DHCP_config.ParamLeaseTime/2 + DHCP_config.ParamListenTime);

    // 8. verify that the DUT: Sends "DHCPREQUEST" Message
    if (dhcp_packet_3.length > 0 )
    {
        printf("\n The packet DHCP is received with success \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REBINDING failed : The packet received isn't a DHCP packet \n");
        free(option_field_1);
        free(option_field_2);
        return 1;
    }
    // Extract the options from DHCPREQUEST
    Option = (char*)GetDHCPField(&dhcp_packet_3, DHCP, options);
    if ((uint8_t)GetDHCPField(&dhcp_packet_3 , DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPREQUEST)
    {
        printf("\n The DHCPREQUEST is received with success  \n");


    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REBINDING failed : The field received (packet_type) isn't a DHCPREQUEST \n");
        free(option_field_1);
        free(option_field_2);
        return 1;
    }

    // 9. TESTER:DHCP Server <SERVER-1> Listens (upto 87.5% of <ParamLeaseTime>) on <DIface-0>
    // (uint32_t)ceil((DHCP_config.ParamLeaseTime*0.875)
    dhcp_packet_4  = ListenDHCP(filter, DHCP_config.REMOTE_CLIENTn_T2 + DHCP_config.ParamListenTime);

    // 10. verify that the DUT: Sends "DHCPREQUEST" Message
    if (dhcp_packet_4.length > 0 )
    {
        printf("\n The packet DHCP is received with success \n");
    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REBINDING failed : The packet received isn't a DHCP packet \n");
        return 1;
    }
    // Extract the options from DHCPREQUEST
    Option = (char*)GetDHCPField(&dhcp_packet_4, DHCP, options);
    if ((uint8_t)GetDHCPField(&dhcp_packet_4, DHCP, packet_type) == 1 && Option[4] == 53 && Option[6] == DHCPREQUEST)
    {
        printf("\n The DHCPREQUEST is received with success  \n");
        free(option_field_1);
        free(option_field_2);
        return 0;

    }
    else
    {
        printf("\n DHCPCLIENT_STATE_REBINDING failed : The field received (packet_type) isn't a DHCPREQUEST \n");
        free(option_field_1);
        free(option_field_2);
        return 1;
    }
}