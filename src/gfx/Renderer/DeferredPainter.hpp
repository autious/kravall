#ifndef SRC_GFX_RENDERER_DEFERRED_PAINTER_HPP
#define SRC_GFX_RENDERER_DEFERRED_PAINTER_HPP

namespace GFX
{
	class DeferredPainter
	{
	public:
		DeferredPainter();
		~DeferredPainter();

		void Initialize();
		void Render(GLuint FBO);

	private:
	};
}

#endif