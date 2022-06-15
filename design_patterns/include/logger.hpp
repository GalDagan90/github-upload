/****************************************************************************
*Author : Gal Dagan
*Date : 15/06/2022

****************************************************************************/
#ifndef __LOGGER_H_RD5678_ILRD__
#define __LOGGER_H_RD5678_ILRD__

#include <string>
#include <fstream>

namespace design_pattern
{

class Logger
{
public:
	Logger(const std::string& fileAddress = "/home/user/github/design_patterns/logger.txt");
	~Logger();

	Logger(const Logger& other) = delete;
	Logger& operator=(const Logger& other) = delete;
	Logger(Logger&& other) = delete;
	Logger& operator=(Logger&& other) = delete;

	void SetAddress(const std::string& address);
	void Log(const std::string& msg);

private:
	std::string m_fileAddress;
	bool m_isOpen;
	std::fstream m_file;
};



} // namespace ilrd_5678


#endif /* __LOGGER_H_RD5678_ILRD__ */
