#pragma once

#include "raylib.h"

#include <vector>

#include "columns/columnBase.h"
#include "neutron/neutronStream.h"

enum RBMKState {
    OFFLINE = 0,
    RUNNING = 1,
    MELTED = 2
};

class RBMK {
    public:
        RBMK();

        // main
        void updateControl();
        void update();
        void draw();
        void reset();

        // neutrons
        void addStream(NeutronStream* stream);

        // radiation
        void emitRadiation(double rad);

        // states
        void changeState(RBMKState newState);
        void meltdown();

        // other
        void az5();
        void cycleCompressors();
        void selectControlRods();
        void selectGroup(ColumnColor col);
        void assignGroup(ColumnColor col);

        // columns
        ColumnBase* placeColumn(Vector2 pos, ColumnBase *column);
        ColumnBase* makeColumnFromType(ColumnType type);
        ColumnBase* getColumn(int i);

        // utils
        Vector2 posFromIndex(int i);
        int indexFromPos(Vector2 pos);

        // vars
        RBMKState state = OFFLINE;
        std::vector<ColumnBase*> columns;
        std::vector<NeutronStream*> streams;

        Vector2 columnGridPosition;

        double radiationEmitted = 0;
};