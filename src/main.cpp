#include <iostream>
#include <memory>
#include "serverTest.h"
#include <thread>
#include "Logger.h" 

#if defined (_WIN64)
#include <conio.h>
#endif
//выход из сервера
void ToExit();

//указатель на Logger
//std::shared_ptr<Logger> Log(new Logger);

int main(int args, char* argv[]) {
    setlocale(LC_ALL, "ru");
      
    std::thread ta(ToExit);

    m_server serv;    
   
    if (ta.joinable()) {
        ta.join();
    }

    return 0;
}


#if defined (__linux__)
void ToExit() {
    //выход из сервера не доработано
    char c;
    std::cout << "To exit, press ESC and Enter. To display Logs, press 'l'." << std::endl;
    c = getchar();

    if (c == 'l') {
        //указатель на Logger
        std::shared_ptr<Logger> Log(new Logger);
        Log->readLogFile();
        ToExit();
    }
    
    if (c == 27) {
        // std::cout << "enter" << std::endl;
        exit(0);
    }
}
#elif defined(_WIN64)

void ToExit() {
    //выход из сервера не доработано
    char c;
    std::cout << "To exit, press ESC and Enter. To display Logs, press 'l'." << std::endl;
    c = getch();
    
    if (c == 'l') {
        //указатель на Logger
        std::shared_ptr<Logger> Log(new Logger);
        Log->readLogFile();
        ToExit();
    }
    
    if (c == 27) {
        // std::cout << "enter" << std::endl;
        exit(0);
    }   
}
#endif

