#pragma once

#include "raylib.h"

#include <vector>
#include <string>

#include "columns/columnBase.h"
#include "submenu/submenu.h"

class RBMKBuilder {
    public:
        RBMKBuilder();

        void update();
        void draw();

        bool isMouseWithinGrid();
        Vector2 getSelectedPosition();
        ColumnType getTypeFromIndex(int i);
        std::string getStringFromType(ColumnType type);
        bool hasSubmenu(ColumnType type);
        Submenu* makeSubmenuFromType(ColumnType type, Vector2 columnPos);

        bool active = true;
        Texture2D ui;

        Submenu* submenu;
        bool submenuActive = false;

        int columnIndex = 0;
};