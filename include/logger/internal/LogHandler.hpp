#ifndef LOGHANDLERHPP
#define LOGHANDLERHPP


#include <string>
#include <internal/LogData.hpp>


class LogHandler
{
public:
	LogHandler();
	virtual ~LogHandler();

	virtual void Log( const char* message ) { }

protected:
	LogSystem::LogType m_type;
};




#endif