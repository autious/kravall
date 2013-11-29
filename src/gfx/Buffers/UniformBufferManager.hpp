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

		/*!
		Create the basic uniform block for the camera
		\param programID id of the program to link to
		*/
		void CreateBasicCameraUBO(GLuint programID);

		/*!
		Set the basic camera uniform block
		\param basicCamera camera to be passed to the program
		*/
		void SetBasicCameraUBO(const BasicCamera& basicCamera);

		/*!
		Get the camera ubo id
		\return basic camera ubo id
		*/
		GLuint GetBasicCameraUBO();

		void CreateExampleBuffer(GLuint programID);
		void SetExampleBufferData(const ExampleBuffer& eb);
		GLuint GetExampleBuffer();
		
	private:
		GLuint m_exampleUBO;
		GLuint m_basicCameraUBO;
	};
}

#endif