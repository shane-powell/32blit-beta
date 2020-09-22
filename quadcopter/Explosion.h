#pragma once
#include "Entity.h"
struct Explosion : Entity {
    uint8_t lengthUp = 0;
    uint8_t lengthDown = 0;
    uint8_t lengthLeft = 0;
    uint8_t lengthRight = 0;
    int16_t lifeTime = 50;

};

