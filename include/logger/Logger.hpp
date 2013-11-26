#ifndef LOGGERHPP
#define LOGGERHPP

#include <iostream>



#ifdef LOGGER_DLL_EXPORT
#define DLLSETTING __declspec(dllexport)
#else 
#define DLLSETTING __declspec(dllimport)
#endif




DLLSETTING void loggerprint(char text[]);








#undef DLLSETTING
#endif