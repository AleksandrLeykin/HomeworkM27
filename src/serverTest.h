#pragma once

#include <iostream>
#include <vector>
#include <string>

#if defined (__linux__)

#include <sys/socket.h>
#include <unistd.h> //для read()
#include <netinet/in.h> //для структуры sockaddr_in

#elif defined(_WIN64)

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#endif

//макрос дл¤ печати количества активных пользователей
#define PRINTUSERS if (nclients) std::cout << nclients << " - User on-line" << std::endl; \
						else std::cout << "No user on line\n";

//Key constants
//const char IP_SERV[] = "127.0.0.1";
const int PORT_NUM = 7777;
const short BUFF_SIZE = 1024;

//display received messages показ прин¤тых сообщений
std::string receivedMessages(const std::string& name);

//write message to table запись сooбщени¤ в таблицу 
std::string writingMessage(const std::string& name1, const std::string& name2, const std::string& strMes);

#if defined (__linux__)
//подключение пользователя
void SetToClient(int client_socket);

//Регистрация пользователя User registration
std::string userRegistration(int client_sock, char buff[BUFF_SIZE]);

//enter chat вход в чат
std::string enterChat(int client_sock, char buff[BUFF_SIZE]);

//name verification сверка имени
bool nameVerification(int client_sock, char buff[BUFSIZ]);

//exchange with user обмен с пользователем
std::string recAndTransMess(int client_sock, const std::string& str, char buff[BUFSIZ]);

#elif defined(_WIN64)
//подключение пользователя
void SetToClient(SOCKET client_socket);

//Регистрация пользователя User registration
std::string  userRegistration(SOCKET client_sock, char buff[BUFF_SIZE]);

//enter chat вход в чат
std::string enterChat(SOCKET client_sock, char buff[BUFF_SIZE]);

//name verification сверка имени
bool nameVerification(SOCKET client_sock, char buff[BUFSIZ]);

//exchange with user обмен с пользователем
std::string recAndTransMess(SOCKET client_sock, const std::string& str, char buff[BUFSIZ]);

#endif

class m_server {
private:
    //Структура, описывающая адрес интернет-сокета.
    sockaddr_in serveraddr{}, clientaddr{};
    socklen_t length{};
#if defined (__linux__)    

    // Создадим сокет  Let's create a socket
    int socket_server{}, socket_client{};

#elif defined(_WIN64)
    SOCKET socket_server{};
    SOCKET socket_client{};

    //Initializing the Socket Library Инициализация Библиотеки cокетов
    WSAData wsData = { 0 };
#endif
    //загрузка_сервера
    void server_loading();
    void close_server();

public:
    m_server() { server_loading(); }
    ~m_server() { close_server(); }

};