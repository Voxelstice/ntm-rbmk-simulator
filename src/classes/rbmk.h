#pragma once

#include "raylib.h"

#include <vector>

#include "columns/columnBase.h"

enum RBMKState {
    OFFLINE = 0,
    RUNNING = 1
};

class RBMK {
    public:
        RBMK();

        ColumnBase* placeColumn(Vector2 pos, ColumnBase *column);
        ColumnBase* makeColumnFromType(ColumnType type);

        void update();
        void draw();

        void reset();
        void changeState(RBMKState newState);

        ColumnBase* getColumn(int i);

        Vector2 RBMK::posFromIndex(int i);
        int RBMK::indexFromPos(Vector2 pos);

        RBMKState state = OFFLINE;
        std::vector<ColumnBase*> columns;

        Vector2 columnGridPosition;
};