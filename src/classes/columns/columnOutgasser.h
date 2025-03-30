#pragma once

#include "raylib.h"

#include "columnBase.h"
#include "columnFluxReceiver.h"

class ColumnOutgasser : public ColumnFluxReceiver {
    public:
        ColumnOutgasser();

        void update();
        void draw(Vector2 columnSize, Vector2 destPos);

        void receiveFlux(NeutronStream* stream);

        bool canProcess();
};