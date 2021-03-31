#include "SDL++/SDL.hpp"
#include "SDL++/ray.hpp"
#include <SDL_image.h>

using namespace SDL;

int main(int argc, char* argv[]) {
	Init();

	Window w;
	Renderer r;

	Input input;

	Point& mouse = input.mouse;
	Ray ray;
	Point& windowSize = input.windowSize; 
	windowSize = { 500, 500 };
	CreateWindowAndRenderer(windowSize, w, r);

	w.SetResizable(true)
	 .SetTitle("Sample window");

	for (int frame = 0; input.running; frame++) {
		input.Update();

		if (input.buttonDown(SDL_BUTTON_LEFT))
			ray.origin = mouse;
		ray.dir = mouse - ray.origin;

		r.SetDrawColor(VERY_DARK_BLUE)
		 .Clear()
		 .SetDrawColor(WHITE);

		Rect rect(windowSize / 2 - Point(20, 20), { 40, 40 });

		if (ray.intersectRect(rect)) {
			r.DrawLineF(ray.origin, ray.hit.point)
			 .SetDrawColor(CYAN)
			 .DrawLineF(ray.hit.point, ray.hit.point + ray.hit.normal * 15.0)
			 .SetDrawColor(GREEN);
		}
		else {
			r.DrawLineF(ray.origin, mouse)
			 .SetDrawColor(RED);
		}

		r.FillRect(rect).Present();

		Delay(16);
	}

	Quit();

	return 0;
}