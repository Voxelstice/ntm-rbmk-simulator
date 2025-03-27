#include "raylib.h"

// loading
Font mainFont;
float fontSpacing = 1.0f;

void TextLoad() {
    mainFont = LoadFont("assets/Monocraft.otf");
    SetTextLineSpacing(4);
}
void TextUnload() {
    if (IsFontValid(mainFont)) UnloadFont(mainFont);
}

void TextDraw(const char *text, Vector2 pos, float fontSize, Color color) {
    if (IsFontValid(mainFont)) {
        DrawTextEx(mainFont, text, pos, fontSize, fontSpacing, color);
    }
}
Vector2 TextMeasure(const char *text, float fontSize) {
    Vector2 textSize = { 0.0f, 0.0f };
    if (IsFontValid(mainFont)) {
        textSize = MeasureTextEx(mainFont, text, fontSize, fontSpacing);
    }
    return textSize;
}