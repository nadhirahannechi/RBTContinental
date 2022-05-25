/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file     SOMEIPSRV_SD_BEHAVIOR_client.cpp
 *  \brief    Implementation of SOMEIPSRV_SD_BEHAVIOR test cases
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
#include "SOMEIPSRV_SD_BEHAVIOR.hpp"

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

void run_01()
{
    //condition.wait(its_lock);
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

void run_03()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void run_04()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_01(const std::shared_ptr<vsomeip::message> &_response)
{
}

void on_message_02(const std::shared_ptr<vsomeip::message> &_response)
{
}

void on_message_03(const std::shared_ptr<vsomeip::message> &_response)
{
}

void on_message_04(const std::shared_ptr<vsomeip::message> &_response)
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
        SOMEIPSRV_SD_BEHAVIOR_01();
        break;
    case 2:
        SOMEIPSRV_SD_BEHAVIOR_02();
        break;
    case 3:
        SOMEIPSRV_SD_BEHAVIOR_03();
        break;
    case 4:
        SOMEIPSRV_SD_BEHAVIOR_04();

    default:
        printf("\n\t TEST NUMBER NOT FOUND \n");
        break;
    }
}

int SOMEIPSRV_SD_BEHAVIOR_01()
{
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_01);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);

    if (sd_return.SD_Result == Receive_E_OK)
    {
        std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) \n"
                  << std::endl;
        test_ok = true;
    }
    else if (sd_return.SD_Result == Receive_Timout)
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE \n"
                   << std::endl;
        test_ok &= false;
    }
    SD_Listen_Return *sd_return2 = Listen_2_Offer(SERVICE_ID_1_REP_BASE_INTV * 2 + ParamToleranceTime, SERVICE_ID_1);
    if (sd_return2[0].SD_Result == Receive_E_OK && sd_return2[1].SD_Result == Receive_E_OK)
    {
        std::cout << "\nPart 2 of test is Ok: Notification received (OFFER SERVICE) \n"
                  << std::endl;
        test_ok = true;
    }
    else if (sd_return2[0].SD_Result == Receive_E_OK && sd_return2[1].SD_Result == Receive_Timout)
    {
        std::cout << "\nPart 2 of test is NOT Ok: Only one notification is received (OFFER SERVICE)\n"
                  << std::endl;
        test_ok &= false;
    }
    else
    {
        std::cout << "\nPart 2 of test is NOT Ok: Didn't receive any Notification (OFFER SERVICE) \n"
                  << std::endl;
        test_ok &= false;
    }
    if (test_ok)
    {
        if (std::chrono::duration_cast<std::chrono::microseconds>(sd_return2[1].timestamp - sd_return2[0].timestamp).count() >= SERVICE_ID_1_REP_BASE_INTV * 2 - ParamToleranceTimeMillisec)
        {
            std::cout << "\nPart 3 of test is Ok\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 3 of test is NOT Ok \n"
                      << std::endl;
            test_ok = false;
        }
    }
    else
    {
        std::cout << "\nPart 3 of test is NOT Ok \n"
                  << std::endl;
        test_ok = false;
    }
    if (test_ok)
    {
        if (std::chrono::duration_cast<std::chrono::microseconds>(sd_return2[1].timestamp - sd_return2[0].timestamp).count() <= SERVICE_ID_1_REP_BASE_INTV * 2 + ParamToleranceTimeMillisec)
        {
            std::cout << "\nPart 4 of test is Ok\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 4 of test is NOT Ok \n"
                      << std::endl;
            test_ok = false;
        }
    }
    else
    {
        std::cout << "\nPart 4 of test is NOT Ok \n"
                  << std::endl;
        test_ok = false;
    }
    std::thread sender(run_01);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_SD_BEHAVIOR_01 : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_SD_BEHAVIOR_01 : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_SD_BEHAVIOR_02()
{

    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_01);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);

    if (sd_return.SD_Result == Receive_E_OK)
    {
        std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) \n"
                  << std::endl;
        test_ok = true;
    }
    else if (sd_return.SD_Result == Receive_Timout)
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE \n"
                   << std::endl;
        test_ok &= false;
    }
    sleep(SERVICE_ID_1_TOTAL_REP_INTV); // DUT Enters Main Phase
    SD_Listen_Return *sd_return2 = Listen_2_Offer(SERVICE_ID_1_REP_BASE_INTV * 2 + ParamToleranceTime, SERVICE_ID_1);
    if (sd_return2[0].SD_Result == Receive_E_OK && sd_return2[1].SD_Result == Receive_E_OK)
    {
        std::cout << "\nPart 2 of test is Ok: Notification received (OFFER SERVICE) \n"
                  << std::endl;
        test_ok = true;
    }
    else if (sd_return2[0].SD_Result == Receive_E_OK && sd_return2[1].SD_Result == Receive_Timout)
    {
        std::cout << "\nPart 2 of test is NOT Ok: Only one notification is received (OFFER SERVICE)\n"
                  << std::endl;
        test_ok &= false;
    }
    else
    {
        std::cout << "\nPart 2 of test is NOT Ok: Didn't receive any Notification (OFFER SERVICE) \n"
                  << std::endl;
        test_ok &= false;
    }
    if (test_ok)
    {
        if (std::chrono::duration_cast<std::chrono::seconds>(sd_return2[1].timestamp - sd_return2[0].timestamp).count() >= SERVICE_ID_1_CYCLE_INTV - ParamToleranceTime)
        {
            std::cout << "\nPart 3 of test is Ok\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 3 of test is NOT Ok \n"
                      << std::endl;
            test_ok = false;
        }
    }
    else
    {
        std::cout << "\nPart 3 of test is NOT Ok \n"
                  << std::endl;
        test_ok = false;
    }
    if (test_ok)
    {
        if (std::chrono::duration_cast<std::chrono::seconds>(sd_return2[1].timestamp - sd_return2[0].timestamp).count() <= SERVICE_ID_1_CYCLE_INTV + ParamToleranceTime)
        {
            std::cout << "\nPart 4 of test is Ok\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 4 of test is NOT Ok \n"
                      << std::endl;
            test_ok = false;
        }
    }
    else
    {
        std::cout << "\nPart 4 of test is NOT Ok \n"
                  << std::endl;
        test_ok = false;
    }
    std::thread sender(run_01);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_SD_BEHAVIOR_02 : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_SD_BEHAVIOR_02 : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_SD_BEHAVIOR_03()
{
    printf("NOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_SD_BEHAVIOR_04()
{
    printf("NOT IMPLEMENTED YET ");
    return 2;
}

/** \}    end of addtogroup */