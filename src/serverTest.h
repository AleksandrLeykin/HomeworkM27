#pragma once

#include <iostream>
#include <vector>
#include <string>

#if defined (__linux__)
#include <sys/socket.h>
#include <unistd.h> //для read()
#include <netinet/in.h> //для структуры sockaddr_in

#elif defined(_WIN64)
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#endif

//макрос дл¤ печати количества активных пользователей
#define PRINTUSERS if (nclients) std::cout << nclients << " - User on-line" << std::endl; \
						else std::cout << "No user on line\n";

//Key constants
//const char IP_SERV[] = "127.0.0.1";
const int PORT_NUM = 7777;
const short BUFF_SIZE = 1024;

//подключение пользователя
void SetToClient(int client_socket);

//Регистрация пользователя User registration
void userRegistration(int client_sock, char buff[BUFF_SIZE]);

//enter chat вход в чат
void enterChat(int client_sock, char buff[BUFF_SIZE]);

//exchange with user обмен с пользователем
std::string recAndTransMess(int client_sock,const std::string& str, char buff[BUFSIZ]);

class m_server {
private:
//Структура, описывающая адрес интернет-сокета.
#if defined (__linux__)
    sockaddr_in serveraddr{}, clientaddr{};
    socklen_t length{};
    // Создадим сокет  Let's create a socket
    int socket_server{}, socket_client{};

#elif defined(_WIN64)
    SOCKET server_socket{};
	SOCKET client_socket{};
    //Initializing the Socket Library Инициализация Библиотеки cокетов
	WSAData wsData = {0};	
#endif
//загрузка_сервера
void server_loading();
void close_server();

public:
m_server() { server_loading();}
~m_server() { close_server(); }

};

