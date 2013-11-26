#include "LogData.hpp"

#include "LogHandler.hpp"
#include "ConsoleHandler.hpp"

LogHandler* LogSystem::debugHandler		= new ConsoleHandler( LogSystem::LogType::logType_debug );
LogHandler* LogSystem::fatalHandler		= new ConsoleHandler( LogSystem::LogType::logType_fatal );
LogHandler* LogSystem::errorHandler		= new ConsoleHandler( LogSystem::LogType::logType_error );
LogHandler* LogSystem::warningHandler	= new ConsoleHandler( LogSystem::LogType::logType_warning );

#undef debug	
#undef fatal	
#undef error	
#undef warning	

LogSystem::LogData LogSystem::debug(	LogSystem::LogType::logType_debug );
LogSystem::LogData LogSystem::fatal(	LogSystem::LogType::logType_fatal );
LogSystem::LogData LogSystem::error(	LogSystem::LogType::logType_error );
LogSystem::LogData LogSystem::warning(  LogSystem::LogType::logType_warning );


LogSystem::LogData& operator<< ( LogSystem::LogData& data, StandardEndLine obj )
{
	obj(data.GetData());
	return data;
}

LogSystem::LogData::LogData(LogType type)
{
	m_type = type;
}

LogSystem::LogData::~LogData()
{
	std::string message = this->ss.str();

	switch( m_type )
	{
	case LogType::logType_debug :
		LogSystem::debugHandler->Log( message );
		break;

	case LogType::logType_fatal :
		LogSystem::fatalHandler->Log( message );
		break;

	case LogType::logType_error :
		LogSystem::errorHandler->Log( message );
		break;

	case LogType::logType_warning :
		LogSystem::warningHandler->Log( message );
		break;
	}
	
	this->ss.flush();
}