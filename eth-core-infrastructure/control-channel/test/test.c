#include "TestabilityProtocol_Intern.h"
#include "TestabilityProtocol_api.h"




TP_ResultID_t  generalGetVersion_cb_client(TP_ResultID_t a,uint16 b, uint16 v, uint16 c)
{
    TP_TRACE_DEBUG("cb TP_ResultID_t TP_ResultID_t = %d version1 %d version2 %d %version %d  ",a,b,v,c);

}
TP_ResultID_t  GenaralStartTest_cb_client(TP_ResultID_t b)
{
    TP_TRACE_DEBUG("cb TP_ResultID_t %d  ",b);

}
uint16 socket_test;
static uint16 socket_test2;
TP_ResultID_t  GenaralEndTest_cb_client(TP_ResultID_t b,uint16 socketID)
{
    socket_test2 = socketID;
    TP_TRACE_DEBUG("cb TP_ResultID_t %d socketID %d  ",b,socketID);
    

}

TP_ResultID_t GenaralTCP_UDP_CREATE_cb_client(TP_ResultID_t b,uint16 socketID)
{
    socket_test = socketID;
    TP_TRACE_DEBUG("cb TP_ResultID_t %d socketID %d  ",b,socketID);

}
TP_ResultID_t  TCP_UDP_SHUTDOWN_cb_client(TP_ResultID_t b,uint16 socketID)
{
    TP_TRACE_DEBUG("cb TP_ResultID_t %d socketID %d  ",b,socketID);

}

TP_ResultID_t  ARP_CLEAR_CACHE_cb_client(TP_ResultID_t b)
{
    TP_TRACE_DEBUG("cb TP_ResultID_t : %d  ",b);
}

TP_ResultID_t  ARP_ADD_STATIC_ENTRY_cb_client(TP_ResultID_t b, vint8 ip, vint8 mac)
{
    TP_TRACE_DEBUG("cb TP_ResultID_t : %d  ",b);
}

TP_ResultID_t  ARP_DELETE_STATIC_ENTRY_cb_client(TP_ResultID_t b, vint8 ip)
{
    TP_TRACE_DEBUG("cb TP_ResultID_t : %d  ",b);
}


TP_ResultID_t  TCP_UDP_configure_cb_client(TP_ResultID_t b,uint16 socketID)
{

    TP_TRACE_DEBUG("cb TP_ResultID_t %d socketID %d  ",b,socketID);

}

int acceptsocket = 0;

TP_ResultID_t  TCP_listenAndAccept_cb_event(TP_ResultID_t b,uint16 listenSocketId, uint16 newSocketId, uint16 port, ip4addr address)
{
    acceptsocket = newSocketId;
    TP_TRACE_DEBUG("cb TP_ResultID_t %d socketID %d  ",b,acceptsocket);

}

#ifdef ICMPTEST
TP_ResultID_t ICMP_CB(TP_ResultID_t b){


    TP_TRACE_DEBUG("ICMP TP_ResultID_t %d  ",b);

}
#endif
int main()
{
    ip4addr localhost;
    localhost.dataLength = 4;
    localhost.Data[0] = 127;
    localhost.Data[1] = 0;
    localhost.Data[2] = 0;
    localhost.Data[3] = 1;
    TP_OpenControlChannel(localhost,56000);
#ifdef ICMPTEST

    ip4addr ipv4;
    ipv4.dataLength = 4;
    ipv4.Data[0] = 0;
    ipv4.Data[1] = 0;
    ipv4.Data[2] = 0;
    ipv4.Data[3] = 0;
    text ifn ;
    /*to set interface name */
    /* ifn.dataLength = 2;
    ifn.Data = calloc(2, 1);
    ifn.Data[0]='l';
    ifn.Data[1]='o'; */
    /* test without providing interface name  */
    ifn.dataLength = 0;
    /*  */ 

    vint8 data;
    data.dataLength = 4;
    data.Data = calloc(4, 1);
    data.Data[0]=69;
    data.Data[1]=69;
    data.Data[2]=69;
    data.Data[3]=69;

    TP_IcmpEchoRequest(ICMP_CB,ifn,ipv4,data);
#endif


    //TP_ClearArpCache(ARP_CLEAR_CACHE_cb_client);

    vint8 addip;
    addip.dataLength = 14;
    char IP[] = "192.168.20.200";
    addip.Data = IP;
    vint8 addmac;
    addmac.dataLength = 17;
    char MAC[] = "aa:bb:cc:dd:ee:ff";
    addmac.Data = (uint8 *) MAC;
    /*addmac.Data = malloc(addmac.dataLength);
    addmac.Data[0]  = 0xaa;
    addmac.Data[1] = ":";*/

    //TP_AddStaticEntry(ARP_ADD_STATIC_ENTRY_cb_client,addip,addmac);
    //TP_DeleteStaticEntry(ARP_DELETE_STATIC_ENTRY_cb_client,addip);


    //TP_GenaralStartTest(GenaralStartTest_cb_client);
    //sleep(2);
    //TP_GenaralGetVersion(generalGetVersion_cb_client);
    //sleep(2);
    /*ip4addr ipv4;
    ipv4.dataLength = 4;
    ipv4.Data[0] = 192;
    ipv4.Data[1] = 168;
    ipv4.Data[2] = 20;
    ipv4.Data[3] = 0;
    
    text ifName;
    ifName.dataLength = 10;
    ifName.Data = malloc(10);
    char * ifn = "BOMeth10"; 
    memcpy(ifName.Data, ifn, 10);*/
    //TP_Ipv4StaticRoute(NULL_PTR,ifName, ipv4 ,24,localhost);
    //TP_EthInterfaceUp(NULL_PTR,ifName);
    //TP_EthInterfaceDown(NULL_PTR,ifName);
    //TP_TcpGetState(TP_TcpGetState_cb, socketId)
    TP_GenaralStartTest(NULL_PTR);
    sleep(3);

    //TP_TcpCloseSocket(GenaralEndTest_cb_client, socket_test, TRUE);
    //TP_UdpCloseSocket(TCP_UDP_SHUTDOWN_cb_client, socket_test);
    ////TP_GenaralEndTest(GenaralEndTest_cb_client,0,(text){0});
   // TP_TcpCreateAndBind(GenaralEndTest_cb_client, 1, 52068, localhost);
   // sleep(3);
   // TP_TcpListenAndAccept(generalGetVersion_cb_client, TCP_listenAndAccept_cb_event, socket_test2, 10);
   // sleep(3);
   // TP_TcpCreateAndBind(GenaralTCP_UDP_CREATE_cb_client, 1, 52069, localhost);
   // sleep(3);
   // TP_TcpConnect(TCP_UDP_SHUTDOWN_cb_client, socket_test, 52068, localhost);
   // sleep(3);
   //     sleep(3);
   // TP_TcpCloseSocket(NULL_PTR,  socket_test2, TRUE);
   //     sleep(3);
   // TP_TcpCloseSocket(NULL_PTR,  socket_test, TRUE);

    /*TP_TcpSocketState(NULL_PTR, 52068, localhost, 52069, localhost);
    sleep(3);
    TP_TcpSocketState(NULL_PTR, 52068, localhost, 0, (ip4addr) {0});*/
    //vint8 data;
    //data.dataLength = 3;
    //data.Data = calloc(3, 1);
    //data.Data = "et";
    //TP_TcpSendData(NULL_PTR, acceptsocket, 0, 0, data);
    //sleep(3);
    //
    //TP_TcpReceiveAndForward(NULL_PTR, NULL_PTR, socket_test2, 10, 0xFFFF);
    //sleep(3);
    //TP_TcpSendData( NULL_PTR, acceptsocket, 0, 0,data);
    sleep(3);
    text test ;
    test.dataLength = 7;
    test.Data = "enp0s3";
    //memcpy(test.Data,"BOMtest12",10);
    TP_DhcpInitClient(NULL_PTR,test);
    sleep(3);
    TP_DhcpStopClient(NULL_PTR,test);
    sleep(3);
    TP_GenaralEndTest(NULL_PTR,2 ,test);
    sleep(3);
    TP_CloseControlChannel();

    return 0;
}
