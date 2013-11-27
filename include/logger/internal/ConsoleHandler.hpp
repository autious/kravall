#ifndef CONSOLEHANDLERHPP
#define CONSOLEHANDLERHPP

#include <internal/LogHandler.hpp>


class ConsoleHandler : public LogHandler
{
public:
	ConsoleHandler();
	ConsoleHandler( LogSystem::LogType type );
	virtual ~ConsoleHandler() override;

	virtual void Log( const char* message ) override;

private:


};




#endif