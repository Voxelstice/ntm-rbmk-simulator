#pragma once

#include "raylib.h"

#include "columnBase.h"

class ColumnControlRod : public ColumnBase {
    public:
        ColumnControlRod();

        void update();
        void draw(Vector2 columnSize, Vector2 destPos);
};