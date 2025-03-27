#pragma once

#include "raylib.h"

#include "columnBase.h"

class ColumnOutgasser : public ColumnBase {
    public:
        ColumnOutgasser();

        void update();
        void draw(Vector2 columnSize, Vector2 destPos);
};