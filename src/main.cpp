#include <iostream>
#include "serverTest.h"
#include <thread>

#if defined (_WIN64)
#include <conio.h>
#endif
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


#if defined (__linux__)
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
#elif defined(_WIN64)

void ToExit() {
    //выход из сервера не доработано
    char c;
    std::cout << "To exit, press ESC and Enter Для выхода нажмите  ESC и ввод" << std::endl;
    c = getch();
    if (c == 27) {
        // std::cout << "enter" << std::endl;
        exit(0);
    }
}
#endif

