// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/tileentity/machine/rbmk/TileEntityRBMKBase.java

#include "raylib.h"
#include "raymath.h"

#include <cmath>

#include "../../mathHelper.h"

#include "columnBase.h"
#include "../dials.h"
#include "../../main.h"

ColumnBase::ColumnBase() {
    neighbors.clear();
}

void ColumnBase::baseInit() {
    // this just gets neighbors
    neighbors.clear();

    if (active == false) return;

    for (Vector2 dir : getCardinalDirections()) {
        int i = rbmk->indexFromPos(Vector2Add(pos, dir));
        ColumnBase *base = rbmk->getColumn(i);
        if (base->active == true) {
            neighbors.push_back(base);
        }
    }
}

void ColumnBase::baseUpdate() {
    moveHeat();

    if (rbmkDials.dialReasimBoilers == true)
        boilWater();

    coolPassively();
}

void ColumnBase::baseReset() {
    heat = 20.0;
    water = 0;
    steam = 0;
}

// main stuff
void ColumnBase::boilWater() {
    if (heat < 100.0) return;

    double heatConsumption = rbmkDials.dialBoilerHeatConsumption;
    double availableHeat = (heat - 100.0) / heatConsumption;
    double availableWater = water;
    double availableSpace = maxSteam - steam;

    int processedWater = (int) std::floor(std::min(std::min(availableHeat, availableWater), availableSpace) * clamp_double(rbmkDials.dialReasimBoilerSpeed, 0.0, 1.0));

    if (processedWater <= 0) return;

    water -= processedWater;
    steam += processedWater;
    heat -= processedWater * heatConsumption;
}
void ColumnBase::moveHeat() {
    if (heat == 20.0 && rbmkDials.dialReasimBoilers == true)
        return;

    double heatTot = heat;
    int waterTot = water;
    int steamTot = steam;

    rec.clear();
    rec.push_back(this);

    for (ColumnBase* base : neighbors) {
        rec.push_back(base);
        heatTot += base->heat;
        waterTot += base->water;
        steamTot += base->steam;
    }

    int members = (int)rec.size();
    double stepSize = rbmkDials.dialColumnHeatFlow;

    if (members > 1) {
        double targetHeat = heatTot / (double)members;

        int tWater = waterTot / members;
        int rWater = waterTot % members;
        int tSteam = steamTot / members;
        int rSteam = steamTot % members;

        for (ColumnBase* base : rec) {
            double delta = targetHeat - base->heat;
            base->heat += delta * stepSize;

            base->water = tWater;
            base->steam = tSteam;
        }

        water += rWater;
        steam += rSteam;
    }
}
void ColumnBase::coolPassively() {
    heat -= rbmkDials.dialPassiveCooling;

    if (heat < 20.0) heat = 20.0;
}

// virtual
void ColumnBase::init() { }
void ColumnBase::update() { }
void ColumnBase::draw(Vector2 columnSize, Vector2 destPos) { }
void ColumnBase::reset() { }