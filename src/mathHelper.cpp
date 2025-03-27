#include "raylib.h"
#include "raymath.h"

#include <cmath>
#include <algorithm>
#include <vector>

#include "mathHelper.h"

double math_euler = 2.71828; // not sure how precise it is in java
double math_pi = 3.14159; // raylib also defines PI, but im gonna define my own in case i need to change it

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

Vector2 Vector2Round(Vector2 vec, int div) {
    return { std::round(vec.x/(float)div)*div, std::round(vec.y/(float)div)*div };
}
Vector2 Vector2Floor(Vector2 vec, int div) {
    return { std::floor(vec.x/(float)div)*div, std::floor(vec.y/(float)div)*div };
}