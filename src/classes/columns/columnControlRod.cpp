#include "raylib.h"

#include "columnBase.h"
#include "columnControlRod.h"

#include "../dials.h"
#include "../../main.h"

ColumnControlRod::ColumnControlRod() {
    type = COLUMN_CONTROL;
}

void ColumnControlRod::update() {
    
}
void ColumnControlRod::draw(Vector2 columnSize, Vector2 destPos) {
    controlPanel->drawTex(controlPanel->ui, {20, 172}, columnSize, destPos, columnSize, 4);
}