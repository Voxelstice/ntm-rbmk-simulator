#include "raylib.h"

#include "columnBase.h"
#include "columnAbsorber.h"

#include "../dials.h"
#include "../../main.h"
#include "../../utils.h"

ColumnAbsorber::ColumnAbsorber() {
    type = COLUMN_ABSORBER;
}

void ColumnAbsorber::update() {
    
}
void ColumnAbsorber::draw(Vector2 columnSize, Vector2 destPos) {
    DrawTextureS(controlPanel->ui, {60, 172}, columnSize, destPos, columnSize, 4);
}