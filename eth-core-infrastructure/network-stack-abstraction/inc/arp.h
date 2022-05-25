#ifndef ARP_H
#define ARP_H

#include "layer.h"
#include "common.h"
#include "payload.h"

typedef struct 
{   
    uint16_t ar_hrd;		// Format of hardware address.
    uint16_t ar_pro;		// Format of protocol address.  
    uint8_t ar_hln;		// Length of hardware address.  
    uint8_t ar_pln;		// Length of protocol address.  
    uint16_t ar_op;		// ARP opcode (command).  
	uint8_t arp_sha[6];	// sender hardware address 
	uint8_t arp_spa[4];		// sender protocol address 
	uint8_t arp_tha[6];	// target hardware address 
	uint8_t arp_tpa[4];		// target protocol address 

}arp_header;

class arp : public layer
{
    public:
    arp();
    arp( char *);
    ~arp();
    uint16_t Get_ar_hdr();
    uint16_t Get_ar_pro();
    uint8_t Get_ar_hln();
    uint8_t Get_ar_pln();
    uint16_t Get_ar_op();
    std::string Get_sha();
    std::string Get_spa();
    std::string Get_tha();
    std::string Get_tpa();
    void Set_ar_hdr(uint16_t);
    void Set_ar_pro(uint16_t);
    void  Set_ar_hln(uint8_t);
    void  Set_ar_pln(uint8_t);
    void  Set_ar_op(uint16_t);
    void  Set_sha(std::string);
    void  Set_spa(std::string);
    void  Set_tha(std::string);
    void  Set_tpa(std::string);
    virtual const char * Get_header_data();
    private:
    arp_header m_arp_header;
};


#endif