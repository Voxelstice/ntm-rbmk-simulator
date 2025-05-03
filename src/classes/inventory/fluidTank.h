#pragma once

#include "raylib.h"

#include <string>

typedef enum FluidType {
    FLUID_NONE,

    FLUID_WATER,
    FLUID_STEAM,
    FLUID_HOTSTEAM,
    FLUID_SUPERHOTSTEAM,
    FLUID_ULTRAHOTSTEAM,
} FluidType;

class FluidTank {
    public:
        FluidTank(FluidType m_fluidType, int m_maxFluid);

        // SETTERS
        void setFill(int m_fluid);
        void setMaxFill(int m_maxFluid);
        void setTankType(FluidType m_fluidType);

        // GETTERS
        int getFill();
        int getMaxFill();
        FluidType getTankType();

        // OTHER
        void tankInfoTooltip(Rectangle rect);

        // VARS
        FluidType fluidType;
        int fluid = 0;
        int maxFluid = 1000;
};

std::string getTankTypeString(FluidType type);