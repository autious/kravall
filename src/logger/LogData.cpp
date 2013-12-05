#include "internal/LogData.hpp"

#include "internal/LogHandler.hpp"
#include "internal/ConsoleHandler.hpp"
#include <sstream>

#include <mutex>
#include <cassert>


LogHandler* LogSystem::debugHandler[]		= {new ConsoleHandler( LogSystem::LogType::logType_debug ),nullptr,nullptr,nullptr};
LogHandler* LogSystem::fatalHandler[]		= {new ConsoleHandler( LogSystem::LogType::logType_fatal ),nullptr,nullptr,nullptr};
LogHandler* LogSystem::errorHandler[]		= {new ConsoleHandler( LogSystem::LogType::logType_error ),nullptr,nullptr,nullptr};
LogHandler* LogSystem::warningHandler[]	    = {new ConsoleHandler( LogSystem::LogType::logType_warning ),nullptr,nullptr,nullptr};


char ignoreList[1024];

struct IgnoreListnitializer
{
	IgnoreListnitializer()
	{
		ignoreList[0] = '\0';
	}

} onlyUsedToInitIgnoreList;

static std::mutex logMutex;

void LogSystem::Mute( const char* prefix )
{
	logMutex.lock();

	std::stringstream ss;
	ss << prefix << " " << ignoreList;

	std::string msg = ss.str();
	std::strcpy( ignoreList, msg.c_str() );

	logMutex.unlock();
}

void LogSystem::Unmute( const char* prefix )
{
	logMutex.lock();

	std::stringstream ss;
	std::stringstream out;
	ss << ignoreList;

	std::string tt;
	while( ss.rdbuf()->in_avail() != 0 )
	{
		ss >> tt;
		if( std::strcmp( prefix, tt.c_str() ) != 0 )
			out << tt;
	}

	std::string msg = out.str();
	std::strcpy( ignoreList, msg.c_str() );

	logMutex.unlock();
}

void LogSystem::RegisterLogHandler( LogHandler** handlerChannel, LogHandler* newHandler )
{
	logMutex.lock();

    bool worked = false;

    for( int i = 0; i < LOGGER_LIMIT; i++ )
    {
        if( handlerChannel[i] == nullptr )
        {
            handlerChannel[i] = newHandler;
            worked = true;
            break;
        } 
    }

    assert( worked );

	logMutex.unlock();
}

void LogSystem::SetNewLogHandler( LogHandler** handlerChannel, LogHandler* newHandler )
{
	logMutex.lock();

	if( handlerChannel[0] )
		delete handlerChannel[0];
	handlerChannel[0] = newHandler;

	logMutex.unlock();
}

LogSystem::LogData& operator<< ( const LogSystem::LogData& data, StandardEndLine obj )
{
	LogSystem::LogData& temp = (LogSystem::LogData&)data; // unix hack, nab compiler...
	std::stringstream ss;
	ss << data.m_message;
	obj( ss );
	
	std::string msg = ss.str();
	std::strcpy( temp.m_message, msg.c_str() );

	return temp;
}

LogSystem::LogData::LogData(LogType type, const char* prefix )
{
	m_type = type;
	m_prefix = (char*)prefix;
	m_message[0] = '\0';
}

LogSystem::LogData::~LogData()
{
	if( strlen( m_message ) == 0 )
		return;

	logMutex.lock();

	{
		std::stringstream ss;
		ss << ignoreList;

		std::string tt;
		while( ss.rdbuf()->in_avail() != 0 )
		{
			ss >> tt;
			if( std::strcmp( m_prefix, tt.c_str() ) == 0 )
			{
				logMutex.unlock();
				return;
			}
		}
	}
	
	std::stringstream ss;
	ss << "[" << m_prefix << "]:" << m_message;
	std::string msg = ss.str();

    for( int i = 0; i < LOGGER_LIMIT; i++ )
    {
        switch( m_type )
        {
        case LogType::logType_debug :
            if( debugHandler[i] )
                debugHandler[i]->Log( msg.c_str() );
            break;

        case LogType::logType_fatal :
            if( fatalHandler[i] )
                fatalHandler[i]->Log( msg.c_str() );
            break;

        case LogType::logType_error :
            if( errorHandler[i] )
                errorHandler[i]->Log( msg.c_str() );
            break;

        case LogType::logType_warning :
            if( warningHandler[i] )
                warningHandler[i]->Log( msg.c_str() );
            break;
        }
    }

	logMutex.unlock();
}

