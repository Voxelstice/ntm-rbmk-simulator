#include "raylib.h"

#include "submenu.h"
#include "submenuBoiler.h"
#include "../../utils.h"
#include "../../textureCache.h"

SubmenuBoiler::SubmenuBoiler(Vector2 m_columnPos) : Submenu(m_columnPos) {
    ui = TexCache_Get("assets/gui/gui_rbmk_boiler.png");
}

void SubmenuBoiler::open() {}
void SubmenuBoiler::update() {}
void SubmenuBoiler::draw() {
    // ALIGNMENT MALICE
    Vector2 guiPosition = {(float)GetScreenWidth() / 8 - (176 / 2), (float)GetScreenHeight() / 8 - (110 / 2)};
    DrawTextureS(ui, {0, 0}, {176, 110}, guiPosition, {176, 110}, 4);
}
void SubmenuBoiler::close() {

}