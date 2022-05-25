#ifndef TCPCONFIG_H
#define TCPCONFIG_H

typedef struct
{
    unsigned int DUT_Port;
    unsigned int Tester_Port;
    char DUT_IP[16];
    char TESTER_IP[16];
    unsigned int Maxcon;
    unsigned int ISN;

    unsigned int mss;
    unsigned int window;
    unsigned int nss;
    unsigned int ssz;
    unsigned int tmx;
    unsigned int tfn;
    unsigned int wnp;
    unsigned int uopt;
    unsigned int msl;
    unsigned int PORT1;
    unsigned int SEQ1;
} TCP_config_t;

extern TCP_config_t TCPConfig;

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************************************
*
*   FUNCTION NAME: Set_TCP_Config
*
***************************************************************************************************/
/**
* @brief
*  This function will configure TCP protocol.
*  
* @parameters
* @param[in] conf: The new configuration. 
*
* @return void
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
void Set_TCP_Config(TCP_config_t conf);

/***************************************************************************************************
*
*   FUNCTION NAME: Get_TCP_Config
*
***************************************************************************************************/
/**
* @brief
*  This function will return the configuration values.
*  
* @parameters
* @param[in] void 
*
* @return Current configutation
*
* @note
* -
*
* @warning
* -
*
***************************************************************************************************/
TCP_config_t Get_TCP_Config();

#ifdef __cplusplus
}
#endif

#endif