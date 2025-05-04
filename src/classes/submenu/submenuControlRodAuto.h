#pragma once
#include "raylib.h"

#include <string>

#include "submenu.h"

class SubmenuControlRodAuto : public Submenu {
    public:
        SubmenuControlRodAuto(Vector2 m_columnPos);

        void open();
        void update();
        void draw();
        void close();
};