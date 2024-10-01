#include "serverTest.h"
#include "mySQLTest.h"

#include <memory.h>
#include <string>
#include <thread>
#include <netdb.h> //для gethostbyaddr()
#include <arpa/inet.h> //для функции inet_ntoa преобразует сетевой адрес Интернета (IPv4) в строку ASCII

//переменная - количество активных пользователей
int nclients = 0;

void m_server::server_loading()
{
    //переменная erStat для анализирования ошибок при подключении
    int erStat = 0;
    socket_server = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_server == -1) {
        std::cout << "Socket not created! сокет не создан" << std::endl;
        return;
    }

    //Address for receiving incoming messages. Адрес для приема входящих сообщений
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //port number for communication номер порта для связи 
    serveraddr.sin_port = htons(PORT_NUM);
    //IPv4
    serveraddr.sin_family = AF_INET;

    // Привяжем сокет
    erStat = bind(socket_server, (sockaddr*)& serveraddr, sizeof(serveraddr));
    if (erStat == -1) {
        std::cout << "Socket not bound! сокет не привязан!" << std::endl;
        return;
    }
    
    //Let's set the server to receive data Поставим сервер на прием данных
    erStat = listen(socket_server, 0X100);
    if (erStat == -1) {
        std::cout << "The socket cannot listen for new connections." << std::endl;
        return;
    }
    else 
        std::cout << "Server is listening... Сервер слушает..." << std::endl;

    length = sizeof(clientaddr);

    //выход из сервера не доработано
    // char c;
    // std::cout << "To exit, press ESC and Enter Для выхода нажмите  ESC и ввод";
    // c = getchar();
    // if (c == 27) {
    //     std::cout << "enter" << std::endl;
    //     exit(0);
    // }    

    // цикл извлечения запросов на подключение из очереди
    while (socket_client = accept(socket_server, (sockaddr*)& clientaddr, &length))
    {
        // увеличиваем счетчик подключившихся клиентов 
        nclients++;
        
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

void SetToClient(int client_socket) {

   char buffer[1024];
   int bytes_rectv = 0;   

   //прием строки от клиента и возвращение ее клиенту
   while (bytes_rectv = recv(client_socket, &buffer[0], BUFF_SIZE, 0) && bytes_rectv != -1)
   {
        //memset(buffer, 0, 1024);
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
    if (client_socket != -1)
    {
       close(client_socket);
    }  
}

void userRegistration(int client_sock, char buff[BUFF_SIZE])
{
    std::string str = "Enter you name:";
    //обнуление буфера
    bzero(buff, BUFF_SIZE);
    send(client_sock, str.c_str(), str.length(), 0);    
    recv(client_sock, &buff[0], BUFF_SIZE, 0);
    std::string name = buff;

    bzero(buff, BUFF_SIZE);
    str = "Enter you surname:";
	send(client_sock, str.c_str(), str.length(), 0);
    recv(client_sock, &buff[0], BUFF_SIZE, 0);
	std::string surname = buff;

    bzero(buff, BUFF_SIZE);
    str = "Enter you email: ";
	send(client_sock, str.c_str(), str.length(), 0);
    recv(client_sock, &buff[0], BUFF_SIZE, 0);
    std::string email = buff;

    bzero(buff, BUFF_SIZE);
    str = "Enter you pass: ";
    send(client_sock, str.c_str(), str.length(), 0);
    recv(client_sock, &buff[0], BUFF_SIZE, 0);
    std::string pass = buff;

    mySQLTest mysql;
    std::string result = " ";
	result = mysql.addingUser(name, surname, email, pass);
	send(client_sock, result.c_str(), result.length(), 0);

    bzero(buff, BUFF_SIZE);
    recv(client_sock, &buff[0], BUFF_SIZE, 0);
	result = buff;

    if (result == "y") {
	   // result = mysql.userLogin(clientName, pass);
		send(client_sock, result.c_str(), result.length(), 0);
	}
    
}

void enterChat(int client_sock, char buff[BUFF_SIZE])
{
    std::string clientname = recAndTransMess(client_sock, "Enter you name:", buff);

    std::string pass = recAndTransMess(client_sock, "Enter you pass:", buff);

    mySQLTest mysql;
    std::string result = mysql.userLogin(clientname, pass);
    send(client_sock, result.c_str(), result.length(), 0);	
}

std::string recAndTransMess(int client_sock,const std::string& str, char buff[BUFSIZ])
{	
	//обнуление буфера
    bzero(buff, BUFF_SIZE);
	send(client_sock, str.c_str(), str.length(), 0);
		
	recv(client_sock, &buff[0], BUFF_SIZE, 0);	
	return buff;
}

void m_server::close_server()
{
    // закрываем сокет, завершаем соединение
    close(socket_server);
}
