#pragma once

#include "raylib.h"

#include <vector>
#include <string>

#include "columns/columnBase.h"
#include "submenu/submenu.h"

class RBMKBuilder {
    public:
        RBMKBuilder();

        // helper functions
        bool isMouseWithinGrid();
        Vector2 getSelectedPosition();

        // selector stuff
        void resetSelector();
        void toggleSelectorTile(int tile);
        void setSelectorTile(int tile, bool state);
        bool getSelectorTile(int tile);

        // column stuff
        ColumnType getTypeFromIndex(int i);
        std::string getStringFromType(ColumnType type);

        // submenu stuff
        bool hasSubmenu(ColumnType type);
        Submenu* makeSubmenuFromType(ColumnType type, Vector2 columnPos);

        // main
        void update();
        void draw();

        // vars
        bool active = true;
        Texture2D ui;

        Submenu* submenu;
        bool submenuActive = false;

        int columnIndex = 0;

        bool selectorMode = false;
        bool selected[15 * 15];
};