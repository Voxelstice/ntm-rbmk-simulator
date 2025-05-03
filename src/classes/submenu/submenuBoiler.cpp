#include "raylib.h"
#include "raymath.h"

#include <cmath>

#include "submenu.h"
#include "submenuBoiler.h"

#include "../../main.h"
#include "../../audio.h"
#include "../../utils.h"
#include "../../textureCache.h"

#include "../columns/columnBoiler.h"

SubmenuBoiler::SubmenuBoiler(Vector2 m_columnPos) : Submenu(m_columnPos) {
    ui = TexCache_Get("assets/gui/gui_rbmk_boiler.png");
}

void SubmenuBoiler::open() {}
void SubmenuBoiler::update() {
    Vector2 guiPosition = {(float)GetScreenWidth() / 8 - (176 / 2), (float)GetScreenHeight() / 8 - (110 / 2)};

    ColumnBoiler* column = (ColumnBoiler*)rbmk->getColumn(rbmk->indexFromPos(columnPos));
    if (column->active == true) {
        Rectangle compressorLever = {(guiPosition.x + 36) * 4, (guiPosition.y + 24) * 4, 14 * 4, 58 * 4};
        if (CheckCollisionPointRec(GetMousePosition(), compressorLever) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            column->cycleCompressor();
            Audio_PlaySound(AUDIOSAMPLE_CLICK);
        }
    }
}
void SubmenuBoiler::draw() {
    // ALIGNMENT MALICE
    Vector2 guiPosition = {(float)GetScreenWidth() / 8 - (176 / 2), (float)GetScreenHeight() / 8 - (110 / 2)};
    DrawTextureS(ui, {0, 0}, {176, 110}, guiPosition, {176, 110}, 4);

    ColumnBoiler* column = (ColumnBoiler*)rbmk->getColumn(rbmk->indexFromPos(columnPos));
    if (column->active == true) {
        int compLvl = column->getCompressionLevel();
        DrawTextureS(ui, {float(194 + (14*compLvl)), 0}, {14, 58}, Vector2Add(guiPosition, {36, 24}), {14, 58}, 4);

        float f = (float)std::floor(58 * (float(column->feed->getFill()) / float(column->feed->getMaxFill())));
        DrawTextureS(ui, {176, 58-f}, {14, f}, Vector2Add(guiPosition, {126, 82 - f}), {14, f}, 4);

        float s = (float)std::floor(22 * (float(column->steam->getFill()) / float(column->steam->getMaxFill())));
        if (s > 0) s++;
        if (s > 22) s++;
        DrawTextureS(ui, {190, 24-s}, {4, s}, Vector2Add(guiPosition, {91, 65 - s}), {4, s}, 4);

        // NOTE: the feedwater tank tooltip rect is actually kind of wonky and doesn't cover the entire tank, and overcovers it.
        // this is also in the minecraft mod, too. its just a little detail, but im noting it here.
        column->feed->tankInfoTooltip({(guiPosition.x + 126) * 4, (guiPosition.y + 24) * 4, 16 * 4, 56 * 4});
        column->steam->tankInfoTooltip({(guiPosition.x + 89) * 4, (guiPosition.y + 39) * 4, 8 * 4, 28 * 4});
    }
}
void SubmenuBoiler::close() {

}