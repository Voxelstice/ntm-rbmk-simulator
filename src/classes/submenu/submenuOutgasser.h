#pragma once
#include "raylib.h"

#include <string>

#include "submenu.h"

class SubmenuOutgasser : public Submenu {
    public:
        SubmenuOutgasser(Vector2 m_columnPos);

        void open();
        void update();
        void draw();
        void close();
};