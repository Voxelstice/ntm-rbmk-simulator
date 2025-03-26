#pragma once

#include "raylib.h"

#include <vector>

class Column {
    public:
        Column();

        void update();
        void draw();

        // its so tempting to use floats...
        // but for the sake of accuracy we're gonna remain type-accurate
        double temp = 20.0;

        // realishit sim!
        int water = 0;
        int steam = 0;

        int maxWater = 16000;
        int maxSteam = 16000;
};