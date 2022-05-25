#ifndef IPOPTION_H
#define IPOPTION_H

#include "layer.h"
#include "common.h"
#include "payload.h"

typedef struct 
{
    u_int8_t ipt_code;			/* IPOPT_TS */
    u_int8_t ipt_len;			/* size of structure (variable) */
    u_int8_t ipt_ptr;			/* index of current entry */
    unsigned int ipt_flg:4;		/* flags, see below */
    unsigned int ipt_oflw:4;		/* overflow counter */
    u_int32_t data[9];

}ip_option_header;

class ipOption : public layer
{
    public:
    ipOption();
    ipOption( char *);
    ~ipOption();

    uint8_t Get_ipt_code();
    uint8_t Get_ipt_len();
    uint8_t Get_ipt_ptr();
    uint8_t Get_ipt_flg();
    uint8_t Get_ipt_oflw();
    uint32_t * Get_ipt_data();
 
    void Set_ipt_code(uint8_t);
    void Set_ipt_len(uint8_t);
    void Set_ipt_ptr(uint8_t);
    void Set_ipt_flg(uint8_t);
    void Set_ipt_oflw(uint8_t);
    void Set_ipt_data(uint32_t *);
    
    virtual const char * Get_header_data();
    private:
    ip_option_header m_ip_option_header;
};


#endif // IPOPTION_H