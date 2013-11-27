#include <internal/LogHandler.hpp>

#include <ostream>
#include <iostream>

LogHandler::LogHandler()
{
	m_type = LogSystem::LogType::logType_debug;
}

LogHandler::~LogHandler()
{
}

void LogHandler::Log( const char* message )
{
}