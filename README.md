# SDL++ - A C++ wrapper for SDL
While it is true SDL2 is natively compatible with C++, it is somewhat unwieldy to use. Drawing a white rectangle, for example, needs some code such as `SDL_Rect rect{25,25,50,50};SDL_SetRenderDrawColor(r,255,255,255,255);SDL_RenderFillRect(r,&rect);`.
With a bit of C++, though, SDL can be much shorter! This same example in SDL++ would look like `r.SetDrawColor(WHITE).FillRect({25,25,50,50});`. Half the size!

SDL++ is mostly small improvements like this; adding together positively, while staying close to SDL's workflow.
Where appropriate, objects are objects, pointers are references, macros are enums, and systems such as time or input have simple ways to manage them.

## Installing
SDL++ is provided as a standard C++ Visual Studio project, however just the contents of `SDL++\include` and `SDL++\source` are necessary.
If you wish to compile the VS project, please follow my guide for installing SDL2 and SDL2_image: https://github.com/DDunda/InstallSDL2

When I install SDL++ this way in the future, I will use the `SDL++` environment variable.

## Credits
Since this an SDL wrapper, this project uses SDL. The api, interal functions, and documentation comments, are all taken directly from the SDL library (which you can find [here](https://www.libsdl.org/download-2.0.php), by the way).
