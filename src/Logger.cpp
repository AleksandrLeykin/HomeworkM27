#include "Logger.h"
#include <ctime>
#include <chrono>

Logger::Logger()
{
	//ios::in: файл открывается для ввода (чтения), 
	// ios::app: файл открывается для дозаписи. Старые данные не удаляются.
	m_log.open(m_logfile, std::ios::in | std::ios::app);
}

Logger::~Logger()
{
	m_log.close();
}

void Logger::recordLogFile(const std::string& str) {
	//защищаем потоки	
	m_mutex.lock();
		
	if (m_log.is_open()) {
		//пробую удалить окончоние строки
		std::string str1 = fileWriteTime();
		str1 = str1.erase(str1.size() - 1);
		m_log << str1 << " " << str << "\n";
	}

	m_mutex.unlock();
}

void Logger::readLogFile()
{
	std::this_thread::sleep_for(std::chrono::microseconds(100));
	m_mutex.lock_shared();
	// окрываем файл для чтения
	if (m_log.is_open()) {
		while (std::getline(m_log, m_logEntry)) {
			std::cout << m_logEntry << std::endl;
		}
	}
	m_mutex.unlock_shared();
}

std::string fileWriteTime()
{
	time_t now = time(0);
	return ctime(&now);
}
