// i tried to hold my urge off making a dedicated utils file
#include "raylib.h"

void DrawTextureS(Texture2D tex, Vector2 srcPos, Vector2 srcSize, Vector2 destPos, Vector2 destSize, float scale) {
    DrawTexturePro(tex, {srcPos.x, srcPos.y, srcSize.x, srcSize.y}, {destPos.x*scale, destPos.y*scale, destSize.x*scale, destSize.y*scale}, {0, 0}, 0, WHITE);
}