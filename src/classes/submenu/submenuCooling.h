#pragma once
#include "raylib.h"

#include <string>

#include "submenu.h"

class SubmenuCooling : public Submenu {
    public:
        SubmenuCooling();

        void open();
        void update();
        void draw();
        void close();
};