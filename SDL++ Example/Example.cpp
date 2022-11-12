#include<SDL.hpp>

using namespace SDL;

SDL_HitTestResult hit_test(SDL_Window* win, const SDL_Point* point, void* data) {
	return (SDL_HitTestResult)(point->y < 50 ? HitTestResult::DRAGGABLE : HitTestResult::NORMAL);
}

int main(int argc, char* argv[]) {
	Init();

	Input input;

	Point& mouse = input.mouse;

	Window w;
	Renderer r;
	Point& windowSize = input.windowSize = { 500, 500 };

	Rect rect(windowSize / 2 - Point(20, 20), { 40, 40 });
	CreateWindowAndRenderer(windowSize, w, r);

	w.SetTitle("Sample window");
	w.SetHitTest(hit_test, NULL);

	bool boxVisible = false;

	// Listeners are easier than Observers since they use lamdas/function objects rather than inheritence.
	// This is slower at runtime, but a lot faster to write.
	Listener<const Event&> toggle_visibility = {[&](const Event& e) {
		if (e.button.button == (Uint8)Button::RIGHT)
			boxVisible = !boxVisible;
	} };
	input.RegisterEventType(Event::Type::MOUSEBUTTONDOWN, toggle_visibility);

	for (int frame = 0; input.running; frame++) {
		input.Update();

		if (input.button(Button::LEFT))
			rect.pos = mouse;

		r.SetDrawColour(VERY_DARK_BLUE).Clear();
		if(boxVisible) r.SetDrawColour(WHITE).FillRect(rect);
		r.SetDrawColour(VERY_LIGHT_AZURE).FillRect({ 0, 0, windowSize.w, 50 });

		r.Present();

		Delay(16);
	}
	Quit();

	w.SetHitTest(NULL, NULL);

	return 0;
}