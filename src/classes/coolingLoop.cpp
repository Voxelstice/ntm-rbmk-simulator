// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/tileentity/machine/TileEntityMachineTurbine.java
// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/tileentity/machine/TileEntityCondenser.java

// so here, we're gonna assume the following ruleset:
// all fluids transfer Instantly
// if the fluid a tank has is gonna overfill a tank, it'll only take the amount to not overfill it
// if there's more than 1 tank, the amount transferred is going to be split and rounded
// clamping is handled by the fluid tank itself

// this is going to update in this order:
// 1, reservoir water is made and transferred to the boilers
// 2, boiler steam is transferred to turbines
// 3, steam is used, power is made, and steam is taken out of the turbine and to the next
// 4, low pressure steam is transferred to the condensers
// 5, condenser condenses low pressure steam to water.
// 6, condenser water is transferred to water reservoirs.

#include "raylib.h"

#include <cmath>

#include "coolingLoop.h"
#include "columns/columnBoiler.h"

#include "../main.h"

FluidType getTypeFromCompression(int compression) {
    switch (compression) {
        case 1:     return FLUID_STEAM;
        case 2:     return FLUID_HOTSTEAM;
        case 3:     return FLUID_SUPERHOTSTEAM;
        case 4:     return FLUID_ULTRAHOTSTEAM;
        default:    return FLUID_SPENTSTEAM;
    }
}

CoolingLoop::CoolingLoop() {
    reconfigure();
}

void CoolingLoop::reconfigure() {
    turbines.clear();
    condensers.clear();
    reservoirs.clear();

    // begin with turbines.
    int turbineCompressionCount = turbineCompression + 1;
    int turbineSteamInMax = 64000;
    int turbineSteamOutMax = 128000;
    switch (turbineType) {
        case TURBINE_SMALL: {
            turbineSteamInMax = 64000;
            turbineSteamOutMax = 128000;
            break;
        }
        case TURBINE_INDUSTRIAL: {
            turbineSteamInMax = 512000;
            turbineSteamOutMax = 10240000;
            break;
        }
        case TURBINE_CHUNGUS: {
            turbineSteamInMax = 1000000000;
            turbineSteamOutMax = 1000000000;
            break;
        }
    }

    for (int i = 0; i < turbineCount; i++) {
        for (int i2 = 0; i2 < turbineCompressionCount; i2++) {
            FluidTank* steamIn = new FluidTank(getTypeFromCompression(turbineCompressionCount-i2), turbineSteamInMax);
            FluidTank* steamOut = new FluidTank(getTypeFromCompression(turbineCompressionCount-i2-1), turbineSteamOutMax);
            turbines.push_back(steamIn);
            turbines.push_back(steamOut);
        }
    }

    // condensers.
    int condenserTankSize = 100;
    switch (condenserType) {
        case CONDENSER_SMALL: {
            condenserTankSize = 100;
            break;
        }
        case CONDENSER_AUXILIARY: {
            condenserTankSize = 1000;
            break;
        }
        case CONDENSER_LARGE: {
            condenserTankSize = 10000;
            break;
        }
        case CONDENSER_POWERED: {
            condenserTankSize = 1000000;
            break;
        }
    }
    
    for (int i = 0; i < condenserCount; i++) {
        FluidTank* steamIn = new FluidTank(FLUID_SPENTSTEAM, condenserTankSize);
        FluidTank* waterOut = new FluidTank(FLUID_WATER, condenserTankSize);
        condensers.push_back(steamIn);
        condensers.push_back(waterOut);
    }

    // finally, reservoirs.
    int reservoirTankSize = 16000;
    switch (reservoirType) {
        case RESERVOIR_SMALL: {
            reservoirTankSize = 16000;
            break;
        }
        case RESERVOIR_MEDIUM: {
            reservoirTankSize = 256000;
            break;
        }
        case RESERVOIR_BIG: {
            reservoirTankSize = 2048000;
            break;
        }
    }
    for (int i = 0; i < reservoirCount; i++) {
        FluidTank* tank = new FluidTank(FLUID_WATER, reservoirTankSize);
        tank->setFill(reservoirTankSize);
        reservoirs.push_back(tank);
    }

    // we're complete.
}
void CoolingLoop::reset() {
    // just reset the contents of the tanks
    for (int i = 0; i < turbines.size(); i++) {
        turbines[i]->setFill(0);
    }
    for (int i = 0; i < condensers.size(); i++) {
        condensers[i]->setFill(0);
    }
    for (int i = 0; i < reservoirs.size(); i++) {
        reservoirs[i]->setFill(reservoirs[i]->getMaxFill());
    }
}

void CoolingLoop::update() {
    // count the boilers for later
    int boilerCount = 0;
    for (int i = 0; i < 15*15; i++) {
        if (rbmk->columns[i]->active == false) continue;
        if (rbmk->columns[i]->type == COLUMN_BOILER) {
            ColumnBoiler* boiler = (ColumnBoiler*) rbmk->columns[i];
            boilerCount++;
        }
    }

    // 1, reservoir water is made and transferred to the boilers
    for (int i2 = 0; i2 < reservoirs.size(); i2++) {
        // fill it with some water
        int waterFillRate = 50;
        if (reservoirFillWeight == RESERVOIRFILL_HEAVY) waterFillRate = 500;
        reservoirs[i2]->setFill(reservoirs[i2]->getFill() + waterFillRate);

        // transfer water
        int fluidToTransfer = (int)std::floor((float)reservoirs[i2]->getFill() / (float)boilerCount);
        for (int i = 0; i < 15*15; i++) {
            if (rbmk->columns[i]->active == false) continue;
            if (rbmk->columns[i]->type == COLUMN_BOILER) {
                ColumnBoiler* boiler = (ColumnBoiler*) rbmk->columns[i];
                boiler->feed->setFill(boiler->feed->getFill() + fluidToTransfer);
                reservoirs[i2]->setFill(reservoirs[i2]->getFill() - fluidToTransfer);
            }
        }
    }

    // 2, boiler steam is transferred to turbines
    for (int i = 0; i < 15*15; i++) {
        if (rbmk->columns[i]->active == false) continue;
        if (rbmk->columns[i]->type == COLUMN_BOILER) {
            ColumnBoiler* boiler = (ColumnBoiler*) rbmk->columns[i];
            int fluidToTransfer = (int)std::floor((float)boiler->steam->getFill() / (float)turbineCount);
            for (int i2 = 0; i2 < turbineCount; i2++) {
                int i3 = i2 * (turbineCompression + 1);
                turbines[i3]->setFill(turbines[i3]->getFill() + fluidToTransfer);
                boiler->steam->setFill(boiler->steam->getFill() - fluidToTransfer);
            }
        }
    }

    // 3, steam is used, power is made, and steam is taken out of the turbine and to the next
    int turbineCompressionCount = turbineCompression + 1;
    for (int i = 0; i < turbineCount; i++) {
        for (int i2 = 0; i2 < turbineCompressionCount; i2++) {
            int i3 = i + i2;

            FluidTank* steamIn = turbines[i3];
            FluidTank* steamOut = turbines[i3+1];

            int inputOps = steamIn->getFill();

            /*
            		tanks[1].setTankType(trait.coolsTo);
					int inputOps = tanks[0].getFill() / trait.amountReq;
					int outputOps = (tanks[1].getMaxFill() - tanks[1].getFill()) / trait.amountProduced;
					int cap = maxSteamPerTick / trait.amountReq;
					int ops = Math.min(inputOps, Math.min(outputOps, cap));
					tanks[0].setFill(tanks[0].getFill() - ops * trait.amountReq);
					tanks[1].setFill(tanks[1].getFill() + ops * trait.amountProduced);
					this.power += (ops * trait.heatEnergy * eff);
					info[0] = ops * trait.amountReq;
					info[1] = ops * trait.amountProduced;
					info[2] = ops * trait.heatEnergy * eff;
					valid = true;
            */
        }
    }

    // 4, low pressure steam is transferred to the condensers

    // 5, condenser condenses low pressure steam to water.

    // 6, condenser water is transferred to water reservoirs.

}