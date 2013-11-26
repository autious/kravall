#ifndef SRC_CORE_WINDOWHANDLING_GLFW_INCLUDE_HPP
#define SRC_CORE_WINDOWHANDLING_GLFW_INCLUDE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
#	define GLFW_EXPOSE_NATIVE_WIN32
#	define GLFW_EXPOSE_NATIVE_WGL
#else
#	define GLFW_EXPOSE_NATIVE_X11
#	define GLFW_EXPOSE_NATIVE_GLX
#endif
#include <GLFW/glfw3native.h>

#endif
