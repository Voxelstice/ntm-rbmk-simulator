#pragma once
#include "raylib.h"

enum TextAlignment {
    ALIGN_LEFT = 0,
    ALIGN_CENTER = 1,
    ALIGN_RIGHT = 2,

    ALIGN_TOP = 3,
    ALIGN_MIDDLE = 4,
    ALIGN_BOTTOM = 5,
};

void TextLoad();
void TextUnload();

void TextDraw(const char *text, Vector2 pos, float fontSize, Color color);
void TextDrawAlign(const char *text, Vector2 pos, float fontSize, Color color, TextAlignment alignX, TextAlignment alignY);
Vector2 TextMeasure(const char *text, float fontSize);