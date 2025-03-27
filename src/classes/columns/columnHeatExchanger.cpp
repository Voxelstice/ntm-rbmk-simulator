#include "raylib.h"

#include <cmath>

#include "../../mathHelper.h"
using namespace MathHelper;

#include "columnBase.h"
#include "columnHeatExchanger.h"

#include "../dials.h"
#include "../../main.h"

ColumnHeatExchanger::ColumnHeatExchanger() {
    type = COLUMN_HEATEX;
}

void ColumnHeatExchanger::update() {
    
}
void ColumnHeatExchanger::draw(Vector2 columnSize, Vector2 destPos) {
    controlPanel->drawTex(controlPanel->ui, {130, 172}, columnSize, destPos, columnSize, 4);
}