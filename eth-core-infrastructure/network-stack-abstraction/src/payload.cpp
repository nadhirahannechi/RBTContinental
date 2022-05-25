#include "payload.h"
#include "common.h"
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;
        
        
payload:: payload() : layer(PAYLOAD_LAYER_Code,8)
{
    memset(p.data, 0, MAX_SIZE_DATA);
    strcpy(p.data, "Test API");
    p.length=8;
}
payload::payload(char* data):layer(PAYLOAD_LAYER_Code,strlen(data))
{
    p.length=strlen(data);
    memset(p.data, 0, MAX_SIZE_DATA);
    strcpy(p.data, data);
}

payload::payload(char* data, int length):layer(PAYLOAD_LAYER_Code,length)
{
    p.length=strlen(data);
    memset(p.data, 0, MAX_SIZE_DATA);
    memcpy(p.data, data,length);
}

payload::~payload()
{

}
char * payload::Get_Data()
{
    return p.data;
}
int payload::Get_Length()
{
    return p.length;
}
void payload::Set_Data(char* data)
{
   
    strcpy(p.data, data);
    Setlength(strlen(data));
    Set_length(p.length);
}
void payload::Setlength(int length)
{
   
    p.length = length;
    Set_length(p.length);
}

const char * payload::Get_header_data()
{
    return p.data;
}