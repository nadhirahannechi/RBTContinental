#ifndef LAYER_H
#define LAYER_H

#include <iostream>
#include <string>
#include <stdint.h>

class layer {
   public:
      layer(int,int);
      ~layer();
      virtual const char * Get_header_data()=0;
      int GetProtoID();
      int Get_length();

   protected:
      void SetProtoID(int);
      void Set_length(int);
      int m_protoID;
      int m_length;
};

#endif 
