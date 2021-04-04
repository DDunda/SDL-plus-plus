#include "SDL++/SDL.hpp"

int main(int argc, char* argv[]) {
	using namespace SDL;
	Init();

	Input input;

	Point& mouse = input.mouse;

	Window w;
	Renderer r;
	Point& windowSize = input.windowSize = { 500, 500 };

	Rect rect(windowSize / 2 - Point(20, 20), { 40, 40 });
	CreateWindowAndRenderer(windowSize, w, r);

	w.SetTitle("Sample window");

	for (int frame = 0; input.running; frame++) {
		input.Update();

		if (input.button(SDL_BUTTON_LEFT))
			rect.pos = mouse;

		r.SetDrawColor(VERY_DARK_BLUE).Clear();
		r.SetDrawColor(WHITE).FillRect(rect);


		r.Present();

		Delay(16);
	}
	Quit();

	return 0;
}