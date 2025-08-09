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
    for (int i2 = 0; i2 < reservoirCount; i2++) {
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
    long powerProduced = 0;
    int turbineCompressionCount = turbineCompression + 1;
    for (int i = 0; i < turbineCount; i++) {
        for (int i2 = 0; i2 < turbineCompressionCount; i2++) {
            int i3 = i + i2;

            FluidTank* steamIn = turbines[i3];
            FluidTank* steamOut = turbines[i3+1];

            FluidTrait_Coolable* trait = getTraitForFluid(steamIn->fluidType);

            steamOut->setTankType(trait->coolsTo);

            int inputOps = steamIn->getFill() / trait->amountReq;
            int outputOps = (steamOut->getMaxFill() - steamOut->getFill()) / trait->amountProduced;

            // apparently this behavior DIFFERS between all 3 turbine types.
            // we have to compensate for that.
            int cap = 0;
            if (turbineType == TURBINE_SMALL) {
                cap = 6000 / trait->amountReq;
            } else if (turbineType == TURBINE_INDUSTRIAL) {
                cap = (int) std::ceil(steamIn->getFill() / trait->amountReq / 5.0);
            }

            int ops = 0;
            if (cap == 0) {
                ops = std::min(inputOps, outputOps);
            } else {
                ops = std::min(inputOps, std::min(outputOps, cap));
            }

            // efficiency.
            double eff = 0.85;
            if (turbineType == TURBINE_INDUSTRIAL) eff = 1;

            // back to normal.
            steamIn->setFill(steamIn->getFill() - ops * trait->amountReq);
            steamOut->setFill(steamOut->getFill() + ops * trait->amountProduced);

            long powerGen = ops * trait->heatEnergy * eff;
            powerProduced += powerGen;

            // i dont know what this is used for currently.
            //info[0] = ops * trait.amountReq;
			//info[1] = ops * trait.amountProduced;
			//info[2] = ops * trait.heatEnergy * eff;

            // are we done? i think we are
        }
    }

    printf("power gen: %.2Lf MHE/s\n", powerProduced/50000.0);

    // 4, low pressure steam is transferred to the condensers
    for (int i = 0; i < turbineCount; i++) {
        for (int i2 = 0; i2 < turbineCompressionCount; i2++) {
            FluidTank* steamOut = turbines[i + i2 + 1];

            int fluidToTransfer = (int)std::floor((float)steamOut->getFill() / (float)condenserCount);
            for (int i2 = 0; i2 < condenserCount; i2++) {
                int i3 = i2 * 2;
                condensers[i3]->setFill(condensers[i3]->getFill() + fluidToTransfer);
                steamOut->setFill(steamOut->getFill() - fluidToTransfer);
            }
        }
    }

    // 5, condenser condenses low pressure steam to water.
    for (int i = 0; i < condenserCount; i++) {
        FluidTank* steamIn = condensers[i * 2];
        FluidTank* waterOut = condensers[(i * 2)+1];

        int convert = std::min(steamIn->getFill(), waterOut->getMaxFill() - waterOut->getFill());
        steamIn->setFill(steamIn->getFill() - convert);
        waterOut->setFill(waterOut->getFill() + convert);
    }

    // 6, condenser water is transferred to water reservoirs.
    for (int i = 0; i < condenserCount; i++) {
        FluidTank* waterOut = condensers[(i * 2)+1];

        int fluidToTransfer = (int)std::floor((float)waterOut->getFill() / (float)reservoirCount);
        for (int i2 = 0; i2 < reservoirCount; i2++) {
            reservoirs[i2]->setFill(reservoirs[i2]->getFill() + fluidToTransfer);
            waterOut->setFill(waterOut->getFill() - fluidToTransfer);
        }
    }
}

// i hope this doesn't eat performance
FluidTrait_Coolable* CoolingLoop::getTraitForFluid(FluidType type) {
    FluidTrait_Coolable* trait = new FluidTrait_Coolable();
    if (type == FLUID_STEAM) {
        trait->coolsTo = FLUID_SPENTSTEAM;
        trait->amountReq = 100;
        trait->amountProduced = 1;
        trait->heatEnergy = 200;
    } else if (type == FLUID_HOTSTEAM) {
        trait->coolsTo = FLUID_STEAM;
        trait->amountReq = 1;
        trait->amountProduced = 10;
        trait->heatEnergy = 2;
    } else if (type == FLUID_SUPERHOTSTEAM) {
        trait->coolsTo = FLUID_HOTSTEAM;
        trait->amountReq = 1;
        trait->amountProduced = 10;
        trait->heatEnergy = 18;
    } else if (type == FLUID_ULTRAHOTSTEAM) {
        trait->coolsTo = FLUID_SUPERHOTSTEAM;
        trait->amountReq = 1;
        trait->amountProduced = 10;
        trait->heatEnergy = 120;
    }
    return trait;
}