#include "raylib.h"

#include "columnBase.h"
#include "columnHeatExchanger.h"

#include "../dials.h"
#include "../../main.h"
#include "../../utils.h"

ColumnHeatExchanger::ColumnHeatExchanger() {
    type = COLUMN_HEATEX;
}

void ColumnHeatExchanger::update() {
    
}
void ColumnHeatExchanger::draw(Vector2 columnSize, Vector2 destPos) {
    DrawTextureS(controlPanel->ui, {130, 172}, columnSize, destPos, columnSize, 4);
}