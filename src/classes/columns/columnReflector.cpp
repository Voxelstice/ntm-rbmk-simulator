#include "raylib.h"

#include <cmath>

#include "../../mathHelper.h"
using namespace MathHelper;

#include "columnBase.h"
#include "columnReflector.h"

#include "../dials.h"
#include "../../main.h"

ColumnReflector::ColumnReflector() {
    type = COLUMN_REFLECTOR;
}

void ColumnReflector::update() {
    
}
void ColumnReflector::draw(Vector2 columnSize, Vector2 destPos) {
    controlPanel->drawTex(controlPanel->ui, {70, 172}, columnSize, destPos, columnSize, 4);
}