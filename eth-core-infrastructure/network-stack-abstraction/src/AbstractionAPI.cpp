

#include "AbstractionAPI.h"
#include "NetworkLib.h"
#include <arpa/inet.h>
#include <stdlib.h>
#include <cstring>

char interface [25];
char DUT_HW_Address [18];
char DUT_IP_Address[16];
char Tester_HW_Address[18];  
char Tester_IP_Address[16];

using namespace std;


void Set_Network_Abstration_API_Config(Net_API_config_t configuration)
{   
    Net_Abst_API_TRACE_DEBUG("Set configuration\n");
    //set network abstration global values configutation
    strcpy(interface,configuration.S_interface);
    strcpy(DUT_HW_Address,configuration.S_DUT_HW_Address);
    strcpy(DUT_IP_Address,configuration.S_DUT_IP_Address);
    strcpy(Tester_HW_Address,configuration.S_Tester_HW_Address);  
    strcpy(Tester_IP_Address,configuration.S_Tester_IP_Address);
}

Net_API_config_t Get_Network_Abstration_API_Config()
{   
    Net_Abst_API_TRACE_DEBUG("Get configuration");
    //get network abstration global values configutation
    Net_API_config_t configuration;
    strcpy(configuration.S_interface,interface);
    strcpy(configuration.S_DUT_HW_Address,DUT_HW_Address);
    strcpy(configuration.S_DUT_IP_Address,DUT_IP_Address);
    strcpy(configuration.S_Tester_HW_Address,Tester_HW_Address);  
    strcpy(configuration.S_Tester_IP_Address,Tester_IP_Address);
    return configuration;

}

TCP_Packet CreateTCP()
{
    Net_Abst_API_TRACE_DEBUG("Create TCP Packet \n");
    // construct ethernet layer with default type (IP)
    ethernet eth;
    eth.Set_DestinationMAC(DUT_HW_Address);
    eth.Set_SourceMac(Tester_HW_Address);

    //Construct IP layer 
    ip IP;
    IP.Set_IHL(5);
    //IP.Set_TotalLength(htons(48));
    IP.Set_Protocol(6);
    IP.Set_Identification(0x22);
    IP.Set_DstAddress(DUT_IP_Address);
    IP.Set_SrcAddress(Tester_IP_Address);
    //IP.Set_CorrectHeaderChecksum();

    //Construct TCP layer 
    tcp TCP;

   

    // Construct Packet and add layers
    packet Packet;
    Packet.AddLayer(&eth);
    Packet.AddLayer(&IP);
    Packet.AddLayer(&TCP);
    Packet.SetCorrectLength();

    Packet.printPacket();

    Packet.Set_CorrectChecksum();


            

    //Get TCP packet
    packetData P_Data;
    P_Data=Packet.GetPacketData();

    TCP_Packet P_TCP;
    memcpy(P_TCP.data,P_Data.data,P_Data.length);
    P_TCP.length= P_Data.length;
    return P_TCP;

}

int SendTCP(TCP_Packet packetTCP)
{
    Net_Abst_API_TRACE_DEBUG("Send TCP Packet \n");
    packet p((char*)packetTCP.data,packetTCP.length);
    p.printPacket();
    return p.sendPacket(interface);
     
}

int EditTCPField( TCP_Packet* packetTCP, int proto_code, int field, void * value)
{
    packet p((char*)packetTCP->data,packetTCP->length);
    p.printPacket();
    ethernet * eth = new ethernet();
    ip* IPH = new ip();
    tcp * TCPH = new tcp();
    payload * payloadh = new payload();


    Net_Abst_API_TRACE_DEBUG("Edit TCP packet, Layer= %d, field= %d with ",proto_code,field );
    cout<< value<<endl;



    //edit Eth header
    switch (proto_code)
    { 
        case (ETHERNET):
       
            
            if(p.GetLayer(ETHERNET,(layer**)&eth)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
            switch(field)
            {
            case(ETH_destinationMAC):
                eth->Set_DestinationMAC( static_cast<char*> (value) );
                
            break;
            
            case(ETH_sourceMac):
                eth->Set_SourceMac( static_cast<char*> (value) );
                
            break;

            
            case(ETH_type):
                eth->Set_Type(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                
            break;

            default:
                Net_Abst_API_ERROR(": wrong field code \n");
                return 1;
            }

        break;

    

    //edit IP header
        case (IP):
    
            if(p.GetLayer(IP,(layer**)&IPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }

            switch(field)
            {
                case(IP_IHL):
                    IPH->Set_IHL(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;

                case(IP_Version):
                    IPH->Set_Version (static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;

                case(IP_Tos):
                    IPH->Set_TOS(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value))); 
                    
                break;

                case(IP_TotalLength):
                    IPH->Set_TotalLength(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    
                break;

                case(IP_Identification):
                    IPH->Set_Identification(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    
                break;

                case(IP_Offset):
                    IPH->Set_Offset(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    
                break;

                case(IP_Ttl):
                    IPH->Set_TTL(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;

                case(IP_Protocol):
                    IPH->Set_Protocol(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;
                
                case(IP_HeaderChecksum):
                    IPH->Set_HeaderChecksum(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;
                
                case(IP_SrcAddress):
                    IPH->Set_SrcAddress(static_cast<char*> (value) );
                    
                break;
                
                case(IP_DstAddress):
                    IPH->Set_DstAddress(static_cast<char*> (value) ) ;
                    
                break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                    return 1;
                
            
            }
        break;
    

    //edit tcp header
        case (TCP):
  
            if(p.GetLayer(TCP,(layer**)&TCPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }

        
            switch(field)
            {
                case(TCP_SrcPort):
                    TCPH->Set_SrcPort(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                    
                break; 

                case (TCP_DstPort):
                    TCPH->Set_DstPort(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))) ;
                    
                break;

                case (TCP_SeqNumber):
                    TCPH->Set_SeqNumber(static_cast<uint32_t>(reinterpret_cast<intptr_t>(value))) ;
                    
                break;
            
                case (TCP_AckNumber):
                    TCPH->Set_AckNumber(static_cast<uint32_t>(reinterpret_cast<intptr_t>(value))) ;
                    
                break;

                case (TCP_Reserved):
                    TCPH->Set_Reserved(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))) ;
                    
                break;
            
                case (TCP_HeaderLength):
                    TCPH->Set_Header_Length(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value))) ;
                    
                break;
            
                case (TCP_FlagFIN):
                    TCPH->Set_FlagFIN(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value))) ;
                    
                break;
            
                case (TCP_FlagSYN):
                    TCPH->Set_FlagSYN(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value))) ;
                    
                break;
            
                case (TCP_FlagRST):
                    TCPH->Set_FlagRST(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value))) ;
                    
                break;
            
                case (TCP_FlagPSH):
                    TCPH->Set_FlagPSH(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value))) ;
                    
                break;
            
                case (TCP_FlagACK):
                    TCPH->Set_FlagACK(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value))) ;
                    
                break;
            
                case (TCP_FlagURG):
                    TCPH->Set_FlagURG(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value))) ;
                    
                break;
            
                case (TCP_WindowsSize):
                    TCPH->Set_WindowsSize(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))) ;           
                    
                break;

                case (TCP_Checksum):
                    TCPH->Set_Checksum(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))) ;
                    
                break;
            
                case (TCP_UrgPointer):
                    TCPH->Set_UrgPointer(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))) ;
                    
                break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                    return 1;
                

            }
            
        break;

        //edit payload layer
        case (PAYLAOD):
    
            if (p.GetLayer(PAYLAOD,(layer**)&payloadh)==1) 
            {
                p.AddLayer(payloadh);
            }

            if(field==PAYLOAD_data)
            {


                // set new data
                payloadh->Set_Data(static_cast<char*> (value));
                // update ip total length
                if(p.GetLayer(IP,(layer**)&IPH)==1)
                {
                    Net_Abst_API_ERROR("Layer not found \n");
                    return 1;
                }
                else 
                {
                    //misra
                }
                    p.SetCorrectLength();


                }
            else if(field= PAYLOAD_length)
            {

                // get old length 
                int oldPayloadLength= payloadh->Get_length();

                // set new data
                payloadh->Setlength(static_cast<uint32_t>(reinterpret_cast<intptr_t>(value)));

                // update ip total length
                if(p.GetLayer(IP,(layer**)&IPH)==1)
                {
                    Net_Abst_API_ERROR("Layer not found \n");
                    return 1;
                }
                else 
                {
                    //misra
                }
                    p.SetCorrectLength();

                }
                else 
                {
                    Net_Abst_API_ERROR(": wrong field code \n");
                    return 1;

                }
        break;

        default:
            Net_Abst_API_ERROR(": wrong layer code \n");
            return 1;
        

    }


    Net_Abst_API_TRACE_DEBUG("field edited with success \n");
    p.printPacket();
    packetData P_Data;
    P_Data = p.GetPacketData();
    memcpy(packetTCP->data,P_Data.data,P_Data.length);
    packetTCP->length = P_Data.length;
    return 0;
}

intptr_t GetTCPField(TCP_Packet* packetTCP, int proto_code, int field)
{  
    Net_Abst_API_TRACE_DEBUG("Get TCP packet, Layer= %d, field= %d \n",proto_code,field);
    packet p((char*)packetTCP->data,packetTCP->length);
    char *dmac, *smac,*sadd,*dadd;
    ethernet * eth = new ethernet();
    ip* IPH = new ip();
    tcp * TCPH = new tcp();
    payload * payloadh = new payload;
    tcpoptions * tcpoptionsh;
    //edit Eth header
    switch (proto_code)
    { 
        case (ETHERNET):
       
             if(p.GetLayer(ETHERNET,(layer**)&eth)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
           else
           {
               return 0;
           }


            switch(field)
            {
            case(ETH_destinationMAC):

                dmac=(char*)malloc(strlen(eth->Get_DestinationMAC().c_str()));
                memcpy(dmac,eth->Get_DestinationMAC().c_str(),strlen(eth->Get_DestinationMAC().c_str()));
                return (intptr_t) dmac;
            break;
            
            case(ETH_sourceMac):
                smac=(char*)malloc(strlen(eth->Get_SourceMac().c_str()));
                memcpy(smac,eth->Get_SourceMac().c_str(),strlen(eth->Get_SourceMac().c_str()));
                return (intptr_t) smac;
            break;

            
            case(ETH_type):
                return  (intptr_t) eth->Get_Type() ;
            break;

            default:
                Net_Abst_API_ERROR(": wrong field code \n");
            }
            
        break;

    

    //edit IP header
        case (IP):
    
            if(p.GetLayer(IP,(layer**)&IPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }

            switch(field)
            {
                case(IP_IHL):
                    return (intptr_t )IPH->Get_IHL();
                break;

                case(IP_Version):
                    return (intptr_t) IPH->Get_Version ();
                break;

                case(IP_Tos):
                    return (intptr_t) IPH->Get_TOS();
                break;

                case(IP_TotalLength):
                    return (intptr_t)IPH->Get_TotalLength(); 
                break;

                case(IP_Identification):
                    return (intptr_t )IPH->Get_Identification(); 
                break;

                case(IP_Offset):
                    return (intptr_t)IPH->Get_Offset(); 
                break;

                case(IP_Ttl):
                   return (intptr_t)IPH->Get_TTL();
                break;

                case(IP_Protocol):
                    return (intptr_t)IPH->Get_Protocol();
                break;
                
                case(IP_HeaderChecksum):
                    return (intptr_t)IPH->Get_HeaderChecksum();
                break;
                
                case(IP_SrcAddress):
                    sadd=(char*)malloc(strlen(IPH->Get_SrcAddress().c_str()));
                    memcpy(sadd,IPH->Get_SrcAddress().c_str(),strlen(IPH->Get_SrcAddress().c_str()));
                    return (intptr_t) sadd;
                break;
                
                case(IP_DstAddress):
                    dadd=(char*)malloc(strlen(IPH->Get_DstAddress().c_str()));
                    memcpy(dadd,IPH->Get_DstAddress().c_str(),strlen(IPH->Get_DstAddress().c_str()));
                    return (intptr_t) dadd;
                break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                
            
            }
        break;
    

    //edit tcp header
        case (TCP):
  
            if(p.GetLayer(TCP,(layer**)&TCPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }

        
            switch(field)
            {
                case(TCP_SrcPort):
                    return  intptr_t( TCPH->Get_SrcPort()) ;
                break; 

                case (TCP_DstPort):
                    return  intptr_t( TCPH->Get_DstPort()) ;
                break;

                case (TCP_SeqNumber):
                    return  intptr_t( TCPH->Get_SeqNumber()) ;
                break;
            
                case (TCP_AckNumber):
                    return  intptr_t( TCPH->Get_AckNumber()) ;
                break;

                case (TCP_Reserved):
                    return  intptr_t( TCPH->Get_Reserved()) ;
                break;
            
                case (TCP_HeaderLength):
                    return  intptr_t( TCPH->Get_Header_Length()) ;
                break;
            
                case (TCP_FlagFIN):
                    return  intptr_t( TCPH->Get_FlagFIN()) ;
                break;
            
                case (TCP_FlagSYN):
                    return  intptr_t( TCPH->Get_FlagSYN()) ;
                break;
            
                case (TCP_FlagRST):
                    return  intptr_t( TCPH->Get_FlagRST()) ;
                break;
            
                case (TCP_FlagPSH):
                    return  intptr_t( TCPH->Get_FlagPSH()) ;
                break;
            
                case (TCP_FlagACK):
                    return  intptr_t( TCPH->Get_FlagACK()) ;
                break;
            
                case (TCP_FlagURG):
                    return  intptr_t( TCPH->Get_FlagURG()) ;
                break;
            
                case (TCP_WindowsSize):
                    return  intptr_t( TCPH->Get_WindowsSize()) ;           
                break;

                case (TCP_Checksum):
                    return  intptr_t( TCPH->Get_Checksum()) ;
                break;
            
                case (TCP_UrgPointer):
                    return  intptr_t( TCPH->Get_UrgPointer()) ;
                break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                

            }
            
        break;

        //get payload layer
        case (PAYLAOD):
    
            if(p.GetLayer(PAYLAOD,(layer**)&payloadh)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
            if(field==PAYLOAD_data)
            {
                return  intptr_t( payloadh->Get_Data()) ;

            }
            else if(field= PAYLOAD_length)
            {
                return (intptr_t) payloadh->Get_length();
            }
        break;


        default:
            Net_Abst_API_ERROR(": wrong layer code \n");
        

    }


}

TCP_Packet ListenTCP(Packet_filter f)
{
    Net_Abst_API_TRACE_DEBUG("Listen TCP for up to 1 s  \n");
    filter filter;
    filter.ETHType =htons(ETH_IP);
    filter.IPproto=TCP;
    strcpy(filter.SrcIPAddress,f.SrcAdd);
    strcpy(filter.DstIPAddress,f.dstAdd);
    filter.DstPort= f.Dstport;
    filter.SrcPort= f.Srcport;

    sniffer s;
    s.Set_Filter(filter);
    s.Set_Timeout(1);
    s.Set_Interface((char *)interface);
    s.Start_Sniff();
    packet p=s.Get_Packet();
    packetData P_Data;
    P_Data=p.GetPacketData();
    TCP_Packet tcp_p;
    memcpy(tcp_p.data,P_Data.data,P_Data.length);
    tcp_p.length=P_Data.length;
    if (tcp_p.length!=0)
    {
        Net_Abst_API_TRACE_DEBUG("Receive TCP packet\n");
        p.printPacket();
    }
    else 
    {
        Net_Abst_API_TRACE_DEBUG("cannot Receive TCP packet\n");
    }
    return tcp_p;
}

int TCP_Compute_checksum(TCP_Packet* tcp_p)
{
    Net_Abst_API_TRACE_DEBUG("Compute TCP checksum\n");

    packet p((char *)tcp_p->data,tcp_p->length);

    if( p.Set_CorrectChecksum()==0)
    {
        memcpy(tcp_p->data,p.GetPacketData().data,tcp_p->length);
        Net_Abst_API_TRACE_DEBUG("Compute TCP checksum Done\n");
           
        return 0;
    }
    else 
    {
        Net_Abst_API_ERROR("Cannot Compute TCP checksum \n");
        return 1;
    }
}

int TCP_Verify_Correct_checksum(TCP_Packet tcp_p)
{
    packet p((char *)tcp_p.data,tcp_p.length);
    if( p.Verify_Checksum()==0)
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}

int TCP_Add_Option(TCP_Packet* packetTCP,uint8_t* Data,uint8_t Length)
{
    Net_Abst_API_TRACE_DEBUG("Add TCP option \n");
    packet p((char*)packetTCP->data,packetTCP->length);
    tcpoptions* TCP_Option= new  tcpoptions();
    tcp* TCPH;
    ip* IPH;
    //add the tcp option
    if(p.GetLayer(TCP_OPTIONS,(layer**)&TCP_Option)==1)
    {
        TCP_Option->Add_Option(Data,Length);
        p.AddLayer(TCP_Option);

    }
    else
    {
        TCP_Option->Add_Option(Data,Length);

    }

    //update the total length(IP field) 
    if(p.GetLayer(IP,(layer**)&IPH)==1)
    {
        Net_Abst_API_ERROR("Layer IP not found \n");
        return 1;
    }
    else
    {
        uint16_t Old_Total_Length= IPH->Get_TotalLength();
        IPH->Set_TotalLength(Old_Total_Length+Length) ;
        

        
    }


    //update the tcp header length
    if(p.GetLayer(TCP,(layer**)&TCPH)==1)
    {
        Net_Abst_API_ERROR("Layer TCP not found \n");
        return 1;
    }
    else
    {
        TCPH->Set_Header_Length((TCP_Option->Get_length()/4)+5) ;

        
    }

    //udate TCP_PACKET
    Net_Abst_API_TRACE_DEBUG("TCP option added with success \n");
    packetData P_Data;
    P_Data = p.GetPacketData();
    memcpy(packetTCP->data,P_Data.data,P_Data.length);
    packetTCP->length = P_Data.length;

    return 0;



}

int Get_TCP_Option_Value(TCP_Packet packetTCP, uint8_t optionKind, uint8_t* value)
{
    Net_Abst_API_TRACE_DEBUG("Get TCP option value of kind %d \n" , optionKind);
    packet p((char*)packetTCP.data,packetTCP.length);
    tcpoptions* TCP_Option= new  tcpoptions();
    
    //get the tcp option layer
    if(p.GetLayer(TCP_OPTIONS,(layer**)&TCP_Option)==1)
    {
        Net_Abst_API_ERROR("Layer TCP options not found \n");
        return 1;
    }
    else
    {
        //misra
    }

    //get the exist tcp options
    uint8_t* tcp_options_list = (uint8_t*) malloc(TCP_Option->Get_Length()*sizeof(uint8_t));
    memset(tcp_options_list,0,TCP_Option->Get_Length());
    memcpy(tcp_options_list,TCP_Option->Get_Data(),TCP_Option->Get_Length());


    /***************** General forme of TCP options except kind = 0 or kind = 1 **************************
    ****** kind    : 1 byte ***********
    ****** Length  : 1 byte ***********   
    ****** Value   : Length Byte(s) ***********
    */

    /************** kind = 0 (End of Option List) or kind = 1 (No-Operation) *****************
    ****** kind    : 1 byte ***********/

    // iterate through the elements

    int count=0;

    while (!(tcp_options_list[count] == optionKind) && (count<TCP_Option->Get_Length()) ) 
    { 
        // if kind == 0 or 1 ----------->  count ++
        if ((tcp_options_list[count] == 0) || (tcp_options_list[count] == 1))
            count ++ ;

        // else count+=  Length
        else
            count+= tcp_options_list[count+1];
        

    }


    if (tcp_options_list[count] == optionKind)
    {   
        // if kind == 0 or 1 ----------->  count ++
        if ((tcp_options_list[count] == 0) || (tcp_options_list[count] == 1))
        {
            value[0]= tcp_options_list[count] ;
            delete(tcp_options_list);
            return 0;
        }

        // else count+=  Length
        else
        {
            int value_position=count+2;
            // total option length - 2 Bytes (1 byte : kind, 1 byte : length)
            int value_length= tcp_options_list[count+1] -2 ;
            memcpy(value,&(tcp_options_list[value_position]),value_length);
            delete(tcp_options_list);

            return 0;

        }
    }
    else
    {
        Net_Abst_API_ERROR("TCP option not found \n");
        delete(tcp_options_list);
        return 1;
    }

    

}

UDP_Packet CreateUDP()
{
    // construct ethernet layer with default type (IP)
    ethernet eth;
    eth.Set_DestinationMAC(DUT_HW_Address);
    eth.Set_SourceMac(Tester_HW_Address);

    //Construct IP layer 
    ip IP;
    IP.Set_IHL(5);
    IP.Set_TotalLength(36);
    IP.Set_Protocol(17);
    IP.Set_Identification(0x0022);
    IP.Set_DstAddress(DUT_IP_Address);
    IP.Set_SrcAddress(Tester_IP_Address);

    //Construct UDP layer 
    udp UDP;
    //Construct Payload layer
    payload p((char*)"TEST UDP");


    // Construct Packet and add layers
    packet Packet;
    Packet.AddLayer(&eth);
    Packet.AddLayer(&IP);
    Packet.AddLayer(&UDP);
    Packet.AddLayer(&p);

    //Get UDP packet
    packetData P_Data;
    P_Data=Packet.GetPacketData();

    UDP_Packet P_UDP;
    memcpy(P_UDP.data,P_Data.data,P_Data.length);
    P_UDP.length= P_Data.length;
    return P_UDP;

}

int SendUDP(UDP_Packet packetUDP)
{
    packet p((char*)packetUDP.data,packetUDP.length);
    return p.sendPacket(interface);
}

int EditUDPField( UDP_Packet *packetUDP, int proto_code, int field, void * value)
{
    Net_Abst_API_TRACE_DEBUG("Edit UDP packet, Layer= %d, field= %d with ",proto_code,field );
    cout<<field<<endl;
    packet p((char*)packetUDP->data,packetUDP->length);
    char *dmac, *smac,*sadd,*dadd;
    ethernet * eth = new ethernet();
    ip* IPH =new ip();
    udp * UDPH = new udp();
    payload * payloadh = new payload();

      //edit Eth header
    switch (proto_code)
    { 
        case (ETHERNET):
       
             if(p.GetLayer(ETHERNET,(layer**)&eth)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else
            {
                //misra
            }

            switch(field)
            {
            case(ETH_destinationMAC):
                eth->Set_DestinationMAC( static_cast<char*> (value) );
                
            break;
            
            case(ETH_sourceMac):
                eth->Set_SourceMac( static_cast<char*> (value) );
                
            break;

            
            case(ETH_type):
                eth->Set_Type(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                
            break;

            default:
                Net_Abst_API_ERROR(": wrong field code \n");
                return 1;
            }

        break;

    

    //edit IP header
        case (IP):
    
            if(p.GetLayer(IP,(layer**)&IPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }

            switch(field)
            {
                case(IP_IHL):
                    IPH->Set_IHL(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;

                case(IP_Version):
                    IPH->Set_Version (static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;

                case(IP_Tos):
                    IPH->Set_TOS(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value))); 
                    
                break;

                case(IP_TotalLength):
                    IPH->Set_TotalLength(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    
                break;

                case(IP_Identification):
                    IPH->Set_Identification(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    
                break;

                case(IP_Offset):
                    IPH->Set_Offset(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    
                break;

                case(IP_Ttl):
                    IPH->Set_TTL(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;

                case(IP_Protocol):
                    IPH->Set_Protocol(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;
                
                case(IP_HeaderChecksum):
                    IPH->Set_HeaderChecksum(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;
                
                case(IP_SrcAddress):
                    IPH->Set_SrcAddress(static_cast<char*> (value) );
                    
                break;
                
                case(IP_DstAddress):
                    IPH->Set_DstAddress(static_cast<char*> (value) ) ;
                    
                break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                    return 1;
                
            
            }
        break;
    

    //edit udp header
        case (UDP):
  
           if(p.GetLayer(UDP,(layer**)&UDPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }

        
            switch(field)
            {
                case(UDP_SrcPort):
                    UDPH->Set_SrcPort(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                    
                break; 

                case (UDP_DstPort):
                    UDPH->Set_DstPort(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;

                case (UDP_Length):
                    UDPH->Set_Length(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;
            
                case (UDP_Checksum):
                    UDPH->Set_Checksum(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                return 1;
                

            }
            
        break;

        //edit payload layer
        case (PAYLAOD):
    
            if(p.GetLayer(PAYLAOD,(layer**)&payloadh)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
            if (payloadh == NULL) {
                payloadh = new payload();
                p.AddLayer(payloadh);
            }

            if(field==PAYLOAD_data)
            {
                // get old length 
                uint16_t oldPayloadLength= payloadh->Get_length();

                // set new data
                payloadh->Set_Data(static_cast<char*> (value));

                // update ip total length
                if(p.GetLayer(IP,(layer**)&IPH)==1)
                {
                    Net_Abst_API_ERROR("Layer not found \n");
                    return 1;
                }
                else 
                {
                    //misra
                }
                p.SetCorrectLength();
            }
            else if(field= PAYLOAD_length)
            {

                // get old length 
                int oldPayloadLength= payloadh->Get_length();

                // set new data
                payloadh->Setlength(static_cast<uint32_t>(reinterpret_cast<intptr_t>(value)));

                // update ip total length
                if(p.GetLayer(IP,(layer**)&IPH)==1)
                {
                    Net_Abst_API_ERROR("Layer not found \n");
                    return 1;
                }
                else 
                {
                    //misra
                }
                int oldIPTotalLength= IPH->Get_TotalLength();
                int newPayloadLength= payloadh->Get_length();
                IPH->Set_TotalLength(htons((uint16_t) oldIPTotalLength-oldPayloadLength+newPayloadLength));
               
                
            }
            else 
            {
                Net_Abst_API_ERROR(": wrong field code \n");
                return 1;

            }
        break;
        default:
            Net_Abst_API_ERROR(": wrong layer code \n");
            return 1;
        

    }


    Net_Abst_API_TRACE_DEBUG("field edited with success \n");
    packetData P_Data;
    P_Data = p.GetPacketData();
    memcpy(packetUDP->data,P_Data.data,P_Data.length);
    packetUDP->length = P_Data.length;
    return 0;

}

intptr_t GetUDPField(UDP_Packet* packetUDP, int proto_code, int field)
{   
    Net_Abst_API_TRACE_DEBUG("Get UDP packet, Layer= %d, field= %d \n",proto_code,field);
    packet p((char*)packetUDP->data,packetUDP->length);
    char *dmac, *smac,*sadd,*dadd;
    ethernet * eth = new ethernet();
    ip* IPH =new ip();
    udp * UDPH = new udp();
    payload * payloadh = new payload();
    //edit Eth header
    switch (proto_code)
    { 
        case (ETHERNET):
       
            if(p.GetLayer(ETHERNET,(layer**)&eth)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else
            {
                 //misra

            }

            switch(field)
            {
            case(ETH_destinationMAC):

                dmac=(char*)malloc(strlen(eth->Get_DestinationMAC().c_str()));
                memcpy(dmac,eth->Get_DestinationMAC().c_str(),strlen(eth->Get_DestinationMAC().c_str()));
                return (intptr_t) dmac;
            break;
            
            case(ETH_sourceMac):
                smac=(char*)malloc(strlen(eth->Get_SourceMac().c_str()));
                memcpy(smac,eth->Get_SourceMac().c_str(),strlen(eth->Get_SourceMac().c_str()));
                return (intptr_t) smac;
            break;

            
            case(ETH_type):
                return  (intptr_t) eth->Get_Type() ;
            break;

            default:
                Net_Abst_API_ERROR(": wrong field code \n");
            }
            
        break;

    

    //edit IP header
        case (IP):
    
            if(p.GetLayer(IP,(layer**)&IPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }

            switch(field)
            {
                case(IP_IHL):
                    return (intptr_t )IPH->Get_IHL();
                break;

                case(IP_Version):
                    return (intptr_t) IPH->Get_Version ();
                break;

                case(IP_Tos):
                    return (intptr_t) IPH->Get_TOS();
                break;

                case(IP_TotalLength):
                    return (intptr_t)IPH->Get_TotalLength(); 
                break;

                case(IP_Identification):
                    return (intptr_t )IPH->Get_Identification(); 
                break;

                case(IP_Offset):
                    return (intptr_t)IPH->Get_Offset(); 
                break;

                case(IP_Ttl):
                   return (intptr_t)IPH->Get_TTL();
                break;

                case(IP_Protocol):
                    return (intptr_t)IPH->Get_Protocol();
                break;
                
                case(IP_HeaderChecksum):
                    return (intptr_t)IPH->Get_HeaderChecksum();
                break;
                
                case(IP_SrcAddress):
                    sadd=(char*)malloc(strlen(IPH->Get_SrcAddress().c_str()));
                    memcpy(sadd,IPH->Get_SrcAddress().c_str(),strlen(IPH->Get_SrcAddress().c_str()));
                    return (intptr_t) sadd;
                break;
                
                case(IP_DstAddress):
                    dadd=(char*)malloc(strlen(IPH->Get_DstAddress().c_str()));
                    memcpy(dadd,IPH->Get_DstAddress().c_str(),strlen(IPH->Get_DstAddress().c_str()));
                    return (intptr_t) dadd;
                break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                
            
            }
        break;
    

    //edit tcp header
        case (UDP):
  
            if(p.GetLayer(UDP,(layer**)&UDPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }

        
            switch(field)
            {
                case(UDP_SrcPort):
                    return (intptr_t) UDPH->Get_SrcPort();
                break; 

                case (UDP_DstPort):
                    return (intptr_t) UDPH->Get_DstPort();
                break;

                case (UDP_Length):
                    return (intptr_t) UDPH->Get_Length();
                break;
            
                case (UDP_Checksum):
                    return (intptr_t) UDPH->Get_Checksum();
                break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                

            }
            
        break;

        //edit payload layer
        case (PAYLAOD):
    
            if(p.GetLayer(PAYLAOD,(layer**)&payloadh)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
            if(field==PAYLOAD_data)
            {
                return  intptr_t( payloadh->Get_Data()) ;

            }
            else if(field= PAYLOAD_length)
            {
                return (intptr_t) payloadh->Get_length();
            }
        break;

        default:
            Net_Abst_API_ERROR(": wrong layer code \n");
        

    }
}

UDP_Packet ListenUDP(Packet_filter f, int timeout)
{
    Net_Abst_API_TRACE_DEBUG("Listen UDP for up to timeout s  \n");
    filter filter;
    filter.ETHType = htons(ETH_IP);
    filter.IPproto=UDP;
    strcpy(filter.SrcIPAddress,f.SrcAdd);
    strcpy(filter.DstIPAddress,f.dstAdd);
    filter.DstPort= f.Dstport;
    filter.SrcPort= f.Srcport;

    sniffer s;
    s.Set_Filter(filter);
    s.Set_Timeout(timeout);
    s.Set_Interface((char *)interface);
    s.Start_Sniff();
    packet p=s.Get_Packet();
    packetData P_Data;
    P_Data=p.GetPacketData();
    UDP_Packet udp_p;
    memcpy(udp_p.data,P_Data.data,P_Data.length);
    udp_p.length=P_Data.length;
    if (udp_p.length!=0)
    {
        Net_Abst_API_TRACE_DEBUG("Receive ICMP packet\n");
        p.printPacket();
    }
    else 
    {
        Net_Abst_API_TRACE_DEBUG("cannot Receive ICMP packet\n");
    }
    return udp_p;
}

int UDP_Compute_checksum(UDP_Packet* udp_p)
{
    Net_Abst_API_TRACE_DEBUG("Compute UDP checksum\n");
    packet p((char *)udp_p->data,udp_p->length);
    if( p.Set_CorrectChecksum()==0)
    {
        memcpy(udp_p->data,p.GetPacketData().data,p.GetPacketData().length);
        Net_Abst_API_TRACE_DEBUG("Compute UDP checksum Done\n");
        return 0;
    }
    else 
    {
        Net_Abst_API_ERROR("Cannot Compute UDP checksum\n");
        return 1;
    }
}

int UDP_Verify_Correct_checksum(UDP_Packet udp_p)
{
    packet p((char *)udp_p.data,udp_p.length);
    if( p.Verify_Checksum()==0)
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}

ICMP_Packet CreateICMP()
{
    // construct ethernet layer with default type (IP)
    ethernet eth;
    eth.Set_DestinationMAC(DUT_HW_Address);
    eth.Set_SourceMac(Tester_HW_Address);

    //Construct IP layer 
    ip IP;
    IP.Set_IHL(5);
    IP.Set_TotalLength(37);
    IP.Set_Protocol(1);
    IP.Set_Identification(0x22);
    IP.Set_DstAddress(DUT_IP_Address);
    IP.Set_SrcAddress(Tester_IP_Address);
    IP.Set_CorrectHeaderChecksum();

    //Construct ICMP layer 
    icmp ICMP;
    //Construct Payload layer
    payload p((char*)"TEST ICMP");

    // Set correct checksum in the ICMP layer
   // ICMP.updateChecksum(p.Get_Data());

    // Construct Packet and add layers
    packet Packet;
    Packet.AddLayer(&eth);
    Packet.AddLayer(&IP);
    Packet.AddLayer(&ICMP);
    Packet.AddLayer(&p);
    Packet.SetCorrectLength();
    //Packet.Set_CorrectChecksum();

    //Get ICMP packet
    packetData P_Data;
    P_Data=Packet.GetPacketData();

    ICMP_Packet P_ICMP;

    memcpy(P_ICMP.data,P_Data.data,P_Data.length);

    P_ICMP.length= P_Data.length;
    return P_ICMP;

}

int SendICMP(ICMP_Packet packetICMP)
{   
    packet p((char*)packetICMP.data,packetICMP.length);
    return p.sendPacket((char*)interface );
}

int EditICMPField( ICMP_Packet *packetICMP, int proto_code, int field, void * value)
{
    Net_Abst_API_TRACE_DEBUG("Edit ICMP packet, Layer= %d, field= %d with ",proto_code,field );
    cout<<field<<endl;
    packet p((char*)packetICMP->data,packetICMP->length);
    char *dmac, *smac,*sadd,*dadd;
    ethernet * eth = new ethernet();
    ip* IPH = new ip();
    icmp * ICMPH = new icmp();
    payload * payloadh = new payload();

    //edit Eth header
    switch (proto_code)
    { 
        case (ETHERNET):
       
            if(p.GetLayer(ETHERNET,(layer**)&eth)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else
            {
                //misra
            }
            switch(field)
            {
            case(ETH_destinationMAC):
                eth->Set_DestinationMAC( static_cast<char*> (value) );
                break;
            
            case(ETH_sourceMac):
                eth->Set_SourceMac( static_cast<char*> (value) );
                break;

            case(ETH_type):
                eth->Set_Type(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                break;

            default:
                Net_Abst_API_ERROR(": wrong field code \n");
                return 1;
            }

        break;

    //edit IP header
        case (IP):
    
            if(p.GetLayer(IP,(layer**)&IPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
            switch(field)
            {
                case(IP_IHL):
                    IPH->Set_IHL(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    break;

                case(IP_Version):
                    IPH->Set_Version (static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    break;

                case(IP_Tos):
                    IPH->Set_TOS(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value))); 
                    break;

                case(IP_TotalLength):
                    IPH->Set_TotalLength(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    break;

                case(IP_Identification):
                    IPH->Set_Identification(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    break;

                case(IP_Offset):
                    IPH->Set_Offset(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    break;

                case(IP_Ttl):
                    IPH->Set_TTL(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    break;

                case(IP_Protocol):
                    IPH->Set_Protocol(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    break;
                
                case(IP_HeaderChecksum):
                    IPH->Set_HeaderChecksum(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                    break;
                
                case(IP_SrcAddress):
                    IPH->Set_SrcAddress(static_cast<char*> (value) );
                    break;
                
                case(IP_DstAddress):
                    IPH->Set_DstAddress(static_cast<char*> (value) ) ;
                    break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                    return 1;            
            }
        break;
    
    //edit udp header
        case (ICMP):
  
           if(p.GetLayer(ICMP,(layer**)&ICMPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }        
            switch(field)
            {
                case(ICMP_type):
                    ICMPH->Set_type(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    
                break; 

                case (ICMP_code):
                    ICMPH->Set_code(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;

                case (ICMP_checksum):
                    ICMPH->Set_checksum(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                break;
            
                case (ICMP_identifier):
                    ICMPH->Set_identifer(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                break;

                case (ICMP_sequence):
                   ICMPH->Set_sequence(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                return 1;
            }
            
        break;

        //edit payload layer
        case (PAYLAOD):
    
            if(p.GetLayer(PAYLAOD,(layer**)&payloadh)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
            if (payloadh == NULL) {
                payloadh = new payload();
                p.AddLayer(payloadh);
            }

            if(field==PAYLOAD_data)
            {
                // get old length 
                uint16_t oldPayloadLength= payloadh->Get_length();

                // set new data
                payloadh->Set_Data(static_cast<char*> (value));

                // update ip total length
                if(p.GetLayer(IP,(layer**)&IPH)==1)
                {
                    Net_Abst_API_ERROR("Layer not found \n");
                    return 1;
                }
                else 
                {
                    //misra
                }
                p.SetCorrectLength();
            }
            else if(field= PAYLOAD_length)
            {

                // get old length 
                int oldPayloadLength= payloadh->Get_length();

                // set new data
                payloadh->Setlength(static_cast<uint32_t>(reinterpret_cast<intptr_t>(value)));

                // update ip total length
                if(p.GetLayer(IP,(layer**)&IPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
                int oldIPTotalLength= IPH->Get_TotalLength();
                int newPayloadLength= payloadh->Get_length();
                IPH->Set_TotalLength(htons((uint16_t) oldIPTotalLength-oldPayloadLength+newPayloadLength));               
            }
            else 
            {
                Net_Abst_API_ERROR(": wrong field code \n");
                return 1;
            }
        break;

        default:
            Net_Abst_API_ERROR(": wrong layer code \n");
            return 1;
    }
    Net_Abst_API_TRACE_DEBUG("field edited with success \n");
    packetData P_Data;
    P_Data = p.GetPacketData();
    memcpy(packetICMP->data,P_Data.data,P_Data.length);
    packetICMP->length = P_Data.length;
    return 0;
}

intptr_t GetICMPField(ICMP_Packet* packetICMP, int proto_code, int field)
{   
    Net_Abst_API_TRACE_DEBUG("Get ICMP packet, Layer= %d, field= %d \n",proto_code,field);
    packet p((char*)packetICMP->data,packetICMP->length);
    char *dmac, *smac,*sadd,*dadd;
    ethernet * eth = new ethernet();
    ip* IPH =new ip();
    icmp * ICMPH= new icmp();
    payload * payloadh = new payload();
    //edit Eth header
    switch (proto_code)
    { 
        case (ETHERNET):
       
            if(p.GetLayer(ETHERNET,(layer**)&eth)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else
            {
                //misra
            }

            switch(field)
            {
            case(ETH_destinationMAC):

                dmac=(char*)malloc(strlen(eth->Get_DestinationMAC().c_str()));
                memcpy(dmac,eth->Get_DestinationMAC().c_str(),strlen(eth->Get_DestinationMAC().c_str()));
                return (intptr_t) dmac;
            break;
            
            case(ETH_sourceMac):
                smac=(char*)malloc(strlen(eth->Get_SourceMac().c_str()));
                memcpy(smac,eth->Get_SourceMac().c_str(),strlen(eth->Get_SourceMac().c_str()));
                return (intptr_t) smac;
            break;

            
            case(ETH_type):
                return  (intptr_t) eth->Get_Type() ;
            break;

            default:
                Net_Abst_API_ERROR(": wrong field code \n");
            }
            
        break;

    

    //edit IP header
        case (IP):
    
            if(p.GetLayer(IP,(layer**)&IPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }

            switch(field)
            {
                case(IP_IHL):
                    return (intptr_t )IPH->Get_IHL();
                break;

                case(IP_Version):
                    return (intptr_t) IPH->Get_Version ();
                break;

                case(IP_Tos):
                    return (intptr_t) IPH->Get_TOS();
                break;

                case(IP_TotalLength):
                    return (intptr_t)IPH->Get_TotalLength(); 
                break;

                case(IP_Identification):
                    return (intptr_t )IPH->Get_Identification(); 
                break;

                case(IP_Offset):
                    return (intptr_t)IPH->Get_Offset(); 
                break;

                case(IP_Ttl):
                   return (intptr_t)IPH->Get_TTL();
                break;

                case(IP_Protocol):
                    return (intptr_t)IPH->Get_Protocol();
                break;
                
                case(IP_HeaderChecksum):
                    return (intptr_t)IPH->Get_HeaderChecksum();
                break;
                
                case(IP_SrcAddress):
                    sadd=(char*)malloc(strlen(IPH->Get_SrcAddress().c_str()));
                    memcpy(sadd,IPH->Get_SrcAddress().c_str(),strlen(IPH->Get_SrcAddress().c_str()));
                    return (intptr_t) sadd;
                break;
                
                case(IP_DstAddress):
                    dadd=(char*)malloc(strlen(IPH->Get_DstAddress().c_str()));
                    memcpy(dadd,IPH->Get_DstAddress().c_str(),strlen(IPH->Get_DstAddress().c_str()));
                    return (intptr_t) dadd;
                break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                
            
            }
        break;
    

    //edit ICMP header
        case (ICMP):
  
            if(p.GetLayer(ICMP,(layer**)&ICMPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }

            switch(field)
            {
                case(ICMP_type):
                    return (intptr_t) ICMPH->Get_type();
                break; 

                case (ICMP_code):
                    return (intptr_t) ICMPH->Get_code();
                break;

                case (ICMP_checksum):
                    return (intptr_t) ICMPH->Get_checksum();
                break;
            
                case (ICMP_identifier):
                    return (intptr_t) ICMPH->Get_identifier();
                break;
				
                case (ICMP_sequence):
                    return (intptr_t) ICMPH->Get_seuqence();
                break;

                case (ICMP_pointer):
                    return (intptr_t)ICMPH->Get_pointer();
                break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                

            }
            
        break;

        //edit payload layer
        case (PAYLAOD):
    
            if(p.GetLayer(PAYLAOD,(layer**)&payloadh)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
            if(field==PAYLOAD_data)
            {
                return  intptr_t( payloadh->Get_Data()) ;

            }
            else if(field= PAYLOAD_length)
            {
                return (intptr_t) payloadh->Get_length();
            }
        break;

        default:
            Net_Abst_API_ERROR(": wrong layer code \n");
        

    }
}

ICMP_Packet ListenICMP(Packet_filter f, int timeout)
{
    Net_Abst_API_TRACE_DEBUG("Listen ICMP for up to timeout s  \n");
    filter filter;
    filter.ETHType =htons(ETH_IP);
    filter.IPproto=ICMP;
    strcpy(filter.SrcIPAddress,f.SrcAdd);
    strcpy(filter.DstIPAddress,f.dstAdd);

    sniffer s;
    s.Set_Filter(filter);
    s.Set_Timeout(timeout);
    s.Set_Interface((char *)interface);
    s.Start_Sniff();
    packet p=s.Get_Packet();
    packetData P_Data;
    P_Data=p.GetPacketData();
    ICMP_Packet icmp_p;
    memcpy(icmp_p.data,P_Data.data,P_Data.length);
    icmp_p.length=P_Data.length;
    if (icmp_p.length!=0)
    {
        Net_Abst_API_TRACE_DEBUG("Receive ICMP packet\n");
        p.printPacket();
    }
    else 
    {
        Net_Abst_API_TRACE_DEBUG("cannot Receive ICMP packet\n");
    }
    return icmp_p;
}

int ICMP_Compute_checksum(ICMP_Packet* icmp_p)
{
    Net_Abst_API_TRACE_DEBUG("Compute ICMP checksum\n");
    packet p((char *)icmp_p->data,icmp_p->length);
    if( p.Set_CorrectChecksum()==0)
    {
        memcpy(icmp_p->data,p.GetPacketData().data,p.GetPacketData().length);
        Net_Abst_API_TRACE_DEBUG("Compute ICMP checksum Done\n");
        return 0;
    }
    else 
    {
        Net_Abst_API_ERROR("Cannot Compute ICMP checksum\n");
        return 1;
    }
}

ARP_Packet CreateARP()
{
    // construct ethernet layer with  type ARP
    ethernet eth;
    eth.Set_DestinationMAC("ff:ff:ff:ff:ff:ff");
    eth.Set_SourceMac(Tester_HW_Address);
    eth.Set_Type(htons(ETH_ARP));
    //Construct arp layer 
    arp arph;
    arph.Set_spa(Tester_IP_Address);
    arph.Set_sha(Tester_HW_Address);
    arph.Set_tpa(DUT_IP_Address);
    
    // Construct Packet and add layers
    packet Packet;

    Packet.AddLayer(&eth);
    Packet.AddLayer(&arph);

    //Get ICMP packet
    packetData P_Data;
    P_Data=Packet.GetPacketData();

    ARP_Packet P_ARP;
    memcpy(P_ARP.data,P_Data.data,P_Data.length);
    
    P_ARP.length= P_Data.length;
    return P_ARP;
}

int SendARP(ARP_Packet packetARP)
{
    packet p((char*)packetARP.data,packetARP.length);
    p.sendArpPacket(interface);
    return 1;
}


int EditARPField( ARP_Packet *packetARP, int proto_code, int field, void * value)
{

    packet p((char*)packetARP->data,packetARP->length);
    ethernet * eth = new ethernet();
    arp* arph = new arp();
    //edit Eth header
    if (proto_code == ETHERNET)
    {   
        if(p.GetLayer(ETHERNET,(layer**)&eth)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {   
                //misra 
            }
        if (field== ETH_destinationMAC)
        {
            eth->Set_DestinationMAC( static_cast<char*> (value) );
        }
        else if (field== ETH_sourceMac)
        {
            eth->Set_SourceMac( static_cast<char*> (value) );

        }
        else if (field== ETH_type )
        {
            eth->Set_Type(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
        }

    }

    else if(proto_code ==  ARP)
    {
        if(p.GetLayer(ARP,(layer**)&arph)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {   
                //misra
            }
        if(field == hardware_address_format)
        {
            arph->Set_ar_hdr(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field == protocol_address_format)
        {
            arph->Set_ar_pro(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field  == Length_hardware_address)
        {
            arph->Set_ar_hln(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field  == Length_protocol_address)
        {
            arph->Set_ar_pln(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field  == opcode)
        {
            arph->Set_ar_op(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field  == sender_hardware_address)
        {
            arph->Set_sha(static_cast<char*> (value));
        }
        else if(field  == sender_protocol_address)
        {
            arph->Set_spa(static_cast<char*> (value));
        }

        else if(field  == target_hardware_address)
        {   
            arph->Set_tha(static_cast<char*> (value));
        }

        else if(field  == target_protocol_address)
        {
            arph->Set_tpa(static_cast<char*> (value));
        }
    }
    
    packetData P_Data;
    P_Data = p.GetPacketData();
    memcpy(packetARP->data,P_Data.data,P_Data.length);
    packetARP->length = P_Data.length;
    //p.printPacket();
}

intptr_t GetARPField(ARP_Packet* packetARP, int proto_code, int field)
{   
    packet p((char*)packetARP->data,packetARP->length);
    ethernet * eth = new ethernet();
    arp* arph = new arp();
    //edit Eth header
    if (proto_code == ETHERNET)
    {   
        if(p.GetLayer(ETHERNET,(layer**)&eth)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
        if (field== ETH_destinationMAC)
        {   
            char * dmac=(char*)malloc(strlen(eth->Get_DestinationMAC().c_str()));
            memcpy(dmac,eth->Get_DestinationMAC().c_str(),strlen(eth->Get_DestinationMAC().c_str()));
            return (intptr_t) dmac;

           
        }
        else if (field== ETH_sourceMac)
        {
            char * smac=(char*)malloc(strlen(eth->Get_SourceMac().c_str()));
            memcpy(smac,eth->Get_SourceMac().c_str(),strlen(eth->Get_SourceMac().c_str()));
            return (intptr_t) smac;


        }
        else if (field== ETH_type )
        {
            return  (intptr_t) eth->Get_Type();

        }

    }

    else if (proto_code == ARP)
    {
        if(p.GetLayer(ARP,(layer**)&arph)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
        if(field == hardware_address_format)
        {
            return (intptr_t) arph->Get_ar_hdr();
        }
        else if(field == protocol_address_format)
        {
            return (intptr_t) arph->Get_ar_pro();
        }
        else if(field  == Length_hardware_address)
        {
            return (intptr_t) arph->Get_ar_hln();
        }
        else if(field  == Length_protocol_address)
        {
            return (intptr_t) arph->Get_ar_pln();
        }
        else if(field  == opcode)
        {
            return (intptr_t) arph->Get_ar_op();
        }
        else if(field  == sender_hardware_address)
        {
            char * smac=(char*)malloc(strlen(arph->Get_sha().c_str()));
            memcpy(smac,arph->Get_sha().c_str(),strlen(arph->Get_sha().c_str()));
            return (intptr_t) smac;
        }
        else if(field  == sender_protocol_address)
        {
            char * smac=(char*)malloc(strlen(arph->Get_spa().c_str()));
            memcpy(smac,arph->Get_spa().c_str(),strlen(arph->Get_spa().c_str()));
            return (intptr_t) smac;
        }

        else if(field  == target_hardware_address)
        {
            char * smac=(char*)malloc(strlen(arph->Get_tha().c_str()));
            memcpy(smac,arph->Get_tha().c_str(),strlen(arph->Get_tha().c_str()));
            return (intptr_t) smac;
        }

        else if(field  == target_protocol_address)
        {
            char * smac=(char*)malloc(strlen(arph->Get_tpa().c_str()));
            memcpy(smac,arph->Get_tpa().c_str(),strlen(arph->Get_tpa().c_str()));
            return (intptr_t) smac;
        }
    }
}

ARP_Packet ListenARP(Packet_filter f, int timeout)
{
    filter filter;
    filter.ETHType = htons(ETH_ARP);
    filter.arp_operation = f.arp_operation;
    strcpy(filter.spa,f.spa);
    strcpy(filter.tpa,f.tpa);
    sniffer s;
    s.Set_Filter(filter);
    s.Set_Timeout(timeout);
    s.Set_Interface((char *)interface);
    s.Start_Sniff();
    packet p=s.Get_Packet();
    packetData P_Data;
    P_Data=p.GetPacketData();
    ARP_Packet arp_p;
    memcpy(arp_p.data,P_Data.data,P_Data.length);
    arp_p.length=P_Data.length;
    return arp_p;
}

DHCP_Packet CreateDHCP()
{
    // construct ethernet layer with default type (IP)
    ethernet eth;
    eth.Set_DestinationMAC("ff:ff:ff:ff:ff:ff");
    eth.Set_SourceMac(Tester_HW_Address);

    //Construct IP layer 
    ip IP;
    IP.Set_Protocol(UDP);
    IP.Set_DstAddress("255.255.255.255");
    IP.Set_SrcAddress(Tester_IP_Address);
    IP.Set_TotalLength(576);

    //Construct UDP layer 
    udp UDP;
    UDP.Set_SrcPort(DHCP_SERVER_PORT);
    UDP.Set_DstPort(DHCP_CLIENT_PORT);
    UDP.Set_Length(556);

    //Construct Payload layer
    dhcp DHCP;
    //DHCP.Set_yiaddr(DUT_IP_Address);
    DHCP.Set_siaddr(Tester_IP_Address);

    // Construct Packet and add layers
    packet Packet;
    Packet.AddLayer(&eth);
    Packet.AddLayer(&IP);
    Packet.AddLayer(&UDP);
    Packet.AddLayer(&DHCP);

    //Get ICMP packet
    packetData P_Data;
    P_Data=Packet.GetPacketData();

    DHCP_Packet P_DHCP;
    memcpy(P_DHCP.data,P_Data.data,P_Data.length);
    P_DHCP.length= P_Data.length;
    return P_DHCP;
}

int SendDHCP(DHCP_Packet packetDHCP)
{
    packet p((char*)packetDHCP.data,packetDHCP.length);
    return p.sendPacket((char*)interface);
}

int EditDHCPField( DHCP_Packet* packetDHCP, int proto_code, int field, void * value)
{
    Net_Abst_API_TRACE_DEBUG("Edit DHCP packet, Layer= %d, field= %d with ",proto_code,field );
    packet p((char*)packetDHCP->data,packetDHCP->length);
    ethernet * eth = new ethernet();
    ip* IPH =new ip();
    udp *udph = new udp();
    dhcp *dhcph= new dhcp;
    //edit Eth header
    if (proto_code == ETHERNET)
    {   
        if(p.GetLayer(ETHERNET,(layer**)&eth)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
        if (field== ETH_destinationMAC)
        {
            eth->Set_DestinationMAC( static_cast<char*> (value) );
        }
        else if (field== ETH_sourceMac)
        {
            eth->Set_SourceMac( static_cast<char*> (value) );

        }
        else if (field== ETH_type )
        {
            eth->Set_Type(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
        }
    }
    //edit IP header
    else if (proto_code == IP)
    {
        if(p.GetLayer(IP,(layer**)&IPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }

        if (field==IP_IHL)
        {
            IPH->Set_IHL(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field==IP_Version)
        {
           IPH->Set_Version (static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field==IP_TOS)
        {
          IPH->Set_TOS(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value))); 
        }
        else if(field==IP_TotalLength)
        {
           IPH->Set_TotalLength(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
        }
        else if(field==IP_Identification)
        {
           IPH->Set_Identification(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
        }
        else if(field==IP_Offset)
        {
           IPH->Set_Offset(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
        }
        else if(field==IP_Ttl)
        {
           IPH->Set_TTL(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field==IP_Protocol)
        {
            IPH->Set_Protocol(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field==IP_HeaderChecksum)
        {
            IPH->Set_HeaderChecksum(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field==IP_SrcAddress)
        {
            IPH->Set_SrcAddress(static_cast<char*> (value) );
        }
        else if(field==IP_DstAddress)
        {
           IPH->Set_DstAddress(static_cast<char*> (value) ) ;
        }
    }
    //edit UDP header
    else if(proto_code == UDP)
    {
        if(p.GetLayer(UDP,(layer**)&udph)==1)
            {
                Net_Abst_API_ERROR("Layer UDP not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
        if(field == UDP_SrcPort)
        {
            udph->Set_SrcPort(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field == UDP_DstPort)
        {
            udph->Set_DstPort(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field == UDP_Length)
        {
            udph->Set_Length(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field == UDP_Checksum)
        {
            udph->Set_Checksum(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
        }

    }
	
    //edit DHCP header
	else if (proto_code = DHCP)
    {   
        Net_Abst_API_TRACE_DEBUG("proto code = DHCP\n");
        dhcp * dhcph;
        if(p.GetLayer(DHCP,(layer**)&dhcph)==1)
        {
            Net_Abst_API_ERROR("Layer DHCP not found \n");
            return 1;
        }
        else 
        {
            //misra
        }
        if(field ==packet_type )
        {
            dhcph->Set_op(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field == type_of_hardware_address )
        {
            dhcph->Set_htype(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field == length_of_hardware_address )
        {
            dhcph->Set_hlen(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field == hops )
        {
            dhcph->Set_hops(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field == random_transaction_id_number)
        {   
            dhcph->Set_xid(static_cast<uint32_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field == seconds_used_in_timing )
        {
            dhcph->Set_secs(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field == flags )
        {
            dhcph->Set_flags(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
        }
        else if(field == ciaddr )
        {
            dhcph->Set_ciaddr(static_cast<char*> (value));
        }
        else if(field == yiaddr )
        {
            dhcph->Set_yiaddr(static_cast<char*> (value));
        }
        else if(field == siaddr )
        {
            dhcph->Set_siaddr(static_cast<char*> (value));
        }
        else if(field == giaddr )
        {
            dhcph->Set_giaddr(static_cast<char*> (value));
        }
        else if(field == chaddr)
        {
            dhcph->Set_chaddr(static_cast<char*> (value));
        }
        else if(field == sname)
        {
            dhcp_value *val = (dhcp_value *)value;
            dhcph->Set_sname((char*)val->data,val->len);
        }
        else if(field == file)
        {
            dhcp_value *val = (dhcp_value *)value;
            dhcph->Set_file((char*)val->data,val->len);
        }
        else if(field == options)
        {   
            dhcp_value *val = (dhcp_value *)value;
            dhcph->Set_option((char*)val->data,val->len);
        }

    }
    packetData P_Data;
    P_Data = p.GetPacketData();
    memcpy(packetDHCP->data,P_Data.data,P_Data.length);
    packetDHCP->length = P_Data.length;
    return 0;
}


intptr_t GetDHCPField(DHCP_Packet* packetDHCP, int proto_code, int field)
{   
    Net_Abst_API_TRACE_DEBUG("Get DHCP packet, Layer= %d, field= %d \n",proto_code,field);
    packet p((char*)packetDHCP->data, packetDHCP->length);
    ethernet * eth = new ethernet();
    ip* IPH =new ip();
    udp *udph = new udp();
    dhcp *dhcph = new dhcp;
    //edit Eth header
    if (proto_code == ETHERNET)
    {   
        if(p.GetLayer(ETHERNET,(layer**)&eth)==1)
        {
            Net_Abst_API_ERROR("Layer not found \n");
            return 1;
        }
        else 
        {
            //misra
        }
        if (field== ETH_destinationMAC)
        {   
            char * dmac=(char*)malloc(strlen(eth->Get_DestinationMAC().c_str()));
            memcpy(dmac,eth->Get_DestinationMAC().c_str(),strlen(eth->Get_DestinationMAC().c_str()));
            return (intptr_t) dmac;    
        }
        else if (field== ETH_sourceMac)
        {
            char * smac=(char*)malloc(strlen(eth->Get_SourceMac().c_str()));
            memcpy(smac,eth->Get_SourceMac().c_str(),strlen(eth->Get_SourceMac().c_str()));
            return (intptr_t) smac;
        }
        else if (field== ETH_type )
        {
            return  (intptr_t) eth->Get_Type();
        }
    }

    else if (proto_code == IP)
    {
        if(p.GetLayer(IP,(layer**)&IPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }

        if (field==IP_IHL)
        {
            return (intptr_t )IPH->Get_IHL();
        }
        else if(field==IP_Version)
        {
            return (intptr_t) IPH->Get_Version ();
        }
        else if(field==IP_TOS)
        {
          return (intptr_t) IPH->Get_TOS();
        }
        else if(field==IP_TotalLength)
        {
           return (intptr_t)IPH->Get_TotalLength();
        }
        else if(field==IP_Identification)
        {
           return (intptr_t )IPH->Get_Identification();
        }
        else if(field==IP_Offset)
        {
            return (intptr_t)IPH->Get_Offset();
        }
        else if(field==IP_Ttl)
        {
           return (intptr_t)IPH->Get_TTL();
        }
        else if(field==IP_Protocol)
        {
            return (intptr_t)IPH->Get_Protocol();

        }
        else if(field==IP_HeaderChecksum)
        {
            return (intptr_t)IPH->Get_HeaderChecksum();

        }
        else if(field==IP_SrcAddress)
        {
            char * sadd=(char*)malloc(strlen(IPH->Get_SrcAddress().c_str()));
            memcpy(sadd,IPH->Get_SrcAddress().c_str(),strlen(IPH->Get_SrcAddress().c_str()));
            return (intptr_t) sadd;
            
        }
        else if(field==IP_DstAddress)
        {
            char * dadd=(char*)malloc(strlen(IPH->Get_DstAddress().c_str()));
            memcpy(dadd,IPH->Get_DstAddress().c_str(),strlen(IPH->Get_DstAddress().c_str()));
            return (intptr_t) dadd;
        }
    }
    else if(proto_code == UDP)
    {
        if(p.GetLayer(UDP,(layer**)&udph)==1)
            {
                Net_Abst_API_ERROR("Layer UDP not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
        if(field == UDP_SrcPort)
        {
            return (intptr_t )udph->Get_SrcPort();
        }
        else if(field == UDP_DstPort)
        {
            return (intptr_t )udph->Get_DstPort();
        }
        else if(field == UDP_Length)
        {
            return (intptr_t )udph->Get_length();
        }
        else if(field == UDP_Checksum)
        {
            return (intptr_t )udph->Get_Checksum();
        }
    }
    else if(proto_code == DHCP)
    {
        if(p.GetLayer(DHCP,(layer**)&dhcph)==1)
        {
            Net_Abst_API_ERROR("Layer DHCP not found \n");
            return 1;
        }
        else 
        {
            //misra
        }
        if(field ==packet_type )
        {
            return (intptr_t )dhcph->Get_op();
        }
        else if(field == type_of_hardware_address )
        {
            return (intptr_t )dhcph->Get_htype();
        }
        else if(field == length_of_hardware_address )
        {
            return (intptr_t )dhcph->Get_hlen();
        }
        else if(field == hops )
        {
            return (intptr_t )dhcph->Get_hops();
        }
        else if(field == random_transaction_id_number )
        {
            return (intptr_t )dhcph->Get_xid();
        }
        else if(field == seconds_used_in_timing )
        {
            return (intptr_t )dhcph->Get_secs();
        }
        else if(field == flags )
        {
            return (intptr_t )dhcph->Get_flags();
        }
        else if(field == ciaddr )
        {
            char * sadd=(char*)malloc(strlen(dhcph->Get_ciaddr().c_str()));
            memcpy(sadd,dhcph->Get_ciaddr().c_str(),strlen(dhcph->Get_ciaddr().c_str()));
            return (intptr_t) sadd;
        }
        else if(field == yiaddr )
        {
            char * sadd=(char*)malloc(strlen(dhcph->Get_yiaddr().c_str()));
            memcpy(sadd,dhcph->Get_yiaddr().c_str(),strlen(dhcph->Get_yiaddr().c_str()));
            return (intptr_t) sadd;
        }
        else if(field == siaddr )
        {
            char * sadd=(char*)malloc(strlen(dhcph->Get_siaddr().c_str()));
            memcpy(sadd,dhcph->Get_siaddr().c_str(),strlen(dhcph->Get_siaddr().c_str()));
            return (intptr_t) sadd;
        }
        else if(field == giaddr )
        {
            char * sadd=(char*)malloc(strlen(dhcph->Get_giaddr().c_str()));
            memcpy(sadd,dhcph->Get_giaddr().c_str(),strlen(dhcph->Get_giaddr().c_str()));
            return (intptr_t) sadd;
        }
        else if(field == chaddr)
        {
            char * sadd=(char*)malloc(strlen(dhcph->Get_chaddr().c_str()));
            memcpy(sadd,dhcph->Get_chaddr().c_str(),strlen(dhcph->Get_chaddr().c_str()));
            return (intptr_t) sadd;
        }
        else if(field == sname)
        {
            char* snamech = (char*)malloc(64);
            memcpy(snamech,(char*)dhcph->Get_sname(),64); 
            return (intptr_t)snamech;
        }
        else if(field == file)
        {
            char* filech = (char*)malloc(128);
            memcpy(filech,(char*)dhcph->Get_file(),128); 
            return (intptr_t)filech;
        }
        else if(field == options)
        {
            char* optionch = (char*)malloc(312);
            memcpy(optionch,(char*)dhcph->Get_options(),312); 
            return (intptr_t)optionch;
        }
    }
}

DHCP_Packet ListenDHCP(Packet_filter f, int timeout)
{   
    Net_Abst_API_TRACE_DEBUG("Start ListenDHCP\n");
    filter filter;
    filter.ETHType = htons(ETH_IP);
    filter.IPproto = UDP;
    strcpy(filter.SrcIPAddress,f.SrcAdd);
    strcpy(filter.DstIPAddress,f.dstAdd);
    filter.DstPort = DHCP_SERVER_PORT;
    filter.SrcPort = DHCP_CLIENT_PORT;
    sniffer s;
    s.Set_Filter(filter);
    s.Set_Timeout(timeout);
    s.Set_Interface((char *)interface);
    s.Start_Sniff();
    packet p=s.Get_Packet();
    packetData P_Data;
    P_Data=p.GetPacketData();
    DHCP_Packet dhcp_p;
    memcpy(dhcp_p.data,P_Data.data,P_Data.length);
    dhcp_p.length=P_Data.length;
    return dhcp_p;
}

IP_Packet CreateIP()
{
    Net_Abst_API_TRACE_DEBUG("Create IP Packet \n");
    // construct ethernet layer with default type (IP)
    ethernet eth;
    eth.Set_DestinationMAC(DUT_HW_Address);
    eth.Set_SourceMac(Tester_HW_Address);

    //Construct IP layer 
    ip IP;
    IP.Set_IHL(5);
    //IP.Set_TotalLength(htons(48));
    IP.Set_Identification(0x22);
    IP.Set_DstAddress(DUT_IP_Address);
    IP.Set_SrcAddress(Tester_IP_Address);
    //IP.Set_CorrectHeaderChecksum();

    //Construct Payload layer
    payload p;
    p.Setlength(0);
   

    // Construct Packet and add layers
    packet Packet;
    Packet.AddLayer(&eth);
    Packet.AddLayer(&IP);
    Packet.AddLayer(&p);
    Packet.SetCorrectLength();
    //Packet.Set_CorrectChecksum();

    //Get IP packet
    packetData P_Data;
    P_Data=Packet.GetPacketData();

    IP_Packet P_IP;
    memcpy(P_IP.data,P_Data.data,P_Data.length);
    P_IP.length= P_Data.length;
    return P_IP;

}

int SendIP(IP_Packet packetIP)
{
    Net_Abst_API_TRACE_DEBUG("Send IP Packet \n");
    packet p((char*)packetIP.data,packetIP.length);
    p.printPacket();
    return p.sendPacket(interface);
     
}

int EditIPField( IP_Packet* packetIP, int proto_code, int field, void * value)
{
    
    packet p((char*)packetIP->data,packetIP->length);
    ethernet * eth = new ethernet();
    ip* IPH =new ip();
    payload * payloadh = new payload();


    Net_Abst_API_TRACE_DEBUG("Edit IP packet, Layer= %d, field= %d with ",proto_code,field );
    cout<< value<<endl;



    //edit Eth header
    switch (proto_code)
    { 
        case (ETHERNET):
       
            if(p.GetLayer(ETHERNET,(layer**)&eth)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
           else
           {
               return 0;
           }

            switch(field)
            {
            case(ETH_destinationMAC):
                eth->Set_DestinationMAC( static_cast<char*> (value) );
                
            break;
            
            case(ETH_sourceMac):
                eth->Set_SourceMac( static_cast<char*> (value) );
                
            break;

            
            case(ETH_type):
                eth->Set_Type(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                
            break;

            default:
                Net_Abst_API_ERROR(": wrong field code \n");
                return 1;
            }

        break;

    

    //edit IP header
        case (IP):
    
            if(p.GetLayer(IP,(layer**)&IPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }

            switch(field)
            {
                case(IP_IHL):
                    IPH->Set_IHL(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;

                case(IP_Version):
                    IPH->Set_Version (static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;

                case(IP_Tos):
                    IPH->Set_TOS(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value))); 
                    
                break;

                case(IP_TotalLength):
                    IPH->Set_TotalLength(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    
                break;

                case(IP_Identification):
                    IPH->Set_Identification(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    
                break;

                case(IP_Offset):
                    IPH->Set_Offset(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    
                break;

                case(IP_Ttl):
                    IPH->Set_TTL(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;

                case(IP_Protocol):
                    IPH->Set_Protocol(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;
                
                case(IP_HeaderChecksum):
                    IPH->Set_HeaderChecksum(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;
                
                case(IP_SrcAddress):
                    IPH->Set_SrcAddress(static_cast<char*> (value) );
                    
                break;
                
                case(IP_DstAddress):
                    IPH->Set_DstAddress(static_cast<char*> (value) ) ;
                    
                break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                    return 1;
                
            
            }
        break;


        //edit payload layer
        case (PAYLAOD):
    
            if (p.GetLayer(PAYLAOD,(layer**)&payloadh)==1) {
                payloadh = new payload();
                p.AddLayer(payloadh);
            }

            if(field==PAYLOAD_data)
            {
                // get old length 
                uint16_t oldPayloadLength= payloadh->Get_length();

                // set new data
                payloadh->Set_Data(static_cast<char*> (value));

                // update ip total length
                if(p.GetLayer(IP,(layer**)&IPH)==1)
                {
                    Net_Abst_API_ERROR("Layer not found \n");
                    return 1;
                }
                else 
                {
                    //misra
                }
                    p.SetCorrectLength();


                }
            else if(field= PAYLOAD_length)
            {

                // get old length 
                int oldPayloadLength= payloadh->Get_length();

                // set new data
                payloadh->Setlength(static_cast<uint32_t>(reinterpret_cast<intptr_t>(value)));

                // update ip total length
                if(p.GetLayer(IP,(layer**)&IPH)==1)
                {
                    Net_Abst_API_ERROR("Layer not found \n");
                    return 1;
                }
                else 
                {
                    //misra
                }
                    p.SetCorrectLength();

                }
                else 
                {
                    Net_Abst_API_ERROR(": wrong field code \n");
                    return 1;

                }
        break;

        default:
            Net_Abst_API_ERROR(": wrong layer code \n");
            return 1;
        

    }


    Net_Abst_API_TRACE_DEBUG("field edited with success \n");
    packetData P_Data;
    P_Data = p.GetPacketData();
    memcpy(packetIP->data,P_Data.data,P_Data.length);
    packetIP->length = P_Data.length;
    return 0;
}

intptr_t GetIPField(IP_Packet* packetIP, int proto_code, int field)
{  
    Net_Abst_API_TRACE_DEBUG("Get IP packet, Layer= %d, field= %d \n",proto_code,field);
    packet p((char*)packetIP->data,packetIP->length);
    char *dmac, *smac,*sadd,*dadd;
    ethernet * eth = new ethernet();
    ip* IPH =new ip();
    payload * payloadh = new payload();
    //edit Eth header
    switch (proto_code)
    { 
        case (ETHERNET):
       
             if(p.GetLayer(ETHERNET,(layer**)&eth)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
           else
           {
               return 0;
           }


            switch(field)
            {
            case(ETH_destinationMAC):

                dmac=(char*)malloc(strlen(eth->Get_DestinationMAC().c_str()));
                memcpy(dmac,eth->Get_DestinationMAC().c_str(),strlen(eth->Get_DestinationMAC().c_str()));
                return (intptr_t) dmac;
            break;
            
            case(ETH_sourceMac):
                smac=(char*)malloc(strlen(eth->Get_SourceMac().c_str()));
                memcpy(smac,eth->Get_SourceMac().c_str(),strlen(eth->Get_SourceMac().c_str()));
                return (intptr_t) smac;
            break;

            
            case(ETH_type):
                return  (intptr_t) eth->Get_Type() ;
            break;

            default:
                Net_Abst_API_ERROR(": wrong field code \n");
            }
            
        break;

    

    //edit IP header
        case (IP):
    
            if(p.GetLayer(IP,(layer**)&IPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }

            switch(field)
            {
                case(IP_IHL):
                    return (intptr_t )IPH->Get_IHL();
                break;

                case(IP_Version):
                    return (intptr_t) IPH->Get_Version ();
                break;

                case(IP_Tos):
                    return (intptr_t) IPH->Get_TOS();
                break;

                case(IP_TotalLength):
                    return (intptr_t)IPH->Get_TotalLength(); 
                break;

                case(IP_Identification):
                    return (intptr_t )IPH->Get_Identification(); 
                break;

                case(IP_Offset):
                    return (intptr_t)IPH->Get_Offset(); 
                break;

                case(IP_Ttl):
                   return (intptr_t)IPH->Get_TTL();
                break;

                case(IP_Protocol):
                    return (intptr_t)IPH->Get_Protocol();
                break;
                
                case(IP_HeaderChecksum):
                    return (intptr_t)IPH->Get_HeaderChecksum();
                break;
                
                case(IP_SrcAddress):
                    sadd=(char*)malloc(strlen(IPH->Get_SrcAddress().c_str()));
                    memcpy(sadd,IPH->Get_SrcAddress().c_str(),strlen(IPH->Get_SrcAddress().c_str()));
                    return (intptr_t) sadd;
                break;
                
                case(IP_DstAddress):
                    dadd=(char*)malloc(strlen(IPH->Get_DstAddress().c_str()));
                    memcpy(dadd,IPH->Get_DstAddress().c_str(),strlen(IPH->Get_DstAddress().c_str()));
                    return (intptr_t) dadd;
                break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                
            
            }
        break;
    
        //edit payload layer
        case (PAYLAOD):
    
            if(p.GetLayer(PAYLAOD,(layer**)&payloadh)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
            if(field==PAYLOAD_data)
            {
                return  intptr_t( payloadh->Get_Data()) ;

            }
            else if(field= PAYLOAD_length)
            {
                return (intptr_t) payloadh->Get_length();
            }
        break;

        default:
            Net_Abst_API_ERROR(": wrong layer code \n");
        

    }


}

IP_Packet ListenIP(Packet_filter f, int timeout)
{
    Net_Abst_API_TRACE_DEBUG("Listen IP for up to %d s  \n",timeout);
    filter filter;
    filter.ETHType =htons(ETH_IP);
    filter.IPproto=TCP;
    strcpy(filter.SrcIPAddress,f.SrcAdd);
    strcpy(filter.DstIPAddress,f.dstAdd);
    filter.DstPort= f.Dstport;
    filter.SrcPort= f.Srcport;

    sniffer s;
    s.Set_Filter(filter);
    s.Set_Timeout(timeout);
    s.Set_Interface((char *)interface);
    s.Start_Sniff();
    packet p=s.Get_Packet();
    packetData P_Data;
    P_Data=p.GetPacketData();
    IP_Packet ip_p;
    memcpy(ip_p.data,P_Data.data,P_Data.length);
    ip_p.length=P_Data.length;
    if (ip_p.length!=0)
    {
        Net_Abst_API_TRACE_DEBUG("Receive IP packet\n");
        p.printPacket();
    }
    else 
    {
        Net_Abst_API_TRACE_DEBUG("cannot Receive IP packet\n");
    }
    return ip_p;
}


int IP_Compute_checksum(IP_Packet* ip_p)
{
    packet p((char *)ip_p->data,ip_p->length);
    ip* IPH =new ip();
    if(p.GetLayer(IP,(layer**)&IPH)==1)
    {
        Net_Abst_API_ERROR("Layer not found \n");
        return 1;
    }
    else 
    {
        //misra
    }

    IPH->Set_CorrectHeaderChecksum();
    packetData P_Data;
    P_Data = p.GetPacketData();
    memcpy(ip_p->data,P_Data.data,ip_p->length);
    return 0;
   
}

ICMP_Option_Packet CreateICMPOption()
{   
    Net_Abst_API_TRACE_DEBUG("Create ICMP_OPTION Packet \n");
    // construct ethernet layer with default type (IP)
    ethernet eth;
    eth.Set_DestinationMAC(DUT_HW_Address);
    eth.Set_SourceMac(Tester_HW_Address);

    //Construct IP layer 
    ip IP;
    IP.Set_IHL(15);
    IP.Set_TotalLength(76);
    IP.Set_Protocol(1);
    IP.Set_DstAddress(DUT_IP_Address);
    IP.Set_SrcAddress(Tester_IP_Address);

    //constract IP option layer
    ipOption IP_OPTION;
    uint32_t data[9];
    memset(data,0,36);
    data[0] = 2000000000;
    IP_OPTION.Set_ipt_data(data);

    //Construct ICMP layer 
    icmp ICMP;

    //Construct Payload layer
    payload p((char*)"IPOption");

    // Construct Packet and add layers
    packet Packet;
    Packet.AddLayer(&eth);
    Packet.AddLayer(&IP);
    Packet.AddLayer(&IP_OPTION);
    Packet.AddLayer(&ICMP);
    Packet.AddLayer(&p);
    //Packet.SetCorrectLength();
    Packet.Set_CorrectChecksum();

    //Get ICMP packet
    packetData P_Data;
    P_Data=Packet.GetPacketData();

    ICMP_Option_Packet P_ICMP_OPTION;

    memcpy(P_ICMP_OPTION.data,P_Data.data,P_Data.length);
    P_ICMP_OPTION.length = P_Data.length;
    return P_ICMP_OPTION;
}

int SendICMPOption(ICMP_Option_Packet packetICMPOption)
{   
    packet p((char*)packetICMPOption.data,packetICMPOption.length);
    return p.sendPacket((char*)interface );
}

int EditICMPOptionField( ICMP_Option_Packet *packetICMPoption, int proto_code, int field, void * value)
{
    Net_Abst_API_TRACE_DEBUG("Edit ICMP option packet, Layer= %d, field= %d with ",proto_code,field );
    cout<<field<<endl;
    packet p((char*)packetICMPoption->data,packetICMPoption->length);
    char *dmac, *smac,*sadd,*dadd;
    ethernet * eth = new ethernet();
    ip* IPH = new ip();
    ipOption* IPOH = new ipOption(); 
    icmp * ICMPH = new icmp();
    payload * payloadh = new payload();

    //edit Eth header
    switch (proto_code)
    { 
        case (ETHERNET):
       
            if(p.GetLayer(ETHERNET,(layer**)&eth)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else
            {
                //misra
            }
            switch(field)
            {
            case(ETH_destinationMAC):
                eth->Set_DestinationMAC( static_cast<char*> (value) );
                break;
            
            case(ETH_sourceMac):
                eth->Set_SourceMac( static_cast<char*> (value) );
                break;

            case(ETH_type):
                eth->Set_Type(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                break;

            default:
                Net_Abst_API_ERROR(": wrong field code \n");
                return 1;
            }

        break;

    //edit IP header
        case (IP):
    
            if(p.GetLayer(IP,(layer**)&IPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
            switch(field)
            {
                case(IP_IHL):
                    IPH->Set_IHL(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    break;

                case(IP_Version):
                    IPH->Set_Version (static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    break;

                case(IP_Tos):
                    IPH->Set_TOS(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value))); 
                    break;

                case(IP_TotalLength):
                    IPH->Set_TotalLength(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    break;

                case(IP_Identification):
                    IPH->Set_Identification(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    break;

                case(IP_Offset):
                    IPH->Set_Offset(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    break;

                case(IP_Ttl):
                    IPH->Set_TTL(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    break;

                case(IP_Protocol):
                    IPH->Set_Protocol(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    break;
                
                case(IP_HeaderChecksum):
                    IPH->Set_HeaderChecksum(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                    break;
                
                case(IP_SrcAddress):
                    IPH->Set_SrcAddress(static_cast<char*> (value) );
                    break;
                
                case(IP_DstAddress):
                    IPH->Set_DstAddress(static_cast<char*> (value) ) ;
                    break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                    return 1;            
            }
            break;

        //edit ip option
        case (IP_Options):
            if(p.GetLayer(IP_Options,(layer**)&IPOH)==1)
            {
                Net_Abst_API_ERROR("Layer IP_OPTION not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
            switch (field)
            {
            case ipt_code:
                IPOH->Set_ipt_code(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                break;
            case ipt_len:
                IPOH->Set_ipt_len(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                break;
            case ipt_ptr:
                IPOH->Set_ipt_ptr(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                break;
            case ipt_flg:
                IPOH->Set_ipt_flg(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                break;
            case ipt_oflw:
                IPOH->Set_ipt_oflw(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                break;
                case data:
                IPOH->Set_ipt_data(static_cast<uint32_t*> (value));
                break;
            default:
                break;
            }
            break;

    
    //edit ICMP header
        case (ICMP):
  
           if(p.GetLayer(ICMP,(layer**)&ICMPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }        
            switch(field)
            {
                case(ICMP_type):
                    ICMPH->Set_type(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    break; 

                case (ICMP_code):
                    ICMPH->Set_code(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    break;

                case (ICMP_checksum):
                
                    ICMPH->Set_checksum(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                    break;
            
                case (ICMP_identifier):
                    ICMPH->Set_identifer(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    break;

                case (ICMP_sequence):
                   ICMPH->Set_sequence(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                    break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                return 1;
            }
            
        break;

        //edit payload layer
        case (PAYLAOD):
    
            if(p.GetLayer(PAYLAOD,(layer**)&payloadh)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
            if (payloadh == NULL) {
                payloadh = new payload();
                p.AddLayer(payloadh);
            }

            if(field==PAYLOAD_data)
            {
                // get old length 
                uint16_t oldPayloadLength= payloadh->Get_length();

                // set new data
                payloadh->Set_Data(static_cast<char*> (value));

                // update ip total length
                if(p.GetLayer(IP,(layer**)&IPH)==1)
                {
                    Net_Abst_API_ERROR("Layer not found \n");
                    return 1;
                }
                else 
                {
                    //misra
                }
                p.SetCorrectLength();
            }
            else if(field= PAYLOAD_length)
            {

                // get old length 
                int oldPayloadLength= payloadh->Get_length();

                // set new data
                payloadh->Setlength(static_cast<uint32_t>(reinterpret_cast<intptr_t>(value)));

                // update ip total length
                if(p.GetLayer(IP,(layer**)&IPH)==1)
                {
                    Net_Abst_API_ERROR("Layer not found \n");
                    return 1;
                }
                else 
                {
                    //misra
                }
                int oldIPTotalLength= IPH->Get_TotalLength();
                int newPayloadLength= payloadh->Get_length();
                IPH->Set_TotalLength(htons((uint16_t) oldIPTotalLength-oldPayloadLength+newPayloadLength));               
            }
            else 
            {
                Net_Abst_API_ERROR(": wrong field code \n");
                return 1;
            }
        break;

        default:
            Net_Abst_API_ERROR(": wrong layer code \n");
            return 1;
    }
    Net_Abst_API_TRACE_DEBUG("field edited with success \n");
    packetData P_Data;
    P_Data = p.GetPacketData();
    memcpy(packetICMPoption->data,P_Data.data,P_Data.length);
    packetICMPoption->length = P_Data.length;
    return 0;
}

intptr_t GetICMPOptionField(ICMP_Option_Packet* packetICMPoption, int proto_code, int field)
{   
    Net_Abst_API_TRACE_DEBUG("Get ICMP packet, Layer= %d, field= %d \n",proto_code,field);
    packet p((char*)packetICMPoption->data,packetICMPoption->length);
    char *dmac, *smac,*sadd,*dadd;
    ethernet * eth = new ethernet();
    ip* IPH =new ip();
    ipOption* IPOH = new ipOption(); 
    icmp * ICMPH= new icmp();
    payload * payloadh = new payload();
    uint32_t Data[9];
    //edit Eth header
    switch (proto_code)
    { 
        case (ETHERNET):
       
            if(p.GetLayer(ETHERNET,(layer**)&eth)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else
            {
                 //misra
            }
            switch(field)
            {
            case(ETH_destinationMAC):

                dmac=(char*)malloc(strlen(eth->Get_DestinationMAC().c_str()));
                memcpy(dmac,eth->Get_DestinationMAC().c_str(),strlen(eth->Get_DestinationMAC().c_str()));
                return (intptr_t) dmac;
                break;
            case(ETH_sourceMac):
                smac=(char*)malloc(strlen(eth->Get_SourceMac().c_str()));
                memcpy(smac,eth->Get_SourceMac().c_str(),strlen(eth->Get_SourceMac().c_str()));
                return (intptr_t) smac;
                break;

            
            case(ETH_type):
                return  (intptr_t) eth->Get_Type() ;
                break;

            default:
                Net_Abst_API_ERROR(": wrong field code \n");
            }    
            break;

    //edit IP header
        case (IP):
    
            if(p.GetLayer(IP,(layer**)&IPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
            switch(field)
            {
                case(IP_IHL):
                    return (intptr_t )IPH->Get_IHL();
                    break;

                case(IP_Version):
                    return (intptr_t) IPH->Get_Version ();
                    break;

                case(IP_Tos):
                    return (intptr_t) IPH->Get_TOS();
                    break;

                case(IP_TotalLength):
                    return (intptr_t)IPH->Get_TotalLength(); 
                    break;

                case(IP_Identification):
                    return (intptr_t )IPH->Get_Identification(); 
                    break;

                case(IP_Offset):
                    return (intptr_t)IPH->Get_Offset(); 
                    break;

                case(IP_Ttl):
                   return (intptr_t)IPH->Get_TTL();
                    break;

                case(IP_Protocol):
                    return (intptr_t)IPH->Get_Protocol();
                    break;
                
                case(IP_HeaderChecksum):
                    return (intptr_t)IPH->Get_HeaderChecksum();
                    break;
                
                case(IP_SrcAddress):
                    sadd=(char*)malloc(strlen(IPH->Get_SrcAddress().c_str()));
                    memcpy(sadd,IPH->Get_SrcAddress().c_str(),strlen(IPH->Get_SrcAddress().c_str()));
                    return (intptr_t) sadd;
                    break;
                
                case(IP_DstAddress):
                    dadd=(char*)malloc(strlen(IPH->Get_DstAddress().c_str()));
                    memcpy(dadd,IPH->Get_DstAddress().c_str(),strlen(IPH->Get_DstAddress().c_str()));
                    return (intptr_t) dadd;
                    break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
            }
        break;
    
    //get ip option

        case IP_Options:
            if(p.GetLayer(IP_Options,(layer**)&IPOH)==500)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
            switch (field)
            {
            case ipt_code:
                return (intptr_t)IPOH->Get_ipt_code();
                break;
            case ipt_len:
                return (intptr_t)IPOH->Get_ipt_len();
                break;
            case ipt_ptr:
                return (intptr_t)IPOH->Get_ipt_ptr();
                break;
            case ipt_flg:
                return (intptr_t)IPOH->Get_ipt_flg();
                break;
            case ipt_oflw:
                return (intptr_t)IPOH->Get_ipt_oflw();
                break;
            case data:
                memcpy((char*)Data,(char*)IPOH->Get_ipt_data(),36);
                return (intptr_t)Data;
                break;
            default:
                break;
            }
            break;

    //get ICMP header
        case (ICMP):
  
            if(p.GetLayer(ICMP,(layer**)&ICMPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }

            switch(field)
            {
                case(ICMP_type):
                    return (intptr_t) ICMPH->Get_type();
                break; 

                case (ICMP_code):
                    return (intptr_t) ICMPH->Get_code();
                break;

                case (ICMP_checksum):
                    return (intptr_t) ICMPH->Get_checksum();
                break;
            
                case (ICMP_identifier):
                    return (intptr_t) ICMPH->Get_identifier();
                break;
				
                case (ICMP_sequence):
                    return (intptr_t) ICMPH->Get_seuqence();
                break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
            }
            
        break;

        //edit payload layer
        case (PAYLAOD):
    
            if(p.GetLayer(PAYLAOD,(layer**)&payloadh)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
            if(field==PAYLOAD_data)
            {
                return  intptr_t( payloadh->Get_Data()) ;

            }
            else if(field= PAYLOAD_length)
            {
                return (intptr_t) payloadh->Get_length();
            }
        break;

        default:
            Net_Abst_API_ERROR(": wrong layer code \n");
    }
}

int cleanUpSocket(void)
{
    packet p;
    return p.cleanUp();
}

IP_OPTION_Packet CreateIPoption()
{
    Net_Abst_API_TRACE_DEBUG("Create IP_OPTION Packet \n");
    // construct ethernet layer with default type (IP)
    ethernet eth;
    eth.Set_DestinationMAC(DUT_HW_Address);
    eth.Set_SourceMac(Tester_HW_Address);

    //Construct IP layer 
    ip IP;
    IP.Set_IHL(15);
    IP.Set_Protocol(1);
    IP.Set_TotalLength(68);
    IP.Set_DstAddress(DUT_IP_Address);
    IP.Set_Offset(2); // MF = 0
    IP.Set_SrcAddress(Tester_IP_Address);
    

    //constract IP option layer
    ipOption IP_OPTION;
    uint32_t data[9];
    memset(data,0,36);
    data[0] = 2000000000;
    IP_OPTION.Set_ipt_data(data);
    
    //Construct Payload layer
    payload p((char*)"IPOption");
   

    // Construct Packet and add layers
    packet Packet;
    Packet.AddLayer(&eth);
    Packet.AddLayer(&IP);
    Packet.AddLayer(&IP_OPTION);
    Packet.AddLayer(&p);
    Packet.Set_CorrectChecksum();

    //Get IP packet
    packetData P_Data;
    P_Data=Packet.GetPacketData();

    IP_OPTION_Packet P_IP;
    memcpy(P_IP.data,P_Data.data,P_Data.length);
    P_IP.length= P_Data.length;
    return P_IP;
}

int SendIPOption(IP_OPTION_Packet packetIP)
{
    Net_Abst_API_TRACE_DEBUG("Send IP Packet \n");
    packet p((char*)packetIP.data,packetIP.length);
    return p.sendPacket(interface);
}

int ICMP_Option_Compute_checksum(ICMP_Option_Packet *icmp_op_p)
{
    Net_Abst_API_TRACE_DEBUG("Compute ICMP Option checksum\n");
    packet p((char *)icmp_op_p->data,icmp_op_p->length);
    if( p.Set_CorrectChecksum()==0)
    {
        memcpy(icmp_op_p->data,p.GetPacketData().data,p.GetPacketData().length);
        Net_Abst_API_TRACE_DEBUG("Compute ICMP checksum Done\n");
        return 0;
    }
    else 
    {
        Net_Abst_API_ERROR("Cannot Compute ICMP checksum\n");
        return 1;
    }
}

ICMP_Timestamp_Packet CreateICMPTimestamp()
{   
    Net_Abst_API_TRACE_DEBUG("create ICMP Timestamp packet start\n");
    // construct ethernet layer with default type (IP)
    ethernet eth;
    eth.Set_DestinationMAC(DUT_HW_Address);
    eth.Set_SourceMac(Tester_HW_Address);

    //Construct IP layer 
    ip IP;
    IP.Set_IHL(5);
    IP.Set_TotalLength(40);
    IP.Set_Protocol(1);
    IP.Set_DstAddress(DUT_IP_Address);
    IP.Set_SrcAddress(Tester_IP_Address);
    IP.Set_CorrectHeaderChecksum();

    //Construct ICMP layer 
    icmpTimestamp ICMP_TIME_STAMP;
    
    // Construct Packet and add layers
    packet Packet;
    Packet.AddLayer(&eth);
    Packet.AddLayer(&IP);
    Packet.AddLayer(&ICMP_TIME_STAMP);
    Packet.Set_CorrectChecksum();

    //Get ICMP packet
    packetData P_Data;
    P_Data=Packet.GetPacketData();

    ICMP_Timestamp_Packet P_ICMP;
    memcpy(P_ICMP.data,P_Data.data,P_Data.length);
    P_ICMP.length= P_Data.length;

    Net_Abst_API_TRACE_DEBUG("create ICMP Timestamp packet end\n");
    return P_ICMP;
}

int SendICMPTimestamp(ICMP_Timestamp_Packet packetICMPTimestamp)
{   
    packet p((char*)packetICMPTimestamp.data,packetICMPTimestamp.length);
    return p.sendPacket((char*)interface );
}

int EditICMPTimestampField( ICMP_Timestamp_Packet *packetICMPTimestamp, int proto_code, int field, void * value)
{
    Net_Abst_API_TRACE_DEBUG("Edit ICMP Timestamp packet, Layer= %d, field= %d with ",proto_code,field );
    cout<<field<<endl;
    packet p((char*)packetICMPTimestamp->data,packetICMPTimestamp->length);
    char *dmac, *smac,*sadd,*dadd;
    ethernet * eth = new ethernet();
    ip* IPH = new ip();
    icmpTimestamp * ICMPTimestampH = new icmpTimestamp();
    payload * payloadh = new payload();

    //edit Eth header
    switch (proto_code)
    { 
        case (ETHERNET):
       
            if(p.GetLayer(ETHERNET,(layer**)&eth)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else
            {
                //misra
            }
            switch(field)
            {
            case(ETH_destinationMAC):
                eth->Set_DestinationMAC( static_cast<char*> (value) );
                break;
            
            case(ETH_sourceMac):
                eth->Set_SourceMac( static_cast<char*> (value) );
                break;

            case(ETH_type):
                eth->Set_Type(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                break;

            default:
                Net_Abst_API_ERROR(": wrong field code \n");
                return 1;
            }

        break;

    //edit IP header
        case (IP):
    
            if(p.GetLayer(IP,(layer**)&IPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
            switch(field)
            {
                case(IP_IHL):
                    IPH->Set_IHL(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    break;

                case(IP_Version):
                    IPH->Set_Version (static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    break;

                case(IP_Tos):
                    IPH->Set_TOS(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value))); 
                    break;

                case(IP_TotalLength):
                    IPH->Set_TotalLength(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    break;

                case(IP_Identification):
                    IPH->Set_Identification(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    break;

                case(IP_Offset):
                    IPH->Set_Offset(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                    break;

                case(IP_Ttl):
                    IPH->Set_TTL(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    break;

                case(IP_Protocol):
                    IPH->Set_Protocol(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    break;
                
                case(IP_HeaderChecksum):
                    IPH->Set_HeaderChecksum(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                    break;
                
                case(IP_SrcAddress):
                    IPH->Set_SrcAddress(static_cast<char*> (value) );
                    break;
                
                case(IP_DstAddress):
                    IPH->Set_DstAddress(static_cast<char*> (value) ) ;
                    break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                    return 1;            
            }
        break;
    
    //edit udp header
        case (ICMP_Timestamp):
  
           if(p.GetLayer(ICMP_Timestamp,(layer**)&ICMPTimestampH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }        
            switch(field)
            {
                case(ICMP_Timestamp_type):
                    ICMPTimestampH->Set_type(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    
                break; 

                case (ICMP_Timestamp_code):
                    ICMPTimestampH->Set_code(static_cast<uint8_t>(reinterpret_cast<intptr_t>(value)));
                    
                break;

                case (ICMP_Timestamp_checksum):
                    ICMPTimestampH->Set_checksum(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                break;
            
                case (ICMP_Timestamp_identifier):
                    ICMPTimestampH->Set_identifer(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value))); 
                break;

                case (ICMP_Timestamp_sequence):
                   ICMPTimestampH->Set_sequence(static_cast<uint16_t>(reinterpret_cast<intptr_t>(value)));
                break;

                case (ICMP_Timestamp_originate_timestamp):
                   ICMPTimestampH->Set_originate_time_stamp(static_cast<uint32_t>(reinterpret_cast<intptr_t>(value)));
                break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                return 1;
            }
            
        break;

        //edit payload layer
        case (PAYLAOD):
    
            if(p.GetLayer(PAYLAOD,(layer**)&payloadh)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
            if (payloadh == NULL) {
                payloadh = new payload();
                p.AddLayer(payloadh);
            }

            if(field==PAYLOAD_data)
            {
                // get old length 
                uint16_t oldPayloadLength= payloadh->Get_length();

                // set new data
                payloadh->Set_Data(static_cast<char*> (value));

                // update ip total length
                if(p.GetLayer(IP,(layer**)&IPH)==1)
                {
                    Net_Abst_API_ERROR("Layer not found \n");
                    return 1;
                }
                else 
                {
                    //misra
                }
                p.SetCorrectLength();
            }
            else if(field= PAYLOAD_length)
            {

                // get old length 
                int oldPayloadLength= payloadh->Get_length();

                // set new data
                payloadh->Setlength(static_cast<uint32_t>(reinterpret_cast<intptr_t>(value)));

                // update ip total length
                if(p.GetLayer(IP,(layer**)&IPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
                int oldIPTotalLength= IPH->Get_TotalLength();
                int newPayloadLength= payloadh->Get_length();
                IPH->Set_TotalLength(htons((uint16_t) oldIPTotalLength-oldPayloadLength+newPayloadLength));               
            }
            else 
            {
                Net_Abst_API_ERROR(": wrong field code \n");
                return 1;
            }
        break;

        default:
            Net_Abst_API_ERROR(": wrong layer code \n");
            return 1;
    }
    Net_Abst_API_TRACE_DEBUG("field edited with success \n");
    packetData P_Data;
    P_Data = p.GetPacketData();
    memcpy(packetICMPTimestamp->data,P_Data.data,P_Data.length);
    packetICMPTimestamp->length = P_Data.length;
    return 0;
}

intptr_t GetICMPTimestampField(ICMP_Timestamp_Packet* packetICMPTimestamp, int proto_code, int field)
{   
    Net_Abst_API_TRACE_DEBUG("Get ICMP packet, Layer= %d, field= %d \n",proto_code,field);
    packet p((char*)packetICMPTimestamp->data,packetICMPTimestamp->length);
    char *dmac, *smac,*sadd,*dadd;
    ethernet * eth = new ethernet();
    ip* IPH =new ip();
    icmpTimestamp * ICMPTimestampH = new icmpTimestamp();
    payload * payloadh = new payload();
    //edit Eth header
    switch (proto_code)
    { 
        case (ETHERNET):
       
            if(p.GetLayer(ETHERNET,(layer**)&eth)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else
            {
                //misra
            }

            switch(field)
            {
            case(ETH_destinationMAC):

                dmac=(char*)malloc(strlen(eth->Get_DestinationMAC().c_str()));
                memcpy(dmac,eth->Get_DestinationMAC().c_str(),strlen(eth->Get_DestinationMAC().c_str()));
                return (intptr_t) dmac;
            break;
            
            case(ETH_sourceMac):
                smac=(char*)malloc(strlen(eth->Get_SourceMac().c_str()));
                memcpy(smac,eth->Get_SourceMac().c_str(),strlen(eth->Get_SourceMac().c_str()));
                return (intptr_t) smac;
            break;

            
            case(ETH_type):
                return  (intptr_t) eth->Get_Type() ;
            break;

            default:
                Net_Abst_API_ERROR(": wrong field code \n");
            }
            
        break;

    //edit IP header
        case (IP):
    
            if(p.GetLayer(IP,(layer**)&IPH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }

            switch(field)
            {
                case(IP_IHL):
                    return (intptr_t )IPH->Get_IHL();
                break;

                case(IP_Version):
                    return (intptr_t) IPH->Get_Version ();
                break;

                case(IP_Tos):
                    return (intptr_t) IPH->Get_TOS();
                break;

                case(IP_TotalLength):
                    return (intptr_t)IPH->Get_TotalLength(); 
                break;

                case(IP_Identification):
                    return (intptr_t )IPH->Get_Identification(); 
                break;

                case(IP_Offset):
                    return (intptr_t)IPH->Get_Offset(); 
                break;

                case(IP_Ttl):
                   return (intptr_t)IPH->Get_TTL();
                break;

                case(IP_Protocol):
                    return (intptr_t)IPH->Get_Protocol();
                break;
                
                case(IP_HeaderChecksum):
                    return (intptr_t)IPH->Get_HeaderChecksum();
                break;
                
                case(IP_SrcAddress):
                    sadd=(char*)malloc(strlen(IPH->Get_SrcAddress().c_str()));
                    memcpy(sadd,IPH->Get_SrcAddress().c_str(),strlen(IPH->Get_SrcAddress().c_str()));
                    return (intptr_t) sadd;
                break;
                
                case(IP_DstAddress):
                    dadd=(char*)malloc(strlen(IPH->Get_DstAddress().c_str()));
                    memcpy(dadd,IPH->Get_DstAddress().c_str(),strlen(IPH->Get_DstAddress().c_str()));
                    return (intptr_t) dadd;
                break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
            }
        break;
    
    //edit ICMP header
        case (ICMP_Timestamp):
  
            if(p.GetLayer(ICMP_Timestamp,(layer**)&ICMPTimestampH)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }

            switch(field)
            {
                case(ICMP_Timestamp_type):
                    return (intptr_t) ICMPTimestampH->Get_type();
                break; 

                case (ICMP_Timestamp_code):
                    return (intptr_t) ICMPTimestampH->Get_code();
                break;

                case (ICMP_Timestamp_checksum):
                    return (intptr_t) ICMPTimestampH->Get_checksum();
                break;
            
                case (ICMP_Timestamp_identifier):
                    return (intptr_t) ICMPTimestampH->Get_identifier();
                break;
				
                case (ICMP_Timestamp_sequence):
                    return (intptr_t) ICMPTimestampH->Get_seuqence();
                break;

                case (ICMP_Timestamp_originate_timestamp):
                    return (intptr_t) ICMPTimestampH->Get_originate_time_stamp();
                break;

                case (ICMP_Timestamp_receive_timestamp):
                    return (intptr_t) ICMPTimestampH->Get_receive_time_stamp();
                break;

                case (ICMP_Timestamp_transmit_timestamp):
                    return (intptr_t) ICMPTimestampH->Get_transmit_time_stamp();
                break;

                default:
                    Net_Abst_API_ERROR(": wrong field code \n");
                

            }
            
        break;

        //edit payload layer
        case (PAYLAOD):
    
            if(p.GetLayer(PAYLAOD,(layer**)&payloadh)==1)
            {
                Net_Abst_API_ERROR("Layer not found \n");
                return 1;
            }
            else 
            {
                //misra
            }
            if(field==PAYLOAD_data)
            {
                return  intptr_t( payloadh->Get_Data()) ;

            }
            else if(field= PAYLOAD_length)
            {
                return (intptr_t) payloadh->Get_length();
            }
        break;

        default:
            Net_Abst_API_ERROR(": wrong layer code \n");
    }
}

ICMP_Timestamp_Packet ListenICMPTimestamp(Packet_filter f, int timeout)
{
    Net_Abst_API_TRACE_DEBUG("Listen ICMP Timestamp for up to timeout s  \n");
    filter filter;
    filter.ETHType =htons(ETH_IP);
    filter.IPproto=ICMP;
    strcpy(filter.SrcIPAddress,f.SrcAdd);
    strcpy(filter.DstIPAddress,f.dstAdd);

    sniffer s;
    s.Set_Filter(filter);
    s.Set_Timeout(timeout);
    s.Set_Interface((char *)interface);
    s.Start_Sniff();
    packet p=s.Get_Packet();
    packetData P_Data;
    P_Data=p.GetPacketData();
    ICMP_Timestamp_Packet icmp_p;
    memcpy(icmp_p.data,P_Data.data,P_Data.length);
    icmp_p.length=P_Data.length;
    if (icmp_p.length!=0)
    {
        Net_Abst_API_TRACE_DEBUG("Receive ICMP Timestamp packet\n");
        p.printPacket();
    }
    else 
    {
        Net_Abst_API_TRACE_DEBUG("cannot Receive ICMP Timestamp packet\n");
    }
    return icmp_p;
}


int ICMP_Timestamp_Compute_checksum(ICMP_Timestamp_Packet* icmp_p)
{
    Net_Abst_API_TRACE_DEBUG("Compute ICMP Tiemstamp checksum\n");
    packet p((char *)icmp_p->data,icmp_p->length);
    if( p.Set_CorrectChecksum()==0)
    {
        memcpy(icmp_p->data,p.GetPacketData().data,p.GetPacketData().length);
        Net_Abst_API_TRACE_DEBUG("Compute ICMP Timestamp checksum Done\n");
        return 0;
    }
    else 
    {
        Net_Abst_API_ERROR("Cannot Compute ICMP Timestamp checksum\n");
        return 1;
    }
}