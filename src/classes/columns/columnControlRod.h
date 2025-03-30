#pragma once

#include "raylib.h"

#include "columnBase.h"

class ColumnControlRod : public ColumnBase {
    public:
        ColumnControlRod();

        // main
        void init();
        void update();
        void draw(Vector2 columnSize, Vector2 destPos);
        void reset();

        // controlling
        void setTarget(double target);
        double getMulti();

        // other
        std::vector<std::string> getInfo();

        // vars
        double startingLevel;
        double lastLevel;
        double level;
        double targetLevel;
        double speed = 0.00277; // 18 sec
};