#include "raylib.h"

#include "submenu.h"
#include "submenuFuelRod.h"
#include "../../utils.h"

SubmenuFuelRod::SubmenuFuelRod(Vector2 m_columnPos) : Submenu(m_columnPos) {
    // i probably should just have a texture cache tbh
    ui = LoadTexture("assets/gui/gui_rbmk_element.png");
}

void SubmenuFuelRod::open() {}
void SubmenuFuelRod::update() {}
void SubmenuFuelRod::draw() {
    // ALIGNMENT MALICE
    Vector2 guiPosition = {(float)GetScreenWidth() / 8 - (176 / 2), (float)GetScreenHeight() / 8 - (110 / 2)};
    DrawTextureS(ui, {0, 0}, {176, 110}, guiPosition, {176, 110}, 4);
}
void SubmenuFuelRod::close() {
    UnloadTexture(ui);
}