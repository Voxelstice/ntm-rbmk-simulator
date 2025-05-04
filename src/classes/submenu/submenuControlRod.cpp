#include "raylib.h"
#include "raymath.h"

#include "submenu.h"
#include "submenuControlRod.h"

#include "../../main.h"
#include "../../utils.h"
#include "../../audio.h"
#include "../../tooltip.h"
#include "../../textureCache.h"

#include "../columns/columnControlRod.h"

SubmenuControlRod::SubmenuControlRod(Vector2 m_columnPos) : Submenu(m_columnPos) {
    ui = TexCache_Get("assets/gui/gui_rbmk_control.png");
    name = "RBMK Control Rods";
}

void SubmenuControlRod::open() {}
void SubmenuControlRod::update() {
    Vector2 guiPosition = {(float)GetScreenWidth() / 8 - (176 / 2), (float)GetScreenHeight() / 8 - (110 / 2)};

    ColumnControlRod* column = (ColumnControlRod*)rbmk->getColumn(rbmk->indexFromPos(columnPos));
    if (column->active == true) {
        Rectangle rodRect = {(guiPosition.x + 71)*4, (guiPosition.y + 29)*4, 16*4, 56*4};
        SetTooltipOnHover(TextFormat("%i%%", int(column->level * 100)), rodRect);

        for (int i = 0; i < 5; i++) {
            Rectangle colorBtnRect = {(guiPosition.x + 28) * 4, (guiPosition.y + 26 + i * 11) * 4, 12 * 4, 10 * 4};
            Rectangle levelBtnRect = {(guiPosition.x + 118) * 4, (guiPosition.y + 26 + i * 11) * 4, 30 * 4, 10 * 4};

            if (CheckCollisionPointRec(GetMousePosition(), colorBtnRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Audio_PlaySound(AUDIOSAMPLE_CLICK);
                if (column->col == i) column->col = COLUMNCOLOR_NONE;
                else column->col = (ColumnColor)i;
            }
            if (CheckCollisionPointRec(GetMousePosition(), levelBtnRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                column->setTarget(1.0f - (i * 0.25f));
                Audio_PlaySound(AUDIOSAMPLE_CLICK);
            }
        }
    }
}
void SubmenuControlRod::draw() {
    // ALIGNMENT MALICE
    Vector2 guiPosition = {(float)GetScreenWidth() / 8 - (176 / 2), (float)GetScreenHeight() / 8 - (110 / 2)};
    DrawTextureS(ui, {0, 0}, {176, 110}, guiPosition, {176, 110}, 4);
    
    ColumnControlRod* column = (ColumnControlRod*)rbmk->getColumn(rbmk->indexFromPos(columnPos));
    if (column->active == true) {
        int height = int(56.0f * (1.0f - column->level));
        if (height > 0) DrawTextureS(ui, {176, 56 - (float)height}, {8, (float)height}, Vector2Add(guiPosition, {75, 29}), {8, (float)height}, 4);

        if (column->col != COLUMNCOLOR_NONE) 
            DrawTextureS(ui, {184, float(column->col * 10)}, {12, 10}, Vector2Add(guiPosition, {28, 26 + float(column->col*11)}), {12, 10}, 4);
    }
}
void SubmenuControlRod::close() {
    
}