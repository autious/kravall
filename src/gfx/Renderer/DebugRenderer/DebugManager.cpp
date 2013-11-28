#include "DebugManager.hpp"

namespace GFX
{
	DebugManager& DebugDrawing()
	{
		static DebugManager debugManager;
		return debugManager;
	}

	DebugManager::DebugManager()
	{
	}
	DebugManager::~DebugManager()
	{
	}

	void DebugManager::Init()
	{
	}

	void DebugManager::Render()
	{
	}

	void DebugManager::AddPoint(DebugPoint point)
	{
		m_points.push_back(point);
	}
	void DebugManager::AddLine(DebugLine line)
	{
		m_lines.push_back(line);
	}
	void DebugManager::AddLineWorld(DebugLine line)
	{
		m_linesWorld.push_back(line);
	}
	void DebugManager::AddRect(DebugRect rect, bool filled)
	{
		if (filled)
			m_filledRects.push_back(rect);
		else
		{
			// Add lines instead
			DebugLine l1;
			l1.color = rect.color;
			l1.start = glm::vec3(rect.position.x, rect.position.y, 0.0f);
			l1.end = glm::vec3(rect.position.x+rect.dimensions.x, rect.position.y, 0.0f);
			l1.thickness = 1.0f;
			m_lines.push_back(l1);

			DebugLine l2;
			l2.color = rect.color;
			l2.start = l1.end;
			l2.end = glm::vec3(rect.position.x + rect.dimensions.x, rect.position.y + rect.dimensions.y, 0.0f);
			l2.thickness = 1.0f;
			m_lines.push_back(l2);

			DebugLine l3;
			l3.color = rect.color;
			l3.start = l2.end;
			l3.end = glm::vec3(rect.position.x, rect.position.y + rect.dimensions.y, 0.0f);
			l3.thickness = 1.0f;
			m_lines.push_back(l3);

			DebugLine l4;
			l4.color = rect.color;
			l4.start = l3.end;
			l4.end = glm::vec3(rect.position.x, rect.position.y, 0.0f);
			l4.thickness = 1.0f;
			m_lines.push_back(l4);


		}
	}
	void DebugManager::AddBox(DebugBox box, bool filled)
	{
		if (filled)
			m_filledBoxes.push_back(box);
		else
		{
			DebugLine line;
			line.color = box.color;
			line.thickness = 1.0f;

			glm::vec3 p = box.position;
			glm::vec3 d = box.dimensions / 2.0f;

			glm::vec3 p1 = glm::vec3(p.x + d.x, p.y + d.y, p.z - d.z);
			glm::vec3 p2 = glm::vec3(p.x + d.x, p.y + d.y, p.z + d.z);
			glm::vec3 p3 = glm::vec3(p.x - d.x, p.y + d.y, p.z + d.z);
			glm::vec3 p4 = glm::vec3(p.x - d.x, p.y + d.y, p.z - d.z);
			glm::vec3 p5 = glm::vec3(p.x + d.x, p.y - d.y, p.z - d.z);
			glm::vec3 p6 = glm::vec3(p.x + d.x, p.y - d.y, p.z + d.z);
			glm::vec3 p7 = glm::vec3(p.x - d.x, p.y - d.y, p.z + d.z);
			glm::vec3 p8 = glm::vec3(p.x - d.x, p.y - d.y, p.z - d.z);


			//1
			line.start = p1;
			line.end = p2;
			DebugDrawing().AddLineWorld(line);
			//2
			line.start = p3;
			DebugDrawing().AddLineWorld(line);
			//3
			line.end = p4;
			DebugDrawing().AddLineWorld(line);
			//4
			line.start = p8;
			DebugDrawing().AddLineWorld(line);
			//5
			line.end = p7;
			DebugDrawing().AddLineWorld(line);
			//6
			line.start = p6;
			DebugDrawing().AddLineWorld(line);
			//7
			line.end = p5;
			DebugDrawing().AddLineWorld(line);
			//8
			line.start = p1;
			DebugDrawing().AddLineWorld(line);
			//9
			line.end = p4;
			DebugDrawing().AddLineWorld(line);

			// last edges
			line.start = p5;
			line.end = p8;
			DebugDrawing().AddLineWorld(line);

			line.start = p6;
			line.end = p2;
			DebugDrawing().AddLineWorld(line);

			line.start = p7;
			line.end = p3;
			DebugDrawing().AddLineWorld(line);
		}
	}
	void DebugManager::AddSphere(DebugSphere sphere, bool filled)
	{
		if (filled)
			m_filledSpheres.push_back(sphere);
		else
		{

		}
	}
	void DebugManager::AddCircle(DebugCircle circle, bool filled)
	{
		if (filled)
			m_filledCircles.push_back(circle);
		else
		{

		}
	}
}