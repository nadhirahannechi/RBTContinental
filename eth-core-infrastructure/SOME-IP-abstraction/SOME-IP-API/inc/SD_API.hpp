/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   SD_API.hpp
 *  @brief  SOME/IP Service Discovery interface
 *  \author Wided FATHALLAH
 *
 *  \addtogroup SW_package
 *  \{
 */
#ifndef SD_API_H_
#define SD_API_H_

/******************************************************************************
* INCLUDE FILES
******************************************************************************/
#include <chrono>
#include <vsomeip/vsomeip.hpp>
#include "udp.hpp"
#include "/home/focus/SOME-IP/build-common-api-cpp-native/vsomeip/implementation/utility/include/byteorder.hpp"
#include "/home/focus/SOME-IP/build-common-api-cpp-native/vsomeip/implementation/message/include/deserializer.hpp"
#include "/home/focus/SOME-IP/build-common-api-cpp-native/vsomeip/implementation/service_discovery/include/message_impl.hpp"
#include "/home/focus/SOME-IP/build-common-api-cpp-native/vsomeip/implementation/service_discovery/include/service_discovery.hpp"
#include "/home/focus/SOME-IP/build-common-api-cpp-native/vsomeip/implementation/service_discovery/include/constants.hpp"
#include "/home/focus/SOME-IP/build-common-api-cpp-native/vsomeip/implementation/service_discovery/include/enumeration_types.hpp"
#include "/home/focus/SOME-IP/build-common-api-cpp-native/vsomeip/implementation/service_discovery/include/eventgroupentry_impl.hpp"
#include "/home/focus/SOME-IP/build-common-api-cpp-native/vsomeip/implementation/message/include/message_impl.hpp"
#include "/home/focus/SOME-IP/build-common-api-cpp-native/vsomeip/implementation/service_discovery/include/option_impl.hpp"
#include "/home/focus/SOME-IP/build-common-api-cpp-native/vsomeip/implementation/service_discovery/include/ip_option_impl.hpp"

/******************************************************************************
* MACROS
******************************************************************************/


/******************************************************************************
* DEFINES
******************************************************************************/

/*For a time stamp in microseconds*/
using time_stamp = std::chrono::time_point<std::chrono::system_clock,
                                           std::chrono::microseconds>;

/** \brief   The sniff return code */

typedef enum
{
    Receive_E_OK      = 0x00U,      /* E_OK */
    Receive_Timout    = 0xFFU,      /* E_NOT_OK  due to  timeout */
} SD_Result_Type;

/** \brief  struct  for the sniff fonction return */

typedef struct SD_Listen_Return {
    SD_Result_Type SD_Result;                           /*the return code of the sniff*/
    vsomeip::sd::message_impl *SD_Received_Message;     /*the received service discovery  msg (if received)*/
    time_stamp timestamp;
};


/******************************************************************************
* GLOBAL VARIABLES AND CONSTANTS
******************************************************************************/


/******************************************************************************
* GLOBAL FUNCTIONS PROTOTYPES
******************************************************************************/


/***************************************************************************************************
*
*   FUNCTION NAME: ListenOffer
*
***************************************************************************************************/
/**
* @brief
*  This fuction will  sniff the traffic and waits to capture the Offer Service msg containig the [in] parameters as fields
*  if  __timeOfListen seconds passed without receiving  the expected msg  a timeout error code will be returned
*
* @par Parameters
* @param[in]    __timeOfListen                              time to  wait before returning timeout error code (in seconds)
* @param[in]    __servieID                                  the service id to  listen for
* @param[in]    __optType                                   the option type to  listen for
* @param[in]    __NumberOfIPV4Endpoint                      the number of expected IPV4 Endpoints
* @param[in]    __NumberOfExpectedOfferServiceEntries       The number of expected Offer Service entries                       
*
* @return SD_Listen_Return :  Contains the msg  received and the return code
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
SD_Listen_Return ListenOffer(double __timeOfListen,  vsomeip::service_t __servieID ,vsomeip::sd::option_type_e __optType=(vsomeip::sd::option_type_e)0x0,
int __NumberOfIPV4Endpoint=0 ,int __NumberOfExpectedOfferServiceEntries=0);


/***************************************************************************************************
*
*   FUNCTION NAME: Listen_2_Offer
*
***************************************************************************************************/
/**
* @brief
*  This fuction will  sniff the traffic and waits to capture 2 packets of Offer Service msg containig the [in] parameters as fields
*  if  __timeOfListen seconds passed without receiving  the expected msg  a timeout error code will be returned
*
* @par Parameters
* @param[in]    __timeOfListen                              time to  wait before returning timeout error code (in seconds)
* @param[in]    __servieID                                  the service id to  listen for
* @param[in]    __optType                                   the option type to  listen for
* @param[in]    __NumberOfIPV4Endpoint                      the number of expected IPV4 Endpoints
* @param[in]    __NumberOfExpectedOfferServiceEntries       The number of expected Offer Service entries                       
*
* @return SD_Listen_Return* :  a pointer on the 2 messages received and their return code
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/

SD_Listen_Return* Listen_2_Offer(double __timeOfListen, vsomeip::service_t __servieID, vsomeip::sd::option_type_e __optType=(vsomeip::sd::option_type_e)0x0,
                             int __NumberOfIPV4Endpoint=0, int __NumberOfExpectedOfferServiceEntries=0);

/***************************************************************************************************
*
*   FUNCTION NAME: ListenSubscribeAck
*
***************************************************************************************************/
/**
* @brief
*  This fuction will  sniff the traffic and waits to capture the Subscribe Ack msg containig the [in] parameters as fields
*  if  __timeOfListen seconds passed without receiving  the expected msg  a timeout error code will be returned
*
* @par Parameters
* @param[in]    __timeOfListen                              time to  wait before returning timeout error code (in seconds)
* @param[in]    __servieID                                  the service id to  listen for
* @param[in]    __optType                                   the option type to  listen for
* @param[in]    __instanceID                                the instance id to  listen for
* @param[in]    __eventGroupID                              the event group id to  listen for                     
*
* @return SD_Listen_Return :  Contains the msg  received and the return code
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/

SD_Listen_Return ListenSubscribeAck(double __timeOfListen ,vsomeip::service_t __servieID ,vsomeip::eventgroup_t __eventGroupID,
     vsomeip::sd::option_type_e __optType= (vsomeip::sd::option_type_e)0x0,vsomeip::instance_t __instanceID = 0x00);

/***************************************************************************************************
*
*   FUNCTION NAME: get_first_entry
*
***************************************************************************************************/
/**
* @brief
*  This fuction will  extract the first entry from the entry array and return it
*
* @par Parameters
* @param[in]    msg                                         the msg to  extract the first entry from
*
* @return vsomeip::sd::entry_impl* 
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
vsomeip::sd::entry_impl* get_first_entry(vsomeip::sd::message_impl * msg);

/***************************************************************************************************
*
*   FUNCTION NAME: get_first_eventGroup_entry
*
***************************************************************************************************/
/**
* @brief
*  This fuction will  extract the first event group entry from the entry array and return it
*
* @par Parameters
* @param[in]    msg                                         the msg to  extract the first entry from
*
* @return vsomeip::sd::eventgroupentry_impl
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
vsomeip::sd::eventgroupentry_impl get_first_eventGroup_entry(vsomeip::sd::message_impl *msg );

vsomeip::sd::option_impl* get_ipv4_option (vsomeip::sd::message_impl *msg );

/***************************************************************************************************
*
*   FUNCTION NAME: get_number_of_ipv4_option
*
***************************************************************************************************/
/**
* @brief
*  This fuction will calculate the number of ipv4 endpoint options and return it
*
* @par Parameters
* @param[in]    msg                                         the msg to  extract the first entry from
*
* @return int                                                  the calculated number of ipv4 endpoint options
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
int get_number_of_ipv4_option (vsomeip::sd::message_impl *msg );



/***************************************************************************************************
*
*   FUNCTION NAME: get_port_of_ipv4_option
*
***************************************************************************************************/
/**
* @brief
*  This fuction will return the port number of ipv4 endpoint option
*
* @par Parameters
* @param[in]    msg                                         the msg to  extract the port from
*
* @return int                                                  the port number of ipv4 endpoint option
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/

int get_port_of_ipv4_option (vsomeip::sd::message_impl *msg );

#endif /* SD_API_H_ */
/** \}    end of addtogroup */
