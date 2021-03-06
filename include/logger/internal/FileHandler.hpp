#ifndef FILEHANDLERHPP
#define FILEHANDLERHPP

#include <fstream>
#include <logger/internal/LogHandler.hpp>

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
	DLLSETTING FileHandler( LogSystem::LogType type, std::string path, bool append );
	
	virtual ~FileHandler() override;

	/*! 
		\param message will print message to file if file is open 
	*/
	virtual void Log( const char* message ) override;

private:
	std::fstream m_file;

};



#undef DLLSETTING
#endif
