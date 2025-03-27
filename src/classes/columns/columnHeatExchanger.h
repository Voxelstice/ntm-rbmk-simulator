#pragma once

#include "raylib.h"

#include "columnBase.h"

class ColumnHeatExchanger : public ColumnBase {
    public:
        ColumnHeatExchanger();

        void update();
        void draw(Vector2 columnSize, Vector2 destPos);
};