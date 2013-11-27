#include "internal/FileHandler.hpp"


#include <iostream>


FileHandler::FileHandler()
{
	m_type = LogSystem::LogType::logType_debug;
}

FileHandler::FileHandler( LogSystem::LogType type, std::string prefix )
{
	m_type = type;
}


FileHandler::~FileHandler()
{
}


 void FileHandler::Log( const char* message )
 {
	 std::cout << message;
 }