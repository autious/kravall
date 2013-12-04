#include "internal/ConsoleHandler.hpp"


#include <iostream>



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


void ConsoleHandler::Log( const char* message )
{

     /*
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
     */

#ifdef LOGGER_LINUX_CONSOLE

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

     switch( m_type )
     {
     case LogSystem::LogType::logType_debug :
         break;

    case LogSystem::LogType::logType_error :
         std::cout << KRED;
         break;

    case LogSystem::LogType::logType_fatal :
         std::cout << KRED;
         break;

    case LogSystem::LogType::logType_warning :
         std::cout << KYEL;
         break;

     }
#endif
    std::cout << message;
#ifdef LOGGER_LINUX_CONSOLE
    std::cout << KNRM;
#endif
}
