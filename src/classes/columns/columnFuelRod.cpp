#include "raylib.h"

#include <cmath>

#include "../../mathHelper.h"
using namespace MathHelper;

#include "columnBase.h"
#include "columnFuelRod.h"

#include "../dials.h"
#include "../../main.h"

ColumnFuelRod::ColumnFuelRod() {
    type = COLUMN_FUEL;
}

void ColumnFuelRod::update() {
    if (rbmk->state == RUNNING) {
        heat += 1000;
    }
}
void ColumnFuelRod::draw(Vector2 columnSize, Vector2 destPos) {
    controlPanel->drawTex(controlPanel->ui, {10, 172}, columnSize, destPos, columnSize, 4);
}