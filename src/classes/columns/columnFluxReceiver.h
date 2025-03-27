// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/tileentity/machine/rbmk/IRBMKFluxReceiver.java

#pragma once

#include "raylib.h"

#include "columnBase.h"
#include "../neutron/neutronStream.h"

class ColumnFluxReceiver : public ColumnBase {
    public:
        ColumnFluxReceiver();

        virtual void init();
        virtual void update();
        virtual void draw(Vector2 columnSize, Vector2 destPos);
        virtual void reset();

        virtual void receiveFlux(NeutronStream stream);

        bool moderated = false;
};