#include "raylib.h"
#include "raymath.h"

#include "submenu.h"
#include "submenuFuelRod.h"
#include "../../utils.h"
#include "../../textureCache.h"
#include "../../tooltip.h"

#include "../../main.h"
#include "../columns/columnFuelRod.h"

SubmenuFuelRod::SubmenuFuelRod(Vector2 m_columnPos) : Submenu(m_columnPos) {
    ui = TexCache_Get("assets/gui/gui_rbmk_element.png");
    ui_util = TexCache_Get("assets/gui/gui_utility.png");
    name = "RBMK Fuel Channel";
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

            if (column->fuel->itemHullHeat > 1000) {
                DrawTextureS(ui_util, {8, 16}, {16, 16}, Vector2Add(guiPosition, {-16, 20}), {16, 16}, 4); 
                SetTooltipOnHover("Fuel skin temperature has exceeded 1,000 C,\nautoloaders can no longer cycle fuel!", {(guiPosition.x - 16)*4, (guiPosition.y + 20)*4, 16*4, 16*4});
            }
            if (column->fuel->itemHullHeat > 200) {
                DrawTextureS(ui_util, {24, 16}, {16, 16}, Vector2Add(guiPosition, {-16, 36}), {16, 16}, 4);
                SetTooltipOnHover("Fuel skin temperature has exceeded 200 C,\nfuel can no longer be removed by hand!", {(guiPosition.x - 16)*4, (guiPosition.y + 36)*4, 16*4, 16*4});
            }
        }

        column->itemSlot->draw();
    }
}
void SubmenuFuelRod::close() {

}