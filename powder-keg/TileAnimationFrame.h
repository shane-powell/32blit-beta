#pragma once
#include <cstdint>

#include "Frame.h"

class TileAnimationFrame : public Frame {
public:
    uint16_t tileId;

    TileAnimationFrame(uint16_t tileId, uint8_t frameLength) {
        this->tileId = tileId;
        this->frameLength = frameLength;
    };
};

