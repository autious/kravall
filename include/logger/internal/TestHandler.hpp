#ifndef TESTHANDLERHPP
#define TESTHANDLERHPP

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
	Potential child to LogHandler and will not print anything, used for automated tests
*/
class TestHandler : public LogHandler
{
public:
	TestHandler() { }

	virtual ~TestHandler() override { }

	/*! function does nothing, used for automated tests */
	virtual void Log( const char* message ) override
	{
	}

private:

};

#undef DLLSETTING
#endif
