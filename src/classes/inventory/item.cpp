#include "raylib.h"
#include "raymath.h"

#include "../../utils.h"
#include "../../tooltip.h"
#include "item.h"

Item::Item(Vector2 m_position, std::string m_internalName, Texture2D m_tex) {
    internalName = m_internalName;
    tex = m_tex;
    position = m_position;
}

void Item::setTooltip(std::string m_tooltip) {
    tooltip = m_tooltip;
}

void Item::update() {
    Rectangle rect = {position.x, position.y, 16 * 4, 16 * 4};
    SetTooltipOnHover(tooltip, rect);
}

void Item::draw() {
    if (IsTextureValid(tex)) {
        // apparently you can have 32x32 items, while still having 16x16 slots
        // i THINK the fluid identifiers are 16x16? not sure, i have to check assets again
        DrawTextureS(tex, {0, 0}, {(float)tex.width, (float)tex.height}, position, {16 * 4, 16 * 4}, 1);
    } else {
        DrawRectangleV(position, {16 * 4, 16 * 4}, PURPLE);
    }
}