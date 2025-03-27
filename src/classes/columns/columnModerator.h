#pragma once

#include "raylib.h"

#include "columnBase.h"

class ColumnModerator : public ColumnBase {
    public:
        ColumnModerator();

        void update();
        void draw(Vector2 columnSize, Vector2 destPos);
};