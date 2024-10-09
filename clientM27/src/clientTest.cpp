#include "clientTest.h"

myClient::myClient()
{
    
}

myClient::~myClient()
{
    shutdown(clientSocket, SHUT_RDWR);
	close(clientSocket);
    std::cout << "Test of exit!" << std::endl;	
}

void myClient::clientLoading()
{
    // Creating a socket Создадание сокета
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cout << "Socket not created! Сокет не создан!" << std::endl;
		return;
    }
    
    // Setting the server address Установка  адреса сервера
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT_NUM);
    serverAddr.sin_family = AF_INET;

    // Establishing a connection to the server  Установка соединения с сервером
    connectionClient = connect(clientSocket, (sockaddr*)& serverAddr, sizeof(serverAddr));
    if (connectionClient == -1) {
        std::cout << "No connection to the server! Нет соединения с сервером!" << std::endl;
        return;
    }
    
    // Buffers for sending and receiving data // Буферы для отправки и получения данных
     char clientBuffer[BUFF_SIZE];
     char serverBuff[BUFF_SIZE];
    // The size of sending / receiving packet in bytes Размер отправляемого/принимаемого пакета в байтах
	short packet_size = 0;

    // Взаимодействие с сервером
    while (1) {
        //обнуляем хранилище сообщений
        bzero(clientBuffer, sizeof(clientBuffer));
        //заполняем буфер
        strcpy(clientBuffer, m_str.c_str());

        // Check whether client like to stop chatting Проверьте, хочет ли клиент прекратить общение
		if (clientBuffer[0] == 'e' && clientBuffer[1] == 'x' && clientBuffer[2] == 'i' && clientBuffer[3] == 't') {
			// Terminate a connection opened on a socket Завершить соединения, открытого на сокете
            shutdown(clientSocket, SHUT_RDWR);
			close(clientSocket);			
			return;
		}

        //sending message to server отправка сообщения на сервер
        packet_size = send(clientSocket, clientBuffer, BUFF_SIZE, 0);
        if (packet_size == -1) {
            std::cout << "Can't send message to Server. Error # " << std::endl;
            shutdown(clientSocket, SHUT_RDWR);
			close(clientSocket);			
			return;
        }
        
        //обнуляем хранилище сообщений
        bzero(serverBuff, sizeof(serverBuff));

        //receiving message from server прием сообщения с сервера
        packet_size = recv(clientSocket, serverBuff, BUFF_SIZE, 0);
        if ( packet_size == -1) {
            std::cout << "Can't receive message from Server. Error # " << std::endl;
            shutdown(clientSocket, SHUT_RDWR);
			close(clientSocket);			
			return;
        }
        else
			std::cout << "Server message: " << serverBuff << std::endl;

        //go to messages переход на сообщения
        if (serverBuff[0] == 'H')
        {
            //here we try to accept messages sent to the user
            //здесь пробуем принять посланные для пользователя сообщения
            setMyString("m");
            std::string result = receptionTransmissionMes(clientSocket, m_str);
			std::cout << result << std::endl;

            //переход на сообщения
			setMyString("u");
            result = receptionTransmissionMes(clientSocket, m_str);
			std::cout << "Registered users:\n" << result << std::endl;
            userMessage(clientSocket);
        }
        m_str = getLineOfText();

        // std::cin >> m_str;
        // std::cin.clear();
        // std::cin.ignore(32767, '\n');
        // bzero(clientBuffer, sizeof(clientBuffer));
    }
    
}

void myClient::userMessage(int client_sock)
{
    setMyString("n");
    //запрос имени name request
	std::string result = receptionTransmissionMes(client_sock, m_str);
	std::cout << result << std::endl;
    //отправка имени sending name
	std::string username = getLineOfText();
	result = receptionTransmissionMes(client_sock, username);
	std::cout << result << std::endl;

    //набор сообщения typing a message
	std::string userMessage = getLineOfText();
	result = receptionTransmissionMes(client_sock, userMessage);
	std::cout << result << std::endl;
}

std::string myClient::receptionTransmissionMes(int client_sock, const std::string &name)
{
    char serverBuff[BUFF_SIZE];
    //sending message to server отправка сообщения на сервер  
    if (send(client_sock, name.c_str(), name.length(), 0) == -1) {
        shutdown(clientSocket, SHUT_RDWR);
		close(clientSocket);			
		return "Can't send message to Server. Error # ";
    }
    //receiving message from server прием сообщения с сервера
    if (recv(client_sock, serverBuff, sizeof(serverBuff), 0) == -1)
    {
        shutdown(clientSocket, SHUT_RDWR);
		close(clientSocket);
        return "Can't receive message from Server. Error # ";
    }

    std::string str = "";
    int size = sizeof(serverBuff) / sizeof(serverBuff[0]);
    for (int i = 0; i < size; i++) {
        if (serverBuff[i] != '\0') {
            str += serverBuff[i];
        }
        else 
            break;       
    }  
    
   // std::string str(serverBuff, sizeof(serverBuff));
    return str;
}