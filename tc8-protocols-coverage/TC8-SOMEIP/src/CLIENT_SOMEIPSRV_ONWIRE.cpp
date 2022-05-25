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
#include "SOMEIPSRV_ONWIRE.hpp"

std::shared_ptr<vsomeip::application> app;
std::mutex mutex;
std::condition_variable condition;
std::unique_lock<std::mutex> its_lock(mutex);
int test = 0;
bool test_ok = false;
int test_number;
bool msg_received = false;
auto start = std::chrono::steady_clock::now();

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
/*
     * Handle signal to shutdown
     */
void stop()
{
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    condition.notify_one();
}

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

void on_message_01(const std::shared_ptr<vsomeip::message> &_response)
{

    test++;
    if (test == 1)
    {
        if (_response->get_interface_version() == SERVICE_ID_1_INTF_VER_MAJ)
        {
            std::cout << "\nPart 2 of test is Ok : Correct Major version\n";
        }
        else
        {
            test_ok &= false;
        }
        condition.notify_one();
    }
    if (test == 2)
    {
        if (_response->get_message_type() == vsomeip::message_type_e::MT_ERROR)
        {
            if (_response->get_return_code() == vsomeip::return_code_e::E_WRONG_INTERFACE_VERSION)
            {
                std::cout << "\nPart 3 of test is Ok : Error message received with WRONG_INTERFACE_VERSION code\n\n";
            }
        }
        else
        {
            test_ok &= false;
        }
        //condition.notify_one();
        app->stop();
    }
}
void run_02()
{

    condition.wait(its_lock);
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_service(SERVICE_ID_1);
    request->set_instance(SERVICE_ID_1_INSTANCE_ID);
    request->set_method(METHOD_ID_1_SI_1);
    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    std::string s("Request :) ");
    for (int i = 0; i < s.size(); i++)
    {
        its_payload_data.push_back(s[i]);
    }
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);
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

void on_message_02(const std::shared_ptr<vsomeip::message> &_response)
{

    if (_response->get_service() == SERVICE_ID_1 && _response->get_return_code() == vsomeip::return_code_e::E_OK)
    {
        if ((uint16_t)(_response->get_method() & 0x8000) == 0x0000)
        {
            msg_received = true;
            std::cout << "\nPart 2 of test is Ok : Correct 0-bit field of Method/EventID\n"
                      << std::endl;
        }
        else
        {
            std::cout << "\nPart 2 of test is NOT Ok : wrong  0-bit field of Method/EventID = " << (uint16_t)(_response->get_method() & 0x8000) << "\n"
                      << std::endl;
            test_ok = test_ok && false;
        }
    }
}
void run_03()
{

    condition.wait(its_lock);
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_instance(SERVICE_ID_1_INSTANCE_ID);
    request->set_client(CLIENT_ID_1);
    request->set_session(SESSION_ID_1);
    request->set_service(SERVICE_ID_1);
    request->set_method(METHOD_ID_1_SI_1);
    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    std::string s("Request :) ");
    for (int i = 0; i < s.size(); i++)
    {
        its_payload_data.push_back(s[i]);
    }
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);
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

void on_message_03(const std::shared_ptr<vsomeip::message> &_response)
{

    if (_response->get_service() == SERVICE_ID_1 && _response->get_return_code() == vsomeip::return_code_e::E_OK)
    {
        if (_response->get_client() == CLIENT_ID_1 && _response->get_session() == SESSION_ID_1)
        {
            msg_received = true;
            std::cout << "\nPart 2 of test is Ok :  Correct Request ID\n"
                      << std::endl;
        }
        else
        {
            std::cout << "\nPart 2 of test is NOT Ok : wrong   Correct Request ID = "
                      << "\n"
                      << std::endl;
            test_ok = test_ok && false;
        }
    }
}
void run_04()
{

    condition.wait(its_lock);
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_instance(SERVICE_ID_1_INSTANCE_ID);
    request->set_client(CLIENT_ID_1);
    request->set_session(SESSION_ID_1);
    request->set_service(SERVICE_ID_1);
    request->set_method(METHOD_ID_1_SI_1);
    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    std::string s("Request :) ");
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
    //request n°2
    std::shared_ptr<vsomeip::message> request2;
    request2 = vsomeip::runtime::get()->create_request();
    request2->set_message_type(vsomeip::message_type_e::MT_REQUEST);
    request2->set_instance(SERVICE_ID_1_INSTANCE_ID);
    request2->set_client(CLIENT_ID_1);
    request2->set_session(SESSION_ID_1);
    request2->set_service(SERVICE_ID_1);
    request2->set_method(METHOD_ID_1_SI_1);
    std::shared_ptr<vsomeip::payload> its_payload2 = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data2;
    for (int i = 0; i < s.size(); i++)
    {
        its_payload_data2.push_back(s[i]);
    }
    its_payload2->set_data(its_payload_data2);
    request2->set_payload(its_payload2);
    test++;
    msg_received = false;
    app->send(request2, true);
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
        std::cout << "\nPart 3 of test is NOT Ok: Timout without receiving any response fron DUT\n\n";
        test_ok = test_ok && false;
    }
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_04(const std::shared_ptr<vsomeip::message> &_response)
{
    if (test == 1)
    {
        if (_response->get_request() == CLIENT1_CURR_REQUEST_ID && _response->get_service() == SERVICE_ID_1 && _response->get_return_code() == vsomeip::return_code_e::E_OK)
        {
            msg_received = true;
            std::cout << "\nPart 2 of test is Ok : Response received \n"
                      << std::endl;
        }
    }
    else if (test == 2)
    {
        if (_response->get_request() == CLIENT1_CURR_REQUEST_ID && _response->get_service() == SERVICE_ID_1 && _response->get_return_code() == vsomeip::return_code_e::E_OK)
        {
            msg_received = true;
            std::cout << "\nPart 3 of test is Ok : Response received \n"
                      << std::endl;
        }
    }
}
void run_06()
{
    condition.wait(its_lock);
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_client(CLIENT_ID_1);
    request->set_session(SESSION_ID_1);
    request->set_service(SERVICE_ID_1);
    request->set_instance(SERVICE_ID_1_INSTANCE_ID);
    request->set_method(METHOD_ID_1_SI_1);
    request->set_interface_version(SERVICE_ID_1_INTF_VER_MAJ);
    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    std::string s("Request whith correct version :D");
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

    // Test 2
    msg_received = false;
    std::shared_ptr<vsomeip::message> request2;
    request2 = vsomeip::runtime::get()->create_request();
    request->set_client(CLIENT_ID_1);
    request->set_session(SESSION_ID_1);
    request2->set_service(SERVICE_ID_1);
    request2->set_instance(SERVICE_ID_1_INSTANCE_ID);
    request2->set_method(METHOD_ID_1_SI_1);
    request2->set_interface_version(UNKNOWN_INTF_VER_MAJ_SI_1);

    std::shared_ptr<vsomeip::payload> its_payload2 = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data2;
    std::string s2("Request whith wrong version 3:)");
    for (int i = 0; i < s2.size(); i++)
    {
        its_payload_data2.push_back(s2[i]);
    }
    its_payload2->set_data(its_payload_data2);
    request2->set_payload(its_payload2);
    test++;
    app->send(request2, true);
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
        std::cout << "\nPart 3 of test is NOT Ok: Timout without receiving any message fron DUT\n\n";
        test_ok = test_ok && false;
    }
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_06(const std::shared_ptr<vsomeip::message> &_response)
{
    if (test == 1)
    {
        if (_response->get_request() == CLIENT1_CURR_REQUEST_ID && _response->get_service() == SERVICE_ID_1 && _response->get_method() == METHOD_ID_1_SI_1 && _response->get_return_code() == vsomeip::return_code_e::E_OK)
        {
            if (_response->get_interface_version() == SERVICE_ID_1_INTF_VER_MAJ)
            {
                msg_received = true;
                std::cout << "\nPart 2 of test is Ok : Correct Major version\n";
            }
            else
            {
                test_ok &= false;
                std::cout << "\nPart 2 of test is NOT Ok : wrong Major version\n";
            }
        }
    }
    if (test == 2)
    {
        if (_response->get_message_type() == vsomeip::message_type_e::MT_ERROR)
        {
            if (_response->get_return_code() == vsomeip::return_code_e::E_WRONG_INTERFACE_VERSION)
            {
                msg_received = true;
                std::cout << "\nPart 3 of test is Ok : Error message received with WRONG_INTERFACE_VERSION code\n\n";
            }
        }
        else
        {
            std::cout << "\nPart 3 of test is NOT Ok \n\n";
            test_ok &= false;
        }
    }
}
void run_07()
{
    condition.wait(its_lock);
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_client(CLIENT_ID_1);
    request->set_session(SESSION_ID_1);
    request->set_service(SERVICE_ID_1);
    request->set_instance(SERVICE_ID_1_INSTANCE_ID);
    request->set_method(METHOD_ID_1_SI_1);

    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    std::string s("Request whith correct version :D");
    for (int i = 0; i < s.size(); i++)
    {
        its_payload_data.push_back(s[i]);
    }
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);

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

void on_message_07(const std::shared_ptr<vsomeip::message> &_response)
{

    test++;
    if (test == 1)
    {
        if (_response->get_interface_version() == SERVICE_ID_1_INTF_VER_MAJ && _response->get_request() == CLIENT1_CURR_REQUEST_ID && _response->get_service() == SERVICE_ID_1 && _response->get_method() == METHOD_ID_1_SI_1 && _response->get_return_code() == vsomeip::return_code_e::E_OK)
        {
            msg_received = true;
            std::cout << "\nPart 2 of test is Ok :Response received\n";
        }
        else
        {
            test_ok &= false;
        }
    }
}
void run_10()
{
    condition.wait(its_lock);
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_client(CLIENT_ID_1);
    request->set_session(SESSION_ID_1);
    request->set_service(SERVICE_ID_1);
    request->set_instance(SERVICE_ID_1_INSTANCE_ID);
    request->set_method(METHOD_ID_1_SI_1);
    request->set_interface_version(SERVICE_ID_1_INTF_VER_MAJ);

    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    std::string s("Request whith correct version :D");
    for (int i = 0; i < s.size(); i++)
    {
        its_payload_data.push_back(s[i]);
    }
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);

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

    // Test 2

    msg_received = false;
    std::shared_ptr<vsomeip::message> request2;
    request2 = vsomeip::runtime::get()->create_request();
    request2->set_instance(SERVICE_ID_1_INSTANCE_ID);
    request->set_client(CLIENT_ID_1);
    request->set_session(SESSION_ID_1);
    request2->set_interface_version(SERVICE_ID_1_INTF_VER_MAJ);
    request2->set_service(UNKNOWN_SERVICE_ID);
    request2->set_method(METHOD_ID_1_SI_1);

    std::shared_ptr<vsomeip::payload> its_payload2 = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data2;
    std::string s2("Request whith wrong version 3:)");
    for (int i = 0; i < s2.size(); i++)
    {
        its_payload_data2.push_back(s2[i]);
    }
    its_payload2->set_data(its_payload_data2);
    request2->set_payload(its_payload2);

    app->send(request2, true);
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
        std::cout << "\nPart 3 of test is NOT Ok: Timout without receiving any message fron DUT\n\n";
        test_ok = test_ok && false;
    }
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_10(const std::shared_ptr<vsomeip::message> &_response)
{

    test++;
    if (test == 1)
    {
        if (_response->get_request() == CLIENT1_CURR_REQUEST_ID && _response->get_service() == SERVICE_ID_1 && _response->get_method() == METHOD_ID_1_SI_1 && _response->get_return_code() == vsomeip::return_code_e::E_OK)
        {
            msg_received = true;
            std::cout << "\nPart 2 of test is Ok : Response received \n";
        }
    }
    if (test == 2)
    {
        if (_response->get_request() == CLIENT1_CURR_REQUEST_ID && _response->get_service() == UNKNOWN_SERVICE_ID && _response->get_method() == METHOD_ID_1_SI_1 && _response->get_return_code() == vsomeip::return_code_e::E_UNKNOWN_SERVICE)
        {
            if (_response->get_message_type() == vsomeip::message_type_e::MT_ERROR)
            {
                msg_received = true;
                std::cout << "\nPart 3 of test is Ok : Error message received with UNKNOWN_SERVICE code\n\n";
            }
            else
            {
                test_ok &= false;
            }
        }
        //condition.notify_one();
    }
}

void run_11()
{
    condition.wait(its_lock);
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_client(CLIENT_ID_1);
    request->set_session(SESSION_ID_1);
    request->set_service(SERVICE_ID_1);
    request->set_method(METHOD_ID_1_SI_1);
    request->set_instance(SERVICE_ID_1_INSTANCE_ID);
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

void on_message_11(const std::shared_ptr<vsomeip::message> &_response)
{
    if (test == 1)
    {
        if (_response->get_request() == CLIENT1_CURR_REQUEST_ID && _response->get_service() == SERVICE_ID_1 && _response->get_method() == METHOD_ID_1_SI_1)
        {
            if (_response->get_return_code() == vsomeip::return_code_e::E_OK)
            {
                msg_received = true;
                std::cout << "\nPart 2 of test is Ok : Correct return code \n";
            }
            else
            {
                test_ok &= false;
                std::cout << "\nPart 2 of test is NOT Ok : wrong return code\n";
            }
        }
    }
}
void run_12()
{
    condition.wait(its_lock);
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_client(CLIENT_ID_1);
    request->set_session(SESSION_ID_1);
    request->set_service(SERVICE_ID_1);
    request->set_instance(SERVICE_ID_1_INSTANCE_ID);
    request->set_method(UNKNOWN_METHOD_ID_SI_1);
    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    std::string s("Request whith correct version :D");
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

void on_message_12(const std::shared_ptr<vsomeip::message> &_response)
{
    if (test == 1)
    {
        if (_response->get_request() == CLIENT1_CURR_REQUEST_ID && _response->get_service() == SERVICE_ID_1 && _response->get_method() == UNKNOWN_METHOD_ID_SI_1)
        {
            if (_response->get_message_type() == vsomeip::message_type_e::MT_ERROR)
            {
                msg_received = true;
                std::cout << "\nPart 2 of test is Ok : Message type error received\n";
            }
            else
            {
                test_ok &= false;
                std::cout << "\nPart 2 of test is NOT Ok : Didn't receive any message type error\n";
            }
        }
    }
}

int main(int argc, char *argv[])
{
    test_number = atoi(argv[1]);
    switch (test_number)
    {
    case 1:
        SOMEIPSRV_ONWIRE_MESSAGE_01();
        break;
    case 2:
        SOMEIPSRV_ONWIRE_MESSAGE_02();
        break;
    case 3:
        SOMEIPSRV_ONWIRE_MESSAGE_03();
        break;
    case 4:
        SOMEIPSRV_ONWIRE_MESSAGE_04();
        break;
    case 5:
        SOMEIPSRV_ONWIRE_MESSAGE_05();
        break;
    case 6:
        SOMEIPSRV_ONWIRE_MESSAGE_06();
        break;
    case 7:
        SOMEIPSRV_ONWIRE_MESSAGE_07();
        break;
    case 11:
        SOMEIPSRV_ONWIRE_MESSAGE_10();
        break;
    case 13:
        SOMEIPSRV_ONWIRE_MESSAGE_11();
        break;
    case 14:
        SOMEIPSRV_ONWIRE_MESSAGE_12();
        break;
    default:
        printf("\n\t TEST NUMBER NOT FOUND \n");
        break;
    }
}

int SOMEIPSRV_ONWIRE_MESSAGE_01()
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
    std::thread sender(run_01);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_06 : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_06 : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_ONWIRE_MESSAGE_02()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_02);
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
    std::thread sender(run_02);
    app->start();
    sender.join();
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_02 : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_02 : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_ONWIRE_MESSAGE_03()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_03);
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
    std::thread sender(run_03);
    app->start();
    sender.join();
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_03 : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_03 : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_ONWIRE_MESSAGE_04()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_04);
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
    std::thread sender(run_04);
    app->start();
    sender.join();
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_04 : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_04 : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_ONWIRE_MESSAGE_05()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_ONWIRE_MESSAGE_06()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_06);
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
    std::thread sender(run_06);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_06 : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_06 : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_ONWIRE_MESSAGE_07()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_07);
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
    std::thread sender(run_07);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_07 : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_07 : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_ONWIRE_MESSAGE_10()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_10);
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
    std::thread sender(run_10);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_10 : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_10 : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_ONWIRE_MESSAGE_11()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_11);
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
    std::thread sender(run_11);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_06 : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_06 : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_ONWIRE_MESSAGE_12()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_12);
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
    std::thread sender(run_12);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_06 : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_ONWIRE_06 : Test_NOK \n";
        return 1;
    }
}