#pragma once

#include "raylib.h"

#include "columnBase.h"

class ColumnCooler : public ColumnBase {
    public:
        ColumnCooler();

        void update();
        void draw(Vector2 columnSize, Vector2 destPos);
};