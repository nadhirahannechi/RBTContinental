#ifndef DHCP_STATE_MACHINE_H
#define DHCP_STATE_MACHINE_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "TestabilityProtocolTypes.h"
#include "TestabilityProtocol_api.h"
#include "AbstractionAPI.h"
#include <math.h>


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
int DHCPCLIENT_STATE_REQUESTING();



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
int DHCPCLIENT_STATE_BOUND();


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
int DHCPCLIENT_STATE_RENEWING();



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
int DHCPCLIENT_STATE_REBINDING();

#endif






