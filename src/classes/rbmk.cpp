#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <cmath>

#include "dials.h"
#include "rbmk.h"

#include "../main.h"
#include "../tooltip.h"
#include "../utils.h"

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

    streams.clear();
    columns.clear();

    for (int i = 0; i < 15 * 15; i++) {
        ColumnBase *column = new ColumnBase();
        column->active = false;
        column->pos = posFromIndex(i);

        columns.push_back(column);
    }

    placeColumn({7, 7}, new ColumnBlank()); // center

    radiationEmitted = 0;
}

// main
void RBMK::updateControl() {
    bool pressedUp = IsKeyPressed(KEY_UP);
    bool pressedDown = IsKeyPressed(KEY_DOWN);

    for (int i = 0; i < 15*15; i++) {
        if (columns[i]->active == false) continue;

        if (columns[i]->type == COLUMN_CONTROL) {
            ColumnControlRod *rod = (ColumnControlRod *)columns[i];
            if (pressedUp) rod->setTarget(rod->targetLevel+0.1);
            if (pressedDown) rod->setTarget(rod->targetLevel-0.1);
        }
    }
}
void RBMK::update() {
    if (state == RUNNING) {
        // update columns
        radiationEmitted = 0;
        streams.clear();
        for (int i = 0; i < 15*15; i++) {
            if (columns[i]->active == false) continue;

            columns[i]->baseUpdate();
            columns[i]->update();
        }

        // update streams
        for (NeutronStream* stream : streams) {
            stream->runStreamInteraction();
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
            DrawTextureS(controlPanel->ui, {0, 182+columnSize.x-heatY}, {10, heatY}, Vector2Add(destPos, {0, columnSize.y-heatY}), {10, heatY}, 4);

            if (state != OFFLINE && indexFromPos(rbmkBuilder->getSelectedPosition()) == i && rbmkBuilder->submenuActive == false) {
                std::string tooltipData = "";
                tooltipData += rbmkBuilder->getStringFromType(column->type);
                tooltipData += "\n";
                tooltipData += "Column Temperature: " + std::string(TextFormat("%.1f", column->heat)) + " C";

                // for the moron (that is me by the way)
                // this is where you get all the info
                // https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/tileentity/machine/rbmk/TileEntityRBMKConsole.java#L405
                std::vector<std::string> info = column->getInfo();
                for (std::string str : info) {
                    tooltipData += "\n";
                    tooltipData += str;
                }

                if (column->moderated == true) {
                    tooltipData += "\n";
                    tooltipData += "Moderated";
                }
                
                SetTooltip(tooltipData.c_str());
            }
        }
    }
}
void RBMK::reset() {
    radiationEmitted = 0;
    streams.clear();
    columns.clear();

    for (int i = 0; i < 15*15; i++) {
        ColumnBase* column = new ColumnBase();
        column->active = false;
        column->pos = posFromIndex(i);

        columns.push_back(column);
    }

    columns[112]->active = true;
}

// neutrons
void RBMK::addStream(NeutronStream* stream) {
    streams.push_back(stream);
}

// radiation
void RBMK::emitRadiation(double rad) {
    radiationEmitted += rad;
}

// states
void RBMK::changeState(RBMKState newState) {
    if (newState == RUNNING) {
        for (int i = 0; i < 15*15; i++) {
            if (columns[i]->active == false) continue;
            if (columns[i]->type == COLUMN_NONE) {
                columns[i]->active = false;
                continue;
            }

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

// columns
ColumnBase* RBMK::placeColumn(Vector2 pos, ColumnBase* column) {
    column->active = true;
    column->pos = pos;
    columns[indexFromPos(pos)] = column;
    return column;
}
ColumnBase* RBMK::makeColumnFromType(ColumnType type) {
    switch (type) {
        case COLUMN_BLANK:          return new ColumnBlank();
        case COLUMN_FUEL:           return new ColumnFuelRod();
        case COLUMN_CONTROL:        return new ColumnControlRod();
        case COLUMN_CONTROL_AUTO:   return new ColumnControlRodAuto();
        case COLUMN_BOILER:         return new ColumnBoiler();
        case COLUMN_MODERATOR:      return new ColumnModerator();
        case COLUMN_ABSORBER:       return new ColumnAbsorber();
        case COLUMN_REFLECTOR:      return new ColumnReflector();
        case COLUMN_OUTGASSER:      return new ColumnOutgasser();
        case COLUMN_STORAGE:        return new ColumnStorage();
        case COLUMN_COOLER:         return new ColumnCooler();
        case COLUMN_HEATEX:         return new ColumnHeatExchanger();

        default:                    return new ColumnBase();
    }
}
ColumnBase* RBMK::getColumn(int i) {
    return columns[i];
}

// utils
Vector2 RBMK::posFromIndex(int i) {
    int x = i % 15;
    int y = i / 15;
    return {float(x), float(y)};
}
int RBMK::indexFromPos(Vector2 pos) {
    return int(pos.y * 15 + pos.x);
}