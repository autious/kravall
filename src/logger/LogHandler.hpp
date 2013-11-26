#ifndef LOGHANDLERHPP
#define LOGHANDLERHPP


#include <string>
#include "LogData.hpp"


class LogHandler
{
public:
	LogHandler();
	~LogHandler();

	virtual void Log( std::string message ) { }

protected:

	LogSystem::LogType m_type;


};




#endif