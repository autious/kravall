#ifdef GFX_DLL_EXPORT
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif


#define GFX_SUCCESS 0
#define GFX_FAIL 1

namespace GFX
{
	/*!
	Initializes the graphics engine on the currently bound context.
	\return Returns GFX_SUCCESS if successful, else returns GFX_FAIL
	*/
	DLL_API int Init();

	/*!
	Executes all draw calls made to the graphics engine and
	renders it to the screen.
	*/
	DLL_API void Render();
}

namespace GFXDebug
{

}

namespace GFXData
{

}