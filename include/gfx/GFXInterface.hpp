#ifdef _WIN32
#ifdef DLL_EXPORT
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
#endif

namespace GFX
{
	static DLL_API void Render();
}