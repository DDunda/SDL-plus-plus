#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef SDL_mouse_hpp_
#define SDL_mouse_hpp_
#pragma once

#include <SDL_mouse.h>

#include "video.hpp"

#include <memory>

namespace SDL
{
	// Cursor types creating system cursors
	enum class SystemCursor
	{
		ARROW = SDL_SYSTEM_CURSOR_ARROW,	     // Arrow
		IBEAM = SDL_SYSTEM_CURSOR_IBEAM,	     // I-beam
		WAIT = SDL_SYSTEM_CURSOR_WAIT,	         // Wait
		CROSSHAIR = SDL_SYSTEM_CURSOR_CROSSHAIR, // Crosshair
		WAITARROW = SDL_SYSTEM_CURSOR_WAITARROW, // Small wait cursor (or Wait if not available)
		SIZENWSE = SDL_SYSTEM_CURSOR_SIZENWSE,   // Double arrow pointing northwest and southeast
		SIZENESW = SDL_SYSTEM_CURSOR_SIZENESW,   // Double arrow pointing northeast and southwest
		SIZEWE = SDL_SYSTEM_CURSOR_SIZEWE,	     // Double arrow pointing west and east
		SIZENS = SDL_SYSTEM_CURSOR_SIZENS,	     // Double arrow pointing north and south
		SIZEALL = SDL_SYSTEM_CURSOR_SIZEALL,     // Four pointed arrow pointing north, south, east, and west
		NO = SDL_SYSTEM_CURSOR_NO,		         // Slashed circle or crossbones
		HAND = SDL_SYSTEM_CURSOR_HAND,	         // Hand
		NUM_CURSORS = SDL_NUM_SYSTEM_CURSORS
	};

	// Scroll direction types for the Scroll event
	enum class MouseWheelDirection
	{
		NORMAL = SDL_MOUSEWHEEL_NORMAL,  // The scroll direction is normal
		FLIPPED = SDL_MOUSEWHEEL_FLIPPED // The scroll direction is flipped / natural
	};

	struct Cursor
	{
		// This is custom destructor for smart pointers that destroys SDL_Cursor through SDL
		static void DestroyCursor(SDL_Cursor* cursor);

		// This is custom destructor for smart pointers that does not destroy the SDL_Cursor. This is for pointers you do not own
		static void DontDestroyCursor(SDL_Cursor* cursor);

		// This creates a smart pointer to an SDL_Cursor with a custom destructor
		static std::shared_ptr<SDL_Cursor> MakeSharedPtr(SDL_Cursor* cursor);

		// This creates a Cursor from a SDL_Cursor pointer, taking ownership of the pointer
		static Cursor FromPtr(SDL_Cursor* cursor);

		// This creates a Cursor from a SDL_Cursor pointer, but does not take ownership of the pointer
		static Cursor FromUnownedPtr(SDL_Cursor* cursor);

		std::shared_ptr<SDL_Cursor> cursor = nullptr;

		Cursor(std::shared_ptr<SDL_Cursor> cursor);

		/**
		 *  Create a cursor, using the specified bitmap data and
		 *		 mask (in MSB format).
		 *
		 *  The cursor width must be a multiple of 8 bits.
		 *
		 *  The cursor is created in black and white according to the following:
		 *  <table>
		 *  <tr><td> data </td><td> mask </td><td> resulting pixel on screen </td></tr>
		 *  <tr><td>  0   </td><td>  1   </td><td> White </td></tr>
		 *  <tr><td>  1   </td><td>  1   </td><td> Black </td></tr>
		 *  <tr><td>  0   </td><td>  0   </td><td> Transparent </td></tr>
		 *  <tr><td>  1   </td><td>  0   </td><td> Inverted colour if possible, black
		 *										 if not. </td></tr>
		 *  </table>
		 */
		Cursor(const Uint8* data, const Uint8* mask, int w, int h, int hot_x, int hot_y);
		// Create a colour cursor.
		Cursor(Surface& surface, int hot_x, int hot_y);
		// Create a system cursor.
		Cursor(SystemCursor id);

		inline bool operator==(const Cursor& that) { return cursor == that.cursor; }
		inline bool operator!=(const Cursor& that) { return cursor != that.cursor; }

		inline bool operator==(const SDL_Cursor* that) { return cursor.get() == that; }
		inline bool operator!=(const SDL_Cursor* that) { return cursor.get() != that; }

		// Return the active cursor.
		static Cursor GetCursor();

		// Return the default cursor.
		static Cursor GetDefaultCursor();

		// Set the active cursor.
		void SetActive();

		// Show the cursor.
		void Show();
		// Hide the cursor.
		void Hide();
		/**
		 *  Query the current cursor state.
		 *
		 *  \return true if the cursor is shown, or false if the cursor is hidden.
		 */
		bool Shown();
	};

	// Get the window which currently has mouse focus.
	Window GetMouseFocus();

	/**
	 *  \brief Retrieve the current state of the mouse.
	 *
	 *  The current button state is returned as a button bitmask, which can
	 *  be tested using ButtonMask, and x and y are set to the mouse cursor
	 *  position relative to the focus window for the currently selected
	 *  mouse.
	 */
	Uint32 GetMouseState(int& x, int& y);

	/**
	 *  \brief Retrieve the current state of the mouse.
	 *
	 *  The current button state is returned as a button bitmask, which can
	 *  be tested using ButtonMask.
	 */
	Uint32 GetMouseState();

#if SDL_VERSION_ATLEAST(2, 0, 4)
	/**
	 *  \brief Get the current state of the mouse, in relation to the desktop
	 *
	 *  This works just like GetMouseState(), but the coordinates will be
	 *  reported relative to the top-left of the desktop. This can be useful if
	 *  you need to track the mouse outside of a specific window and
	 *  CaptureMouse() doesn't fit your needs. For example, it could be
	 *  useful if you need to track the mouse while dragging a window, where
	 *  coordinates relative to a window might not be in sync at all times.
	 *
	 *  \note GetMouseState() returns the mouse position as SDL understands
	 *		it from the last pump of the event queue. This function, however,
	 *		queries the OS for the current mouse position, and as such, might
	 *		be a slightly less efficient function. Unless you know what you're
	 *		doing and have a good reason to use this function, you probably want
	 *		GetMouseState() instead.
	 *
	 *  \param x Returns the current X coord, relative to the desktop.
	 *  \param y Returns the current Y coord, relative to the desktop.
	 *  \return The current button state as a bitmask, which can be tested using ButtonMask.
	 */
	inline Uint32 GetGlobalMouseState(int& x, int& y) { return SDL_GetGlobalMouseState(&x, &y); }

	/**
	 *  \brief Get the current state of the mouse, in relation to the desktop
	 *
	 *  This works just like GetMouseState(), but the coordinates will be
	 *  reported relative to the top-left of the desktop. This can be useful if
	 *  you need to track the mouse outside of a specific window and
	 *  CaptureMouse() doesn't fit your needs. For example, it could be
	 *  useful if you need to track the mouse while dragging a window, where
	 *  coordinates relative to a window might not be in sync at all times.
	 *
	 *  \note GetMouseState() returns the mouse position as SDL understands
	 *		it from the last pump of the event queue. This function, however,
	 *		queries the OS for the current mouse position, and as such, might
	 *		be a slightly less efficient function. Unless you know what you're
	 *		doing and have a good reason to use this function, you probably want
	 *		GetMouseState() instead.
	 *
	 *  \param pos Returns the current coordinates, relative to the desktop.
	 *  \return The current button state as a bitmask, which can be tested using ButtonMask.
	 */
	inline Uint32 GetGlobalMouseState(Point& pos) { return GetGlobalMouseState(pos.x, pos.y); }

	/**
	 *  \brief Get the current state of the mouse, in relation to the desktop
	 *
	 *  This works just like GetMouseState(), but the coordinates will be
	 *  reported relative to the top-left of the desktop. This can be useful if
	 *  you need to track the mouse outside of a specific window and
	 *  CaptureMouse() doesn't fit your needs. For example, it could be
	 *  useful if you need to track the mouse while dragging a window, where
	 *  coordinates relative to a window might not be in sync at all times.
	 *
	 *  \note GetMouseState() returns the mouse position as SDL understands
	 *		it from the last pump of the event queue. This function, however,
	 *		queries the OS for the current mouse position, and as such, might
	 *		be a slightly less efficient function. Unless you know what you're
	 *		doing and have a good reason to use this function, you probably want
	 *		GetMouseState() instead.
	 *
	 *  \return The current button state as a bitmask, which can be tested using ButtonMask.
	 */
	inline Uint32 GetGlobalMouseState() { return SDL_GetGlobalMouseState(NULL, NULL); }
#endif

	/**
	 *  Retrieve the relative state of the mouse.
	 *
	 *  The current button state is returned as a button bitmask, which can
	 *  be tested using ButtonMask, and x and y are set to the
	 *  mouse deltas since the last call to GetRelativeMouseState().
	 */
	Uint32 GetRelativeMouseState(int& x, int& y);

	/**
	 *  \brief Moves the mouse to the given position within the window.
	 *
	 *  \param window The window to move the mouse into, or NULL for the current mouse focus
	 *  \param x The x coordinate within the window
	 *  \param y The y coordinate within the window
	 *
	 *  \note This function generates a mouse motion event
	 */
	void WarpMouseInWindow(Window& window, int x, int y);

	/**
	 *  \brief Moves the mouse to the given position in global screen space.
	 *
	 *  \param x The x coordinate
	 *  \param y The y coordinate
	 *  \return 0 on success, -1 on error (usually: unsupported by a platform).
	 *
	 *  \note This function generates a mouse motion event
	 */
	int WarpMouseGlobal(int x, int y);


	/**
	 *  \brief Capture the mouse, to track input outside an SDL window.
	 *
	 *  \param enabled Whether or not to enable capturing
	 *
	 *  Capturing enables your app to obtain mouse events globally, instead of
	 *  just within your window. Not all video targets support this function.
	 *  When capturing is enabled, the current window will get all mouse events,
	 *  but unlike relative mode, no change is made to the cursor and it is
	 *  not restrained to your window.
	 *
	 *  This function may also deny mouse input to other windows--both those in
	 *  your application and others on the system--so you should use this
	 *  function sparingly, and in small bursts. For example, you might want to
	 *  track the mouse while the user is dragging something, until the user
	 *  releases a mouse button. It is not recommended that you capture the mouse
	 *  for long periods of time, such as the entire time your app is running.
	 *
	 *  While captured, mouse events still report coordinates relative to the
	 *  current (foreground) window, but those coordinates may be outside the
	 *  bounds of the window (including negative values). Capturing is only
	 *  allowed for the foreground window. If the window loses focus while
	 *  capturing, the capture will be disabled automatically.
	 *
	 *  While capturing is enabled, the current window will have the
	 *  SDL_WINDOW_MOUSE_CAPTURE flag set.
	 *
	 *  \return 0 on success, or -1 if not supported.
	 */
	int CaptureMouse(bool enabled);

	/**
	 *  \brief Set relative mouse mode.
	 *
	 *  \param enabled Whether or not to enable relative mode
	 *
	 *  \return 0 on success, or -1 if relative mode is not supported.
	 *
	 *  While the mouse is in relative mode, the cursor is hidden, and the
	 *  driver will try to report continuous motion in the current window.
	 *  Only relative motion events will be delivered, the mouse position
	 *  will not change.
	 *
	 *  \note This function will flush any pending mouse motion.
	 */
	int SetRelativeMouseMode(bool enabled);
	// Query whether relative mouse mode is enabled.
	bool GetRelativeMouseMode();

	enum class Button {
		LEFT = SDL_BUTTON_LEFT,
		MIDDLE = SDL_BUTTON_MIDDLE,
		RIGHT = SDL_BUTTON_RIGHT,
		X1 = SDL_BUTTON_X1,
		X2 = SDL_BUTTON_X2
	};

	enum class ButtonMask {
		LMASK = SDL_BUTTON_LMASK,
		MMASK = SDL_BUTTON_MMASK,
		RMASK = SDL_BUTTON_RMASK,
		X1MASK = SDL_BUTTON_X1MASK,
		X2MASK = SDL_BUTTON_X2MASK
	};
}

#endif
#endif