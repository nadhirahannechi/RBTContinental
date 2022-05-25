#include "IPv4_Common.h"

void stoiIP (const char *in_s, uint8 *out_ip) 
{
    char temp_c;
    uint8 octet_i = 0;
    uint8 octet_p = 0;;
    for (uint8 i = 0; i < strlen(in_s); i++ ) 
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
