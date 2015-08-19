#ifndef __CHAOS_TEXTURE_HPP_INLCUDED__
#define __CHAOS_TEXTURE_HPP_INLCUDED__

#include <memory>
#include <string>

namespace chaos
{
	class texture2d
	{
	public:
		texture2d(std::string const& filename);
		~texture2d();

		unsigned int width() const;
		unsigned int height() const;

		void bind() const;

	private:
		texture2d(texture2d const&) = delete;
		texture2d& operator=(texture2d const&) = delete;

		struct impl;
		std::unique_ptr<impl> _impl;
	};
}

#endif
