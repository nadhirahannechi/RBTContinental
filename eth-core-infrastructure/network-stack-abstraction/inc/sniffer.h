#ifndef SNIFFER_H
#define SNIFFER_H
#include "common.h"
#include "packet.h"
class sniffer
{
    public:
        sniffer();
        ~sniffer();
        void Set_Interface(char *);
        void Set_Filter(filter );
        void Set_Timeout(int );
        char * Get_Interface();
        filter Get_Filter();
        int Get_Timeout();
        void Start_Sniff();
        packet Get_Packet();
    private:
        char * m_Interface;
        filter m_filter;
        int m_Timeout;
        packet m_packet;
};





#endif
