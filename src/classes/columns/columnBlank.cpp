#include "raylib.h"

#include "columnBase.h"
#include "columnBlank.h"

#include "../dials.h"
#include "../../main.h"

ColumnBlank::ColumnBlank() {
    type = BLANKK;
}

void ColumnBlank::draw(Vector2 columnSize, Vector2 destPos) {
    controlPanel->drawTex(controlPanel->ui, {0, 172}, columnSize, destPos, columnSize, 4);
}