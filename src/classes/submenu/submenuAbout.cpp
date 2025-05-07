#include "raylib.h"

#include "../../text.h"

#include "submenu.h"
#include "submenuAbout.h"

SubmenuAbout::SubmenuAbout() : Submenu({0, 0}) {
    name = "";
}

void SubmenuAbout::open() {}
void SubmenuAbout::update() {}
void SubmenuAbout::draw() {
    TextDraw("NTM RBMK Simulator", {64, 64}, 32.0f, WHITE);
    TextDraw("A RBMK simulator, using the mechanics that are used in the minecraft mod HBM's Nuclear Tech Mod.", {64, 64+32}, 16.0f, WHITE);
    TextDraw("Majority of the simulation code is directly from the mod itself.", {64, 64+32+32}, 16.0f, WHITE);
    TextDraw("All sprites with the exception of some are also directly from the mod itself.", {64, 64+32+32+16}, 16.0f, WHITE);
    TextDraw("Everything else is made by myself.", {64, 64+32+32+32}, 16.0f, WHITE);

    // is it clickable? no.
    TextDraw("NTM RBMK Simulator by Voxelstice. https://github.com/Voxelstice/ntm-rbmk-simulator", {64, 192}, 16.0f, WHITE);
    TextDraw("Nuclear Tech Mod by HbmMods. https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT", {64, 192+16}, 16.0f, WHITE);
}
void SubmenuAbout::close() { }