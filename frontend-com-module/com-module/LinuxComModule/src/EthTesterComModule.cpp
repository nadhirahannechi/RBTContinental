#include "TCPServer.h"
#include <thread>
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
    TCPServer ethtcpServer;
    //thread t2(TCP_BASICS_01);
    ethtcpServer.start();/*
    while (1)
        sleep(1);*/
    //t2.join();
    return 0;
}
