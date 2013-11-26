


#include <Logger.hpp>


void loggerprint(char text[])
{
	std::cout << text << std::endl;
}



int main(int args, char argch[])
{
	loggerprint( "Logger is working" );


	return 0;
}