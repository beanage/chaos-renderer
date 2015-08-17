
#ifndef __CHAOS_RENDERER_H_INCLUDED__
#define __CHAOS_RENDERER_H_INCLUDED__

#include <vector>
#include <memory>
#include <glm/mat4x4.hpp>

namespace chaos
{
	class renderer
	{
		friend struct scoped_renderer;
	public:
		class mesh2d;

		renderer(int width, int height);
		~renderer();
		
		void resize_window(int width, int height);

		void render(mesh2d const& mesh_, glm::mat4 model = glm::mat4());

		void flip();

	private:
		void begin();
		void end();

		struct impl;
		std::unique_ptr<impl> _impl;
	};

	class renderer::mesh2d
	{
		friend class renderer;
	public:
		mesh2d(std::initializer_list<float> const& vertices);
		~mesh2d();

	private:
		void bind() const;
		
		size_t size() const;

		struct impl;
		std::unique_ptr<impl> _impl;
	};

	struct scoped_renderer
	{
		scoped_renderer(renderer& target);
		~scoped_renderer();

		renderer& target;
	};
}

#endif
