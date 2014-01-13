#ifndef SRC_CORE_WINDOWHANDLING_GLFW_INCLUDE_HPP
#define SRC_CORE_WINDOWHANDLING_GLFW_INCLUDE_HPP

#include <glm/glm.hpp> //For some reason glm refuses to compile if included after this header
                       //Therefore i put a copy here.
#include <glm/ext.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
#	define GLFW_EXPOSE_NATIVE_WIN32
#	define GLFW_EXPOSE_NATIVE_WGL
#else
#	define GLFW_EXPOSE_NATIVE_X11
#	define GLFW_EXPOSE_NATIVE_GLX
#endif

#ifdef _MSC_VER
// This should temporarily disable warning caused by windows.h
#pragma warning( disable : 4005 )
#include <GLFW/glfw3native.h>
#pragma warning( default : 4005 )
#else
#include <GLFW/glfw3native.h>
#endif


extern GLFWwindow* mainWindow;

#endif
