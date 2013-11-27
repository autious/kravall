


#include <Logger.hpp>
#include <string>

#include <iostream>


#include <logger.hpp>

#include <handlers.hpp>


#define gfx LogData( LogSystem::LogType::logType_debug,		"gfx" )


int main(int args, char argch[])
{
	LogSystem::SetNewLogHandler( &LogSystem::debugHandler, new ConsoleHandler( LogSystem::LogType::logType_error ) );

	LogSystem::Mute( "gfx" );
	LogSystem::Unmute( "gfx" );

	logger::gfx << "det är fel i grafiken!" << std::endl;
	logger::debug << "well, this is exciting, ppl : " << 55454 << std::endl;
	logger::error << "well, this is exciting, ppl : " << 55454 << std::endl;
	logger::warning << "well, this is exciting, ppl : " << 55454 << std::endl;
	logger::fatal << "well, this is exciting, ppl : " << 55454 << std::endl;
	


	std::cin.get();
	return 0;
}

