

#ifndef UDP_TEST_SPLIT_H
#define UDP_TEST_SPLIT_H

/***************************************************************************************************
*
*   FUNCTION NAME: ExtractIPAddress
*
***************************************************************************************************/
/**
* @brief
*    A function that divided an IP to 4 parts
*
 *   @par Parameters
*    @param[in] IPaddr: Type uint8 Contains the DUT/TESTER address
*    @param[in] ipAddress: Type ipaddr
* @return   ipAddress
 *
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/

void extractIpAddress(uint8 *IPaddr,ip4addr *ipAddress);

#endif //UDP_TEST_SPLIT_H
