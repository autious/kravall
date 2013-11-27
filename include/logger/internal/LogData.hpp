#ifndef LOGDATAHPP
#define LOGDATAHPP

#ifdef LOGGER_DLL_EXPORT
#define DLLSETTING __declspec(dllexport)
#else 
#define DLLSETTING __declspec(dllimport)
#endif


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

	DLLSETTING class LogData
	{
	public:
		DLLSETTING LogData( LogType type, const char* prefix );
		DLLSETTING ~LogData();

		char* m_message;
		char* m_prefix;

	private:		
		LogType m_type;
	};
	
	extern LogData debug;
	extern LogData fatal;
	extern LogData error;
	extern LogData warning;
	
	DLLSETTING extern LogHandler* debugHandler;
	DLLSETTING extern LogHandler* fatalHandler;
	DLLSETTING extern LogHandler* errorHandler;
	DLLSETTING extern LogHandler* warningHandler;

	extern char* ignoreList;
	DLLSETTING void Mute( const char* prefix );
	DLLSETTING void Unmute( const char* prefix );

	DLLSETTING void SetNewLogHandler( LogHandler** handlerChannel, LogHandler* newHandler );
}

typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
typedef CoutType& (*StandardEndLine)(CoutType&);
DLLSETTING LogSystem::LogData& operator<< ( LogSystem::LogData& data, StandardEndLine obj );

template < class T >
LogSystem::LogData& operator<< ( LogSystem::LogData& data, const T& obj )
{
	std::stringstream ss;
	ss << data.m_message;
	ss << obj;

	std::string msg = ss.str();
	char* temp = new char[msg.size()];
	std::strcpy( temp, msg.c_str() );
	data.m_message = temp;

	return data;
}

#define debug	LogData( LogSystem::LogType::logType_debug,		"debug" )
#define fatal	LogData( LogSystem::LogType::logType_fatal,		"fatal" )
#define error	LogData( LogSystem::LogType::logType_error,		"error" )
#define warning	LogData( LogSystem::LogType::logType_warning,	"warning" )

// add more here at need...




#define logger	LogSystem

#undef DLLSETTING
#endif
