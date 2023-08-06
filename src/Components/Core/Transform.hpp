#pragma once

#include "Vector2f.hpp"
#include <stdint.h>

struct Transform
{
    Vector2f position;
    Vector2f scale;
    uint32_t rotation;
};