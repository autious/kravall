#include "internal/ClopHandler.hpp"


#include <iostream>

ClopHandler::ClopHandler( clopback * cb )
{
	m_type = LogSystem::LogType::logType_debug;
    m_clopCallback = cb;
}

ClopHandler::ClopHandler( clopback * cb, LogSystem::LogType type)
{
	m_type = type;
    m_clopCallback = cb;
}


ClopHandler::~ClopHandler()
{
}


void ClopHandler::Log( const char* message )
{
    m_clopCallback( m_type, message );
    /*
     switch( m_type )
     {
     case LogSystem::LogType::logType_debug :
         break;

    case LogSystem::LogType::logType_error :
         break;

    case LogSystem::LogType::logType_fatal :
         break;

    case LogSystem::LogType::logType_warning :
         break;

     }
    */
}
