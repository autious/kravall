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
        Binds the specified uniform block withing the specified shader program to the specified binding point.
        \param programID The program that contains the uniform block to set the value of.
        \param uniformBufferName A pointer to a null terminated string that contains the name of the uniform block.
        \param bindinIndex An unsigned integer that specifies which binding point the uniform block should use. The UniformBufferManager contains defined values for binding points to use for certain uniform buffers. 
        */
        void SetUniformBlockBindingIndex(GLuint programID, const char* uniformBufferName, GLuint bindingIndex);

		/*!
		Set the basic camera uniform block
		\param basicCamera camera to be passed to the program
		*/
		void SetBasicCameraUBO(const BasicCamera& basicCamera);
        
        static const GLuint CAMERA_BINDING_INDEX;

	private:
		void CreateBasicCameraUBO();

		GLuint m_basicCameraUBO;
	};
}

#endif
