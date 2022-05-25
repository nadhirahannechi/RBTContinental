#ifndef PADDING_H
#define PADDING_H
/*********************************************************************************************************************************
*
*   FUNCTION NAME: UDP_Padding_02
 *   *
**********************************************************************************************************************************/
/**
* @brief
*   Ensure that when the DUT is requested to generate a UDP packet with an even payload size then the DUT generates a UDP packet
    containing Data indicating value of the received even <UDPConfiguration.udpUserDataSize> with no padding bytes
*
* @return int : 0: DUT: DUT generates a UDP packet containing Data indicating value <UDPConfiguration.udpUserDataSize> with no padding bytes
 *        int : 1: DUT: DUT generates a UPD packet with padding bytes
*
* @note
* -
*
* @warning
* -
*
***********************************************************************************************************************************/
int UDP_Padding_02();

#endif //PADDING_H