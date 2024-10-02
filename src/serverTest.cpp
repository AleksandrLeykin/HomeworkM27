#include "serverTest.h"
#include "mySQLTest.h"

#include <memory.h>
#include <string>
#include <thread>

#if defined (__linux__)
#include <netdb.h> //для gethostbyaddr()
#include <arpa/inet.h> //для функции inet_ntoa преобразует сетевой адрес Интернета (IPv4) в строку ASCII

#define CLEAR_BUFFER bzero(buff, BUFF_SIZE);

#elif defined(_WIN64)

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define CLEAR_BUFFER memset(buff, 0, BUFF_SIZE);


#endif



//переменная - количество активных пользователей
int nclients = 0;

void m_server::server_loading()
{
    //переменная erStat для анализирования ошибок при подключении
    int erStat = 0;
#if defined (__linux__)
    socket_server = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_server == -1) {
        std::cout << "Socket not created! сокет не создан" << std::endl;
        return;
    }
    //Address for receiving incoming messages. Адрес для приема входящих сообщений
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
#elif defined(_WIN64)
    //переменная erStat для анализирования ошибок при подключении
    erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
    if (erStat != 0) {
        std::cout << "Error Winsock initialisation #" << WSAGetLastError();
        return;
    }
    else	std::cout << "WinSock initialisation is ok!" << std::endl;

    socket_server = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_server == INVALID_SOCKET) {
        std::cout << "Socket not created! сокет не создан" << std::endl;
        close_server();
        return;
    }
    //Address for receiving incoming messages. Адрес для приема входящих сообщений
    serveraddr.sin_addr.s_addr = INADDR_ANY;
#endif
    
    //port number for communication номер порта для связи 
    serveraddr.sin_port = htons(PORT_NUM);
    //IPv4
    serveraddr.sin_family = AF_INET;

    // Привяжем сокет
    erStat = bind(socket_server, (sockaddr*)& serveraddr, sizeof(serveraddr));
#if defined (__linux__)
    if (erStat == -1) {
#elif defined(_WIN64)
    if (erStat != 0) {
#endif   
        std::cout << "Socket not bound! сокет не привязан!" << std::endl;
        return;
    }

    //Let's set the server to receive data Поставим сервер на прием данных
    erStat = listen(socket_server, 0X100);
#if defined (__linux__)
    if (erStat == -1) {
#elif defined(_WIN64)
    if (erStat != 0) {
#endif     
        std::cout << "The socket cannot listen for new connections." << std::endl;
        return;
    }
    else 
        std::cout << "Server is listening... Сервер слушает..." << std::endl;

    length = sizeof(clientaddr);  

    // цикл извлечения запросов на подключение из очереди
    while (socket_client = accept(socket_server, (sockaddr*)& clientaddr, &length))
    {
        // увеличиваем счетчик подключившихся клиентов 
        nclients++;
        
        // пытаемся получить имя хоста. Структура hostent используется функциями для хранения 
       // информации о данном хосте, такой как имя хоста, адрес IPv4 и т. д.
        hostent* clHost;
        clHost = gethostbyaddr((char*)& clientaddr.sin_addr.s_addr, 4, AF_INET);
        
        // вывод сведений о клиенте
		//Функция inet_ntoa преобразует сетевой адрес Интернета (IPv4) в строку ASCII в стандартном формате 
		// Интернета с разделительными точками.
        std::cout << clHost->h_name << " " << inet_ntoa(clientaddr.sin_addr) << " new connect!\n";
        PRINTUSERS     
    
        std::thread(SetToClient, socket_client).detach();
    }    
}

#if defined (__linux__)
void SetToClient(int client_socket) {

    char buffer[1024];
    int bytes_rectv = 0;
   //прием строки от клиента и возвращение ее клиенту
   while (bytes_rectv = recv(client_socket, &buffer[0], BUFF_SIZE, 0) && bytes_rectv != -1) {
#elif defined(_WIN64)   
   void SetToClient(SOCKET client_socket) {

       char buffer[1024];
       int bytes_rectv = 0;
   //прием строки от клиента и возвращение ее клиенту
   while ((bytes_rectv = recv(client_socket, &buffer[0], BUFF_SIZE, 0)) && bytes_rectv != SOCKET_ERROR) {
#endif   
        memset(buffer, 0, 1024);
        std::string result = "";
        switch (*buffer)
        {        
        case 'r':
            userRegistration(client_socket, buffer);
            break;
        case 'v':
            enterChat(client_socket, buffer);
            break;
        
        default:
            break;
        }
   }

    // если мы здесь, то произошел выход из цикла по причине
    // возращения функцией recv ошибки - соединение с клиентом разорвано
    nclients--;
    std::cout << "-disconnect\n";
    PRINTUSERS
    // закрываем сокет closesocket(client_socket);
#if defined (__linux__)
    if (client_socket != -1) {
       close(client_socket);
    }  
#elif defined(_WIN64)   
    // закрываем сокет closesocket(client_socket);
    if (client_socket != INVALID_SOCKET) {
        closesocket(client_socket);
    }
#endif   
}

#if defined (__linux__)
void userRegistration(client_sock, char buff[BUFF_SIZE])
#elif defined(_WIN64)   
void userRegistration(SOCKET client_sock, char buff[BUFF_SIZE])
#endif   
{
    std::string str = "Enter you name:";
    //обнуление буфера   
    CLEAR_BUFFER
    send(client_sock, str.c_str(), str.length(), 0);    
    recv(client_sock, &buff[0], BUFF_SIZE, 0);
    std::string name = buff;

    CLEAR_BUFFER
    str = "Enter you surname:";
	send(client_sock, str.c_str(), str.length(), 0);
    recv(client_sock, &buff[0], BUFF_SIZE, 0);
	std::string surname = buff;

    CLEAR_BUFFER
    str = "Enter you email: ";
	send(client_sock, str.c_str(), str.length(), 0);
    recv(client_sock, &buff[0], BUFF_SIZE, 0);
    std::string email = buff;

    CLEAR_BUFFER
    str = "Enter you pass: ";
    send(client_sock, str.c_str(), str.length(), 0);
    recv(client_sock, &buff[0], BUFF_SIZE, 0);
    std::string pass = buff;

    mySQLTest mysql;
    std::string result = " ";
	result = mysql.addingUser(name, surname, email, pass);
	send(client_sock, result.c_str(), result.length(), 0);

    CLEAR_BUFFER
    recv(client_sock, &buff[0], BUFF_SIZE, 0);
	result = buff;

    if (result == "y") {
	   // result = mysql.userLogin(clientName, pass);
		send(client_sock, result.c_str(), result.length(), 0);
	}
    
}

#if defined (__linux__)
void enterChat(int client_sock, char buff[BUFF_SIZE])
#elif defined(_WIN64)   
void enterChat(SOCKET client_sock, char buff[BUFF_SIZE])
#endif  
{
    std::string clientname = recAndTransMess(client_sock, "Enter you name:", buff);

    std::string pass = recAndTransMess(client_sock, "Enter you pass:", buff);

    mySQLTest mysql;
    std::string result = mysql.userLogin(clientname, pass);
    send(client_sock, result.c_str(), result.length(), 0);	
}

#if defined (__linux__)
std::string recAndTransMess(int client_sock, const std::string& str, char buff[BUFSIZ])
#elif defined(_WIN64)   
std::string recAndTransMess(SOCKET client_sock, const std::string& str, char buff[BUFSIZ])
#endif  
{	
	//обнуление буфера
    CLEAR_BUFFER
	send(client_sock, str.c_str(), str.length(), 0);
		
	recv(client_sock, &buff[0], BUFF_SIZE, 0);	
	return buff;
}

void m_server::close_server() {
    // закрываем сокет, завершаем соединение
#if defined (__linux__)
    close(socket_server);
#elif defined(_WIN64) 
     closesocket(socket_server);    
#endif   
}
