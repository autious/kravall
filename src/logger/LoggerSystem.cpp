#include "LoggerSystem.hpp"


#include "ConsoleOutput.hpp"
#include "FileOutput.hpp"

LoggerSystem& operator << (LoggerSystem& logger, const LoggData data)
{
	logger.Log(data.m_ss.str().c_str());
	return logger;
}

LoggerSystem::LoggerSystem()
{
	m_prefix = "debug";
	m_output = new ConsoleOutput();
}

LoggerSystem::LoggerSystem( TypeOfLogOutput type, std::string prefix )
{
	m_prefix = prefix;
	std::string path = "../../../logs/" + m_prefix;

	if (m_output)
		delete m_output;

	switch( type )
	{
	case TypeOfLogOutput::Console :
		m_output = new ConsoleOutput();
		break;

	case TypeOfLogOutput::File :
		m_output = new FileOutput( path.c_str() );
		break;


		// add new types of output here
		
	}
}


LoggerSystem::~LoggerSystem()
{
	if( m_output )
		delete m_output;
}
