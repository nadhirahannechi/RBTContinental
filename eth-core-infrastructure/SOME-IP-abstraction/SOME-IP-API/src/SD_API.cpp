/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file     SD_API.cpp
 *  \brief    Implementation of SOME/IP service discovery API
 *  \author   Wided FATHALLAH
 *
 *  \addtogroup SW_package
 *  \{
 */

/******************************************************************************
* INCLUDE FILES
******************************************************************************/
#include "SD_API.hpp"
#include "configuration.hpp"
/******************************************************************************
* DEFINES AND MACROS
******************************************************************************/

/******************************************************************************
* GLOBAL VARIABLES AND CONSTANTS
******************************************************************************/

/******************************************************************************
*  FUNCTIONS
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
SD_Listen_Return ListenOffer(double __timeOfListen, vsomeip::service_t __servieID, vsomeip::sd::option_type_e __optType,
                             int __NumberOfIPV4Endpoint, int __NumberOfExpectedOfferServiceEntries)
{
    bool waiting = true;
    bool found = false;
    int sockfd = connectUdp(SOME_IP_MULTICAST_IP_ADDR, UDP_PORT);
    // Test if the socket is in non-blocking mode:
    if (fcntl(sockfd, F_GETFL) & O_NONBLOCK)
    {
        // socket is non-blocking
    }

    // Put the socket in non-blocking mode:
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK) < 0)
    {
        // handle error
    }
    int n = -1;
    uint8_t __buf[4096];
    vsomeip::sd::message_impl sd_msg;
    static struct SD_Listen_Return ret_Listen;
    auto start = std::chrono::steady_clock::now();
    while (waiting)
    {
        found = false;
        n = recvUdp(sockfd, __buf, 4096);
        if (n > 0)
        {
            vsomeip::deserializer its_deserializer(__buf, n, 0); //
            vsomeip::service_t its_service = VSOMEIP_BYTES_TO_WORD(__buf[VSOMEIP_SERVICE_POS_MIN], __buf[VSOMEIP_SERVICE_POS_MAX]);
            vsomeip::method_t its_method = VSOMEIP_BYTES_TO_WORD(__buf[VSOMEIP_METHOD_POS_MIN], __buf[VSOMEIP_METHOD_POS_MAX]);
            if (its_service == vsomeip::sd::service && its_method == vsomeip::sd::method)
            { //sd
                found = true;
                if (sd_msg.deserialize(&its_deserializer) == true)
                {
                    for (auto e : sd_msg.get_entries())
                    {
                        if (found == false)
                        {
                            found = true;
                            if (e->is_service_entry() && e->get_type() == vsomeip::sd::entry_type_e::OFFER_SERVICE && e->get_service() == __servieID)
                            {
                                if ((uint8_t)__optType != 0x00)
                                {
                                    if ((int)__optType == (int)(sd_msg.get_options()[0]->get_type()))
                                    {
                                        found &= true;
                                    }
                                    else
                                    {
                                        found &= false;
                                    }
                                }
                            }
                            else
                            {
                                found &= false;
                            }
                        }
                    }
                    if (__NumberOfIPV4Endpoint != 0)
                    {
                        int nb_ipv4 = 0;
                        int lim = sd_msg.get_options().size();
                        for (int i = 0; i < lim; i++)
                        {
                            if (sd_msg.get_options()[0]->get_type() == vsomeip::sd::option_type_e::IP4_ENDPOINT)
                            {
                                nb_ipv4++;
                            }
                        }
                        if (__NumberOfIPV4Endpoint == nb_ipv4)
                        {
                            found &= true;
                        }
                        else
                        {
                            found &= false;
                        }
                    }
                    if (__NumberOfExpectedOfferServiceEntries != 0)
                    {
                        if (sd_msg.get_entries().size() == __NumberOfExpectedOfferServiceEntries)
                        {
                            found &= true;
                        }
                        else
                        {
                            found &= false;
                        }
                    }
                }
            }
            if (found)
            {
                ret_Listen.SD_Result = Receive_E_OK;
                ret_Listen.SD_Received_Message = &sd_msg;
                closeUdp(sockfd);
                waiting = false;
                return ret_Listen;
            }
        }
        auto end = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() > __timeOfListen)
        {
            waiting = false;
            ret_Listen.SD_Result = Receive_Timout;
            closeUdp(sockfd);
            return ret_Listen;
        }
    }
}

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
SD_Listen_Return *Listen_2_Offer(double __timeOfListen, vsomeip::service_t __servieID, vsomeip::sd::option_type_e __optType,
                                 int __NumberOfIPV4Endpoint, int __NumberOfExpectedOfferServiceEntries)
{
    bool waiting = true;
    bool found = false;
    int nb_msg = 0;
    int sockfd = connectUdp(SOME_IP_MULTICAST_IP_ADDR, UDP_PORT);
    // Test if the socket is in non-blocking mode:
    if (fcntl(sockfd, F_GETFL) & O_NONBLOCK)
    {
        // socket is non-blocking
    }

    // Put the socket in non-blocking mode:
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK) < 0)
    {
        // handle error
    }
    int n = -1;
    uint8_t __buf[4096];
    vsomeip::sd::message_impl sd_msg;
    static struct SD_Listen_Return ret_Listen[2];
    auto start = std::chrono::steady_clock::now();
    while (waiting)
    {
        found = false;
        n = recvUdp(sockfd, __buf, 4096);
        if (n > 0)
        {
            time_stamp ts = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
            vsomeip::deserializer its_deserializer(__buf, n, 0); //
            vsomeip::service_t its_service = VSOMEIP_BYTES_TO_WORD(__buf[VSOMEIP_SERVICE_POS_MIN], __buf[VSOMEIP_SERVICE_POS_MAX]);
            vsomeip::method_t its_method = VSOMEIP_BYTES_TO_WORD(__buf[VSOMEIP_METHOD_POS_MIN], __buf[VSOMEIP_METHOD_POS_MAX]);
            if (its_service == vsomeip::sd::service && its_method == vsomeip::sd::method)
            { //sd
                found = true;
                if (sd_msg.deserialize(&its_deserializer) == true)
                {
                    for (auto e : sd_msg.get_entries())
                    {
                        if (found == false)
                        {
                            found = true;
                            if (e->is_service_entry() && e->get_type() == vsomeip::sd::entry_type_e::OFFER_SERVICE && e->get_service() == __servieID)
                            {
                                if ((uint8_t)__optType != 0x00)
                                {
                                    if ((int)__optType == (int)(sd_msg.get_options()[0]->get_type()))
                                    {
                                        found &= true;
                                    }
                                    else
                                    {
                                        found &= false;
                                    }
                                }
                            }
                            else
                            {
                                found &= false;
                            }
                        }
                    }
                    if (__NumberOfIPV4Endpoint != 0)
                    {
                        int nb_ipv4 = 0;
                        int lim = sd_msg.get_options().size();
                        for (int i = 0; i < lim; i++)
                        {
                            if (sd_msg.get_options()[0]->get_type() == vsomeip::sd::option_type_e::IP4_ENDPOINT)
                            {
                                nb_ipv4++;
                            }
                        }
                        if (__NumberOfIPV4Endpoint == nb_ipv4)
                        {
                            found &= true;
                        }
                        else
                        {
                            found &= false;
                        }
                    }
                    if (__NumberOfExpectedOfferServiceEntries != 0)
                    {
                        if (sd_msg.get_entries().size() == __NumberOfExpectedOfferServiceEntries)
                        {
                            found &= true;
                        }
                        else
                        {
                            found &= false;
                        }
                    }
                }
            }
            if (found)
            {
                ret_Listen[nb_msg].SD_Result = Receive_E_OK;
                ret_Listen[nb_msg].SD_Received_Message = &sd_msg;
                ret_Listen[nb_msg].timestamp = ts;
                nb_msg++;
                if (nb_msg >= 2)
                {
                    closeUdp(sockfd);
                    waiting = false;
                    return ret_Listen;
                }
            }
        }
        auto end = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() > __timeOfListen)
        {
            waiting = false;
            ret_Listen[nb_msg].SD_Result = Receive_Timout;
            nb_msg++;
            closeUdp(sockfd);
            if (nb_msg < 2)
            {
                ret_Listen[nb_msg + 1].SD_Result = Receive_Timout;
            }
            return ret_Listen;
        }
    }
}

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
SD_Listen_Return ListenSubscribeAck(double __timeOfListen, vsomeip::service_t __servieID, vsomeip::eventgroup_t __eventGroupID,
                                    vsomeip::sd::option_type_e __optType, vsomeip::instance_t __instanceID)
{
    bool waiting = true;
    bool found = false;
    uint8_t __buf[4096];
    static struct SD_Listen_Return ret_Listen;
    int sockfd = connectUdp(SOME_IP_MULTICAST_IP_ADDR, UDP_PORT);
    // Test if the socket is in non-blocking mode:
    if (fcntl(sockfd, F_GETFL) & O_NONBLOCK)
    {
        // socket is non-blocking
    }
    // Put the socket in non-blocking mode:
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK) < 0)
    {
        // handle error
    }
    auto start = std::chrono::steady_clock::now();
    while (waiting)
    {
        found = false;
        int n = recvUdp(sockfd, __buf, 4096);
        if (n > 0)
        {
            vsomeip::sd::message_impl sd_msg;
            vsomeip::deserializer its_deserializer(__buf, n, 0); //
            vsomeip::service_t its_service = VSOMEIP_BYTES_TO_WORD(__buf[VSOMEIP_SERVICE_POS_MIN], __buf[VSOMEIP_SERVICE_POS_MAX]);
            vsomeip::method_t its_method = VSOMEIP_BYTES_TO_WORD(__buf[VSOMEIP_METHOD_POS_MIN], __buf[VSOMEIP_METHOD_POS_MAX]);
            if (its_service == vsomeip::sd::service && its_method == vsomeip::sd::method)
            { //sd
                found = true;
                if (sd_msg.deserialize(&its_deserializer) == true)
                {
                    for (auto e : sd_msg.get_entries())
                    {
                        if (found == false)
                        {
                            found = true;
                            if (e->is_eventgroup_entry() && e->get_type() == vsomeip::sd::entry_type_e::SUBSCRIBE_EVENTGROUP_ACK && e->get_service() == __servieID)
                            {
                                std::shared_ptr<vsomeip::sd::eventgroupentry_impl> casted_entry =
                                    std::static_pointer_cast<vsomeip::sd::eventgroupentry_impl>(e);
                                if (casted_entry->get_eventgroup() == __eventGroupID)
                                {
                                    if ((uint8_t)__optType != 0x00)
                                    {
                                        if ((int)__optType == (int)(sd_msg.get_options()[0]->get_type()))
                                        {
                                            found &= true;
                                        }
                                        else
                                        {
                                            found &= false;
                                        }
                                    }
                                    if (__instanceID != 0)
                                    {
                                        if (casted_entry->get_instance() == __instanceID)
                                        {
                                            found &= true;
                                        }
                                        else
                                        {
                                            found &= false;
                                        }
                                    }
                                }
                                else
                                {
                                    found &= false;
                                }
                            }
                            else
                            {
                                found = false;
                            }
                        }
                    }
                }
                if (found)
                {
                    ret_Listen.SD_Result = Receive_E_OK;
                    ret_Listen.SD_Received_Message = &sd_msg;
                    closeUdp(sockfd);
                    waiting = false;
                    return ret_Listen;
                }
            }
            auto end = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() > __timeOfListen)
            {
                waiting = false;
                ret_Listen.SD_Result = Receive_Timout;
                closeUdp(sockfd);
                return ret_Listen;
            }
        }
    }
}
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
vsomeip::sd::entry_impl *get_first_entry(vsomeip::sd::message_impl *msg)
{
    for (int i = 0; i < msg->get_entries().size(); i++)
    {
        vsomeip::sd::entry_impl *e = msg->get_entries()[i].get();
        return e;
    }
}

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
vsomeip::sd::eventgroupentry_impl get_first_eventGroup_entry(vsomeip::sd::message_impl *msg)
{
    //for (auto e : msg->get_entries()) {
    for (int i = 0; i < msg->get_entries().size(); i++)
    {
        std::shared_ptr<vsomeip::sd::entry_impl> e = msg->get_entries()[i];
        std::shared_ptr<vsomeip::sd::eventgroupentry_impl> casted_entry =
            std::static_pointer_cast<vsomeip::sd::eventgroupentry_impl>(e);
        vsomeip::sd::eventgroupentry_impl *e2 = casted_entry.get();
        return *e2;
    }
}
vsomeip::sd::option_impl *get_ipv4_option(vsomeip::sd::message_impl *msg)
{
    for (auto o : msg->get_options())
    {
        if (o->get_type() == vsomeip::sd::option_type_e::IP4_ENDPOINT)
        {
            return o.get();
        }
    }
}

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
int get_number_of_ipv4_option(vsomeip::sd::message_impl *msg)
{
    int nbr = 0;
    for (auto o : msg->get_options())
    {
        if (o->get_type() == vsomeip::sd::option_type_e::IP4_ENDPOINT)
        {
            nbr++;
        }
        std ::cout << "STOP " << nbr << "\n";
    }
    return nbr;
}

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
int get_port_of_ipv4_option(vsomeip::sd::message_impl *msg)
{
    std::shared_ptr<vsomeip::sd::ip_option_impl> ip_opt;
    for (auto o : msg->get_options())
    {
        if (o->get_type() == vsomeip::sd::option_type_e::IP4_ENDPOINT)
        {
            ip_opt = std::static_pointer_cast<vsomeip::sd::ip_option_impl>(o);
            break;
        }
    }
    return ip_opt->get_port();
}