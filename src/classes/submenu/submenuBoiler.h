#pragma once
#include "raylib.h"

#include <string>

#include "submenu.h"

class SubmenuBoiler : public Submenu {
    public:
        SubmenuBoiler(Vector2 m_columnPos);

        void open();
        void update();
        void draw();
        void close();
};