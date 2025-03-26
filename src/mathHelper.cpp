#include "raylib.h"
#include "raymath.h"

#include <cmath>
#include <algorithm>
#include <vector>

#include "mathHelper.h"

namespace MathHelper {
    double clamp_double(double value, double minimum, double maximum) {
        return std::min(std::max(value, minimum), maximum);
    }

    std::vector<Vector2> getCardinalDirections() {
        std::vector<Vector2> vector;
        vector.clear();
        vector.push_back({1, 0});
        vector.push_back({-1, 0});
        vector.push_back({0, 1});
        vector.push_back({0, -1});
        return vector;
    }
}