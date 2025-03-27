// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/tileentity/machine/rbmk/TileEntityRBMKRod.java

#pragma once

#include "raylib.h"

#include "columnBase.h"
#include "columnFluxReceiver.h"
#include "../fuel/rbmkFuelRod.h"

class ColumnFuelRod : public ColumnFluxReceiver {
    public:
        ColumnFuelRod();

        void init();
        void update();
        void draw(Vector2 columnSize, Vector2 destPos);
        void reset();

        void receiveFlux(NeutronStream stream);
        double fluxFromType(NType type);

        void spreadFlux(double flux, double ratio);

        std::vector<std::string> getInfo();

        double fluxFastRatio = 0.0;
        double fluxQuantity = 0.0;
        double lastFluxQuantity = 0.0;
        double lastFluxRatio = 0.0;

        bool hasRod = false;

        RBMKFuelRod* fuel;
};