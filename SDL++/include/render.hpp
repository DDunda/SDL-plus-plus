#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef SDL_render_hpp_
#define SDL_render_hpp_
#pragma once

#include <SDL_render.h>

#include "container.hpp"
#include "rect.hpp"
#include "video.hpp"

#include <memory>

namespace SDL
{
	struct Texture;

#if SDL_VERSION_ATLEAST(2, 0, 18)
	struct Vertex
	{
		union {
			struct {
				FPoint position;        /**< Vertex position, in Renderer coordinates  */
				Colour colour;          /**< Vertex color */
				FPoint tex_coord;       /**< Normalized texture coordinates, if needed */
			};
			SDL_Vertex vertex;
		};

		inline constexpr Vertex(FPoint position, Colour colour, FPoint tex_coord)
			: position(position), colour(colour), tex_coord(tex_coord) {}

		inline constexpr Vertex(SDL_Vertex vertex)
			: position(vertex.position), colour(vertex.color), tex_coord(vertex.tex_coord) {}

		inline constexpr Vertex(const Vertex& v)
			: position(v.position), colour(v.colour), tex_coord(v.tex_coord) {}

		inline constexpr Vertex(Vertex&& v) noexcept
			: position(v.position), colour(v.colour), tex_coord(v.tex_coord) {}

		inline constexpr Vertex& operator=(const Vertex& v)
		{
			position = v.position;
			colour = v.colour;
			tex_coord = v.tex_coord;
			return *this;
		}

		inline constexpr Vertex& operator=(Vertex&& v) noexcept
		{
			position = v.position;
			colour = v.colour;
			tex_coord = v.tex_coord;
			return *this;
		}
	};
#endif

	// A structure representing rendering state
	struct Renderer
	{
		// Flags used when creating a rendering context
		enum class Flags
		{
			SOFTWARE      = SDL_RENDERER_SOFTWARE,         // The renderer is a software fallback
			ACCELERATED   = SDL_RENDERER_ACCELERATED,      // The renderer uses hardware acceleration
			PRESENTVSYNC  = SDL_RENDERER_PRESENTVSYNC,     // Present is synchronized with the refresh rate
			TARGETTEXTURE = SDL_RENDERER_TARGETTEXTURE     // The renderer supports rendering to texture
		};

		// Information on the capabilities of a render driver or context.
		typedef SDL_RendererInfo Info;

#pragma region Safety

		// This is custom destructor for smart pointers that destroys SDL_Renderers through SDL
		inline static void DestroyRenderer(SDL_Renderer* renderer)
			{ SDL_DestroyRenderer(renderer); }

		// This is custom destructor for smart pointers that does not destroy the Renderer. This is for pointers you do not own
		inline static void DontDestroyRenderer(SDL_Renderer* renderer)
		{}

		// This creates a smart pointer to an SDL_Renderer with a custom destructor
		inline static std::shared_ptr<SDL_Renderer> MakeSharedPtr(SDL_Renderer* renderer)
			{ return std::shared_ptr<SDL_Renderer>(renderer, DestroyRenderer); }

		// This creates a Renderer from a SDL_Renderer pointer, taking ownership of the pointer
		inline static Renderer FromPtr(SDL_Renderer* renderer)
			{ return Renderer(MakeSharedPtr(renderer)); }

		// This creates a Renderer from a SDL_Renderer pointer, but does not take ownership of the pointer
		inline static Renderer FromUnownedPtr(SDL_Renderer* renderer)
			{ return Renderer(std::shared_ptr<SDL_Renderer>(renderer, DontDestroyRenderer)); }

		std::shared_ptr<SDL_Renderer> renderer = nullptr;

#pragma endregion 

#pragma region Constructors

		inline Renderer(std::shared_ptr<SDL_Renderer> _renderer)
			: renderer(_renderer) {}

		/**
		*  \brief    Create a 2D rendering context for a window.
		 *
		 *  \param    window: The window where rendering is displayed.
		 *  \param    flags:  ::SDL_RendererFlags.
		 *  \param    index:  The index of the rendering driver to initialize, or -1 to
		 *                    initialize the first one supporting the requested flags.
		 *
		 *  \return   A valid rendering context or NULL if there was an error.
		 */
		inline Renderer(Window& window, Uint32 flags, int index = -1)
			: Renderer(MakeSharedPtr(SDL_CreateRenderer(window.window.get(), index, flags))) {}

		/**
		 *  \brief    Create a 2D software rendering context for a surface.
		 *
		 *  \param    surface: The surface where rendering is done.
		 *
		 *  \return   A valid rendering context or NULL if there was an error.
		 */
		inline Renderer(Surface& surface)
			: Renderer(MakeSharedPtr(SDL_CreateSoftwareRenderer(surface.surface.get()))) {}

		// Get the renderer associated with a window.
		inline Renderer(Window& window)
			: Renderer(MakeSharedPtr(SDL_GetRenderer(window.window.get()))) {}

#if SDL_VERSION_ATLEAST(2, 0, 22)
		/**
		 * Get the window associated with a renderer.
		 *
		 * \param renderer the renderer to query
		 * \returns the window on success or invalid window on failure; call
		 *          SDL::GetError() for more information.
		 */
		inline Window GetWindow() { return Window::FromUnownedPtr(SDL_RenderGetWindow(renderer.get())); }
#endif

		inline Renderer()
			: Renderer(nullptr) {}
		inline Renderer(const Renderer& r)
			: Renderer(r.renderer) {}
		inline Renderer(Renderer&& r) noexcept
			{ std::swap(renderer, r.renderer); }
		inline Renderer& operator=(const Renderer& r)
		{
			renderer = r.renderer;
			return *this;
		}
		inline Renderer& operator=(Renderer&& r) noexcept
		{
			std::swap(renderer, r.renderer);
			return *this;
		}

#pragma endregion 

#pragma region Renderer State
		/**
		 *  \brief    Set the drawing scale for rendering on the current target.
		 *
		 *  \param    scale: The scaling factor
		 *
		 *  \details  The drawing coordinates are scaled by the x/y scaling factors
		 *            before they are used by the renderer.  This allows resolution
		 *            independent drawing with a single coordinate system.
		 *
		 *  \note     If this results in scaling or subpixel drawing by the
		 *            rendering backend, it will be handled using the appropriate
		 *            quality hints.  For best results use integer scaling factors.
		 */
		inline bool SetScale(const FPoint& scale)
			{ return SDL_RenderSetScale(renderer.get(), scale.x, scale.y) == 0; }

		/**
		 *  \brief    Set the drawing scale for rendering on the current target.
		 *
		 *  \param    scaleX: The horizontal scaling factor
		 *  \param    scaleY: The vertical scaling factor
		 *
		 *  \details  The drawing coordinates are scaled by the x/y scaling factors
		 *            before they are used by the renderer.  This allows resolution
		 *            independent drawing with a single coordinate system.
		 *
		 *  \note     If this results in scaling or subpixel drawing by the
		 *            rendering backend, it will be handled using the appropriate
		 *            quality hints.  For best results use integer scaling factors.
		 */
		inline bool SetScale(float scaleX, float scaleY)
			{ return SDL_RenderSetScale(renderer.get(), scaleX, scaleY) == 0; }

		/**
		 *  \brief    Get the drawing scale for the current target.
		 *
		 *  \return   The scaling factor
		 */
		inline FPoint GetScale()
		{
			FPoint returnVal;
			SDL_RenderGetScale(renderer.get(), &returnVal.x, &returnVal.y);
			return returnVal;
		}

		/**
		 *  \brief    Get the drawing scale for the current target.
		 *
		 *  \param    scale: A reference to be filled in with the scaling factor
		 */
		inline Renderer& GetScale(FPoint& scale)
			{ SDL_RenderGetScale(renderer.get(), &scale.x, &scale.y); return *this; }

		/**
		 *  \brief    Get the drawing scale for the current target.
		 *
		 *  \param    scaleX: A reference to be filled in with the horizontal scaling factor
		 *  \param    scaleY: A reference to be filled in with the vertical scaling factor\
		 */
		inline Renderer& GetScale(float& scaleX, float& scaleY)
			{ SDL_RenderGetScale(renderer.get(), &scaleX, &scaleY); return *this; }

#if SDL_VERSION_ATLEAST(2, 0, 18)
		/**
		 * Get logical coordinates of point in renderer when given real coordinates of
		 * point in window.
		 *
		 * Logical coordinates will differ from real coordinates when render is scaled
		 * and logical renderer size set
		 *
		 * \param windowX the real X coordinate in the window
		 * \param windowY the real Y coordinate in the window
		 * \param logicalX the pointer filled with the logical x coordinate
		 * \param logicalY the pointer filled with the logical y coordinate
		 */
		inline Renderer& WindowToLogical(int windowX, int windowY, float* logicalX, float* logicalY)
			{ SDL_RenderWindowToLogical(renderer.get(), windowX, windowY, logicalX, logicalY); return *this; }

		/**
		 * Get logical coordinates of point in renderer when given real coordinates of
		 * point in window.
		 *
		 * Logical coordinates will differ from real coordinates when render is scaled
		 * and logical renderer size set
		 *
		 * \param windowX the real X coordinate in the window
		 * \param windowY the real Y coordinate in the window
		 * \param logicalX the reference filled with the logical x coordinate
		 * \param logicalY the reference filled with the logical y coordinate
		 */
		inline Renderer& WindowToLogical(int windowX, int windowY, float& logicalX, float& logicalY)
			{ SDL_RenderWindowToLogical(renderer.get(), windowX, windowY, &logicalX, &logicalY); return *this; }

		/**
		 * Get logical coordinates of point in renderer when given real coordinates of
		 * point in window.
		 *
		 * Logical coordinates will differ from real coordinates when render is scaled
		 * and logical renderer size set
		 *
		 * \param window the real coordinates in the window
		 * \param logical the reference filled with the logical coordinates
		 */
		inline Renderer& WindowToLogical(const Point& window, FPoint& logical)
			{ SDL_RenderWindowToLogical(renderer.get(), window.x, window.y, &logical.x, &logical.y); return *this; }

		/**
		 * Get logical coordinates of point in renderer when given real coordinates of
		 * point in window.
		 *
		 * Logical coordinates will differ from real coordinates when render is scaled
		 * and logical renderer size set
		 *
		 * \param window the real coordinates in the window
		 * \return an FPoint with the logical coordinates
		 */
		inline FPoint WindowToLogical(const Point& window)
		{
			FPoint logical;
			SDL_RenderWindowToLogical(renderer.get(), window.x, window.y, &logical.x, &logical.y);
			return logical;
		}

		/**
		 * Get real coordinates of point in window when given logical coordinates of
		 * point in renderer.
		 *
		 * Logical coordinates will differ from real coordinates when render is scaled
		 * and logical renderer size set
		 *
		 * \param logicalX the logical x coordinate
		 * \param logicalY the logical y coordinate
		 * \param windowX the pointer filled with the real X coordinate in the window
		 * \param windowY the pointer filled with the real Y coordinate in the window
		 */
		inline Renderer& LogicalToWindow(float logicalX, float logicalY, int* windowX, int* windowY)
			{ SDL_RenderLogicalToWindow(renderer.get(), logicalX, logicalY, windowX, windowY); return *this; }

		/**
		 * Get real coordinates of point in window when given logical coordinates of
		 * point in renderer.
		 *
		 * Logical coordinates will differ from real coordinates when render is scaled
		 * and logical renderer size set
		 *
		 * \param logicalX the logical x coordinate
		 * \param logicalY the logical y coordinate
		 * \param windowX the reference filled with the real X coordinate in the window
		 * \param windowY the reference filled with the real Y coordinate in the window
		 */
		inline Renderer& LogicalToWindow(float logicalX, float logicalY, int& windowX, int& windowY)
			{ SDL_RenderLogicalToWindow(renderer.get(), logicalX, logicalY, &windowX, &windowY); return *this; }

		/**
		 * Get real coordinates of point in window when given logical coordinates of
		 * point in renderer.
		 *
		 * Logical coordinates will differ from real coordinates when render is scaled
		 * and logical renderer size set
		 *
		 * \param logical the logical coordinates
		 * \param window the reference filled with the real coordinates in the window
		 */
		inline Renderer& LogicalToWindow(const FPoint& logical, Point& window)
			{ SDL_RenderLogicalToWindow(renderer.get(), logical.x, logical.y, &window.x, &window.y); return *this; }

		/**
		 * Get real coordinates of point in window when given logical coordinates of
		 * point in renderer.
		 *
		 * Logical coordinates will differ from real coordinates when render is scaled
		 * and logical renderer size set
		 *
		 * \param logical the logical coordinates
		 * \return a Point with the real coordinates in the window
		 */
		inline Point LogicalToWindow(const FPoint& logical)
		{
			Point window;
			SDL_RenderLogicalToWindow(renderer.get(), logical.x, logical.y, &window.x, &window.y);
			return window;
		}
#endif

		/**
		 *  \brief    Set the colour used for drawing operations (Rect, Line and Clear).
		 *
		 *  \param    colour: The colour value used to draw on the rendering target.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool SetDrawColour(const Colour& colour)
			{ return SDL_SetRenderDrawColor(renderer.get(), colour.r, colour.g, colour.b, colour.a) == 0; }

		/**
		 *  \brief    Set the colour used for drawing operations (Rect, Line and Clear).
		 *
		 *  \param    r: The red value used to draw on the rendering target.
		 *  \param    g: The green value used to draw on the rendering target.
		 *  \param    b: The blue value used to draw on the rendering target.
		 *  \param    a: The alpha value used to draw on the rendering target, usually
		 *               ::SDL_ALPHA_OPAQUE (255).
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool SetDrawColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
			{ return SDL_SetRenderDrawColor(renderer.get(), r, g, b, a) == 0; }

		/**
		 *  \brief    Get the colour used for drawing operations (Rect, Line and Clear).
		 *
		 *  \param    colour: A reference to be filled with the colour used to draw on the rendering target.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool GetDrawColour(Colour& colour)
			{ return SDL_GetRenderDrawColor(renderer.get(), &colour.r, &colour.g, &colour.b, &colour.a) == 0; }

		/**
		 *  \brief    Get the colour used for drawing operations (Rect, Line and Clear).
		 *
		 *  \param    r: A reference to be filled with the red value used to draw on the rendering target.
		 *  \param    g: A reference to be filled with the green value used to draw on the rendering target.
		 *  \param    b: A reference to be filled with the blue value used to draw on the rendering target.
		 *  \param    a: A reference to be filled with the alpha value used to draw on the rendering target,
		 *               usually ::SDL_ALPHA_OPAQUE (255).
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool GetDrawColour(Uint8& r, Uint8& g, Uint8& b, Uint8& a)
			{ return SDL_GetRenderDrawColor(renderer.get(), &r, &g, &b, &a) == 0; }

		/**
		 *  \brief    Set the blend mode used for drawing operations (Fill and Line).
		 *
		 *  \param    blendMode SDL::BlendMode to use for blending.
		 *
		 *  \note     If the blend mode is not supported, the closest supported mode is
		 *            chosen.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool SetDrawBlendMode(const BlendMode& blendMode)
			{ return SDL_SetRenderDrawBlendMode(renderer.get(), (SDL_BlendMode)blendMode) == 0; }

		/**
		 *  \brief    Get the blend mode used for drawing operations.
		 *
		 *  \param    blendMode A reference to be filled in with the current blend mode.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool GetDrawBlendMode(BlendMode& blendMode)
			{ return SDL_GetRenderDrawBlendMode(renderer.get(), (SDL_BlendMode*)&blendMode) == 0; }

		/**
		 *  \brief    Clear the current rendering target with the drawing colour
		 *
		 *            This function clears the entire rendering target, ignoring the viewport and
		 *            the clip rectangle.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool Clear()
			{ return SDL_RenderClear(renderer.get()) == 0; }

#if SDL_VERSION_ATLEAST(2, 0, 10)
		/**
		 *  \brief    Force the rendering context to flush any pending commands to the
		 *            underlying rendering API.
		 *
		 *  \note     You do not need to (and in fact, shouldn't) call this function unless
		 *            you are planning to call into OpenGL/Direct3D/Metal/whatever directly
		 *            in addition to using an Renderer.
		 *
		 *  \details  This is for a very-specific case: if you are using SDL's render API,
		 *            you asked for a specific renderer backend (OpenGL, Direct3D, etc),
		 *            you set SDL_HINT_RENDER_BATCHING to "1", and you plan to make
		 *            OpenGL/D3D/whatever calls in addition to SDL render API calls. If all of
		 *            this applies, you should call Flush() between calls to SDL's render API
		 *            and the low-level API you're using in cooperation.
		 *
		 *            In all other cases, you can ignore this function. This is only here to
		 *            get maximum performance out of a specific situation. In all other cases,
		 *            SDL will do the right thing, perhaps at a performance loss.
		 *
		 *            This function is first available in SDL 2.0.10, and is not needed in
		 *            2.0.9 and earlier, as earlier versions did not queue rendering commands
		 *            at all, instead flushing them to the OS immediately.
		 */
		inline bool Flush()
			{ return SDL_RenderFlush(renderer.get()) == 0; }
#endif

		// Update the screen with rendering performed.
		inline Renderer& Present()
			{ SDL_RenderPresent(renderer.get()); return *this; }

#pragma endregion 

#pragma region Primitive Drawing
		
		/**
		 *  \brief    Draw a point on the current rendering target.
		 *
		 *  \param    x: The x coordinate of the point.
		 *  \param    y: The y coordinate of the point.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawPoint(int x, int y)
			{ return SDL_RenderDrawPoint(renderer.get(), x, y) == 0; }

		/**
		 *  \brief    Draw a point on the current rendering target.
		 *
		 *  \param    point: The coordinates of the point.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawPoint(const Point& point)
			{ return SDL_RenderDrawPoint(renderer.get(), point.x, point.y) == 0; }

		/**
		 *  \brief    Draw multiple points on the current rendering target.
		 *
		 *  \param    points:   The points to draw
		 *  \param    count:    The number of points to draw
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawPoints(const Point* points, int count)
			{ return SDL_RenderDrawPoints(renderer.get(), (const SDL_Point*)points, count) == 0; }

		/**
		 *  \brief    Draw multiple points on the current rendering target.
		 *
		 *  \param    points: The points to draw
		 *
		 *  \return   true on success, or false on error
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<Point, T>::is_continuous_container>>
		inline bool DrawPoints(const T& points)
			{ return SDL_RenderDrawPoints(renderer.get(), (const SDL_Point*)points.data(), (int)points.size()) == 0; }

		/**
		 *  \brief    Draw multiple points on the current rendering target.
		 *
		 *  \param    points: The points to draw
		 *
		 *  \return   true on success, or false on error
		 */
		template <const int size>
		inline bool DrawPoints(const Point (&points)[size])

			{ return SDL_RenderDrawPoints(renderer.get(), (const SDL_Point*)points, size) == 0; }

		/**
		 *  \brief    Draw a line on the current rendering target.
		 *
		 *  \param    x1: The x coordinate of the start point.
		 *  \param    y1: The y coordinate of the start point.
		 *  \param    x2: The x coordinate of the end point.
		 *  \param    y2: The y coordinate of the end point.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawLine(int x1, int y1, int x2, int y2)
			{ return SDL_RenderDrawLine(renderer.get(), x1, y1, x2, y2) == 0; }

		/**
		 *  \brief    Draw a line on the current rendering target.
		 *
		 *  \param    a: The coordinates of the start point.
		 *  \param    b: The coordinates of the end point.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawLine(const Point& a, const Point& b)
			{ return SDL_RenderDrawLine(renderer.get(), a.x, a.y, b.x, b.y) == 0; }

		/**
		 *  \brief    Draw a series of connected lines on the current rendering target.
		 *
		 *  \param    points: The points along the lines
		 *  \param    count:  The number of points, drawing count-1 lines
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawLines(const Point* points, int count)
			{ return SDL_RenderDrawLines(renderer.get(), (const  SDL_Point*)points, count) == 0; }

		/**
		 *  \brief    Draw a series of connected lines on the current rendering target.
		 *
		 *  \param    points: The points along the lines
		 *
		 *  \return   true on success, or false on error
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<Point, T>::is_continuous_container>>
		inline bool DrawLines(const T& points)
			{ return SDL_RenderDrawLines(renderer.get(), (const  SDL_Point*)points.data(), (int)points.size()) == 0; }

		/**
		 *  \brief    Draw a series of connected lines on the current rendering target.
		 *
		 *  \param    points: The points along the lines
		 *
		 *  \return   true on success, or false on error
		 */
		template <const int size>
		inline bool DrawLines(const Point(&points)[size])
			{ return SDL_RenderDrawLines(renderer.get(), (const  SDL_Point*)points, size) == 0; }

#if SDL_VERSION_ATLEAST(2, 0, 10)
		/**
		 *  \brief    Draw a point on the current rendering target.
		 *
		 *  \param    x: The x coordinate of the point.
		 *  \param    y: The y coordinate of the point.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawPointF(float x, float y)
			{ return SDL_RenderDrawPointF(renderer.get(), x, y) == 0; }

		/**
		 *  \brief    Draw a point on the current rendering target.
		 *
		 *  \param    point: The coordinates of the point.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawPointF(const FPoint& point)
			{ return SDL_RenderDrawPointF(renderer.get(), point.x, point.y) == 0; }

		/**
		 *  \brief    Draw multiple points on the current rendering target.
		 *
		 *  \param    points: The points to draw
		 *  \param    count:  The number of points to draw
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawPointsF(const FPoint* points, int count)
			{ return SDL_RenderDrawPointsF(renderer.get(), (const SDL_FPoint*)points, count) == 0; }

		/**
		 *  \brief    Draw multiple points on the current rendering target.
		 *
		 *  \param    points: The points to draw
		 *
		 *  \return   true on success, or false on error
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<FPoint, T>::is_continuous_container>>
		inline bool DrawPointsF(const T& points)
			{ return SDL_RenderDrawPointsF(renderer.get(), (const SDL_FPoint*)points.data(), (int)points.size()) == 0; }

		/**
		 *  \brief    Draw multiple points on the current rendering target.
		 *
		 *  \param    points: The points to draw
		 *
		 *  \return   true on success, or false on error
		 */
		template <const int size>
		inline bool DrawPointsF(const FPoint (&points)[size])
			{ return SDL_RenderDrawPointsF(renderer.get(), (const SDL_FPoint*)points, size) == 0; }

		/**
		 *  \brief    Draw a line on the current rendering target.
		 *
		 *  \param    x1: The x coordinate of the start point.
		 *  \param    y1: The y coordinate of the start point.
		 *  \param    x2: The x coordinate of the end point.
		 *  \param    y2: The y coordinate of the end point.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawLineF(float x1, float y1, float x2, float y2)
			{ return SDL_RenderDrawLineF(renderer.get(), x1, y1, x2, y2) == 0; }

		/**
		 *  \brief    Draw a line on the current rendering target.
		 *
		 *  \param    a: The coordinates of the start point.
		 *  \param    b: The coordinates of the end point.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawLineF(const FPoint& a, const FPoint& b)
			{ return SDL_RenderDrawLineF(renderer.get(), a.x, a.y, b.x, b.y) == 0; }

		/**
		 *  \brief    Draw a series of connected lines on the current rendering target.
		 *
		 *  \param    points: The points along the lines
		 *  \param    count:  The number of points, drawing count-1 lines
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawLinesF(const FPoint* points, int count) { return SDL_RenderDrawLinesF(renderer.get(), (const SDL_FPoint*)points, count) == 0; }

		/**
		 *  \brief    Draw a series of connected lines on the current rendering target.
		 *
		 *  \param    points: The points along the lines
		 *
		 *  \return   true on success, or false on error
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<FPoint, T>::is_continuous_container>>
		inline bool DrawLinesF(const T& points)
			{ return SDL_RenderDrawLinesF(renderer.get(), (const SDL_FPoint*)points.data(), (int)points.size()) == 0; }

		/**
		 *  \brief    Draw a series of connected lines on the current rendering target.
		 *
		 *  \param    points: The points along the lines
		 *
		 *  \return   true on success, or false on error
		 */
		template <const int size>
		inline bool DrawLinesF(const FPoint(&points)[size])
			{ return SDL_RenderDrawLinesF(renderer.get(), (const SDL_FPoint*)points, size) == 0; }

		/**
		*  \brief    Draw a rectangle on the current rendering target.
		 *
		 *  \param    rect: A reference to the destination rectangle.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawRectF(const FRect& rect)
			{ return SDL_RenderDrawRectF(renderer.get(), &rect.rect) == 0; }

		/**
		 *  \brief    Draw a rectangle on the current rendering target.
		 *
		 *  \param    rect: A pointer to the destination rectangle, or NULL to outline the entire rendering target.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawRectF(const FRect* rect)
			{ return SDL_RenderDrawRectF(renderer.get(), (const SDL_FRect*)rect) == 0; }

		/**
		 *  \brief    Draw some number of rectangles on the current rendering target.
		 *
		 *  \param    rects: A pointer to an array of destination rectangles.
		 *  \param    count: The number of rectangles.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawRectsF(const FRect* rects, int count)
			{ return SDL_RenderDrawRectsF(renderer.get(), (const SDL_FRect*)rects, count) == 0; }

		/**
		 *  \brief    Draw some number of rectangles on the current rendering target.
		 *
		 *  \param    rects: A reference to a container of destination rectangles.
		 *
		 *  \return   true on success, or false on error
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<FRect, T>::is_continuous_container>>
		inline bool DrawRectsF(const T& rects)
			{ return SDL_RenderDrawRectsF(renderer.get(), (const SDL_FRect*)rects.data(), (int)rects.size()) == 0; }

		/**
		 *  \brief    Draw some number of rectangles on the current rendering target.
		 *
		 *  \param    rects: A reference to a container of destination rectangles.
		 *
		 *  \return   true on success, or false on error
		 */
		template <const int size>
		inline bool DrawRectsF(const Rect (&rects)[size])
			{ return SDL_RenderDrawRectsF(renderer.get(), (const SDL_FRect*)rects, size) == 0; }

		/**
		 *  \brief    Fill the entire current rendering target with the drawing colour.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool FillF()
			{ return SDL_RenderFillRectF(renderer.get(), NULL) == 0; }

		/**
		 *  \brief    Fill a rectangle on the current rendering target with the drawing colour.
		 *
		 *  \param    rect: A reference to the destination rectangle.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool FillRectF(const FRect & rect)
			{ return SDL_RenderFillRectF(renderer.get(), &rect.rect) == 0; }

		/**
		 *  \brief    Fill a rectangle on the current rendering target with the drawing colour.
		 *
		 *  \param    rect: A pointer to the destination rectangle, or NULL for the entire
		 *                  rendering target.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool FillRectF(const FRect* rect)
			{ return SDL_RenderFillRectF(renderer.get(), (const SDL_FRect*)rect) == 0; }

		/**
		 *  \brief    Fill some number of rectangles on the current rendering target with the drawing colour.
		 *
		 *  \param    rects: A pointer to an array of destination rectangles.
		 *  \param    count: The number of rectangles.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool FillRectsF(const FRect* rects, int count)
			{ return SDL_RenderFillRectsF(renderer.get(), (const SDL_FRect*)rects, count) == 0; }

		/**
		 *  \brief    Fill some number of rectangles on the current rendering target with the drawing colour.
		 *
		 *  \param    rects: A reference to a container of destination rectangles.
		 *
		 *  \return   true on success, or false on error
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<FRect, T>::is_continuous_container>>
		inline bool FillRectsF(const T& rects)
			{ return SDL_RenderFillRectsF(renderer.get(), (const SDL_FRect*)rects.data(), (int)rects.size()) == 0; }

		/**
		 *  \brief    Fill some number of rectangles on the current rendering target with the drawing colour.
		 *
		 *  \param    rects: A reference to a container of destination rectangles.
		 *
		 *  \return   true on success, or false on error
		 */
		template <const int size>
		inline bool FillRectsF(const Rect(&rects)[size])
			{ return SDL_RenderFillRectsF(renderer.get(), (const SDL_FRect*)rects, size) == 0; }
#endif

		/**
		 *  \brief    Draws a rectangle outlining current rendering target.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawOutline()
			{ return SDL_RenderDrawRect(renderer.get(), NULL) == 0; }

		/**
		 *  \brief    Draws a rectangle outlining current rendering target.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawOutlineF()
			{ return SDL_RenderDrawRectF(renderer.get(), NULL) == 0; }

		/**
		 *  \brief    Draw a rectangle on the current rendering target.
		 *
		 *  \param    rect: A pointer to the destination rectangle, or NULL to outline the entire rendering target.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawRect(const Rect* rect)
			{ return SDL_RenderDrawRect(renderer.get(), (const  SDL_Rect*)rect) == 0; }

		/**
		 *  \brief    Draw a rectangle on the current rendering target.
		 *
		 *  \param    rect: A reference to the destination rectangle.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawRect(const Rect& rect)
			{ return SDL_RenderDrawRect(renderer.get(), &rect.rect) == 0; }

		inline bool DrawRectEx(const Rect& rect, const Point& center, float angle = 0.0)
		{
			const float rotX = cosf(angle);
			const float rotY = sinf(angle);

			const FPoint corners[5]
			{
				rect.topLeft    ().rotateAround(center, rotX, rotY),
				rect.topRight   ().rotateAround(center, rotX, rotY),
				rect.bottomRight().rotateAround(center, rotX, rotY),
				rect.bottomLeft ().rotateAround(center, rotX, rotY),
				rect.topLeft    ().rotateAround(center, rotX, rotY)
			};

			return DrawLinesF(corners);
		}

		inline bool DrawRectEx(const Rect& rect, float angle = 0.0)
		{
			const FPoint center = rect.middle();
			const FPoint corner1 = FPoint(-rect.w / 2.f, -rect.h / 2.f).rotate(angle);

			const FPoint corners[5]
			{
				corner1                        + center,
				FPoint( corner1.y, -corner1.x) + center,
				FPoint(-corner1.x, -corner1.y) + center,
				FPoint(-corner1.y,  corner1.x) + center,
				corner1                        + center
			};

			return DrawLinesF(corners);
		}

		inline bool DrawRectExF(const FRect& rect, const FPoint& center, float angle = 0.0)
		{
			const float rotX = cosf(angle);
			const float rotY = sinf(angle);

			const FPoint corners[5]
			{
				rect.topLeft    ().rotateAround(center, rotX, rotY),
				rect.topRight   ().rotateAround(center, rotX, rotY),
				rect.bottomRight().rotateAround(center, rotX, rotY),
				rect.bottomLeft ().rotateAround(center, rotX, rotY),
				rect.topLeft    ().rotateAround(center, rotX, rotY)
			};

			return DrawLinesF(corners);
		}

		inline bool DrawRectExF(const FRect& rect, float angle = 0.0)
		{
			const FPoint center = rect.middle();
			const FPoint corner1 = FPoint(-rect.w / 2.f, -rect.h / 2.f).rotate(angle);

			const FPoint corners[5]
			{
				corner1                        + center,
				FPoint( corner1.y, -corner1.x) + center,
				FPoint(-corner1.x, -corner1.y) + center,
				FPoint(-corner1.y,  corner1.x) + center,
				corner1                        + center
			};

			return DrawLinesF(corners);
		}

		/**
		 *  \brief    Draw some number of rectangles on the current rendering target.
		 *
		 *  \param    rects: A pointer to an array of destination rectangles.
		 *  \param    count: The number of rectangles.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DrawRects(const Rect* rects, int count)
			{ return SDL_RenderDrawRects(renderer.get(), (const  SDL_Rect*)rects, count) == 0; }

		/**
		 *  \brief    Draw some number of rectangles on the current rendering target.
		 *
		 *  \param    rects: A reference to a container of destination rectangles.
		 *
		 *  \return   true on success, or false on error
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<Rect, T>::is_continuous_container>>
		inline bool DrawRects(const T& rects)
			{ return SDL_RenderDrawRects(renderer.get(), (const  SDL_Rect*)rects.data(), (int)rects.size()) == 0; }
		
		/**
		 *  \brief    Draw some number of rectangles on the current rendering target.
		 *
		 *  \param    rects: A reference to a container of destination rectangles.
		 *
		 *  \return   true on success, or false on error
		 */
		template <const int size>
		inline bool DrawRects(const Rect (&rects)[size])
			{ return SDL_RenderDrawRects(renderer.get(), (const  SDL_Rect*)rects, size) == 0; }


		/**
		*  \brief     Fill the entire current rendering target with the drawing colour.
		*
		*  \return    true on success, or false on error
		*/
		inline bool Fill()
			{ return SDL_RenderFillRect(renderer.get(), NULL) == 0; }

		/**
		 *  \brief    Fill a rectangle on the current rendering target with the drawing colour.
		 *
		 *  \param    rect: A pointer to the destination rectangle.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool FillRect(const Rect& rect)
			{ return SDL_RenderFillRect(renderer.get(), &rect.rect) == 0; }

		/**
		 *  \brief    Fill a rectangle on the current rendering target with the drawing colour.
		 *
		 *  \param    rect: A pointer to the destination rectangle, or NULL for the entire
		 *                  rendering target.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool FillRect(const Rect* rect)
			{ return SDL_RenderFillRect(renderer.get(), (const  SDL_Rect*)rect) == 0; }

		/**
		 *  \brief    Fill some number of rectangles on the current rendering target with the drawing colour.
		 *
		 *  \param    rects: A pointer to an array of destination rectangles.
		 *  \param    count: The number of rectangles.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool FillRects(const Rect* rects, int count)
			{ return SDL_RenderFillRects(renderer.get(), (const  SDL_Rect*)rects, count) == 0; }

		/**
		 *  \brief    Fill some number of rectangles on the current rendering target with the drawing colour.
		 *
		 *  \param    rects: A reference to a container of destination rectangles.
		 *
		 *  \return   true on success, or false on error
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<Rect, T>::is_continuous_container>>
		inline bool FillRects(const T& rects)
			{ return SDL_RenderFillRects(renderer.get(), (const  SDL_Rect*)rects.data(), (int)rects.size()) == 0; }

		/**
		 *  \brief    Fill some number of rectangles on the current rendering target with the drawing colour.
		 *
		 *  \param    rects: A reference to a container of destination rectangles.
		 *
		 *  \return   true on success, or false on error
		 */
		template <const int size>
		inline bool FillRects(const Rect (&rects)[size])
			{ return SDL_RenderFillRects(renderer.get(), (const  SDL_Rect*)rects, size) == 0; }

#pragma endregion

#pragma region Render Geometry
#if SDL_VERSION_ATLEAST(2, 0, 18)

		/**
		 * Render a list of triangles, and optionally indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param num_vertices Number of vertices.
		 * \param indices (optional) An array of integer indices into the 'vertices'
		 *                array, if NULL all vertices will be rendered in sequential
		 *                order.
		 * \param num_indices Number of indices.
		 * \return true on success, or false if the operation is not supported
		 */
		inline bool RenderGeometry(const Vertex* vertices, int num_vertices, const int* indices, int num_indices)
			{ return SDL_RenderGeometry(renderer.get(), NULL, (const SDL_Vertex*)vertices, num_vertices, indices, num_indices) == 0; }

		/**
		 * Render a list of triangles, and optionally indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param indices (optional) An array of integer indices into the 'vertices'
		 *                array, if NULL all vertices will be rendered in sequential
		 *                order.
		 * \param num_indices Number of indices.
		 * \return true on success, or false if the operation is not supported
		 */
		template <const int num_vertices>
		inline bool RenderGeometry(const Vertex(&vertices)[num_vertices], const int* indices, int num_indices)
			{ return SDL_RenderGeometry(renderer.get(), NULL, (const SDL_Vertex*)vertices, num_vertices, indices, num_indices) == 0; }

		/**
		 * Render a list of triangles, and optionally indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param indices (optional) An array of integer indices into the 'vertices'
		 *                array, if NULL all vertices will be rendered in sequential
		 *                order.
		 * \param num_indices Number of indices.
		 * \return true on success, or false if the operation is not supported
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<Vertex, T>::is_continuous_container>>
		inline bool RenderGeometry(const T& vertices, const int* indices, int num_indices)
			{ return SDL_RenderGeometry(renderer.get(), NULL, (const SDL_Vertex*)vertices.data(), (int)vertices.size(), indices, num_indices) == 0; }

		/**
		 * Render a list of triangles and indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param num_vertices Number of vertices.
		 * \param indices An array of integer indices into the 'vertices' array.
		 * \return true on success, or false if the operation is not supported
		 */
		template <const int num_indices>
		inline bool RenderGeometry(const Vertex* vertices, int num_vertices, const int(&indices)[num_indices])
			{ return SDL_RenderGeometry(renderer.get(), NULL, (const SDL_Vertex*)vertices, num_vertices, indices, num_indices) == 0; }

		/**
		 * Render a list of triangles and indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param indices An array of integer indices into the 'vertices' array.
		 * \return true on success, or false if the operation is not supported
		 */
		template <const int num_vertices, const int num_indices>
		inline bool RenderGeometry(const Vertex(&vertices)[num_vertices], const int(&indices)[num_indices])
			{ return SDL_RenderGeometry(renderer.get(), NULL, (const SDL_Vertex*)vertices, num_vertices, indices, num_indices) == 0; }

		/**
		 * Render a list of triangles and indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param indices An array of integer indices into the 'vertices' array.
		 * \return true on success, or false if the operation is not supported
		 */
		template <typename T, const int num_indices, typename = typename std::enable_if_t<ContinuousContainer_traits<Vertex, T>::is_continuous_container>>
		inline bool RenderGeometry(const T& vertices, const int(&indices)[num_indices])
			{ return SDL_RenderGeometry(renderer.get(), NULL, (const SDL_Vertex*)vertices.data(), (int)vertices.size(), indices, num_indices) == 0; }

		/**
		 * Render a list of triangles and indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param num_vertices Number of vertices.
		 * \param indices A container of integer indices into the 'vertices' array.
		 * \return true on success, or false if the operation is not supported
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<int, T>::is_continuous_container>>
		inline bool RenderGeometry(const Vertex* vertices, int num_vertices, const T& indices)
			{ return SDL_RenderGeometry(renderer.get(), NULL, (const SDL_Vertex*)vertices, num_vertices, indices.data(), (int)indices.size()) == 0; }

		/**
		 * Render a list of triangles and indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param indices A container of integer indices into the 'vertices' array.
		 * \return true on success, or false if the operation is not supported
		 */
		template <const int num_vertices, typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<int, T>::is_continuous_container>>
		inline bool RenderGeometry(const Vertex(&vertices)[num_vertices], const T& indices)
			{ return SDL_RenderGeometry(renderer.get(), NULL, (const SDL_Vertex*)vertices, num_vertices, indices.data(), (int)indices.size()) == 0; }

		/**
		 * Render a list of triangles and indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param indices A container of integer indices into the 'vertices' array.
		 * \return true on success, or false if the operation is not supported
		 */
		template <
			typename T1,
			typename T2,
			typename = typename std::enable_if_t<
			ContinuousContainer_traits<Vertex, T1>::is_continuous_container &&
			ContinuousContainer_traits<int, T2>::is_continuous_container
			>>
		inline bool RenderGeometry(const T1& vertices, const T2& indices)
			{ return SDL_RenderGeometry(renderer.get(), NULL, (const SDL_Vertex*)vertices.data(), (int)vertices.size(), indices.data(), (int)indices.size()) == 0; }



		/**
		 * Render a list of triangles, and optionally indices into the
		 * vertex arrays. Color and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param xy Vertex positions
		 * \param xy_stride Byte size to move from one element to the next element
		 * \param colour Vertex colours (as Colour)
		 * \param colour_stride Byte size to move from one element to the next element
		 * \param uv Vertex normalized texture coordinates
		 * \param uv_stride Byte size to move from one element to the next element
		 * \param num_vertices Number of vertices.
		 * \param indices (optional) An array of indices into the 'vertices' arrays,
		 *                if NULL all vertices will be rendered in sequential order.
		 * \param num_indices Number of indices.
		 * \param size_indices Index size: 1 (byte), 2 (short), 4 (int)
		 * \return true on success, or false if the operation is not supported
		 */
		inline bool RenderGeometryRaw(
			const float* xy, int xy_stride,
			const Colour* colour, int colour_stride,
			const float* uv, int uv_stride,
			int num_vertices,
			const void* indices, int num_indices,
			int size_indices)
		{
			return SDL_RenderGeometryRaw(
				renderer.get(), NULL,
				xy, xy_stride,
				colour, colour_stride,
				uv, uv_stride,
				num_vertices,
				indices, num_indices,
				size_indices
			) == 0;
		}

		/**
		 * Render a list of triangles and indices into the
		 * vertex arrays. Color and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param xy Vertex positions
		 * \param xy_stride Byte size to move from one element to the next element
		 * \param colour Vertex colours (as Colour)
		 * \param colour_stride Byte size to move from one element to the next element
		 * \param uv Vertex normalized texture coordinates
		 * \param uv_stride Byte size to move from one element to the next element
		 * \param num_vertices Number of vertices.
		 * \param indices An array of indices into the 'vertices' arrays.
		 * \return true on success, or false if the operation is not supported
		 */
		template <
			typename t,
			const int num_indices,
			typename = typename std::enable_if_t<
				sizeof(t) == 1 || sizeof(t) == 2 || sizeof(t) == 4
			>>
			inline bool RenderGeometryRaw(
				const float* xy, int xy_stride,
				const Colour* colour, int colour_stride,
				const float* uv, int uv_stride,
				int num_vertices,
				const t(&indices)[num_indices])
		{
			return SDL_RenderGeometryRaw(
				renderer.get(), NULL,
				xy, xy_stride,
				colour, colour_stride,
				uv, uv_stride,
				num_vertices,
				indices, num_indices,
				sizeof(t)
			) == 0;
		}

		/**
		 * Render a list of triangles and indices into the
		 * vertex arrays. Color and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param xy Vertex positions
		 * \param xy_stride Byte size to move from one element to the next element
		 * \param colour Vertex colours (as Colour)
		 * \param colour_stride Byte size to move from one element to the next element
		 * \param uv Vertex normalized texture coordinates
		 * \param uv_stride Byte size to move from one element to the next element
		 * \param num_vertices Number of vertices.
		 * \param indices An container of indices into the 'vertices' arrays.
		 * \return true on success, or false if the operation is not supported
		 */
		template <
			typename t,
			typename T,
			typename = typename std::enable_if_t<
			ContinuousContainer_traits<t, T>::is_continuous_container &&
			(sizeof(t) == 1 || sizeof(t) == 2 || sizeof(t) == 4)
			>>
			inline bool RenderGeometryRaw(
				const float* xy, int xy_stride,
				const Colour* colour, int colour_stride,
				const float* uv, int uv_stride,
				int num_vertices,
				const T& indices)
		{
			return SDL_RenderGeometryRaw(
				renderer.get(), NULL,
				xy, xy_stride,
				colour, colour_stride,
				uv, uv_stride,
				num_vertices,
				indices.data(), (int)indices.size(),
				sizeof(t)
			) == 0;
		}

		/**
		 * Render a list of triangles, and optionally indices into the
		 * vertex arrays. Color and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param xy Vertex positions
		 * \param xy_stride Byte size to move from one element to the next element
		 * \param colour Vertex colours (as Colour)
		 * \param colour_stride Byte size to move from one element to the next element
		 * \param uv Vertex normalized texture coordinates
		 * \param uv_stride Byte size to move from one element to the next element
		 * \param num_vertices Number of vertices.
		 * \return true on success, or false if the operation is not supported
		 */
		inline bool RenderGeometryRaw(
			const float* xy, int xy_stride,
			const Colour* colour, int colour_stride,
			const float* uv, int uv_stride,
			int num_vertices)
		{
			return SDL_RenderGeometryRaw(
				renderer.get(), NULL,
				xy, xy_stride,
				colour, colour_stride,
				uv, uv_stride,
				num_vertices,
				NULL, 0, 0
			) == 0;
		}

#endif
#pragma endregion

#pragma region Renderer Information

		/**
		 *  \brief    Get the number of 2D rendering drivers available for the current
		 *            display.
		 *
		 *  \details  A render driver is a set of code that handles rendering and texture
		 *            management on a particular display.  Normally there is only one, but
		 *            some drivers may have several available with different capabilities.
		 */
		inline static int GetNumDrivers()
			{ return SDL_GetNumRenderDrivers(); }

		/**
		 *  \brief    Get information about a specific 2D rendering driver for the current
		 *            display.
		 *
		 *  \param    info: A reference to an Info struct to be filled with
		 *                  information on the rendering driver.
		 *
		 *  \return   true on success, false if the index was out of range.
		 */
		inline bool GetDriverInfo(int index, Info& info)
			{ return SDL_GetRenderDriverInfo(index, &info) == 0; }

		// Get information about a rendering context.
		inline bool GetInfo(Info& info)
			{ return SDL_GetRendererInfo(renderer.get(), &info) == 0; }

		// Get the output size in pixels of a rendering context.
		inline bool GetOutputSize(Point& size)
			{ return SDL_GetRendererOutputSize(renderer.get(), &size.w, &size.h) == 0; }

		// Get the output size in pixels of a rendering context.
		inline bool GetOutputSize(int& w, int& h)
			{ return SDL_GetRendererOutputSize(renderer.get(), &w, &h) == 0; }

		/**
		 * \brief     Determines whether a window supports the use of render targets
		 *
		 * \return    true if supported, false if not.
		 */
		inline bool TargetSupported()
			{ return SDL_RenderTargetSupported(renderer.get()) == SDL_TRUE; }

		/**
		 * \brief     Determines whether a window supports the use of render targets
		 *
		 * \param     support: A reference to be filled with true if supported, false if not.
		 */
		inline Renderer& TargetSupported(bool& support)
			{ support = SDL_RenderTargetSupported(renderer.get()) == SDL_TRUE; return *this; }

		/**
		 * \brief     Set a texture as the current rendering target.
		 *
		 * \param     texture: The targeted texture, which must be created with the SDL_TEXTUREACCESS_TARGET flag.
		 *
		 * \return    true on success, or false on error
		 */
		inline bool SetTarget(Texture& texture);

		/**
		 * \brief     Set the default render target as the current rendering target.
		 *
		 * \return    true on success, or false on error
		 */
		inline bool ClearTarget();

		/**
		 * Get the current render target.
		 *
		 * The default render target is the window for which the renderer was created,
		 * and is reported as NULL here.
		 *
		 * \returns the current render target or NULL for the default render target.
		 */
		inline Texture GetTarget();

		/**
		 * Get the current render target.
		 *
		 * The default render target is the window for which the renderer was created,
		 * and is reported as NULL here.
		 *
		 * \returns whether the texture is valid
		 */
		inline bool GetTarget(Texture& target);

#pragma endregion 

#pragma region Scaling

		/**
		 *  \brief    Set device independent resolution for rendering
		 *
		 *  \param    size: The size of the logical resolution
		 *
		 *  \details  This function uses the viewport and scaling functionality to allow a fixed logical
		 *            resolution for rendering, regardless of the actual output resolution.  If the actual
		 *            output resolution doesn't have the same aspect ratio the output rendering will be
		 *            centered within the output display.
		 *
		 *            If the output display is a window, mouse events in the window will be filtered
		 *            and scaled so they seem to arrive within the logical resolution.
		 *
		 *  \note     If this function results in scaling or subpixel drawing by the
		 *            rendering backend, it will be handled using the appropriate
		 *            quality hints.
		 */
		inline bool SetLogicalSize(const Point& size)
			{ return SDL_RenderSetLogicalSize(renderer.get(), size.w, size.h) == 0; }

		/**
		 *  \brief    Set device independent resolution for rendering
		 *
		 *  \param    w: The width of the logical resolution
		 *  \param    h: The height of the logical resolution
		 *
		 *  \details  This function uses the viewport and scaling functionality to allow a fixed logical
		 *            resolution for rendering, regardless of the actual output resolution.  If the actual
		 *            output resolution doesn't have the same aspect ratio the output rendering will be
		 *            centered within the output display.
		 *
		 *            If the output display is a window, mouse events in the window will be filtered
		 *            and scaled so they seem to arrive within the logical resolution.
		 *
		 *  \note     If this function results in scaling or subpixel drawing by the
		 *            rendering backend, it will be handled using the appropriate
		 *            quality hints.
		 */
		inline bool SetLogicalSize(int w, int h)
			{ return SDL_RenderSetLogicalSize(renderer.get(), w, h) == 0; }

		/**
		 *  \brief    Get device independent resolution for rendering
		 *
		 *  \return   The size of the logical resolution
		 */
		inline Point GetLogicalSize()
		{
			Point returnVal;
			SDL_RenderGetLogicalSize(renderer.get(), &returnVal.w, &returnVal.h);
			return returnVal;
		}

		/**
		 *  \brief    Get device independent resolution for rendering
		 *
		 *  \param    size: A reference filled with the size of the logical resolution
		 */
		inline Renderer& GetLogicalSize(Point& size)
			{ SDL_RenderGetLogicalSize(renderer.get(), &size.w, &size.h); return *this; }

		/**
		 *  \brief    Get device independent resolution for rendering
		 *
		 *  \param    w: A reference to be filled with the width of the logical resolution
		 *  \param    h: A reference to be filled with the height of the logical resolution
		 */
		inline Renderer& GetLogicalSize(int& w, int& h)
			{ SDL_RenderGetLogicalSize(renderer.get(), &w, &h); return *this; }

#if SDL_VERSION_ATLEAST(2, 0, 5)
		/**
		 *  \brief    Set whether to force integer scales for resolution-independent rendering
		 *
		 *  \param    enable: Enable or disable integer scaling
		 *
		 *  \note     This function restricts the logical viewport to integer values - that is, when
		 *            a resolution is between two multiples of a logical size, the viewport size is
		 *            rounded down to the lower multiple.
		 */
		inline bool SetIntegerScale(bool enable)
			{ return SDL_RenderSetIntegerScale(renderer.get(), enable ? SDL_TRUE : SDL_FALSE); }

		// Get whether integer scales are forced for resolution-independent rendering
		inline bool GetIntegerScale()
			{ return SDL_RenderGetIntegerScale(renderer.get()) == SDL_TRUE; }
#endif

		// Get whether integer scales are forced for resolution-independent rendering
		inline Renderer& GetIntegerScale(bool& enabled)
			{ enabled = SDL_RenderGetIntegerScale(renderer.get()) == SDL_TRUE; return *this; }

#pragma endregion 

#pragma region Destination Manipulation

		/**
		 *  \brief    Set the drawing area for rendering on the current target.
		 *
		 *  \param    rect: The rectangle representing the drawing area.
		 *                  The x,y of the viewport rect represents the origin for rendering.
		 *
		 *  \return   true on success, or false on error
		 *
		 *  \note     If the window associated with the renderer is resized, the viewport is automatically reset.
		 */
		inline bool SetViewport(const Rect& rect)
			{ SDL_RenderSetViewport(renderer.get(), &rect.rect) == 0; }

		/**
		 *  \brief    Sets the drawing area for rendering to the current target.
		 *
		 *  \return   true on success, or false on error
		 *
		 *  \note     If the window associated with the renderer is resized, the viewport is automatically reset.
		 */
		inline bool ClearViewport()
			{ return SDL_RenderSetViewport(renderer.get(), NULL) == 0; }

		// Get the drawing area for the current target.
		inline Rect GetViewport()
		{
			Rect viewport;
			SDL_RenderGetViewport(renderer.get(), &viewport.rect);
			return viewport;
		}

		// Get the drawing area for the current target.
		inline Renderer& GetViewport(Rect& rect)
			{ SDL_RenderGetViewport(renderer.get(), &rect.rect); return *this; }

		/**
		 *  \brief    Set the clip rectangle for the current target.
		 *
		 *  \param    rect: A reference to the rectangle to set as the clip rectangle,
		 *                  relative to the viewport
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool SetClipRect(const Rect& rect)
			{ return SDL_RenderSetClipRect(renderer.get(), &rect.rect) == 0; }

		/**
		 *  \brief    Disables clipping for the current target.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool DisableClip()
			{ return SDL_RenderSetClipRect(renderer.get(), NULL) == 0; }

		/**
		 *  \brief    Get the clip rectangle for the current target.
		 * 
		 *  \return   A rect filled in with the current clip rectangle, or
		 *            an empty rectangle if clipping is disabled.
		 */
		inline Rect GetClipRect()
		{
			Rect rect;
			SDL_RenderGetClipRect(renderer.get(), &rect.rect);
			return rect;
		}

		/**
		 *  \brief    Get the clip rectangle for the current target.
		 *
		 *  \param    rect: A reference filled in with the current clip rectangle, or
		 *                  an empty rectangle if clipping is disabled.
		 */
		inline Renderer& GetClipRect(Rect& rect)
			{ SDL_RenderGetClipRect(renderer.get(), &rect.rect); return *this; }

#if SDL_VERSION_ATLEAST(2, 0, 4)
		// Get whether clipping is enabled on the given renderer.
		inline bool IsClipEnabled()
			{ return SDL_RenderIsClipEnabled(renderer.get()) == SDL_TRUE; }

		// Get whether clipping is enabled on the given renderer.
		inline Renderer& IsClipEnabled(bool& enabled)
			{ enabled = SDL_RenderIsClipEnabled(renderer.get()) == SDL_TRUE; }
#endif

#pragma endregion 

#pragma region Pixel Reading

		/**
		 *  \brief    Read pixels from the current rendering target.
		 *
		 *  \param    rect:   A reference to the rectangle to read
		 *  \param    pixels: A pointer to be filled in with the pixel data
		 *  \param    pitch:  The pitch of the pixels parameter.
		 *  \param    format: The desired format of the pixel data, or 0 to use the format
		 *                    of the rendering target
		 *
		 *  \return   true on success, or false if pixel reading is not supported.
		 *
		 *  \warning  This is a very slow operation, and should not be used frequently.
		 */
		inline bool ReadPixelsRect(const Rect& rect, void* pixels, int pitch, Uint32 format = 0)
			{ return SDL_RenderReadPixels(renderer.get(), &rect.rect, format, pixels, pitch) == 0; }

		/**
		 *  \brief    Read pixels from the entire current rendering target.
		 *
		 *  \param    pixels: A pointer to be filled in with the pixel data
		 *  \param    pitch:  The pitch of the pixels parameter.
		 *  \param    format: The desired format of the pixel data, or 0 to use the format
		 *                    of the rendering target
		 *
		 *  \return   true on success, or false if pixel reading is not supported.
		 *
		 *  \warning  This is a very slow operation, and should not be used frequently.
		 */
		inline bool ReadPixels(void* pixels, int pitch, Uint32 format = 0)
			{ return SDL_RenderReadPixels(renderer.get(), NULL, format, pixels, pitch) == 0; }

#pragma endregion 

#if SDL_VERSION_ATLEAST(2, 0, 8)
		/**
		 *  \brief    Get the CAMetalLayer associated with the given Metal renderer
		 *
		 *  \return   CAMetalLayer* (as void*) on success, or NULL if the renderer isn't a Metal renderer
		 */
		inline void* GetMetalLayer()
			{ return SDL_RenderGetMetalLayer(renderer.get()); }

		/**
		 *  \brief    Get the Metal command encoder for the current frame
		 *
		 *  \return   id<MTLRenderCommandEncoder> on success, or NULL if the renderer isn't a Metal renderer
		 */
		inline void* GetMetalCommandEncoder()
			{ return SDL_RenderGetMetalCommandEncoder(renderer.get()); }
#endif

#if SDL_VERSION_ATLEAST(2, 0, 18)
		/**
		 * Toggle VSync of the given renderer.
		 *
		 * \param vsync true for on, false for off.
		 * \returns true on success, or false on failure
		 */
		inline bool SetVSync(bool vsync)
			{ return SDL_RenderSetVSync(renderer.get(), vsync ? 1 : 0) == 0; }
#endif
	};

	// An efficient driver-specific representation of pixel data
	struct Texture
	{
		// The scaling mode for a texture.
		enum class ScaleMode
		{
			Nearest = SDL_ScaleModeNearest, // nearest pixel sampling
			Linear  = SDL_ScaleModeLinear,  // linear filtering
			Best    = SDL_ScaleModeBest     // anisotropic filtering
		};

		// The access pattern allowed for a texture.
		enum class Access
		{
			STATIC    = SDL_TEXTUREACCESS_STATIC,    // Changes rarely, not lockable
			STREAMING = SDL_TEXTUREACCESS_STREAMING, // Changes frequently, lockable
			TARGET    = SDL_TEXTUREACCESS_TARGET     // Texture can be used as a render target
		};

		// The texture channel modulation used in Renderer::Copy().
		enum class Modulate {
			NONE  = SDL_TEXTUREMODULATE_NONE,     // No modulation
			COLOR = SDL_TEXTUREMODULATE_COLOR,    // srcC = srcC * colour
			ALPHA = SDL_TEXTUREMODULATE_ALPHA     // srcA = srcA * alpha
		};

#pragma region Safety

		// This is custom destructor for smart pointers that destroys SDL_Texture through SDL
		inline static void DestroyTexture(SDL_Texture* texture)
			{ SDL_DestroyTexture(texture); }

		// This is custom destructor for smart pointers that does not destroy the Texture. This is for pointers you do not own
		inline static void DontDestroyTexture(SDL_Texture* texture)
			{}

		// This creates a smart pointer to an SDL_Texture with a custom destructor
		inline static std::shared_ptr<SDL_Texture> MakeSharedPtr(SDL_Texture* texture)
			{ return std::shared_ptr<SDL_Texture>(texture, DestroyTexture); }

		// This creates a Texture from a SDL_Texture pointer, taking ownership of the pointer
		inline static Texture FromPtr(Renderer& renderer, SDL_Texture* texture)
			{ return Texture(renderer.renderer,MakeSharedPtr(texture)); }

		// This creates a Texture from a SDL_Texture pointer, but does not take ownership of the pointer
		inline static Texture FromUnownedPtr(Renderer& renderer, SDL_Texture* texture)
			{ return Texture(renderer.renderer, std::shared_ptr<SDL_Texture>(texture, DontDestroyTexture)); }

		std::shared_ptr<SDL_Renderer> renderer = nullptr;
		std::shared_ptr<SDL_Texture> texture = nullptr;

		inline Texture(std::shared_ptr<SDL_Renderer> renderer, std::shared_ptr<SDL_Texture> texture)
			: renderer(renderer), texture(texture) {}

#pragma endregion

#pragma region Constructors

		inline Texture()
			: Texture(nullptr, nullptr) {}
		inline Texture(const Texture& txt)
			: Texture(txt.renderer, txt.texture) {}
		inline Texture(Texture&& txt) noexcept
		{
			std::swap(renderer, txt.renderer);
			std::swap(texture, txt.texture);
		}
		inline Texture& operator=(const Texture& that) noexcept
		{
			renderer = that.renderer; texture = that.texture; return *this;
		}
		inline Texture& operator=(Texture&& that) noexcept
		{
			std::swap(renderer, that.renderer);
			std::swap(texture, that.texture);
			return *this;
		}

		/**
		 *  \brief    Create a texture for a rendering context.
		 *
		 *  \param    renderer: The renderer.
		 *  \param    size:     The size of the texture in pixels.
		 *  \param    access:   One of the enumerated values in ::Access.
		 *  \param    format:   The format of the texture.
		 *
		 *  \return   The created texture is returned, or NULL if no rendering context was
		 *            active,  the format was unsupported, or the width or height were out
		 *            of range.
		 *
		 *  \note     The contents of the texture are not defined at creation.
		 */
		inline Texture(Renderer& renderer, const Point& size, Access access = Access::STATIC, Uint32 format = (Uint32)PixelFormatEnum::RGBA32)
			: Texture(FromPtr(renderer, SDL_CreateTexture(renderer.renderer.get(), format, (SDL_TextureAccess)access, size.x, size.y))) {}
		/**
		 *  \brief    Create a texture from an existing surface.
		 *
		 *  \param    renderer: The renderer.
		 *  \param    surface:  The surface containing pixel data used to fill the texture.
		 *
		 *  \return   The created texture is returned, or NULL on error.
		 *
		 *  \note     The surface is not modified or freed by this function.
		 */
		inline Texture(Renderer& renderer, Surface& surface)
			: Texture(FromPtr(renderer, SDL_CreateTextureFromSurface(renderer.renderer.get(), surface.surface.get()))) {}

#pragma endregion 

#pragma region Pixel Editing

		/**
		 *  \brief    Lock a portion of the texture for write-only pixel access.
		 *
		 *  \param    rect:   A reference to the rectangle to lock for access.
		 *  \param    pixels: This is filled in with a pointer to the locked pixels,
		 *                    appropriately offset by the locked area.
		 *  \param    pitch:  This is filled in with the pitch of the locked pixels.
		 *
		 *  \return   true on success, or false if the texture is not valid or was not created with ::SDL_TEXTUREACCESS_STREAMING.
		 */
		inline bool LockRect(const Rect& rect, void*& pixels, int& pitch)
			{ return SDL_LockTexture(texture.get(), (const SDL_Rect*)&rect.rect, &pixels, &pitch) == 0; }

		/**
		 *  \brief    Lock the entire texture for write-only pixel access.
		 *
		 *  \param    pixels: This is filled in with a pointer to the locked pixels,
		 *                    appropriately offset by the locked area.
		 *  \param    pitch:  This is filled in with the pitch of the locked pixels.
		 *
		 *  \return   true on success, or false if the texture is not valid or was not created with ::SDL_TEXTUREACCESS_STREAMING.
		 */
		inline bool Lock(void*& pixels, int& pitch)
			{ return SDL_LockTexture(texture.get(), NULL, &pixels, &pitch) == 0; }

#if SDL_VERSION_ATLEAST(2, 0, 12)
		/**
		 *  \brief    Lock a portion of the texture for write-only pixel access.
		 *            Expose it as a SDL surface.
		 *
		 *  \param    rect:    A reference to the rectangle to lock for access.
		 *  \param    surface: This is filled in with a SDL surface representing the locked area
		 *                     Surface is freed internally after calling UnlockTexture() or deleting the Surface
		 *
		 *  \return   true on success, or false if the texture is not valid or was not created with ::SDL_TEXTUREACCESS_STREAMING.
		 */
		inline bool LockRectToSurface(const Rect& rect, Surface& surface)
		{
			SDL_Surface* surf;
			const bool success = SDL_LockTextureToSurface(texture.get(), &rect.rect, &surf) == 0;
			surface = Surface::FromUnownedPtr(surf);
			return success;
		}

		/**
		 *  \brief    Lock the entire texture for write-only pixel access.
		 *            Expose it as a SDL surface.
		 *
		 *  \param    surface: This is filled in with a SDL surface representing the locked area
		 *                     Surface is freed internally after calling UnlockTexture() or deleting the Surface
		 *
		 *  \return   true on success, or false if the texture is not valid or was not created with ::SDL_TEXTUREACCESS_STREAMING.
		 */
		inline bool LockToSurface(Surface& surface)
		{
			SDL_Surface* surf;
			const bool success = SDL_LockTextureToSurface(texture.get(), NULL, &surf) == 0;
			surface = Surface::FromUnownedPtr(surf);
			return success;
		}
#endif

		/**
		 *  \brief    Unlock a texture, uploading the changes to video memory, if needed.
		 *            If LockToSurface() was called for locking, the SDL surface is freed.
		 */
		inline void Unlock()
			{ SDL_UnlockTexture(texture.get()); }

		/**
		 *  \brief    Update the given texture rectangle with new pixel data.
		 *
		 *  \param    rect      A reference to the rectangle of pixels to update.
		 *  \param    pixels    The raw pixel data in the format of the texture.
		 *  \param    pitch     The number of bytes in a row of pixel data, including padding between lines.
		 *
		 *  \note     The pixel data must be in the format of the texture. The pixel format can be
		 *            queried with Query or QueryFormat.
		 *
		 *  \return   true on success, or false if the texture is not valid.
		 *
		 *  \warning  This is a fairly slow function.
		 */
		inline bool UpdateRect(const Rect& rect, void* pixels, int pitch)
			{ return SDL_UpdateTexture(texture.get(), (const SDL_Rect*)&rect.rect, pixels, pitch) == 0; }

		/**
		 *  \brief    Update the entire texture with new pixel data.
		 *
		 *  \param    pixels    The raw pixel data in the format of the texture.
		 *  \param    pitch     The number of bytes in a row of pixel data, including padding between lines.
		 *
		 *  \note     The pixel data must be in the format of the texture. The pixel format can be
		 *            queried with Query or QueryFormat.
		 *
		 *  \return   true on success, or false if the texture is not valid.
		 *
		 *  \warning  This is a fairly slow function.
		 */
		inline bool Update(void* pixels, int pitch)
			{ return SDL_UpdateTexture(texture.get(), NULL, pixels, pitch) == 0; }

#if SDL_VERSION_ATLEAST(2, 0, 1)
		/**
		 *  \brief    Update a rectangle within a planar YV12 or IYUV texture with new pixel data.
		 *
		 *  \param    rect:   A reference to the rectangle of pixels to update.
		 *  \param    Yplane: The raw pixel data for the Y plane.
		 *  \param    Ypitch: The number of bytes between rows of pixel data for the Y plane.
		 *  \param    Uplane: The raw pixel data for the U plane.
		 *  \param    Upitch: The number of bytes between rows of pixel data for the U plane.
		 *  \param    Vplane: The raw pixel data for the V plane.
		 *  \param    Vpitch: The number of bytes between rows of pixel data for the V plane.
		 *
		 *  \return   true on success, or false if the texture is not valid.
		 *
		 *  \note     You can use UpdateRect() as long as your pixel data is
		 *            a contiguous block of Y and U/V planes in the proper order, but
		 *            this function is available if your pixel data is not contiguous.
		 */
		inline bool UpdateYUVRect(const Rect& rect, const Uint8* Yplane, int Ypitch, const Uint8* Uplane, int Upitch, const Uint8* Vplane, int Vpitch)
			{ return SDL_UpdateYUVTexture(texture.get(), &rect.rect, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch) == 0; }

		/**
		 *  \brief    Update an entire planar YV12 or IYUV texture with new pixel data.
		 *
		 *  \param    Yplane: The raw pixel data for the Y plane.
		 *  \param    Ypitch: The number of bytes between rows of pixel data for the Y plane.
		 *  \param    Uplane: The raw pixel data for the U plane.
		 *  \param    Upitch: The number of bytes between rows of pixel data for the U plane.
		 *  \param    Vplane: The raw pixel data for the V plane.
		 *  \param    Vpitch: The number of bytes between rows of pixel data for the V plane.
		 *
		 *  \return   true on success, or false if the texture is not valid.
		 *
		 *  \note     You can use Update() as long as your pixel data is
		 *            a contiguous block of Y and U/V planes in the proper order, but
		 *            this function is available if your pixel data is not contiguous.
		 */
		inline bool UpdateYUV(const Uint8* Yplane, int Ypitch, const Uint8* Uplane, int Upitch, const Uint8* Vplane, int Vpitch)
			{ return SDL_UpdateYUVTexture(texture.get(), NULL, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch) == 0; }
#endif

#if SDL_VERSION_ATLEAST(2, 0, 16)
		/**
		 * Update a rectangle within a planar NV12 or NV21 texture with new pixels.
		 *
		 * You can use SDL_UpdateTexture() as long as your pixel data is a contiguous
		 * block of NV12/21 planes in the proper order, but this function is available
		 * if your pixel data is not contiguous.
		 *
		 * \param rect a pointer to the rectangle of pixels to update
		 * \param Yplane the raw pixel data for the Y plane.
		 * \param Ypitch the number of bytes between rows of pixel data for the Y
		 *               plane.
		 * \param UVplane the raw pixel data for the UV plane.
		 * \param UVpitch the number of bytes between rows of pixel data for the UV
		 *                plane.
		 * \return true on success, or false if the texture is not valid.
		 */
		inline bool UpdateNVRect(const Rect& rect, const Uint8* Yplane, int Ypitch, const Uint8* UVplane, int UVpitch)
			{ return SDL_UpdateNVTexture(texture.get(), &rect.rect, Yplane, Ypitch, UVplane, UVpitch) == 0; }

		/**
		 * Update an entire planar NV12 or NV21 texture with new pixels.
		 *
		 * You can use SDL_UpdateTexture() as long as your pixel data is a contiguous
		 * block of NV12/21 planes in the proper order, but this function is available
		 * if your pixel data is not contiguous.
		 *
		 * \param Yplane the raw pixel data for the Y plane.
		 * \param Ypitch the number of bytes between rows of pixel data for the Y
		 *               plane.
		 * \param UVplane the raw pixel data for the UV plane.
		 * \param UVpitch the number of bytes between rows of pixel data for the UV
		 *                plane.
		 * \return true on success, or false if the texture is not valid.
		 */
		inline bool UpdateNV(const Uint8* Yplane, int Ypitch, const Uint8* UVplane, int UVpitch)
			{ return SDL_UpdateNVTexture(texture.get(), NULL, Yplane, Ypitch, UVplane, UVpitch) == 0; }
#endif

#pragma endregion 

#pragma region Texture Settings

		/**
		 *  \brief    Query the format of a texture
		 *
		 *  \param    format: A reference to be filled in with the raw format of the texture. The
		 *                    actual format may differ, but pixel transfers will use this
		 *                    format.
		 *
		 *  \return   true on success, or false if the texture is not valid.
		 */
		inline bool QueryFormat(Uint32& format)
			{ return SDL_QueryTexture(texture.get(), &format, NULL, NULL, NULL) == 0; }

		/**
		 *  \brief    Query the access of a texture
		 *
		 *  \param    access: A reference to be filled in with the actual access to the texture.
		 *
		 *  \return   true on success, or false if the texture is not valid.
		 */
		inline bool QueryAccess(Access& access)
			{ return SDL_QueryTexture(texture.get(), NULL, (int*)&access, NULL, NULL) == 0; }

		/**
		 *  \brief    Query the size of a texture
		 *
		 *  \param    size: A reference to be filled in with the width and height of the texture in pixels.
		 *
		 *  \return   true on success, or false if the texture is not valid.
		 */
		inline bool QuerySize(Point& size)
			{ return SDL_QueryTexture(texture.get(), NULL, NULL, &size.w, &size.h) == 0; }

		/**
		 *  \brief    Query the attributes of a texture
		 *
		 *  \param    format: A reference to be filled in with the raw format of the texture. The
		 *                    actual format may differ, but pixel transfers will use this
		 *                    format.
		 *  \param    access: A reference to be filled in with the actual access to the texture.
		 *  \param    size:   A reference to be filled in with the width and height of the texture in pixels.
		 *
		 *  \return   true on success, or false if the texture is not valid.
		 */
		inline bool Query(Uint32& format, Access& access, Point& size)
			{ return SDL_QueryTexture(texture.get(), &format, (int*)&access, &size.w, &size.h) == 0; }

		/**
		 *  \brief    Query the attributes of a texture
		 *
		 *  \param    format: A pointer to be filled in with the raw format of the texture. The
		 *                    actual format may differ, but pixel transfers will use this
		 *                    format.
		 *  \param    access: A pointer to be filled in with the actual access to the texture.
		 *  \param    w:      A pointer to be filled in with the width of the texture in pixels.
		 *  \param    h:      A pointer to be filled in with the height of the texture in pixels.
		 *
		 *  \return   true on success, or false if the texture is not valid.
		 */
		inline bool Query(Uint32* format, Access* access, int* w, int* h)
			{ return SDL_QueryTexture(texture.get(), format, (int*)access, w, h) == 0; }

		/**
		 *  \brief    Set an additional colour value used in render copy operations.
		 *
		 *  \param    r: The red colour value multiplied into copy operations.
		 *  \param    g: The green colour value multiplied into copy operations.
		 *  \param    b: The blue colour value multiplied into copy operations.
		 *
		 *  \return   true on success, or false if the texture is not valid or colour modulation
		 *            is not supported.
		 */
		inline bool SetColourMod(Uint8 r, Uint8 g, Uint8 b)
			{ return SDL_SetTextureColorMod(texture.get(), r, g, b) == 0; }

		/**
		 *  \brief    Get the additional colour value used in render copy operations.
		 *
		 *  \param    r: A reference to be filled in with the current red colour value.
		 *  \param    g: A reference to be filled in with the current green colour value.
		 *  \param    b: A reference to be filled in with the current blue colour value.
		 *
		 *  \return   true on success, or false if the texture is not valid.
		 */
		inline bool GetColourMod(Uint8& r, Uint8& g, Uint8& b)
			{ return SDL_GetTextureColorMod(texture.get(), &r, &g, &b) == 0; }

		/**
		 *  \brief    Get the additional colour value used in render copy operations.
		 *
		 *  \param    c: A reference to be filled in with the current colour value.
		 *
		 *  \return   true on success, or false if the texture is not valid.
		 */
		inline bool GetColourMod(Colour& c)
			{ return SDL_GetTextureColorMod(texture.get(), &c.r, &c.g, &c.b) == 0; }

		/**
		 *  \brief    Set an additional alpha value used in render copy operations.
		 *
		 *  \param    alpha: The alpha value multiplied into copy operations.
		 *
		 *  \return   true on success, or false if the texture is not valid or alpha modulation
		 *            is not supported.
		 */
		inline bool SetAlphaMod(Uint8 alpha)
			{ return SDL_SetTextureAlphaMod(texture.get(), alpha) == 0; }

		/**
		 *  \brief    Get the additional alpha value used in render copy operations.
		 *
		 *  \param    alpha: A reference to be filled in with the current alpha value.
		 *
		 *  \return   true on success, or false if the texture is not valid.
		 */
		inline bool GetAlphaMod(Uint8& alpha)
			{ return SDL_GetTextureAlphaMod(texture.get(), &alpha) == 0; }
		
		/**
		 *  \brief    Get the additional alpha value used in render copy operations.
		 *
		 *  \param    c: A Colour reference to be given the current alpha value.
		 *
		 *  \return   true on success, or false if the texture is not valid.
		 */
		inline bool GetAlphaMod(Colour& c)
			{ return SDL_GetTextureAlphaMod(texture.get(), &c.a) == 0; }

		/**
		 *  \brief    Set an additional colour and alpha value used in render copy operations.
		 *
		 *  \param    r:     The red colour value multiplied into copy operations.
		 *  \param    g:     The green colour value multiplied into copy operations.
		 *  \param    b:     The blue colour value multiplied into copy operations.
		 *  \param    alpha: The blue colour value multiplied into copy operations.
		 *
		 *  \return   0 on success, -1 if colour modulation is not supported,
		 *            -2 if alpha modulation is not supported, or -3 if neither
		 *            are supported or if the texture is not valid.
		 */
		inline int SetMod(const Colour& c)
			{ return( SetColourMod(c.r, c.g, c.b) ? 0 : -1) + (SetAlphaMod(c.a) ? 0 : -2); }

		/**
		 *  \brief    Get the additional colour value used in render copy operations.
		 *
		 *  \param    r:     A reference to be filled in with the current red colour value.
		 *  \param    g:     A reference to be filled in with the current green colour value.
		 *  \param    b:     A reference to be filled in with the current blue colour value.
		 *  \param    alpha: A reference to be filled in with the current alpha value.
		 *
		 *  \return   0 on success, -1 if colour modulation is not supported,
		 *            -2 if alpha modulation is not supported, or -3 if neither
		 *            are supported or if the texture is not valid.
		 */
		inline int GetMod(Colour& c)
			{ return (GetColourMod(c.r, c.g, c.b) ? 0 : -1) + (GetAlphaMod(c.a) ? 0 : -2); }

		/**
		 *  \brief    Set the blend mode used for texture copy operations.
		 *
		 *  \param    blendMode: The blend mode to use for texture blending.
		 *
		 *  \note     If the blend mode is not supported, the closest supported mode is
		 *            chosen.
		 *
		 *  \return   true on success, or false if the texture is not valid or the blend mode is
		 *            not supported.
		 */
		inline bool SetBlendMode(BlendMode blendMode)
			{ return SDL_SetTextureBlendMode(texture.get(), (SDL_BlendMode)blendMode) == 0; }

		/**
		 *  \brief    Get the blend mode used for texture copy operations.
		 *
		 *  \param    blendMode: A reference to be filled in with the current blend mode.
		 *
		 *  \return   true on success, or false if the texture is not valid.
		 */
		inline bool GetBlendMode(BlendMode& blendMode)
			{ return SDL_GetTextureBlendMode(texture.get(), (SDL_BlendMode*)&blendMode) == 0; }

#if SDL_VERSION_ATLEAST(2, 0, 12)
		/**
		 *  \brief    Set the scale mode used for texture scale operations.
		 *
		 *  \param    scaleMode: SDL::ScaleMode to use for texture scaling.
		 *
		 *  \note     If the scale mode is not supported, the closest supported mode is
		 *            chosen.
		 *
		 *  \return   true on success, or false if the texture is not valid.
		 */
		inline bool SetScaleMode(ScaleMode scaleMode)
			{ return SDL_SetTextureScaleMode(texture.get(), (SDL_ScaleMode)scaleMode) == 0; }

		/**
		 *  \brief    Get the scale mode used for texture scale operations.
		 *
		 *  \param    scaleMode: A pointer filled in with the current scale mode.
		 *
		 *  \return   true on success, or false if the texture is not valid.
		 */
		inline bool GetScaleMode(ScaleMode& scaleMode)
			{ return SDL_GetTextureScaleMode(texture.get(), (SDL_ScaleMode*)&scaleMode) == 0; }
#endif

#if SDL_VERSION_ATLEAST(2, 0, 18)
		/**
		 * Associate a user-specified pointer with a texture.
		 *
		 * \param texture the texture to update.
		 * \param userdata the pointer to associate with the texture.
		 * \returns true on success, or false if the texture is not valid.
		 */
		inline bool SetUserData(void* userdata)
			{ return SDL_SetTextureUserData(texture.get(), userdata) == 0; }

		/**
		 * Get the user-specified pointer associated with a texture
		 *
		 * \param texture the texture to query.
		 * \return the pointer associated with the texture, or NULL if the texture is
		 *         not valid.
		 */
		inline void* GetUserData()
			{ return SDL_GetTextureUserData(texture.get()); }
#endif

#pragma endregion 

#pragma region Texture Copying
		
		// Flip constants for CopyEx
		enum class Flip
		{
			NONE       = SDL_FLIP_NONE,       // Do not flip
			HORIZONTAL = SDL_FLIP_HORIZONTAL, // flip horizontally
			VERTICAL   = SDL_FLIP_VERTICAL    // flip vertically
		};

		/**
		 *  \brief    Copy a portion of the texture to the current rendering target.
		 *
		 *  \param    srcrect: A reference to the source rectangle.
		 *  \param    dstrect: A reference to the destination rectangle.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool Copy(const Rect& src, const Rect& dst);

		/**
		 *  \brief    Copy the texture to the current rendering target.
		 *
		 *  \param    dstrect: A reference to the destination rectangle.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool Copy(const Rect& dst);

		/**
		 *  \brief    Copy a portion of the texture to the entire current rendering target.
		 *
		 *  \param    srcrect: A reference to the source rectangle.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool Copy_Fill(const Rect& src);

		/**
		 *  \brief    Copy the texture to the entire current rendering target.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool Copy_Fill();

		/**
		 *  \brief    Copy a portion of the texture to the current rendering target.
		 *
		 *  \param    srcrect: A pointer to the source rectangle, or NULL for the entire texture.
		 *  \param    dstrect: A pointer to the destination rectangle, or NULL for the entire rendering target.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool Copy(const Rect* src, const Rect* dst);

		/**
		 *  \brief    Copy a portion of the texture to the current rendering target, rotating it by an angle around the given center.
		 *
		 *  \param    srcrect: A reference to the source rectangle.
		 *  \param    dstrect: A reference to the destination rectangle.
		 *  \param    center:  A reference to a point indicating the point around which dstrect will be rotated.
		 *  \param    angle:   An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param    flip:    A Texture::Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyEx(const Rect& src, const Rect& dst, const Point& center, double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief    Copy a portion of the texture to the current rendering target, rotating it by an angle around dstrect.w/2, dstrect.h/2.
		 *
		 *  \param    srcrect: A reference to the source rectangle.
		 *  \param    dstrect: A reference to the destination rectangle.
		 *  \param    angle:   An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param    flip:    A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyEx(const Rect& src, const Rect& dst, double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief    Copy the texture to the current rendering target, rotating it by an angle around the given center.
		 *
		 *  \param    dstrect: A reference to the destination rectangle.
		 *  \param    center:  A reference to a point indicating the point around which dstrect will be rotated (if NULL, rotation will be done around dstrect.w/2, dstrect.h/2).
		 *  \param    angle:   An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param    flip:    A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyEx(const Rect& dst, const Point& center, double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief    Copy the texture to the current rendering target, rotating it by an angle around dstrect.w/2, dstrect.h/2
		 *
		 *  \param    dstrect: A reference to the destination rectangle.
		 *  \param    angle:   An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param    flip:    A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyEx(const Rect& dst, double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief    Copy a portion of the texture to the entire current rendering target, rotating it by an angle around the given center
		 *
		 *  \param    srcrect: A reference to the source rectangle.
		 *  \param    center:  A reference to a point indicating the point around which dstrect will be rotated.
		 *  \param    angle:   An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param    flip:    A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyEx_Fill(const Rect& src, const Point& center, double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief    Copy a portion of the texture to the entire current rendering target, rotating it by an angle around dstrect.w/2, dstrect.h/2.
		 *
		 *  \param    srcrect: A reference to the source rectangle.
		 *  \param    angle:   An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param    flip:    A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyEx_Fill(const Rect& src, double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief    Copy the texture to the entire current rendering target, rotating it by an angle around the given center.
		 *
		 *  \param    center: A reference to a point indicating the point around which dstrect will be rotated.
		 *  \param    angle:  An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param    flip:   A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyEx_Fill(const Point& center, double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief    Copy the texture to the entire current rendering target, rotating it by an angle around dstrect.w/2, dstrect.h/2.
		 *
		 *  \param    angle: An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param    flip:  A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyEx_Fill(double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief    Copy a portion of the texture to the current rendering target, rotating it by angle around the given center.
		 *
		 *  \param    srcrect: A pointer to the source rectangle, or NULL for the entire texture.
		 *  \param    dstrect: A pointer to the destination rectangle, or NULL for the entire rendering target.
		 *  \param    center:  A pointer to a point indicating the point around which dstrect will be rotated (if NULL, rotation will be done around dstrect.w/2, dstrect.h/2).
		 *  \param    angle:   An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction.
		 *  \param    flip:    An Flip value stating which flipping actions should be performed on the texture.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyEx(const Rect* src, const Rect* dst, const Point* center, double angle = 0.0, Flip flipType = Flip::NONE);

#if SDL_VERSION_ATLEAST(2, 0, 18)
		/**
		 * Render a list of triangles using a texture, and optionally indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param num_vertices Number of vertices.
		 * \param indices (optional) An array of integer indices into the 'vertices'
		 *                array, if NULL all vertices will be rendered in sequential
		 *                order.
		 * \param num_indices Number of indices.
		 * \return true on success, or false if the operation is not supported
		 */
		inline bool RenderGeometry(const Vertex* vertices, int num_vertices, const int* indices, int num_indices)
			{ return SDL_RenderGeometry(renderer.get(), texture.get(), (const SDL_Vertex*)vertices, num_vertices, indices, num_indices) == 0; }

		/**
		 * Render a list of triangles using a texture, and optionally indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param indices (optional) An array of integer indices into the 'vertices'
		 *                array, if NULL all vertices will be rendered in sequential
		 *                order.
		 * \param num_indices Number of indices.
		 * \return true on success, or false if the operation is not supported
		 */
		template <const int num_vertices>
		inline bool RenderGeometry(const Vertex(&vertices)[num_vertices], const int* indices, int num_indices)
			{ return SDL_RenderGeometry(renderer.get(), texture.get(), (const SDL_Vertex*)vertices, num_vertices, indices, num_indices) == 0; }

		/**
		 * Render a list of triangles using a texture, and optionally indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param indices (optional) An array of integer indices into the 'vertices'
		 *                array, if NULL all vertices will be rendered in sequential
		 *                order.
		 * \param num_indices Number of indices.
		 * \return true on success, or false if the operation is not supported
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<Vertex, T>::is_continuous_container>>
		inline bool RenderGeometry(const T& vertices, const int* indices, int num_indices)
			{ return SDL_RenderGeometry(renderer.get(), texture.get(), (const SDL_Vertex*)vertices.data(), (int)vertices.size(), indices, num_indices) == 0; }

		/**
		 * Render a list of triangles using a texture and indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param num_vertices Number of vertices.
		 * \param indices An array of integer indices into the 'vertices' array.
		 * \return true on success, or false if the operation is not supported
		 */
		template <const int num_indices>
		inline bool RenderGeometry(const Vertex* vertices, int num_vertices, const int(&indices)[num_indices])
			{ return SDL_RenderGeometry(renderer.get(), texture.get(), (const SDL_Vertex*)vertices, num_vertices, indices, num_indices) == 0; }

		/**
		 * Render a list of triangles using a texture and indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param indices An array of integer indices into the 'vertices' array.
		 * \return true on success, or false if the operation is not supported
		 */
		template <const int num_vertices, const int num_indices>
		inline bool RenderGeometry(const Vertex(&vertices)[num_vertices], const int(&indices)[num_indices])
			{ return SDL_RenderGeometry(renderer.get(), texture.get(), (const SDL_Vertex*)vertices, num_vertices, indices, num_indices) == 0; }

		/**
		 * Render a list of triangles using a texture and indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param indices An array of integer indices into the 'vertices' array.
		 * \return true on success, or false if the operation is not supported
		 */
		template <typename T, const int num_indices, typename = typename std::enable_if_t<ContinuousContainer_traits<Vertex, T>::is_continuous_container>>
		inline bool RenderGeometry(const T& vertices, const int(&indices)[num_indices])
			{ return SDL_RenderGeometry(renderer.get(), texture.get(), (const SDL_Vertex*)vertices.data(), (int)vertices.size(), indices, num_indices) == 0; }

		/**
		 * Render a list of triangles using a texture and indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param num_vertices Number of vertices.
		 * \param indices A container of integer indices into the 'vertices' array.
		 * \return true on success, or false if the operation is not supported
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<int, T>::is_continuous_container>>
		inline bool RenderGeometry(const Vertex* vertices, int num_vertices, const T& indices)
			{ return SDL_RenderGeometry(renderer.get(), texture.get(), (const SDL_Vertex*)vertices, num_vertices, indices.data(), (int)indices.size()) == 0; }

		/**
		 * Render a list of triangles using a texture and indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param indices A container of integer indices into the 'vertices' array.
		 * \return true on success, or false if the operation is not supported
		 */
		template <const int num_vertices, typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<int, T>::is_continuous_container>>
		inline bool RenderGeometry(const Vertex(&vertices)[num_vertices], const T& indices)
			{ return SDL_RenderGeometry(renderer.get(), texture.get(), (const SDL_Vertex*)vertices, num_vertices, indices.data(), (int)indices.size()) == 0; }

		/**
		 * Render a list of triangles using a texture and indices into the
		 * vertex array. Colour and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param vertices Vertices.
		 * \param indices A container of integer indices into the 'vertices' array.
		 * \return true on success, or false if the operation is not supported
		 */
		template <
			typename T1,
			typename T2,
			typename = typename std::enable_if_t<
			ContinuousContainer_traits<Vertex, T1>::is_continuous_container &&
			ContinuousContainer_traits<int, T2>::is_continuous_container
			>>
		inline bool RenderGeometry(const T1& vertices, const T2& indices)
			{ return SDL_RenderGeometry(renderer.get(), texture.get(), (const SDL_Vertex*)vertices.data(), (int)vertices.size(), indices.data(), (int)indices.size()) == 0; }

		/**
		 * Render a list of triangles using a texture. Colour and alpha
		 * modulation is done per vertex (SetColourMod and SetAlphaMod are
		 * ignored).
		 *
		 * \param vertices Vertices.
		 * \param num_vertices Number of vertices.
		 * \return true on success, or false if the operation is not supported
		 */
		inline bool RenderGeometry(const Vertex* vertices, int num_vertices)
			{ return SDL_RenderGeometry(renderer.get(), texture.get(), (const SDL_Vertex*)vertices, num_vertices, NULL, 0) == 0; }

		/**
		 * Render a list of triangles using a texture. Colour and alpha
		 * modulation is done per vertex (SetColourMod and SetAlphaMod are
		 * ignored).
		 *
		 * \param vertices Vertices.
		 * \return true on success, or false if the operation is not supported
		 */
		template <const int num_vertices>
		inline bool RenderGeometry(const Vertex(&vertices)[num_vertices])
			{ return SDL_RenderGeometry(renderer.get(), texture.get(), (const SDL_Vertex*)vertices, num_vertices, NULL, 0) == 0; }

		/**
		 * Render a list of triangles using a texture. Colour and alpha
		 * modulation is done per vertex (SetColourMod and SetAlphaMod are
		 * ignored).
		 *
		 * \param vertices Vertices.
		 * \return true on success, or false if the operation is not supported
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<Vertex, T>::is_continuous_container>>
		inline bool RenderGeometry(const T& vertices)
			{ return SDL_RenderGeometry(renderer.get(), texture.get(), (const SDL_Vertex*)vertices.data(), (int)vertices.size(), NULL, 0) == 0; }

		/**
		 * Render a list of triangles using a texture, and optionally indices into the
		 * vertex arrays. Color and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param xy Vertex positions
		 * \param xy_stride Byte size to move from one element to the next element
		 * \param colour Vertex colours (as Colour)
		 * \param colour_stride Byte size to move from one element to the next element
		 * \param uv Vertex normalized texture coordinates
		 * \param uv_stride Byte size to move from one element to the next element
		 * \param num_vertices Number of vertices.
		 * \param indices (optional) An array of indices into the 'vertices' arrays,
		 *                if NULL all vertices will be rendered in sequential order.
		 * \param num_indices Number of indices.
		 * \param size_indices Index size: 1 (byte), 2 (short), 4 (int)
		 * \return true on success, or false if the operation is not supported
		 */
		inline bool RenderGeometryRaw(
			const float* xy, int xy_stride,
			const Colour* colour, int colour_stride,
			const float* uv, int uv_stride,
			int num_vertices,
			const void* indices, int num_indices,
			int size_indices)
		{
			return SDL_RenderGeometryRaw(
				renderer.get(),
				texture.get(),
				xy, xy_stride,
				colour, colour_stride,
				uv, uv_stride,
				num_vertices,
				indices, num_indices,
				size_indices
			) == 0;
		}

		/**
		 * Render a list of triangles using a texture and indices into the
		 * vertex arrays. Color and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param xy Vertex positions
		 * \param xy_stride Byte size to move from one element to the next element
		 * \param colour Vertex colours (as Colour)
		 * \param colour_stride Byte size to move from one element to the next element
		 * \param uv Vertex normalized texture coordinates
		 * \param uv_stride Byte size to move from one element to the next element
		 * \param num_vertices Number of vertices.
		 * \param indices An array of indices into the 'vertices' arrays.
		 * \return true on success, or false if the operation is not supported
		 */
		template <
			typename t,
			const int num_indices,
			typename = typename std::enable_if_t<
				sizeof(t) == 1 ||
				sizeof(t) == 2 ||
				sizeof(t) == 4
			>>
		inline bool RenderGeometryRaw(
			const float* xy, int xy_stride,
			const Colour* colour, int colour_stride,
			const float* uv, int uv_stride,
			int num_vertices,
			const t(&indices)[num_indices])
		{
			return SDL_RenderGeometryRaw(
				renderer.get(),
				texture.get(),
				xy, xy_stride,
				colour, colour_stride,
				uv, uv_stride,
				num_vertices,
				indices, num_indices,
				sizeof(t)
			) == 0;
		}

		/**
		 * Render a list of triangles using a texture and indices into the
		 * vertex arrays. Color and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param xy Vertex positions
		 * \param xy_stride Byte size to move from one element to the next element
		 * \param colour Vertex colours (as Colour)
		 * \param colour_stride Byte size to move from one element to the next element
		 * \param uv Vertex normalized texture coordinates
		 * \param uv_stride Byte size to move from one element to the next element
		 * \param num_vertices Number of vertices.
		 * \param indices An container of indices into the 'vertices' arrays.
		 * \return true on success, or false if the operation is not supported
		 */
		template <
			typename t,
			typename T,
			typename = typename std::enable_if_t<
				ContinuousContainer_traits<t, T>::is_continuous_container &&
				(sizeof(t) == 1 || sizeof(t) == 2 || sizeof(t) == 4)
			>>
		inline bool RenderGeometryRaw(
			const float* xy, int xy_stride,
			const Colour* colour, int colour_stride,
			const float* uv, int uv_stride,
			int num_vertices,
			const T& indices)
		{
			return SDL_RenderGeometryRaw(
				renderer.get(),
				texture.get(),
				xy, xy_stride,
				colour, colour_stride,
				uv, uv_stride,
				num_vertices,
				indices.data(), (int)indices.size(),
				sizeof(T)
			) == 0;
		}

		/**
		 * Render a list of triangles using a texture, and optionally indices into the
		 * vertex arrays. Color and alpha modulation is done per vertex
		 * (SetColourMod and SetAlphaMod are ignored).
		 *
		 * \param xy Vertex positions
		 * \param xy_stride Byte size to move from one element to the next element
		 * \param colour Vertex colours (as Colour)
		 * \param colour_stride Byte size to move from one element to the next element
		 * \param uv Vertex normalized texture coordinates
		 * \param uv_stride Byte size to move from one element to the next element
		 * \param num_vertices Number of vertices.
		 * \return true on success, or false if the operation is not supported
		 */
		inline bool RenderGeometryRaw(
			const float* xy, int xy_stride,
			const Colour* colour, int colour_stride,
			const float* uv, int uv_stride,
			int num_vertices)
		{
			return SDL_RenderGeometryRaw(
				renderer.get(),
				texture.get(),
				xy, xy_stride,
				colour, colour_stride,
				uv, uv_stride,
				num_vertices,
				NULL, 0, 0
			) == 0;
		}
#endif

#if SDL_VERSION_ATLEAST(2, 0, 10)
		/**
		 *  \brief    Copy a portion of the texture to the current rendering target.
		 *
		 *  \param    srcrect: A reference to the source rectangle.
		 *  \param    dstrect: A reference to the destination rectangle.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyF(const Rect& src, const FRect& dst);

		/**
		 *  \brief    Copy the texture to the current rendering target.
		 *
		 *  \param    dstrect: A reference to the destination rectangle.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyF(const FRect& dst);

		/**
		 *  \brief    Copy a portion of the texture to the entire current rendering target.
		 *
		 *  \param    srcrect: A reference to the source rectangle.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyF_Fill(const Rect& src);

		/**
		 *  \brief    Copy the texture to the entire current rendering target.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyF_Fill();

		/**
		 *  \brief    Copy a portion of the texture to the current rendering target.
		 *
		 *  \param    srcrect: A pointer to the source rectangle, or NULL for the entire
		 *                     texture.
		 *  \param    dstrect: A pointer to the destination rectangle, or NULL for the
		 *                     entire rendering target.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyF(const Rect* src, const FRect* dst);

		/**
		 *  \brief    Copy a portion of the texture to the current rendering target, rotating it by an angle around the given center.
		 *
		 *  \param    srcrect: A reference to the source rectangle.
		 *  \param    dstrect: A reference to the destination rectangle.
		 *  \param    center:  A reference to a point indicating the point around which dstrect will be rotated.
		 *  \param    angle:   An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param    flip:    A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyExF(const Rect& src, const FRect& dst, const FPoint& center, double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief    Copy a portion of the texture to the current rendering target, rotating it by an angle around dstrect.w/2, dstrect.h/2.
		 *
		 *  \param    srcrect: A reference to the source rectangle.
		 *  \param    dstrect: A reference to the destination rectangle.
		 *  \param    angle:   An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param    flip:    A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyExF(const Rect& src, const FRect& dst, double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief    Copy the texture to the current rendering target, rotating it by an angle around the given center.
		 *
		 *  \param    dstrect: A reference to the destination rectangle.
		 *  \param    center:  A reference to a point indicating the point around which dstrect will be rotated.
		 *  \param    angle:   An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param    flip:    A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyExF(const FRect& dst, const FPoint& center, double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief    Copy the texture to the current rendering target, rotating it by an angle around dstrect.w/2, dstrect.h/2
		 *
		 *  \param    dstrect: A reference to the destination rectangle.
		 *  \param    angle:   An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param    flip:    A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyExF(const FRect& dst, double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief    Copy a portion of the texture to the entire current rendering target, rotating it by an angle around the given center
		 *
		 *  \param    srcrect: A reference to the source rectangle.
		 *  \param    center:  A reference to a point indicating the point around which dstrect will be rotated.
		 *  \param    angle:   An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param    flip:    A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyExF_Fill(const Rect& src, const FPoint& center, double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief    Copy a portion of the texture to the entire current rendering target, rotating it by an angle around dstrect.w/2, dstrect.h/2.
		 *
		 *  \param    srcrect: A reference to the source rectangle.
		 *  \param    angle:   An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param    flip:    A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyExF_Fill(const Rect& src, double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief    Copy the texture to the entire current rendering target, rotating it by an angle around the given center.
		 *
		 *  \param    center: A reference to a point indicating the point around which dstrect will be rotated.
		 *  \param    angle:  An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param    flip:   A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyExF_Fill(const FPoint& center, double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief    Copy the texture to the entire current rendering target, rotating it by an angle around dstrect.w/2, dstrect.h/2.
		 *
		 *  \param    angle: An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param    flip:  A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyExF_Fill(double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief    Copy a portion of the texture to the current rendering target, rotating it by angle around the given center.
		 *
		 *  \param    srcrect: A pointer to the source rectangle, or NULL for the entire texture.
		 *  \param    dstrect: A pointer to the destination rectangle, or NULL for the entire rendering target.
		 *  \param    center:  A pointer to a point indicating the point around which dstrect will be rotated (if NULL, rotation will be done around dstrect.w/2, dstrect.h/2).
		 *  \param    angle:   An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction.
		 *  \param    flip:    An Flip value stating which flipping actions should be performed on the texture.
		 *
		 *  \return   true on success, or false on error
		 */
		inline bool CopyExF(const Rect* src, const FRect* dst, const FPoint* center, double angle = 0.0, Flip flipType = Flip::NONE);
#endif

#pragma endregion 

	};

	inline bool Texture::Copy      (const Rect* src, const  Rect* dst) { return SDL_RenderCopy (renderer.get(), texture.get(), (SDL_Rect*)src, ( SDL_Rect*)dst) == 0; }
	inline bool Texture::Copy      (const Rect& src, const  Rect& dst) { return SDL_RenderCopy (renderer.get(), texture.get(), &src.rect, &dst.rect) == 0; }
	inline bool Texture::Copy      (                 const  Rect& dst) { return SDL_RenderCopy (renderer.get(), texture.get(), NULL,      &dst.rect) == 0; }
	inline bool Texture::Copy_Fill (const Rect& src                  ) { return SDL_RenderCopy (renderer.get(), texture.get(), &src.rect, NULL     ) == 0; }
	inline bool Texture::Copy_Fill (                                 ) { return SDL_RenderCopy (renderer.get(), texture.get(), NULL,      NULL     ) == 0; }

	inline bool Texture::CopyEx      (const Rect* src, const  Rect* dst, const  Point* center, double angle, Flip flipType) { return SDL_RenderCopyEx (renderer.get(), texture.get(), (SDL_Rect*)src, ( SDL_Rect*)dst, angle, ( SDL_Point*)center, (SDL_RendererFlip)flipType) == 0; }
	inline bool Texture::CopyEx      (const Rect& src, const  Rect& dst, const  Point& center, double angle, Flip flipType) { return SDL_RenderCopyEx (renderer.get(), texture.get(), &src.rect, &dst.rect, angle, &center.point, (SDL_RendererFlip)flipType) == 0; }
	inline bool Texture::CopyEx      (const Rect& src, const  Rect& dst,                       double angle, Flip flipType) { return SDL_RenderCopyEx (renderer.get(), texture.get(), &src.rect, &dst.rect, angle, NULL,          (SDL_RendererFlip)flipType) == 0; }
	inline bool Texture::CopyEx      (                 const  Rect& dst, const  Point& center, double angle, Flip flipType) { return SDL_RenderCopyEx (renderer.get(), texture.get(), NULL,      &dst.rect, angle, &center.point, (SDL_RendererFlip)flipType) == 0; }
	inline bool Texture::CopyEx      (                 const  Rect& dst,                       double angle, Flip flipType) { return SDL_RenderCopyEx (renderer.get(), texture.get(), NULL,      &dst.rect, angle, NULL,          (SDL_RendererFlip)flipType) == 0; }
	inline bool Texture::CopyEx_Fill (const Rect& src,                   const  Point& center, double angle, Flip flipType) { return SDL_RenderCopyEx (renderer.get(), texture.get(), &src.rect, NULL,      angle, &center.point, (SDL_RendererFlip)flipType) == 0; }
	inline bool Texture::CopyEx_Fill (const Rect& src,                                         double angle, Flip flipType) { return SDL_RenderCopyEx (renderer.get(), texture.get(), &src.rect, NULL,      angle, NULL,          (SDL_RendererFlip)flipType) == 0; }
	inline bool Texture::CopyEx_Fill (                                   const  Point& center, double angle, Flip flipType) { return SDL_RenderCopyEx (renderer.get(), texture.get(), NULL,      NULL,      angle, &center.point, (SDL_RendererFlip)flipType) == 0; }
	inline bool Texture::CopyEx_Fill (                                                         double angle, Flip flipType) { return SDL_RenderCopyEx (renderer.get(), texture.get(), NULL,      NULL,      angle, NULL,          (SDL_RendererFlip)flipType) == 0; }

#if SDL_VERSION_ATLEAST(2, 0, 10)
	inline bool Texture::CopyF     (const Rect* src, const FRect* dst) { return SDL_RenderCopyF(renderer.get(), texture.get(), (SDL_Rect*)src, (SDL_FRect*)dst) == 0; }
	inline bool Texture::CopyF     (const Rect& src, const FRect& dst) { return SDL_RenderCopyF(renderer.get(), texture.get(), &src.rect, &dst.rect) == 0; }
	inline bool Texture::CopyF     (                 const FRect& dst) { return SDL_RenderCopyF(renderer.get(), texture.get(), NULL,      &dst.rect) == 0; }
	inline bool Texture::CopyF_Fill(const Rect& src                  ) { return SDL_RenderCopyF(renderer.get(), texture.get(), &src.rect, NULL     ) == 0; }
	inline bool Texture::CopyF_Fill(                                 ) { return SDL_RenderCopyF(renderer.get(), texture.get(), NULL,      NULL     ) == 0; }

	inline bool Texture::CopyExF     (const Rect* src, const FRect* dst, const FPoint* center, double angle, Flip flipType) { return SDL_RenderCopyExF(renderer.get(), texture.get(), (SDL_Rect*)src, (SDL_FRect*)dst, angle, (SDL_FPoint*)center, (SDL_RendererFlip)flipType) == 0; }
	inline bool Texture::CopyExF     (const Rect& src, const FRect& dst, const FPoint& center, double angle, Flip flipType) { return SDL_RenderCopyExF(renderer.get(), texture.get(), &src.rect, &dst.rect, angle, &center.point, (SDL_RendererFlip)flipType) == 0; }
	inline bool Texture::CopyExF     (const Rect& src, const FRect& dst,                       double angle, Flip flipType) { return SDL_RenderCopyExF(renderer.get(), texture.get(), &src.rect, &dst.rect, angle, NULL,          (SDL_RendererFlip)flipType) == 0; }
	inline bool Texture::CopyExF     (                 const FRect& dst, const FPoint& center, double angle, Flip flipType) { return SDL_RenderCopyExF(renderer.get(), texture.get(), NULL,      &dst.rect, angle, &center.point, (SDL_RendererFlip)flipType) == 0; }
	inline bool Texture::CopyExF     (                 const FRect& dst,                       double angle, Flip flipType) { return SDL_RenderCopyExF(renderer.get(), texture.get(), NULL,      &dst.rect, angle, NULL,          (SDL_RendererFlip)flipType) == 0; }
	inline bool Texture::CopyExF_Fill(const Rect& src,                   const FPoint& center, double angle, Flip flipType) { return SDL_RenderCopyExF(renderer.get(), texture.get(), &src.rect, NULL,      angle, &center.point, (SDL_RendererFlip)flipType) == 0; }
	inline bool Texture::CopyExF_Fill(const Rect& src,                                         double angle, Flip flipType) { return SDL_RenderCopyExF(renderer.get(), texture.get(), &src.rect, NULL,      angle, NULL,          (SDL_RendererFlip)flipType) == 0; }
	inline bool Texture::CopyExF_Fill(                                   const FPoint& center, double angle, Flip flipType) { return SDL_RenderCopyExF(renderer.get(), texture.get(), NULL,      NULL,      angle, &center.point, (SDL_RendererFlip)flipType) == 0; }
	inline bool Texture::CopyExF_Fill(                                                         double angle, Flip flipType) { return SDL_RenderCopyExF(renderer.get(), texture.get(), NULL,      NULL,      angle, NULL,          (SDL_RendererFlip)flipType) == 0; }
#endif

	inline bool Renderer::SetTarget  (Texture& texture) { return SDL_SetRenderTarget(renderer.get(), texture.texture.get()) == 0; }
	inline bool Renderer::ClearTarget(                ) { return SDL_SetRenderTarget(renderer.get(), NULL                 ) == 0; }

	inline Texture Renderer::GetTarget()
	{
		return Texture::FromUnownedPtr(*this, SDL_GetRenderTarget(renderer.get()));
	}

	inline bool Renderer::GetTarget(Texture& target)
	{
		SDL_Texture* const txt = SDL_GetRenderTarget(renderer.get());

		if (txt == NULL) 
		{
			return false;
		}
			
		target = Texture::FromUnownedPtr(*this, txt);
		return true;
	}

	/**
	 *  \brief    Create a window and default renderer
	 *
	 *  \param    size:         The size of the window
	 *  \param    window:       A reference filled with the window, or NULL on error
	 *  \param    renderer:     A reference filled with the renderer, or NULL on error
	 *  \param    window_flags: The flags used to create the window
	 *
	 *  \return   true on success, or false on error
	 */
	inline bool CreateWindowAndRenderer(const Point& size, Window& window, Renderer& renderer, Uint32 window_flags = SDL_WINDOW_SHOWN & SDL_WINDOW_RESIZABLE)
	{
		SDL_Window* w;
		SDL_Renderer* r;
		if (SDL_CreateWindowAndRenderer(size.w, size.h, window_flags, &w, &r) != 0)
		{
			renderer = Renderer::FromUnownedPtr(NULL);
			window = Window::FromUnownedPtr(NULL);
			return false;
		}
		renderer = Renderer::FromPtr(r);
		window = Window::FromPtr(w);
		return true;
	}

	constexpr Colour VERY_LIGHT_RED       = { 255, 191, 191, 255 };
	constexpr Colour VERY_LIGHT_ORANGE    = { 255, 223, 191, 255 };
	constexpr Colour VERY_LIGHT_YELLOW    = { 255, 255, 191, 255 };
	constexpr Colour VERY_LIGHT_LIME      = { 223, 255, 191, 255 };
	constexpr Colour VERY_LIGHT_GREEN     = { 191, 255, 191, 255 };
	constexpr Colour VERY_LIGHT_TURQUOISE = { 191, 255, 223, 255 };
	constexpr Colour VERY_LIGHT_CYAN      = { 191, 255, 255, 255 };
	constexpr Colour VERY_LIGHT_AZURE     = { 191, 223, 255, 255 };
	constexpr Colour VERY_LIGHT_BLUE      = { 191, 191, 255, 255 };
	constexpr Colour VERY_LIGHT_VIOLET    = { 223, 191, 255, 255 };
	constexpr Colour VERY_LIGHT_MAGENTA   = { 255, 191, 255, 255 };
	constexpr Colour VERY_LIGHT_RASPBERRY = { 255, 191, 223, 255 };

	constexpr Colour LIGHT_RED            = { 255, 128, 128, 255 };
	constexpr Colour LIGHT_ORANGE         = { 255, 191, 128, 255 };
	constexpr Colour LIGHT_YELLOW         = { 255, 255, 128, 255 };
	constexpr Colour LIGHT_LIME           = { 191, 255, 128, 255 };
	constexpr Colour LIGHT_GREEN          = { 128, 255, 128, 255 };
	constexpr Colour LIGHT_TURQUOISE      = { 128, 255, 191, 255 };
	constexpr Colour LIGHT_CYAN           = { 128, 255, 255, 255 };
	constexpr Colour LIGHT_AZURE          = { 128, 191, 255, 255 };
	constexpr Colour LIGHT_BLUE           = { 128, 128, 255, 255 };
	constexpr Colour LIGHT_VIOLET         = { 191, 128, 255, 255 };
	constexpr Colour LIGHT_MAGENTA        = { 255, 128, 255, 255 };
	constexpr Colour LIGHT_RASPBERRY      = { 255, 128, 191, 255 };

	constexpr Colour RED                  = { 255,   0,   0, 255 };
	constexpr Colour ORANGE               = { 255, 128,   0, 255 };
	constexpr Colour YELLOW               = { 255, 255,   0, 255 };
	constexpr Colour LIME                 = { 128, 255,   0, 255 };
	constexpr Colour GREEN                = {   0, 255,   0, 255 };
	constexpr Colour TURQUOISE            = {   0, 255, 128, 255 };
	constexpr Colour CYAN                 = {   0, 255, 255, 255 };
	constexpr Colour AZURE                = {   0, 128, 255, 255 };
	constexpr Colour BLUE                 = {   0,   0, 255, 255 };
	constexpr Colour VIOLET               = { 128,   0, 255, 255 };
	constexpr Colour MAGENTA              = { 255,   0, 255, 255 };
	constexpr Colour RASPBERRY            = { 255,   0, 128, 255 };

	constexpr Colour DARK_RED             = { 128,   0,   0, 255 };
	constexpr Colour DARK_ORANGE          = { 128,  64,   0, 255 };
	constexpr Colour DARK_YELLOW          = { 128, 128,   0, 255 };
	constexpr Colour DARK_LIME            = {  64, 128,   0, 255 };
	constexpr Colour DARK_GREEN           = {   0, 128,   0, 255 };
	constexpr Colour DARK_TURQUOISE       = {   0, 128,  64, 255 };
	constexpr Colour DARK_CYAN            = {   0, 128, 128, 255 };
	constexpr Colour DARK_AZURE           = {   0,  64, 128, 255 };
	constexpr Colour DARK_BLUE            = {   0,   0, 128, 255 };
	constexpr Colour DARK_VIOLET          = {  64,   0, 128, 255 };
	constexpr Colour DARK_MAGENTA         = { 128,   0, 128, 255 };
	constexpr Colour DARK_RASPBERRY       = { 128,   0,  64, 255 };

	constexpr Colour VERY_DARK_RED        = {  64,   0,   0, 255 };
	constexpr Colour VERY_DARK_ORANGE     = {  64,  32,   0, 255 };
	constexpr Colour VERY_DARK_YELLOW     = {  64,  64,   0, 255 };
	constexpr Colour VERY_DARK_LIME       = {  32,  64,   0, 255 };
	constexpr Colour VERY_DARK_GREEN      = {   0,  64,   0, 255 };
	constexpr Colour VERY_DARK_TURQUOISE  = {   0,  64,  32, 255 };
	constexpr Colour VERY_DARK_CYAN       = {   0,  64,  64, 255 };
	constexpr Colour VERY_DARK_AZURE      = {   0,  32,  64, 255 };
	constexpr Colour VERY_DARK_BLUE       = {   0,   0,  64, 255 };
	constexpr Colour VERY_DARK_VIOLET     = {  32,   0,  64, 255 };
	constexpr Colour VERY_DARK_MAGENTA    = {  64,   0,  64, 255 };
	constexpr Colour VERY_DARK_RASPBERRY  = {  64,   0,  32, 255 };

	constexpr Colour WHITE                = { 255, 255, 255, 255 };
	constexpr Colour VERY_LIGHT_GREY      = { 223, 223, 223, 255 };
	constexpr Colour LIGHT_GREY           = { 191, 191, 191, 255 };
	constexpr Colour GREY                 = { 128, 128, 128, 255 };
	constexpr Colour DARK_GREY            = {  85,  85,  85, 255 };
	constexpr Colour VERY_DARK_GREY       = {  43,  43,  43, 255 };
	constexpr Colour BLACK                = {   0,   0,   0, 255 };

	namespace GL
	{
		/**
		 *  \brief    Bind the texture to the current OpenGL/ES/ES2 context for use with
		 *            OpenGL instructions.
		 *
		 *  \param    texture: The SDL texture to bind
		 *  \param    texsize: A reference to floats that will be filled with the texture size
		 *
		 *  \return   true on success, or false if the operation is not supported
		 */
		inline bool BindTexture(Texture& texture, FPoint& texsize)
			{ return SDL_GL_BindTexture(texture.texture.get(), &texsize.w, &texsize.h) == 0; }

		/**
		 *  \brief    Unbind a texture from the current OpenGL/ES/ES2 context.
		 *
		 *  \param    texture: The SDL texture to unbind
		 *
		 *  \return   true on success, or false if the operation is not supported
		 */
		inline bool UnbindTexture(Texture& texture)
			{ return SDL_GL_UnbindTexture(texture.texture.get()) == 0; }
	}
}

#endif
#endif