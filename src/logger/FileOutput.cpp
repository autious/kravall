#include "FileOutput.hpp"


FileOutput::FileOutput( const char* filePath )
{
	m_isOpen = false;
	m_file.open( filePath, std::ios::app | std::ios::out );
	if( m_file.is_open() )
		m_isOpen = true;
}

FileOutput::~FileOutput()
{
	if (m_file.is_open())
		m_file.close();
}

void FileOutput::Log( const char* message )
{
	m_file << message;
}