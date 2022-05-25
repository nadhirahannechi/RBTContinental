#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TestabilityProtocol_Intern.h"


void extractIpAddress(uint8 *IPaddr,ip4addr *ipAddress)
{

    uint16 len=0;
    uint8 oct[4]={0},cnt=0,cnt1=0,i,buf[5];

    len= strlen(IPaddr);
    for(i=0;i<len;i++)
    {
        if(IPaddr[i]!='.'){
            buf[cnt++] =IPaddr[i];
            }
        if(IPaddr[i]=='.' || i==len-1){
            buf[cnt]='\0';
            cnt=0;
            oct[cnt1++]=atoi(buf);
            }
        }
    ipAddress->Data[0]=oct[0];
    ipAddress->Data[1]=oct[1];
    ipAddress->Data[2]=oct[2];
    ipAddress->Data[3]=oct[3];
}
