#pragma once
#include "raylib.h"

#include <string>

class Submenu {
    public:
        Submenu(Vector2 m_columnPos);

        virtual void open();
        virtual void update();
        virtual void draw();
        virtual void close();

        std::string name = "";

        Texture2D ui;
        Vector2 columnPos;
};