#include <iostream>
#include <string>
#include <cstring>
#include "layer.h"
#include "tcpoptions.h"
#include "common.h"
#include <netinet/in.h>

using namespace std;

tcpoptions::tcpoptions():layer(TCP_OPTIONS_LAYER_Code,0)
{
    m_TCP_O.length=0;
    memset(m_TCP_O.data,0,1480);
}



tcpoptions::~tcpoptions()
{

}

int tcpoptions::Add_Option(uint8_t* data,uint8_t length)
{
    memcpy(m_TCP_O.data+ m_TCP_O.length,data,length);
    m_TCP_O.length+=length;
    Set_length(m_TCP_O.length);
    return 0; 
}

uint8_t * tcpoptions::Get_Data()
{
    return m_TCP_O.data;
}

uint32_t tcpoptions::Get_Length()
{
    return m_TCP_O.length;
}

const char * tcpoptions::Get_header_data()
{

    return (char *)m_TCP_O.data;

}
