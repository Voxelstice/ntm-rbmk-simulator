#include "raylib.h"
#include "raymath.h"

enum TextAlignment {
    ALIGN_LEFT = 0,
    ALIGN_CENTER = 1,
    ALIGN_RIGHT = 2,

    ALIGN_TOP = 3,
    ALIGN_MIDDLE = 4,
    ALIGN_BOTTOM = 5,
};

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

Vector2 TextMeasure(const char *text, float fontSize) {
    Vector2 textSize = { 0.0f, 0.0f };
    if (IsFontValid(mainFont)) {
        textSize = MeasureTextEx(mainFont, text, fontSize, fontSpacing);
    }
    return textSize;
}
void TextDraw(const char *text, Vector2 pos, float fontSize, Color color) {
    if (IsFontValid(mainFont)) {
        DrawTextEx(mainFont, text, pos, fontSize, fontSpacing, color);
    }
}
void TextDrawAlign(const char *text, Vector2 pos, float fontSize, Color color, TextAlignment alignX, TextAlignment alignY) {
    if (IsFontValid(mainFont)) {
        Vector2 measure = TextMeasure(text, fontSize);
        Vector2 offset = {0, 0};

        if (alignX == ALIGN_CENTER) offset.x = -measure.x/2;
        else if (alignX == ALIGN_RIGHT) offset.x = -measure.x;

        if (alignY == ALIGN_MIDDLE) offset.y = -measure.y/2;
        else if (alignY == ALIGN_BOTTOM) offset.y = -measure.y;

        DrawTextEx(mainFont, text, Vector2Add(pos, offset), fontSize, fontSpacing, color);
    }
}