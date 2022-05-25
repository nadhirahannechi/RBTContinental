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
#include "SOMEIPSRV_OPTIONS.hpp"

std::shared_ptr<vsomeip::application> app;
std::mutex mutex;
std::condition_variable condition;

std::unique_lock<std::mutex> its_lock(mutex);


int test = 0;
bool test_ok = false;
int test_number;

void run()
{
    condition.wait(its_lock);

    if (test_number >= 8)
    {
        std::set<vsomeip::eventgroup_t> its_groups;
        its_groups.insert(EVENT_GROUP_ID_1_SI_1);
        app->request_event(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_ID_1_EG_ID_1, its_groups, true);
        app->subscribe(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_GROUP_ID_1_SI_1);
        SD_Listen_Return sd_return;

        if (test_number == 8)
        {
            sd_return = ListenSubscribeAck(ParamListenTime, SERVICE_ID_1, EVENT_GROUP_ID_1_SI_1, vsomeip::sd::option_type_e::IP4_MULTICAST);
            //check_subscribe_ack_validation(test_number, sd_return);
        }
        else
        {
            sd_return = ListenSubscribeAck(ParamListenTime, SERVICE_ID_1, EVENT_GROUP_ID_1_SI_1);
            //check_subscribe_ack_validation(test_number, sd_return);
        }
        app->unsubscribe(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_GROUP_ID_1_SI_1);
        app->release_event(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, EVENT_ID_1_EG_ID_1);
    }
    app->clear_all_handler();
    app->release_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    app->stop();
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
        SOMEIPSRV_OPTIONS_01();
        break;
    case 2:
        SOMEIPSRV_OPTIONS_02();
        break;
    case 3:
        SOMEIPSRV_OPTIONS_03();
        break;
    case 4:
        SOMEIPSRV_OPTIONS_04();
        break;
    case 5:
        SOMEIPSRV_OPTIONS_05();
        break;
    case 6:
        SOMEIPSRV_OPTIONS_06();
        break;
    case 7:
        SOMEIPSRV_OPTIONS_07();
        break;
    case 8:
        SOMEIPSRV_OPTIONS_08();
        break;
    case 9:
        SOMEIPSRV_OPTIONS_09();
        break;
    case 10:
        SOMEIPSRV_OPTIONS_10();
        break;
    case 11:
        SOMEIPSRV_OPTIONS_11();
        break;
    case 12:
        SOMEIPSRV_OPTIONS_12();
        break;
    case 13:
        SOMEIPSRV_OPTIONS_13();
        break;
    case 14:
        SOMEIPSRV_OPTIONS_14();
        break;
    case 15:
        SOMEIPSRV_OPTIONS_15();
        break;

    default:
        printf("\n\t TEST NUMBER NOT FOUND \n");
        break;
    }
}

int SOMEIPSRV_OPTIONS_01()
{
    app = vsomeip::runtime::get()->create_application("testClient");
    app->init();
    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);
    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1, vsomeip::sd::option_type_e::IP4_ENDPOINT, 1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        vsomeip::sd::option_impl *o;
        o = get_ipv4_option(sd_return.SD_Received_Message);
        if (o->get_length() == 0x0009)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) \n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT OK: Notification received (OFFER SERVICE)  with wrong  length of ipv4 endpoint = " << sd_return.SD_Received_Message->get_options()[0].get()->get_length() << "\n"
                      << std::endl;
        }
    }
    else if (sd_return.SD_Result == Receive_Timout)
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE \n"
                   << std::endl;
    }
    std::thread sender(run);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_OPTIONS_01: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_OPTIONS_01: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_OPTIONS_02()
{
    app = vsomeip::runtime::get()->create_application("testClient");

    app->init();

    app->register_availability_handler(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID, on_availability);

    app->request_service(SERVICE_ID_1, SERVICE_ID_1_INSTANCE_ID);

    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl daemon-reload ");
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl start SomeipServer.service");
    SD_Listen_Return sd_return = ListenOffer(ParamListenTime, SERVICE_ID_1);
    if (sd_return.SD_Result == Receive_E_OK)
    {
        vsomeip::sd::option_impl *o;
        if ((int)(sd_return.SD_Received_Message->get_options()[0]->get_type()) == (int)vsomeip::sd::option_type_e::IP4_ENDPOINT)
        {
            std::cout << "\nPart 1 of test is Ok: Notification received (OFFER SERVICE) with Type of Option Array is set to SOMEIP_OPTION_IPV4_ENDPOINT \n"
                      << std::endl;
            test_ok = true;
        }
        else
        {
            std::cout << "\nPart 1 of test is NOT Ok: Notification received (OFFER SERVICE) with wrong Type of Option Array \n"
                      << std::endl;
            std ::cout << "Found : "<<(int)(sd_return.SD_Received_Message->get_options()[0]->get_type())<<" , Expected : " << (int) vsomeip::sd::option_type_e::IP4_ENDPOINT<<std::endl;
        }
    }
    else if (sd_return.SD_Result == Receive_Timout)
    {
        std ::cout << "\nPart 1 of test is NOT Ok: Timeout without receiving OFFER SERVICE \n"
                   << std::endl;
    }
    std::thread sender(run);
    app->start();
    sender.join();
    system("sshpass -p raspberry ssh pi@192.168.20.117 sudo systemctl stop SomeipServer.service");
    if (test_ok)
    {
        std::cout << "\nSOMEIPSRV_OPTIONS_02: : Test_OK \n";
        return 0;
    }
    else
    {
        std::cout << "\nSOMEIPSRV_OPTIONS_02: : Test_NOK \n";
        return 1;
    }
}
int SOMEIPSRV_OPTIONS_03()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_OPTIONS_04()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_OPTIONS_05()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_OPTIONS_06()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_OPTIONS_07()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_OPTIONS_08()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_OPTIONS_09()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_OPTIONS_10()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_OPTIONS_11()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_OPTIONS_12()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_OPTIONS_13()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_OPTIONS_14()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
int SOMEIPSRV_OPTIONS_15()
{
    printf("\tNOT IMPLEMENTED YET ");
    return 2;
}
