#ifndef LOGHANDLERHPP
#define LOGHANDLERHPP

#include <string>
#include <internal/LogData.hpp>

#ifdef LOGGER_DLL_EXPORT
#define DLLSETTING __declspec(dllexport)
#else 
#define DLLSETTING __declspec(dllimport)
#endif

DLLSETTING class LogHandler
{
public:
	DLLSETTING LogHandler();
	DLLSETTING virtual ~LogHandler();

	DLLSETTING virtual void Log( const char* message );

protected:
	LogSystem::LogType m_type;
};



#undef DLLSETTING
#endif