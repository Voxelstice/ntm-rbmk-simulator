// i tried to hold my urge off making a dedicated utils file
#include "raylib.h"

#include <vector>
#include <string>

void DrawTextureS(Texture2D tex, Vector2 srcPos, Vector2 srcSize, Vector2 destPos, Vector2 destSize, float scale) {
    DrawTexturePro(tex, {srcPos.x, srcPos.y, srcSize.x, srcSize.y}, {destPos.x*scale, destPos.y*scale, destSize.x*scale, destSize.y*scale}, {0, 0}, 0, WHITE);
}

std::string StringConcat(std::vector<std::string> strings, std::string separator) {
    std::string res = "";
    for (int i = 0; i < strings.size(); i++) {
        res += strings[i];
        if (i < strings.size()-1) res += separator;
    }
    return res;
}

// https://stackoverflow.com/a/14678800
std::string ReplaceString(std::string subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}