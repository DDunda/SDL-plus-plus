# SDL++ — An SDL wrapper for C++

Imagine that you are writing some code with SDL and want to draw a white square to the screen:
```cpp
SDL_Rect rect = { 25,25,50,50 };
SDL_SetRenderDrawColor(r, 255,255,255,255);
SDL_RenderFillRect(r, &rect);
```
This is okay, but it feels a little excessive for something so simple, right?
With a bit of C++ your life with SDL can indeed be much simpler!

With SDL++ you can write the same stuff, with less code:
```cpp
r.SetDrawColor(WHITE);
r.FillRect({ 25,25,50,50 });
```
Half the size!

SDL++ is mostly improvements like this; small additions and nice interfaces that stay close to SDL's workflow, but make it easier to write and understand.

Where appropriate, objects are objects, pointers are references, macros are enums or constants, and there are some new optional objects that can manage some systems like time and input.
Generally, object wrappers will also handle your resource's lifetimes too, so you do not need to worry about memory leaks.

## Installing
SDL++ is provided as a standard C++ Visual Studio project, however just the contents of `SDL++\include` and `SDL++\source` are necessary.
If you wish to compile the VS project, please follow my guide for installing SDL2 and SDL2_image: https://github.com/DDunda/InstallSDL2

I install SDL++ the same way in my projects, using `SDLpp` as the environment variable for the path.

## Credits
This project uses SDL because it is an SDL wrapper, obviously. The API, internal functions, and documentation comments are all taken directly from the SDL library, which you can find [here](https://github.com/libsdl-org/SDL).
