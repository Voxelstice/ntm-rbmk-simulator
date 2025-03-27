#pragma once

#include "raylib.h"

#include <vector>

#include "columns/columnBase.h"

class RBMKBuilder {
    public:
        RBMKBuilder();

        void update();
        void draw();

        bool isMouseWithinGrid();
        Vector2 getSelectedPosition();
        ColumnType getTypeFromIndex();

        bool active = true;
        Texture2D ui;

        int columnIndex = 0;
};