#include "internal/LogData.hpp"

#include "internal/LogHandler.hpp"
#include "internal/ConsoleHandler.hpp"
#include <sstream>

#include <mutex>


LogHandler* LogSystem::debugHandler		= new ConsoleHandler( LogSystem::LogType::logType_debug );
LogHandler* LogSystem::fatalHandler		= new ConsoleHandler( LogSystem::LogType::logType_fatal );
LogHandler* LogSystem::errorHandler		= new ConsoleHandler( LogSystem::LogType::logType_error );
LogHandler* LogSystem::warningHandler	= new ConsoleHandler( LogSystem::LogType::logType_warning );


char LogSystem::ignoreList[1024];
struct IgnoreListnitializer
{
	IgnoreListnitializer()
	{
		LogSystem::ignoreList[0] = '\0';
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

void LogSystem::SetNewLogHandler( LogHandler** handlerChannel, LogHandler* newHandler )
{
	logMutex.lock();

	if( *handlerChannel )
		delete *handlerChannel;
	*handlerChannel = newHandler;

	logMutex.unlock();
}

LogSystem::LogData& operator<< ( LogSystem::LogData& data, StandardEndLine obj )
{
	std::stringstream ss;
	ss << data.m_message;
	obj( ss );
	
	std::string msg = ss.str();
	std::strcpy( data.m_message, msg.c_str() );

	return data;
}

LogSystem::LogData::LogData(LogType type, const char* prefix )
{
	m_type = type;
	m_prefix = (char*)prefix;
	m_message[0] = '\0';
}

LogSystem::LogData::~LogData()
{
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
	ss << m_prefix << ":: " << m_message;
	std::string msg = ss.str();

	switch( m_type )
	{
	case LogType::logType_debug :
		if( LogSystem::debugHandler )
			LogSystem::debugHandler->Log( msg.c_str() );
		break;

	case LogType::logType_fatal :
		if( LogSystem::fatalHandler )
			LogSystem::fatalHandler->Log( msg.c_str() );
		break;

	case LogType::logType_error :
		if( LogSystem::errorHandler )
			LogSystem::errorHandler->Log( msg.c_str() );
		break;

	case LogType::logType_warning :
		if( LogSystem::warningHandler )
			LogSystem::warningHandler->Log( msg.c_str() );
		break;
	}

	logMutex.unlock();
}