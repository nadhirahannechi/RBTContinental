#include "udp.hpp"

int connectUdp (char* ipAddr, int  port){
    bool wainting = true ;
	int sockfd; 
	struct sockaddr_in addr,addr2; 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		std::cout<<"socket creation failed"<<std::endl; 
	} 
	//std::cout<<"socket creation"<<std::endl;
    u_int yes = 1;
     if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes)) < 0){
       std::cout <<"Reusing ADDR failed"<<std::endl;
    }
	memset(&addr, 0, sizeof(addr)); 
	memset(&addr2, 0, sizeof(addr2)); 
	
	// Filling server information 
	addr.sin_family = AF_INET; // IPv4 
	addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	addr.sin_port = htons(port); 
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&addr,sizeof(addr)) < 0 ) 
	{ 
	    std::cout<<"bind failed"<<std::endl;
        perror("Error printed by perror");  
	} 
	//std::cout<<"bind done"<<std::endl; 

 // use setsockopt() to request that the kernel join a multicast group
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(ipAddr);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)) < 0){
        std::cout<< "setsockopt"<<std::endl;
    }
return sockfd;
}

int recvUdp(int __fd, void *__buf, size_t __n){
    int n = recv(__fd, __buf, __n, 0);
    return n;
}
int closeUdp(int __fd){
	close (__fd);
   //std::cout<< "socket closed " << std::endl;
   return 1;
}