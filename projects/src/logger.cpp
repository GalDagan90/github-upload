/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/
#include <boost/chrono/system_clocks.hpp>
#include <time.h>
#include <iostream>

#include "logger.hpp"

namespace framework
{

static std::string PrepareMsgForLogging(const std::string& txt, 
											int m_sev);
static std::string GetTime();

static const std::string g_sevirityLUT[] = {" -INFO - ", 
											" - WARNING - ",
											" - ERROR - "};
/******************************************************************************
*								class MsgTask	    						  *
*******************************************************************************/

class Logger::MsgTask : public ThreadPool::Task
{
public:
	MsgTask(Logger& logger, const std::string& txt) 
		: m_logger(logger), m_logMsg(txt)
	{
		//empty ctor
	}

	void operator()()
	{
		m_logger.m_ofstream << m_logMsg << std::endl;
	}


private:
	Logger& m_logger;
	std::string m_logMsg;
};

std::string PrepareMsgForLogging(const std::string& txt, int sev)
{
	return (GetTime() + " " + txt + g_sevirityLUT[sev]); 
}

std::string GetTime()
{
	time_t curr_time;
	tm *curr_time_ptr;
	char string_buffer[20];

	time(&curr_time);
	curr_time_ptr = localtime(&curr_time);

	std::strftime(string_buffer, 20, "%d/%m/%Y %H:%M:%S", curr_time_ptr);

	return string_buffer;
}

/******************************************************************************
*								class ChangeFile    						  *
*******************************************************************************/

class Logger::ChangeFile : public ThreadPool::Task
{
public:
	ChangeFile(Logger& logger, const std::string& filePath) 
		: m_logger(logger), m_newFilePath(filePath)
	{
		//empty ctor
	}

	void operator()()
	{
		if (m_logger.m_ofstream.is_open())
		{
			m_logger.m_ofstream.close();
		}
		m_logger.m_filePath = m_newFilePath;
		m_logger.m_ofstream.open(m_logger.m_filePath.c_str(),
					  std::ofstream::out | std::ofstream::app);
	}


private:
	Logger& m_logger;
	std::string m_newFilePath;
};


/******************************************************************************
*								class ChangeSeverity   						  *
*******************************************************************************/

class Logger::ChangeSeverity : public ThreadPool::Task
{
public:
	ChangeSeverity(Logger& logger, Severity newSev)
		: m_logger(logger), m_newSeverity(newSev)
	{
		//empty ctor
	}

	void operator()()
	{
		m_logger.m_sev = m_newSeverity;
	}


private:
	Logger& m_logger;
	Severity m_newSeverity;
};

/*******************************************************************************
*								class WaitTask								   *
*******************************************************************************/
class Logger::WaitTask : public ThreadPool::Task
{
public:
    WaitTask(boost::interprocess::interprocess_semaphore& sem) : m_sem(sem)
    {
        //Empty Body
    }

    void operator()()
    {
		m_sem.post();
    }

private:
	boost::interprocess::interprocess_semaphore& m_sem;
};

/*******************************************************************************
*								class Logger								   *
*******************************************************************************/

Logger::Logger() 
	: m_loggingThread(1), m_filePath("./logger.txt"), 
	m_sev(INFO), m_ofstream()
{
	m_ofstream.open(m_filePath.c_str(), std::ofstream::out | std::ofstream::app);
	m_loggingThread.Run();
}

Logger::~Logger()
{
	boost::interprocess::interprocess_semaphore m_sem(0);

	boost::shared_ptr<ThreadPool::Task> task(new WaitTask(m_sem));
	m_loggingThread.AddTask(task, 
						static_cast<ThreadPool::Priority>(ThreadPool::LOW - 1));

	m_sem.wait();

	if (m_ofstream.is_open())
	{
		m_ofstream.close();
	}
}

void Logger::SetFilePath(const std::string& filePath)
{
	boost::mutex::scoped_lock lock(m_mutex);
	boost::shared_ptr<ThreadPool::Task> task(new ChangeFile(*this, filePath));
	m_loggingThread.AddTask(task);
}

void Logger::SetSeverityThreshold(Severity sev)
{
	boost::mutex::scoped_lock lock(m_mutex);
	boost::shared_ptr<ThreadPool::Task> task(new ChangeSeverity(*this, sev));
	m_loggingThread.AddTask(task);
}

void Logger::Log(const std::string& txt, Severity sev)
{
	boost::mutex::scoped_lock lock(m_mutex);
	if (sev >= m_sev)
	{
		std::string msg = PrepareMsgForLogging(txt, sev);
		boost::shared_ptr<ThreadPool::Task> task(new MsgTask(*this, msg));
		m_loggingThread.AddTask(task);
	}
}

/******************************************************************************/

}//framework