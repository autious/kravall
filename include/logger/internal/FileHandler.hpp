#ifndef FILEHANDLERHPP
#define FILEHANDLERHPP

#include <internal/LogHandler.hpp>


class FileHandler : public LogHandler
{
public:
	FileHandler();
	FileHandler( LogSystem::LogType type, std::string prefix );
	virtual ~FileHandler() override;

	virtual void Log( const char* message ) override;

private:


};




#endif