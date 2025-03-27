#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <cmath>

#include "rbmk.h"
#include "../main.h"

#include "dials.h"

// THE COLUMN HORDE
#include "columns/columnBlank.h"
#include "columns/columnFuelRod.h"
#include "columns/columnControlRod.h"
#include "columns/columnControlRodAuto.h"
#include "columns/columnBoiler.h"
#include "columns/columnModerator.h"
#include "columns/columnAbsorber.h"
#include "columns/columnReflector.h"
#include "columns/columnOutgasser.h"
#include "columns/columnStorage.h"
#include "columns/columnCooler.h"
#include "columns/columnHeatExchanger.h"

RBMK::RBMK() {
    if (rbmkDials.varsEmbedded == false) columnGridPosition = {86, 11};
    else columnGridPosition = {0, 0};

    columns.clear();

    for (int i = 0; i < 15 * 15; i++) {
        ColumnBase *column = new ColumnBase();
        column->active = false;
        column->pos = posFromIndex(i);

        columns.push_back(column);
    }

    placeColumn({7, 7}, new ColumnBlank()); // center
}

ColumnBase* RBMK::placeColumn(Vector2 pos, ColumnBase* column) {
    column->active = true;
    column->pos = pos;
    columns[indexFromPos(pos)] = column;
    return column;
}
ColumnBase* RBMK::makeColumnFromType(ColumnType type) {
         if (type == COLUMN_BLANK)          return new ColumnBlank();
    else if (type == COLUMN_FUEL)           return new ColumnFuelRod();
    else if (type == COLUMN_CONTROL)        return new ColumnControlRod();
    else if (type == COLUMN_CONTROL_AUTO)   return new ColumnControlRodAuto();
    else if (type == COLUMN_BOILER)         return new ColumnBoiler();
    else if (type == COLUMN_MODERATOR)      return new ColumnModerator();
    else if (type == COLUMN_ABSORBER)       return new ColumnAbsorber();
    else if (type == COLUMN_REFLECTOR)      return new ColumnReflector();
    else if (type == COLUMN_OUTGASSER)      return new ColumnOutgasser();
    else if (type == COLUMN_STORAGE)        return new ColumnStorage();
    else if (type == COLUMN_COOLER)         return new ColumnCooler();
    else if (type == COLUMN_HEATEX)         return new ColumnHeatExchanger();

    else                                    return new ColumnBase();
}

void RBMK::update() {
    if (state == RUNNING) {
        for (int i = 0; i < 15*15; i++) {
            if (columns[i]->active == false) continue;

            columns[i]->baseUpdate();
            columns[i]->update();
        }
    } else if (state == MELTED) {

    } else {
        for (int i = 0; i < 15*15; i++) {
            if (columns[i]->active == false) continue;

            columns[i]->baseReset();
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
            Vector2 destPos = Vector2Add(columnGridPosition, {columnSize.x * float(x), columnSize.y * float(y)});

            // column itself
            column->draw(columnSize, destPos);

            // heat
            float heatAlpha = Clamp((float) ((column->heat-20.0) / column->maxHeat), 0.0f, 1.0f);
            float heatY = std::round(10.0f * heatAlpha);
            controlPanel->drawTex(controlPanel->ui, {0, 182+columnSize.x-heatY}, {10, heatY}, Vector2Add(destPos, {0, columnSize.y-heatY}), {10, heatY}, 4);
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
            if (columns[i]->active == false) continue;
            
            columns[i]->baseInit();
            columns[i]->init();
        }
    } else if (newState == MELTED) {

    } else if (newState == OFFLINE) {
        for (int i = 0; i < 15*15; i++) {
            if (columns[i]->active == false) continue;

            columns[i]->baseReset();
            columns[i]->reset();
        }
    }

    state = newState;
}
void RBMK::meltdown() {
    changeState(MELTED);
}

Vector2 RBMK::posFromIndex(int i) {
    int x = i % 15;
    int y = i / 15;
    return {float(x), float(y)};
}
int RBMK::indexFromPos(Vector2 pos) {
    return int(pos.y * 15 + pos.x);
}