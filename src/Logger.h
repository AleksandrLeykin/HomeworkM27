#pragma once

#include <iostream>
#include <fstream>
#include <shared_mutex>
#include <string>

//отметка времени timestamp
std::string fileWriteTime();

class Logger {
public:
	Logger();
	~Logger();

	//запись в файл write to file
	void recordLogFile(const std::string& str);
	//reading from file
	void readLogFile();

private:	
	std::fstream m_log;
	std::shared_mutex m_mutex;
	std::string m_logEntry = "";	
#if defined (__linux__)
	std::string m_logfile = "./log.txt";
#elif defined (_WIN64)
	std::string m_logfile = "log.txt";
#endif
};