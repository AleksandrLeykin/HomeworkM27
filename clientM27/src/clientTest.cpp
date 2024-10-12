#include "clientTest.h"

#if defined (__linux__)
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
        close(clientSocket);
        std::cout << "Socket not created! Сокет не создан!" << std::endl;
        return;
    }

    // Setting the server address Установка  адреса сервера
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT_NUM);
    serverAddr.sin_family = AF_INET;

    // Establishing a connection to the server  Установка соединения с сервером
    connectionClient = connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (connectionClient == -1) {
        close(clientSocket);
        std::cout << "No connection to the server! Нет соединения с сервером!" << std::endl;
        return;
    }

    // Buffers for sending and receiving data // Буферы для отправки и получения данных
    // char clientBuffer[BUFF_SIZE];
    // char serverBuff[BUFF_SIZE];
     std::vector<char> serverBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);
    // The size of sending / receiving packet in bytes Размер отправляемого/принимаемого пакета в байтах
    short packet_size = 0;

    // Взаимодействие с сервером
    while (1) {
        // //обнуляем хранилище сообщений
        // bzero(clientBuffer, sizeof(clientBuffer));
        // //заполняем буфер
        // strcpy(clientBuffer, m_str.c_str());
        clientBuff = { cbegin(m_str), cend(m_str) };

        // Check whether client like to stop chatting Проверьте, хочет ли клиент прекратить общение
        if (clientBuff[0] == 'e' && clientBuff[1] == 'x' && clientBuff[2] == 'i' && clientBuff[3] == 't') {
            // Terminate a connection opened on a socket Завершить соединения, открытого на сокете
            shutdown(clientSocket, SHUT_RDWR);
            close(clientSocket);
            return;
        }

        //sending message to server отправка сообщения на сервер
        packet_size = send(clientSocket, clientBuff.data(), clientBuff.size(), 0);
        if (packet_size == -1) {
            std::cout << "Can't send message to Server. Error # " << std::endl;
            shutdown(clientSocket, SHUT_RDWR);
            close(clientSocket);
            return;
        }

        // //обнуляем хранилище сообщений
        // bzero(serverBuff, sizeof(serverBuff));
        //очистка вектора serverBuff
        auto it = serverBuff.cbegin();
        serverBuff.insert(it, serverBuff.size(), '\0');

        //receiving message from server прием сообщения с сервера
        packet_size = recv(clientSocket, serverBuff.data(), serverBuff.size(), 0);
        if (packet_size == -1) {
            std::cout << "Can't receive message from Server. Error # " << std::endl;
            shutdown(clientSocket, SHUT_RDWR);
            close(clientSocket);
            return;
        }
        else
            std::cout << "Server message: " << serverBuff.data() << std::endl;

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
        //m_str = getLineOfText();

        std::cin >> m_str;
        std::cin.clear();
        std::cin.ignore(32767, '\n');
        clientBuff.clear();
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

std::string myClient::receptionTransmissionMes(int client_sock, const std::string& name)
{
    //char serverBuff[BUFF_SIZE];
    std::vector<char> serverBuff(BUFF_SIZE);
    //sending message to server отправка сообщения на сервер  
    if (send(client_sock, name.c_str(), name.length(), 0) == -1) {
        shutdown(clientSocket, SHUT_RDWR);
        close(clientSocket);
        return "Can't send message to Server. Error # ";
    }
    //receiving message from server прием сообщения с сервера
    if (recv(client_sock, serverBuff.data(), serverBuff.size(), 0) == -1)
    {
        shutdown(clientSocket, SHUT_RDWR);
        close(clientSocket);
        return "Can't receive message from Server. Error # ";
    }

    std::string str = "";
    //int size = sizeof(serverBuff) / sizeof(serverBuff[0]);
    for (int i = 0; i < serverBuff.size(); i++) {
        if (serverBuff[i] != '\0') {
            str += serverBuff[i];
        }
        else
            break;
    }

    // std::string str(serverBuff, sizeof(serverBuff));
    return str;
}


#elif defined(_WIN64)

void myClient::clientLoading()
{
    // For checking errors in sockets functions // Для проверки ошибок в функциях сокетов
    int erStat;
    // WinSock initialization // Инициализация WinSock
    WSADATA wsData;
    erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
    if (erStat != 0) {
        std::cout << "Error WinSock version initializaion #";
        std::cout << WSAGetLastError();
        return;
    }
    //else 		std::cout << "WinSock initialization is OK" << std::endl;

    // Socket initialization
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
    }
    //else 		std::cout << "Client socket initialization is OK" << std::endl;

    // reset the address зануляем адрес   
    ZeroMemory(&serverAddr, sizeof(serverAddr));
    // Установим адрес сервера	
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    // Зададим номер порта
    serverAddr.sin_port = htons(SERVER_PORT_NUM);
    // Используем IPv4
    serverAddr.sin_family = AF_INET;

    // Установим соединение с сервером
    erStat = connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (erStat != 0) {
        std::cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return;
    }
    /*else
        std::cout << "Connection established SUCCESSFULLY. Ready to send a message to Server"
        << "Соединение установлено УСПЕШНО.\nГотово к отправке сообщения на сервер" << std::endl;*/

        // Buffers for sending and receiving data // Буферы для отправки и получения данных
    std::vector<char> serverBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);
    // The size of sending / receiving packet in bytes // Размер отправляемого/принимаемого пакета в байтах
    short packet_size = 0;

    while (true)
    {
        clientBuff = { cbegin(m_str), cend(m_str) };

        // Check whether client like to stop chatting 
        if (clientBuff[0] == 'e' && clientBuff[1] == 'x' && clientBuff[2] == 'i' && clientBuff[3] == 't') {
            //The shutdown function disables sends or receives on a socket.
            //Функция выключения отключает отправку и получение данных на сокете.
            shutdown(clientSocket, SD_BOTH);
            closesocket(clientSocket);
            WSACleanup();
            return;
        }

        //sending message to server отправка сообщения на сервер
        packet_size = send(clientSocket, clientBuff.data(), clientBuff.size(), 0);
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return;
        }

        //очистка вектора serverBuff
        auto it = serverBuff.cbegin();
        serverBuff.insert(it, serverBuff.size(), '\0');
        //receiving message from server прием сообщения с сервера
        packet_size = recv(clientSocket, serverBuff.data(), serverBuff.size(), 0);
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return;
        }
        else
            std::cout << "Server message: " << serverBuff.data() << std::endl;

        //переход на сообщения
        if (serverBuff[0] == 'H')
        {
            //здесь пробуем принять посланные для пользователя сообщения
            setMyString("m");
            std::string result = receptionTransmissionMes(clientSocket, m_str);
            std::cout << result << std::endl;

            //переход на сообщения
            setMyString("u");
            //clientBuff = { cbegin(m_str), cend(m_str) };

            result = receptionTransmissionMes(clientSocket, m_str);
            std::cout << "Registered users:\n" << result << std::endl;

            userMessage(clientSocket);
        }

        m_str = "";
        std::cin >> m_str;

        std::cin.clear();
        std::cin.ignore(32767, '\n');

        clientBuff.clear();
    }
}

void myClient::userMessage(SOCKET client_sock) {
    setMyString("n");
    //запрос имени name request
    std::string result = receptionTransmissionMes(client_sock, m_str);

    /*   for (int i = 0; i < result.size(); i++) {
           if (result[i] != '\0')
           {
               std::cout << result[i];
           }
           else {
               std::cout << std::endl;
               break;
           }
       }*/

    std::cout << result << std::endl;
    //отправка имени sending name
    std::string username = getLineOfText();
    result = receptionTransmissionMes(client_sock, username);

    //for (int i = 0; i < result.size(); i++) {
    //	if (result[i] != '\0')
    //	{
    //		std::cout << result[i];
    //	}
    //	else {
    //		std::cout << std::endl;
    //		break;
    //	}
    //}

    std::cout << result << std::endl;

    //набор сообщения typing a message
    std::string userMessage = getLineOfText();
    result = receptionTransmissionMes(client_sock, userMessage);

    /*for (int i = 0; i < result.size(); i++) {
        if (result[i] != '\0')
        {
            std::cout << result[i];
        }
        else {
            std::cout << std::endl;
            break;
        }
    }*/

    std::cout << result << std::endl;
}

std::string myClient::receptionTransmissionMes(SOCKET client_sock, const std::string& name)
{
    std::vector<char> serverBuff(BUFF_SIZE);

    //sending message to server отправка сообщения на сервер
    if (send(client_sock, name.c_str(), name.length(), 0) == SOCKET_ERROR) {
        closesocket(client_sock);
        WSACleanup();
        return "Can't send message to Server. Error # ";
    }
    //receiving message from server прием сообщения с сервера	
    if (recv(client_sock, serverBuff.data(), serverBuff.size(), 0) == SOCKET_ERROR) {
        closesocket(client_sock);
        WSACleanup();
        return "Can't receive message from Server. Error # ";
    }
    std::string str = "";
    for (int i = 0; i < serverBuff.size(); i++)
    {
        if (serverBuff[i] != '\0') {
            str += serverBuff[i];
        }
        else
            break;
    }
    return str;
}

#endif