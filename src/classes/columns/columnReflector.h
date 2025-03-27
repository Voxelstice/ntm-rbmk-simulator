#pragma once

#include "raylib.h"

#include "columnBase.h"

class ColumnReflector : public ColumnBase {
    public:
        ColumnReflector();

        void update();
        void draw(Vector2 columnSize, Vector2 destPos);
};