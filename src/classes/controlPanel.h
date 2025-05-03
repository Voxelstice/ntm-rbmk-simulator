#pragma once

#include "raylib.h"

#include "rbmk.h"

class ControlPanel {
    public:
        ControlPanel(RBMK* m_rbmk);

        void update();
        void draw();

        RBMK* rbmk;

        Texture2D ui;

        bool az5Lid = true;
        double lastPress = 0;
};