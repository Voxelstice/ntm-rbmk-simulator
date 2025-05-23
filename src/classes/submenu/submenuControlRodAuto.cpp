#include "raylib.h"

#include "submenu.h"
#include "submenuControlRodAuto.h"
#include "../../utils.h"
#include "../../textureCache.h"

SubmenuControlRodAuto::SubmenuControlRodAuto(Vector2 m_columnPos) : Submenu(m_columnPos) {
    ui = TexCache_Get("assets/gui/gui_rbmk_control_auto.png");
    name = "RBMK Automatic Control Rods";
}

void SubmenuControlRodAuto::open() {}
void SubmenuControlRodAuto::update() {}
void SubmenuControlRodAuto::draw() {
    // ALIGNMENT MALICE
    Vector2 guiPosition = {(float)GetScreenWidth() / 8 - (176 / 2), (float)GetScreenHeight() / 8 - (110 / 2)};
    DrawTextureS(ui, {0, 0}, {176, 110}, guiPosition, {176, 110}, 4);
}
void SubmenuControlRodAuto::close() {

}