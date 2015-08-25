#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "chaos-renderer.hpp"
#include "chaos-texture.hpp"
#include "chaos-fps-limiter.hpp"

using namespace std;
using namespace chaos;

int main(int argc, char const *argv[])
{
	try {
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
			cout << "Could not init video.";
		
		renderer game_renderer(640, 480);
		renderer::mesh2d test_mesh({
			{{0.0,   0.0}, {0.0, 0.0}},
			{{100.0, 0.0}, {1.0, 0.0}},
			{{0.0, 100.0}, {0.0, 1.0}},
			
			{{100.0, 0.0},   {1.0, 0.0}},
			{{100.0, 100.0}, {1.0, 1.0}},
			{{0.0, 100.0},   {0.0, 1.0}},
		});

		texture2d tex("test.bmp");

		fps_limiter fps(60);

		SDL_StartTextInput();
		bool quit = false;
		while(!quit)
		{
			SDL_Event e;
			scoped_renderer auto_flip(game_renderer);
			
			int mx, my;
			SDL_GetMouseState(&mx, &my);
			
			tex.bind();
			game_renderer.render(test_mesh, glm::translate(glm::mat4(), glm::vec3(mx, my, 0.0f)));
			
			while(SDL_PollEvent(&e) != 0)
				if(e.type == SDL_QUIT)
					quit = true;

			cout << fps.fps() << endl;

			fps.wait();
		}
		SDL_StopTextInput();
	} catch (runtime_error const& e) {
		cout << e.what() << endl;
	} catch (...) {
		cout << "Fatal error." << endl;
	}

	return 0;
}
