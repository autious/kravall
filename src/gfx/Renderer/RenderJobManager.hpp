#ifndef SRC_GFX_RENDER_JOB_MANAGER_HPP
#define SRC_GFX_RENDER_JOB_MANAGER_HPP

#include <vector>
#include <gfx/BitmaskDefinitions.hpp>

namespace GFX
{


	class RenderJobManager
	{
	public:
		RenderJobManager();
		~RenderJobManager();

		void Sort();
		void AddRenderJob(GFXBitmask mask, void* val);
		void Clear();

		struct RenderJob
		{
			GFXBitmask bitmask;
			void* value;
		};

		std::vector<RenderJob>& GetJobs();

	private:
		friend int compare_ints(const void* a, const void* b);

		std::vector<RenderJob> m_renderJobs;
	};
}

#endif