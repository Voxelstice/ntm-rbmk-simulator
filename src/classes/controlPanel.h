#pragma once

#include "raylib.h"

#include "rbmk.h"

class ControlPanel {
    public:
        ControlPanel(RBMK* m_rbmk);

        void drawTex(Texture2D tex, Vector2 srcPos, Vector2 srcSize, Vector2 destPos, Vector2 destSize, float scale);

        void update();
        void draw();

        RBMK* rbmk;

        Texture2D ui;
};