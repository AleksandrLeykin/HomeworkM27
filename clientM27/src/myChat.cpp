#include "myChat.h"
#include "clientTest.h"
#include <memory>

void myChat::interChat()
{
    while (true)
    {
        std::cout << "Для входа нажмите - v, дл¤ регистрации - r, дл¤ выхода - 3 или exit:\n"
			      << "To enter, press - v, to register - r, to exit - 3 or exit: ";
        char symbol = getChar();        

        switch (symbol)
        {
        case 'v':
        {
            myClient client;
            client.setMyString("v");
            client.clientLoading();
        }            
            break;
        case 'r':
        {
            myClient client;
            client.setMyString("r");
            client.clientLoading();
        }
            
            break;
        case '3':	
        {
            myClient client;
            client.setMyString("exit");
        }					
			return;    
        default:
            std::cout << "Введен не верный символ!! Invalid character entered!!" << "\n";
            break;
        }
    }   
}
