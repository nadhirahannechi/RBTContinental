#include"ip_option.h"

ipOption :: ipOption() : layer(IP_OPTION_LAYER_Code,sizeof(ip_option_header))
{
    m_ip_option_header.ipt_code = 68;
    m_ip_option_header.ipt_len = 10;
    m_ip_option_header.ipt_ptr = 9;
    m_ip_option_header.ipt_flg = 0;
    m_ip_option_header.ipt_oflw = 0;
    memset(m_ip_option_header.data,0,36);
}

ipOption :: ipOption(char * header) : layer(IP_OPTION_LAYER_Code,sizeof(ip_option_header))
{
   memcpy(&m_ip_option_header, header, sizeof(ip_option_header));
}

ipOption :: ~ipOption()
{

}

uint8_t ipOption::Get_ipt_code()
{
    return m_ip_option_header.ipt_code;
}

uint8_t ipOption::Get_ipt_len()
{
    return m_ip_option_header.ipt_len;
}

uint8_t ipOption::Get_ipt_ptr()
{
    return m_ip_option_header.ipt_ptr;
}

uint8_t ipOption::Get_ipt_flg()
{
    return m_ip_option_header.ipt_flg;
}

uint8_t ipOption::Get_ipt_oflw()
{
    return m_ip_option_header.ipt_oflw;
}

uint32_t * ipOption::Get_ipt_data()
{
    return m_ip_option_header.data;
}

void ipOption::Set_ipt_code(uint8_t code)
{
    m_ip_option_header.ipt_code = code;
}

void ipOption::Set_ipt_len(uint8_t len)
{
    m_ip_option_header.ipt_len = len;
}

void ipOption::Set_ipt_ptr(uint8_t ptr)
{
    m_ip_option_header.ipt_ptr = ptr;
}

void ipOption::Set_ipt_flg(uint8_t flg)
{
    m_ip_option_header.ipt_flg = flg;
}

void ipOption::Set_ipt_oflw(uint8_t oflw)
{
    m_ip_option_header.ipt_oflw = oflw;
}

void ipOption::Set_ipt_data(uint32_t* data)
{
    memcpy((char*)m_ip_option_header.data,(char*)data,36);
}

const char * ipOption::Get_header_data()
{
    return (char *)&m_ip_option_header;
}
