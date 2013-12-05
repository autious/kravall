#include "internal/ClopHandler.hpp"


#include <iostream>
#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

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
    std::string str( message );
    str.erase(std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), str.end());
    m_clopCallback( m_type, str.c_str() );
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
