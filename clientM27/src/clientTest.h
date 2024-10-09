#pragma once

#include <iostream>
#include <vector>
#include <string>

//Key constants
// Enter IPv4 address of Server
const char SERVER_IP[] = "127.0.0.1";
// Enter Listening port on Server side
const short SERVER_PORT_NUM = 7777;
// Maximum size of buffer for exchange info between server and client
const short BUFF_SIZE = 1024;

#if defined (__linux__)
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class myClient {
private:
    int clientSocket{};
    int connectionClient{};
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;

    std::string m_str = "";    
   
    //ввод строки (line input)
    std::string getLineOfText() {
		std::string str = "";
		getline(std::cin, str);		
 		return str;
	}
    //приём Передача Сообщения
    std::string receptionTransmissionMes(int client_sock, const std::string& name);
    void userMessage(int client_sock);

public:
    myClient();
    ~myClient();

    void setMyString(const std::string& str) {
        m_str = str;
    }

    void clientLoading();
};

#elif defined(_WIN64)

#include <WinSock2.h>;
#include <WS2tcpip.h>
#include <stdio.h>
//#include <inaddr.h>

#pragma comment(lib, "Ws2_32.lib")

#endif