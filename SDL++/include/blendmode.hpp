#pragma once

#include <SDL_blendmode.h>

namespace SDL {
	// The blend mode used in RenderCopy() and drawing operations.
	enum class BlendMode {
		NONE = SDL_BLENDMODE_NONE,   // no blending       (dstRGB =  srcRGB                                ) (dstA =         dstA                     )
		BLEND = SDL_BLENDMODE_BLEND, // alpha blending    (dstRGB = (srcRGB * srcA  ) + (dstRGB * (1-srcA))) (dstA =  srcA         + (dstA * (1-srcA)))
		ADD = SDL_BLENDMODE_ADD,     // additive blending (dstRGB = (srcRGB * srcA  ) +  dstRGB            ) (dstA =         dstA                     )
		MOD = SDL_BLENDMODE_MOD,     // colour modulate    (dstRGB =  srcRGB * dstRGB                       ) (dstA =         dstA                     )
		MUL = SDL_BLENDMODE_MUL,     // colour multiply    (dstRGB = (srcRGB * dstRGB) + (dstRGB * (1-srcA))) (dstA = (srcA * dstA) + (dstA * (1-srcA)))
		INVALID = SDL_BLENDMODE_INVALID
	};

	// The blend operation used when combining source and destination pixel components
	enum class BlendOperation
	{
		ADD = SDL_BLENDOPERATION_ADD,                   // dst + src     : supported by all renderers
		SUBTRACT = SDL_BLENDOPERATION_SUBTRACT,         // dst - src     : supported by D3D9, D3D11, OpenGL, OpenGLES
		REV_SUBTRACT = SDL_BLENDOPERATION_REV_SUBTRACT, // src - dst     : supported by D3D9, D3D11, OpenGL, OpenGLES
		MINIMUM = SDL_BLENDOPERATION_MINIMUM,           // min(dst, src) : supported by D3D11
		MAXIMUM = SDL_BLENDOPERATION_MAXIMUM            // max(dst, src) : supported by D3D11
	};

	// The normalized factor used to multiply pixel components
	enum class BlendFactor {
		ZERO                 = SDL_BLENDFACTOR_ZERO,                 // 0, 0, 0, 0
		ONE                  = SDL_BLENDFACTOR_ONE,                  // 1, 1, 1, 1
		SRC_COLOUR           = SDL_BLENDFACTOR_SRC_COLOR,            // srcR, srcG, srcB, srcA
		SRC_ALPHA            = SDL_BLENDFACTOR_SRC_ALPHA,            // srcA, srcA, srcA, srcA
		DST_COLOUR           = SDL_BLENDFACTOR_DST_COLOR,            // dstR, dstG, dstB, dstA
		DST_ALPHA            = SDL_BLENDFACTOR_DST_ALPHA,            // dstA, dstA, dstA, dstA
		ONE_MINUS_SRC_COLOUR = SDL_BLENDFACTOR_ONE_MINUS_SRC_COLOR,  // 1-srcR, 1-srcG, 1-srcB, 1-srcA
		ONE_MINUS_SRC_ALPHA  = SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,  // 1-srcA, 1-srcA, 1-srcA, 1-srcA
		ONE_MINUS_DST_COLOUR = SDL_BLENDFACTOR_ONE_MINUS_DST_COLOR,  // 1-dstR, 1-dstG, 1-dstB, 1-dstA
		ONE_MINUS_DST_ALPHA  = SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA   // 1-dstA, 1-dstA, 1-dstA, 1-dstA
	};

	/**
	 *  \brief Create a custom blend mode, which may or may not be supported by a given renderer
	 *
	 *  \param srcColourFactor source colour factor
	 *  \param dstColourFactor destination colour factor
	 *  \param colourOperation colour operation
	 *  \param srcAlphaFactor source alpha factor
	 *  \param dstAlphaFactor destination alpha factor
	 *  \param alphaOperation alpha operation
	 *
	 *  The result of the blend mode operation will be:
	 *      dstRGB = dstRGB * dstColourFactor colourOperation srcRGB * srcColourFactor
	 *  and
	 *      dstA = dstA * dstAlphaFactor alphaOperation srcA * srcAlphaFactor
	 */
	static BlendMode ComposeCustomBlendMode(
		BlendFactor srcColourFactor,
		BlendFactor dstColourFactor,
		BlendOperation colourOperation,
		BlendFactor srcAlphaFactor,
		BlendFactor dstAlphaFactor,
		BlendOperation alphaOperation
	);
}