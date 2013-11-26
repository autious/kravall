#ifndef LOGGERSYSTEMHPP
#define LOGGERSYSTEMHPP

#include "OutputObject.hpp"
#include <ostream>
#include <sstream>

enum TypeOfLogOutput
{
	Console,
	File,
};

class LoggData
{
public:
	LoggData( std::string prefix ) { m_ss << prefix << "::  "; }
	std::stringstream m_ss;
};

class LoggerSystem
{
public:
	LoggerSystem();
	LoggerSystem( TypeOfLogOutput type, std::string prefix );
	~LoggerSystem();

	void Log( const char* message ) { m_output->Log( message ); } 
	void SetPrefix( std::string prefix ) { m_prefix = prefix; }
	std::string GetPrefix() { return m_prefix; }

private:
	OutputObject* m_output;
	std::string m_prefix;
};

/*
template < class T >
LoggerSystem& operator << (LoggerSystem& logger, const T& obj)
{	
	std::stringstream ss( obj );
	logger.Log( ss.str().c_str() );
	return logger;
}
*/
//template < class T >
LoggerSystem& operator << (LoggerSystem& logger, const LoggData data);

template < class T >
LoggData& operator << (LoggData& data, const T& obj)
{
	data.ss << obj;
	return data;
}



#endif