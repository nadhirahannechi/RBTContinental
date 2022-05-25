#include "common.h"

void stoiIP(const char *in_s, uint8_t *out_ip) 
{
    char temp_c;
    uint8_t octet_i = 0;
    uint8_t octet_p = 0;
    for (uint8_t i = 0; i < strlen(in_s); i++ ) 
    {
        temp_c = in_s[i];
        if (temp_c == '.') 
        {
            out_ip[octet_p] = octet_i;
            octet_i = 0;
            octet_p += 1;
        } 
        else 
        {
            octet_i *= 10;
            octet_i += temp_c - '0';
        }
    }
    out_ip[octet_p] = octet_i;
}

void lower_to_upper_mac(const char  mac_addr[], char MAC_ADDR[])
{
    for(int i=0 ;i<17;++i)
    {   
        if(mac_addr[i]!=':')
        {   
            MAC_ADDR[i] = toupper(mac_addr[i]);
        }
        else
        {
            MAC_ADDR[i] = ':';
        }
    }
}
