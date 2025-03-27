#include "raylib.h"

#include <cmath>

#include "../../mathHelper.h"
using namespace MathHelper;

#include "columnBase.h"
#include "columnControlRod.h"
#include "columnControlRodAuto.h"

#include "../dials.h"
#include "../../main.h"

ColumnControlRodAuto::ColumnControlRodAuto() {
    type = COLUMN_CONTROL_AUTO;
}

void ColumnControlRodAuto::update() {
    
}
void ColumnControlRodAuto::draw(Vector2 columnSize, Vector2 destPos) {
    controlPanel->drawTex(controlPanel->ui, {30, 172}, columnSize, destPos, columnSize, 4);
}