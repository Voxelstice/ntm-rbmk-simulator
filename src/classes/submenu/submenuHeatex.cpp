#include "raylib.h"

#include "submenu.h"
#include "submenuHeatex.h"
#include "../../utils.h"

SubmenuHeatex::SubmenuHeatex(Vector2 m_columnPos) : Submenu(m_columnPos) {
    ui = LoadTexture("assets/gui/gui_rbmk_heater.png");
}

void SubmenuHeatex::open() {}
void SubmenuHeatex::update() {}
void SubmenuHeatex::draw() {
    // ALIGNMENT MALICE
    Vector2 guiPosition = {(float)GetScreenWidth() / 8 - (176 / 2), (float)GetScreenHeight() / 8 - (110 / 2)};
    DrawTextureS(ui, {0, 0}, {176, 110}, guiPosition, {176, 110}, 4);
}
void SubmenuHeatex::close() {
    UnloadTexture(ui);
}