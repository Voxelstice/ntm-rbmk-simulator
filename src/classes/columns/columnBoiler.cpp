#include "raylib.h"
#include "raymath.h"

#include <cmath>

#include "columnBase.h"
#include "columnBoiler.h"

#include "../dials.h"
#include "../../main.h"
#include "../../utils.h"

// boiler-specific stuff
double getHeatFromSteam(FluidType type) {
    switch(type) {
        case FLUID_STEAM:           return 100.0f;
        case FLUID_HOTSTEAM:        return 300.0f;
        case FLUID_SUPERHOTSTEAM:   return 450.0f;
        case FLUID_ULTRAHOTSTEAM:   return 600.0f;
        default:                    return 0.0f;
    }
}
double getFactorFromSteam(FluidType type) {
    switch(type) {
        case FLUID_STEAM:           return 1.0f;
        case FLUID_HOTSTEAM:        return 10.0f;
        case FLUID_SUPERHOTSTEAM:   return 100.0f;
        case FLUID_ULTRAHOTSTEAM:   return 1000.0f;
        default:                    return 0.0f;
    }
}

ColumnBoiler::ColumnBoiler() {
    type = COLUMN_BOILER;

    feed = new FluidTank(FLUID_WATER, 10000);
    steam = new FluidTank(FLUID_STEAM, 1000000);
}

// main
void ColumnBoiler::init() {
    feed->setFill(0);
    steam->setFill(0);
    consumption = 0;
    output = 0;
}
void ColumnBoiler::update() {
    consumption = 0;
    output = 0;

    double heatCap = getHeatFromSteam(steam->getTankType());
    double heatProvided = heat - heatCap;

    if (heatProvided > 0) {
        double HEAT_PER_MB_WATER = rbmkDials.dialBoilerHeatConsumption;
        double steamFactor = getFactorFromSteam(steam->getTankType());
        int waterUsed = 0;
        int steamProduced = 0;

        if (steam->getTankType() == FLUID_ULTRAHOTSTEAM) {
            steamProduced = (int)std::floor((heatProvided / HEAT_PER_MB_WATER) * 100.0f / steamFactor);
            waterUsed = (int)std::floor(steamProduced / 100.0f * steamFactor);

            if (feed->getFill() < waterUsed) {
                steamProduced = (int)std::floor(feed->getFill() / 100.0f * steamFactor);
                waterUsed = (int)std::floor(steamProduced / 100.0f * steamFactor);
            }
        } else {
            waterUsed = (int)std::floor(heatProvided / HEAT_PER_MB_WATER);
            waterUsed = std::min(waterUsed, feed->getFill());
            steamProduced = (int)std::floor((waterUsed * 100.0f) / steamFactor);
        }

        consumption = waterUsed;
        output = steamProduced;

        feed->setFill(feed->getFill() - waterUsed);
        steam->setFill(steam->getFill() + steamProduced);

        if (steam->getFill() > steam->getMaxFill())
            steam->setFill(steam->getMaxFill());

        heat -= waterUsed * HEAT_PER_MB_WATER;
    }
}
void ColumnBoiler::draw(Vector2 columnSize, Vector2 destPos) {
    DrawTextureS(controlPanel->ui, {40, 172}, columnSize, destPos, columnSize, 4);
    DrawTextureS(controlPanel->ui, {44, float(183+(2*getCompressionLevel()))}, {2, 2}, Vector2Add(destPos, {4, float(1+(2*getCompressionLevel()))}), {2, 2}, 4);

    // feed
    float f = std::floor(8.0f * (float(feed->getFill()) / float(feed->getMaxFill())));
    DrawTextureS(controlPanel->ui, {41, 182+10-f-1}, {3, f}, Vector2Add(destPos, {1, 9-f}), {3, f}, 4);

    // steam
    float s = std::floor(8.0f * (float(steam->getFill()) / float(steam->getMaxFill())));
    DrawTextureS(controlPanel->ui, {46, 182+10-s-1}, {3, s}, Vector2Add(destPos, {6, 9-s}), {3, s}, 4);
}
void ColumnBoiler::reset() {
    feed->setFill(0);
    steam->setFill(0);
    consumption = 0;
    output = 0;
}

// other
std::vector<std::string> ColumnBoiler::getInfo() {
    std::vector<std::string> vector;

    vector.push_back(TextFormat("Feedwater: %i / %i", feed->getFill(), feed->getMaxFill()));
    vector.push_back(TextFormat("Steam: %i / %i", steam->getFill(), steam->getMaxFill()));
    vector.push_back("Compressor: " + getTankTypeString(steam->getTankType()));
    
    return vector;
}
void ColumnBoiler::cycleCompressor() {
    switch (steam->getTankType()) {
        case FLUID_STEAM:           return steam->setTankType(FLUID_HOTSTEAM);
        case FLUID_HOTSTEAM:        return steam->setTankType(FLUID_SUPERHOTSTEAM);
        case FLUID_SUPERHOTSTEAM:   return steam->setTankType(FLUID_ULTRAHOTSTEAM);
        case FLUID_ULTRAHOTSTEAM:   return steam->setTankType(FLUID_STEAM);

        default:                    return steam->setTankType(FLUID_HOTSTEAM);
    }
}

void ColumnBoiler::setCompressionLevel(int level) {
    switch (level) {
        case 0:      return steam->setTankType(FLUID_STEAM);
        case 1:      return steam->setTankType(FLUID_HOTSTEAM);
        case 2:      return steam->setTankType(FLUID_SUPERHOTSTEAM);
        case 3:      return steam->setTankType(FLUID_ULTRAHOTSTEAM);

        default:     return steam->setTankType(FLUID_STEAM);
    }
}
int ColumnBoiler::getCompressionLevel() {
    switch (steam->getTankType()) {
        case FLUID_STEAM:           return 0;
        case FLUID_HOTSTEAM:        return 1;
        case FLUID_SUPERHOTSTEAM:   return 2;
        case FLUID_ULTRAHOTSTEAM:   return 3;

        default:                    return 0;
    }
}