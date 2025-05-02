#include "raylib.h"

#include "columnBase.h"
#include "columnOutgasser.h"

#include "../dials.h"
#include "../../main.h"
#include "../../utils.h"

ColumnOutgasser::ColumnOutgasser() {
    type = COLUMN_OUTGASSER;
}

void ColumnOutgasser::update() {
    
}
void ColumnOutgasser::draw(Vector2 columnSize, Vector2 destPos) {
    DrawTextureS(controlPanel->ui, {80, 172}, columnSize, destPos, columnSize, 4);
}

void ColumnOutgasser::receiveFlux(NeutronStream* stream) {

}
bool ColumnOutgasser::canProcess() {
    return false;
}