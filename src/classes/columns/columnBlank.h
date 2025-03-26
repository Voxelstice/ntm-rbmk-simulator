#pragma once

#include "raylib.h"

#include "columnBase.h"

class ColumnBlank : public ColumnBase {
    public:
        ColumnBlank();

        void draw(Vector2 columnSize, Vector2 destPos);
};