#ifndef CONSOLEHANDLERHPP
#define CONSOLEHANDLERHPP

#include <internal/LogHandler.hpp>

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
	Potential child to LogHandler and will print messages to std::cout
*/
class ConsoleHandler : public LogHandler
{
public:
	DLLSETTING ConsoleHandler();

	/*!	\param Type is the channel to use this object. */
	DLLSETTING ConsoleHandler( LogSystem::LogType type );
	DLLSETTING virtual ~ConsoleHandler() override;

	/*! \param message will be printed to std::cout */
	DLLSETTING  virtual void Log( const char* message ) override;

private:


};

#undef DLLSETTING
#endif
