#ifndef LOGDATAHPP
#define LOGDATAHPP



#ifdef _WIN32
	#ifdef LOGGER_DLL_EXPORT
	#define DLLSETTING __declspec(dllexport)
	#else 
	#define DLLSETTING __declspec(dllimport)
	#endif
#else 
#define DLLSETTING
#endif


#include <iostream>
#include <ostream>
#include <sstream>

class LogHandler;

/*! 
	Namespace for the logging system and overridden by macro to "logger"
	Use is "logger::youAction", eg. 
	logger::debug << "this is a debug message" << std::endl;
*/
namespace LogSystem
{
	/*!
		Channels for output by the log system.
	*/
	enum LogType
	{
		logType_debug,
		logType_fatal,
		logType_error,
		logType_warning,
	};

	/*!
		Container object for one "<<" chain. Should only be used via macros.
	*/
	DLLSETTING class LogData
	{
	public:
		/*!
			\param type is which channel to use.
			\param prefix is what prefix to use. Should be set via macro. Messages can be muted via this prefix.
		*/
		LogData( LogType type, const char* prefix );

		/*!
			Once the LogData object is destroyed the printing goes into 
			action using other utility in the LogSystem namespace.
		*/
		~LogData();

		char m_message[512];
		char* m_prefix;

	private:		
		LogType m_type;
	};
	
	/*! only here for intelisense help, will be overrided by macro. */
	extern LogData debug;
	/*! only here for intelisense help, will be overrided by macro. */
	extern LogData fatal;
	/*! only here for intelisense help, will be overrided by macro. */
	extern LogData error;
	/*! only here for intelisense help, will be overrided by macro. */
	extern LogData warning;
	
	/*! channel for 'debug' messages, default is ConsoleHandler */
	DLLSETTING extern LogHandler* debugHandler;
	/*! channel for 'fatal' messages, default is ConsoleHandler */
	DLLSETTING extern LogHandler* fatalHandler;
	/*! channel for 'error' messages, default is ConsoleHandler */
	DLLSETTING extern LogHandler* errorHandler;
	/*! channel for 'warning' messages, default is ConsoleHandler */
	DLLSETTING extern LogHandler* warningHandler;

	/*!
		\param prefix to be muted, same one as stated in the macro function for that channel. eg. "debug"
	*/
	DLLSETTING void Mute( const char* prefix );
	
	/*!
		\param prefix to be unmuted, same one as stated in the macro function for that channel. eg. "debug"
	*/
	DLLSETTING void Unmute( const char* prefix );

	/*!
		Will delete the current object and replace it with the new object.
		\param handlerChannel is the channel to replace, eg. LogSystem::debugHandler
		\param newHander is a pointer to the new hadler for this channel. eg. ConsoleHandler.
	*/
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
	std::strcpy( data.m_message, msg.c_str() );

	return data;
}


/*! Macros overriding logger::debug etc. to use a temp object. */
#define debug	LogData( LogSystem::LogType::logType_debug,		"debug" )
#define fatal	LogData( LogSystem::LogType::logType_fatal,		"fatal" )
#define error	LogData( LogSystem::LogType::logType_error,		"error" )
#define warning	LogData( LogSystem::LogType::logType_warning,	"warning" )
#define logger	LogSystem

// add more here at need...




#undef DLLSETTING
#endif
