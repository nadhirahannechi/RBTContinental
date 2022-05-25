#ifndef COMMONS_FUNCTIONS_H
#define COMMONS_FUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "TestabilityProtocolTypes.h"
#include "TestabilityProtocol_api.h"
#include "AbstractionAPI.h"

/***************************************************************************************************
*
*   FUNCTION NAME: extractIpAddress
*
***************************************************************************************************/
/**
* @brief
*  Extract the IP Address
*
* @par
* @param[in] IPaddr : Type uint8 : Input address
* @param[in] ipAddress : Type ip4addr : Address to initialize
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
void extractIpAddress(uint8 *IPaddr, ip4addr *ipAddress);


#endif