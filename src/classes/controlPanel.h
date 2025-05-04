#pragma once

#include "raylib.h"

class ControlPanel {
    public:
        ControlPanel();

        void update();
        void draw();

        Texture2D ui;

        bool az5Lid = true;
        double lastPress = 0;
};