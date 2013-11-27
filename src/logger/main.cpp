


#include <Logger.hpp>
#include <string>

#include <iostream>


#include <logger.hpp>

#include <handlers.hpp>




#define gfx LogData( LogSystem::LogType::logType_debug,		"gfx" )

/*! 
	Contains example code for the log system.
*/
int main(int args, char argch[])
{

	logger::SetNewLogHandler( &logger::debugHandler, 
		new FileHandler( logger::LogType::logType_debug, "../../../../riot_log.txt", true) );

	logger::debug << " detta ska gå till en fil! " << std::endl;	

	LogSystem::SetNewLogHandler( &LogSystem::debugHandler, new ConsoleHandler( LogSystem::LogType::logType_error ) );

	LogSystem::Mute( "gfx" );
	LogSystem::Unmute( "gfx" );

	logger::gfx << "this uses a new prefix to print messages" << std::endl;
	logger::debug << "well, this is exciting, ppl : " << 55454 << std::endl;
	logger::error << "well, this is exciting, ppl : " << 55454 << std::endl;
	logger::warning << "well, this is exciting, ppl : " << 55454 << std::endl;
	logger::fatal << "well, this is exciting, ppl : " << 55454 << std::endl;
	

	std::cin.get();
	return 0;
}

