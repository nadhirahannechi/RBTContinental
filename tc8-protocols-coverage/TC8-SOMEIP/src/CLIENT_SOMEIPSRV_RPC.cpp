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
#include "SOMEIPSRV_RPC.hpp"

std::shared_ptr<vsomeip::application> app;
std::mutex mutex;
std::condition_variable condition;
std::unique_lock<std::mutex> its_lock(mutex);
auto start = std::chrono::steady_clock::now();
int test = 0;
bool test_ok = false;
int test_number;
bool msg_received = false;


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
void run_01()
{
    condition.wait(its_lock);
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_client(CLIENT_ID_1);
    request->set_session(SESSION_ID_1);
    request->set_interface_version(SERVICE_ID_1_INTF_VER_MAJ);
    request->set_service(SERVICE_ID_1);
    request->set_method(METHOD_ID_GET_SI_1);
    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    std::string s("This is a Request No Return :D");
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
        std::cout << "\nPart 2 of test is  Ok: Timout without receiving any message fron DUT\n\n";
        test_ok = test_ok && true;
    }
    else
    {
        std::cout << "\nPart 2 of test is NOT Ok: message received fron DUT\n\n";
        test_ok = test_ok && false;
    }
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_01(const std::shared_ptr<vsomeip::message> &_response)
{
    msg_received = true;
}
void run_04()
{
    condition.wait(its_lock);
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_service(SERVICE_ID_1);
    request->set_instance(SERVICE_ID_1_INSTANCE_ID);
    request->set_method(METHOD_ID_1_SI_1);
    request->set_message_type(vsomeip::message_type_e::MT_REQUEST_NO_RETURN);
    request->set_interface_version(SERVICE_ID_1_INTF_VER_MAJ);
    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    std::string s("This is a Request No Return :D");
    for (int i = 0; i < s.size(); i++)
    {
        its_payload_data.push_back(s[i]);
    }
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);
    app->send(request, true);
    start = std::chrono::steady_clock::now();
    while (true)
    {
        auto end = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() > ParamListenTime)
        {
            break;
        }
    }
    if (msg_received == true)
    {
        std::cout << "\nPart 2 of test is NOT OK: a received response fron DUT\n\n";
        test_ok = test_ok && false;
    }
    else
    {
        std::cout << "\nPart 2 of test is Ok: Timout without receiving any response fron DUT\n\n";
        test_ok = test_ok && true;
    }
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_04(const std::shared_ptr<vsomeip::message> &_response)
{
    if ((_response->get_request() == CLIENT1_CURR_REQUEST_ID) && (_response->get_service() == SERVICE_ID_1) && (_response->get_method() == UNKNOWN_METHOD_ID_SI_1) && (_response->get_return_code() == vsomeip::return_code_e::E_OK))
    {
        msg_received = true;
    }
}
void run_05()
{
    condition.wait(its_lock);
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_message_type(vsomeip::message_type_e::MT_REQUEST_NO_RETURN);
    request->set_client(CLIENT_ID_1);
    request->set_session(SESSION_ID_1);
    request->set_interface_version(SERVICE_ID_1_INTF_VER_MAJ);
    request->set_service(SERVICE_ID_1);
    request->set_method(UNKNOWN_METHOD_ID_SI_1);

    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    std::string s("This is a Request No Return :D");
    for (int i = 0; i < s.size(); i++)
    {
        its_payload_data.push_back(s[i]);
    }
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);

    app->send(request, true);
    start = std::chrono::steady_clock::now();
    //std::cout<< "\nWaiting for 15 seconds \n\n";
    //sleep(15);
    // app -> stop();
    while (true)
    {
        auto end = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() > ParamListenTime)
        {
            break;
        }
    }
    if (msg_received == true)
    {
        std::cout << "\nPart 2 of test is NOT OK: a received response fron DUT\n\n";
        test_ok = test_ok && false;
    }
    else
    {
        std::cout << "\nPart 2 of test is Ok: Timout without receiving any response fron DUT\n\n";
        test_ok = test_ok && true;
    }
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_05(const std::shared_ptr<vsomeip::message> &_response)
{

    if ((_response->get_request() == CLIENT1_CURR_REQUEST_ID) && (_response->get_service() == SERVICE_ID_1) && (_response->get_method() == UNKNOWN_METHOD_ID_SI_1) && (_response->get_return_code() == vsomeip::return_code_e::E_UNKNOWN_METHOD))
    {
        msg_received = true;
    }
}
void run_06()
{

    condition.wait(its_lock);
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_message_type(vsomeip::message_type_e::MT_REQUEST);
    request->set_client(CLIENT_ID_1);
    request->set_session(SESSION_ID_1);
    request->set_interface_version(SERVICE_ID_1_INTF_VER_MAJ);
    request->set_service(SERVICE_ID_1);
    request->set_method(UNKNOWN_METHOD_ID_SI_1);

    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    std::string s("This is a Request No Return :D");
    for (int i = 0; i < s.size(); i++)
    {
        its_payload_data.push_back(s[i]);
    }
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);

    app->send(request, true);
    start = std::chrono::steady_clock::now();
    //std::cout<< "\nWaiting for 15 seconds \n\n";
    //sleep(15);
    // app -> stop();
    while (true)
    {
        auto end = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() > ParamListenTime)
        {
            break;
        }
    }
    if (msg_received == true)
    {
        std::cout << "\nPart 2 of test is OK: a received response fron DUT\n\n";
        test_ok = test_ok && true;
    }
    else
    {
        std::cout << "\nPart 2 of test is NOT Ok: Timout without receiving any response fron DUT\n\n";
        test_ok = test_ok && false;
    }
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_06(const std::shared_ptr<vsomeip::message> &_response)
{

    if ((_response->get_request() == CLIENT1_CURR_REQUEST_ID) && (_response->get_service() == SERVICE_ID_1) && (_response->get_method() == UNKNOWN_METHOD_ID_SI_1))
    {
        if (((uint8_t)_response->get_return_code() & (uint8_t)0xC0) == (uint8_t)0x00)
        {
            msg_received = true;
        }
    }
}

void run_07()
{

    condition.wait(its_lock);
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_message_type(vsomeip::message_type_e::MT_REQUEST);
    request->set_client(CLIENT_ID_1);
    request->set_session(SESSION_ID_1);
    request->set_interface_version(SERVICE_ID_1_INTF_VER_MAJ);
    request->set_return_code((vsomeip::return_code_e)0xC0);
    request->set_service(SERVICE_ID_1);
    request->set_method(METHOD_ID_1_SI_1);

    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    std::string s("This is a Request No Return :D");
    for (int i = 0; i < s.size(); i++)
    {
        its_payload_data.push_back(s[i]);
    }
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);

    app->send(request, true);
    start = std::chrono::steady_clock::now();
    //std::cout<< "\nWaiting for 15 seconds \n\n";
    //sleep(15);
    // app -> stop();
    while (true)
    {
        auto end = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() > ParamListenTime)
        {
            break;
        }
    }
    if (msg_received == true)
    {
        std::cout << "\nPart 2 of test is  OK: a received response fron DUT\n\n";
        test_ok = test_ok && true;
    }
    else
    {
        std::cout << "\nPart 2 of test is NOT Ok: Timout without receiving any response fron DUT\n\n";
        test_ok = test_ok && false;
    }
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_07(const std::shared_ptr<vsomeip::message> &_response)
{

    if ((_response->get_request() == CLIENT1_CURR_REQUEST_ID) && (_response->get_service() == SERVICE_ID_1) && (_response->get_method() == METHOD_ID_1_SI_1) && (_response->get_return_code() == vsomeip::return_code_e::E_OK))
    {
        msg_received = true;
    }
}
void run_08()
{
    condition.wait(its_lock);
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_message_type(vsomeip::message_type_e::MT_REQUEST);
    request->set_client(CLIENT_ID_1);
    request->set_session(SESSION_ID_1);
    request->set_interface_version(SERVICE_ID_1_INTF_VER_MAJ);
    request->set_return_code((vsomeip::return_code_e)0x01);
    request->set_service(SERVICE_ID_1);
    request->set_method(METHOD_ID_1_SI_1);
    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    std::string s("This is a Request No Return :D");
    for (int i = 0; i < s.size(); i++)
    {
        its_payload_data.push_back(s[i]);
    }
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);
    test++;
    msg_received = false;
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
    if (msg_received == true)
    {
        std::cout << "\nPart 2 of test is NOT OK: a received response fron DUT\n\n";
        test_ok = test_ok && false;
    }
    else
    {
        std::cout << "\nPart 2 of test is Ok: Timout without receiving any response fron DUT\n\n";
        test_ok = test_ok && true;
    }

    //condition.wait(its_lock);
    //request n°2
    std::shared_ptr<vsomeip::message> request2;
    request2 = vsomeip::runtime::get()->create_request();
    request2->set_message_type(vsomeip::message_type_e::MT_REQUEST);
    request2->set_client(CLIENT_ID_1);
    request2->set_session(SESSION_ID_1);
    request2->set_interface_version(SERVICE_ID_1_INTF_VER_MAJ);
    request2->set_return_code((vsomeip::return_code_e)0x1F);
    request2->set_service(SERVICE_ID_1);
    request2->set_method(METHOD_ID_1_SI_1);
    std::shared_ptr<vsomeip::payload> its_payload2 = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data2;
    //std::string s("This is a Request No Return :D");
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
    if (msg_received == true)
    {
        std::cout << "\nPart 3 of test is NOT OK: a received response fron DUT\n\n";
        test_ok = test_ok && false;
    }
    else
    {
        std::cout << "\nPart 3 of test is Ok: Timout without receiving any response fron DUT\n\n";
        test_ok = test_ok && true;
    }
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_08(const std::shared_ptr<vsomeip::message> &_response)
{
    if (test == 1)
    {
        if ((_response->get_request() == CLIENT1_CURR_REQUEST_ID) && (_response->get_service() == SERVICE_ID_1) && (_response->get_method() == METHOD_ID_1_SI_1))
        {
            msg_received = true;
        }
    }
    else if (test == 2)
    {
        if ((_response->get_request() == CLIENT1_CURR_REQUEST_ID) && (_response->get_service() == SERVICE_ID_1) && (_response->get_method() == METHOD_ID_1_SI_1))
        {
            msg_received = true;
        }
    }
}
void run_09()
{
    condition.wait(its_lock);
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_message_type(vsomeip::message_type_e::MT_REQUEST);
    request->set_client(CLIENT_ID_1);
    request->set_session(SESSION_ID_1);
    request->set_interface_version(SERVICE_ID_1_INTF_VER_MAJ);
    request->set_service(SERVICE_ID_1);
    request->set_method(UNKNOWN_METHOD_ID_SI_1);

    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    std::string s("This is a Request No Return :D");
    for (int i = 0; i < s.size(); i++)
    {
        its_payload_data.push_back(s[i]);
    }
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);
    //UDP_Packet  pack = CreateUDP();
    //SendUDP(pack);

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

void on_message_09(const std::shared_ptr<vsomeip::message> &_response)
{

    if ((_response->get_request() == CLIENT1_CURR_REQUEST_ID) && (_response->get_service() == SERVICE_ID_1) && (_response->get_method() == UNKNOWN_METHOD_ID_SI_1) && (_response->get_return_code() == vsomeip::return_code_e::E_UNKNOWN_METHOD))
    {
        if (_response->get_length() == 8)
        {
            msg_received = true;
            std::cout << "\nPart 2 of test is  OK: a received SOMEIP_MSG_TYPE_ERROR Message fron DUT with length= 8 \n\n";
        }
        else
        {
            std::cout << "\nPart 2 of test is NOT OK: a received SOMEIP_MSG_TYPE_ERROR Message fron DUT with wrong length= " << _response->get_length() << " \n\n";
            test_ok = test_ok && false;
        }
    }
}

int main(int argc, char *argv[])
{
    test_number = atoi(argv[1]);
    switch (test_number)
    {
    case 1:
        SOMEIPSRV_RPC_MESSAGE_01();
        break;
    case 2:
        SOMEIPSRV_RPC_MESSAGE_02();
        break;
    case 3:
        SOMEIPSRV_RPC_MESSAGE_03();
        break;
    case 4:
        SOMEIPSRV_RPC_MESSAGE_04();
        break;
    case 5:
        SOMEIPSRV_RPC_MESSAGE_05();
        break;
    case 6:
        SOMEIPSRV_RPC_MESSAGE_06();
        break;
    case 7:
        SOMEIPSRV_RPC_MESSAGE_07();
        break;
    case 8:
        SOMEIPSRV_RPC_MESSAGE_08();
        break;
    case 9:
        SOMEIPSRV_RPC_MESSAGE_09();
        break;
    case 11:
        SOMEIPSRV_RPC_MESSAGE_10();
        break;
    case 13:
        SOMEIPSRV_RPC_MESSAGE_11();
        break;
    case 14:
        SOMEIPSRV_RPC_MESSAGE_13();
        break;
    case 15:
        SOMEIPSRV_RPC_MESSAGE_14();
        break;
    case 16:
        SOMEIPSRV_RPC_MESSAGE_17();
        break;
    case 17:
        SOMEIPSRV_RPC_MESSAGE_18();
        break;
    case 18:
        SOMEIPSRV_RPC_MESSAGE_19();
        break;
    case 19:
        SOMEIPSRV_RPC_MESSAGE_20();
        break;
    default:
        printf("\n\t TEST NUMBER NOT FOUND \n");
        break;
    }
}

int SOMEIPSRV_RPC_MESSAGE_01()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_01);
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
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
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_RPC_01: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_RPC_01: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_RPC_MESSAGE_02()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_RPC_MESSAGE_03()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_RPC_MESSAGE_04()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.244 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_04);
    //system("sshpass -p raspberry ssh pi@192.168.20.244 sudo systemctl daemon-reload ");
    //system("sshpass -p raspberry ssh pi@192.168.20.244 sudo systemctl start SomeipServer.service");
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
    //system("sshpass -p raspberry ssh pi@192.168.20.244 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_RPC_04: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_RPC_04: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_RPC_MESSAGE_05()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_05);
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) \n"
                  << std::endl;
        test_ok = true;
    }
    std::thread sender(run_05);
    app->start();
    sender.join();
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_RPC_05: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_RPC_05: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_RPC_MESSAGE_06()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_06);
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) \n"
                  << std::endl;
        test_ok = true;
    }
    std::thread sender(run_06);
    app->start();
    sender.join();
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_RPC_06: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_RPC_06: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_RPC_MESSAGE_07()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_07);
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
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
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_RPC_07: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_RPC_07: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_RPC_MESSAGE_08()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_08);
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) \n"
                  << std::endl;
        test_ok = true;
    }
    std::thread sender(run_08);
    app->start();
    sender.join();
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_RPC_08: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_RPC_08: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_RPC_MESSAGE_09()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_09);
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) \n"
                  << std::endl;
        test_ok = true;
    }
    std::thread sender(run_09);
    app->start();
    sender.join();
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_RPC_09: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_RPC_09: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_RPC_MESSAGE_10()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_RPC_MESSAGE_11()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_RPC_MESSAGE_13()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_RPC_MESSAGE_14()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_RPC_MESSAGE_17()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_RPC_MESSAGE_18()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_RPC_MESSAGE_19()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_RPC_MESSAGE_20()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}