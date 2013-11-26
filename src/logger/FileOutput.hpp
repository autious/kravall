#ifndef FILEOUTPUTHPP
#define FILEOUTPUTHPP

#include "OutputObject.hpp"

#include <fstream>

class FileOutput : public OutputObject
{
public:
	FileOutput( const char* filePath );
	~FileOutput();

	void Log(const char* message) override;
	
private:
	bool m_isOpen;
	std::fstream m_file;
};










#endif