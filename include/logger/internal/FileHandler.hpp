#ifndef FILEHANDLERHPP
#define FILEHANDLERHPP

#include <iostream>
#include <fstream>
#include <internal/LogHandler.hpp>

#ifdef _WIN32
	#ifdef LOGGER_DLL_EXPORT
		#define DLLSETTING __declspec(dllexport)
	#else 
		#define DLLSETTING __declspec(dllimport)
	#endif
#else 
	#define DLLSETTING
#endif

/*!
	Potential child of LogHandler and will print messages to file. 
*/
class FileHandler : public LogHandler
{
public:
	DLLSETTING FileHandler();

	/*!	
	\param Type is the channel to use this object. 
	\param Path is the relative path to the file.
	\param Append will cause the messages to be appended to the file.
	*/
	FileHandler( LogSystem::LogType type, std::string path, bool append )
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
	
	virtual ~FileHandler() override
	{
		if( m_file.is_open() )
			m_file.close();
	}

	/*! 
		\param message will print message to file if file is open 
	*/
	virtual void Log( const char* message ) override
	{
		if( m_file.is_open() )
		 m_file << message;
	}

private:
	std::fstream m_file;

};



#undef DLLSETTING
#endif
