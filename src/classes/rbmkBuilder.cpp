#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <cmath>

#include "rbmkBuilder.h"
#include "../mathHelper.h"
using namespace MathHelper;

#include "../main.h"

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
ColumnType RBMKBuilder::getTypeFromIndex() {
    // has to be hardcoded :(

    // ROW 1
    if (columnIndex == 0) return COLUMN_BLANK;
    else if (columnIndex == 1) return COLUMN_FUEL;
    else if (columnIndex == 2) return COLUMN_CONTROL;
    else if (columnIndex == 3) return COLUMN_CONTROL_AUTO;

    // ROW 2
    else if (columnIndex == 4) return COLUMN_BOILER;
    else if (columnIndex == 5) return COLUMN_MODERATOR;
    else if (columnIndex == 6) return COLUMN_ABSORBER;
    else if (columnIndex == 7) return COLUMN_REFLECTOR;

    // ROW 3
    else if (columnIndex == 8) return COLUMN_OUTGASSER;
    else if (columnIndex == 9) return COLUMN_STORAGE;
    else if (columnIndex == 10) return COLUMN_COOLER;
    else if (columnIndex == 11) return COLUMN_HEATEX;

    // ROW 4
    // ...is pretty much empty

    return COLUMN_NONE;
}

void RBMKBuilder::update() {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int i = y * 4 + x;
            Rectangle rect = {
                (244+10+(10*(float)x))*4, (11+(10*(float)y))*4,
                40, 40
            };

            if (CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                columnIndex = i;
            }
        }
    }

    Vector2 rbmkPos = getSelectedPosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isMouseWithinGrid()) {
        rbmk->placeColumn(rbmkPos, rbmk->makeColumnFromType(getTypeFromIndex()));
    }
}
void RBMKBuilder::draw() {
    controlPanel->drawTex(ui, {0, 0}, {60, 172}, {244, 0}, {60, 172}, 4);

    // draw selection
    Vector2 pos = Vector2Multiply(getSelectedPosition(), {10, 10});
    if (isMouseWithinGrid())
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