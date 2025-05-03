#pragma once

#include "raylib.h"

#include <string>

class Item {
    public:
        Item(Vector2 m_position, std::string m_internalName, Texture2D m_tex);

        void setTooltip(std::string m_tooltip);

        void update();
        void draw();

        Vector2 position;
        std::string internalName;
        std::string tooltip;
        Texture2D tex;
};