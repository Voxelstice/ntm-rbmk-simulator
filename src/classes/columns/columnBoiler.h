#pragma once

#include "raylib.h"

#include "columnBase.h"

class ColumnBoiler : public ColumnBase {
    public:
        ColumnBoiler();

        void update();
        void draw(Vector2 columnSize, Vector2 destPos);
};