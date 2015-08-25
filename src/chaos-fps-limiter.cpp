#include "chaos-fps-limiter.hpp"

#include <SDL2/SDL.h>
#include <iostream>

using namespace chaos;

fps_limiter::fps_limiter(int limit)
{
	reset(limit);
}

void fps_limiter::reset(int limit)
{
	_limit = limit;
	_frame_no = 0;
	_fps = 0;
	_last_limit_ticks = SDL_GetTicks();
	_last_frame_ticks = SDL_GetTicks();
}

int fps_limiter::fps() const
{
	return _fps;
}

void fps_limiter::wait()
{
	++_frame_no;

	int target = _last_limit_ticks + 1000.0f / _limit;
	int ticks = SDL_GetTicks();

	if (ticks - _last_frame_ticks >= 1000) {
		_fps = _frame_no;
		_frame_no = 0;
		_last_frame_ticks = SDL_GetTicks();
	}

	if (!SDL_TICKS_PASSED(ticks, target)) {
		SDL_Delay(target - ticks);
	}

	_last_limit_ticks = SDL_GetTicks();
}
