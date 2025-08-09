#pragma once

#include "raylib.h"

#include "columnBase.h"
#include "../inventory/fluidTank.h"

class ColumnBoiler : public ColumnBase {
    public:
        ColumnBoiler();

        // main
        void init();
        void update();
        void draw(Vector2 columnSize, Vector2 destPos);
        void reset();

        // other
        std::vector<std::string> getInfo();
        void cycleCompressor();

        void setCompressionLevel(int level);
        int getCompressionLevel();

        // vars
        FluidTank* feed;
        FluidTank* steam;

        int consumption = 0;
        int output = 0;
};