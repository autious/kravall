#ifndef LogSystemHPP
#define LogSystemHPP

#ifdef LOGGER_DLL_EXPORT
#define DLLSETTING __declspec(dllexport)
#else 
#define DLLSETTING __declspec(dllimport)
#endif

#include "internal/LogData.hpp"

#undef DLLSETTING
#endif