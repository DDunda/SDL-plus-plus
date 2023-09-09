#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef SDL_shape_hpp_
#define SDL_shape_hpp_
#pragma once

#include <SDL_shape.h>

#include "pixels.hpp"
#include "video.hpp"

namespace SDL
{
    static constexpr int NONSHAPEABLE_WINDOW = -1;
    static constexpr int INVALID_SHAPE_ARGUMENT = -2;
    static constexpr int WINDOW_LACKS_SHAPE = -3;

    namespace WindowShape
    {
        /**
         * Create a window that can be shaped with the specified position, dimensions,
         * and flags.
         *
         * \param title The title of the window, in UTF-8 encoding.
         * \param x The x position of the window, ::SDL_WINDOWPOS_CENTERED, or
         *          ::SDL_WINDOWPOS_UNDEFINED.
         * \param y The y position of the window, ::SDL_WINDOWPOS_CENTERED, or
         *          ::SDL_WINDOWPOS_UNDEFINED.
         * \param w The width of the window.
         * \param h The height of the window.
         * \param flags The flags for the window, a mask of SDL_WINDOW_BORDERLESS with
         *              any of the following: ::SDL_WINDOW_OPENGL,
         *              ::SDL_WINDOW_INPUT_GRABBED, ::SDL_WINDOW_HIDDEN,
         *              ::SDL_WINDOW_RESIZABLE, ::SDL_WINDOW_MAXIMIZED,
         *              ::SDL_WINDOW_MINIMIZED, ::SDL_WINDOW_BORDERLESS is always set,
         *              and ::SDL_WINDOW_FULLSCREEN is always unset.
         * \return the window created, or NULL if window creation failed.
         */
        static Window CreateShapedWindow(const char* title, unsigned int x, unsigned int y, unsigned int w, unsigned int h, Uint32 flags = SDL_WINDOW_INPUT_GRABBED)
            { return Window::FromPtr(SDL_CreateShapedWindow(title, x, y, w, h, flags)); }

        /**
         * Create a window that can be shaped with the specified position, dimensions,
         * and flags.
         *
         * \param title The title of the window, in UTF-8 encoding.
         * \param pos The position of the window, may include ::SDL_WINDOWPOS_CENTERED, or
         *          ::SDL_WINDOWPOS_UNDEFINED.
         * \param size The size of the window.
         * \param flags The flags for the window, a mask of SDL_WINDOW_BORDERLESS with
         *              any of the following: ::SDL_WINDOW_OPENGL,
         *              ::SDL_WINDOW_INPUT_GRABBED, ::SDL_WINDOW_HIDDEN,
         *              ::SDL_WINDOW_RESIZABLE, ::SDL_WINDOW_MAXIMIZED,
         *              ::SDL_WINDOW_MINIMIZED, ::SDL_WINDOW_BORDERLESS is always set,
         *              and ::SDL_WINDOW_FULLSCREEN is always unset.
         * \return the window created, or NULL if window creation failed.
         */
        static Window CreateShapedWindow(const char* title, Point pos, Point size, Uint32 flags = SDL_WINDOW_INPUT_GRABBED)
            { return Window::FromPtr(SDL_CreateShapedWindow(title, (unsigned int)pos.x, (unsigned int)pos.y, (unsigned int)size.w, (unsigned int)size.h, flags)); }

        /**
         * Create a window that can be shaped with the specified position, dimensions,
         * and flags.
         *
         * \param title The title of the window, in UTF-8 encoding.
         * \param shape The position and size of the window. The position may include
         *              ::SDL_WINDOWPOS_CENTERED, or ::SDL_WINDOWPOS_UNDEFINED.
         * \param flags The flags for the window, a mask of SDL_WINDOW_BORDERLESS with
         *              any of the following: ::SDL_WINDOW_OPENGL,
         *              ::SDL_WINDOW_INPUT_GRABBED, ::SDL_WINDOW_HIDDEN,
         *              ::SDL_WINDOW_RESIZABLE, ::SDL_WINDOW_MAXIMIZED,
         *              ::SDL_WINDOW_MINIMIZED, ::SDL_WINDOW_BORDERLESS is always set,
         *              and ::SDL_WINDOW_FULLSCREEN is always unset.
         * \return the window created, or NULL if window creation failed.
         */
        static Window CreateShapedWindow(const char* title, Rect shape, Uint32 flags = SDL_WINDOW_INPUT_GRABBED)
            { return Window::FromPtr(SDL_CreateShapedWindow(title, (unsigned int)shape.x, (unsigned int)shape.y, (unsigned int)shape.w, (unsigned int)shape.h, flags)); }

        /**
         * Return whether the given window is a shaped window.
         *
         * \param window The window to query for being shaped.
         * \return SDL_TRUE if the window is a window that can be shaped, SDL_FALSE if
         *         the window is unshaped or NULL.
         *
         * \since This function is available since SDL 2.0.0.
         *
         * \sa SDL_CreateShapedWindow
         */
        static bool IsShapedWindow(const Window& window) { return SDL_IsShapedWindow(window.window.get()) == SDL_TRUE; }

        /** \brief An enum denoting the specific type of contents present in an SDL_WindowShapeParams union. */
        enum class ShapeMode
        {
            /** \brief The default mode, a binarized alpha cutoff of 1. */
            Default = ShapeModeDefault,
            /** \brief A binarized alpha cutoff with a given integer value. */
            BinarizeAlpha = ShapeModeBinarizeAlpha,
            /** \brief A binarized alpha cutoff with a given integer value, but with the opposite comparison. */
            ReverseBinarizeAlpha = ShapeModeReverseBinarizeAlpha,
            /** \brief A colour key is applied. */
            ColourKey = ShapeModeColorKey,
            /** \brief A colour key is applied. */
            ColorKey = ShapeModeColorKey
        };

#define SDL_SHAPEMODEALPHA(mode) (mode == ShapeModeDefault || mode == ShapeModeBinarizeAlpha || mode == ShapeModeReverseBinarizeAlpha)

        
        /** \brief A struct that tags the SDL_WindowShapeParams union with an enum describing the type of its contents. */
        struct Mode 
        {
            /** \brief The mode of these window-shape parameters. */
            ShapeMode mode;
            /** \brief A union containing parameters for shaped windows. */
            union
            {
                /** \brief A cutoff alpha value for binarization of the window shape's alpha channel. */
                Uint8 binarizationCutoff;
                Colour colourKey;
            } parameters; /** \brief Window-shape parameters. */

            explicit operator SDL_WindowShapeMode() const
            {
                SDL_WindowShapeMode m = {};
                m.mode = (WindowShapeMode)mode;
                m.parameters.colorKey = parameters.colourKey;
                return m;
            }
        };

        /**
         * Set the shape and parameters of a shaped window.
         *
         * \param window The shaped window whose parameters should be set.
         * \param shape A surface encoding the desired shape for the window.
         * \param shape_mode The parameters to set for the shaped window.
         * \return 0 on success, SDL_INVALID_SHAPE_ARGUMENT on an invalid shape
         *         argument, or SDL_NONSHAPEABLE_WINDOW if the SDL_Window given does
         *         not reference a valid shaped window.
         */
        int SetWindowShape(Window& window, Surface& shape, const Mode& shape_mode)
            { return SDL_SetWindowShape(window.window.get(), shape.surface.get(), (SDL_WindowShapeMode*)&shape_mode); }

        /**
         * Get the shape parameters of a shaped window.
         *
         * \param window The shaped window whose parameters should be retrieved.
         * \param shape_mode An empty shape-mode structure to fill, or NULL to check
         *                   whether the window has a shape.
         * \return 0 if the window has a shape and, provided shape_mode was not NULL,
         *         shape_mode has been filled with the mode data,
         *         SDL_NONSHAPEABLE_WINDOW if the SDL_Window given is not a shaped
         *         window, or SDL_WINDOW_LACKS_SHAPE if the SDL_Window given is a
         *         shapeable window currently lacking a shape.
         */
        int GetShapedWindowMode(Window& window, Mode& shape_mode)
            { return SDL_GetShapedWindowMode(window.window.get(), (SDL_WindowShapeMode*)&shape_mode); }
    }
}

#endif
#endif