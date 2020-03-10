#pragma once
#include "Entity.h"
struct Projectile : Entity {
    uint8_t transform = 0;
    int16_t lifetime = 300;
    int16_t vel_x = 0;
    int16_t vel_y = 0;
};

