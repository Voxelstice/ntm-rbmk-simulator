#pragma once

#include "raylib.h"

#include <vector>
#include <string>

#include "columns/columnBase.h"

class RBMKBuilder {
    public:
        RBMKBuilder();

        void update();
        void draw();

        bool isMouseWithinGrid();
        Vector2 getSelectedPosition();
        ColumnType getTypeFromIndex(int i);
        std::string getStringFromType(ColumnType type);

        bool active = true;
        Texture2D ui;

        int columnIndex = 0;
};