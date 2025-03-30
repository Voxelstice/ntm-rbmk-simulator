#pragma once

#include <string>

void DrawTooltip();
void SetTooltip(std::string tooltip);
void SetTooltipOnHover(std::string tooltip, Vector2 pos, Vector2 size);
void SetTooltipOnHover(std::string tooltip, Rectangle rect);