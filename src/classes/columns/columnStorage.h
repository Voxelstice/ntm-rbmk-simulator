#pragma once

#include "raylib.h"

#include "columnBase.h"

class ColumnStorage : public ColumnBase {
    public:
        ColumnStorage();

        void draw(Vector2 columnSize, Vector2 destPos);
};