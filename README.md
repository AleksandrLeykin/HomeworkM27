# HomeworkM27

1. Класс ServerTest при загрузке создает сервер и отслеживает подключение к нему пользователей.
 При первом подключении пользователя создает базу данных(chattestdb) в mySQL и в зависимости от запросов пользавателя,  производит регистрацию, вход и передачу сообшений в чате с записью в таблицы БД 
 (user, messages) при помощи функций (SetToClient(), userRegistration(), enterChat())
 Выход из программы сервера осуществляется пока "костылем" функцией в новом потоке ToExit()

2. Класс mySQLTest осуществляет непосредственное подключение к mySQL и работу сервера с базой данных при помощи функций 
(addingUser(), userLogin())
