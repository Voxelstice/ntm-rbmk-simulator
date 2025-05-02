#include "raylib.h"

#include "columnBase.h"
#include "columnCooler.h"

#include "../dials.h"
#include "../../main.h"
#include "../../utils.h"

ColumnCooler::ColumnCooler() {
    type = COLUMN_COOLER;
}

void ColumnCooler::update() {
    
}
void ColumnCooler::draw(Vector2 columnSize, Vector2 destPos) {
    DrawTextureS(controlPanel->ui, {120, 172}, columnSize, destPos, columnSize, 4);
}