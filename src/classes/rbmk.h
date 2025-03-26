#pragma once

#include "raylib.h"

#include <vector>

#include "columns/column.h"

class RBMK {
    public:
        RBMK();

        void update();
        void draw();

    private:
        std::vector<Column> columns;
};