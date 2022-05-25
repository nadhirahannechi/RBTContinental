/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file     SOMEIPSRV_BASIC_client.cpp
 *  \brief    Implementation of SOMEIPSRV_BASIC test cases 
 *  \author   Wided FATHALLAH
 *
 *  \addtogroup SW_package
 *  \{
 */

/******************************************************************************
* INCLUDE FILES
******************************************************************************/

#include <iomanip>
#include <iostream>
#include <sstream>
#include <condition_variable>
#include <thread>
#include <unistd.h>
#include <vsomeip/vsomeip.hpp>
//sniff offer service
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h> //malloc
#include "configuration.hpp"
#include "SD_API.hpp"
#include "SOMEIPSRV_BASIC.hpp"

/******************************************************************************
* DEFINES AND MACROS
******************************************************************************/

/******************************************************************************
* GLOBAL VARIABLES AND CONSTANTS
******************************************************************************/

std::shared_ptr<vsomeip::application> app;
std::mutex mutex;
std::condition_variable condition;
std::unique_lock<std::mutex> its_lock(mutex);
int test = 0;
bool test_ok = false;
int test_number;
bool msg_received = false;
auto start = std::chrono::steady_clock::now();

/******************************************************************************
* STATIC FUNCTION PROTOTYPES
******************************************************************************/

/******************************************************************************
* GLOBAL FUNCTIONS
******************************************************************************/

/******************************************************************************
* STATIC FUNCTIONS
******************************************************************************/


void check_offer_service_validation_02(SD_Listen_Return sd_return);

void run_01()
{
    condition.wait(its_lock);
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_service(SERVICE_ID_1);
    request->set_instance(SERVICE_ID_1_INSTANCE_ID);
    request->set_method(METHOD_ID_1_SI_1);
    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    std::string s("Request :D");
    for (int i = 0; i < s.size(); i++)
    {
        its_payload_data.push_back(s[i]);
    }
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);
    test++;
    app->send(request, true);
    start = std::chrono::steady_clock::now();
    while (msg_received == false)
    {
        auto end = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() > ParamListenTime)
        {
            break;
        }
    }

    if (msg_received == false)
    {
        std::cout << "\nPart 2 of test is NOT Ok: Timout without receiving any message fron DUT\n\n";
        test_ok = test_ok && false;
    }
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}
void run_02()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_01(const std::shared_ptr<vsomeip::message> &_response)
{
    if (test == 1)
    {
        if (_response->get_service() == SERVICE_ID_1 && _response->get_method() == METHOD_ID_1_SI_1 && _response->get_return_code() == vsomeip::return_code_e::E_OK)
        {
            msg_received = true;
            std::cout << "\nPart 2 of test is Ok : Response received\n";
        }
    }
}

void on_message_02(const std::shared_ptr<vsomeip::message> &_response)
{
}

void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available)
{
    std::cout << "CLIENT: Service ["
              << std::setw(4) << std::setfill('0') << std::hex << _service << "." << _instance
              << "] is "
              << (_is_available ? "available." : "NOT available.")
              << std::endl;
    if (_is_available)
        condition.notify_one();
}

int main(int argc, char *argv[])
{
    test_number = atoi(argv[1]);
    switch (test_number)
    {
    case 1:
        SOMEIPSRV_BASIC_01();
        break;
    case 2:
        SOMEIPSRV_BASIC_02();
        break;
    case 3:
        SOMEIPSRV_BASIC_03();
        break;
    default:
        printf("\n\t TEST NUMBER NOT FOUND \n");
        break;
    }
}

int SOMEIPSRV_BASIC_01()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_01);
    //app->register_message_handler(SD_SERVICE_ID,vsomeip::ANY_INSTANCE,NOTIFICATION_METHOD_ID, on_offer_service);
    //app->get_offered_services_async(vsomeip::offer_type_e::OT_REMOTE,on_offer_service);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) \n"
                  << std::endl;
        test_ok = true;
    }
    else
    {
        std::cout << "\nPart 1 of test is NOT Ok: Didn't receive any Notification (OFFER SERVICE) \n"
                  << std::endl;
        test_ok &= false;
    }
    std::thread sender(run_01);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_BASIC_01 : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_BASIC_01 : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_BASIC_02()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, 0xFFFF);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_02);
    //app->register_message_handler(SD_SERVICE_ID,vsomeip::ANY_INSTANCE,NOTIFICATION_METHOD_ID, on_offer_service);
    //app->get_offered_services_async(vsomeip::offer_type_e::OT_REMOTE,on_offer_service);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1); //ToDO
    check_offer_service_validation_02(sd_return);
    std::thread sender(run_02);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_BASIC_02 : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_BASIC_02 : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_BASIC_03()
{
    printf("NOT IMPLEMENTED YET ");
    return 2;
}
void check_offer_service_validation_02(SD_Listen_Return sd_return)
{
    if (sd_return.SD_Result == Receive_E_OK)
    {
        vsomeip::sd::entry_impl *e;
        e = get_first_entry(sd_return.SD_Received_Message);
        if (e->get_instance() != 0 && e->get_instance() != 0xffff)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE)  with correct instance ID \n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT OK: Notification received (OFFER SERVICE)  with wrong instance ID" << e->get_instance() << " \n"
                      << std::endl;
        }
    }
}

/** \}    end of addtogroup */