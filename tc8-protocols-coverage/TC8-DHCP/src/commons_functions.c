#include "commons_functions.h"

/***************************************************************************************************
*
*   FUNCTION NAME: extractIpAddress
*
***************************************************************************************************/
/**
* @brief
*  Extract the IP Address
*
* @par
* @param[in] IPaddr : Type uint8 : Input address
* @param[in] ipAddress : Type ip4addr : Address to initialize
*
* @return
* -
*
* @note
* -
*
* @warning
* -
*
**/
void extractIpAddress(uint8 *IPaddr, ip4addr *ipAddress)
{
    uint16 len = 0;
    uint8 oct[4] = {0},cnt = 0,cnt1 = 0, i, buf[5];

    len = strlen((char*)IPaddr);
    for(i=0; i<len; i++)
    {
        if(IPaddr[i]!='.')
        {
            buf[cnt++] =IPaddr[i];
        }
        if(IPaddr[i]=='.' || i==len-1)
        {
            buf[cnt]='\0';
            cnt=0;
            oct[cnt1++]=atoi((char*)buf);
        }
    }
    ipAddress->Data[0]=oct[0];
    ipAddress->Data[1]=oct[1];
    ipAddress->Data[2]=oct[2];
    ipAddress->Data[3]=oct[3];
}