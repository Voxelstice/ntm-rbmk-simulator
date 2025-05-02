#include "raylib.h"

#include "columnBase.h"
#include "columnModerator.h"

#include "../dials.h"
#include "../../main.h"
#include "../../utils.h"

ColumnModerator::ColumnModerator() {
    type = COLUMN_MODERATOR;
}

void ColumnModerator::draw(Vector2 columnSize, Vector2 destPos) {
    DrawTextureS(controlPanel->ui, {50, 172}, columnSize, destPos, columnSize, 4);
}