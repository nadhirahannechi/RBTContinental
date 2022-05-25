
#include "layer.h"
 
using namespace std;


layer::layer(int ProtoID, int length)
{
    m_protoID = ProtoID;
    m_length = length; 
}

layer::~layer()
{

}

void layer :: SetProtoID(int ProtoID)
{
    m_protoID = ProtoID;
}

void layer :: Set_length(int length)
{
    m_length = length; 
}

int layer :: GetProtoID()
{
    return m_protoID;
}

int layer :: Get_length()
{
    return m_length;
}
