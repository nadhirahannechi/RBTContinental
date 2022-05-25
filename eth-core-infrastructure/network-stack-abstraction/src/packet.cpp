#include "packet.h"
#include "ethernet.h"
#include "ip.h"
#include "icmp.h"
#include "udp.h"
#include "payload.h"
#include "tcp.h"
#include "arp.h"
#include "dhcp.h"
#include "tcpoptions.h"
#include "ip_option.h"
#include "icmpTimestamp.h"

#include <sys/types.h>
#include <cstring>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <time.h>


using namespace std;
int sockId = -1;
packet::packet()
{

}

int packet::cleanUp(void)
{
	int ret = close(sockId);
	sockId = -1;
	return ret;
}
packet::packet(char* data, uint32_t length)
{
	int count=0;
	ethernet *eth = new ethernet(data);
	this->AddLayer(eth);
	if (htons(eth->Get_Type())== ETH_IP)
	{
		ip *IP= new ip(data+ sizeof(ethernet_header));
		this->AddLayer(IP);
		int totalLength= length-sizeof(ethernet_header) ;
		count+=sizeof(IpHeader);
		if(IP->Get_IHL() == 15 && IP->Get_Offset() == 2) // MF =0
		{
			ipOption* ip_option = new ipOption(data + sizeof(ethernet_header)+ sizeof(IpHeader));
			this->AddLayer(ip_option);
			count+=sizeof(ip_option_header);
			payload *p = new payload(data+ sizeof(ethernet_header)+ sizeof(IpHeader)+ sizeof(ip_option_header),totalLength-count);
			this->AddLayer(p);
		}
		else
		{
			//ICMP layer
			if(IP->Get_Protocol()== ICMP_LAYER_Code)
			{	
				if(IP->Get_IHL() == 15)
				{
					ipOption *ip_option = new ipOption(data + sizeof(ethernet_header)+ sizeof(IpHeader));
					this->AddLayer(ip_option);
					count+=sizeof(ip_option_header);
					icmp *ICMP= new icmp(data+ sizeof(ethernet_header)+ sizeof(IpHeader )+ sizeof(ip_option_header));
					this->AddLayer(ICMP);
					count+=sizeof(icmp_header);
					if (count<totalLength)
					{	
						payload *p = new payload(data+ sizeof(ethernet_header)+ sizeof(IpHeader)+ sizeof(ip_option_header) +sizeof(icmp_header),totalLength-count);
						this->AddLayer(p);
					}
					else 
					{
						payload *p = new payload();
						p->Setlength(0);
						this->AddLayer(p);
					}
				}
				else
				{
					icmp *ICMP= new icmp(data+ sizeof(ethernet_header)+ sizeof(IpHeader));
					if(ICMP->Get_type() == ICMP_TIMESTAMP || ICMP->Get_type() == ICMP_TIMESTAMPREPLY)
					{
						icmpTimestamp* ICMP_TIME_STAMP = new icmpTimestamp(data+ sizeof(ethernet_header)+ sizeof(IpHeader)); 
						this->AddLayer(ICMP_TIME_STAMP);
						//count+=sizeof(icmp_time_stamp_header);
						/*payload *p = new payload(data+ sizeof(ethernet_header)+ sizeof(IpHeader)+ sizeof(icmp_time_stamp_header),totalLength-count);
						this->AddLayer(p);*/
					}
					else
					{
						this->AddLayer(ICMP);
						count+=sizeof(icmp_header);
						if (count<totalLength)
						{	
							payload *p = new payload(data+ sizeof(ethernet_header)+ sizeof(IpHeader)+ sizeof(icmp_header),totalLength-count);
							this->AddLayer(p);
						}
						else 
						{
							payload *p = new payload();
							p->Setlength(0);
							this->AddLayer(p);
						}
					}
				}
			}
			//UDP layer
			else if(IP->Get_Protocol()== UDP_LAYER_Code)
			{
				udp *UDP= new udp(data+ sizeof(ethernet_header)+ sizeof(IpHeader));
				this->AddLayer(UDP);
				count+=sizeof(UDPHeader);
				if((UDP->Get_DstPort() == 68 && UDP->Get_SrcPort() == 67) || (UDP->Get_DstPort() == 67 && UDP->Get_SrcPort() == 68))
				{	
					printf("add dhcp part \n");
					dhcp *DHCP = new dhcp(data+ sizeof(ethernet_header)+ sizeof(IpHeader) + sizeof(UDPHeader));
					this->AddLayer((DHCP));
				}
				else if (count<totalLength)
				{	
					printf("add payload\n");
					payload *p = new payload(data+ sizeof(ethernet_header)+ sizeof(IpHeader)+ sizeof(UDPHeader));
					this->AddLayer(p);
				}
			}
			//TCP layer
			else if(IP->Get_Protocol()== TCP_LAYER_Code)
			{
				tcp *TCP= new tcp(data+ sizeof(ethernet_header)+ sizeof(IpHeader));
				this->AddLayer(TCP);
				count+=sizeof(TCPHeader);
				uint8_t TCP_HeaderLength = (uint8_t)TCP->Get_Header_Length();
				if (TCP_HeaderLength>5)
				{
					tcpoptions* TCP_Option= new tcpoptions();
					TCP_Option->Add_Option((uint8_t*)(data+ sizeof(ethernet_header)+ sizeof(IpHeader)+sizeof(TCPHeader)),(TCP_HeaderLength-5)*4);
					count+=(TCP_HeaderLength-5)*4;
					this->AddLayer(TCP_Option);


				}
				else 
				{
					//misra
				}
				if (count<totalLength)
				{
					payload *p = new payload(data+ sizeof(ethernet_header)+ sizeof(IpHeader)+ sizeof(TCPHeader)+(TCP_HeaderLength-5)*4 , totalLength-count);
					this->AddLayer(p);
				}
				else 
				{
					//misra
				}

			}
			
			// Payload layer
			else 
			{
				payload *p = new payload(data+ sizeof(ethernet_header)+ sizeof(IpHeader),totalLength-count);
				this->AddLayer(p);
			}
		}
	}
	else if (htons(eth->Get_Type())== ETH_ARP)
	{
		arp *ARP= new arp(data+ sizeof(ethernet_header));
		this->AddLayer(ARP);
	}
}

packet::~packet()
{
	LayerStack.clear();
}

void packet::AddLayer(layer * L)
{
	if (L->Get_length())
	{	
   		LayerStack.push_back(L);
   	}
}

int packet::GetLayer(int LayerCode ,layer ** RLayer)
{	
	for(vector<layer*>::iterator it = LayerStack.begin(); it != LayerStack.end(); it++)
    {	
		layer* l{*it};
		if (l->GetProtoID() == LayerCode)
		{
			*RLayer=l;
			return 0;
		}
	}
	return 1;
}


void packet::sendArpPacket(const char * Interface)
{   
    int sockfd;
	struct ifreq if_idx;
	struct ifreq if_mac;
	struct sockaddr_ll socket_address;
	char ifName[IFNAMSIZ];
	int BUF_SIZ=MAX_SIZE_IP_PACKET;
	/* Get interface name */

	strcpy(ifName, Interface);

	/* Open RAW socket to send on */
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
	    perror("socket");
	}

	/* Get the index of the interface to send on */
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");
	/* Get the MAC address of the interface to send on */
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
	    perror("SIOCGIFHWADDR");
    packetData buffer =GetPacketData();
	ethernet_header *eth= (ethernet_header *) buffer.data;
	
	socket_address.sll_family = AF_PACKET;
	socket_address.sll_ifindex = if_idx.ifr_ifindex; // index of interface
	socket_address.sll_protocol = htons(ETH_P_ARP);
	socket_address.sll_halen = ETH_ALEN; // length of destination mac address
	socket_address.sll_pkttype = PACKET_OTHERHOST;
	socket_address.sll_hatype = ARPHRD_ETHER;
	socket_address.sll_addr[0] = 0xff;
	socket_address.sll_addr[1] = 0xff;
	socket_address.sll_addr[2] = 0xff;
	socket_address.sll_addr[3] = 0xff;
	socket_address.sll_addr[4] = 0xff;
	socket_address.sll_addr[5] = 0xff;
	/* Destination MAC */
	socket_address.sll_addr[0] = eth->destinationMAC[0];
	socket_address.sll_addr[1] = eth->destinationMAC[1];
	socket_address.sll_addr[2] = eth->destinationMAC[2];
	socket_address.sll_addr[3] = eth->destinationMAC[3];
	socket_address.sll_addr[4] = eth->destinationMAC[4];
	socket_address.sll_addr[5] = eth->destinationMAC[5];
	 				
    if ( (  sendto(sockfd, (const char*) buffer.data,buffer.length, 0, (struct sockaddr*) &socket_address, sizeof (socket_address))) < 1) 
		{
			perror("send failed\n");
		}
}

int packet::sendPacket(const char * Interface)
{   

	struct ifreq if_idx;
	struct ifreq if_mac;
	struct sockaddr_ll socket_address;
	char ifName[IFNAMSIZ];
	int BUF_SIZ=MAX_SIZE_IP_PACKET;
	/* Get interface name */

	strcpy(ifName, Interface);
	if(sockId == -1)
	{
		printf("*********************** sockId = %d ***********************", sockId);
		/* Open RAW socket to send on */
		if ((sockId = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
		    Net_Abst_API_ERROR("socket creation failed");
			return -1;
		}
	}
	else
	{
		// Do nothing
	}

	/* Get the index of the interface to send on */
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockId, SIOCGIFINDEX, &if_idx) < 0)
	  {
		  Net_Abst_API_ERROR("SIOCGIFINDEX");
		  return -1;
	  }   
	/* Get the MAC address of the interface to send on */
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockId, SIOCGIFHWADDR, &if_mac) < 0)
	{
		Net_Abst_API_ERROR("SIOCGIFHWADDR");
		return -1;
	}
    packetData buffer =GetPacketData();
	ethernet_header *eth= (ethernet_header *) buffer.data;
	/* Index of the network device */
	socket_address.sll_ifindex = if_idx.ifr_ifindex;
	/* Address length*/
	socket_address.sll_halen = ETH_ALEN;
	/* Destination MAC */
	socket_address.sll_addr[0] = eth->destinationMAC[0];
	socket_address.sll_addr[1] = eth->destinationMAC[1];
	socket_address.sll_addr[2] = eth->destinationMAC[2];
	socket_address.sll_addr[3] = eth->destinationMAC[3];
	socket_address.sll_addr[4] = eth->destinationMAC[4];
	socket_address.sll_addr[5] = eth->destinationMAC[5];
	 				
    if ( (  sendto(sockId, (const char*) buffer.data,buffer.length, 0, (struct sockaddr*) &socket_address, sizeof (socket_address))) < 1) 
	{
			Net_Abst_API_ERROR("send failed\n");
			return -1;
	}
	return 0;
}

void packet::printPacket(void)
{
	ip* IP;
	icmp* ICMP;
	udp* UDP;
	tcp* TCP;
	payload* P;
	tcpoptions* TCPO;
	for(vector<layer*>::iterator it = LayerStack.begin(); it != LayerStack.end(); it++)
    {	ethernet* eth;

		layer* l{*it};
		switch (l->GetProtoID())
        { 
        	case (ETH_LAYER_Code):
			// Eth layer
			eth= (ethernet*) l;
				cout<< "******************Ethernet Layer*****************"<<endl;

				cout<< "Destination MAC: "<< eth->Get_DestinationMAC()<<endl;
    			cout<< "Source  MAC: "<<eth->Get_SourceMac()<<endl;
    			cout<< "Type : "<< eth->Get_Type()<<endl;


			break;
			// Ip layer
        	case (IP_LAYER_Code):
				IP= (ip*) l;
				cout<< "******************IP Layer*****************"<<endl;

				cout<<"Version : "<<IP->Get_Version()<<endl;
    			cout<<"IHL : "<<IP->Get_IHL()<<endl;
    			cout<<"TOS : "<<(int)IP->Get_TOS()<<endl;
    			cout<<"Total length :"<<IP->Get_TotalLength()<<endl;
    			cout<<"Id : "<<htons(IP->Get_Identification())<<endl;
    			cout<<"Offset : "<<IP->Get_Offset()<<endl;
    			cout<<"TTL : "<<(int)IP->Get_TTL()<<endl;
    			cout<<"Protocol : "<<(int)IP->Get_Protocol()<<endl;
    			cout<<"Checksum : "<<IP->Get_HeaderChecksum()<<endl;
    			cout<<"Source address : "<<IP->Get_SrcAddress()<<endl;
    			cout<<"Destination address : "<<IP->Get_DstAddress()<<endl;

			break;
			// ICMP layer
        	case (ICMP_LAYER_Code):
				ICMP= (icmp*) l;
				cout<< "******************ICMP Layer*****************"<<endl;
				cout<<"Type : "<<+ICMP->Get_type()<<endl;
    			cout<<"Code : "<<+ICMP->Get_code()<<endl;
    			cout<<"Checksum : "<<ICMP->Get_checksum()<<endl;
    			cout<<"ID : "<<ICMP->Get_identifier()<<endl;
    			cout<<"Sequence : "<<ICMP->Get_seuqence()<<endl;
			break;

			// UDP layer
        	case (UDP_LAYER_Code):
				UDP= (udp*) l;
				cout<< "******************UDP Layer*****************"<<endl;
				cout<<"SRC Port : "<<+UDP->Get_SrcPort()<<endl;
				cout<<"DST Port : "<<+UDP->Get_DstPort()<<endl;
				cout<<"Length : "<<+UDP->Get_Length()<<endl;
				cout<<"Checksum : "<<+UDP->Get_Checksum()<<endl;
			break;

			// TCP layer
        	case (TCP_LAYER_Code):
				TCP= (tcp*) l;
				cout<< "******************TCP Layer*****************"<<endl;
				cout<<"SRC Port : "<<TCP->Get_SrcPort()<<endl;
				cout<<"DST Port : "<<TCP->Get_DstPort()<<endl;
				cout<<"SEQ number : "<<TCP->Get_SeqNumber()<<endl;
				cout<<"ACK number : "<<TCP->Get_AckNumber()<<endl;
				cout<<"Reserver : "<<TCP->Get_Reserved()<<endl;
				cout<<"Header length : "<<( unsigned int)TCP->Get_Header_Length()<<endl;
				cout<<"Flag URG : "<<TCP->Get_FlagURG()<<endl;
				cout<<"Flag ACK : "<<TCP->Get_FlagACK()<<endl;
				cout<<"Flag PSH : "<<TCP->Get_FlagPSH()<<endl;
				cout<<"Flag RST : "<<TCP->Get_FlagRST()<<endl;
				cout<<"Flag SYN : "<<TCP->Get_FlagSYN()<<endl;
				cout<<"Flag FIN : "<<TCP->Get_FlagFIN()<<endl;
				cout<<"Windows : "<<+TCP->Get_WindowsSize()<<endl;
				cout<<"Checksum : "<<+TCP->Get_Checksum()<<endl;
				cout<<"Urg Pointer : "<<+TCP->Get_UrgPointer()<<endl;
			break;

			//TCP options
			case (TCP_OPTIONS_LAYER_Code):
				TCPO=(tcpoptions*) l;
				cout<< "******************TCP options Layer*****************"<<endl;
				uint8_t* options;
				options=TCPO->Get_Data();
				for (int i=0;i<TCPO->Get_length();i++)
					printf("%02x ",options[i]);
				printf("\n");
			break;
			// payload layer
			case (PAYLOAD_LAYER_Code):

				if  (l->Get_length() != 0) 
				{
				P= (payload*) l;
				cout<< "******************Payload Layer*****************"<<endl;
				cout<<"Length : "<<P->Get_Length()<<endl;
				cout<<"Data : "<<P->Get_Data()<<endl;
				}
			break;
			default:
				//misra
			break;
		}	

	}	
}

packetData packet::GetPacketData()
{
	packetData p;
	int totalLength=0;
    uint8_t  buffer[MAX_SIZE_IP_PACKET];
    for(vector<layer*>::iterator it = LayerStack.begin(); it != LayerStack.end(); it++)
    {
		layer* l{*it};
		if(l->Get_length() != 0)
		{
			
			if (l->GetProtoID()==108)
			{
				payload* p=(payload*)l;
				memcpy(buffer+totalLength, p->Get_header_data(),p->Get_length());
				totalLength+=p->Get_length();
			}
			else 
			{
				memcpy(buffer+totalLength, l->Get_header_data(),l->Get_length());
				totalLength+=l->Get_length();
			}
		}
	}	
	memcpy(p.data,buffer,totalLength);
	p.length=totalLength;
	return p;
}

packet packet::sendRecvPacket(const char * Interface)
{   
    int sockfd;
	struct ifreq if_idx;
	struct ifreq if_mac;
	struct sockaddr_ll socket_address;
	char ifName[IFNAMSIZ];
	int BUF_SIZ=MAX_SIZE_IP_PACKET;
	struct sockaddr_ll  from;
	int fromlen = sizeof(from);
	/* Get interface name */

	strcpy(ifName, Interface);

	/* Open RAW socket to send on */
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
	    perror("socket");
	}

	/* Get the index of the interface to send on */
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");
	/* Get the MAC address of the interface to send on */
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
	    perror("SIOCGIFHWADDR");
    int totalLength=0;
    char  buffer[MAX_SIZE_IP_PACKET];
	memset(buffer, 0, BUF_SIZ);
    for(vector<layer*>::iterator it = LayerStack.begin(); it != LayerStack.end(); it++)
    {
		layer* l{*it};
		memcpy(buffer+totalLength, l->Get_header_data(),l->Get_length());
		totalLength+=l->Get_length();


	}

	ethernet_header *eth= (ethernet_header *) buffer;

	/* Index of the network device */
	socket_address.sll_ifindex = if_idx.ifr_ifindex;
	/* Address length*/
	socket_address.sll_halen = ETH_ALEN;
	/* Destination MAC */
	socket_address.sll_addr[0] = eth->destinationMAC[0];
	socket_address.sll_addr[1] = eth->destinationMAC[1];
	socket_address.sll_addr[2] = eth->destinationMAC[2];
	socket_address.sll_addr[3] = eth->destinationMAC[3];
	socket_address.sll_addr[4] = eth->destinationMAC[4];
	socket_address.sll_addr[5] = eth->destinationMAC[5];
					
    if ( (  sendto(sockfd, buffer,totalLength, 0, (struct sockaddr*) &socket_address, sizeof (socket_address))) < 1) 
		{
			perror("send failed\n");
		}
	char buf[MAX_SIZE_IP_PACKET];
	
	cout<<"listener: Waiting to recvfrom...\n";
	for(time_t start=time(NULL);time(NULL)-start<1;)
    {
	int numbytes = recvfrom(sockfd, buf, BUF_SIZ, 0,0,0);
	packet PacketRecv(buf,numbytes);
	packet PacketS(buffer,totalLength);
	ethernet* eth = new ethernet; 
	if(PacketS.GetLayer(ETH_LAYER_Code,(layer**)&eth)==1)
    {
        Net_Abst_API_ERROR("Layer not found \n");
		packet NULL_Packet;
	    return NULL_Packet;
	}
    else 
    {
        //misra
    }
	

	if (eth->Get_Type()== htons(ETH_IP))
	{
	ip* IPS = new ip();
	ip* IPR = new ip();


	if(PacketS.GetLayer(IP_LAYER_Code,(layer**)&IPS)==1)
    {
        Net_Abst_API_ERROR("Layer not found \n");
	packet NULL_Packet;
	return NULL_Packet;
	}
    else 
    {
        //misra
    }

	if(PacketRecv.GetLayer(IP_LAYER_Code,(layer**)&IPR)==1)
    {
        Net_Abst_API_ERROR("Layer not found \n");
	packet NULL_Packet;
	return NULL_Packet;
	}
    else 
    {
        //misra
    }
	if((IPR->Get_SrcAddress()==IPS->Get_DstAddress()) && (IPR->Get_DstAddress()==IPS->Get_SrcAddress()) && (IPR->Get_Protocol()==IPS->Get_Protocol())  )
	{
		close(sockfd);
		return PacketRecv;
	}
	}
	}
	//cout<<"listener: got packet  bytes "<< numbytes<<endl;
	cout<<"cannot receive the packet"<<endl;
	close(sockfd);

	packet NULL_Packet;
	return NULL_Packet;
}

int packet::SetCorrectLength(void)
{
	//calculate length 
	uint16_t totalLength=0;
	for(vector<layer*>::iterator it = LayerStack.begin(); it != LayerStack.end(); it++)
    {
		layer* l{*it};
		if(l->GetProtoID()!=ETH_LAYER_Code)
		{
			totalLength+=l->Get_length();
		}
	}
	//Check Eth Type
	ethernet* eth = new ethernet();
	if( GetLayer(ETH_LAYER_Code,(layer**)&eth)==1)
    {
        Net_Abst_API_ERROR("Layer not found \n");
        return 1;
    }
    else 
    {
		//misra
    }

	// if type = ip
	if(eth->Get_Type()==htons(ETH_IP))
	{	// Set field ip total length
		ip* IP = new ip();
		if( GetLayer(IP_LAYER_Code,(layer**)&IP)==1)
    	{
        	Net_Abst_API_ERROR("Layer not found \n");
        	return 1;
    	}
    	else 
    	{
    	    //misra
    	}

		IP->Set_TotalLength(totalLength);
		totalLength-= IP->Get_length();
		// check IP proto 
		int IPproto = IP->Get_Protocol();

		// if udp
		if(IPproto== UDP_LAYER_Code)
		{
			udp* UDPH = new udp();
			if( GetLayer(UDP_LAYER_Code,(layer**)&UDPH)==1)
    		{
        		Net_Abst_API_ERROR("Layer not found \n");
        		return 1;
    		}
    		else 
    		{
    		    //misra
    		}
			UDPH->Set_Length(totalLength);
			return 0;
		}
		
	}

}

int packet::Set_CorrectChecksum(void) {
	ip* IP = new ip();
	if(this->GetLayer(IP_LAYER_Code,(layer**)&IP)==1)
    {
		Net_Abst_API_ERROR("Layer IP not found \n");
        return 1;
    }
    else 
    {
         //misra
    }
	IP->Set_HeaderChecksum(0x00);
	/* ip option correct checksum */
	if(IP->Get_IHL() == 15)
	{
		ipOption* icmpOptionHeader = new ipOption();
		if(this->GetLayer(IP_OPTION_LAYER_Code,(layer**)&icmpOptionHeader)==1)
		{
			Net_Abst_API_ERROR("Layer IP_OPTION not found \n");
			return 1;
		}
		else 
		{
			//misra
		}
		char data[sizeof(IpHeader)+sizeof(ip_option_header)];
		memcpy(data,IP->Get_header_data(),sizeof(IpHeader));
		memcpy(data+sizeof(IpHeader),icmpOptionHeader->Get_header_data(),sizeof(ip_option_header));
		IP->Set_HeaderChecksum(htons(in_cksum((unsigned short*)data,sizeof(IpHeader)+sizeof(ip_option_header))));
	}
	else
	{
		IP->Set_CorrectHeaderChecksum();
	}
	if(IP->Get_Protocol()==TCP_LAYER_Code)
	{
		IpHeader* pIph= (IpHeader*)IP->Get_header_data();
		uint16_t tcpLen = ntohs(pIph->TotalLength) - (pIph->IHL<<2);
		char *buffer=(char *)malloc((int)tcpLen+12);

		memcpy(buffer,pIph->SrcAddress,4);
		memcpy(buffer+4,pIph->DstAddress,4);
		memset(buffer+8,0,1);
		uint8_t TCPProto=6;
		uint16_t TCPL=htons(tcpLen);
		memcpy(buffer+9,&TCPProto,1);
		memcpy(buffer+10,&TCPL,2);
		int count=12;

		//the length
		tcp* TCPH = new tcp();
		if(this->GetLayer(TCP_LAYER_Code,(layer**)&TCPH)==0)
        {
            TCPH->Set_Checksum(0x00);
			memcpy(buffer+12,TCPH->Get_header_data(),TCPH->Get_length());
			count+=TCPH->Get_length();
        }
        else 
        {
            Net_Abst_API_ERROR("Layer TCP not found \n");
            return 1;
        }
		
		tcpoptions* TCP_OP;
		if (this->GetLayer(TCP_OPTIONS_LAYER_Code,(layer**)&TCP_OP)==0) 
		{
			memcpy(buffer + count, TCP_OP->Get_Data(), TCP_OP->Get_length());
			count+=TCP_OP->Get_length();
		}
		else 
		{
			//misra
		}
		payload* P;

		if (this->GetLayer(PAYLOAD_LAYER_Code,(layer**)&P)==0) 
		{
		
			
			if (P->Get_Length() != 0) {
				memcpy(buffer + count, P->Get_header_data(), P->Get_length());
				count+=P->Get_length();
			}
		TCPH->Set_Checksum(htons(in_cksum((unsigned short*)buffer,count)));
		}
		else 
		{

			TCPH->Set_Checksum(htons(in_cksum((unsigned short*)buffer,count)));
		}
		free(buffer);
		//delete(TCPH);
		//delete(IP);
		return 0;
	}
	else if (IP->Get_Protocol()==UDP_LAYER_Code)
	{
		IpHeader* pIph= (IpHeader*)IP->Get_header_data();
		uint16_t udpLen = ntohs(pIph->TotalLength) - (pIph->IHL<<2);
		char *buffer=(char *)malloc((int)udpLen+12);

		memcpy(buffer,pIph->SrcAddress,4);
		memcpy(buffer+4,pIph->DstAddress,4);
		memset(buffer+8,0,1);
		uint8_t UDPProto=17;
		uint16_t UDPL=htons(udpLen);
		memcpy(buffer+9,&UDPProto,1);
		memcpy(buffer+10,&UDPL,2);

		//the length
		
		udp* UDPH = new udp();
		if(this->GetLayer(UDP_LAYER_Code,(layer**)&UDPH)==1)
		{
			Net_Abst_API_ERROR("Layer UDP not found \n");
			return 1;
		}
		else 
		{
			//misra
		}
		
		payload* P;
		if (this->GetLayer(PAYLOAD_LAYER_Code,(layer**)&P)==0) {
			UDPH->Set_Checksum(0x00);
			memcpy(buffer+12,UDPH->Get_header_data(),UDPH->Get_length());
			if (P->Get_Length() != 0) {
				memcpy(buffer + 12 + UDPH->Get_length(), P->Get_header_data(), P->Get_length());
			}
		UDPH->Set_Checksum(htons(in_cksum((unsigned short*)buffer,12+UDPH->Get_length()+P->Get_length())));
		}
		else {
			UDPH->Set_Checksum(0x00);
			memcpy(buffer+12,UDPH->Get_header_data(),UDPH->Get_length());
			UDPH->Set_Checksum(htons(in_cksum((unsigned short*)buffer,12+UDPH->Get_length())));
		}
		free(buffer);
		//delete(UDPH);
		//delete(IP);
		return 0;
	}
	else if (IP->Get_Protocol()==ICMP_LAYER_Code)
	{
		icmp* ICMPH = new icmp();
		if(this->GetLayer(ICMP_LAYER_Code,(layer**)&ICMPH)==1)
		{	
			icmpTimestamp* ICMP_TIME_STAMP = new icmpTimestamp();
			
			if(this->GetLayer(ICMP_TIME_STAMP_LAYER_CODE,(layer**)&ICMP_TIME_STAMP)==1)
			{
				Net_Abst_API_ERROR("Layer ICMP not found \n");
				return 1;
			}
			else
			{
				//misra
			}
			ICMP_TIME_STAMP->Set_checksum(0);
			Net_Abst_API_ERROR("here we compute icmp timestamp checksum \n");
			ICMP_TIME_STAMP->Set_CorrectHeaderChecksum();
			return 0;
		}
		else 
		{
			//misra
		}
		ICMPH->Set_checksum(0);
		payload* P = new payload();
		if(this->GetLayer(PAYLOAD_LAYER_Code,(layer**)&P)==1)
		{
			Net_Abst_API_ERROR("Layer PAYLOAD not found \n");
			return 1;
		}
		else 
		{
			//misra
		}
		
		if (P->Get_Length() != 0) 
		{
			Net_Abst_API_ERROR("here we compute ICMP checksum \n");
			char *data = (char*) malloc (sizeof(icmp_header)+P->Get_Length());
    		memcpy(data, ICMPH->Get_header_data(), sizeof(icmp_header));
    		memcpy(data+sizeof(icmp_header), P->Get_header_data(), P->Get_Length());
			cout << "ICMP CHECKSUM: "  << in_cksum((unsigned short*)data,sizeof(icmp_header)+P->Get_Length()) << endl;
    		ICMPH->Set_checksum(htons(in_cksum((unsigned short*)data,sizeof(icmp_header)+P->Get_Length())));
			free(data);
		}
		else 
		{
			char *data = (char*) malloc (sizeof(icmp_header));
    		memcpy(data, ICMPH->Get_header_data(), sizeof(icmp_header));
    		ICMPH->Set_checksum(htons(in_cksum((unsigned short*)data,sizeof(icmp_header))));
			free(data);
		}
		//delete(ICMPH);
		//delete(P);
		//delete(IP);
		return 0;
	}
}



int packet::Verify_Checksum(void) 
{
	int checksumresult;
	ip* IP = new ip();
	if(this->GetLayer(IP_LAYER_Code,(layer**)&IP)==1)
    {
		Net_Abst_API_ERROR("Layer not found \n");
        return 1;
    }
    else 
    {
         //misra
    }
	
	if(IP->Get_Protocol()==TCP_LAYER_Code)
	{
		IpHeader* pIph= (IpHeader*)IP->Get_header_data();
		uint16_t tcpLen = ntohs(pIph->TotalLength) - (pIph->IHL<<2);
		char *buffer=(char *)malloc((int)tcpLen+12);

		memcpy(buffer,pIph->SrcAddress,4);
		memcpy(buffer+4,pIph->DstAddress,4);
		memset(buffer+8,0,1);
		uint8_t TCPProto=6;
		uint16_t TCPL=htons(tcpLen);
		memcpy(buffer+9,&TCPProto,1);
		memcpy(buffer+10,&TCPL,2);

		//the length
		tcp* TCPH = new tcp();
		if(this->GetLayer(TCP_LAYER_Code,(layer**)&TCPH)==1)
        {
            Net_Abst_API_ERROR("Layer not found \n");
            return 1;
        }
        else 
        {
            //misra
        }
		payload* P;
		if (this->GetLayer(PAYLOAD_LAYER_Code,(layer**)&P)==0) 
		{
			memcpy(buffer+12,TCPH->Get_header_data(),TCPH->Get_length());
			if (P->Get_Length() != 0) {
				memcpy(buffer + 12 + TCPH->Get_length(), P->Get_header_data(), P->Get_length());
		}
		checksumresult= htons(in_cksum((unsigned short*)buffer,12+TCPH->Get_length()+P->Get_length()));
		delete(buffer);
		return checksumresult;
		}
		else 
		{
			memcpy(buffer+12,TCPH->Get_header_data(),TCPH->Get_length());
			checksumresult = htons(in_cksum((unsigned short*)buffer,12+TCPH->Get_length()));
			delete(buffer);
			return checksumresult;
		}
	}
	else if (IP->Get_Protocol()==UDP_LAYER_Code)
	{
		IpHeader* pIph= (IpHeader*)IP->Get_header_data();
		uint16_t udpLen = ntohs(pIph->TotalLength) - (pIph->IHL<<2);
		char *buffer=(char *)malloc((int)udpLen+12);

		memcpy(buffer,pIph->SrcAddress,4);
		memcpy(buffer+4,pIph->DstAddress,4);
		memset(buffer+8,0,1);
		uint8_t UDPProto=17;
		uint16_t UDPL=htons(udpLen);
		memcpy(buffer+9,&UDPProto,1);
		memcpy(buffer+10,&UDPL,2);

		//the length
		udp* UDPH = new udp();
		if(this->GetLayer(UDP_LAYER_Code,(layer**)&UDPH)==1)
		{
			Net_Abst_API_ERROR("Layer not found \n");
			return 1;
		}
		else 
		{
			//misra
		}
		
		
		payload* P;
		if (this->GetLayer(PAYLOAD_LAYER_Code,(layer**)&P)==0) 
		{
			memcpy(buffer+12,UDPH->Get_header_data(),UDPH->Get_length());
			if (P->Get_Length() != 0) {
				memcpy(buffer + 12 + UDPH->Get_length(), P->Get_header_data(), P->Get_length());
			}
		checksumresult = htons(in_cksum((unsigned short*)buffer,12+UDPH->Get_length()+P->Get_length()));
		delete(buffer);
		return checksumresult;
		}
		else {
			memcpy(buffer+12,UDPH->Get_header_data(),UDPH->Get_length());
			checksumresult = htons(in_cksum((unsigned short*)buffer,12+UDPH->Get_length()));
			delete(buffer);
			return checksumresult;
		}
	}
	else if (IP->Get_Protocol()==ICMP_LAYER_Code)
	{
		icmp* ICMPH = new icmp();
		if(this->GetLayer(ICMP_LAYER_Code,(layer**)&ICMPH)==1)
		{
			Net_Abst_API_ERROR("Layer not found \n");
			return 1;
		}
		else 
		{
			//misra
		}
		
		payload* P;
		if (this->GetLayer(PAYLOAD_LAYER_Code,(layer**)&P)==0) 
		{
			char *data = (char*) malloc (sizeof(icmp_header)+P->Get_Length());
    		memcpy(data, ICMPH->Get_header_data(), sizeof(icmp_header));
    		memcpy(data+sizeof(icmp_header), P->Get_header_data(), P->Get_Length());
    		checksumresult = in_cksum((unsigned short*)data,sizeof(icmp_header)+P->Get_Length());
			delete(data);
			return checksumresult;

		}
		else 
		{
			char *data = (char*) malloc (sizeof(icmp_header));
    		memcpy(data, ICMPH->Get_header_data(), sizeof(icmp_header));
    		checksumresult = htons(in_cksum((unsigned short*)data,sizeof(icmp_header)));
			delete(data);
			return checksumresult;

		}

	}
	
}
