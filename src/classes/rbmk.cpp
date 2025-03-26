#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <cmath>

#include "rbmk.h"
#include "../main.h"

#include "columns/columnBlank.h"
#include "columns/columnFuelRod.h"

RBMK::RBMK() {
    columns.clear();

    for (int i = 0; i < 15*15; i++) {
        ColumnBase* column = new ColumnBase();
        column->active = false;
        column->pos = posFromIndex(i);

        columns.push_back(column);
    }

    placeColumn({7, 7}, new ColumnFuelRod());
    for (int i = 1; i < 5; i++) {
        float i2 = float(i);
        placeColumn({7-i2, 7}, new ColumnBlank());
        placeColumn({7+i2, 7}, new ColumnBlank());
        placeColumn({7, 7-i2}, new ColumnBlank());
        placeColumn({7, 7+i2}, new ColumnBlank());

        placeColumn({7-i2, 7-i2}, new ColumnBlank());
        placeColumn({7+i2, 7+i2}, new ColumnBlank());
        placeColumn({7+i2, 7-i2}, new ColumnBlank());
        placeColumn({7-i2, 7+i2}, new ColumnBlank());
    }
    

    //changeState(RUNNING);
}

ColumnBase* RBMK::placeColumn(Vector2 pos, ColumnBase* column) {
    column->active = true;
    column->pos = pos;
    columns[indexFromPos(pos)] = column;
    return column;
}

void RBMK::update() {
    if (state == RUNNING) {
        for (int i = 0; i < 15*15; i++) {
            columns[i]->update();
        }
    } else {
        for (int i = 0; i < 15*15; i++) {
            columns[i]->reset();
        }
    }
}
void RBMK::draw() {
    for (int i = 0; i < 15*15; i++) {
        ColumnBase *column = columns[i];

        int x = i % 15;
        int y = i / 15;

        if (column->active == true) {
            Vector2 columnSize = {10, 10};
            Vector2 destPos = {86 + columnSize.x * float(x), 11 + columnSize.y * float(y)};

            // column itself
            column->draw(columnSize, destPos);

            // heat
            float heatAlpha = Clamp((float) (column->heat / column->maxHeat), 0.0f, 1.0f);
            float heatY = std::round(10.0f * heatAlpha);
            controlPanel->drawTex(controlPanel->ui, {0, 182}, {10, heatY}, Vector2Add(destPos, {0, columnSize.y-heatY}), {10, heatY}, 4);
        }
    }
}

ColumnBase* RBMK::getColumn(int i) {
    return columns[i];
}

void RBMK::reset() {
    columns.clear();

    for (int i = 0; i < 15*15; i++) {
        ColumnBase* column = new ColumnBase();
        column->active = false;
        column->pos = posFromIndex(i);

        columns.push_back(column);
    }

    columns[112]->active = true;
}
void RBMK::changeState(RBMKState newState) {
    if (newState == RUNNING) {
        for (int i = 0; i < 15*15; i++) {
            columns[i]->init();
        }
    } else if (newState == OFFLINE) {
        for (int i = 0; i < 15*15; i++) {
            columns[i]->reset();
        }
    }

    state = newState;
}

Vector2 RBMK::posFromIndex(int i) {
    int x = i % 15;
    int y = i / 15;
    return {float(x), float(y)};
}
int RBMK::indexFromPos(Vector2 pos) {
    return int(pos.y * 15 + pos.x);
}