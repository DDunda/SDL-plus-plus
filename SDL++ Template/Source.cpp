#include<SDL.hpp>

using namespace SDL;

int main(int argc, char* argv[]) {
	Init();

	Input input;

	Window w;
	Renderer r;
	Point& windowSize = input.windowSize = { 500, 500 };

	CreateWindowAndRenderer(windowSize, w, r);

	w.SetTitle("New window");

	for (int frame = 0; input.running; frame++) {
		input.Update();

		r.SetDrawColour(VERY_DARK_BLUE).Clear().Present();

		Delay(16);
	}

	Quit();

	return 0;
}