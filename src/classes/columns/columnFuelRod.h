#pragma once

#include "raylib.h"

#include "columnBase.h"

class ColumnFuelRod : public ColumnBase {
    public:
        ColumnFuelRod();

        void update();
        void draw(Vector2 columnSize, Vector2 destPos);
};