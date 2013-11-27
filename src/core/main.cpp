#ifdef RUN_GTEST 
    #include <gtest/gtest.h>
#endif

#include <iostream>

#include "WindowHandling/InitializeGLFW.hpp"

#include <gfx/GFXInterface.hpp>


GLFWwindow* init()
{
	GLFWwindow* window;
	
	Core::InitializeGLFW(&window, 1280, 720);

	if (GFX::Init(1280,720) == GFX_FAIL)
		return nullptr;

    return window;
}

void run( GLFWwindow * window )
{
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			break;

		GFX::Render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    glfwDestroyWindow( window );

	glfwTerminate();
}

int main(int argc, char** argv)
{
#ifdef RUN_GTEST
    ::testing::InitGoogleTest(&argc, argv);
#endif
    GLFWwindow* window = init();
    if( window == nullptr )
       return -1; 
#ifdef RUN_GTEST
    int gtestReturn = RUN_ALL_TESTS();
    if( gtestReturn != 0 )
        return gtestReturn;
#endif
#ifndef SKIP_RUN
    run( window );
#endif
#ifdef RUN_GTEST
    return gtestReturn;
#else
	return 0;
#endif
}
