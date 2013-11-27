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
	}
	void DebugManager::AddRect(DebugRect rect, bool filled)
	{
	}
	void DebugManager::AddBox(DebugBox box, bool filled)
	{
	}
	void DebugManager::AddSphere(DebugSphere sphere, bool filled)
	{
	}
	void DebugManager::AddCircle(DebugCircle circle, bool filled)
	{
	}
}