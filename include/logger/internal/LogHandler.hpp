#ifndef LOGHANDLERHPP
#define LOGHANDLERHPP

#include <string>
#include <logger/internal/LogData.hpp>


#ifdef _WIN32
	#ifdef LOGGER_DLL_EXPORT
		#define DLLSETTING __declspec(dllexport)
	#else 
		#define DLLSETTING __declspec(dllimport)
	#endif
#else 
	#define DLLSETTING
#endif

/*!
	Virutal object for indirection of output options. 
	Parent for ConsoleHandler and FileHandler etc. 
*/
class LogHandler
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
