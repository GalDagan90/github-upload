/****************************************************************************
*Author : Gal Dagan
*Date : 4.1.22
*Reviewer : Omer
*
*Description : Logger API
*
*Lab : RD5678
****************************************************************************/
#ifndef __LOGGER_H_RD5678_ILRD__
#define __LOGGER_H_RD5678_ILRD__

#include <fstream>

#include "singleton.hpp"
#include "thread_pool.hpp"


namespace framework
{

class Logger : private boost::noncopyable   
{

public:
    enum Severity
	{
		INFO,
		WARNING,
		ERROR
	};

	void SetFilePath(const std::string& filePath);
	void SetSeverityThreshold(Severity sev);

	void Log(const std::string& txt, Severity sev);

private:
	Logger();
	~Logger();

	ThreadPool m_loggingThread;
	std::string m_filePath;
	Severity m_sev;
	std::ofstream m_ofstream;
	boost::mutex m_mutex;

	friend class Singleton<Logger>;

	class MsgTask;
	class ChangeFile;
	class ChangeSeverity;
	class WaitTask;
};


}//framework


#endif /* __LOGGER_H_RD5678_ILRD__ */
