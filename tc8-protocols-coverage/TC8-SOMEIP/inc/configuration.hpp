/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   configuration.hpp
 *  @brief  
 *  \author Wided FATHALLAH
 *
 *  \addtogroup SW_package
 *  \{
 */
#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_
/*==================================================================================================
*                                        Include Files
==================================================================================================*/

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
#define CLIENT_ID_1 0x1313
#define SESSION_ID_1  0x0001
#define SESSION_ID_2  0x0002


//#define SD_SERVICE_ID 0xFFFF
#define NOTIFICATION_METHOD_ID 0x8100
//#define UNKNOWN_METHOD_ID_SI_1 0x0001 // not defined id  
#define UDP_PORT 30490
//#define MULTICAST_ADDRESS "224.224.224.245"
// need to  be defined correctly  followig the tc8  configuration  
//<DIface-n> n-th Interface of DUT
#define ParamListenTime 10 /*time to  listen  on <DIface-0> in seconds */

#define SERVICE_ID_1 0x1234 /*Service ID of Service 1*/
#define SERVICE_ID_2 0x1243 /*Service ID of Service 2*/
#define SERVICE_ID_1_INSTANCE_ID 0x5678 /*Instance ID of Service 1*/
#define SERVICE_ID_1_MAJ_VER 0XFF /*Major version supported by Service 1*/
#define SERVICE_ID_1_INTF_VER_MAJ 0x00 /*Interface Version of Service 1*/
#define SERVICE_ID_2_INTF_VER_MAJ /*Interface Version of Service 2*/
#define SERVICE_ID_1_TTL 3/*TTL value of Service 1.*/
#define SERVICE_ID_1_MINOR_VER 0xFFFFFF/*Minor version supported by Service 1*/
#define SERVICE_ID_1_INST_1 /*First Instance ID of Service 1.*/
#define EVENT_GROUP_ID_2 /*Eventgroup ID 2.*/
#define EVENT_GROUP_ID_1_SI_1 0x4455 /*Eventgroup ID 1 of Service 1.*/
#define SERVICE_ID_1_UDP_PORT /*UDP port number where Service 1 runs.*/
#define SOME_IP_MULTICAST_IP_ADDR "224.224.224.245" /*Multicast Address used by SOME/IP-SD.*/
#define CLIENT1_CURR_REQUEST_ID 0x13130001/*Current Request ID of Client 1 (client_id_1 / session_id_1).*/
#define EVENT_ID_1_EG_ID_1 0x7788 /*Field Event ID 1 of Eventgroup ID 1 that will be sent out initially at initial subscription*/
#define EVENT_ID_1_EG_ID_2 /*Field Event ID 1 of Eventgroup ID 2 that will be sent out initially at initial subscription*/
#define EVENT_ID_2_EG_ID_1 /*Event ID 2 of Eventgroup ID 1.*/
#define EVENT_ID_2_EG_ID_2 /*Event ID 2 of Eventgroup ID 2*/
#define EVENT_ID_2_EG_ID_1_CYCLIC_TIME /*Cycle Time of Event ID-2 of Eventgroup 1.(Here Event ID 1 is assumed to be a cyclic event)*/
#define EVENT_GROUP_ID_1_SI_1_MulticastAddr /*Muticast Address of Eventgroup 1 of Service 1 where the event and notifications will be multicast.*/
#define UNKNOWN_SERVICE_ID 0x4321/* Invalid Service ID.*/
#define UNKNOWN_EVENT_GROUP_ID /*Invalid Eventgroup ID. */
#define UNKNOWN_METHOD_ID_SI_1 0x0422 /*Invalid Method ID of Service ID 1.*/
#define UNKNOWN_INTF_VER_MAJ_SI_1 0x01 /*Invalid Interface Version of Service ID 1.*/
#define SERVICE_ID_1_REP_BASE_INTV 400 /*Repetition Base Interval of Service 1 (value in milliseconds).*/
#define SERVICE_ID_1_TOTAL_REP_INTV  200 * ((2^3)-1 )
/*Total Repetition Period of Service 1. Calculated using
Repetition max value as - (Repetition_base_intv) * ((2 ^
repetiton_max) - 1)*/
#define SERVICE_ID_1_INITIAL_WAIT_TIME /*Initial Wait time for Service 1.*/
#define SERVICE_ID_1_CYCLE_INTV /*Offer Cycle interval of Service 1.*/
#define METHOD_ID_1_SI_1 0x0421 /*First InOut Type Method Listed in XML file for Service 1.*/
#define METHOD_ID_1_SI_2 /*First InOut Type Method Listed in XML file for Service 2.*/
#define METHOD_ID_GET_SI_1 0x0001
#define METHOD_ID_SET_SI_1 0x0002
#define SERVICE_ID_2_TRANSPORT_PORT /*Transport port of Service ID 2.(UDP/TCP)*/
#define SERVICE_ID_2_INITIAL_WAIT_TIME /*Time taken by the DUT before Sending Offer Service message.*/
#define SERVER1_IP_ADDR /*IP Address of SOME/IP-SD Server 1.*/
#define  CLIENT_IP_ADDR /*ANVL client1 IP address.*/
#define  CLIENT1_UDP_PORT /*ANVL client1 UDP port.*/
#define  FOREGROUND false /*Boolean value set to false to indicate script running in foreground.*/
#define BACKGROUND true /*Boolean value set to true to indicate script running in background.*/
#define ParamToleranceTimeMillisec 1000 /*This is tolerance time taken from parameter file (value in milliseconds).*/
#define ParamToleranceTime 1 /*This is tolerance time taken from parameter file (value in seconds).*/
#define  CLIENT1_LISTEN_TIME /*Listen time used by ANVL taken from parameter file.*/
#define BUSY_WAIT
/*It is to indicate ANVL will be listening to incoming
packets while waiting.*/
#define  LAZY_WAIT
/*It is to indicate ANVL will not be listening to incoming
packets while waiting.
ListenTime
This is the maximum time interval for which anvl waits
for a packet for cases when a certain event has been
triggered on the DUT either by some protocol timer or
using some external mechanism (script).
[Default: 10 second]
ToleranceTime
Tolerance time associated with an event. When waiting
or listening then this number will be added with the
actual wait-time or listen-time.
[Default: 1 second] 
*/

/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/*==================================================================================================
*                                        Global Variables
==================================================================================================*/

/*==================================================================================================
*                                     local Functions prototypes
==================================================================================================*/

/*==================================================================================================
*                                     Global Functions prototypes
==================================================================================================*/

#endif 
/** \}    end of addtogroup */
