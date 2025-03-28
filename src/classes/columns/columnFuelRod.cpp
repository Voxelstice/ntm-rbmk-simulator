// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/tileentity/machine/rbmk/TileEntityRBMKRod.java
// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/2d48960e2d12e8a5b03ccb92de8849f067b85046/src/main/java/com/hbm/inventory/gui/GUIRBMKConsole.java#L359

#include "raylib.h"
#include "raymath.h"

#include <cmath>

#include "columnBase.h"
#include "columnFuelRod.h"

#include "../dials.h"
#include "../../main.h"

#include "../fuel/fuelRegistry.h"

ColumnFuelRod::ColumnFuelRod() {
    type = COLUMN_FUEL;
    fuel = new RBMKFuelRod("");
}

void ColumnFuelRod::init() {
    fuel = PrepareFuel("rbmk_fuel_ueu");

    hasRod = fuel->active;
    if (hasRod == true) {
        fuel->reset();
    }
}
void ColumnFuelRod::update() {
    hasRod = fuel->active;

    if (hasRod) {
        double fluxRatioOut;
        double fluxQuantityOut;

        NType rType = fuel->rType;
        if(rType == NTYPE_SLOW)
            fluxRatioOut = 0.0;
        else
            fluxRatioOut = 1.0;

        double fluxIn = fluxFromType(fuel->nType);
        fluxQuantityOut = fuel->burn(fluxIn);

        fuel->updateHeat(1.0);
        heat += fuel->provideHeat(heat, 1.0);

        if (heat > maxHeat) {
            if (rbmkDials.dialDisableMeltdowns == false) {
                rbmk->meltdown();
            }
            lastFluxRatio = 0.0;
            lastFluxQuantity = 0.0;
            fluxQuantity = 0.0;
            return;
        }

        if (heat > 10000) heat = 10000;

        lastFluxQuantity = fluxQuantity;
        lastFluxRatio = fluxFastRatio;

        spreadFlux(fluxQuantityOut, fluxRatioOut);

        fluxQuantity = 0.0;
        fluxFastRatio = 0.0;
    }
}
void ColumnFuelRod::draw(Vector2 columnSize, Vector2 destPos) {
    // NOTE: reasim uses X 90
    controlPanel->drawTex(controlPanel->ui, {10, 172}, columnSize, destPos, columnSize, 4);

    if (hasRod == true) {
        float coreHeatY = std::round(8.0f * Clamp((float)((fuel->itemCoreHeat - 20.0) / fuel->meltingPoint), 0.0f, 1.0f));
        float enrichmentY = std::round(8.0f * (float)(fuel->getEnrichment()));
        float xenonY = std::round(8.0f * (float)(fuel->getPoisonLevel()/100.0));

        //printf("%i\n", (int)fuel->itemYield);

        controlPanel->drawTex(controlPanel->ui, {11, 183+8-coreHeatY}, {2, coreHeatY}, Vector2Add(destPos, {1, 1+8-coreHeatY}), {2, coreHeatY}, 4); // core heat
        controlPanel->drawTex(controlPanel->ui, {14, 183+8-enrichmentY}, {2, enrichmentY}, Vector2Add(destPos, {4, 1+8-enrichmentY}), {2, enrichmentY}, 4); // depletion

        // interestingly xenon isn't even drawn at all!
        controlPanel->drawTex(controlPanel->ui, {17, 183+8-xenonY}, {2, xenonY}, Vector2Add(destPos, {7, 1+8-xenonY}), {2, xenonY}, 4); // xenon
    }
}
void ColumnFuelRod::reset() {
    fuel->reset();
}

void ColumnFuelRod::receiveFlux(NeutronStream stream) {
    double fastFlux = fluxQuantity * fluxFastRatio;
    double fastFluxIn = stream.fluxQuantity * stream.fluxRatio;

    fluxQuantity += stream.fluxQuantity;
    fluxFastRatio = (fastFlux + fastFluxIn) / fluxQuantity;
}

double ColumnFuelRod::fluxFromType(NType type) {
    double fastFlux = fluxQuantity * fluxFastRatio;
    double slowFlux = fluxQuantity * (1.0 - fluxFastRatio);

    switch(type) {
        case NTYPE_SLOW: return slowFlux + fastFlux * 0.5;
        case NTYPE_FAST: return fastFlux + slowFlux * 0.3;
        case NTYPE_ANY: return fluxQuantity;
    }

    return 0.0;
}

void ColumnFuelRod::spreadFlux(double flux, double ratio) {

}

std::vector<std::string> ColumnFuelRod::getInfo() {
    std::vector<std::string> vector;

    if (hasRod == true) {
        vector.push_back("Depletion: " + std::string(TextFormat("%.3f", (1.0-fuel->getEnrichment())*100.0)) + "%");
        vector.push_back("Xenon poison: " + std::string(TextFormat("%.3f", fuel->getPoisonLevel())) + "%");
        vector.push_back("Core temperature: " + std::string(TextFormat("%.1f", fuel->itemCoreHeat)) + " C");
        vector.push_back("Skin temperature: " + std::string(TextFormat("%.1f", fuel->itemHullHeat)) + " C / " + std::string(TextFormat("%.1f", fuel->meltingPoint)) + " C");
    }
    return vector;
}