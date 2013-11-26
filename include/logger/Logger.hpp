#ifndef LogSystemHPP
#define LogSystemHPP



#ifdef LogSystem_DLL_EXPORT
#define DLLSETTING __declspec(dllexport)
#else 
#define DLLSETTING __declspec(dllimport)
#endif






//template< class T >
//DLLSETTING void LogSystemprint( T );








#undef DLLSETTING
#endif