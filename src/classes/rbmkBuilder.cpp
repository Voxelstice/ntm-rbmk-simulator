#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <cmath>
#include <string>

#include "rbmkBuilder.h"
#include "dials.h"

#include "../main.h"
#include "../text.h"
#include "../tooltip.h"
#include "../mathHelper.h"

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
    switch (i) {
        // row 1
        case 0: return COLUMN_BLANK;
        case 1: return COLUMN_FUEL;
        case 2: return COLUMN_CONTROL;
        case 3: return COLUMN_CONTROL_AUTO;
        // row 2
        case 4: return COLUMN_BOILER;
        case 5: return COLUMN_MODERATOR;
        case 6: return COLUMN_ABSORBER;
        case 7: return COLUMN_REFLECTOR;
        // row 3
        case 8: return COLUMN_OUTGASSER;
        case 9: return COLUMN_STORAGE;
        case 10: return COLUMN_COOLER;
        case 11: return COLUMN_HEATEX;
        // row 4
        default: return COLUMN_NONE;
    }
}
std::string RBMKBuilder::getStringFromType(ColumnType type) {
    // i should have maybe used a switch from the beginning
    switch (type) {
        case COLUMN_BLANK:          return "BLANK";
        case COLUMN_FUEL:           return "FUEL";
        case COLUMN_FUEL_SIM:       return "FUEL_SIM";
        case COLUMN_CONTROL:        return "CONTROL";
        case COLUMN_CONTROL_AUTO:   return "CONTROL_AUTO";
        case COLUMN_BOILER:         return "BOILER";
        case COLUMN_MODERATOR:      return "MODERATOR";
        case COLUMN_ABSORBER:       return "ABSORBER";
        case COLUMN_REFLECTOR:      return "REFLECTOR";
        case COLUMN_OUTGASSER:      return "OUTGASSER";
        case COLUMN_STORAGE:        return "STORAGE";
        case COLUMN_COOLER:         return "COOLER";
        case COLUMN_HEATEX:         return "HEATEX";
        default:                    return "NONE";
    }
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

    if (rbmk->state == OFFLINE) {
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

        TextDrawAlign("Offline", Vector2Add({244*4, (float)GetScreenHeight() - 48}, {60*2, 0}), 16.0f, BLACK, ALIGN_CENTER, ALIGN_MIDDLE);
    } else if (rbmk->state == RUNNING) {
        controlPanel->drawTex(ui, {60, 0}, {60, 172}, {244, 0}, {60, 172}, 4);

        TextDrawAlign("Online", Vector2Add({244*4, (float)GetScreenHeight() - 48}, {60*2, 0}), 16.0f, BLACK, ALIGN_CENTER, ALIGN_MIDDLE);
    } else if (rbmk->state == MELTED) {
        controlPanel->drawTex(ui, {60, 0}, {60, 172}, {244, 0}, {60, 172}, 4);

        TextDrawAlign("Destroyed", Vector2Add({244*4, (float)GetScreenHeight() - 48}, {60*2, 0}), 16.0f, BLACK, ALIGN_CENTER, ALIGN_MIDDLE);
    } else {
        controlPanel->drawTex(ui, {60, 0}, {60, 172}, {244, 0}, {60, 172}, 4);

        TextDrawAlign("Unknown", Vector2Add({244*4, (float)GetScreenHeight() - 48}, {60*2, 0}), 16.0f, BLACK, ALIGN_CENTER, ALIGN_MIDDLE);
    }
}