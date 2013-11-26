#ifndef CONSOLEOUTPUTHPP
#define CONSOLEOUTPUTHPP

#include "OutputObject.hpp"

class ConsoleOutput : public OutputObject
{
public:
	ConsoleOutput();
	~ConsoleOutput();

	void Log( const char* message ) override;


};










#endif