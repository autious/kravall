#ifndef LOGHANDLERHPP
#define LOGHANDLERHPP

#include <string>
#include <internal/LogData.hpp>

#ifdef LOGGER_DLL_EXPORT
#define DLLSETTING __declspec(dllexport)
#else 
#define DLLSETTING __declspec(dllimport)
#endif

/*!
	Virutal object for indirection of output options. 
	Parent for ConsoleHandler and FileHandler etc. 
*/
DLLSETTING class LogHandler
{
public:
	DLLSETTING LogHandler();
	DLLSETTING virtual ~LogHandler();

	/*! 
		Overloaded by child object and will print message accordingly.
	*/
	DLLSETTING virtual void Log( const char* message );

protected:
	LogSystem::LogType m_type;
};



#undef DLLSETTING
#endif