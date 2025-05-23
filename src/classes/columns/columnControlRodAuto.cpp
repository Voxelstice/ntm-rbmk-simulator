#include "raylib.h"

#include "columnBase.h"
#include "columnControlRod.h"
#include "columnControlRodAuto.h"

#include "../dials.h"
#include "../../main.h"
#include "../../utils.h"

ColumnControlRodAuto::ColumnControlRodAuto() {
    type = COLUMN_CONTROL_AUTO;
}

void ColumnControlRodAuto::update() {
    
}
void ColumnControlRodAuto::draw(Vector2 columnSize, Vector2 destPos) {
    DrawTextureS(controlPanel->ui, {30, 172}, columnSize, destPos, columnSize, 4);
}