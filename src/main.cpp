#include <iostream>
//#include "mySQLTest.h"
#include "serverTest.h"
#include <thread>

//выход из сервера
void ToExit();

int main(int args, char* argv[]) {
    setlocale(LC_ALL, "ru");

    std::thread ta(ToExit);
 
    m_server serv;
    
    if (ta.joinable()) {
        ta.join();
    }
    

    return 0;
}

void ToExit() {
    //выход из сервера не доработано
    char c;
    std::cout << "To exit, press ESC and Enter Для выхода нажмите  ESC и ввод" << std::endl;
    c = getchar();
    if (c == 27) {
       // std::cout << "enter" << std::endl;
        exit(0);
    }    
}