#ifndef IPV4REASSEMBLY_H
#define IPV4REASSEMBLY_H

#ifdef __cplusplus
extern "C"
{
#endif  // #ifdef __cplusplus

/**************************************************************************************************/
/*                                                                                                */
/*                                           Defines                                              */
/*                                                                                                */
/**************************************************************************************************/

#define PARAMETER_TOLERANCE_TIME 5 

/**************************************************************************************************/
/*                                                                                                */
/*                                         Prototypes                                             */
/*                                                                                                */
/**************************************************************************************************/

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_REASSEMBLY_04
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives a series of unordered IPv4 Fragments of a large IPv4 packet 
*  then the DUT reassembles and accept the IPv4 packet.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_REASSEMBLY_04();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_REASSEMBLY_06
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives a series of IPv4 Fragments of a large IPv4 packet containing an
*  IPv4 Header containing an Offset indicating a value different than 0 but with correct increment
*  then the DUT does not reassemble and accept the IPv4 packet.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_REASSEMBLY_06();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_REASSEMBLY_07
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives a series of IPv4 Fragments of a large IPv4 packet including the
*  first and the last fragment but missing some in between then the DUT does not reassemble and 
*  accept the IPv4 packet.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_REASSEMBLY_07();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_REASSEMBLY_09
*
***************************************************************************************************/
/**
* @brief
*  Ensure that when the DUT receives an IPv4 packet containing an IPv4 Header containing Flags and
*  containing a MF flag indicating that there are more fragments coming: MF = 1, then the DUT 
*  discards the IPv4 Packet silently.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_REASSEMBLY_09();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_REASSEMBLY_10
*
***************************************************************************************************/
/**
* @brief
*  The current recommendation for the initial timer setting is 15 seconds.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_REASSEMBLY_10();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_REASSEMBLY_11
*
***************************************************************************************************/
/**
* @brief
*  The initial setting of the timer is a lower bound on the reassembly waiting time. This is because
*  the waiting time will be increased if the Time to Live in the arriving fragment is greater than
*  the current timer value.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_REASSEMBLY_11();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_REASSEMBLY_12
*
***************************************************************************************************/
/**
* @brief
*  The initial setting of the timer is a lower bound on the reassembly waiting time. The waiting
*  time will not be decreased if it is less the Time to Live in the arriving fragment.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_REASSEMBLY_12();

/***************************************************************************************************
*
*   FUNCTION NAME: IPv4_REASSEMBLY_13
*
***************************************************************************************************/
/**
* @brief
*  In the case that two or more fragments contain the same data either identically or through a 
*  partial overlap, this procedure will use the more recently arrived copy in the data buffer and 
*  datagram delivered.
*
* @return int 0: The test  was successful | 1: The test failed 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int IPv4_REASSEMBLY_13();

#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus
#endif