#include "RenderJobManager.hpp"
#include <algorithm>

namespace GFX
{

	int compare_ints(const void* a, const void* b)   // comparison function
	{
		GFX::RenderJobManager::RenderJob arg1 = *reinterpret_cast<const GFX::RenderJobManager::RenderJob*>(a);
		GFX::RenderJobManager::RenderJob arg2 = *reinterpret_cast<const GFX::RenderJobManager::RenderJob*>(b);
		if(arg1.bitmask < arg2.bitmask) return -1;
		if(arg1.bitmask > arg2.bitmask) return 1;
		return 0;
	}

	RenderJobManager::RenderJobManager()
	{
		m_renderJobs.reserve(10000);
	}

	RenderJobManager::~RenderJobManager()
	{
	}

	void RenderJobManager::Sort()
	{
		std::qsort(m_renderJobs.data(), m_renderJobs.size(), sizeof(RenderJob), compare_ints);
	}

	void RenderJobManager::AddRenderJob(GFXBitmask mask, void* val)
	{
		RenderJob rj;
		rj.bitmask = mask;
		rj.value = val;
		m_renderJobs.push_back(rj);
	}

	void RenderJobManager::Clear()
	{
		m_renderJobs.clear();
	}

	std::vector<RenderJobManager::RenderJob>& RenderJobManager::GetJobs()
	{
		return m_renderJobs;
	}

}
