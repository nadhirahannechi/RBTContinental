#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <iostream>
#include <string>
#include "layer.h"
#include"common.h"

typedef struct 
{   
    int length;
    char data[MAX_SIZE_DATA];
} Payload;
class payload : public layer 
{
    public:
        payload();
        payload(char *);
        payload(char *,int);
        ~payload();
        char * Get_Data();
        int Get_Length();
        void Set_Data(char*);
        void Setlength(int);
        virtual const char * Get_header_data();

    private:
        Payload p;
};
#endif