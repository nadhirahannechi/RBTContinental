#ifndef DHCP_H
#define DHCP_H

#include "layer.h"
#include "common.h"


typedef struct 
{   
        uint8_t  op;                   /* packet type */
        uint8_t  htype;                /* type of hardware address for this machine (Ethernet, etc) */
        uint8_t  hlen;                 /* length of hardware address (of this machine) */
        uint8_t  hops;                 /* hops */
        uint32_t xid;                  /* random transaction id number - chosen by this machine */
        uint16_t secs;                 /* seconds used in timing */
        uint16_t flags;                /* flags */
        uint8_t ciaddr[4];              /* IP address of this machine (if we already have one) */
        uint8_t yiaddr[4];              /* IP address of this machine (offered by the DHCP server) */
        uint8_t siaddr[4];              /* IP address of DHCP server */
        uint8_t giaddr[4];              /* IP address of DHCP relay */
        unsigned char chaddr [MAX_DHCP_CHADDR_LENGTH];      /* hardware address of this machine */
        char sname [MAX_DHCP_SNAME_LENGTH];    /* name of DHCP server */
        char file [MAX_DHCP_FILE_LENGTH];      /* boot file name (used for diskless booting?) */
	    unsigned char options[MAX_DHCP_OPTIONS_LENGTH];  /* options */

}dhcp_header;

class dhcp : public layer
{
    public:
    dhcp();
    dhcp( char *);
    ~dhcp();
    uint8_t Get_op();
    uint8_t Get_htype();
    uint8_t Get_hlen();
    uint8_t Get_hops();
    uint32_t Get_xid();
    uint16_t Get_secs();
    uint16_t Get_flags();
    std::string Get_ciaddr();
    std::string Get_yiaddr();
    std::string Get_siaddr();
    std::string Get_giaddr();
    std::string Get_chaddr();
    uint8_t* Get_sname();
    uint8_t* Get_file();
    uint8_t* Get_options();

    void Set_op(uint8_t);
    void Set_htype(uint8_t);
    void Set_hlen(uint8_t);
    void Set_hops(uint8_t);
    void Set_xid(uint32_t);
    void Set_secs(uint16_t);
    void Set_flags(uint16_t);
    void Set_ciaddr(std::string);
    void Set_yiaddr(std::string);
    void Set_siaddr(std::string);
    void Set_giaddr(std::string);
    void Set_chaddr(std::string);
    void Set_sname(char *, int );
    void Set_file(char *, int );
    void Set_option(char *, int );
    /* il reste les set for sname,file and options(add options function par exemple)*/
    virtual const char * Get_header_data();
    private:
    dhcp_header m_dhcp_header;
    uint16_t sname_ptr;
    uint16_t file_ptr;
    uint16_t option_ptr;
};
#endif