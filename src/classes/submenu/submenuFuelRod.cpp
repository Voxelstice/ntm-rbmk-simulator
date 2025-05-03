#include "raylib.h"
#include "raymath.h"

#include "submenu.h"
#include "submenuFuelRod.h"
#include "../../utils.h"
#include "../../textureCache.h"

#include "../../main.h"
#include "../columns/columnFuelRod.h"

SubmenuFuelRod::SubmenuFuelRod(Vector2 m_columnPos) : Submenu(m_columnPos) {
    ui = TexCache_Get("assets/gui/gui_rbmk_element.png");
}

void SubmenuFuelRod::open() {}
void SubmenuFuelRod::update() {
    ColumnFuelRod* column = (ColumnFuelRod*)rbmk->getColumn(rbmk->indexFromPos(columnPos));
    if (column->active == true) {
        column->itemSlot->update();
    }
}
void SubmenuFuelRod::draw() {
    // ALIGNMENT MALICE
    Vector2 guiPosition = {(float)GetScreenWidth() / 8 - (176 / 2), (float)GetScreenHeight() / 8 - (110 / 2)};
    DrawTextureS(ui, {0, 0}, {176, 110}, guiPosition, {176, 110}, 4);
    
    // its safe to assume its a fuel rod because this submenu would only appear on a fuel rod.
    // all we have to check is if its active.
    ColumnFuelRod* column = (ColumnFuelRod*)rbmk->getColumn(rbmk->indexFromPos(columnPos));
    if (column->active == true) {
        column->itemSlot->changePosition(Vector2Scale(Vector2Add(guiPosition, {80, 45}), 4.0f));

        if (column->hasRod == true) {
            DrawTextureS(ui, {176, 0}, {18, 67}, Vector2Add(guiPosition, {34, 21}), {18, 67}, 4);

            // depletion
            int d = int(67 * (1.0f-column->fuel->getEnrichment()));
            DrawTextureS(ui, {194, 0}, {18, (float)d}, Vector2Add(guiPosition, {34, 21}), {18, (float)d}, 4);

            // xenon
            int x = int(58.0f * column->fuel->getPoisonLevel());
            DrawTextureS(ui, {212, 58-(float)x}, {14, (float)x}, Vector2Add(guiPosition, {126, 82-(float)x}), {14, (float)x}, 4);
        }

        column->itemSlot->draw();
    }
}
void SubmenuFuelRod::close() {

}