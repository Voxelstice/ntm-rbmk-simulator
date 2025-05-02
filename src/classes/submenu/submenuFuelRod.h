#pragma once
#include "raylib.h"

#include <string>

#include "submenu.h"

class SubmenuFuelRod : public Submenu {
    public:
        SubmenuFuelRod(Vector2 m_columnPos);

        void open();
        void update();
        void draw();
        void close();

        std::string name = "";
};