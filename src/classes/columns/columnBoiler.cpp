#include "raylib.h"

#include "columnBase.h"
#include "columnBoiler.h"

#include "../dials.h"
#include "../../main.h"

ColumnBoiler::ColumnBoiler() {
    type = COLUMN_BOILER;
}

void ColumnBoiler::update() {
    
}
void ColumnBoiler::draw(Vector2 columnSize, Vector2 destPos) {
    controlPanel->drawTex(controlPanel->ui, {40, 172}, columnSize, destPos, columnSize, 4);
}