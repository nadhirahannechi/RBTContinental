#ifndef  IPV4COMMON_H
#define IPV4COMMON_H

#ifdef __cplusplus
extern "C"
{
#endif  /* #ifdef __cplusplus*/

/**************************************************************************************************/
/*                                                                                                */
/*                                           Defines                                              */
/*                                                                                                */
/**************************************************************************************************/

#include "TestabilityProtocolTypes.h"

void stoiIP (const char *in_s, uint8 *out_ip);

#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus*/
#endif
