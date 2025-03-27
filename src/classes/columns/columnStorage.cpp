#include "raylib.h"

#include <cmath>

#include "columnBase.h"
#include "columnStorage.h"

#include "../dials.h"
#include "../../main.h"

ColumnStorage::ColumnStorage() {
    type = COLUMN_STORAGE;
}

void ColumnStorage::draw(Vector2 columnSize, Vector2 destPos) {
    controlPanel->drawTex(controlPanel->ui, {110, 172}, columnSize, destPos, columnSize, 4);
}