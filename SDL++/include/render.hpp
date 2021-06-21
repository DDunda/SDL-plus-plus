#pragma once

#include <tuple>
#include <SDL_render.h>
#include <stack>
#include "rect.hpp"
#include "video.hpp"

namespace SDL {
	struct Texture;

	// \brief A structure representing rendering state
	struct Renderer {
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

		SDL_Renderer* renderer = nullptr;
		bool freeRenderer = false;
		int error = 0;

		Renderer(const Renderer& r);
		Renderer(Renderer&& r) noexcept;
		Renderer& operator=(Renderer that);

		Renderer(SDL_Renderer* renderer = NULL, bool free = false);
		/**
		 *  \brief Create a 2D rendering context for a window.
		 *
		 *  \param window The window where rendering is displayed.
		 *  \param flags    ::SDL_RendererFlags.
		 *  \param index    The index of the rendering driver to initialize, or -1 to
		 *                  initialize the first one supporting the requested flags.
		 *
		 *  \return A valid rendering context or NULL if there was an error.
		 */
		Renderer(Window& window, Uint32 flags, int index = -1);
		/**
		 *  \brief Create a 2D software rendering context for a surface.
		 *
		 *  \param surface The surface where rendering is done.
		 *
		 *  \return A valid rendering context or NULL if there was an error.
		 */
		Renderer(Surface& surface);
		// \brief Get the renderer associated with a window.
		Renderer(Window& window);
		~Renderer();

		// Resets the error
		Renderer& FlushError();

		/**
		 *  \brief Set the drawing scale for rendering on the current target.
		 *
		 *  \param scale The scaling factor
		 *
		 *  The drawing coordinates are scaled by the x/y scaling factors
		 *  before they are used by the renderer.  This allows resolution
		 *  independent drawing with a single coordinate system.
		 *
		 *  \note If this results in scaling or subpixel drawing by the
		 *        rendering backend, it will be handled using the appropriate
		 *        quality hints.  For best results use integer scaling factors.
		 */
		Renderer& SetScale(const FPoint& scale);
		/**
		 *  \brief Set the drawing scale for rendering on the current target.
		 *
		 *  \param scaleX The horizontal scaling factor
		 *  \param scaleY The vertical scaling factor
		 *
		 *  The drawing coordinates are scaled by the x/y scaling factors
		 *  before they are used by the renderer.  This allows resolution
		 *  independent drawing with a single coordinate system.
		 *
		 *  \note If this results in scaling or subpixel drawing by the
		 *        rendering backend, it will be handled using the appropriate
		 *        quality hints.  For best results use integer scaling factors.
		 */
		Renderer& SetScale(float scaleX, float scaleY);

		/**
		 *  \brief Get the drawing scale for the current target.
		 *
		 *  \return The scaling factor
		 */
		FPoint GetScale();
		/**
		 *  \brief Get the drawing scale for the current target.
		 *
		 *  \param scale  A reference to be filled in with the scaling factor
		 */
		Renderer& GetScale(FPoint& scale);
		/**
		 *  \brief Get the drawing scale for the current target.
		 *
		 *  \param scaleX A reference to be filled in with the horizontal scaling factor
		 *  \param scaleY A reference to be filled in with the vertical scaling factor\
		 */
		Renderer& GetScale(float& scaleX, float& scaleY);

		/**
		 *  \brief    Set the colour used for drawing operations (Rect, Line and Clear).
		 *
		 *  \param    colour: The colour value used to draw on the rendering target.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& SetDrawColour(const Colour& colour);
		/**
		 *  \brief    Set the colour used for drawing operations (Rect, Line and Clear).
		 *
		 *  \param r The red value used to draw on the rendering target.
		 *  \param g The green value used to draw on the rendering target.
		 *  \param b The blue value used to draw on the rendering target.
		 *  \param a The alpha value used to draw on the rendering target, usually
		 *           ::SDL_ALPHA_OPAQUE (255).
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& SetDrawColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
		/**
		 *  \brief    Get the colour used for drawing operations (Rect, Line and Clear).
		 *
		 *  \param    colour: A reference to be filled with the colour used to draw on the rendering target.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& GetDrawColour(Colour& colour);
		/**
		 *  \brief    Get the colour used for drawing operations (Rect, Line and Clear).
		 *
		 *  \param r A reference to be filled with the red value used to draw on the rendering target.
		 *  \param g A reference to be filled with the green value used to draw on the rendering target.
		 *  \param b A reference to be filled with the blue value used to draw on the rendering target.
		 *  \param a A reference to be filled with the alpha value used to draw on the rendering target,
		 *           usually ::SDL_ALPHA_OPAQUE (255).
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& GetDrawColour(Uint8& r, Uint8& g, Uint8& b, Uint8& a);

		/**
		 *  \brief Set the blend mode used for drawing operations (Fill and Line).
		 *
		 *  \param blendMode SDL::BlendMode to use for blending.
		 *
		 *  \return 0 on success, or -1 on error
		 *
		 *  \note If the blend mode is not supported, the closest supported mode is
		 *        chosen.
		 */
		Renderer& SetDrawBlendMode(const BlendMode& blendMode);
		/**
		 *  \brief Get the blend mode used for drawing operations.
		 *
		 *  \param blendMode A reference to be filled in with the current blend mode.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& GetDrawBlendMode(BlendMode& blendMode);

		/**
		 *  \brief    Clear the current rendering target with the drawing colour
		 *
		 *  This function clears the entire rendering target, ignoring the viewport and
		 *  the clip rectangle.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& Clear();
		/**
		 *  \brief Force the rendering context to flush any pending commands to the
		 *         underlying rendering API.
		 *
		 *  You do not need to (and in fact, shouldn't) call this function unless
		 *  you are planning to call into OpenGL/Direct3D/Metal/whatever directly
		 *  in addition to using an SDL_Renderer.
		 *
		 *  This is for a very-specific case: if you are using SDL's render API,
		 *  you asked for a specific renderer backend (OpenGL, Direct3D, etc),
		 *  you set SDL_HINT_RENDER_BATCHING to "1", and you plan to make
		 *  OpenGL/D3D/whatever calls in addition to SDL render API calls. If all of
		 *  this applies, you should call SDL_RenderFlush() between calls to SDL's
		 *  render API and the low-level API you're using in cooperation.
		 *
		 *  In all other cases, you can ignore this function. This is only here to
		 *  get maximum performance out of a specific situation. In all other cases,
		 *  SDL will do the right thing, perhaps at a performance loss.
		 *
		 *  This function is first available in SDL 2.0.10, and is not needed in
		 *  2.0.9 and earlier, as earlier versions did not queue rendering commands
		 *  at all, instead flushing them to the OS immediately.
		 */
		Renderer& Flush();
		// \brief Update the screen with rendering performed.
		Renderer& Present();

		/**
		 *  \brief Draw a line on the current rendering target.
		 *
		 *  \param a The coordinates of the start point.
		 *  \param b The coordinates of the end point.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawLine(const Point& a, const Point& b);
		/**
		 *  \brief Draw a line on the current rendering target.
		 *
		 *  \param a The coordinates of the start point.
		 *  \param b The coordinates of the end point.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawLineF(const FPoint& a, const FPoint& b);
		/**
		 *  \brief Draw a series of connected lines on the current rendering target.
		 *
		 *  \param points The points along the lines
		 *  \param count The number of points, drawing count-1 lines
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawLines(const Point* points, int count);
		/**
		 *  \brief Draw a series of connected lines on the current rendering target.
		 *
		 *  \param points The points along the lines
		 *  \param count The number of points, drawing count-1 lines
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawLinesF(const FPoint* points, int count);
		/**
		 *  \brief Draw a series of connected lines on the current rendering target.
		 *
		 *  \param points The points along the lines
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawLines(const std::vector<Point>& points);
		/**
		 *  \brief Draw a series of connected lines on the current rendering target.
		 *
		 *  \param points The points along the lines
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawLinesF(const std::vector<FPoint>& points);

		/**
		 *  \brief Draw a point on the current rendering target.
		 *
		 *  \param point The coordinates of the point.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawPoint(const Point& point);
		/**
		 *  \brief Draw a point on the current rendering target.
		 *
		 *  \param point The coordinates of the point.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawPointF(const FPoint& point);
		/**
		 *  \brief Draw multiple points on the current rendering target.
		 *
		 *  \param renderer The renderer which should draw multiple points.
		 *  \param points The points to draw
		 *  \param count The number of points to draw
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawPoints(const Point* points, int count);
		/**
		 *  \brief Draw multiple points on the current rendering target.
		 *
		 *  \param points The points to draw
		 *  \param count The number of points to draw
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawPointsF(const FPoint* points, int count);
		/**
		 *  \brief Draw multiple points on the current rendering target.
		 *
		 *  \param points The points to draw
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawPoints(const std::vector<Point>& points);
		/**
		 *  \brief Draw multiple points on the current rendering target.
		 *
		 *  \param points The points to draw
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawPointsF(const std::vector<FPoint>& points);

		/**
		 *  \brief Draws a rectangle outlining current rendering target.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawOutline();
		/**
		 *  \brief Draws a rectangle outlining current rendering target.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawOutlineF();
		/**
		 *  \brief Draw a rectangle on the current rendering target.
		 *
		 *  \param rect A reference to the destination rectangle.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawRect(const Rect& rect);
		/**
		 *  \brief Draw a rectangle on the current rendering target.
		 *
		 *  \param rect A pointer to the destination rectangle, or NULL to outline the entire rendering target.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawRect(const Rect* rect);
		/**
		 *  \brief Draw a rectangle on the current rendering target.
		 *
		 *  \param rect A reference to the destination rectangle.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawRectF(const FRect& rect);
		/**
		 *  \brief Draw a rectangle on the current rendering target.
		 *
		 *  \param rect A pointer to the destination rectangle, or NULL to outline the entire rendering target.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawRectF(const FRect* rect);
		/**
		 *  \brief Draw some number of rectangles on the current rendering target.
		 *
		 *  \param rects A pointer to an array of destination rectangles.
		 *  \param count The number of rectangles.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawRects(const Rect* rects, int count);
		/**
		 *  \brief Draw some number of rectangles on the current rendering target.
		 *
		 *  \param rects A pointer to an array of destination rectangles.
		 *  \param count The number of rectangles.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawRectsF(const FRect* rects, int count);
		/**
		 *  \brief Draw some number of rectangles on the current rendering target.
		 *
		 *  \param rects A reference to a vector of destination rectangles.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawRects(const std::vector<Rect>& rects);
		/**
		 *  \brief Draw some number of rectangles on the current rendering target.
		 *
		 *  \param rects A reference to a vector of destination rectangles.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DrawRectsF(const std::vector<FRect>& rects);

		/**
		*  \brief     Fill the entire current rendering target with the drawing colour.
		*
		*  \return 0 on success, or -1 on error
		*/
		Renderer& Fill();
		/**
		 *  \brief    Fill the entire current rendering target with the drawing colour.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& FillF();
		/**
		 *  \brief    Fill a rectangle on the current rendering target with the drawing colour.
		 *
		 *  \param rect A pointer to the destination rectangle.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& FillRect(const Rect& rect);
		/**
		 *  \brief    Fill a rectangle on the current rendering target with the drawing colour.
		 *
		 *  \param rect A pointer to the destination rectangle, or NULL for the entire
		 *              rendering target.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& FillRect(const Rect* rect);
		/**
		 *  \brief    Fill a rectangle on the current rendering target with the drawing colour.
		 *
		 *  \param rect A reference to the destination rectangle.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& FillRectF(const FRect& rect);
		/**
		 *  \brief    Fill a rectangle on the current rendering target with the drawing colour.
		 *
		 *  \param rect A pointer to the destination rectangle, or NULL for the entire
		 *              rendering target.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& FillRectF(const FRect* rect);
		/**
		 *  \brief    Fill some number of rectangles on the current rendering target with the drawing colour.
		 *
		 *  \param rects A pointer to an array of destination rectangles.
		 *  \param count The number of rectangles.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& FillRects(const Rect* rects, int count);
		/**
		 *  \brief    Fill some number of rectangles on the current rendering target with the drawing colour.
		 *
		 *  \param rects A pointer to an array of destination rectangles.
		 *  \param count The number of rectangles.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& FillRectsF(const FRect* rects, int count);
		/**
		 *  \brief    Fill some number of rectangles on the current rendering target with the drawing colour.
		 *
		 *  \param rects A reference to a vector of destination rectangles.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& FillRects(const std::vector<Rect>& rects);
		/**
		 *  \brief    Fill some number of rectangles on the current rendering target with the drawing colour.
		 *
		 *  \param rects A reference to a vector of destination rectangles.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& FillRectsF(const std::vector<FRect>& rects);

		/**
		 *  \brief Get the number of 2D rendering drivers available for the current
		 *         display.
		 *
		 *  A render driver is a set of code that handles rendering and texture
		 *  management on a particular display.  Normally there is only one, but
		 *  some drivers may have several available with different capabilities.
		 */
		static int GetNumDrivers();
		/**
		 *  \brief Get information about a specific 2D rendering driver for the current
		 *         display.
		 *
		 *  \param info  A reference to an Info struct to be filled with
		 *               information on the rendering driver.
		 *
		 *  \return 0 on success, -1 if the index was out of range.
		 */
		Renderer& GetDriverInfo(int index, Info& info);

		// \brief Get information about a rendering context.
		Renderer& GetInfo(Info& info);

		// \brief Get the output size in pixels of a rendering context.
		Point GetOutputSize();
		// \brief Get the output size in pixels of a rendering context.
		Renderer& GetOutputSize(Point& size);
		// \brief Get the output size in pixels of a rendering context.
		Renderer& GetOutputSize(int& w, int& h);

		/**
		 * \brief Determines whether a window supports the use of render targets
		 *
		 * \return true if supported, false if not.
		 */
		bool TargetSupported();
		/**
		 * \brief Determines whether a window supports the use of render targets
		 *
		 * \param support A reference to be filled with true if supported, false if not.
		 */
		Renderer& TargetSupported(bool& support);

		/**
		 * \brief Set a texture as the current rendering target.
		 *
		 * \param texture The targeted texture, which must be created with the SDL_TEXTUREACCESS_TARGET flag, or NULL for the default render target
		 *
		 * \return 0 on success, or -1 on error
		 */
		Renderer& SetTarget(Texture& texture);
		/**
		 * \brief Get the current render target or NULL for the default render target.
		 *
		 * \return The current render target
		 */
		Texture GetTarget();
		/**
		 * \brief Get the current render target or NULL for the default render target.
		 *
		 * \return The current render target
		 */
		Renderer& GetTarget(Texture& target);

		/**
		 *  \brief Set device independent resolution for rendering
		 *
		 *  \param size   The size of the logical resolution
		 *
		 *  This function uses the viewport and scaling functionality to allow a fixed logical
		 *  resolution for rendering, regardless of the actual output resolution.  If the actual
		 *  output resolution doesn't have the same aspect ratio the output rendering will be
		 *  centered within the output display.
		 *
		 *  If the output display is a window, mouse events in the window will be filtered
		 *  and scaled so they seem to arrive within the logical resolution.
		 *
		 *  \note If this function results in scaling or subpixel drawing by the
		 *        rendering backend, it will be handled using the appropriate
		 *        quality hints.
		 */
		Renderer& SetLogicalSize(const Point& size);
		/**
		 *  \brief Set device independent resolution for rendering
		 *
		 *  \param w      The width of the logical resolution
		 *  \param h      The height of the logical resolution
		 *
		 *  This function uses the viewport and scaling functionality to allow a fixed logical
		 *  resolution for rendering, regardless of the actual output resolution.  If the actual
		 *  output resolution doesn't have the same aspect ratio the output rendering will be
		 *  centered within the output display.
		 *
		 *  If the output display is a window, mouse events in the window will be filtered
		 *  and scaled so they seem to arrive within the logical resolution.
		 *
		 *  \note If this function results in scaling or subpixel drawing by the
		 *        rendering backend, it will be handled using the appropriate
		 *        quality hints.
		 */
		Renderer& SetLogicalSize(int w, int h);

		/**
		 *  \brief Get device independent resolution for rendering
		 *
		 *  \return       The size of the logical resolution
		 */
		Point GetLogicalSize();
		/**
		 *  \brief Get device independent resolution for rendering
		 *
		 *  \param size   A reference filled with the size of the logical resolution
		 */
		Renderer& GetLogicalSize(Point& size);
		/**
		 *  \brief Get device independent resolution for rendering
		 *
		 *  \param w      A reference to be filled with the width of the logical resolution
		 *  \param h      A reference to be filled with the height of the logical resolution
		 */
		Renderer& GetLogicalSize(int& w, int& h);

		/**
		 *  \brief Set whether to force integer scales for resolution-independent rendering
		 *
		 *  \param enable   Enable or disable integer scaling
		 *
		 *  This function restricts the logical viewport to integer values - that is, when
		 *  a resolution is between two multiples of a logical size, the viewport size is
		 *  rounded down to the lower multiple.
		 */
		Renderer& SetIntegerScale(bool enable);
		// brief Get whether integer scales are forced for resolution-independent rendering
		bool GetIntegerScale();
		// brief Get whether integer scales are forced for resolution-independent rendering
		Renderer& GetIntegerScale(bool& enabled);

		/**
		 *  \brief Set the drawing area for rendering on the current target.
		 *
		 *  \param rect The rectangle representing the drawing area.
		 *
		 *  The x,y of the viewport rect represents the origin for rendering.
		 *
		 *  \return 0 on success, or -1 on error
		 *
		 *  \note If the window associated with the renderer is resized, the viewport is automatically reset.
		 */
		Renderer& SetViewport(const Rect& rect);
		/**
		 *  \brief Sets the drawing area for rendering to the current target.
		 *
		 *  \return 0 on success, or -1 on error
		 *
		 *  \note If the window associated with the renderer is resized, the viewport is automatically reset.
		 */
		Renderer& FillViewport();

		// \brief Get the drawing area for the current target.
		Rect GetViewport();
		// \brief Get the drawing area for the current target.
		Renderer& GetViewport(Rect& rect);

		/**
		 *  \brief Set the clip rectangle for the current target.
		 *
		 *  \param rect   A reference to the rectangle to set as the clip rectangle,
		 *                relative to the viewport
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& SetClipRect(const Rect& rect);
		/**
		 *  \brief Disables clipping for the current target.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		Renderer& DisableClip();

		/**
		 *  \brief Get the clip rectangle for the current target.
		 * 
		 *  \return         A rect filled in with the current clip rectangle, or
		 *                an empty rectangle if clipping is disabled.
		 */
		Rect GetClipRect();
		/**
		 *  \brief Get the clip rectangle for the current target.
		 *
		 *  \param rect    A reference filled in with the current clip rectangle, or
		 *                an empty rectangle if clipping is disabled.
		 */
		Renderer& GetClipRect(Rect& rect);

		// \brief Get whether clipping is enabled on the given renderer.
		bool IsClipEnabled();
		// \brief Get whether clipping is enabled on the given renderer.
		Renderer& IsClipEnabled(bool& enabled);

		/**
		 *  \brief Read pixels from the current rendering target.
		 *
		 *  \param rect   A reference to the rectangle to read
		 *  \param pixels A pointer to be filled in with the pixel data
		 *  \param pitch  The pitch of the pixels parameter.
		 *  \param format The desired format of the pixel data, or 0 to use the format
		 *                of the rendering target
		 *
		 *  \return 0 on success, or -1 if pixel reading is not supported.
		 *
		 *  \warning This is a very slow operation, and should not be used frequently.
		 */
		Renderer& ReadPixelsRect(const Rect& rect, void* pixels, int pitch, Uint32 format = 0);
		/**
		 *  \brief Read pixels from the entire current rendering target.
		 *
		 *  \param pixels A pointer to be filled in with the pixel data
		 *  \param pitch  The pitch of the pixels parameter.
		 *  \param format The desired format of the pixel data, or 0 to use the format
		 *                of the rendering target
		 *
		 *  \return 0 on success, or -1 if pixel reading is not supported.
		 *
		 *  \warning This is a very slow operation, and should not be used frequently.
		 */
		Renderer& ReadPixels(void* pixels, int pitch, Uint32 format = 0);

		/**
		 *  \brief Get the CAMetalLayer associated with the given Metal renderer
		 *
		 *  \return CAMetalLayer* on success, or NULL if the renderer isn't a Metal renderer
		 */
		void* GetMetalLayer();
		/**
		 *  \brief Get the Metal command encoder for the current frame
		 *
		 *  \return id<MTLRenderCommandEncoder> on success, or NULL if the renderer isn't a Metal renderer
		 */
		void* GetMetalCommandEncoder();
	};

	// \brief An efficient driver-specific representation of pixel data
	struct Texture {
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
			NONE = SDL_TEXTUREMODULATE_NONE,     // No modulation
			COLOR = SDL_TEXTUREMODULATE_COLOR,    // srcC = srcC * color
			ALPHA = SDL_TEXTUREMODULATE_ALPHA     // srcA = srcA * alpha
		};
		// Flip constants for CopyEx
		enum class Flip {
			NONE       = SDL_FLIP_NONE,       // Do not flip
			HORIZONTAL = SDL_FLIP_HORIZONTAL, // flip horizontally
			VERTICAL   = SDL_FLIP_VERTICAL    // flip vertically
		};

		Renderer& renderer;
		SDL_Texture* texture;
		bool freeTexture = false;

		Texture(Texture& txt);
		Texture(Texture&& txt) noexcept;
		Texture& operator=(Texture that);

		Texture(Renderer& renderer, SDL_Texture* texture, bool free = true);
		/**
		 *  \brief Create a texture for a rendering context.
		 *
		 *  \param renderer The renderer.
		 *  \param size   The size of the texture in pixels.
		 *  \param access One of the enumerated values in ::Access.
		 *  \param format The format of the texture.
		 *
		 *  \return The created texture is returned, or NULL if no rendering context was
		 *          active,  the format was unsupported, or the width or height were out
		 *          of range.
		 *
		 *  \note The contents of the texture are not defined at creation.
		 */
		Texture(Renderer& renderer, const Point& size, Access access = Access::STATIC, Uint32 format = (Uint32)PixelFormatEnum::RGBA32);
		/**
		 *  \brief Create a texture from an existing surface.
		 *
		 *  \param renderer The renderer.
		 *  \param surface The surface containing pixel data used to fill the texture.
		 *
		 *  \return The created texture is returned, or NULL on error.
		 *
		 *  \note The surface is not modified or freed by this function.
		 */
		Texture(Renderer& renderer, Surface& surface);

		~Texture();

		/**
		 *  \brief Lock a portion of the texture for write-only pixel access.
		 *
		 *  \param rect      A reference to the rectangle to lock for access.
		 *  \param pixels    This is filled in with a pointer to the locked pixels,
		 *                   appropriately offset by the locked area.
		 *  \param pitch     This is filled in with the pitch of the locked pixels.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid or was not created with ::SDL_TEXTUREACCESS_STREAMING.
		 */
		int LockRect(const Rect& rect, void*& pixels, int& pitch);
		/**
		 *  \brief Lock the entire texture for write-only pixel access.
		 *
		 *  \param pixels    This is filled in with a pointer to the locked pixels,
		 *                   appropriately offset by the locked area.
		 *  \param pitch     This is filled in with the pitch of the locked pixels.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid or was not created with ::SDL_TEXTUREACCESS_STREAMING.
		 */
		int Lock(void*& pixels, int& pitch);

		/**
		 *  \brief Lock a portion of the texture for write-only pixel access.
		 *         Expose it as a SDL surface.
		 *
		 *  \param rect      A reference to the rectangle to lock for access.
		 *  \param surface   This is filled in with a SDL surface representing the locked area
		 *                   Surface is freed internally after calling UnlockTexture or deleting the Surface
		 *
		 *  \return 0 on success, or -1 if the texture is not valid or was not created with ::SDL_TEXTUREACCESS_STREAMING.
		 */
		int LockRectToSurface(const Rect& rect, Surface& surface);
		/**
		 *  \brief Lock the entire texture for write-only pixel access.
		 *         Expose it as a SDL surface.
		 *
		 *  \param surface   This is filled in with a SDL surface representing the locked area
		 *                   Surface is freed internally after calling UnlockTexture or deleting the Surface
		 *
		 *  \return 0 on success, or -1 if the texture is not valid or was not created with ::SDL_TEXTUREACCESS_STREAMING.
		 */
		int LockToSurface(Surface& surface);

		/**
		 *  \brief Unlock a texture, uploading the changes to video memory, if needed.
		 *         If LockToSurface() was called for locking, the SDL surface is freed.
		 */
		void Unlock();

		/**
		 *  \brief Update the given texture rectangle with new pixel data.
		 *
		 *  \param rect      A reference to the rectangle of pixels to update.
		 *  \param pixels    The raw pixel data in the format of the texture.
		 *  \param pitch     The number of bytes in a row of pixel data, including padding between lines.
		 *
		 *  The pixel data must be in the format of the texture. The pixel format can be
		 *  queried with Query or QueryFormat.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid.
		 *
		 *  \note This is a fairly slow function.
		 */
		int UpdateRect(const Rect& rect, void* pixels, int pitch);
		/**
		 *  \brief Update the entire texture with new pixel data.
		 *
		 *  \param pixels    The raw pixel data in the format of the texture.
		 *  \param pitch     The number of bytes in a row of pixel data, including padding between lines.
		 *
		 *  The pixel data must be in the format of the texture. The pixel format can be
		 *  queried with Query or QueryFormat.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid.
		 *
		 *  \note This is a fairly slow function.
		 */
		int Update(void* pixels, int pitch);

		/**
		 *  \brief Update a rectangle within a planar YV12 or IYUV texture with new pixel data.
		 *
		 *  \param rect      A reference to the rectangle of pixels to update.
		 *  \param Yplane    The raw pixel data for the Y plane.
		 *  \param Ypitch    The number of bytes between rows of pixel data for the Y plane.
		 *  \param Uplane    The raw pixel data for the U plane.
		 *  \param Upitch    The number of bytes between rows of pixel data for the U plane.
		 *  \param Vplane    The raw pixel data for the V plane.
		 *  \param Vpitch    The number of bytes between rows of pixel data for the V plane.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid.
		 *
		 *  \note You can use UpdateRect() as long as your pixel data is
		 *        a contiguous block of Y and U/V planes in the proper order, but
		 *        this function is available if your pixel data is not contiguous.
		 */
		int UpdateYUVRect(const Rect& rect, const Uint8* Yplane, int Ypitch, const Uint8* Uplane, int Upitch, const Uint8* Vplane, int Vpitch);
		/**
		 *  \brief Update a the entire planar YV12 or IYUV texture with new pixel data.
		 *
		 *  \param Yplane    The raw pixel data for the Y plane.
		 *  \param Ypitch    The number of bytes between rows of pixel data for the Y plane.
		 *  \param Uplane    The raw pixel data for the U plane.
		 *  \param Upitch    The number of bytes between rows of pixel data for the U plane.
		 *  \param Vplane    The raw pixel data for the V plane.
		 *  \param Vpitch    The number of bytes between rows of pixel data for the V plane.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid.
		 *
		 *  \note You can use Update() as long as your pixel data is
		 *        a contiguous block of Y and U/V planes in the proper order, but
		 *        this function is available if your pixel data is not contiguous.
		 */
		int UpdateYUV(const Uint8* Yplane, int Ypitch, const Uint8* Uplane, int Upitch, const Uint8* Vplane, int Vpitch);

		/**
		 *  \brief Copy a portion of the texture to the current rendering target.
		 *
		 *  \param srcrect   A reference to the source rectangle.
		 *  \param dstrect   A reference to the destination rectangle.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int Copy(const Rect& src, const Rect& dst);
		/**
		 *  \brief Copy the texture to the current rendering target.
		 *
		 *  \param dstrect   A reference to the destination rectangle.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int Copy(const Rect& dst);
		/**
		 *  \brief Copy a portion of the texture to the entire current rendering target.
		 *
		 *  \param srcrect   A reference to the source rectangle.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int Copy_Fill(const Rect& src);
		/**
		 *  \brief Copy the texture to the entire current rendering target.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int Copy_Fill();
		/**
		 *  \brief Copy a portion of the texture to the current rendering target.
		 *
		 *  \param srcrect   A pointer to the source rectangle, or NULL for the entire
		 *                   texture.
		 *  \param dstrect   A pointer to the destination rectangle, or NULL for the
		 *                   entire rendering target.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int Copy(const Rect* src, const Rect* dst);

		/**
		 *  \brief Copy a portion of the texture to the current rendering target.
		 *
		 *  \param srcrect   A reference to the source rectangle.
		 *  \param dstrect   A reference to the destination rectangle.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyF(const Rect& src, const FRect& dst);
		/**
		 *  \brief Copy the texture to the current rendering target.
		 *
		 *  \param dstrect   A reference to the destination rectangle.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyF(const FRect& dst);
		/**
		 *  \brief Copy a portion of the texture to the entire current rendering target.
		 *
		 *  \param srcrect   A reference to the source rectangle.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyF_Fill(const Rect& src);
		/**
		 *  \brief Copy the texture to the entire current rendering target.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyF_Fill();
		/**
		 *  \brief Copy a portion of the texture to the current rendering target.
		 *
		 *  \param srcrect   A pointer to the source rectangle, or NULL for the entire
		 *                   texture.
		 *  \param dstrect   A pointer to the destination rectangle, or NULL for the
		 *                   entire rendering target.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyF(const Rect* src, const FRect* dst);

		/**
		 *  \brief Copy a portion of the texture to the current rendering target, rotating it by an angle around the given center.
		 *
		 *  \param srcrect  A reference to the source rectangle.
		 *  \param dstrect  A reference to the destination rectangle.
		 *  \param center   A reference to a point indicating the point around which dstrect will be rotated.
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param flip     A Renderer::Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyEx(const Rect& src, const Rect& dst, const Point& center, double angle = 0.0, Flip flipType = Flip::SDL_FLIP_NONE);
		/**
		 *  \brief Copy a portion of the texture to the current rendering target, rotating it by an angle around dstrect.w/2, dstrect.h/2.
		 *
		 *  \param srcrect  A reference to the source rectangle.
		 *  \param dstrect  A reference to the destination rectangle.
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param flip     A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyEx(const Rect& src, const Rect& dst, double angle = 0.0, Flip flipType = Flip::SDL_FLIP_NONE);
		/**
		 *  \brief Copy the texture to the current rendering target, rotating it by an angle around the given center.
		 *
		 *  \param dstrect  A reference to the destination rectangle.
		 *  \param center   A reference to a point indicating the point around which dstrect will be rotated (if NULL, rotation will be done around dstrect.w/2, dstrect.h/2).
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param flip     A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyEx(const Rect& dst, const Point& center, double angle = 0.0, Flip flipType = Flip::SDL_FLIP_NONE);
		/**
		 *  \brief Copy the texture to the current rendering target, rotating it by an angle around dstrect.w/2, dstrect.h/2
		 *
		 *  \param dstrect   A reference to the destination rectangle.
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param flip     A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyEx(const Rect& dst, double angle = 0.0, Flip flipType = Flip::SDL_FLIP_NONE);
		/**
		 *  \brief Copy a portion of the texture to the entire current rendering target, rotating it by an angle around the given center
		 *
		 *  \param srcrect  A reference to the source rectangle.
		 *  \param center   A reference to a point indicating the point around which dstrect will be rotated.
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param flip     A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyEx_Fill(const Rect& src, const Point& center, double angle = 0.0, Flip flipType = Flip::NONE);
		/**
		 *  \brief Copy a portion of the texture to the entire current rendering target, rotating it by an angle around dstrect.w/2, dstrect.h/2.
		 *
		 *  \param srcrect  A reference to the source rectangle.
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param flip     A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyEx_Fill(const Rect& src, double angle = 0.0, Flip flipType = Flip::NONE);
		/**
		 *  \brief Copy the texture to the entire current rendering target, rotating it by an angle around the given center.
		 *
		 *  \param center   A reference to a point indicating the point around which dstrect will be rotated.
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param flip     A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyEx_Fill(const Point& center, double angle = 0.0, Flip flipType = Flip::NONE);
		/**
		 *  \brief Copy the texture to the entire current rendering target, rotating it by an angle around dstrect.w/2, dstrect.h/2.
		 *
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param flip     A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyEx_Fill(double angle = 0.0, Flip flipType = Flip::NONE);
		/**
		 *  \brief Copy a portion of the texture to the current rendering target, rotating it by angle around the given center.
		 *
		 *  \param srcrect  A pointer to the source rectangle, or NULL for the entire texture.
		 *  \param dstrect  A pointer to the destination rectangle, or NULL for the entire rendering target.
		 *  \param center   A pointer to a point indicating the point around which dstrect will be rotated (if NULL, rotation will be done around dstrect.w/2, dstrect.h/2).
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction.
		 *  \param flip     An Flip value stating which flipping actions should be performed on the texture.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyEx(const Rect* src, const Rect* dst, const Point* center, double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief Copy a portion of the texture to the current rendering target, rotating it by an angle around the given center.
		 *
		 *  \param srcrect  A reference to the source rectangle.
		 *  \param dstrect  A reference to the destination rectangle.
		 *  \param center   A reference to a point indicating the point around which dstrect will be rotated.
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param flip     A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyExF(const Rect& src, const FRect& dst, const FPoint& center, double angle = 0.0, Flip flipType = Flip::NONE);
		/**
		 *  \brief Copy a portion of the texture to the current rendering target, rotating it by an angle around dstrect.w/2, dstrect.h/2.
		 *
		 *  \param srcrect  A reference to the source rectangle.
		 *  \param dstrect  A reference to the destination rectangle.
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param flip     A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyExF(const Rect& src, const FRect& dst, double angle = 0.0, Flip flipType = Flip::NONE);
		/**
		 *  \brief Copy the texture to the current rendering target, rotating it by an angle around the given center.
		 *
		 *  \param dstrect  A reference to the destination rectangle.
		 *  \param center   A reference to a point indicating the point around which dstrect will be rotated (if NULL, rotation will be done around dstrect.w/2, dstrect.h/2).
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param flip     A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyExF(const FRect& dst, const FPoint& center, double angle = 0.0, Flip flipType = Flip::NONE);
		/**
		 *  \brief Copy the texture to the current rendering target, rotating it by an angle around dstrect.w/2, dstrect.h/2
		 *
		 *  \param dstrect   A reference to the destination rectangle.
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param flip     A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyExF(const FRect& dst, double angle = 0.0, Flip flipType = Flip::NONE);
		/**
		 *  \brief Copy a portion of the texture to the entire current rendering target, rotating it by an angle around the given center
		 *
		 *  \param srcrect  A reference to the source rectangle.
		 *  \param center   A reference to a point indicating the point around which dstrect will be rotated.
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param flip     A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyExF_Fill(const Rect& src, const FPoint& center, double angle = 0.0, Flip flipType = Flip::NONE);
		/**
		 *  \brief Copy a portion of the texture to the entire current rendering target, rotating it by an angle around dstrect.w/2, dstrect.h/2.
		 *
		 *  \param srcrect  A reference to the source rectangle.
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param flip     A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyExF_Fill(const Rect& src, double angle = 0.0, Flip flipType = Flip::NONE);
		/**
		 *  \brief Copy the texture to the entire current rendering target, rotating it by an angle around the given center.
		 *
		 *  \param center   A reference to a point indicating the point around which dstrect will be rotated.
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param flip     A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyExF_Fill(const FPoint& center, double angle = 0.0, Flip flipType = Flip::NONE);
		/**
		 *  \brief Copy the texture to the entire current rendering target, rotating it by an angle around dstrect.w/2, dstrect.h/2.
		 *
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
		 *  \param flip     A Flip value stating which flipping actions should be performed on the texture
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyExF_Fill(double angle = 0.0, Flip flipType = Flip::NONE);
		/**
		 *  \brief Copy a portion of the texture to the current rendering target, rotating it by angle around the given center.
		 *
		 *  \param srcrect  A pointer to the source rectangle, or NULL for the entire texture.
		 *  \param dstrect  A pointer to the destination rectangle, or NULL for the entire rendering target.
		 *  \param center   A pointer to a point indicating the point around which dstrect will be rotated (if NULL, rotation will be done around dstrect.w/2, dstrect.h/2).
		 *  \param angle    An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction.
		 *  \param flip     An Flip value stating which flipping actions should be performed on the texture.
		 *
		 *  \return 0 on success, or -1 on error
		 */
		int CopyExF(const Rect* src, const FRect* dst, const FPoint* center, double angle = 0.0, Flip flipType = Flip::NONE);

		/**
		 *  \brief Query the format of a texture
		 *
		 *  \param format  A reference to be filled in with the raw format of the texture.  The
		 *                 actual format may differ, but pixel transfers will use this
		 *                 format.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid.
		 */
		int QueryFormat(Uint32& format);
		/**
		 *  \brief Query the access of a texture
		 *
		 *  \param access  A reference to be filled in with the actual access to the texture.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid.
		 */
		int QueryAccess(int& access);
		/**
		 *  \brief Query the size of a texture
		 *
		 *  \param size    A reference to be filled in with the width and height of the texture in pixels.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid.
		 */
		int QuerySize(Point& size);
		/**
		 *  \brief Query the attributes of a texture
		 *
		 *  \param format  A reference to be filled in with the raw format of the texture.  The
		 *                 actual format may differ, but pixel transfers will use this
		 *                 format.
		 *  \param access  A reference to be filled in with the actual access to the texture.
		 *  \param size    A reference to be filled in with the width and height of the texture in pixels.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid.
		 */
		int Query(Uint32& format, int& access, Point& size);
		/**
		 *  \brief Query the attributes of a texture
		 *
		 *  \param format  A pointer to be filled in with the raw format of the texture.  The
		 *                 actual format may differ, but pixel transfers will use this
		 *                 format.
		 *  \param access  A pointer to be filled in with the actual access to the texture.
		 *  \param w       A pointer to be filled in with the width of the texture in pixels.
		 *  \param h       A pointer to be filled in with the height of the texture in pixels.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid.
		 */
		int Query(Uint32* format, int* access, int* w, int* h);

		/**
		 *  \brief    Set an additional colour value used in render copy operations.
		 *
		 *  \param    r: The red colour value multiplied into copy operations.
		 *  \param    g: The green colour value multiplied into copy operations.
		 *  \param    b: The blue colour value multiplied into copy operations.
		 *
		 *  \return   0 on success, or -1 if the texture is not valid or colour modulation
		 *            is not supported.
		 */
		int SetColourMod(Uint8 r, Uint8 g, Uint8 b);
		/**
		 *  \brief    Get the additional colour value used in render copy operations.
		 *
		 *  \param    r: A reference to be filled in with the current red colour value.
		 *  \param    g: A reference to be filled in with the current green colour value.
		 *  \param    b: A reference to be filled in with the current blue colour value.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid.
		 */
		int GetColourMod(Uint8& r, Uint8& g, Uint8& b);

		/**
		 *  \brief Set an additional alpha value used in render copy operations.
		 *
		 *  \param alpha     The alpha value multiplied into copy operations.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid or alpha modulation
		 *          is not supported.
		 */
		int SetAlphaMod(Uint8 alpha);
		/**
		 *  \brief Get the additional alpha value used in render copy operations.
		 *
		 *  \param alpha     A reference to be filled in with the current alpha value.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid.
		 */
		int GetAlphaMod(Uint8& alpha);

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
		int SetMod(const Colour& c);
		/**
		 *  \brief    Get the additional colour value used in render copy operations.
		 *
		 *  \param    r:     A reference to be filled in with the current red colour value.
		 *  \param    g:     A reference to be filled in with the current green colour value.
		 *  \param    b:     A reference to be filled in with the current blue colour value.
		 *  \param    alpha: A reference to be filled in with the current alpha value.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid.
		 */
		int GetMod(Colour& c);

		/**
		 *  \brief Set the blend mode used for texture copy operations.
		 *
		 *  \param blendMode ::BlendMode to use for texture blending.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid or the blend mode is
		 *          not supported.
		 *
		 *  \note If the blend mode is not supported, the closest supported mode is
		 *        chosen.
		 */
		int SetBlendMode(BlendMode blendMode);
		/**
		 *  \brief Get the blend mode used for texture copy operations.
		 *
		 *  \param blendMode A reference to be filled in with the current blend mode.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid.
		 */
		int GetBlendMode(BlendMode& blendMode);

		/**
		 *  \brief Set the scale mode used for texture scale operations.
		 *
		 *  \param scaleMode ::SDL::ScaleMode to use for texture scaling.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid.
		 *
		 *  \note If the scale mode is not supported, the closest supported mode is
		 *        chosen.
		 */
		int SetScaleMode(ScaleMode scaleMode);
		/**
		 *  \brief Get the scale mode used for texture scale operations.
		 *
		 *  \param scaleMode A pointer filled in with the current scale mode.
		 *
		 *  \return 0 on success, or -1 if the texture is not valid.
		 */
		int GetScaleMode(ScaleMode& scaleMode);
	};

	/**
	 *  \brief Create a window and default renderer
	 *
	 *  \param size     The size of the window
	 *  \param window   A reference filled with the window, or NULL on error
	 *  \param renderer A reference filled with the renderer, or NULL on error
	 *  \param window_flags The flags used to create the window
	 *
	 *  \return 0 on success, or -1 on error
	 */
	int CreateWindowAndRenderer(const Point& size, Window& window, Renderer& renderer, Uint32 window_flags = SDL_WINDOW_SHOWN & SDL_WINDOW_RESIZABLE);

	const Colour VERY_LIGHT_RED = { 255, 191, 191 };
	const Colour VERY_LIGHT_ORANGE = { 255, 223, 191 };
	const Colour VERY_LIGHT_YELLOW = { 255, 255, 191 };
	const Colour VERY_LIGHT_LIME = { 223, 255, 191 };
	const Colour VERY_LIGHT_GREEN = { 191, 255, 191 };
	const Colour VERY_LIGHT_TURQUOISE = { 191, 255, 223 };
	const Colour VERY_LIGHT_CYAN = { 191, 255, 255 };
	const Colour VERY_LIGHT_AZURE = { 191, 223, 255 };
	const Colour VERY_LIGHT_BLUE = { 191, 191, 255 };
	const Colour VERY_LIGHT_VIOLET = { 223, 191, 255 };
	const Colour VERY_LIGHT_MAGENTA = { 255, 191, 255 };
	const Colour VERY_LIGHT_RASPBERRY = { 255, 191, 223 };

	const Colour LIGHT_RED = { 255, 128, 128 };
	const Colour LIGHT_ORANGE = { 255, 191, 128 };
	const Colour LIGHT_YELLOW = { 255, 255, 128 };
	const Colour LIGHT_LIME = { 191, 255, 128 };
	const Colour LIGHT_GREEN = { 128, 255, 128 };
	const Colour LIGHT_TURQUOISE = { 128, 255, 191 };
	const Colour LIGHT_CYAN = { 128, 255, 255 };
	const Colour LIGHT_AZURE = { 128, 191, 255 };
	const Colour LIGHT_BLUE = { 128, 128, 255 };
	const Colour LIGHT_VIOLET = { 191, 128, 255 };
	const Colour LIGHT_MAGENTA = { 255, 128, 255 };
	const Colour LIGHT_RASPBERRY = { 255, 128, 191 };

	const Colour RED = { 255, 0, 0 };
	const Colour ORANGE = { 255, 128, 0 };
	const Colour YELLOW = { 255, 255, 0 };
	const Colour LIME = { 128, 255, 0 };
	const Colour GREEN = { 0, 255, 0 };
	const Colour TURQUOISE = { 0, 255, 128 };
	const Colour CYAN = { 0, 255, 255 };
	const Colour AZURE = { 0, 128, 255 };
	const Colour BLUE = { 0, 0, 255 };
	const Colour VIOLET = { 128, 0, 255 };
	const Colour MAGENTA = { 255, 0, 255 };
	const Colour RASPBERRY = { 255, 0, 128 };

	const Colour DARK_RED = { 128, 0, 0 };
	const Colour DARK_ORANGE = { 128, 64, 0 };
	const Colour DARK_YELLOW = { 128, 128, 0 };
	const Colour DARK_LIME = { 64, 128, 0 };
	const Colour DARK_GREEN = { 0, 128, 0 };
	const Colour DARK_TURQUOISE = { 0, 128, 64 };
	const Colour DARK_CYAN = { 0, 128, 128 };
	const Colour DARK_AZURE = { 0, 64, 128 };
	const Colour DARK_BLUE = { 0, 0, 128 };
	const Colour DARK_VIOLET = { 64, 0, 128 };
	const Colour DARK_MAGENTA = { 128, 0, 128 };
	const Colour DARK_RASPBERRY = { 128, 0, 64 };

	const Colour VERY_DARK_RED = { 64, 0, 0 };
	const Colour VERY_DARK_ORANGE = { 64, 32, 0 };
	const Colour VERY_DARK_YELLOW = { 64, 64, 0 };
	const Colour VERY_DARK_LIME = { 32, 64, 0 };
	const Colour VERY_DARK_GREEN = { 0, 64, 0 };
	const Colour VERY_DARK_TURQUOISE = { 0, 64, 32 };
	const Colour VERY_DARK_CYAN = { 0, 64, 64 };
	const Colour VERY_DARK_AZURE = { 0, 32, 64 };
	const Colour VERY_DARK_BLUE = { 0, 0, 64 };
	const Colour VERY_DARK_VIOLET = { 32, 0, 64 };
	const Colour VERY_DARK_MAGENTA = { 64, 0, 64 };
	const Colour VERY_DARK_RASPBERRY = { 64, 0, 32 };

	const Colour WHITE = { 255, 255, 255 };
	const Colour LIGHT_GREY = { 191, 191, 191 };
	const Colour GREY = { 128, 128, 128 };
	const Colour DARK_GREY = { 64, 64, 64 };
	const Colour VERY_DARK_GREY = { 32, 32, 32 };
	const Colour BLACK = { 0, 0, 0 };

	namespace GL {
		/**
		 *  \brief Bind the texture to the current OpenGL/ES/ES2 context for use with
		 *         OpenGL instructions.
		 *
		 *  \param texture  The SDL texture to bind
		 *  \param texsize  A reference to floats that will be filled with the texture size
		 *
		 *  \return 0 on success, or -1 if the operation is not supported
		 */
		int BindTexture(Texture& texture, FPoint& texsize);

		/**
		 *  \brief Unbind a texture from the current OpenGL/ES/ES2 context.
		 *
		 *  \param texture  The SDL texture to unbind
		 *
		 *  \return 0 on success, or -1 if the operation is not supported
		 */
		int UnbindTexture(Texture& texture);
	}
}