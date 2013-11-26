#include "ConsoleHandler.hpp"


#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif


ConsoleHandler::ConsoleHandler()
{
	m_type = LogSystem::LogType::logType_debug;
}

ConsoleHandler::ConsoleHandler( LogSystem::LogType type )
{
	m_type = type;
}


ConsoleHandler::~ConsoleHandler()
{
}


 void ConsoleHandler::Log( std::string message )
 {



 #ifdef _WIN32

	 switch( m_type )
	 {
	 case LogSystem::LogType::logType_debug :
		 system( "Color 0A" );
		 break;

	case LogSystem::LogType::logType_error :
		 system( "Color 04" );
		 break;

	case LogSystem::LogType::logType_fatal :
		 system( "Color 0C" );
		 break;

	case LogSystem::LogType::logType_warning :
		 system( "Color 0E" );
		 break;

	 }
	 // c fatal
	 // a debug
	 // e warning	 
	 
#endif


	 std::cout << message;
 }