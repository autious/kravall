#ifndef SRC_GFX_RENDERER_DEBUG_MANAGER_HPP
#define SRC_GFX_RENDERER_DEBUG_MANAGER_HPP

#include <GL/glew.h>
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
		glm::vec3 position;
		glm::vec3 dimensions;
		glm::vec4 color;
		float lineWidth;
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

	class DebugManager
	{
	public:
		friend DebugManager& DebugDrawing();
		
		bool ShouldRender() const { return m_shouldRender; }

		void Clear();

		void AddPoint(DebugPoint point);
		void AddLine(DebugLine line);
		void AddLineWorld(DebugLine line);
		void AddRect(DebugRect rect, bool filled);
		void AddBox(DebugBox box, bool filled);
		void AddSphere(DebugSphere sphere);
		void AddCircle(DebugRect circle);

		inline std::vector<DebugPoint>& GetPoints(){ return m_points; }
		inline std::vector<DebugLine>& GetLines(){ return m_lines; }
		inline std::vector<DebugLine>& GetLinesWorld(){ return m_linesWorld; }
		inline std::vector<DebugRect>& GetFilledRects(){ return m_filledRects; }
		inline std::vector<DebugBox>& GetFilledBoxes(){ return m_filledBoxes; }
		inline std::vector<DebugRect>& GetFilledCircles(){ return m_filledCircles; }

		inline void SetConsoleVisible(bool visible){ m_showConsole = visible; }
		inline bool GetConsoleVisible(){ return m_showConsole; }
		inline DebugRect GetConsoleRect(){ return m_consoleRect; }

		void SetConsoleHeight(int height);

	private:
		DebugManager();
		~DebugManager();

	private:
		// Points
		std::vector<DebugPoint> m_points;

		// Lines
		std::vector<DebugLine> m_lines;
		std::vector<DebugLine> m_linesWorld;

		// Tris
		std::vector<DebugRect> m_filledRects;
		std::vector<DebugBox> m_filledBoxes;
		std::vector<DebugRect> m_filledCircles;

		bool m_shouldRender;

		bool m_showConsole;

		DebugRect m_consoleRect;
		int m_consoleHeight;
		glm::vec4 m_consoleColor;

	};

	/*!
	Function to get singleton
	\return DebugManager Returns the RenderCore instance
	*/
	DebugManager& DebugDrawing();
}
#endif
