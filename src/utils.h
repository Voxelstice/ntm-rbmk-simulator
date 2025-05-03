#pragma once
#include "raylib.h"

#include <vector>
#include <string>

void DrawTextureS(Texture2D tex, Vector2 srcPos, Vector2 srcSize, Vector2 destPos, Vector2 destSize, float scale);
std::string StringConcat(std::vector<std::string> strings, std::string separator);
std::string ReplaceString(std::string subject, const std::string &search, const std::string &replace);