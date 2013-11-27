#ifndef CONSOLEHANDLERHPP
#define CONSOLEHANDLERHPP

#include <internal/LogHandler.hpp>

#ifdef LOGGER_DLL_EXPORT
#define DLLSETTING __declspec(dllexport)
#else 
#define DLLSETTING __declspec(dllimport)
#endif


DLLSETTING class ConsoleHandler : public LogHandler
{
public:
	DLLSETTING ConsoleHandler();
	DLLSETTING ConsoleHandler( LogSystem::LogType type );
	DLLSETTING virtual ~ConsoleHandler() override;

	DLLSETTING virtual void Log( const char* message ) override;

private:


};



#undef DLLSETTING
#endif