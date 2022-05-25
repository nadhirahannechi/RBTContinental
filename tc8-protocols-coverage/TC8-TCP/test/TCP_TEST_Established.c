#include "AbstractionAPI.h"
#include "TCPcommon.h"
#include "TCPconfig.h"

#include "TestabilityProtocol_api.h"
#include "TestabilityProtocol_Intern.h"
int socketIDtest= 0;

void CreateAndBind_callbacktest(TP_ResultID_t b, uint16 socket)
{
    socketIDtest = socket;
}

int main() 
{


    // Configure Network API
   Net_API_config_t NetAPIConfig; 
    strcpy(NetAPIConfig.S_interface, "enp0s3");
    strcpy(NetAPIConfig.S_DUT_HW_Address, "b8:27:eb:4b:b2:24");
    strcpy(NetAPIConfig.S_DUT_IP_Address, "169.254.159.220");
    strcpy(NetAPIConfig.S_Tester_HW_Address, "08:00:27:0b:e3:32");
    strcpy(NetAPIConfig.S_Tester_IP_Address, "169.254.159.221");
    Set_Network_Abstration_API_Config(NetAPIConfig);

    // Configure TCP protocol
    TCP_config_t conf;
    strcpy(conf.DUT_IP, "192.168.200.2");
    strcpy(conf.TESTER_IP, "192.168.200.1");
    conf.DUT_Port = 50000;
    conf.Tester_Port = 59256;
    conf.msl = 60;
    Set_TCP_Config(conf);
/* 
    // Controle channel
    stoiIP(TCPConfig.DUT_IP, ipv4.Data);
    TP_OpenControlChannel(ipv4, 56000);
    TP_GenaralStartTest(NULL);
    printf("\n Starting TCP_UNACCEPTABLE_02 ...\n");
    int socketID;
    uint32 rSEQNumber = 0;
    uint32 rACKNumber = 0;
    moveServDUTToEstablished(&socketID, NULL, TCPConfig.DUT_Port, TCPConfig.Maxcon, (uint32*)&rSEQNumber, (uint32*)&rACKNumber);
    TP_GenaralEndTest(NULL,0,(text){0});
    TP_CloseControlChannel();*/
    ip4addr ipv4;
    ipv4.dataLength = 4;
    stoiIP(TCPConfig.DUT_IP, ipv4.Data);
    TP_OpenControlChannel(ipv4, 56000);
    TP_GenaralStartTest(NULL);

    // Move DUT to listen state
    TP_TcpCreateAndBind(CreateAndBind_callbacktest, true, 50001, ipv4);
    sleep(3);
    TP_TcpListenAndAccept(NULL, NULL, socketIDtest, 5);
    sleep(3);
    
    // connect client to the server
    	
    int clientSocket;
	struct sockaddr_in serverAddr;
	char buffer[1024];
    
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	printf("[+]Client Socket Created Sucessfully.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(50001);
	serverAddr.sin_addr.s_addr = inet_addr("192.168.200.2");
    
    struct sockaddr_in newAddr;
    newAddr.sin_family = AF_INET;
	newAddr.sin_port = htons(50002);
	newAddr.sin_addr.s_addr = inet_addr("192.168.200.1");
	bind(clientSocket, (struct sockaddr*)&newAddr, sizeof(newAddr));
    

	connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	printf("[+]Connected to Server.\n");

         sleep(1);
  
    vint8 data;
    data.Data = (uint8*) calloc(4,sizeof(uint8));
    data.Data = (uint8*)"data";
    data.dataLength = 4;
    TP_TcpSendData(NULL, socketIDtest, data.dataLength, 0, data);
    //sleep(30);
	recv(clientSocket, buffer, 1024, 0);
	printf("[+]Data Recv: %s\n", buffer);
	printf("[+]Closing the connection.\n");

    close (clientSocket);
    
    
    
    
    return 0;
}
