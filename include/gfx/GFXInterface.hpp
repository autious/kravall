#ifdef GFX_DLL_EXPORT
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#include <GLFW/glfw3.h>

#define GFX_SUCCESS 0
#define GFX_FAIL 1

namespace GFX
{
<<<<<<< HEAD
	DLL_API void Render();
=======
	/*!
	Binds a context to a window and initializes the graphics engine.
	\param window "window" is the window you wish to bind the context to
	\return Returns GFX_SUCCESS if successful, else returns GFX_FAIL
	*/
	DLL_API int Init(GLFWwindow* window);

	/*!
	Executes all draw calls made to the graphics engine and
	renders it to the screen.
	*/
	DLL_API void Render(GLFWwindow* window);
}

namespace GFXDebug
{

}

namespace GFXData
{

>>>>>>> acc4f58deff6fe161fd9631d9e2b3cf243d8517a
}