#ifndef SRC_GFX_RENDER_JOB_MANAGER_HPP
#define SRC_GFX_RENDER_JOB_MANAGER_HPP

#include <vector>
#include <gfx/BitmaskDefinitions.hpp>
#include <glm/glm.hpp>
namespace GFX
{
	struct FilledRect
	{
		glm::vec3 position;
		glm::vec3 dimensions;
		glm::vec4 color;
		float lineWidth;
	};

	class RenderJobManager
	{
	public:
		RenderJobManager();
		~RenderJobManager();

		void Sort();
		void AddRenderJob(GFXBitmask mask, void* val);
		void Clear();

		void AddFilledRect(FilledRect r);

		struct RenderJob
		{
			GFXBitmask bitmask;
			void* value;
		};

		std::vector<RenderJob>& GetJobs();
		std::vector<FilledRect>& GetFilledRects();

	private:
		friend int compare_ints(const void* a, const void* b);

		std::vector<RenderJob> m_renderJobs;
		std::vector<FilledRect> m_filledRects;

	};
}

#endif