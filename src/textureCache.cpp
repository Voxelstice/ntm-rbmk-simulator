// i've had to actually implement this because of the submenus and the sheer amount of fuels.
// not everything uses it currently (e.g. the rbmk console, the rbmk builder)
// but stuff that will get repeated every now and then is all cached
// is it really a "cache" though? like this might be just considered storing textures in memory

#include "raylib.h"

#include <string>
#include <map>

// main cache mechanism
static std::map<std::string, Texture2D> cache;
Texture2D defaultTexture = { 0 };

// internal functions
void TexCache_Init() {
    Image defaultImage = GenImageColor(1, 1, WHITE);
    defaultTexture = LoadTextureFromImage(defaultImage);
    UnloadImage(defaultImage);

    // ensures it's clean
    cache.clear();
}

bool TexCache_Exists(const char* path) {
    if (cache.contains(path)) return true;
    else return false;
}
void TexCache_Load(const char* path) {
    if (TexCache_Exists(path) == false) {
        if (FileExists(path)) {
            TraceLog(LOG_INFO, TextFormat("TEXCCACHE: Loading %s", path));
            cache.emplace(path, LoadTexture(path));
        } else {
            TraceLog(LOG_INFO, TextFormat("TEXCCACHE: %s does not exist", path));
        }
    }
}
void TexCache_Unload(const char* path) {
    if (TexCache_Exists(path) == true) {
        TraceLog(LOG_INFO, TextFormat("TEXCCACHE: Unloading %s", path));
        UnloadTexture(cache[path]);
        cache.erase(path);
    }
}

void TexCache_UnloadAll() {
    for (const auto& pair : cache) {
        // still segfaults by the way
        TraceLog(LOG_INFO, TextFormat("TEXCCACHE: Unloading %s", pair.first.c_str()));
        UnloadTexture(cache[pair.first]);
    }
    cache.clear();
}

// public functions
Texture2D TexCache_Get(const char* path) {
    if (TexCache_Exists(path) == false)
        TexCache_Load(path);

    if (TexCache_Exists(path) == true) {
        if (IsTextureValid(cache[path])) return cache[path];
        else return defaultTexture;
    } else {
        return defaultTexture;
    }
}