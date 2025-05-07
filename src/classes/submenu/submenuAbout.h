#pragma once
#include "raylib.h"

#include <string>

#include "submenu.h"

class SubmenuAbout : public Submenu {
    public:
        SubmenuAbout();

        void open();
        void update();
        void draw();
        void close();
};