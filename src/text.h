#pragma once
#include "raylib.h"

void TextLoad();
void TextUnload();

void TextDraw(const char *text, Vector2 pos, float fontSize, Color color);
Vector2 TextMeasure(const char *text, float fontSize);