#pragma once
#include "raylib.h"

#include <string>

#include "submenu.h"

class SubmenuControlRod : public Submenu {
    public:
        SubmenuControlRod(Vector2 m_columnPos);

        void open();
        void update();
        void draw();
        void close();
};