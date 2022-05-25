#ifndef TCPOPTIONS_H
#define TCPOPTIONS_H

#include "layer.h"
#include "common.h"



typedef struct 
{
    uint8_t data[1480];
    uint8_t length;
} TCP_OPTION_STRUCT;

class tcpoptions : public layer
 {
    public:
        tcpoptions();
        ~tcpoptions();
        int Add_Option(uint8_t* data,uint8_t length);
        uint8_t * Get_Data();
        uint32_t Get_Length();
        virtual const char * Get_header_data();
    private:
        TCP_OPTION_STRUCT m_TCP_O;
 };


#endif
