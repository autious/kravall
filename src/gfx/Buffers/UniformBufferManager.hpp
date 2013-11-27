#ifndef SRC_GFX_BUFFERS_UNIFORM_BUFFER_MANAGER_HPP
#define SRC_GFX_BUFFERS_UNIFORM_BUFFER_MANAGER_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>

#include "UniformBuffers.hpp"

namespace GFX
{
	class UniformBufferManager
	{

	public:
		UniformBufferManager();
		~UniformBufferManager();

		void CreateExampleBuffer(GLuint programID);
		void SetExampleBufferData(const ExampleBuffer& eb);
		GLuint GetExampleBuffer();
		
	private:
		GLuint m_exampleUBO;
	};
}

#endif