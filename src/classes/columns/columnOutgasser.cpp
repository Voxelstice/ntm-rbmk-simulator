#include "raylib.h"

#include "columnBase.h"
#include "columnOutgasser.h"

#include "../dials.h"
#include "../../main.h"

ColumnOutgasser::ColumnOutgasser() {
    type = COLUMN_OUTGASSER;
}

void ColumnOutgasser::update() {
    
}
void ColumnOutgasser::draw(Vector2 columnSize, Vector2 destPos) {
    controlPanel->drawTex(controlPanel->ui, {80, 172}, columnSize, destPos, columnSize, 4);
}