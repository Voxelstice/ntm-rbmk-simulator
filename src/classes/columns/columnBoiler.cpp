#include "raylib.h"

#include "columnBase.h"
#include "columnBoiler.h"

#include "../dials.h"
#include "../../main.h"
#include "../../utils.h"

ColumnBoiler::ColumnBoiler() {
    type = COLUMN_BOILER;
}

void ColumnBoiler::update() {
    
}
void ColumnBoiler::draw(Vector2 columnSize, Vector2 destPos) {
    DrawTextureS(controlPanel->ui, {40, 172}, columnSize, destPos, columnSize, 4);
}