/****************************************************************************
*Author : Gal Dagan
*Date : 15/06/2022

*****************************************************************************/
#include <iostream>

#include "logger.hpp"

namespace design_pattern
{

Logger::Logger(const std::string& fileAddress) : 
		m_fileAddress{fileAddress}, m_isOpen{false}
{
	m_file.open(m_fileAddress, std::ios::out);
	if (m_file)
	{
		m_isOpen = true;
	}
}

Logger::~Logger()
{
	if (m_isOpen)
	{
		m_file.close();
		m_isOpen = false;
	}
}

void Logger::SetAddress(const std::string& address)
{
	if (m_isOpen)
	{
		m_file.close();
		m_file.open(address, std::ios::out);
	}

	m_fileAddress = std::move(address);
}

void Logger::Log(const std::string& msg)
{
	m_file << msg;
}


} // namespace design_pattern