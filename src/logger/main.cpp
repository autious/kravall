



#include <Logger.hpp>
#include <Handlers.hpp>




#define gfx LogData( LogSystem::LogType::logType_debug,		"gfx" )


void testPrintFunction1()
{
	/*
	logger::SetNewLogHandler( &logger::debugHandler, 
		new FileHandler( logger::LogType::logType_debug, "../../../../riot_log1.txt", true) );
	*/
	for( int i = 0; i < 50; i++ )
		logger::debug << "1 2 3 4 5 6 7 8 9" << std::endl;
}

void testPrintFunction2()
{
	/*
	logger::SetNewLogHandler( &logger::debugHandler, 
		new FileHandler( logger::LogType::logType_debug, "../../../../riot_log2.txt", true) );
	*/

	for( int i = 0; i < 5000; i++ )
		logger::debug << "a b c d e f g h i" << std::endl;
		//std::cout << "a b c d e f g h i" << std::endl;
}

#include <thread>



/*! 
	Contains example code for the log system.
*/
int main(int args, char argch[])
{
	//std::cin.get();


	/*
	logger::SetNewLogHandler( &logger::debugHandler, 
		new FileHandler( logger::LogType::logType_debug, "../../../../riot_log.txt", true) );
	*/
	/*
	std::thread threads[200];
	for( int i = 0; i < 200; i++ )
		threads[i] = (i % 2) == 0 ? std::thread( testPrintFunction1 ) : std::thread( testPrintFunction2 );

	for( int i = 0; i < 200; i++ )
		threads[i].join();
		*/
	/*
	while ( true )
	{
		for( int i = 0; i < 2; i++ )
			testPrintFunction2();
		std::cin.get();
	}
	*/

	//std::cin.get();


	//std::thread one( testPrintFunction1 );
	//std::thread two( testPrintFunction2 );
	//one.join();
	//two.join();

	//logger::debug << " detta ska gå till en fil! " << std::endl;	

	

	LogSystem::SetNewLogHandler( &LogSystem::debugHandler, new ConsoleHandler( LogSystem::LogType::logType_error ) );

	LogSystem::Mute( "gfx" );
	logger::gfx << "this should not be visable" << std::endl;
	LogSystem::Unmute( "gfx" );

	logger::gfx << "this uses a new prefix to print messages" << std::endl;
	logger::debug << "well, this is exciting, ppl : " << 55454 << std::endl;
	logger::error << "well, this is exciting, ppl : " << 55454 << std::endl;
	logger::warning << "well, this is exciting, ppl : " << 55454 << std::endl;
	logger::fatal << "well, this is exciting, ppl : " << 55454 << std::endl;
	

	std::cin.get();
	return 0;
}

