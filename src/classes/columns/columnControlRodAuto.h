#pragma once

#include "raylib.h"

#include "columnBase.h"
#include "columnControlRod.h"

class ColumnControlRodAuto : public ColumnControlRod {
    public:
        ColumnControlRodAuto();

        void update();
        void draw(Vector2 columnSize, Vector2 destPos);
};