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
		m_shouldRender = false;
	}
	DebugManager::~DebugManager()
	{
	}
	void DebugManager::Clear()
	{
		m_points.clear();
		m_lines.clear();
		m_linesWorld.clear();
		m_filledRects.clear();
		m_filledBoxes.clear();
		m_filledCircles.clear();

		bool m_shouldRender = false;
	}
	void DebugManager::AddPoint(DebugPoint point)
	{
		m_shouldRender = true;
		m_points.push_back(point);
	}
	void DebugManager::AddLine(DebugLine line)
	{
		m_shouldRender = true;
		m_lines.push_back(line);
	}
	void DebugManager::AddLineWorld(DebugLine line)
	{
		m_shouldRender = true;
		m_linesWorld.push_back(line);
	}
	void DebugManager::AddRect(DebugRect rect, bool filled)
	{
		m_shouldRender = true;
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
			l2.end = glm::vec3(rect.position.x + rect.dimensions.x, rect.position.y - rect.dimensions.y, 0.0f);
			l2.thickness = 1.0f;
			m_lines.push_back(l2);

			DebugLine l3;
			l3.color = rect.color;
			l3.start = l2.end;
			l3.end = glm::vec3(rect.position.x, rect.position.y - rect.dimensions.y, 0.0f);
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
		m_shouldRender = true;
		if (filled)
			m_filledBoxes.push_back(box);
		else
		{
			DebugLine line;
			line.color = box.color;
			line.thickness = 1.0f;
			line.useDepth = box.useDepth;

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
	void DebugManager::AddSphere(DebugSphere sphere)
	{
		m_shouldRender = true;
		int nPoints = 16;
		float a = glm::radians(360.0f) / static_cast<float>(nPoints);
		float r = sphere.radius;

		// Create 3 circles in 3d-space
		DebugLine l;
		l.useDepth = sphere.useDepth;
		l.color = sphere.color;
		l.thickness = 1.0f;
		// xy-plane
		for (int i = 0; i < nPoints; ++i)
		{
			float n = static_cast<float>(i);
			l.start = sphere.position + r * glm::vec3(cos(n*a), sin(n*a), 0.0f);
			n += 1.0f;
			l.end = sphere.position + r * glm::vec3(cos(n*a), sin(n*a), 0.0f);
			DebugDrawing().AddLineWorld(l);
		}
		// xz-plane
		for (int i = 0; i < nPoints; ++i)
		{
			float n = static_cast<float>(i);
			l.start = sphere.position + r * glm::vec3(cos(n*a), 0.0f, sin(n*a));
			n += 1.0f;
			l.end = sphere.position + r * glm::vec3(cos(n*a), 0.0f, sin(n*a));
			DebugDrawing().AddLineWorld(l);
		}

		// yz-plane
		for (int i = 0; i < nPoints; ++i)
		{
			float n = static_cast<float>(i);
			l.start = sphere.position + r * glm::vec3(0.0f, cos(n*a), sin(n*a));
			n += 1.0f;
			l.end = sphere.position + r * glm::vec3(0.0f, cos(n*a), sin(n*a));
			DebugDrawing().AddLineWorld(l);
		}
	}
	void DebugManager::AddCircle(DebugRect circle)
	{
		m_shouldRender = true;
		m_filledCircles.push_back(circle);
	}
	void DebugManager::AddFrustum(glm::mat4x4 frustum, glm::vec4 color, bool useDepth)
	{
		DebugLine lines[4];
			for (int e = 0; e < 4; e++)
			{
				lines[e].color = color;
				lines[e].thickness = 1.0f;
				lines[e].useDepth = useDepth;
			}

			glm::mat4x4 invMat = glm::inverse(frustum);
			for (int e = 0; e < 4; e++)
			{
				float x = -1 + 2 * (e % 2);
				float y = -1 + 2 * (e / 2);
				
				glm::vec4 start =	invMat * glm::vec4(x, y,  1.0f, 1.0f);
				glm::vec4 end =		invMat * glm::vec4(x, y, -1.0f, 1.0f);
				
				lines[e].start = glm::vec3(glm::vec3(start) / start.w);
				lines[e].end = glm::vec3(glm::vec3(end) / end.w);

				AddLineWorld(lines[e]);
			}
			for (int e = 0; e < 4; e++)
			{
				float z = -1 + 2 * (e % 2);
				float y = -1 + 2 * (e / 2);
				
				glm::vec4 start =	invMat * glm::vec4(-1, y, z, 1.0f);
				glm::vec4 end =		invMat * glm::vec4( 1, y, z, 1.0f);
				
				lines[e].start = glm::vec3(glm::vec3(start) / start.w);
				lines[e].end = glm::vec3(glm::vec3(end) / end.w);

				AddLineWorld(lines[e]);
			}
			for (int e = 0; e < 4; e++)
			{
				float z = -1 + 2 * (e % 2);
				float x = -1 + 2 * (e / 2);
				
				glm::vec4 start =	invMat * glm::vec4(x, -1, z, 1.0f);
				glm::vec4 end =		invMat * glm::vec4(x,  1, z, 1.0f);
				
				lines[e].start = glm::vec3(glm::vec3(start) / start.w);
				lines[e].end = glm::vec3(glm::vec3(end) / end.w);

				AddLineWorld(lines[e]);
			}

			// Draw crosshair to better see direction of the frustum
			for (int e = 0; e < 2; e++)
			{
				float x = -1.0f + 2.0f * e;
				
				glm::vec4 start =	invMat * glm::vec4(-0.3f * x, 1.0f, 1.0f, 1.0f);
				glm::vec4 end =		invMat * glm::vec4( 0.0f	, 1.4f, 1.0f, 1.0f);
				
				lines[e].start = glm::vec3(glm::vec3(start) / start.w);
				lines[e].end = glm::vec3(glm::vec3(end) / end.w);

				AddLineWorld(lines[e]);
			}
	}
}