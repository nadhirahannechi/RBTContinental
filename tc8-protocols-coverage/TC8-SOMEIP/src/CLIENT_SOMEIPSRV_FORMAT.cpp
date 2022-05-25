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
#include "SOMEIPSRV_FORMAT.hpp"

std::shared_ptr<vsomeip::application> app;
std::mutex mutex;
std::condition_variable condition;
std::unique_lock<std::mutex> its_lock(mutex);

int test = 0;
bool test_ok = false;
int test_number;

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
void run_05()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}
void run_06()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}
void run_07()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}
void run_08()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}
// others 9 -11
void run_12()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}
void run_13()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}
void run_14()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}
void run_15()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}
void run_16()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}
void run_17()
{
    condition.wait(its_lock);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
}
void run_19()
{
    condition.wait(its_lock);
    std::set<vsomeip::eventgroup_t> its_groups;
    its_groups.insert(EVENT_GROUP_ID_1_SI_1);
    app->request_event(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_ID_1_EG_ID_1, its_groups, true);
    app->subscribe(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_GROUP_ID_1_SI_1);
    SD_Listen_Return sd_return;
    sd_return = ListenSubscribeAck(ParamListenTime, SERVICE_ID_1, EVENT_GROUP_ID_1_SI_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        vsomeip::sd::eventgroupentry_impl e;
        std::cout << "\nPart 2 of test is Ok: Notification received (SUBSCRIBE ACK) \n"
                  << std::endl;
        test_ok = test_ok && true;
    }
    else
    {
        std::cout << "\nPart 2 of test is NOT OK: Timout without receiving SUBSCRIBE ACK \n"
                  << std::endl;
    }

    app->unsubscribe(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_GROUP_ID_1_SI_1);
    app->release_event(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_ID_1_EG_ID_1);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
    condition.notify_one();
}
void run_21()
{
    condition.wait(its_lock);
    std::set<vsomeip::eventgroup_t> its_groups;
    its_groups.insert(EVENT_GROUP_ID_1_SI_1);
    app->request_event(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_ID_1_EG_ID_1, its_groups, true);
    app->subscribe(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_GROUP_ID_1_SI_1);
    SD_Listen_Return sd_return;
    sd_return = ListenSubscribeAck(ParamListenTime, SERVICE_ID_1, EVENT_GROUP_ID_1_SI_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        vsomeip::sd::eventgroupentry_impl e;

        uint16_t temp = sd_return.SD_Received_Message->get_option_index(sd_return.SD_Received_Message->get_options()[1]);
        if (temp == 0x00)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE)  with Index 1st Option in Entry Array = 0 \n"
                      << std::endl;
            test_ok = test_ok && true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT OK: Notification received (OFFER SERVICE)  with wrong Index 1st Option in Entry Array = " << temp << "\n"
                      << std::endl;
            test_ok = test_ok && false;
        }
    }
    else
    {
        std::cout << "\nPart 2 of test is NOT OK: Timout without receiving SUBSCRIBE ACK \n"
                  << std::endl;
    }

    app->unsubscribe(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_GROUP_ID_1_SI_1);
    app->release_event(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_ID_1_EG_ID_1);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
    condition.notify_one();
}
void run_23()
{
    condition.wait(its_lock);
    std::set<vsomeip::eventgroup_t> its_groups;
    its_groups.insert(EVENT_GROUP_ID_1_SI_1);
    app->request_event(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_ID_1_EG_ID_1, its_groups, true);
    app->subscribe(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_GROUP_ID_1_SI_1);
    SD_Listen_Return sd_return;
    sd_return = ListenSubscribeAck(ParamListenTime, SERVICE_ID_1, EVENT_GROUP_ID_1_SI_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        vsomeip::sd::eventgroupentry_impl e;
        std::cout << "\nPart 2 of test is Ok: Notification received (SUBSCRIBE ACK) \n"
                  << std::endl;
        test_ok = test_ok && true;
    }
    else
    {
        std::cout << "\nPart 2 of test is NOT OK: Timout without receiving SUBSCRIBE ACK \n"
                  << std::endl;
    }

    app->unsubscribe(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_GROUP_ID_1_SI_1);
    app->release_event(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_ID_1_EG_ID_1);
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
    condition.notify_one();
}

int main(int argc, char *argv[])
{
    test_number = atoi(argv[1]);
    switch (test_number)
    {
    case 1:
        SOMEIPSRV_FORMAT_01();
        break;
    case 2:
        SOMEIPSRV_FORMAT_02();
        break;
    case 3:
        SOMEIPSRV_FORMAT_03();
        break;
    case 4:
        SOMEIPSRV_FORMAT_04();
        break;
    case 5:
        SOMEIPSRV_FORMAT_05();
        break;
    case 6:
        SOMEIPSRV_FORMAT_06();
        break;
    case 7:
        SOMEIPSRV_FORMAT_07();
        break;
    case 8:
        SOMEIPSRV_FORMAT_08();
        break;
    case 9:
        SOMEIPSRV_FORMAT_09();
        break;
    case 10:
        SOMEIPSRV_FORMAT_10();
        break;
    case 11:
        SOMEIPSRV_FORMAT_11();
        break;
    case 12:
        SOMEIPSRV_FORMAT_12();
        break;
    case 13:
        SOMEIPSRV_FORMAT_13();
        break;
    case 14:
        SOMEIPSRV_FORMAT_14();
        break;
    case 15:
        SOMEIPSRV_FORMAT_15();
        break;
    case 16:
        SOMEIPSRV_FORMAT_16();
        break;
    case 17:
        SOMEIPSRV_FORMAT_17();
        break;
    case 18:
        SOMEIPSRV_FORMAT_18();
        break;
    case 19:
        SOMEIPSRV_FORMAT_19();
        break;
    case 20:
        SOMEIPSRV_FORMAT_20();
        break;
    case 21:
        SOMEIPSRV_FORMAT_21();
        break;
    case 23:
        SOMEIPSRV_FORMAT_23();
        break;
    case 24:
        SOMEIPSRV_FORMAT_24();
        break;
    case 25:
        SOMEIPSRV_FORMAT_25();
        break;
    case 26:
        SOMEIPSRV_FORMAT_26();
        break;
    case 27:
        SOMEIPSRV_FORMAT_27();
        break;
    case 28:
        SOMEIPSRV_FORMAT_28();
        break;
    default:
        printf("\n\t TEST NUMBER NOT FOUND \n");
        break;
    }
}
int SOMEIPSRV_FORMAT_01()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        if (sd_return.SD_Received_Message->get_client() == 0x0000)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE)  with Client ID = 0x0000\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT OK: Notification received (OFFER SERVICE)  with Client ID = " << sd_return.SD_Received_Message->get_client() << "\n"
                      << std::endl;
        }
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_01);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_01: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_01: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_02()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        if (sd_return.SD_Received_Message->get_session() == 0x0001)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE)  with Session ID = 0x0000\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT OK: Notification received (OFFER SERVICE)  with Session ID = " << sd_return.SD_Received_Message->get_session() << "\n"
                      << std::endl;
        }
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_02);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_02: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_02: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_03()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        if (sd_return.SD_Received_Message->get_protocol_version() == 0x01)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE)  with Protocol Version = 0x01\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT OK: Notification received (OFFER SERVICE)  with Protocol Version = " << sd_return.SD_Received_Message->get_protocol_version() << "\n"
                      << std::endl;
        }
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_03);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_03: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_03: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_04()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        if (sd_return.SD_Received_Message->get_interface_version() == 0x01)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE)  with Interface Version = 0x01\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT OK: Notification received (OFFER SERVICE)  with Interface Version = " << sd_return.SD_Received_Message->get_interface_version() << "\n"
                      << std::endl;
        }
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_04);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_04: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_04: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_05()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        if ((int)sd_return.SD_Received_Message->get_message_type() == 0x02)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE)  with Message Type= 0x02\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT OK: Notification received (OFFER SERVICE)  with Message Type = " << (int)sd_return.SD_Received_Message->get_message_type() << "\n"
                      << std::endl;
        }
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_05);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_05: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_05: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_06()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        if ((int)sd_return.SD_Received_Message->get_return_code() == 0x00)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE)  with Return Code= 0x00\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT OK: Notification received (OFFER SERVICE)  with Return Code = " << (int)sd_return.SD_Received_Message->get_return_code() << "\n"
                      << std::endl;
        }
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_06);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_06: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_06: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_07()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        if ((int)sd_return.SD_Received_Message->get_reboot_flag() == 1)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE)  with Reboot flag = 1\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT OK: Notification received (OFFER SERVICE)  with Reboot flag = " << (int)sd_return.SD_Received_Message->get_reboot_flag() << "\n"
                      << std::endl;
        }
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_07);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_07: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_07: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_08()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        if ((int)sd_return.SD_Received_Message->get_unicast_flag() == 1)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE)  with Unicast flag = 1\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT OK: Notification received (OFFER SERVICE)  with Unicast flag = " << (int)sd_return.SD_Received_Message->get_unicast_flag() << "\n"
                      << std::endl;
        }
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_08);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_08: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_08: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_09()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_FORMAT_10()
{
}
int SOMEIPSRV_FORMAT_11()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    sleep(2);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        vsomeip::sd::entry_impl *e;
        int numberOfEntries;
        bool correct_type;
        correct_type = false;
        //vsomeip::length_t x;
        int type;
        e = get_first_entry(sd_return.SD_Received_Message);
        if ((int)e->get_type() == 0x01)
        {
            correct_type = true;
        }
        else
        {
            type = (int)e->get_type();
        }
        numberOfEntries = sd_return.SD_Received_Message->get_entries().size();
        int entries_length = get_entries_length(sd_return.SD_Received_Message);
        if ((entries_length == numberOfEntries * 16) && correct_type)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE)  with Entry Type  = 0x01 and Entry Length = (NumberOfEntries*16)\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT OK: Notification received (OFFER SERVICE)  with Entry Type  = " << type << " and Entry Length = " << entries_length << "\n"
                      << std::endl;
        }
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_08);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_11: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_11: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_12()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        if (sd_return.SD_Received_Message->get_option_index(sd_return.SD_Received_Message->get_options()[0]) == 0x00)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE)  with Index 1st Option in Entry Array = 0 \n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT OK: Notification received (OFFER SERVICE)  with wrong Index 1st Option in Entry Array"
                      << "\n"
                      << std::endl;
        }
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_12);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_12: Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_12: Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_13()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        vsomeip::sd::entry_impl *e;
        e = get_first_entry(sd_return.SD_Received_Message);
        if (e->get_num_options(1) >= (uint8_t)1)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE)  with Num of Opt 1 in Entry Array >= 1\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT OK: Notification received (OFFER SERVICE)  with Num of Opt 1 in Entry Array = " << e->get_num_options(1) << "\n"
                      << std::endl;
        }
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_13);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_13: Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_13: Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_14()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) \n"
                  << std::endl;
        test_ok = true;
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_14);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_14: Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_14: Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_15()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        vsomeip::sd::entry_impl *e;
        e = get_first_entry(sd_return.SD_Received_Message);
        if (e->get_instance() == (int)SERVICE_ID_1_INSTANCE_ID)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE)  with  Instance ID in Entry Array = " << SERVICE_ID_1_INSTANCE_ID << "\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT OK: Notification received (OFFER SERVICE)  with Instance ID in Entry Array = " << e->get_instance() << "\n"
                      << std::endl;
        }
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_15);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_15: Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_15: Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_16()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        vsomeip::sd::entry_impl *e;
        e = get_first_entry(sd_return.SD_Received_Message);
        if ((uint8_t)e->get_major_version() == (uint8_t)SERVICE_ID_1_MAJ_VER)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE)  with  Major Version in Entry Array = " << SERVICE_ID_1_MAJ_VER << "\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT OK: Notification received (OFFER SERVICE)  with Major Version  in Entry Array = " << (int)e->get_major_version() << "\n"
                      << std::endl;
        }
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_16);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_16: Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_16: Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_17()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        vsomeip::sd::entry_impl *e;
        e = get_first_entry(sd_return.SD_Received_Message);
        if (e->get_ttl() == (int)SERVICE_ID_1_MAJ_VER)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE)  with TTL in Entry Array = " << SERVICE_ID_1_MAJ_VER << "\n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT OK: Notification received (OFFER SERVICE)  with TTL in Entry Array = " << (int)e->get_major_version() << "\n"
                      << std::endl;
        }
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_17);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_17: Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_17: Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_18()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_FORMAT_19()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) \n"
                  << std::endl;
        test_ok = true;
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_19);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_19: Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_19: Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_20()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_FORMAT_21()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) \n"
                  << std::endl;
        test_ok = true;
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_21);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_21: Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_21: Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_23()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServerEvent.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) \n"
                  << std::endl;
        test_ok = true;
    }
    else
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE  \n"
                   << std::endl;
    }
    std::thread sender(run_23);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServerEvent.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_FORMAT_23: Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_FORMAT_23: Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_FORMAT_24()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_FORMAT_25()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_FORMAT_26()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_FORMAT_27()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_FORMAT_28()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}