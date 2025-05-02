#include "raylib.h"

#include "columnBase.h"
#include "columnBlank.h"

#include "../dials.h"
#include "../../main.h"
#include "../../utils.h"

ColumnBlank::ColumnBlank() {
    type = COLUMN_BLANK;
}

void ColumnBlank::draw(Vector2 columnSize, Vector2 destPos) {
    DrawTextureS(controlPanel->ui, {0, 172}, columnSize, destPos, columnSize, 4);
}