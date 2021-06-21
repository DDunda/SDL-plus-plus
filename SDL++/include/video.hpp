#pragma once

#include <SDL_video.h>
#include "rect.hpp"
#include "surface.hpp"

namespace SDL {

	// The flags on a window
	enum class WindowFlags {
		FULLSCREEN         = SDL_WINDOW_FULLSCREEN,         // fullscreen window
		OPENGL             = SDL_WINDOW_OPENGL,             // window usable with OpenGL context
		SHOWN              = SDL_WINDOW_SHOWN,              // window is visible
		HIDDEN             = SDL_WINDOW_HIDDEN,             // window is not visible
		BORDERLESS         = SDL_WINDOW_BORDERLESS,         // no window decoration
		RESIZABLE          = SDL_WINDOW_RESIZABLE,          // window can be resized
		MINIMIZED          = SDL_WINDOW_MINIMIZED,          // window is minimized
		MAXIMIZED          = SDL_WINDOW_MAXIMIZED,          // window is maximized
		INPUT_GRABBED      = SDL_WINDOW_INPUT_GRABBED,      // window has grabbed input focus
		INPUT_FOCUS        = SDL_WINDOW_INPUT_FOCUS,        // window has input focus
		MOUSE_FOCUS        = SDL_WINDOW_MOUSE_FOCUS,        // window has mouse focus
		FULLSCREEN_DESKTOP = SDL_WINDOW_FULLSCREEN_DESKTOP,
		FOREIGN            = SDL_WINDOW_FOREIGN,            // window not created by SDL
		ALLOW_HIGHDPI      = SDL_WINDOW_ALLOW_HIGHDPI,      /* window should be created in high - DPI mode if supported.
		                                                       On macOS NSHighResolutionCapable must be set true in the
		                                                       application's Info.plist for this to have any effect. */
		MOUSE_CAPTURE      = SDL_WINDOW_MOUSE_CAPTURE,      // window has mouse captured (unrelated to INPUT_GRABBED)
		ALWAYS_ON_TOP      = SDL_WINDOW_ALWAYS_ON_TOP,      // window should always be above others
		SKIP_TASKBAR       = SDL_WINDOW_SKIP_TASKBAR,       // window should not be added to the taskbar
		UTILITY            = SDL_WINDOW_UTILITY,            // window should be treated as a utility window
		TOOLTIP            = SDL_WINDOW_TOOLTIP,            // window should be treated as a tooltip
		POPUP_MENU         = SDL_WINDOW_POPUP_MENU,         // window should be treated as a popup menu
		VULKAN             = SDL_WINDOW_VULKAN              // window usable for Vulkan surface
	};

	// Event subtype for window events
	enum class WindowEventID {
		NONE         = SDL_WINDOWEVENT_NONE,         // Never used
		SHOWN        = SDL_WINDOWEVENT_SHOWN,        // Window has been shown
		HIDDEN       = SDL_WINDOWEVENT_HIDDEN,       // Window has been hidden
		EXPOSED      = SDL_WINDOWEVENT_EXPOSED,      // Window has been exposed and should be redrawn
		MOVED        = SDL_WINDOWEVENT_MOVED,        // Window has been moved to data1, data2
		RESIZED      = SDL_WINDOWEVENT_RESIZED,      // Window has been resized to data1xdata2
		SIZE_CHANGED = SDL_WINDOWEVENT_SIZE_CHANGED, // The window size has changed, either as a result of an API call, or through the system or user changing the window size.
		MINIMIZED    = SDL_WINDOWEVENT_MINIMIZED,    // Window has been minimized
		MAXIMIZED    = SDL_WINDOWEVENT_MAXIMIZED,    // Window has been maximized
		RESTORED     = SDL_WINDOWEVENT_RESTORED,     // Window has been restored to normal size and position
		ENTER        = SDL_WINDOWEVENT_ENTER,        // Window has gained mouse focus
		LEAVE        = SDL_WINDOWEVENT_LEAVE,        // Window has lost mouse focus
		FOCUS_GAINED = SDL_WINDOWEVENT_FOCUS_GAINED, // Window has gained keyboard focus
		FOCUS_LOST   = SDL_WINDOWEVENT_FOCUS_LOST,   // Window has lost keyboard focus
		CLOSE        = SDL_WINDOWEVENT_CLOSE,        // The window manager requests that the window be closed
		TAKE_FOCUS   = SDL_WINDOWEVENT_TAKE_FOCUS,   // Window is being offered a focus (should SetWindowInputFocus() on itself or a subwindow, or ignore)
		HIT_TEST     = SDL_WINDOWEVENT_HIT_TEST      // Window had a hit test that wasn't SDL_HITTEST_NORMAL.
	};

	// Event subtype for display events
	enum class DisplayEventID {
		NONE        = SDL_DISPLAYEVENT_NONE,       // Never used
		ORIENTATION = SDL_DISPLAYEVENT_ORIENTATION // Display orientation has changed to data1
	};

	// Possible return values from the SDL::HitTest callback.
	enum class HitTestResult {
		NORMAL = SDL_HITTEST_NORMAL,             // Region is normal. No special properties.
		DRAGGABLE = SDL_HITTEST_DRAGGABLE,          // Region can drag entire window.
		RESIZE_TOPLEFT = SDL_HITTEST_RESIZE_TOPLEFT,
		RESIZE_TOP = SDL_HITTEST_RESIZE_TOP,
		RESIZE_TOPRIGHT = SDL_HITTEST_RESIZE_TOPRIGHT,
		RESIZE_RIGHT = SDL_HITTEST_RESIZE_RIGHT,
		RESIZE_BOTTOMRIGHT = SDL_HITTEST_RESIZE_BOTTOMRIGHT,
		RESIZE_BOTTOM = SDL_HITTEST_RESIZE_BOTTOM,
		RESIZE_BOTTOMLEFT = SDL_HITTEST_RESIZE_BOTTOMLEFT,
		RESIZE_LEFT = SDL_HITTEST_RESIZE_LEFT
	};
	// Callback used for hit-testing.
	typedef SDL_HitTest HitTest;

	// \brief Get the number of video drivers compiled into SDL
	static int GetNumVideoDrivers() { return SDL_GetNumVideoDrivers(); }

	/**
	 *  \brief Get the name of a built in video driver.
	 *
	 *  \note The video drivers are presented in the order in which they are
	 *		normally checked during initialization.
	 */
	static const char* GetVideoDriver(int index) { return SDL_GetVideoDriver(index); }

	/**
	 *  \brief Initialize the video subsystem, specifying a video driver.
	 *
	 *  \param driver_name Initialize a specific driver by name.
	 *
	 *  \return 0 on success, -1 on error
	 *
	 *  This function initializes the video subsystem; setting up a connection
	 *  to the window manager, etc, and determines the available display modes
	 *  and pixel formats, but does not initialize a window or graphics mode.
	 */
	static int VideoInit(const char* driver_name) { return SDL_VideoInit(driver_name); }

	/**
	 *  \brief Initialize the video subsystem with the default video driver.
	 *
	 *  \return 0 on success, -1 on error
	 *
	 *  This function initializes the video subsystem; setting up a connection
	 *  to the window manager, etc, and determines the available display modes
	 *  and pixel formats, but does not initialize a window or graphics mode.
	 */
	static int VideoInit() { return SDL_VideoInit(NULL); }

	/**
	 *  \brief Shuts down the video subsystem.
	 *
	 *  This function closes all windows, and restores the original video mode.
	 */
	static void VideoQuit() { SDL_VideoQuit(); }

	/**
	 *  \brief Returns the name of the currently initialized video driver.
	 *
	 *  \return The name of the current video driver or NULL if no driver
	 *		  has been initialized
	 */
	static const char* GetCurrentVideoDriver() { return SDL_GetCurrentVideoDriver(); }

	struct Display {
		// \brief  The structure that defines a display mode
		typedef SDL_DisplayMode Mode;

		enum class Orientation {
			UNKNOWN = SDL_ORIENTATION_UNKNOWN,                     // The display orientation can't be determined
			LANDSCAPE = SDL_ORIENTATION_LANDSCAPE,                 // The display is in landscape mode, with the right side up, relative to portrait mode
			PORTRAIT = SDL_ORIENTATION_PORTRAIT,                   // The display is in portrait mode
			LANDSCAPE_FLIPPED = SDL_ORIENTATION_LANDSCAPE_FLIPPED, // The display is in landscape mode, with the left side up, relative to portrait mode
			PORTRAIT_FLIPPED = SDL_ORIENTATION_PORTRAIT_FLIPPED    // The display is in portrait mode, upside down
		};

		int index;

		// \brief Returns the number of available video displays.
		static int GetNumVideoDisplays() { return SDL_GetNumVideoDisplays(); }

		/**
		 *  \brief Get the name of a display in UTF-8 encoding
		 *
		 *  \return The name of a display, or NULL for an invalid display index.
		 */
		const char* GetName() { return SDL_GetDisplayName(index); }

		/**
		 *  \brief Get the desktop area represented by a display, with the primary
		 *		 display located at 0,0
		 *
		 *  \return 0 on success, or -1 if the index is out of range.
		 */
		int GetBounds(Rect& rect) { return SDL_GetDisplayBounds(index, &rect.rect); }

		/**
		 *  \brief Get the usable desktop area represented by a display, with the
		 *		 primary display located at 0,0
		 *
		 *  This is the same area as SDL_GetDisplayBounds() reports, but with portions
		 *  reserved by the system removed. For example, on Mac OS X, this subtracts
		 *  the area occupied by the menu bar and dock.
		 *
		 *  Setting a window to be fullscreen generally bypasses these unusable areas,
		 *  so these are good guidelines for the maximum space available to a
		 *  non-fullscreen window.
		 *
		 *  \return 0 on success, or -1 if the index is out of range.
		 */
		int GetUsableBounds(Rect& rect) { return SDL_GetDisplayUsableBounds(index, &rect.rect); }

		/**
		 *  \brief Get the dots/pixels-per-inch for a display
		 *
		 *  \note Diagonal, horizontal and vertical DPI can all be optionally
		 *		returned if the parameter is non-NULL.
		 *
		 *  \return 0 on success, or -1 if no DPI information is available or the index is out of range.
		 */
		int GetDPI(float* ddpi, float* hdpi, float* vdpi) { return SDL_GetDisplayDPI(index, ddpi, hdpi, vdpi); }

		/**
		 *  \brief Get the orientation of a display
		 *
		 *  \return The orientation of the display, or SDL_ORIENTATION_UNKNOWN if it isn't available.
		 */
		Orientation GetOrientation() { return (Orientation)SDL_GetDisplayOrientation(index); }

		// \brief Returns the number of available display modes.
		int GetNumModes() { return SDL_GetNumDisplayModes(index); }

		/**
		 *  \brief Fill in information about a specific display mode.
		 *
		 *  \note     The display modes are sorted in this priority:
		 *            \li bits per pixel -> more colours to fewer colours
		 *            \li width          -> largest to smallest
		 *            \li height         -> largest to smallest
		 *            \li refresh rate   -> highest to lowest
		 */
		int GetMode(int modeIndex, Mode& mode) { return SDL_GetDisplayMode(index, modeIndex, &mode); }

		// \brief Fill in information about the desktop display mode.
		int GetDesktopMode(Mode& mode) { return SDL_GetDesktopDisplayMode(index, &mode); }

		// brief Fill in information about the current display mode.
		int GetCurrentMode(Mode& mode) { return SDL_GetCurrentDisplayMode(index, &mode); }

		/**
		 *  \brief Get the closest match to the requested display mode.
		 *
		 *  \param displayIndex The index of display from which mode should be queried.
		 *  \param mode The desired display mode
		 *  \param closest A pointer to a display mode to be filled in with the closest
		 *				 match of the available display modes.
		 *
		 *  \return The passed in value \c closest, or NULL if no matching video mode
		 *		  was available.
		 *
		 *  The available display modes are scanned, and \c closest is filled in with the
		 *  closest mode matching the requested mode and returned.  The mode format and
		 *  refresh_rate default to the desktop mode if they are 0.  The modes are
		 *  scanned with size being first priority, format being second priority, and
		 *  finally checking the refresh_rate.  If all the available modes are too
		 *  small, then NULL is returned.
		 */
		Mode* GetClosestMode(const Mode& mode, Mode& closest) { return SDL_GetClosestDisplayMode(index, &mode, &closest); }
	};

	// \brief The type used to identify a window
	struct Window {
		SDL_Window* window = NULL;
		bool freeWindow = false;
		int error;

		Window(const Window& wnd) : Window(wnd.window, false) {}
		Window(Window&& wnd) noexcept : Window(wnd.window, wnd.freeWindow) { wnd.window = NULL; wnd.freeWindow = false; }
		Window& operator=(Window that)
		{
			if(freeWindow) SDL_DestroyWindow(window);
			window = that.window;
			freeWindow = that.freeWindow;
			that.freeWindow = false;
			return *this;
		}

		Window(SDL_Window* window = NULL, bool free = false) : window(window), freeWindow(free) {}

		/**
		 *  \brief Create this window with the specified position, dimensions, and flags.
		 *
		 *  \param title The title of the window, in UTF-8 encoding.
		 *  \param shape The position and size of the window
		 *  \param flags The flags for the window, a mask of any of the following:
		 *			   ::SDL_WINDOW_FULLSCREEN,	::SDL_WINDOW_OPENGL,
		 *			   ::SDL_WINDOW_HIDDEN,		::SDL_WINDOW_BORDERLESS,
		 *			   ::SDL_WINDOW_RESIZABLE,	 ::SDL_WINDOW_MAXIMIZED,
		 *			   ::SDL_WINDOW_MINIMIZED,	 ::SDL_WINDOW_INPUT_GRABBED,
		 *			   ::SDL_WINDOW_ALLOW_HIGHDPI, ::SDL_WINDOW_VULKAN.
		 *
		 *  If the window is created with the SDL_WINDOW_ALLOW_HIGHDPI flag, its size
		 *  in pixels may differ from its size in screen coordinates on platforms with
		 *  high-DPI support (e.g. iOS and Mac OS X). Use SDL_GetWindowSize() to query
		 *  the client area's size in screen coordinates, and SDL_GL_GetDrawableSize(),
		 *  SDL_Vulkan_GetDrawableSize(), or SDL_GetRendererOutputSize() to query the
		 *  drawable size in pixels.
		 *
		 *  If the window is created with any of the SDL_WINDOW_OPENGL or
		 *  SDL_WINDOW_VULKAN flags, then the corresponding LoadLibrary function
		 *  (SDL_GL_LoadLibrary or SDL_Vulkan_LoadLibrary) is called and the
		 *  corresponding UnloadLibrary function is called by SDL_DestroyWindow().
		 *
		 *  If SDL_WINDOW_VULKAN is specified and there isn't a working Vulkan driver,
		 *  SDL_CreateWindow() will fail because SDL_Vulkan_LoadLibrary() will fail.
		 *
		 *  \note On non-Apple devices, SDL requires you to either not link to the
		 *		Vulkan loader or link to a dynamic library version. This limitation
		 *		may be removed in a future version of SDL.
		 */
		Window(const char* title, const Rect& shape, Uint32 flags)
			: Window(SDL_CreateWindow(title, shape.x, shape.y, shape.w, shape.h, flags), true) {}

		/**
		 *  \brief Create an SDL window from an existing native window.
		 *
		 *  \param data A pointer to driver-dependent window creation data
		 */
		static Window WindowFrom(const void* data) { return Window(SDL_CreateWindowFrom(data), true); }

		// \brief Get this window from a stored ID, or NULL if it doesn't exist.
		static Window WindowFromID(Uint32 id) { return Window(SDL_GetWindowFromID(id), false); }

		// \brief Get the window that currently has an input grab enabled.
		static Window GetGrabbed() { return Window(SDL_GetGrabbedWindow(), false); }

		// \brief Destroy this window.
		~Window() { if(freeWindow) SDL_DestroyWindow(window); }

		/**
		 *  \brief Get the display index associated with this window.
		 *
		 *  \return the display index of the display containing the center of the
		 *		  window, or -1 on error.
		 */
		Display GetDisplay() { return { SDL_GetWindowDisplayIndex(window) }; }
		// \brief Get the display index associated with this window.
		Window& GetDisplay(Display& display) { display = { SDL_GetWindowDisplayIndex(window) }; return *this; }

		/**
		 *  \brief Set the display mode used when a fullscreen window is visible.
		 *
		 *  By default the window's dimensions and the desktop format and refresh rate
		 *  are used.
		 *
		 *  error is set to 0 on success, or -1 if setting the display mode failed.
		 *
		 *  \param window The window for which the display mode should be set.
		 *  \param mode The mode to use, or NULL for the default mode.
		 */
		Window& SetDisplayMode(const Display::Mode& mode) { error = SDL_SetWindowDisplayMode(window, &mode); return *this; }

		/**
		 *  \brief Set the display mode used when a fullscreen window is visible.
		 *
		 *  By default the window's dimensions and the desktop format and refresh rate
		 *  are used.
		 *
		 *  \param window The window for which the display mode should be set.
		 *  \param mode The mode to use, or NULL for the default mode.
		 *
		 *  error is set to 0 on success, or -1 if setting the display mode failed.
		 */
		Window& SetDefaultDisplayMode() { error = SDL_SetWindowDisplayMode(window, NULL); return *this; }

		// \brief Fill in information about the display mode used when a fullscreen window is visible.
		Window& GetDisplayMode(Display::Mode& mode) { error = SDL_GetWindowDisplayMode(window, &mode);  return *this; }

		// \brief Get the pixel format associated with the window.
		Uint32 GetPixelFormat() { return SDL_GetWindowPixelFormat(window); }
		// \brief Get the pixel format associated with the window.
		Window& GetPixelFormat(Uint32& format) { format = SDL_GetWindowPixelFormat(window); return *this; }

		// \brief Get the numeric ID of this window, for logging purposes.
		Uint32 GetID() { return SDL_GetWindowID(window); }
		// \brief Get the numeric ID of this window, for logging purposes.
		Window& GetID(Uint32& id) { id = SDL_GetWindowID(window); return *this; }

		// \brief Get this window's flags.
		Uint32 GetFlags() { return SDL_GetWindowFlags(window); }
		// \brief Get this window's flags.
		Window& GetFlags(Uint32& flags) { flags = SDL_GetWindowFlags(window); return *this; }

		// \brief Set the title of this window.
		Window& SetTitle(const char* title) { SDL_SetWindowTitle(window, title); return *this; }

		// Get the title of this window.
		const char* GetTitle() { return SDL_GetWindowTitle(window); }
		// Get the title of this window.
		Window& GetTitle(const char* title) { title = SDL_GetWindowTitle(window); return *this; }

		/**
		 *  \brief Set the icon for this window.
		 *
		 *  \param icon The icon for the window.
		 */
		Window& SetIcon(Surface& icon) { SDL_SetWindowIcon(window, icon.surface); return *this; }

		/**
		 *  \brief Associate an arbitrary named pointer with this window.
		 *
		 *  \param name	 The name of the pointer.
		 *  \param userdata The associated pointer.
		 *
		 *  \return The previous value associated with 'name'
		 *
		 *  \note The name is case-sensitive.
		 */
		void* SetData(const char* name, void* data) { return SDL_SetWindowData(window, name, data); }

		/**
		 *  \brief Retrieve the data pointer associated with this window.
		 *
		 *  \param name	 The name of the pointer.
		 *
		 *  \return The value associated with 'name'
		 */
		void* GetData(const char* name) { return SDL_GetWindowData(window, name); }
		/**
		 *  \brief Retrieve the data pointer associated with this window.
		 *
		 *  \param name	 The name of the pointer.
		 *
		 *  \return The value associated with 'name'
		 */
		Window& GetData(const char* name, void*& data) { data = SDL_GetWindowData(window, name); return *this; }

		/**
		 *  \brief Set the position of this window.
		 *
		 *  \param pos		The coordinates of the window in screen coordinates, with
		 *				  optional ::SDL_WINDOWPOS_CENTERED and ::SDL_WINDOWPOS_UNDEFINED parts.
		 *
		 *  \note The window coordinate origin is the upper left of the display.
		 */
		Window& SetPosition(const Point& pos) { SDL_SetWindowPosition(window, pos.x, pos.y); return *this; }

		/**
		 *  \brief Get the position of this window.
		 *
		 *  \return The position of the window.
		 */
		Point GetPosition() {
			Point returnVal;
			SDL_GetWindowPosition(window, &returnVal.x, &returnVal.y);
			return returnVal;
		}
		/**
		 *  \brief Get the position of this window.
		 *
		 *  \param pos		Reference to variable for storing the position, in screen
		 *				  coordinates.
		 */
		Window& GetPosition(Point& pos) { SDL_GetWindowPosition(window, &pos.x, &pos.y); return *this; }
		/**
		 *  \brief Get the position of this window.
		 *
		 *  \param x		Pointer to variable for storing the x position, in screen
		 *				  coordinates. May be NULL.
		 *  \param y		Pointer to variable for storing the y position, in screen
		 *				  coordinates. May be NULL.
		 */
		Window& GetPosition(int* x, int* y) { SDL_GetWindowPosition(window, x, y); return *this; }

		/**
		 *  \brief Set the size of this window's client area.
		 *
		 *  \param w		The width of the window, in screen coordinates. Must be >0.
		 *  \param h		The height of the window, in screen coordinates. Must be >0.
		 *
		 *  \note Fullscreen windows automatically match the size of the display mode,
		 *		and you should use SDL_SetWindowDisplayMode() to change their size.
		 *
		 *  The window size in screen coordinates may differ from the size in pixels, if
		 *  the window was created with SDL_WINDOW_ALLOW_HIGHDPI on a platform with
		 *  high-dpi support (e.g. iOS or OS X). Use SDL_GL_GetDrawableSize() or
		 *  SDL_GetRendererOutputSize() to get the real client area size in pixels.
		 */
		Window& SetSize(int w, int h) { SDL_SetWindowSize(window, w, h); return *this; }
		/**
		 *  \brief Set the size of this window's client area.
		 *
		 *  \param size		The size of the window, in screen coordinates. Must be >0.
		 *
		 *  \note Fullscreen windows automatically match the size of the display mode,
		 *		and you should use SetWindowDisplayMode() to change their size.
		 *
		 *  The window size in screen coordinates may differ from the size in pixels, if
		 *  the window was created with SDL_WINDOW_ALLOW_HIGHDPI on a platform with
		 *  high-dpi support (e.g. iOS or OS X). Use GL_GetDrawableSize() or
		 *  SDL_GetRendererOutputSize() to get the real client area size in pixels.
		 */
		Window& SetSize(const Point& size) { SDL_SetWindowSize(window, size.w, size.h); return *this; }

		/**
		 *  \brief Get the size of this window's client area.
		 * 
		 *  \return The size of this window.
		 *
		 *  The window size in screen coordinates may differ from the size in pixels, if
		 *  the window was created with SDL_WINDOW_ALLOW_HIGHDPI on a platform with
		 *  high-dpi support (e.g. iOS or OS X). Use SDL_GL_GetDrawableSize() or
		 *  SDL_GetRendererOutputSize() to get the real client area size in pixels.
		 */
		Point& GetSize() {
			Point returnVal;
			SDL_GetWindowSize(window, &returnVal.w, &returnVal.h);
			return returnVal;
		}
		/**
		 *  \brief Get the size of this window's client area.
		 *
		 *  \param size		Reference to variable for storing the size, in screen
		 *				  coordinates.
		 *
		 *  The window size in screen coordinates may differ from the size in pixels, if
		 *  the window was created with SDL_WINDOW_ALLOW_HIGHDPI on a platform with
		 *  high-dpi support (e.g. iOS or OS X). Use SDL_GL_GetDrawableSize() or
		 *  SDL_GetRendererOutputSize() to get the real client area size in pixels.
		 */
		Window& GetSize(Point& size) { SDL_GetWindowSize(window, &size.w, &size.h); return *this; }
		/**
		 *  \brief Get the size of this window's client area.
		 *
		 *  \param w		Pointer to variable for storing the width, in screen
		 *				  coordinates. May be NULL.
		 *  \param h		Pointer to variable for storing the height, in screen
		 *				  coordinates. May be NULL.
		 *
		 *  The window size in screen coordinates may differ from the size in pixels, if
		 *  the window was created with SDL_WINDOW_ALLOW_HIGHDPI on a platform with
		 *  high-dpi support (e.g. iOS or OS X). Use SDL_GL_GetDrawableSize() or
		 *  SDL_GetRendererOutputSize() to get the real client area size in pixels.
		 */
		Window& GetSize(int* x, int* y) { SDL_GetWindowSize(window, x, y); return *this; }

		/**
		 *  \brief Get the size of this window's borders (decorations) around the client area.
		 *
		 *  error is set to 0 on success, or -1 if getting this information is not supported.
		 *
		 *  \param top Pointer to variable for storing the size of the top border. NULL is permitted.
		 *  \param left Pointer to variable for storing the size of the left border. NULL is permitted.
		 *  \param bottom Pointer to variable for storing the size of the bottom border. NULL is permitted.
		 *  \param right Pointer to variable for storing the size of the right border. NULL is permitted.
		 *
		 *  \note if this function fails (returns -1), the size values will be
		 *		initialized to 0, 0, 0, 0 (if a non-NULL pointer is provided), as
		 *		if the window in question was borderless.
		 */
		Window& GetBordersSize(int* top, int* left, int* bottom, int* right) { error = SDL_GetWindowBordersSize(window, top, left, bottom, right); return *this; }

		/**
		 *  \brief Set the minimum size of this window's client area.
		 *
		 *  \param min_size	 The minimum size of the window, must be >0
		 *
		 *  \note You can't change the minimum size of a fullscreen window, it
		 *		automatically matches the size of the display mode.
		 */
		Window& SetMinimumSize(const Point& min_size) { SDL_SetWindowMinimumSize(window, min_size.w, min_size.h); return *this; }

		/**
		 *  \brief Get the minimum size of this window's client area.
		 * 
		 *  \return The size of the window
		 */
		Point GetMinimumSize() {
			Point returnVal;
			SDL_GetWindowMinimumSize(window, &returnVal.w, &returnVal.h);
			return returnVal;
		}
		/**
		 *  \brief Get the minimum size of this window's client area.
		 *
		 *  \return The size of the window
		 */
		Window& GetMinimumSize(Point& point) { SDL_GetWindowMinimumSize(window, &point.w, &point.h); return *this; }
		/**
		 *  \brief Get the minimum size of this window's client area.
		 *
		 *  \param w		Pointer to variable for storing the minimum width, may be NULL
		 *  \param h		Pointer to variable for storing the minimum height, may be NULL
		 */
		Window& GetMinimumSize(int* w, int* h) { SDL_GetWindowMinimumSize(window, w, h); return *this; }

		/**
		 *  \brief Set the maximum size of this window's client area.
		 *
		 *  \param window	The window to set a new maximum size.
		 *  \param max_size	 The maximum size of the window, must be >0
		 *
		 *  \note You can't change the maximum size of a fullscreen window, it
		 *		automatically matches the size of the display mode.
		 */
		Window& SetMaximumSize(const Point& max_size) { SDL_SetWindowMaximumSize(window, max_size.w, max_size.h); return *this; }

		/**
		 *  \brief Get the maximum size of this window's client area.
		 * 
		 *  \return			The window size
		 */
		Point GetMaximumSize() {
			Point returnVal;
			SDL_GetWindowMaximumSize(window, &returnVal.w, &returnVal.h);
			return returnVal;
		}
		// \brief Get the maximum size of this window's client area.
		Window& GetMaximumSize(Point& point) { SDL_GetWindowMaximumSize(window, &point.w, &point.h); return *this; }
		/**
		 *  \brief Get the maximum size of this window's client area.
		 *
		 *  \param w		Pointer to variable for storing the maximum width, may be NULL
		 *  \param h		Pointer to variable for storing the maximum height, may be NULL
		 */
		Window& GetMaximumSize(int* w, int* h) { SDL_GetWindowMaximumSize(window, w, h); return *this; }

		/**
		 *  \brief Set the border state of this window.
		 *
		 *  This will add or remove the window's SDL_WINDOW_BORDERLESS flag and
		 *  add or remove the border from the actual window. This is a no-op if the
		 *  window's border already matches the requested state.
		 *
		 *  \param bordered false to remove border, true to add border.
		 *
		 *  \note You can't change the border state of a fullscreen window.
		 */
		Window& SetBordered(bool bordered) { SDL_SetWindowBordered(window, (SDL_bool)bordered); return *this; }

		/**
		 *  \brief Set the user-resizable state of this window.
		 *
		 *  This will add or remove the window's SDL_WINDOW_RESIZABLE flag and
		 *  allow/disallow user resizing of the window. This is a no-op if the
		 *  window's resizable state already matches the requested state.
		 *
		 *  \param resizable true to allow resizing, false to disallow.
		 *
		 *  \note You can't change the resizable state of a fullscreen window.
		 */
		Window& SetResizable(bool resizable) { SDL_SetWindowResizable(window, (SDL_bool)resizable); return *this; }

		// \brief Show this window.
		Window& Show() { SDL_ShowWindow(window); return *this; };

		// \brief Hide this window.
		Window& Hide() { SDL_HideWindow(window); return *this; };

		// \brief Raise this window above other windows and set the input focus.
		Window& Raise() { SDL_RaiseWindow(window); return *this; }

		// \brief Make this window as large as possible.
		Window& Maximize() { SDL_MaximizeWindow(window); return *this; }

		// \brief Minimize this window to an iconic representation.
		Window& Minimize() { SDL_MinimizeWindow(window); return *this; }

		// \brief Restore the size and position of a minimized or maximized window.
		Window& Restore() { SDL_RestoreWindow(window); return *this; };

		/**
		 *  \brief Set this window's fullscreen state.
		 *
		 *  error is set to 0 on success, or -1 if setting the display mode failed.
		 */
		Window& SetFullscreen(Uint32 flags) { SDL_SetWindowFullscreen(window, flags); return *this; }

		/**
		 *  \brief Get the SDL surface associated with the window.
		 *
		 *  \return The window's framebuffer surface, or NULL on error.
		 *
		 *  A new surface will be created with the optimal format for the window,
		 *  if necessary. This surface will be freed when the window is destroyed.
		 *
		 *  \note You may not combine this with 3D or the rendering API on this window.
		 */
		Surface GetSurface() { return SDL_GetWindowSurface(window); }
		/**
		 *  \brief Get the SDL surface associated with the window.
		 *
		 *  A new surface will be created with the optimal format for the window,
		 *  if necessary. This surface will be freed when the window is destroyed.
		 *
		 *  \note You may not combine this with 3D or the rendering API on this window.
		 */
		Window& GetSurface(Surface& surface) {
			surface.~Surface();
			surface.surface = SDL_GetWindowSurface(window);
			surface.freeSurface = false;
			return *this;
		}

		/**
		 *  \brief Copy the window surface to the screen.
		 *
		 *  error is set to 0 on success, or -1 on error.
		 */
		Window& UpdateSurface() { error = SDL_UpdateWindowSurface(window); return *this; }

		/**
		 *  \brief Copy a number of rectangles on the window surface to the screen.
		 *
		 *  error is set to 0 on success, or -1 on error.
		 */
		Window& UpdateSurfaceRects(const std::vector<Rect>& rects) { error = SDL_UpdateWindowSurfaceRects(window, (const SDL_Rect*)rects.data(), rects.size()); return *this; }
		
		/**
		 *  \brief Copy a number of rectangles on the window surface to the screen.
		 *
		 *  error is set to 0 on success, or -1 on error.
		 */
		Window& UpdateSurfaceRects(const Rect* rects, int numrects) { error = SDL_UpdateWindowSurfaceRects(window, (const SDL_Rect*)rects, numrects); return *this; }

		/**
		 *  \brief Set this window's input grab mode.
		 *
		 *  \param window The window for which the input grab mode should be set.
		 *  \param grabbed This is true to grab input, and false to release input.
		 *
		 *  If the caller enables a grab while another window is currently grabbed,
		 *  the other window loses its grab in favor of the caller's window.
		 */
		Window& SetGrab(bool grabbed) { SDL_SetWindowGrab(window, (SDL_bool)grabbed); return *this; }

		/**
		 *  \brief Get this window's input grab mode.
		 *
		 *  \return This returns true if input is grabbed, and false otherwise.
		 */
		bool GetGrab() { return SDL_GetWindowGrab(window) == SDL_TRUE; }
		//\brief Get this window's input grab mode.
		Window& GetGrab(bool& grab) { grab = SDL_GetWindowGrab(window) == SDL_TRUE; return *this; }

		/**
		 *  \brief Set the brightness (gamma correction) for this window.
		 *
		 *  error is set to 0 on success, or -1 if setting the brightness isn't supported.
		 */
		Window& SetBrightness(float brightness) { error = SDL_SetWindowBrightness(window, brightness); return *this; }

		/**
		 *  \brief Get the brightness (gamma correction) for this window.
		 *
		 *  \return The last brightness value passed to SetWindowBrightness()
		 */
		float GetBrightness() { return SDL_GetWindowBrightness(window); }
		/**
		 *  \brief Get the brightness (gamma correction) for this window.
		 *
		 *  \return The last brightness value passed to SetWindowBrightness()
		 */
		Window& GetBrightness(float& brightness) { brightness = SDL_GetWindowBrightness(window); return *this; }

		/**
		 *  \brief Set the opacity for this window
		 *
		 *  \param opacity Opacity (0.0f - transparent, 1.0f - opaque) This will be
		 *				 clamped internally between 0.0f and 1.0f.
		 *
		 *  \return 0 on success, or -1 if setting the opacity isn't supported.
		 */
		int SetOpacity(float opacity) { return SDL_SetWindowOpacity(window, opacity); }

		/**
		 *  \brief Get the opacity of this window.
		 *
		 *  If transparency isn't supported on this platform, opacity will be reported
		 *  as 1.0f without error.
		 *
		 *  \param opacity Opacity (0.0f - transparent, 1.0f - opaque)
		 *
		 *  \return 0 on success, or -1 on error (invalid window, etc).
		 */
		int GetOpacity(float& opacity) { return SDL_GetWindowOpacity(window, &opacity); }


		/**
		 *  \brief Explicitly sets input focus to the window.
		 *
		 *  You almost certainly want SDL_RaiseWindow() instead of this function. Use
		 *  this with caution, as you might give focus to a window that's completely
		 *  obscured by other windows.
		 *
		 *  \return 0 on success, or -1 otherwise.
		 */
		int SetInputFocus() { return SDL_SetWindowInputFocus(window); }

		/**
		 *  \brief    Set the gamma ramp for this window.
		 *
		 *  \details  Set the gamma translation table for the red, green, and blue channels
		 *            of the video hardware.  Each table is an array of 256 16-bit quantities,
		 *            representing a mapping between the input and output for that channel.
		 *            The input is the index into the array, and the output is the 16-bit
		 *            gamma value at that index, scaled to the output colour precision.
		 *
		 *  \param    red:   The translation table for the red channel, or NULL.
		 *  \param    green: The translation table for the green channel, or NULL.
		 *  \param    blue:  The translation table for the blue channel, or NULL.
		 *
		 *  \return   0 on success, or -1 if gamma ramps are unsupported.
		 */
		int SetGammaRamp(const Uint16* red, const Uint16* green, const Uint16* blue) { return SDL_SetWindowGammaRamp(window, red, green, blue); }
		/**
		 *  \brief Get the gamma ramp for this window.
		 *
		 *  \param window The window from which the gamma ramp should be queried.
		 *  \param red   A pointer to a 256 element array of 16-bit quantities to hold
		 *			   the translation table for the red channel, or NULL.
		 *  \param green A pointer to a 256 element array of 16-bit quantities to hold
		 *			   the translation table for the green channel, or NULL.
		 *  \param blue  A pointer to a 256 element array of 16-bit quantities to hold
		 *			   the translation table for the blue channel, or NULL.
		 *
		 *  \return 0 on success, or -1 if gamma ramps are unsupported.
		 */
		int GetGammaRamp(Uint16* red, Uint16* green, Uint16* blue) { return SDL_GetWindowGammaRamp(window, red, green, blue); }

		/**
		 *  \brief Provide a callback that decides if this window region has special properties.
		 *
		 *  Normally windows are dragged and resized by decorations provided by the
		 *  system window manager (a title bar, borders, etc), but for some apps, it
		 *  makes sense to drag them from somewhere else inside the window itself; for
		 *  example, one might have a borderless window that wants to be draggable
		 *  from any part, or simulate its own title bar, etc.
		 *
		 *  This function lets the app provide a callback that designates pieces of
		 *  a given window as special. This callback is run during event processing
		 *  if we need to tell the OS to treat a region of the window specially; the
		 *  use of this callback is known as "hit testing."
		 *
		 *  Mouse input may not be delivered to your application if it is within
		 *  a special area; the OS will often apply that input to moving the window or
		 *  resizing the window and not deliver it to the application.
		 *
		 *  Specifying NULL for a callback disables hit-testing. Hit-testing is
		 *  disabled by default.
		 *
		 *  Platforms that don't support this functionality will return -1
		 *  unconditionally, even if you're attempting to disable hit-testing.
		 *
		 *  Your callback may fire at any time, and its firing does not indicate any
		 *  specific behavior (for example, on Windows, this certainly might fire
		 *  when the OS is deciding whether to drag your window, but it fires for lots
		 *  of other reasons, too, some unrelated to anything you probably care about
		 *  _and when the mouse isn't actually at the location it is testing_).
		 *  Since this can fire at any time, you should try to keep your callback
		 *  efficient, devoid of allocations, etc.
		 *
		 *  \param window The window to set hit-testing on.
		 *  \param callback The callback to call when doing a hit-test.
		 *  \param callback_data An app-defined void pointer passed to the callback.
		 *  \return 0 on success, -1 on error (including unsupported).
		 */
		int SetHitTest(HitTest callback, void* callback_data) { return SDL_SetWindowHitTest(window, callback, callback_data); }
	};

	/**
	 *  \brief Sets the window as a modal for another window
	 *
	 *  \param modal_window The window that should be modal
	 *  \param parent_window The parent window
	 *
	 *  \return 0 on success, or -1 otherwise.
	 */
	static int SetWindowModalFor(Window& modal_window, Window& parent_window) { SDL_SetWindowModalFor(modal_window.window, parent_window.window); }

	// \brief Returns whether the screensaver is currently enabled (default off).
	static bool IsScreenSaverEnabled() { return SDL_IsScreenSaverEnabled(); }

	// \brief Allow the screen to be blanked by a screensaver
	static void EnableScreenSaver() { SDL_EnableScreenSaver(); }

	// \brief Prevent the screen from being blanked by a screensaver
	static void DisableScreenSaver() { SDL_DisableScreenSaver(); };

	// \name OpenGL support functions
	namespace GL {
		// OpenGL configuration attributes
		enum class attr
		{
			RED_SIZE                   = SDL_GL_RED_SIZE,
			GREEN_SIZE                 = SDL_GL_GREEN_SIZE,
			BLUE_SIZE                  = SDL_GL_BLUE_SIZE,
			ALPHA_SIZE                 = SDL_GL_ALPHA_SIZE,
			BUFFER_SIZE                = SDL_GL_BUFFER_SIZE,
			DOUBLEBUFFER               = SDL_GL_DOUBLEBUFFER,
			DEPTH_SIZE                 = SDL_GL_DEPTH_SIZE,
			STENCIL_SIZE               = SDL_GL_STENCIL_SIZE,
			ACCUM_RED_SIZE             = SDL_GL_ACCUM_RED_SIZE,
			ACCUM_GREEN_SIZE           = SDL_GL_ACCUM_GREEN_SIZE,
			ACCUM_BLUE_SIZE            = SDL_GL_ACCUM_BLUE_SIZE,
			ACCUM_ALPHA_SIZE           = SDL_GL_ACCUM_ALPHA_SIZE,
			STEREO                     = SDL_GL_STEREO,
			MULTISAMPLEBUFFERS         = SDL_GL_MULTISAMPLEBUFFERS,
			MULTISAMPLESAMPLES         = SDL_GL_MULTISAMPLESAMPLES,
			ACCELERATED_VISUAL         = SDL_GL_ACCELERATED_VISUAL,
			RETAINED_BACKING           = SDL_GL_RETAINED_BACKING,
			CONTEXT_MAJOR_VERSION      = SDL_GL_CONTEXT_MAJOR_VERSION,
			CONTEXT_MINOR_VERSION      = SDL_GL_CONTEXT_MINOR_VERSION,
			CONTEXT_EGL                = SDL_GL_CONTEXT_EGL,
			CONTEXT_FLAGS              = SDL_GL_CONTEXT_FLAGS,
			CONTEXT_PROFILE_MASK       = SDL_GL_CONTEXT_PROFILE_MASK,
			SHARE_WITH_CURRENT_CONTEXT = SDL_GL_SHARE_WITH_CURRENT_CONTEXT,
			FRAMEBUFFER_SRGB_CAPABLE   = SDL_GL_FRAMEBUFFER_SRGB_CAPABLE,
			CONTEXT_RELEASE_BEHAVIOR   = SDL_GL_CONTEXT_RELEASE_BEHAVIOR,
			CONTEXT_RESET_NOTIFICATION = SDL_GL_CONTEXT_RESET_NOTIFICATION,
			CONTEXT_NO_ERROR           = SDL_GL_CONTEXT_NO_ERROR
		};

		/**
		 *  \brief    Dynamically load the default OpenGL library.
		 *
		 *  \return   0 on success, or -1 if the library couldn't be loaded.
		 *
		 *            This should be done after initializing the video driver, but before
		 *            creating any OpenGL windows.  If no OpenGL library is loaded, the default
		 *            library will be loaded upon creation of the first OpenGL window.
		 *
		 *  \note     If you do this, you need to retrieve all of the GL functions used in
		 *            your program from the dynamic library using GetProcAddress().
		 */
		static int LoadDefaultLibrary() { return SDL_GL_LoadLibrary(NULL); }

		 /**
		  *  \brief Dynamically load an OpenGL library.
		  *
		  *  \param path The platform dependent OpenGL library name, or NULL to open the
		  *			  default OpenGL library.
		  *
		  *  \return 0 on success, or -1 if the library couldn't be loaded.
		  *
		  *  This should be done after initializing the video driver, but before
		  *  creating any OpenGL windows.  If no OpenGL library is loaded, the default
		  *  library will be loaded upon creation of the first OpenGL window.
		  *
		  *  \note If you do this, you need to retrieve all of the GL functions used in
		  *		your program from the dynamic library using SDL_GL_GetProcAddress().
		  */
		static int LoadLibrary(const char* path) { return SDL_GL_LoadLibrary(path); }

		// Get the address of an OpenGL function.
		static void* GetProcAddress(const char* proc) { return SDL_GL_GetProcAddress(proc); }

		// \brief Unload the OpenGL library previously loaded by SDL_GL_LoadLibrary().
		static void UnloadLibrary() { SDL_GL_UnloadLibrary(); }

		// Return true if an OpenGL extension is supported for the current context.
		static bool ExtensionSupported(const char* extension) { return SDL_GL_ExtensionSupported(extension);}

		// \brief Reset all previously set OpenGL context attributes to their default values
		static void ResetAttributes() { SDL_GL_ResetAttributes(); }

		struct GLContext {
			enum class Profile
			{
				CORE = SDL_GL_CONTEXT_PROFILE_CORE,
				COMPATIBILITY = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY,
				ES = SDL_GL_CONTEXT_PROFILE_ES // GLX_CONTEXT_ES2_PROFILE_BIT_EXT
			};
			enum class Flag
			{
				DEBUG = SDL_GL_CONTEXT_DEBUG_FLAG,
				FORWARD_COMPATIBLE = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG,
				ROBUST_ACCESS = SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG,
				RESET_ISOLATION = SDL_GL_CONTEXT_RESET_ISOLATION_FLAG
			};
			enum class ReleaseBehaviour
			{
				NONE = SDL_GL_CONTEXT_RELEASE_BEHAVIOR_NONE,
				FLUSH = SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH
			};
			enum class ResetNotification
			{
				NO_NOTIFICATION = SDL_GL_CONTEXT_RESET_NO_NOTIFICATION,
				LOSE_CONTEXT = SDL_GL_CONTEXT_RESET_LOSE_CONTEXT
			};

			// An opaque handle to an OpenGL context.
			SDL_GLContext context;
			bool freeContext;

			// Create an OpenGL context for use with an OpenGL window, and make it current.
			GLContext(Window& window) : GLContext(SDL_GL_CreateContext(window.window), true) {}

			GLContext(SDL_GLContext context = NULL, bool freeContext = false) : context(context), freeContext(freeContext) {}

			// Delete an OpenGL context.
			~GLContext() { if(freeContext) SDL_GL_DeleteContext(context); }

			/**
			 *  \brief    Set up an OpenGL context for rendering into an OpenGL window.
			 *
			 *  \note     The context must have been created with a compatible window.
			 */
			int MakeCurrent(Window& window) { return SDL_GL_MakeCurrent(window.window, context); }

			// Get the currently active OpenGL window.
			static Window GetCurrentWindow() { return SDL_GL_GetCurrentWindow(); }

			// Get the currently active OpenGL context.
			static GLContext GetCurrentContext() { return SDL_GL_GetCurrentContext(); }

			/**
			 *  \brief    Set an OpenGL window attribute before window creation.
			 *
			 *  \return   0 on success, or -1 if the attribute could not be set.
			 */
			static int SetAttribute(attr attr, int value) { return SDL_GL_SetAttribute((SDL_GLattr)attr, value); }

			/**
			 *  \brief    Get the actual value for an attribute from the current context.
			 *
			 *  \return   0 on success, or -1 if the attribute could not be retrieved.
			 *            The integer at \c value will be modified in either case.
			 */
			static int GetAttribute(attr attr, int& value) { return SDL_GL_GetAttribute((SDL_GLattr)attr, &value); }
		};

		/**
		 *  \brief    Get the size of a window's underlying drawable in pixels (for use
		 *            with glViewport).
		 *

		/**
		 *  \brief Get the size of a window's underlying drawable in pixels (for use
		 *		 with glViewport).
		 *
		 *  \param window   Window from which the drawable size should be queried
		 * 
		 *  \return			The size in pixels
		 *
		 * This may differ from SDL_GetWindowSize() if we're rendering to a high-DPI
		 * drawable, i.e. the window was created with SDL_WINDOW_ALLOW_HIGHDPI on a
		 * platform with high-DPI support (Apple calls this "Retina"), and not disabled
		 * by the SDL_HINT_VIDEO_HIGHDPI_DISABLED hint.
		 */
		static Point GetDrawableSize(Window& window) {
			Point returnVal;
			SDL_GL_GetDrawableSize(window.window, &returnVal.w, &returnVal.h);
			return returnVal;
		}
		/**
		 *  \brief Get the size of a window's underlying drawable in pixels (for use
		 *		 with glViewport).
		 *
		 *  \param window   Window from which the drawable size should be queried
		 *  \param size		Reference to variable for storing the size in pixels
		 *
		 * This may differ from SDL_GetWindowSize() if we're rendering to a high-DPI
		 * drawable, i.e. the window was created with SDL_WINDOW_ALLOW_HIGHDPI on a
		 * platform with high-DPI support (Apple calls this "Retina"), and not disabled
		 * by the SDL_HINT_VIDEO_HIGHDPI_DISABLED hint.
		 */
		static void GetDrawableSize(Window& window, Point& size) { SDL_GL_GetDrawableSize(window.window, &size.w, &size.h); }
		/**
		 *  \brief Get the size of a window's underlying drawable in pixels (for use
		 *		 with glViewport).
		 *
		 *  \param window   Window from which the drawable size should be queried
		 *  \param w		Pointer to variable for storing the width in pixels, may be NULL
		 *  \param h		Pointer to variable for storing the height in pixels, may be NULL
		 *
		 * This may differ from SDL_GetWindowSize() if we're rendering to a high-DPI
		 * drawable, i.e. the window was created with SDL_WINDOW_ALLOW_HIGHDPI on a
		 * platform with high-DPI support (Apple calls this "Retina"), and not disabled
		 * by the SDL_HINT_VIDEO_HIGHDPI_DISABLED hint.
		 */
		static void GetDrawableSize(Window& window, int* w, int* h) { SDL_GL_GetDrawableSize(window.window, w, h); }

		/**
		 *  \brief Set the swap interval for the current OpenGL context.
		 *
		 *  \param interval 0 for immediate updates, 1 for updates synchronized with the
		 *				  vertical retrace. If the system supports it, you may
		 *				  specify -1 to allow late swaps to happen immediately
		 *				  instead of waiting for the next retrace.
		 *
		 *  \return 0 on success, or -1 if setting the swap interval is not supported.
		 */
		static int SetSwapInterval(int interval) { return SDL_GL_SetSwapInterval(interval); }

		/**
		 *  \brief Get the swap interval for the current OpenGL context.
		 *
		 *  \return 0 if there is no vertical retrace synchronization, 1 if the buffer
		 *		  swap is synchronized with the vertical retrace, and -1 if late
		 *		  swaps happen immediately instead of waiting for the next retrace.
		 *		  If the system can't determine the swap interval, or there isn't a
		 *		  valid current context, this will return 0 as a safe default.
		 */
		static int GetSwapInterval() { return SDL_GL_GetSwapInterval(); };

		// \brief Swap the OpenGL buffers for a window, if double-buffering is supported.
		static void SwapWindow(Window& window) { SDL_GL_SwapWindow(window.window); }
	}
}