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
#include "SOMEIPSRV_SD_MESSAGE.hpp"

std::shared_ptr<vsomeip::application> app;
std::mutex mutex;
std::condition_variable condition;
std::unique_lock<std::mutex> its_lock(mutex);
int test = 0;
bool test_ok = false;
int test_number;
bool msg_received = false;
auto start = std::chrono::steady_clock::now();
vsomeip::instance_t extractedInstID1;


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

/*void on_state(vsomeip::state_type_e _state) {
        std::cout << "Application " << app->get_name() << " is "
                << (_state == vsomeip::state_type_e::ST_REGISTERED ?
                        "registered." : "deregistered.")
                << std::endl;

        if (_state == vsomeip::state_type_e::ST_REGISTERED) {
            if (!is_registered_) {
                is_registered_ = true;
                blocked_ = true;
                condition_.notify_one();
            }
        } else {
            is_registered_ = false;
        }
    }*/
void run_01()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_01(const std::shared_ptr<vsomeip::message> &_response)
{
}
void run_03()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_03(const std::shared_ptr<vsomeip::message> &_response)
{
}
void run_04()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_04(const std::shared_ptr<vsomeip::message> &_response)
{
}
void run_05()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_05(const std::shared_ptr<vsomeip::message> &_response)
{
}

void run_06()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_06(const std::shared_ptr<vsomeip::message> &_response)
{
}
void run_07()
{
    //condition.wait(its_lock);
    sleep(SERVICE_ID_1_TTL);
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_client(CLIENT_ID_1);
    request->set_session(SESSION_ID_1);
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
        std::cout << "\nPart 2 of test is Ok: Timout without receiving any message fron DUT\n\n";
        test_ok = test_ok && true;
    }
    else
    {
        std::cout << "\nPart 2 of test is NOT Ok: DUT sends a response \n\n";
        test_ok = test_ok && false;
    }
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_07(const std::shared_ptr<vsomeip::message> &_response)
{
    if (test == 1)
    {
        if (_response->get_request() == CLIENT1_CURR_REQUEST_ID && _response->get_service() == SERVICE_ID_1 && _response->get_method() == METHOD_ID_1_SI_1 && _response->get_return_code() == vsomeip::return_code_e::E_OK)
        {
            msg_received = true;
        }
    }
}
void run_08()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_08(const std::shared_ptr<vsomeip::message> &_response)
{
}
void run_09()
{
    msg_received = false;
     std::cout << "before" << std::endl;
    condition.wait(its_lock);
    std::cout << "after" << std::endl;
    std::set<vsomeip::eventgroup_t> its_groups;
    its_groups.insert(EVENT_GROUP_ID_1_SI_1);
    app->request_event(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_ID_1_EG_ID_1, its_groups, true); // ToDo  remove extract  ....
    app->subscribe(SERVICE_ID_1, extractedInstID1, EVENT_GROUP_ID_1_SI_1);                            //instance set  to 0 causes a problem
    SD_Listen_Return sd_return;
    sd_return = ListenSubscribeAck(ParamListenTime, SERVICE_ID_1, EVENT_GROUP_ID_1_SI_1, (vsomeip::sd::option_type_e)0, extractedInstID1); // Todo : modify the sd Api
    if (sd_return.SD_Result == Receive_E_OK)
    {
        std::cout << "\nPart 2 of test is Ok: Notification received (SUBSCRIBE ACK) \n"
                  << std::endl;
        test_ok = test_ok && true;
    }
    else if (sd_return.SD_Result == Receive_Timout)
    {
        std ::cout << "\nPart 2 of test is NOT Ok: Timeout without receiving SUBSCRIBE ACK \n"
                   << std::endl;
    }
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
    else
    {
        std::cout << "\nPart 3 of test is Ok: message received fron DUT\n\n";
        test_ok = test_ok && true;
    }
    app->unsubscribe(SERVICE_ID_1, extractedInstID1, EVENT_GROUP_ID_1_SI_1);
    app->release_event(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_ID_1_EG_ID_1); //todo : same as request_event
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}

void on_message_09(const std::shared_ptr<vsomeip::message> &_response)
{
    // ToDo : check the port number
    if (_response->get_service() == SERVICE_ID_1 && _response->get_return_code() == vsomeip::return_code_e::E_OK && _response->get_method() == EVENT_ID_1_EG_ID_1) // the method field contains the event id in case of Event notification
    {
        msg_received = true;
    }
}

void run_11()
{
    condition.wait(its_lock);
    msg_received = false;
    std::set<vsomeip::eventgroup_t> its_groups;
    its_groups.insert(EVENT_GROUP_ID_1_SI_1);
    app->request_event(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_ID_1_EG_ID_1, its_groups, true); // ToDo  remove extract  ....
    app->subscribe(SERVICE_ID_1, extractedInstID1, EVENT_GROUP_ID_1_SI_1);
    SD_Listen_Return sd_return;
    sd_return = ListenSubscribeAck(ParamListenTime, SERVICE_ID_1, EVENT_GROUP_ID_1_SI_1, (vsomeip::sd::option_type_e)0, extractedInstID1); // Todo : modify the sd Api
    if (sd_return.SD_Result == Receive_E_OK)
    {
        std::cout << "\nPart 2 of test is Ok: Notification received (SUBSCRIBE ACK) \n"
                  << std::endl;
        test_ok = test_ok && true;
    }
    else if (sd_return.SD_Result == Receive_Timout)
    {
        std ::cout << "\nPart 2 of test is NOT Ok: Timeout without receiving SUBSCRIBE ACK \n"
                   << std::endl;
        test_ok = test_ok && false;
    }
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
    else
    {
        std::cout << "\nPart 3 of test is Ok: message received fron DUT\n\n";
        test_ok = test_ok && true;
    }
    app->unsubscribe(SERVICE_ID_1, extractedInstID1, EVENT_GROUP_ID_1_SI_1);
    app->release_event(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_ID_1_EG_ID_1); //todo : same as request_event
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}
void on_message_11(const std::shared_ptr<vsomeip::message> &_response)
{
    // ToDo : check the port number
    if (_response->get_service() == SERVICE_ID_1 && _response->get_return_code() == vsomeip::return_code_e::E_OK && _response->get_method() == EVENT_ID_1_EG_ID_1) // the method field contains the event id in case of Event notification
    {
        msg_received = true;
    }
}
void run_13()
{
    condition.wait(its_lock);
    std::set<vsomeip::eventgroup_t> its_groups;
    its_groups.insert(EVENT_GROUP_ID_1_SI_1);
    app->request_event(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_ID_1_EG_ID_1, its_groups, true); // ToDo  remove extract  ....
    app->subscribe(SERVICE_ID_1, extractedInstID1, EVENT_GROUP_ID_1_SI_1);
    //subscribe
}
void on_message_13(const std::shared_ptr<vsomeip::message> &_response)
{
}

int main(int argc, char *argv[])
{
    test_number = atoi(argv[1]);
    switch (test_number)
    {
    case 1:
        SOMEIPSRV_SD_MESSAGE_01();
        break;
    case 2:
        SOMEIPSRV_SD_MESSAGE_02();
        break;
    case 3:
        SOMEIPSRV_SD_MESSAGE_03();
        break;
    case 4:
        SOMEIPSRV_SD_MESSAGE_04();
        break;
    case 5:
        SOMEIPSRV_SD_MESSAGE_05();
        break;
    case 6:
        SOMEIPSRV_SD_MESSAGE_06();
        break;
    case 7:
        SOMEIPSRV_SD_MESSAGE_07();
        break;
    case 8:
        SOMEIPSRV_SD_MESSAGE_08();
        break;
    case 9:
        SOMEIPSRV_SD_MESSAGE_09();
        break;
    case 11:
        SOMEIPSRV_SD_MESSAGE_11();
        break;
    case 13:
        SOMEIPSRV_SD_MESSAGE_13();
        break;
    case 14:
        SOMEIPSRV_SD_MESSAGE_14();
        break;
    case 15:
        SOMEIPSRV_SD_MESSAGE_15();
        break;
    case 16:
        SOMEIPSRV_SD_MESSAGE_16();
        break;
    case 17:
        SOMEIPSRV_SD_MESSAGE_17();
        break;
    case 18:
        SOMEIPSRV_SD_MESSAGE_18();
        break;
    case 19:
        SOMEIPSRV_SD_MESSAGE_19();
        break;
    default:
        printf("\n\t TEST NUMBER NOT FOUND \n");
        break;
    }
}

int SOMEIPSRV_SD_MESSAGE_01()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, 0xFFFF);
    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_01);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1, (vsomeip::sd::option_type_e)0, 0, 2);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        //int nbr_ipv4_endpoint = get_number_of_ipv4_option (sd_return.SD_Received_Message);
        //e=get_first_entry(sd_return.SD_Received_Message);
        if (sd_return.SD_Received_Message->get_entries().size() == 2)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) with number of offer service entries =2  \n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT Ok: Notification received (OFFER SERVICE) with wrong number of offer service entries\n"
                      << std::endl;
        }
    }
    else if (sd_return.SD_Result == Receive_Timout)
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE \n"
                   << std::endl;
    }
    vsomeip::instance_t extractedInstID2;
    extractedInstID2 = sd_return.SD_Received_Message->get_entries()[1]->get_instance();
    if (sd_return.SD_Received_Message->get_entries()[0]->get_instance() != extractedInstID2)
    {
        std::cout << "\nPart 2 of test is Ok: Correct Service Instance ID of Offer Service Entry 1 \n"
                  << std::endl;
        test_ok = true;
    }
    else
    {
        std::cout << "\nPart 2 of test is NOT Ok: Wrong Service Instance ID of Offer Service Entry 1\n"
                  << std::endl;
        test_ok &= false;
    }
    std::thread sender(run_01);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_01: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_01: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_SD_MESSAGE_02()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_SD_MESSAGE_03()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, 0xFF, 0xfffffff);
    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_03);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        vsomeip::sd::entry_impl *e;
        e = get_first_entry(sd_return.SD_Received_Message);
        if (e->get_major_version() == SERVICE_ID_1_MAJ_VER)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) with correct major version \n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT Ok: Notification received (OFFER SERVICE) with wrong major version \n"
                      << std::endl;
        }
    }
    else if (sd_return.SD_Result == Receive_Timout)
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE \n"
                   << std::endl;
    }
    std::thread sender(run_03);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_03: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_03: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_SD_MESSAGE_04()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, SERVICE_ID_1_MAJ_VER, 0xfffffff);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_04);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        vsomeip::sd::entry_impl *e;
        e = get_first_entry(sd_return.SD_Received_Message);
        if (e->get_major_version() == SERVICE_ID_1_MAJ_VER)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) with correct major version \n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT Ok: Notification received (OFFER SERVICE) with wrong major version \n"
                      << std::endl;
        }
    }
    else if (sd_return.SD_Result == Receive_Timout)
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE \n"
                   << std::endl;
    }

    std::thread sender(run_04);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_04: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_04: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_SD_MESSAGE_05()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, SERVICE_ID_1_MAJ_VER, 0xfffffff);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_05);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        vsomeip::sd::entry_impl *e;
        e = get_first_entry(sd_return.SD_Received_Message);
        if (e->get_major_version() == SERVICE_ID_1_MINOR_VER)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) with correct minor version \n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT Ok: Notification received (OFFER SERVICE) with wrong minor version \n"
                      << std::endl;
        }
    }
    else if (sd_return.SD_Result == Receive_Timout)
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE \n"
                   << std::endl;
    }

    std::thread sender(run_05);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_05: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_05: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_SD_MESSAGE_06()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, vsomeip::ANY_MAJOR, 0xFFFFFFFF);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_06);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        vsomeip::sd::entry_impl *e;
        e = get_first_entry(sd_return.SD_Received_Message);
        if (e->get_major_version() == SERVICE_ID_1_MINOR_VER)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) with correct minor version \n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT Ok: Notification received (OFFER SERVICE) with wrong minor version \n"
                      << std::endl;
        }
    }
    else if (sd_return.SD_Result == Receive_Timout)
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE \n"
                   << std::endl;
    }

    std::thread sender(run_06);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_06: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_06: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_SD_MESSAGE_07()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_07);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        if ((int)sd_return.SD_Received_Message->get_entries()[0]->get_ttl() == (int)SERVICE_ID_1_TTL)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) with TTL\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT Ok: Notification received (OFFER SERVICE) with wrong TTL" << (int)sd_return.SD_Received_Message->get_entries()[0]->get_ttl() << " \n"
                      << std::endl;
        }
    }
    else if (sd_return.SD_Result == Receive_Timout)
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE \n"
                   << std::endl;
    }

    std::thread sender(run_07);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_07: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_07: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_SD_MESSAGE_08()
{
    //system("sshpass -p raspberry ssh pi@192.168.20.117 sudo mv /home/pi/precompiled/projet_Rpi.2/SomeipServer.service /etc/systemd/system/SomeipServer.service");
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, SERVICE_ID_1_MAJ_VER, 0xfffffff);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_08);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        int nbr_ipv4_endpoint = get_number_of_ipv4_option(sd_return.SD_Received_Message);
        //e=get_first_entry(sd_return.SD_Received_Message);
        if (nbr_ipv4_endpoint >= 1)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) number of IPV4 endpoints >=1 \n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT Ok: Notification received (OFFER SERVICE) with wrong number of IPV4 endpoints \n"
                      << std::endl;
        }
    }
    else if (sd_return.SD_Result == Receive_Timout)
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE \n"
                   << std::endl;
    }

    std::thread sender(run_08);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_08: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_08: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_SD_MESSAGE_09()
{
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_09);

    sleep(2);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    sleep(2);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        std::cout << "3\n";
        int nbr_ipv4_endpoint = get_number_of_ipv4_option(sd_return.SD_Received_Message);
        std::cout << "4\n";
        if (nbr_ipv4_endpoint >= 1)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) number of IPV4 endpoints >=1 \n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT Ok: Notification received (OFFER SERVICE) with wrong number of IPV4 endpoints \n"
                      << std::endl;
        }
    }
    else if (sd_return.SD_Result == Receive_Timout)
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE \n"
                   << std::endl;
    }
    int extractedport = get_port_of_ipv4_option(sd_return.SD_Received_Message);
    extractedInstID1 = sd_return.SD_Received_Message->get_entries()[1]->get_instance();

    std::thread sender(run_09);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_09: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_09: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_SD_MESSAGE_11()
{
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_11);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        int nbr_ipv4_endpoint = get_number_of_ipv4_option(sd_return.SD_Received_Message);
        if (nbr_ipv4_endpoint >= 1)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) number of IPV4 endpoints >=1 \n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT Ok: Notification received (OFFER SERVICE) with wrong number of IPV4 endpoints \n"
                      << std::endl;
        }
    }
    else if (sd_return.SD_Result == Receive_Timout)
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE \n"
                   << std::endl;
    }
    int extractedport = get_port_of_ipv4_option(sd_return.SD_Received_Message);
    extractedInstID1 = sd_return.SD_Received_Message->get_entries()[1]->get_instance();

    std::thread sender(run_11);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_11: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_11: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_SD_MESSAGE_13()
{
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    app->register_message_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, METHOD_ID_1_SI_1, on_message_13);
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
    }
    extractedInstID1 = sd_return.SD_Received_Message->get_entries()[1]->get_instance();
    std::thread sender(run_13);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_13: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_SD_MESSAGE_13: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_SD_MESSAGE_14()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_SD_MESSAGE_15()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_SD_MESSAGE_16()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_SD_MESSAGE_17()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_SD_MESSAGE_18()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_SD_MESSAGE_19()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}