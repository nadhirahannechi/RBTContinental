#ifndef MESSAGEFORMAT_H
#define MESSAGEFORMAT_H

/****************************************************************************************************************************************************************************
*
*   FUNCTION NAME: UDP_MessageFormat_02
 *   *
***********************************************************************************************************************************************************************/
/**
* @brief
*   Ensure that when the DUT receives a UDP packet containing a well-formed Header containing a Source Port field
    containing a Destination Port indicating a value equal to the DUT’s UDP port containing a Length field
    indicating a valid value equal to the size of the sent datagram containing a Checksum field indicating a value equal to the <calculatedUDPChecksum> by the DUT
    then the DUT accepts the UDP packet.
*
* @return int : 0: DUT: DUT accept the UDP Packet| 1: DUT: DUT discards the UDP Packet
*
* @note
* -
*
* @warning
* -
*
******************************************************************************************************************************************************************************/
int UDP_MessageFormat_02();


#endif //MESSAGEFORMAT_H