#include <iostream>
#include <string>
#include <cstring>
#include "layer.h"
#include "tcp.h"
#include "common.h"
#include <netinet/in.h>


using namespace std;
    
tcp::tcp():layer(TCP_LAYER_Code,sizeof(TCPHeader))
{
    memset(&m_tcpHeader, 0,sizeof(TCPHeader));
    m_tcpHeader.SrcPort=htons(20000);
    m_tcpHeader.DstPort=htons(20001);
    m_tcpHeader.SeqNumber=htons(1);
    m_tcpHeader.WindowsSize=htons(155);
    m_tcpHeader.HeaderLength=5;
    
}
    
tcp::tcp(char* header):layer(TCP_LAYER_Code,sizeof(TCPHeader))
{
    memcpy(&m_tcpHeader, header,sizeof(TCPHeader));

}

tcp::~tcp()
{

}
    
uint16_t tcp::Get_SrcPort()
{
    return htons(m_tcpHeader.SrcPort);
}

uint16_t tcp::Get_DstPort()
{
    return htons(m_tcpHeader.DstPort);

}
    
uint32_t tcp::Get_SeqNumber()
{
    return htonl(m_tcpHeader.SeqNumber);

}
    
uint32_t tcp::Get_AckNumber()
{
    return htonl(m_tcpHeader.AckNumber);

}

uint16_t tcp::Get_Reserved()
{
    return m_tcpHeader.Reserved;
}

int tcp::Get_Header_Length()
{
    return m_tcpHeader.HeaderLength;
}
    

int tcp::Get_FlagURG()
{
    return m_tcpHeader.FlagURG;
}

int tcp::Get_FlagACK()
{
    return m_tcpHeader.FlagACK;

}

int tcp::Get_FlagPSH()
{
    return m_tcpHeader.FlagPSH;

}

int tcp::Get_FlagRST()
{
    return m_tcpHeader.FlagRST;

}

int tcp::Get_FlagSYN()
{
    return m_tcpHeader.FlagSYN;

}

int tcp::Get_FlagFIN()
{
    return m_tcpHeader.FlagFIN;
}
    
uint16_t tcp::Get_WindowsSize()
{
    return htons(m_tcpHeader.WindowsSize);

}
    
uint16_t tcp::Get_Checksum()
{
    return htons(m_tcpHeader.Checksum);

}
    
uint16_t tcp::Get_UrgPointer()
{
    return htons(m_tcpHeader.UrgPointer);

}
    
void tcp::Set_SrcPort(uint16_t srcPort)
{
    m_tcpHeader.SrcPort = htons(srcPort);

}
    
void tcp::Set_DstPort(uint16_t DstPort)
{
    m_tcpHeader.DstPort = htons(DstPort);

}
    
void tcp::Set_SeqNumber(uint32_t Seqnumber)
{
    m_tcpHeader.SeqNumber = htonl(Seqnumber);

}
    
void tcp::Set_AckNumber(uint32_t AckNumber)
{
    m_tcpHeader.AckNumber = htonl(AckNumber);

}
    
void tcp::Set_Reserved(uint16_t Reserved)
{
    m_tcpHeader.Reserved = Reserved;

}

void tcp::Set_Header_Length(int HeaderLength)
{
    m_tcpHeader.HeaderLength=HeaderLength;

}



void tcp::Set_FlagURG(int urg)
{
    m_tcpHeader.FlagURG=urg;
}

void tcp::Set_FlagACK(int ack)
{
    m_tcpHeader.FlagACK=ack;

}

void tcp::Set_FlagPSH(int psh)
{
    m_tcpHeader.FlagPSH=psh;
}

void tcp::Set_FlagRST(int rst)
{
    m_tcpHeader.FlagRST=rst;

}

void tcp::Set_FlagSYN(int syn)
{
    m_tcpHeader.FlagSYN=syn;
}

void tcp::Set_FlagFIN(int fin)
{
    m_tcpHeader.FlagFIN=fin;
}

void tcp::Set_WindowsSize(uint16_t WindowsSize)
{
    m_tcpHeader.WindowsSize = htons(WindowsSize);

}
   
void tcp::Set_Checksum(uint16_t Checksum)
{
    m_tcpHeader.Checksum = htons(Checksum);
    
}
    
void tcp::Set_UrgPointer(uint16_t UrgPointer)
{
    m_tcpHeader.UrgPointer = htons(UrgPointer);

}

const char * tcp::Get_header_data()
{
    return (char *)&m_tcpHeader;

}
