#ifndef LOGGERHPP
#define LOGGERHPP



#ifdef LOGGER_DLL_EXPORT
#define DLLSETTING __declspec(dllexport)
#else 
#define DLLSETTING __declspec(dllimport)
#endif

#include <LoggerSystem.hpp>

namespace logger
{
	LoggerSystem debug;
	LoggerSystem fatal;
	LoggerSystem error;
	LoggerSystem warning;
}

//template< class T >
//DLLSETTING void loggerprint( T );








#undef DLLSETTING
#endif