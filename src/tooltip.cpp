#include "raylib.h"
#include "raymath.h"

#include <string>

#include "mathHelper.h"
#include "tooltip.h"
#include "text.h"

std::string tooltipText = "";
bool activateTooltip = false;

Vector2 tooltipOffset = {16, 16};
float tooltipSize = 16;
float tooltipPadding = 6;

float screenPadding = 8;

void DrawTooltip() {
    if (activateTooltip == false) return;

    Vector2 textSize = TextMeasure(tooltipText.c_str(), tooltipSize);
    Vector2 textPos = Vector2Floor(Vector2Add(GetMousePosition(), tooltipOffset), 4);
    Vector2 boxPos = Vector2Add(textPos, {-tooltipPadding, -tooltipPadding});
    Vector2 boxSize = Vector2Add(textSize, {tooltipPadding * 2, tooltipPadding * 2});

    boxPos.x = Clamp(boxPos.x, screenPadding, (float)GetScreenWidth() - boxSize.x - screenPadding);
    boxPos.y = Clamp(boxPos.y, screenPadding, (float)GetScreenHeight() - boxSize.y - screenPadding);

    textPos = Vector2Add(boxPos, {tooltipPadding, tooltipPadding});

    Rectangle boxRect = {boxPos.x, boxPos.y, boxSize.x, boxSize.y};

    DrawRectangleV(boxPos, boxSize, Fade(BLACK, 0.8f));
    DrawRectangleLinesEx(boxRect, 2.0f, Fade(GRAY, 0.8f));
    TextDraw(tooltipText.c_str(), textPos, tooltipSize, WHITE);

    tooltipText = "";
    activateTooltip = false;
}
void SetTooltip(std::string tooltip) {
    tooltipText = tooltip;
    activateTooltip = true;
}
void SetTooltipOnHover(std::string tooltip, Vector2 pos, Vector2 size) {
    SetTooltipOnHover(tooltip, {pos.x, pos.y, size.x, size.y});
}
void SetTooltipOnHover(std::string tooltip, Rectangle rect) {
    if (CheckCollisionPointRec(GetMousePosition(), rect)) {
        tooltipText = tooltip;
        activateTooltip = true;
    }
}