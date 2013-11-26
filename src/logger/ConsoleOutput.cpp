#include "ConsoleOutput.hpp"

#include <iostream>

ConsoleOutput::ConsoleOutput()
{
}


ConsoleOutput::~ConsoleOutput()
{
}


void ConsoleOutput::Log( const char* message )
{
	std::cout << message;
}
