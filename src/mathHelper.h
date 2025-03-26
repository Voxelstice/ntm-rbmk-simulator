#pragma once

#include "raylib.h"
#include <vector>

namespace MathHelper {
    double clamp_double(double value, double minimum, double maximum);
    std::vector<Vector2> getCardinalDirections();
};