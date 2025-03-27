// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/tileentity/machine/rbmk/IRBMKFluxReceiver.java

#include "raylib.h"

#include "columnBase.h"
#include "columnFluxReceiver.h"
#include "../neutron/neutronStream.h"

#include "../dials.h"
#include "../../main.h"

ColumnFluxReceiver::ColumnFluxReceiver() {
    type = COLUMN_NONE;
}

void ColumnFluxReceiver::init() { }
void ColumnFluxReceiver::update() { }
void ColumnFluxReceiver::draw(Vector2 columnSize, Vector2 destPos) { }
void ColumnFluxReceiver::reset() { }

void ColumnFluxReceiver::receiveFlux(NeutronStream stream) { }