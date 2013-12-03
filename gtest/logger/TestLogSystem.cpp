#ifdef RUN_GTEST
#include <gtest/gtest.h>
#include <logger/Handlers.hpp>
#include <logger/Logger.hpp>

#include <fstream>
#include <sstream>
#include <string>

#ifdef __WIN32
#include <windows.h>
#include <psapi.h>
#endif

namespace 
{
	TEST(LogSystemTest, FileTest )
	{

#ifdef __WIN32
		logger::SetNewLogHandler( &logger::debugHandler, 
			new FileHandler( logger::LogType::logType_debug, "loggerTestFile.txt", false) );

		logger::debug << "This file is just for testing " << 123456789 << std::endl; 
		
		// make sure the filehandler closes the conection to the file...
		logger::SetNewLogHandler( &logger::debugHandler, 
			new ConsoleHandler() );

		std::fstream ff;
		ff.open( "loggerTestFile.txt", std::fstream::in );
		ASSERT_TRUE( ff.is_open() );
		
		std::string fromFile;
		std::stringstream ss;
		while( true )
		{
			ff >> fromFile;
			if( ff.eof() )
				break;
			ss << fromFile << " ";
		}

		fromFile = ss.str();

		std::string test = std::string(logger::debug.GetPrefix()) + ":: This file is just for testing 123456789 ";
		int diff = fromFile.compare( test );
		ASSERT_TRUE( diff == 0 );
#endif
	}

	TEST(LogSystemTest, TestMemory )
	{
#ifdef __WIN32
		SIZE_T before;
		SIZE_T after;
		
		{
			PROCESS_MEMORY_COUNTERS_EX pmc;		
			GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
			before = pmc.PrivateUsage;
		}

		logger::SetNewLogHandler( &logger::debugHandler, 
			new TestHandler() );

		for( int i = 0; i < 1000; i++ )
			logger::debug << " this is a test message " << "to check for memory" << " leakage in the log system" << std::endl;

		{
			PROCESS_MEMORY_COUNTERS_EX pmc;		
			GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
			after = pmc.PrivateUsage;
		}

		logger::SetNewLogHandler( &logger::debugHandler, 
			new ConsoleHandler() );

		int diff = after - before;

		ASSERT_FALSE( diff );
#endif
	}
}
#endif
