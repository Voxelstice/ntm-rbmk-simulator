#include "raylib.h"

#include <cmath>

#include "../../mathHelper.h"
using namespace MathHelper;

#include "columnBase.h"
#include "columnCooler.h"

#include "../dials.h"
#include "../../main.h"

ColumnCooler::ColumnCooler() {
    type = COLUMN_COOLER;
}

void ColumnCooler::update() {
    
}
void ColumnCooler::draw(Vector2 columnSize, Vector2 destPos) {
    controlPanel->drawTex(controlPanel->ui, {120, 172}, columnSize, destPos, columnSize, 4);
}