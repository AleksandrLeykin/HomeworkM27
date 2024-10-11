cmake_minimum_required(VERSION 3.26)

set(PROJECT_NAME myServerDB)

project(${PROJECT_NAME})

if(UNIX)
# Укажите путь к MySQL Connector/C++
set(MYSQL_CONNECTOR_CPP_INCLUDE_DIR "/path/to/mysql-connector-c++/include")
set(MYSQL_CONNECTOR_CPP_LIB_DIR "/path/to/mysql-connector-c++/lib")

# Линкуем библиотеки MySQL Connector/C++
target_link_libraries(${PROJECT_NAME}  mysqlcppconn)
target_link_libraries(${PROJECT_NAME} mysqlclient)
endif()

add_executable(${PROJECT_NAME} 
                src/main.cpp 
                src/mySQLTest.cpp 
                src/mySQLTest.h 
                src/serverTest.cpp 
                src/serverTest.h)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)


