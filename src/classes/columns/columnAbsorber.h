#pragma once

#include "raylib.h"

#include "columnBase.h"

class ColumnAbsorber : public ColumnBase {
    public:
        ColumnAbsorber();

        void update();
        void draw(Vector2 columnSize, Vector2 destPos);
};