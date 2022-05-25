//library used

#include "TCPServer.h"

#include "ApiManager.h"

int opt = TRUE;

int master_socket, addrlen, new_socket, client_socket[30],

    max_clients = 30, activity, i, valread, sd;

int max_sd;

struct sockaddr_in address;

uint8_t buffer[buffer_size]; //data buffer of 512

//set of socket descriptors

fd_set readfds;

test_result_frame_t test_result;

std::mutex mtx; // mutex for critical section

TCPServer::TCPServer()

{
}

TCPServer::~TCPServer()

{
}

void TCPServer::start()

{

    printf("starting\n");

    std::thread t1(&TCPServer::ServerStart, this);

    t1.join();
}

void TCPServer::ServerStart()

{

    //initialise all client_socket[] to 0 so not checked

    for (i = 0; i < max_clients; i++)

    {

        client_socket[i] = 0;
    }

    //create a master socket

    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)

    {

        perror("socket failed");

        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections ,

    //this is just a good habit, it will work without this

    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,

                   sizeof(opt)) < 0)

    {

        perror("setsockopt");

        exit(EXIT_FAILURE);
    }

    //type of socket created

    address.sin_family = AF_INET;

    address.sin_addr.s_addr = htonl(INADDR_ANY); //change it to your adress

    address.sin_port = htons(Port);

    //bind the socket to your Adress port 9876

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)

    {

        perror("bind failed");

        exit(EXIT_FAILURE);
    }

    printf("Listener on port %d \n", Port);

    //try to specify maximum of 3 pending connections for the master socket

    if (listen(master_socket, 3) < 0)

    {

        perror("listen");

        exit(EXIT_FAILURE);
    }

    //accept the incoming connection

    addrlen = sizeof(address);

    puts("Waiting for connections ...");

    while (TRUE)

    {

        //clear the socket set

        FD_ZERO(&readfds);

        //add master socket to set

        FD_SET(master_socket, &readfds);

        max_sd = master_socket;

        //add child sockets to set

        for (i = 0; i < max_clients; i++)

        {

            //socket descriptor

            sd = client_socket[i];

            //if valid socket descriptor then add to read list

            if (sd > 0)

                FD_SET(sd, &readfds);

            //highest file descriptor number, need it for the select function

            if (sd > max_sd)

                max_sd = sd;
        }

        //wait for an activity on one of the sockets , timeout is NULL ,

        //so wait indefinitely

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))

        {

            printf("select error");
        }

        //If something happened on the master socket ,

        //then its an incoming connection

        if (FD_ISSET(master_socket, &readfds))

        {

            if ((new_socket = accept(master_socket,

                                     (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)

            {

                perror("accept");

                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands

            printf("New connection , socket fd is %d , ip is : %s , port : %d \n ", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            //add new socket to array of sockets

            for (i = 0; i < max_clients; i++)

            {

                //if position is empty

                if (client_socket[i] == 0)

                {

                    client_socket[i] = new_socket;

                    printf("Adding to list of sockets as %d\n", i);

                    break;
                }
            }
        }

        //else its some IO operation on some other socket

        for (i = 0; i < max_clients; i++)

        {

            sd = client_socket[i];

            if (FD_ISSET(sd, &readfds))

            {

                //Check if it was for closing , and also read the

                //incoming message

                if ((valread = read(sd, buffer, sizeof(buffer))) == 0)

                {

                    //Somebody disconnected , get his details and print

                    getpeername(sd, (struct sockaddr *)&address,

                                (socklen_t *)&addrlen);

                    printf("Host disconnected , ip %s , port %d \n",

                           inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                    //Close the socket and mark as 0 in list for reuse

                    close(sd);

                    client_socket[i] = 0;
                }

                else

                {

                    printf("Recieved from %s : ", inet_ntoa(address.sin_addr));

                    for (int i = 0; i < valread; i++)

                    {

                        printf("%02x, ", buffer[i]);
                    }

                    printf("\n");

                    switch (buffer[0])
                    {
                    case 0x01:
                    {

                        send(sd, buffer, sizeof(buffer), 0);

                        break;
                    }

                    case 0x02:
                    {
                        ComNetAPIConfig_t *ComNetAPIConf = (ComNetAPIConfig_t *)buffer;
                        Net_API_config_t NetAPIConfig;
                        strcpy(NetAPIConfig.S_interface, ComNetAPIConf->S_interface);
                        strcpy(NetAPIConfig.S_DUT_HW_Address, ComNetAPIConf->S_DUT_HW_Address);
                        strcpy(NetAPIConfig.S_DUT_IP_Address, ComNetAPIConf->S_DUT_IP_Address);
                        strcpy(NetAPIConfig.S_Tester_HW_Address, ComNetAPIConf->S_Tester_HW_Address);
                        strcpy(NetAPIConfig.S_Tester_IP_Address, ComNetAPIConf->S_Tester_IP_Address);
                        std::cout << "Recieved Com_Abstraction configuration:\n\tinterface: " << ComNetAPIConf->S_interface << "\n";
                        std::cout << "\n\t Dut MAC Adress: " << ComNetAPIConf->S_DUT_HW_Address << "\n";
                        std::cout << "\n\tDut IP Adress: " << ComNetAPIConf->S_DUT_IP_Address << "\n";
                        std::cout << "\n\tTester MAC Adress: " << ComNetAPIConf->S_Tester_HW_Address << "\n";
                        std::cout << "\n\tTester IP Adress: " << ComNetAPIConf->S_Tester_IP_Address << "\n";
                        std::cout << "Recieved Abstraction configuration:\n\tinterface: " << NetAPIConfig.S_interface;
                        std::cout << "\n\t Dut MAC Adress: " << NetAPIConfig.S_DUT_HW_Address;
                        std::cout << "\n\tDut IP Adress: " << NetAPIConfig.S_DUT_IP_Address;
                        std::cout << "\n\tTester MAC Adress: " << NetAPIConfig.S_Tester_HW_Address;
                        std::cout << "\n\tTester IP Adress: " << NetAPIConfig.S_Tester_IP_Address << "\n";
                        Set_Network_Abstration_API_Config(NetAPIConfig);
                        break;
                    }

                    case 0x03:
                    {
                        internal_protocol_frame_t *rf = (internal_protocol_frame_t *)buffer;
                        internal_protocol_frame_t APIFrame = *rf;
                        int a = sd;
                        std::thread t([=]()
                                      {
                                          printf("protocol frame recieved: %02x, %02x, %04x, %04x, %04x, %04x\n",

                                                 APIFrame.Opcode, APIFrame.TC8ID, APIFrame.Test, APIFrame.Api, APIFrame.Reserved, APIFrame.Checksum);

                                          test_result = APIrunner(APIFrame);

                                          printf("result frame sent to HMI %02x, %02x, %04x, %04x, %04x, %04x, %04x\n",

                                                 test_result.Opcode, test_result.TC8ID, test_result.Test, test_result.Api,

                                                 test_result.ErrorCode, test_result.Reserved, test_result.Checksum);

                                          usleep(500000);

                                          int res = send(a, (uint8_t *)&test_result, sizeof(test_result_frame_t), 0);
                                      });
                        std::cout << "thread detaching\n";
                        t.detach();

                        break;
                    }

                    case 0x04:
                    {
                        Heartbeat *hb = (Heartbeat *)buffer;
                        printf("Heartbeat frame recieved: %02x, %04x, %04x, %04x\n", hb->Opcode, hb->seq_num, hb->Reserved1, hb->Checksum);
                        hb->seq_num++;
                        hb->Checksum = gen_crc16((uint8_t *)hb, sizeof(Heartbeat) - 2);
                        printf("Heartbeat frame sent: %02x, %04x, %04x, %04x\n", hb->Opcode, hb->seq_num, hb->Reserved1, hb->Checksum);
                        send(sd, (uint8_t *)hb, sizeof(Heartbeat), 0);
                        usleep(500000);
                        break;
                    }

                    case 0x05:
                    {
                        ComTCPConfig_t *ComTCPConf = (ComTCPConfig_t *)buffer;
                        TCP_config_t TCPConf = ComTCPConf->TCPConf;
                        std::cout << "TCP Configuration Recieved\n\tDut Port: " << TCPConf.DUT_Port << "\n\tTester Port: " << TCPConf.Tester_Port
                                  << "\n\tDut IP: " << TCPConf.DUT_IP << "\n\tTester IP: " << TCPConf.TESTER_IP << "\n\tMaxcon: " << TCPConf.Maxcon << "\n\tISN: "
                                  << TCPConf.ISN << "\n"
                                  << "\n\tmss: " << TCPConf.mss << "\n"
                                  << "\n\tnss: " << TCPConf.nss << "\n";
                        Set_TCP_Config(TCPConf);
                        break;
                    }

                    case 0x06:
                    {
                        ComARPConfig *comARPconf = (ComARPConfig *)buffer;
                        std::cout << "ARP Configuration recieved:\n";
                        Arp_config_t ARPconfig = comARPconf->ARPconf;
                        std::cout << "DYNAMIC_ARP_CACHETIMEOUT:" << ARPconfig.DYNAMIC_ARP_CACHETIMEOUT << "\n";
                        std::cout << "DYNAMIC_ARP_CACHETIMEOUT:" << ARPconfig.DYNAMIC_ARP_CACHETIMEOUT << "\n";
                        std::cout << "HOST_1_IP:" << ARPconfig.HOST_1_IP << "\n";
                        std::cout << "DIface_0_IP:" << ARPconfig.DIface_0_IP << "\n";
                        std::cout << "ParamListenTime:" << +ARPconfig.ParamListenTime << "\n";
                        std::cout << "MAC_ADDR1:" << ARPconfig.MAC_ADDR1 << "\n";
                        std::cout << "MAC_ADDR2:" << ARPconfig.MAC_ADDR2 << "\n";
                        std::cout << "MAC_ADDR3:" << ARPconfig.MAC_ADDR3 << "\n";
                        std::cout << "DIFACE_O_MAC_ADDR:" << ARPconfig.DIFACE_O_MAC_ADDR << "\n";
                        std::cout << "ARBIT_MAC_ADDR:" << ARPconfig.ARBIT_MAC_ADDR << "\n";
                        std::cout << "ARP_HARDWARE_TYPE_UNKNOWN:" << ARPconfig.ARP_HARDWARE_TYPE_UNKNOWN << "\n";
                        std::cout << "UNKNOWN_HW_ADDR_LEN:" << ARPconfig.UNKNOWN_HW_ADDR_LEN << "\n";
                        std::cout << "ARP_PROTOCOL_UNKNOWN:" << ARPconfig.ARP_PROTOCOL_UNKNOWN << "\n";
                        std::cout << "UNKNOWN_PROTCOL_ADDR_LEN:" << ARPconfig.UNKNOWN_PROTCOL_ADDR_LEN << "\n";
                        std::cout << "ARP_TOLERANCE_TIME:" << +ARPconfig.ARP_TOLERANCE_TIME << "\n";
                        std::cout << "ETHERNET_ADDR_LEN:" << +ARPconfig.ETHERNET_ADDR_LEN << "\n";
                        std::cout << "ARP_HARDWARE_ETHERNET:" << ARPconfig.ARP_HARDWARE_ETHERNET << "\n";
                        std::cout << "IP_ADDR_LEN:" << ARPconfig.IP_ADDR_LEN << "\n";
                        std::cout << "ARP_PROTOCOL_IP:" << ARPconfig.ARP_PROTOCOL_IP << "\n";
                        std::cout << "OPERATION_REQUEST:" << +ARPconfig.OPERATION_REQUEST << "\n";
                        std::cout << "all_zeroes:" << ARPconfig.all_zeroes << "\n";
                        std::cout << "ETHERNET_BROADCAST_ADDR:" << ARPconfig.ETHERNET_BROADCAST_ADDR << "\n";
                        std::cout << "IP_FIRST_UNUSED_ADDR_INTERFACE_1:" << ARPconfig.IP_FIRST_UNUSED_ADDR_INTERFACE_1 << "\n";
                        std::cout << "OPERATION_RESPONSE:" << +ARPconfig.OPERATION_RESPONSE << "\n";
                        set_ARP_config(ARPconfig);
                        break;
                    }
                    case 0x07:
                    {
                        ComUDPConfig *comUDPconf = (ComUDPConfig *)buffer;
                        std::cout << "UDP Configuration recieved:\n";
                        UDP_config_t UDPconfig = comUDPconf->UDPconf;
                        std::cout << "UDPData:" << comUDPconf->UDPconf.UDPData << "\n";
                        std::cout << "testerUDPPort:" << UDPconfig.testerUDPPort << "\n";
                        std::cout << "testerUDPPort2:" << UDPconfig.testerUDPPort2 << "\n";
                        std::cout << "unusedUDPDstPort1:" << UDPconfig.unusedUDPDstPort1 << "\n";
                        std::cout << "unusedUDPSrcPort:" << UDPconfig.unusedUDPSrcPort << "\n";
                        std::cout << "DUTUDPPort:" << UDPconfig.DUTUDPPort << "\n";
                        std::cout << "udpUserDataSize:" << UDPconfig.udpUserDataSize << "\n";
                        std::cout << "calculatedUDPChecksum:" << UDPconfig.calculatedUDPChecksum << "\n";
                        std::cout << "incorrectUDPChecksum:" << UDPconfig.incorrectUDPChecksum << "\n";
                        std::cout << "UDPDefaultData:" << UDPconfig.UDPDefaultData << "\n";
                        std::cout << "UDPDefaultDataLen:" << UDPconfig.UDPDefaultDataLen << "\n";
                        std::cout << "UDPData:" << UDPconfig.UDPData << "\n";
                        std::cout << "UDPDataLen:" << UDPconfig.UDPDataLen << "\n";
                        std::cout << "Dst_IP_addr:" << UDPconfig.Dst_IP_addr << "\n";
                        std::cout << "Host_1_IP:" << UDPconfig.Host_1_IP << "\n";
                        std::cout << "Host_2_IP:" << UDPconfig.Host_2_IP << "\n";
                        std::cout << "AIface_0_BcastIP:" << UDPconfig.AIface_0_BcastIP << "\n";
                        std::cout << "allSystemMCastAddr:" << UDPconfig.allSystemMCastAddr << "\n";
                        Set_UDP_Config(UDPconfig);
                        break;
                    }
                    case 0x08:
                    {
                        ComICMPConfig *comICMPconf = (ComICMPConfig *)buffer;
                        std::cout << "ICMP Configuration recieved:\n";
                        std::cout << "BROADCAST_ADDRESS:" << comICMPconf->ICMPconf.BROADCAST_ADDRESS << "\n";
                        ICMPv4_config_t ICMPconfig = comICMPconf->ICMPconf;
                        std::cout << "DUT_IP:" << ICMPconfig.DUT_IP << "\n";
                        std::cout << "TESTER_IP:" << ICMPconfig.TESTER_IP << "\n";
                        std::cout << "ICMP_IDENTIFIER:" << ICMPconfig.ICMP_IDENTIFIER << "\n";
                        std::cout << "ICMP_SEQUENCE_NUMBER:" << ICMPconfig.ICMP_SEQUENCE_NUMBER << "\n";
                        std::cout << "INVALID_CHECKSUM:" << ICMPconfig.INVALID_CHECKSUM << "\n";
                        std::cout << "FRAGMENT_REASSEMBLY_TIMEOUT:" << ICMPconfig.FRAGMENT_REASSEMBLY_TIMEOUT << "\n";
                        std::cout << "LISTEN_TIME:" << +ICMPconfig.LISTEN_TIME << "\n";
                        std::cout << "UNSUPPORTED_PROTOCOL:" << ICMPconfig.UNSUPPORTED_PROTOCOL << "\n";
                        std::cout << "INVALID_ICMP_TYPE:" << ICMPconfig.INVALID_ICMP_TYPE << "\n";
                        std::cout << "BROADCAST_ADDRESS:" << ICMPconfig.BROADCAST_ADDRESS << "\n";
                        std::cout << "ORINGIN_TIMESTAMP_VALUE:" << ICMPconfig.ORINGIN_TIMESTAMP_VALUE << "\n";
                        Set_ICMPv4_Config(ICMPconfig);
                        break;
                    }
                    case 0x09:
                    {
                        ComIPV4Config *comIPV4conf = (ComIPV4Config *)buffer;
                        std::cout << "IPV4 Configuration recieved:\n";
                        IPv4_config_t IPV4config = comIPV4conf->IPV4conf;
                        std::cout << "DUT_IP:" << IPV4config.DUT_IP << "\n";
                        std::cout << "TESTER_IP:" << IPV4config.TESTER_IP << "\n";
                        std::cout << "DUT_PORT:" << IPV4config.DUT_PORT << "\n";
                        std::cout << "TESTER_PORT:" << IPV4config.TESTER_PORT << "\n";
                        std::cout << "INVALID_CHECKSUM:" << IPV4config.INVALID_CHECKSUM << "\n";
                        std::cout << "TTL:" << IPV4config.TTL << "\n";
                        std::cout << "LARGE_TTL_VALUE:" << IPV4config.LARGE_TTL_VALUE << "\n";
                        std::cout << "LOW_TTL_VALUE:" << IPV4config.LOW_TTL_VALUE << "\n";
                        std::cout << "DIRECTED_BROADCAST_ADDRESS:" << IPV4config.DIRECTED_BROADCAST_ADDRESS << "\n";
                        std::cout << "LIMITED_BROADCAST_ADDRESS:" << IPV4config.LIMITED_BROADCAST_ADDRESS << "\n";
                        std::cout << "LISTEN_TIME:" << IPV4config.LISTEN_TIME << "\n";
                        std::cout << "DUT_PORT:" << IPV4config.DUT_PORT << "\n";
                        std::cout << "IP_INI_REASSEMBLE_TIMEOUT:" << IPV4config.IP_INI_REASSEMBLE_TIMEOUT << "\n";
                        std::cout << "FRAGMENT_REASSEMBLY_TIMEOUT:" << IPV4config.FRAGMENT_REASSEMBLY_TIMEOUT << "\n";
                        std::cout << "IP_VERSION:" << IPV4config.IP_VERSION << "\n";
                        std::cout << "IP_TYPE_ICMP:" << IPV4config.IP_TYPE_ICMP << "\n";
                        std::cout << "IP_TYPE_TCP:" << IPV4config.IP_TYPE_TCP << "\n";
                        std::cout << "MTU:" << IPV4config.MTU << "\n";
                        Set_IPv4_Config(IPV4config);
                        break;
                    }
                    default:
                        break;
                    }
                    bzero(buffer, sizeof(buffer));
                }
            }
        }
    }
}
