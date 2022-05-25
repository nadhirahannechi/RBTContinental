
#include <iostream>
#include <string>
#include <sys/types.h>
#include <cstring>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <time.h>
#include "NetworkLib.h"


using namespace std;
extern char DUT_HW_Address[18];
              
sniffer::sniffer()
{
    /*m_Interface = 0;
    filter DelaultFilter;
    DelaultFilter.ETHType =0x800;
    DelaultFilter.arp_operation = 0;
    DelaultFilter.spa = "  ";
    DelaultFilter.tpa = " ";
    DelaultFilter.IPproto = 0;
    DelaultFilter.SrcIPAddress = "0.0.0.0";
    DelaultFilter.DstIPAddress = "0.0.0.0";
    DelaultFilter.SrcPort = 0;
    DelaultFilter.DstPort = 0;
    m_filter = DelaultFilter;

    m_Timeout = 0;
    m_packet = 0;*/
}

sniffer::~sniffer()
{

}
void sniffer::Set_Interface(char *  Interface)
{
    m_Interface= Interface;
}
void sniffer::Set_Filter(filter f)
{
    m_filter = f;
}
void sniffer::Set_Timeout(int timeout)
{
    m_Timeout = timeout;
}
char * sniffer::Get_Interface()
{
    return m_Interface;
}
filter sniffer::Get_Filter()
{
    return m_filter;
}
int sniffer::Get_Timeout()
{
    return m_Timeout;
}
void sniffer::Start_Sniff()
{   
    Net_Abst_API_TRACE_DEBUG("Start sniff\n");
    int sockfd;
	char ifName[IFNAMSIZ];
	int BUF_SIZ=MAX_SIZE_IP_PACKET;
    char buf[BUF_SIZ];
	/* Get interface name */

	strcpy(ifName, (const char*) m_Interface);

	/* Open RAW socket to send on */
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
	    perror("socket");
	}
    for(time_t start=time(NULL);time(NULL)-start<m_Timeout;)
    {   
        memset(buf,0,BUF_SIZ);
        int numbytes = recvfrom(sockfd, buf, BUF_SIZ, 0,0,0);
        packet p(buf,numbytes);

        ethernet* eth = new ethernet();
        if(p.GetLayer(ETH_LAYER_Code,(layer**)&eth)==1)
		{
			break;
		}
		else 
		{
			//misra
		}

        /* check eth type*/
        // if eth type  IP
        if(eth->Get_Type() == m_filter.ETHType)
        {
            if ((m_filter.ETHType == htons(ETH_IP)))
            {   
                ip* IPLayer = new ip() ;
                if(p.GetLayer(IP_LAYER_Code,(layer**)&IPLayer)==1)
		        {
			        break;
		        }
		        else 
		        {
		        	//misra
		        }
                /* check IP proto  SrcAddress DstAddress*/
                if ((IPLayer->Get_Protocol()== m_filter.IPproto)&&(IPLayer->Get_SrcAddress()== m_filter.SrcIPAddress)&&(IPLayer->Get_DstAddress()== m_filter.DstIPAddress))
                    {   /*if ip proto = UDP check SRC port and Dst port*/
                        if(m_filter.IPproto==UDP_LAYER_Code)
                        {   
                            udp* UDPLayer = new udp();
                            if(p.GetLayer(UDP_LAYER_Code,(layer**)&UDPLayer)==1)
                            {
                                break;
                            }
                            else 
                            {
                                //misra
                            }
                            if(UDPLayer->Get_SrcPort()== 68 && UDPLayer->Get_DstPort() == 67)
                            {   
                                dhcp* DHCPLayer = new dhcp();
                                if(p.GetLayer(DHCP_LAYER_Code,(layer**)&DHCPLayer)==1)
                                {   
                                    Net_Abst_API_TRACE_DEBUG("dhcp layer not found");
                                    break;
                                }
                                else 
                                {
                                    //misra
                                } 
                                Net_Abst_API_TRACE_DEBUG("dhcp received\n");
                                /* check src amc addr */
                                /*if(strcmp(eth->Get_SourceMac().c_str(),DUT_HW_Address) == 0)
                                {
                                    Net_Abst_API_TRACE_DEBUG("mac mrigla");   
                                }
                                else
                                {
                                    printf("mac mouch mrigla: %s != %s\n",eth->Get_SourceMac().c_str(),DUT_HW_Address);
                                }*/
                                m_packet = p;//packet(buf,numbytes);
                                break;
                            }
                            if((UDPLayer->Get_SrcPort()==m_filter.SrcPort)&&(UDPLayer->Get_DstPort()==m_filter.DstPort))
                            {
                                m_packet = p;//packet(buf,numbytes);
                                break;
                            }
                        }
                        /*if ip proto = TCP check src port and dst port*/
                        else if(m_filter.IPproto==TCP_LAYER_Code)
                        {
                            tcp* TCPLayer = new tcp();
                            if(p.GetLayer(TCP_LAYER_Code,(layer**)&TCPLayer)==1)
                            {
                                break;
                            }
                            else 
                            {
                                //misra
                            }  
                            if((TCPLayer->Get_SrcPort()==m_filter.SrcPort)&&(TCPLayer->Get_DstPort()==m_filter.DstPort))
                            {
                                m_packet = packet(buf,numbytes);
                                break;
                            }
                        }

                        /*if ip proto = ICMP */
                        else if (m_filter.IPproto== ICMP_LAYER_Code)
                        {   
                            m_packet = packet(buf,numbytes);
                            break;
                        }
                        /*
                        else if (m_filter.IPproto== ICMP_LAYER_Code)
                        {   
                            icmp* ICMPLayer = new icmp();
                            if(p.GetLayer(ICMP_LAYER_Code,(layer**)&ICMPLayer) == 1)
                            {
                                icmpTimestamp* ICMPTimestampLayer = new icmpTimestamp();
                                if(p.GetLayer(ICMP_TIME_STAMP_LAYER_CODE,(layer**)&ICMPTimestampLayer) == 1)
                                {
                                    break;
                                }
                                else
                                {
                                    m_packet = packet(buf,numbytes);
                                    break;
                                }
                            }
                            else
                            {
                                m_packet = packet(buf,numbytes);
                                break;
                            }
                        }
                        */
                    }
            }
            else if (m_filter.ETHType == htons(ETH_ARP))
            {   
                arp * arph = new arp();
                if(p.GetLayer(ARP_LAYER_Code,(layer**)&arph)==1)
                {
                    break;
                }
                else 
                {
                    //misra
                }   
                
                if((arph->Get_ar_op() == m_filter.arp_operation)&&(arph->Get_spa() == m_filter.spa)&&(arph->Get_tpa() == m_filter.tpa))
                {   
                    m_packet = packet(buf,numbytes);
                    break;
                }   
            } 
        }
    }
	close(sockfd);
}
packet sniffer::Get_Packet()
{
    return m_packet;
}