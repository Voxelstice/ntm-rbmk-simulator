#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <cmath>
#include <string>

#include "rbmkBuilder.h"
#include "dials.h"
#include "../mathHelper.h"

#include "../main.h"
#include "../tooltip.h"

RBMKBuilder::RBMKBuilder() {
    ui = LoadTexture("assets/gui/gui_rbmk_builder.png");
}

bool RBMKBuilder::isMouseWithinGrid() {
    Vector2 roundedPos = Vector2Floor(Vector2Subtract(Vector2Divide(GetMousePosition(), {4,4}), rbmk->columnGridPosition), 10);
    Vector2 clampedPos = Vector2Clamp(roundedPos, {-10, -10}, {150, 150});
    if (clampedPos.x >= 0 && clampedPos.x < 150 && clampedPos.y >= 0 && clampedPos.y < 150) return true;
    else return false;
}
Vector2 RBMKBuilder::getSelectedPosition() {
    Vector2 roundedPos = Vector2Floor(Vector2Subtract(Vector2Divide(GetMousePosition(), {4,4}), rbmk->columnGridPosition), 10);
    return Vector2Divide(Vector2Clamp(roundedPos, {0, 0}, {140, 140}), {10, 10});
}
ColumnType RBMKBuilder::getTypeFromIndex(int i) {
    // has to be hardcoded :(
    // atleast we make it look nice

    // ROW 1
         if (i == 0)  return COLUMN_BLANK;
    else if (i == 1)  return COLUMN_FUEL;
    else if (i == 2)  return COLUMN_CONTROL;
    else if (i == 3)  return COLUMN_CONTROL_AUTO;

    // ROW 2
    else if (i == 4)  return COLUMN_BOILER;
    else if (i == 5)  return COLUMN_MODERATOR;
    else if (i == 6)  return COLUMN_ABSORBER;
    else if (i == 7)  return COLUMN_REFLECTOR;

    // ROW 3
    else if (i == 8)  return COLUMN_OUTGASSER;
    else if (i == 9)  return COLUMN_STORAGE;
    else if (i == 10) return COLUMN_COOLER;
    else if (i == 11) return COLUMN_HEATEX;

    // ROW 4
    // ...is pretty much empty

    return COLUMN_NONE;
}
std::string RBMKBuilder::getStringFromType(ColumnType type) {
         if (type == COLUMN_BLANK)          return "BLANK";
    else if (type == COLUMN_FUEL)           return "FUEL";
    else if (type == COLUMN_FUEL_SIM)       return "FUEL_SIM";
    else if (type == COLUMN_CONTROL)        return "CONTROL";
    else if (type == COLUMN_CONTROL_AUTO)   return "CONTROL_AUTO";
    else if (type == COLUMN_BOILER)         return "BOILER";
    else if (type == COLUMN_MODERATOR)      return "MODERATOR";
    else if (type == COLUMN_ABSORBER)       return "ABSORBER";
    else if (type == COLUMN_REFLECTOR)      return "REFLECTOR";
    else if (type == COLUMN_OUTGASSER)      return "OUTGASSER";
    else if (type == COLUMN_STORAGE)        return "STORAGE";
    else if (type == COLUMN_COOLER)         return "COOLER";
    else if (type == COLUMN_HEATEX)         return "HEATEX";

    return "NONE";
}

void RBMKBuilder::update() {
    if (rbmkDials.varsEmbedded == true) {
        return;
    }

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int i = y * 4 + x;
            Rectangle rect = { (244+10+(10*(float)x))*4, (11+(10*(float)y))*4, 40, 40 };

            if (CheckCollisionPointRec(GetMousePosition(), rect)) {
                SetTooltip(getStringFromType(getTypeFromIndex(i)).c_str());
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) columnIndex = i;
            }
        }
    }

    Vector2 rbmkPos = getSelectedPosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isMouseWithinGrid() && rbmk->state == OFFLINE) {
        rbmk->placeColumn(rbmkPos, rbmk->makeColumnFromType(getTypeFromIndex(columnIndex)));
    }
}
void RBMKBuilder::draw() {
    if (rbmkDials.varsEmbedded == true) {
        return;
    }

    controlPanel->drawTex(ui, {0, 0}, {60, 172}, {244, 0}, {60, 172}, 4);

    // draw selection
    Vector2 pos = Vector2Multiply(getSelectedPosition(), {10, 10});
    if (isMouseWithinGrid() && rbmk->state == OFFLINE)
        controlPanel->drawTex(controlPanel->ui, {0, 192}, {10, 10}, Vector2Add(rbmk->columnGridPosition, pos), {10, 10}, 4);

    // that ui
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int i = y * 4 + x;
            if (columnIndex == i) {
                controlPanel->drawTex(controlPanel->ui, {0, 192}, {10, 10}, {244+10+(10*(float)x), 11+(10*(float)y)}, {10, 10}, 4);
            }
        }
    }
}