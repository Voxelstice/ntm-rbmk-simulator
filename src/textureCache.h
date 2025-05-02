#pragma once
#include "raylib.h"

// internal functions
void TexCache_Init();

bool TexCache_Exists(const char* path);
void TexCache_Load(const char* path);
void TexCache_Unload(const char* path);

void TexCache_UnloadAll();

// public functions
Texture2D TexCache_Get(const char* path);