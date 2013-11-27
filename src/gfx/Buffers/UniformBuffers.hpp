#ifndef SRC_GFX_BUFFERS_UNIFORM_BUFFERS_HPP
#define SRC_GFX_BUFFERS_UNIFORM_BUFFERS_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>

namespace GFX
{
	struct ExampleBuffer
	{
		glm::vec4 colorOne;
		glm::vec4 colorTwo;
		GLfloat floatOne;
		GLfloat floatTwo;
	};

	struct BasicCamera
	{
		glm::mat4 viewMatrix;
		glm::mat4 projMatrix;
	};
}

#endif