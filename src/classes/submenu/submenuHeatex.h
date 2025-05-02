#pragma once
#include "raylib.h"

#include <string>

#include "submenu.h"

class SubmenuHeatex : public Submenu {
    public:
        SubmenuHeatex(Vector2 m_columnPos);

        void open();
        void update();
        void draw();
        void close();

        std::string name = "";
};