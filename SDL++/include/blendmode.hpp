#pragma once

#include <SDL_blendmode.h>

namespace SDL {
    // \brief The blend mode used in SDL_RenderCopy() and drawing operations.
    typedef SDL_BlendMode BlendMode;

    // \brief The blend operation used when combining source and destination pixel components
    typedef SDL_BlendOperation BlendOperation;

    // \brief The normalized factor used to multiply pixel components
    typedef SDL_BlendFactor BlendFactor;

    /**
     *  \brief Create a custom blend mode, which may or may not be supported by a given renderer
     *
     *  \param srcColorFactor source color factor
     *  \param dstColorFactor destination color factor
     *  \param colorOperation color operation
     *  \param srcAlphaFactor source alpha factor
     *  \param dstAlphaFactor destination alpha factor
     *  \param alphaOperation alpha operation
     *
     *  The result of the blend mode operation will be:
     *      dstRGB = dstRGB * dstColorFactor colorOperation srcRGB * srcColorFactor
     *  and
     *      dstA = dstA * dstAlphaFactor alphaOperation srcA * srcAlphaFactor
     */
    static BlendMode ComposeCustomBlendMode(BlendFactor srcColorFactor, BlendFactor dstColorFactor, BlendOperation colorOperation, BlendFactor srcAlphaFactor, BlendFactor dstAlphaFactor, BlendOperation alphaOperation) {
        return SDL_ComposeCustomBlendMode(srcColorFactor, dstColorFactor, colorOperation, srcAlphaFactor, dstAlphaFactor, alphaOperation);
    }
}