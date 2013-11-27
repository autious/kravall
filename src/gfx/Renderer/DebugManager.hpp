#ifndef SRC_GFX_RENDERER_DEBUG_MANAGER_HPP
#define SRC_GFX_RENDERER_DEBUG_MANAGER_HPP
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <vector>

namespace GFX
{
	struct DebugPoint
	{
		glm::vec3 position;
		float size;
		glm::vec4 color;
	};

	struct DebugLine
	{
		glm::vec3 start;
		glm::vec3 end;
		float thickness;
		glm::vec4 color;
	};

	struct DebugRect
	{
		glm::vec2 position;
		glm::vec2 dimensions;
		glm::vec4 color;
	};

	struct DebugBox
	{
		glm::vec3 position;
		glm::vec3 dimensions;
		glm::vec4 color;
	};

	struct DebugSphere
	{
		glm::vec3 position;
		float radius;
		glm::vec4 color;
	};

	struct DebugCircle
	{
		glm::vec2 position;
		float radius;
		glm::vec4 color;
	};

	class DebugManager
	{
	public:
		DebugManager();
		~DebugManager();

		void Init();

		void Render();

		void AddPoint(DebugPoint point);
		void AddLine(DebugLine line);
		void AddRect(DebugRect rect, bool filled);
		void AddBox(DebugBox box, bool filled);
		void AddSphere(DebugSphere sphere, bool filled);
		void AddCircle(DebugCircle circle, bool filled);

	private:
		// Points
		std::vector<DebugPoint> m_points;

		// Lines
		std::vector<DebugLine> m_lines;
		std::vector<DebugRect> m_rects;
		std::vector<DebugBox> m_boxes;
		std::vector<DebugSphere> m_spheres;
		std::vector<DebugCircle> m_circles;

		// Tris
		std::vector<DebugRect> m_filledRects;
		std::vector<DebugBox> m_filledBoxes;
		std::vector<DebugSphere> m_filledSpheres;
		std::vector<DebugCircle> m_filledCircles;

		// VAO
		GLuint m_pointVAO;
	};

	/*!
	Function to get singleton
	\return DebugManager Returns the RenderCore instance
	*/
	DebugManager& DebugDrawing();
}
#endif