#include<SDL.hpp>

using namespace SDL;

int main(int argc, char* argv[])
{
	Init();
	Input::Init();

	Window w;
	Renderer r;
	Point window_size = { 500, 500 };

	bool running = true;

	CreateWindowAndRenderer(window_size, w, r);

	w.SetTitle("New window");

	const Uint32 wID = w.GetID();

	Listener<const Event&> window_resizer(
		[&window_size, &wID](const Event& e)
		{
			if (e.window.event != SDL_WINDOWEVENT_RESIZED) return;
			if (e.window.windowID != wID) return;

			window_size = { e.window.data1, e.window.data2 };
		}
	);

	Input::RegisterEventType(Event::Type::WINDOWEVENT, window_resizer);

	Listener<const Event&> quit_program(
		[&running](const Event& e)
		{
			running = false;
		}
	);

	Input::RegisterEventType(Event::Type::QUIT, quit_program);

	for (int frame = 0; running; frame++)
	{
		Input::Update();

		r.SetDrawColour(VERY_DARK_BLUE);
		r.Clear();
		r.Present();

		Delay(16);
	}

	Quit();

	return 0;
}