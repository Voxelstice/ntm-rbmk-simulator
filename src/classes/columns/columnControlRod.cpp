// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/tileentity/machine/rbmk/TileEntityRBMKControl.java

#include "raylib.h"
#include "raymath.h"

#include <cmath>
#include "../../mathHelper.h"

#include "columnBase.h"
#include "columnControlRod.h"

#include "../dials.h"
#include "../../main.h"

ColumnControlRod::ColumnControlRod() {
    type = COLUMN_CONTROL;

    startingLevel = 0;
    lastLevel = 0;
    level = 0;
    targetLevel = 0;
}

// main
void ColumnControlRod::init() {
    startingLevel = 0;
    lastLevel = 0;
    level = 0;
    targetLevel = 0;
}
void ColumnControlRod::update() {
    lastLevel = level;

    if (level < targetLevel) {
        level += speed * rbmkDials.dialControlSpeed;
        if (level > targetLevel) level = targetLevel;
    }
    if (level > targetLevel) {
        level -= speed * rbmkDials.dialControlSpeed;
        if (level < targetLevel) level = targetLevel;
    }
}
void ColumnControlRod::draw(Vector2 columnSize, Vector2 destPos) {
    controlPanel->drawTex(controlPanel->ui, {20, 172}, columnSize, destPos, columnSize, 4);

    float insertion = 8.0f-std::round((float)level * 8.0f);
    controlPanel->drawTex(controlPanel->ui, {24, 183}, {2, insertion}, Vector2Add(destPos, {4, 1}), {2, insertion}, 4);
}
void ColumnControlRod::reset() {
    startingLevel = 0;
    lastLevel = 0;
    level = 0;
    targetLevel = 0;
}

// controlling
void ColumnControlRod::setTarget(double target) {
    if (target < 0) target = 0;
    else if (target > 1) target = 1;

    startingLevel = target;
    targetLevel = target;
}
double ColumnControlRod::getMulti() {
    double surge = 0.0;
    if (targetLevel < startingLevel && std::abs(level - targetLevel) > 0.01) {
        surge = std::sin(std::pow((1.0 - level), 15.0) * math_pi) * (startingLevel - targetLevel) * rbmkDials.dialControlSurgeMod;
    }
    
    return level + surge;
}

// other
std::vector<std::string> ColumnControlRod::getInfo() {
    std::vector<std::string> vector;
    vector.push_back(std::string(TextFormat("%i", (int)(level*100))) + "%");
    return vector;
}