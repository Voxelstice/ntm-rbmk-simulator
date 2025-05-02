#include "raylib.h"

#include "submenu.h"
#include "submenuOutgasser.h"
#include "../../utils.h"

SubmenuOutgasser::SubmenuOutgasser(Vector2 m_columnPos) : Submenu(m_columnPos) {
    ui = LoadTexture("assets/gui/gui_rbmk_outgasser.png");
}

void SubmenuOutgasser::open() {}
void SubmenuOutgasser::update() {}
void SubmenuOutgasser::draw() {
    // ALIGNMENT MALICE
    Vector2 guiPosition = {(float)GetScreenWidth() / 8 - (176 / 2), (float)GetScreenHeight() / 8 - (110 / 2)};
    DrawTextureS(ui, {0, 0}, {176, 110}, guiPosition, {176, 110}, 4);
}
void SubmenuOutgasser::close() {
    UnloadTexture(ui);
}