#include "raylib.h"

#include "fluidTank.h"

#include "../../tooltip.h"

FluidTank::FluidTank(FluidType m_fluidType, int m_maxFluid) {
    fluidType = m_fluidType;
    fluid = 0;
    maxFluid = m_maxFluid;
}

// SETTERS
void FluidTank::setFill(int m_fluid) {
    fluid = m_fluid;

    // clamp stuff here
    if (fluid <= 0) fluid = 0;
    if (fluid >= maxFluid) fluid = maxFluid;
}
void FluidTank::setMaxFill(int m_maxFluid) {
    maxFluid = m_maxFluid;
}

void FluidTank::setTankType(FluidType m_fluidType) {
    fluidType = m_fluidType;
    setFill(0);
}

// GETTERS
int FluidTank::getFill() {
    return fluid;
}
int FluidTank::getMaxFill() {
    return maxFluid;
}
FluidType FluidTank::getTankType() {
    return fluidType;
}

// OTHER
void FluidTank::tankInfoTooltip(Rectangle rect) {
    SetTooltipOnHover(TextFormat("%s\n%i/%imB", getTankTypeString(fluidType).c_str(), fluid, maxFluid), rect);
}

// OUTSIDE OF FLUIDTANK
std::string getTankTypeString(FluidType type) {
    switch (type) {
        case FLUID_WATER:           return "Water";
        case FLUID_STEAM:           return "Steam";
        case FLUID_HOTSTEAM:        return "Dense Steam";
        case FLUID_SUPERHOTSTEAM:   return "Super Dense Steam";
        case FLUID_ULTRAHOTSTEAM:   return "Ultra Dense Steam";
        default:                    return "NONE";
    }
}