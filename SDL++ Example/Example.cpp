#include<SDL.hpp>
//#include<SDL_image.hpp>
//#include<SDL_mixer.hpp>

SDL_HitTestResult hit_test(SDL_Window* win, const SDL_Point* point, void* data)
{
	using namespace SDL;

	Point size;

	SDL_GetWindowSize(win, &size.w, &size.h);

	if (SDL_GetWindowFlags(win) & SDL_WINDOW_RESIZABLE)
	{
		int top, left, bottom, right;
		SDL_GetWindowBordersSize(win, &top, &left, &bottom, &right);

		if (point->y < 5-top)
		{
			if (point->x < 5-left) return (SDL_HitTestResult)HitTestResult::RESIZE_TOPLEFT;
			if (point->x >= size.w+right - 5) return (SDL_HitTestResult)HitTestResult::RESIZE_TOPRIGHT;
			return (SDL_HitTestResult)HitTestResult::RESIZE_TOP;
		}

		if (point->y >= size.h+bottom - 5)
		{
			if (point->x < 5-left) return (SDL_HitTestResult)HitTestResult::RESIZE_BOTTOMLEFT;
			if (point->x >= size.w+right - 5) return (SDL_HitTestResult)HitTestResult::RESIZE_BOTTOMRIGHT;
			return (SDL_HitTestResult)HitTestResult::RESIZE_BOTTOM;
		}

		if (point->x < 5-left) return (SDL_HitTestResult)HitTestResult::RESIZE_LEFT;
		if (point->x >= size.w+right - 5) return (SDL_HitTestResult)HitTestResult::RESIZE_RIGHT;
	}

	return (SDL_HitTestResult)(point->y < 50 ? HitTestResult::DRAGGABLE : HitTestResult::NORMAL);
}

void Program(int argc, char* argv[])
{
	using namespace SDL;

	Point& mouse = Input::mouse;

	Window w;
	Renderer r;
	Point windowSize = { 500, 500 };

	Rect rect(windowSize / 2 - Point(20, 20), { 40, 40 });
	CreateWindowAndRenderer(windowSize, w, r, SDL_WINDOW_BORDERLESS);

	w.SetTitle("Sample window");
	w.SetHitTest(hit_test, NULL);
	w.SetResizable(true);

	const Uint32 wID = w.GetID();

	bool boxVisible = false;
	bool running = true;

	// Listeners are easier than Observers since they use lamdas/function objects rather than inheritence.
	// This is slower at runtime, but a lot faster to write.
	Listener<const Event&> toggle_visibility(
		[&boxVisible](const Event& e)
		{
			if (e.button.button == (Uint8)Button::RIGHT)
				boxVisible = !boxVisible;
		},
		Input::GetTypedEventSubject(Event::Type::MOUSEBUTTONDOWN)
	);

	Listener<const Event&> window_resizer(
		[&windowSize, &wID](const Event& e)
		{
			if (e.window.event != SDL_WINDOWEVENT_RESIZED) return;
			if (e.window.windowID != wID) return;

			windowSize = { e.window.data1, e.window.data2 };
		},
		Input::GetTypedEventSubject(Event::Type::WINDOWEVENT)
	);

	Listener<const Event&> quit_program(
		[&running](const Event& e)
		{
			running = false;
		},
		Input::GetTypedEventSubject(Event::Type::QUIT)
	);

	for (int frame = 0; running; frame++)
	{
		Input::Update();

		if (Input::button(Button::LEFT))
			rect.pos = mouse;

		r.SetDrawColour(VERY_DARK_BLUE).Clear();
		if(boxVisible) r.SetDrawColour(WHITE).FillRect(rect);
		r.SetDrawColour(VERY_LIGHT_AZURE).FillRect({ 0, 0, windowSize.w, 50 });

		r.Present();

		Delay(16);
	}

	w.SetHitTest(NULL, NULL);
}

int main(int argc, char* argv[])
{
	SDL::Init();
	//SDL::IMG::Init(IMG_INIT_PNG);
	//SDL::Mix::Init(MIX_INIT_MP3 | MIX_INIT_OGG);
	SDL::Input::Init();

	Program(argc, argv);

	SDL::Input::Quit();
	//SDL::Mix::Quit();
	//SDL::IMG::Quit();
	SDL::Quit();

	return 0;
}