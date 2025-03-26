#pragma once

#include "raylib.h"

#include "rbmk.h"

class ControlPanel {
    public:
        ControlPanel(RBMK* m_rbmk);

        void update();
        void draw();

        RBMK* rbmk;
    private:
        Texture2D ui;
};