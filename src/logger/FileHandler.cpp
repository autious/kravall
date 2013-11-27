#include "internal/FileHandler.hpp"


#include <iostream>


FileHandler::FileHandler()
{
	m_type = LogSystem::LogType::logType_debug;
}

FileHandler::FileHandler( LogSystem::LogType type, std::string path, bool append )
{
	m_type = type;
	m_file.open( path, std::fstream::out | (append ? std::fstream::app : std::fstream::trunc ) );

	if( !m_file.is_open() )
	{
		std::cout << "FileHandler cannot open path for debug output: " << path << std::endl;
		return;
	}

	if( append )
		m_file << "=========" << std::endl << "=========" << std::endl;
}


FileHandler::~FileHandler()
{
	if( m_file.is_open() )
		m_file.close();
}


 void FileHandler::Log( const char* message )
 {
	 if( m_file.is_open() )
		 m_file << message;
 }