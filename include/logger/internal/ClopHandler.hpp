#ifndef CLOPHANDLERHPP
#define CLOPHANDLERHPP

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
	Potential child to LogHandler and will print messages to std::cout
*/
typedef void (clopback)( LogSystem::LogType type, const char * message );

class ClopHandler : public LogHandler
{
public:
	DLLSETTING ClopHandler( clopback* cb);

	/*!	\param Type is the channel to use this object. */
	DLLSETTING ClopHandler( clopback* cb, LogSystem::LogType type );
	DLLSETTING virtual ~ClopHandler() override;

	/*! \param message will be printed to std::cout */
	DLLSETTING  virtual void Log( const char* message ) override;

private:
    clopback* m_clopCallback;


};

#undef DLLSETTING
#endif
