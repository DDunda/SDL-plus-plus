#include "SDL.hpp"
#include <SDL_image.h>

using namespace SDL;

Point mouse;
Ray ray;


Window w;
Renderer r;

Input input;

int main(int argc, char* argv[]) {

	Init();

	Point windowSize(500, 500);
	CreateWindowAndRenderer(windowSize, w, r);

	w.flag

	w.SetTitle("Sample window");

	input.typed_callback[WINDOWEVENT] += [&windowSize](const Event& e){
		switch (e.window.event)
		{
		case SDL_WINDOWEVENT_RESIZED:
			windowSize = { e.window.data1, e.window.data2 };
			break;
		}
	};

	for (int frame = 0; true; frame++) {

		SDL_PumpEvents();
		if (SDL_GetMouseState(&mouse.x, &mouse.y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			ray.origin = mouse;
		}

		ray.dir = mouse - ray.origin;

		r.SetDrawColor(VERY_DARK_BLUE)
		 .Clear()
		 .SetDrawColor(WHITE);

		if (ray.intersectRect(Rect( 230, 230, 40, 40 ))) {
			r.DrawLineF(ray.origin, ray.hit.point)
			 .SetDrawColor(CYAN)
			 .DrawLineF(ray.hit.point, ray.hit.point + ray.hit.normal * 15.0)
			 .SetDrawColor(GREEN);
		}
		else {
			r.DrawLineF(ray.origin, mouse)
			 .SetDrawColor(RED);
		}

		r.FillRect({ 230, 230, 40, 40 }).Present();

		Delay(16);
	}

	Quit();

	return 0;
}