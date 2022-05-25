#include <iostream>
//sniff offer service
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h> //malloc
#include <unistd.h> 


int connectUdp (char* ipAddr, int  port);
int recvUdp(int __fd, void *__buf, size_t __n);
int closeUdp(int __fd);