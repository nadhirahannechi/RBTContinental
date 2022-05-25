#ifndef COMMON_H
#define COMMON_H

#include<stdint.h>
#include<string.h>
#include<ctype.h>

void stoiIP(const char *in_s, uint8_t *out_ip);
void lower_to_upper_mac(const char  mac_addr[], char MAC_ADDR[]);


#endif // COMMON_H