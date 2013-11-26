#ifndef LOGDATAHPP
#define LOGDATAHPP

#include <iostream>
#include <ostream>
#include <sstream>

class LogHandler;

namespace LogSystem
{
	enum LogType
	{
		logType_debug,
		logType_fatal,
		logType_error,
		logType_warning,
	};

	class LogData
	{
	public:
		LogData( LogType type );
		~LogData();

		std::stringstream& GetData() { return ss; }

	private:
		std::stringstream ss;
		LogType m_type;
	};
	
	extern LogData debug;
	extern LogData fatal;
	extern LogData error;
	extern LogData warning;
	
	extern LogHandler* debugHandler;
	extern LogHandler* fatalHandler;
	extern LogHandler* errorHandler;
	extern LogHandler* warningHandler;
}

typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
typedef CoutType& (*StandardEndLine)(CoutType&);
LogSystem::LogData& operator<< ( LogSystem::LogData& data, StandardEndLine obj );

template < class T >
LogSystem::LogData& operator<< ( LogSystem::LogData& data, const T& obj )
{
	data.GetData() << obj;
	return data;
}

#define debug	LogData( LogSystem::LogType::logType_debug )
#define fatal	LogData( LogSystem::LogType::logType_fatal )
#define error	LogData( LogSystem::LogType::logType_error )
#define warning	LogData( LogSystem::LogType::logType_warning )
#define logger	LogSystem

#endif