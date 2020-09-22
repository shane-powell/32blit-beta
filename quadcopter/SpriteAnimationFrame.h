#pragma once
#include "Frame.h"
#include "types/rect.hpp"

class SpriteAnimationFrame : public Frame {
    public:
	blit::Rect sprite;

        SpriteAnimationFrame(blit::Rect sprite, uint8_t frameLength) {
            this->sprite = sprite;
            this->frameLength = frameLength;
        };
    };

