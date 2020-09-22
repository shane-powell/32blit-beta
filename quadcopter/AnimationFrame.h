#pragma once
#include "Frame.h"
#include "types/rect.hpp"

class AnimationFrame : public Frame {
	blit::Rect sprite;

    AnimationFrame(blit::Rect sprite, uint8_t frameLength) {
        this->sprite = sprite;
        this->frameLength = frameLength;
    };
};

