#include "raylib.h"

#include <cmath>

#include "../../mathHelper.h"
using namespace MathHelper;

#include "columnBase.h"
#include "columnAbsorber.h"

#include "../dials.h"
#include "../../main.h"

ColumnAbsorber::ColumnAbsorber() {
    type = COLUMN_ABSORBER;
}

void ColumnAbsorber::update() {
    
}
void ColumnAbsorber::draw(Vector2 columnSize, Vector2 destPos) {
    controlPanel->drawTex(controlPanel->ui, {60, 172}, columnSize, destPos, columnSize, 4);
}