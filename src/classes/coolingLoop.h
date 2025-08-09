#pragma once

#include "raylib.h"

#include <vector>

#include "inventory/fluidTank.h"

// enums

// Steam
enum TurbineType { // steam to the lower compression levels while getting power out of them
    TURBINE_SMALL,       // block variant       (internal tank size: 64000 mB, 128000 mB, efficiency: 0.85)
    TURBINE_INDUSTRIAL,  // machine variant     (internal tank size: 512000 mB, 10240000 mB, efficiency: 1)
    TURBINE_CHUNGUS      // big machine variant (internal tank size: 1000000000 mB, efficiency: 0.85)
};
enum CondenserType { // low pressure steam back to water
    CONDENSER_SMALL,     // block variant   (internal tank size: 100 mB)
    CONDENSER_AUXILIARY, // tower variant   (internal tank size: 1000 mB)
    CONDENSER_LARGE,     // tower variant   (internal tank size: 10000 mB)
    CONDENSER_POWERED    // machine variant (internal tank size: 1000000 mB)
};

// Water
enum ReservoirFillWeight { // infinite water
    RESERVOIRFILL_SMALL, // 50 mB
    RESERVOIRFILL_HEAVY  // 500 mB
};
enum ReservoirType { // water storage
    RESERVOIR_SMALL,     // steel barrel (16k mB)
    RESERVOIR_MEDIUM,    // tank         (256k mB)
    RESERVOIR_BIG,       // big ass tank (2048k mB)
};

// fluid trait bullshit
typedef struct FluidTrait_Coolable {
    FluidType coolsTo;
    int amountReq;
    int amountProduced;
    int heatEnergy;
} FluidTrait_Coolable;

class CoolingLoop {
    public:
        CoolingLoop();

        void reconfigure();
        void reset();
        void update();

        FluidTrait_Coolable* getTraitForFluid(FluidType type);

        // config
        int turbineCompression = 0;
        int turbineCount = 1;
        TurbineType turbineType = TURBINE_INDUSTRIAL;

        int condenserCount = 1;
        CondenserType condenserType = CONDENSER_AUXILIARY;

        int reservoirCount = 4;
        ReservoirType reservoirType = RESERVOIR_MEDIUM;
        ReservoirFillWeight reservoirFillWeight = RESERVOIRFILL_HEAVY;

        // actual stuff.
        std::vector<FluidTank*> turbines;    // turbineCount * (turbineCompression+1) * 2
        std::vector<FluidTank*> condensers;  // condenserCount * 2
        std::vector<FluidTank*> reservoirs;  // reservoirCount
};