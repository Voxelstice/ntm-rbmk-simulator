#include "raylib.h"

#include "submenu.h"
#include "submenuControlRod.h"
#include "../../utils.h"

SubmenuControlRod::SubmenuControlRod(Vector2 m_columnPos) : Submenu(m_columnPos) {
    ui = LoadTexture("assets/gui/gui_rbmk_control.png");
}

void SubmenuControlRod::open() {}
void SubmenuControlRod::update() {}
void SubmenuControlRod::draw() {
    // ALIGNMENT MALICE
    Vector2 guiPosition = {(float)GetScreenWidth() / 8 - (176 / 2), (float)GetScreenHeight() / 8 - (110 / 2)};
    DrawTextureS(ui, {0, 0}, {176, 110}, guiPosition, {176, 110}, 4);
}
void SubmenuControlRod::close() {
    UnloadTexture(ui);
}