#ifndef CONSOLEHANDLER
#define CONSOLEHANDLER

#include "LogHandler.hpp"


class ConsoleHandler : public LogHandler
{
public:
	ConsoleHandler();
	ConsoleHandler( LogSystem::LogType type );
	~ConsoleHandler();

	virtual void Log( std::string message ) override;

private:


};




#endif