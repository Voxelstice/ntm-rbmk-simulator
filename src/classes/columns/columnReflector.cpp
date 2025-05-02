#include "raylib.h"

#include "columnBase.h"
#include "columnReflector.h"

#include "../dials.h"
#include "../../main.h"
#include "../../utils.h"

ColumnReflector::ColumnReflector() {
    type = COLUMN_REFLECTOR;
}

void ColumnReflector::update() {
    
}
void ColumnReflector::draw(Vector2 columnSize, Vector2 destPos) {
    DrawTextureS(controlPanel->ui, {70, 172}, columnSize, destPos, columnSize, 4);
}