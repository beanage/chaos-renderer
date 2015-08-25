#ifndef __CHAOS_FPS_LIMITER_HPP_INCLUDED__
#define __CHAOS_FPS_LIMITER_HPP_INCLUDED__

namespace chaos
{
	class fps_limiter
	{
	public:
		static const int default_limit = 60;

		fps_limiter(int limit = default_limit);
		
		void reset(int limit = default_limit);
		int fps() const;
		void wait();

		fps_limiter(fps_limiter const&) = delete;
		fps_limiter& operator=(fps_limiter const&) = delete;

	private:
		int _limit;
		int _last_limit_ticks;
		int _last_frame_ticks;
		int _frame_no;
		int _fps;
	};
}

#endif
