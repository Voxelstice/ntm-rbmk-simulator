#pragma once

#include "raylib.h"
#include <vector>

extern double math_euler;
extern double math_pi;

double clamp_double(double value, double minimum, double maximum);
std::vector<Vector2> getCardinalDirections();

Vector2 Vector2Round(Vector2 vec, int div);
Vector2 Vector2Floor(Vector2 vec, int div);