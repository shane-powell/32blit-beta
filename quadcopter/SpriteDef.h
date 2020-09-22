#pragma once
#include "32blit.hpp"
#include "SpriteAnimationFrame.h"

using namespace blit;


struct SpriteAnimation {
    uint8_t frameIndex = 0;
    std::vector<SpriteAnimationFrame> animationFrames;

    SpriteAnimation() = default;
};

    const Rect bombSprite = Rect(6, 0, 1, 1);
    const Rect explosionCore = Rect(7, 0, 1, 1);
    const Rect explosionLine = Rect(8, 0, 1, 1);
    const Rect explosionEnd = Rect(9, 0, 1, 1);

    const Rect quadSpriteSide = Rect(0, 4, 2, 2);
    const Rect quadSpriteUp = Rect(0, 4, 2, 2);
    const Rect QuadSpriteDown = Rect(0, 4, 2, 2);

	const Rect quadFrame1 = Rect(0, 4, 2, 2);
    const Rect quadFrame2 = Rect(2, 4, 2, 2);
    const Rect quadFrame3 = Rect(4, 4, 2, 2);
    const Rect quadFrame4 = Rect(6, 4, 2, 2);
    const Rect quadFrame5 = Rect(8, 4, 2, 2);
    const Rect quadFrame6 = Rect(10, 4, 2, 2);
    const Rect quadFrame7 = Rect(12, 4, 2, 2);
    const Rect quadFrame8 = Rect(14, 4, 2, 2);



