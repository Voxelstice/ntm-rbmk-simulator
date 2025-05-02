#include "raylib.h"

#include "controlPanel.h"
#include "rbmk.h"
#include "dials.h"
#include "../utils.h"

ControlPanel::ControlPanel(RBMK* m_rbmk) {
    ui = LoadTexture("assets/gui/gui_rbmk_console.png");
    rbmk = m_rbmk;
}

void ControlPanel::update() {
    
}
void ControlPanel::draw() {
    if (rbmkDials.varsEmbedded == true) {
        DrawTextureS(ui, {86, 11}, {150, 150}, {0, 0}, {150, 150}, 4);
        return;
    }

    DrawTexturePro(ui, {0, 0, 244, 172}, {0, 0, 244 * 4, 172 * 4}, {0, 0}, 0, WHITE);

    // fill out these red slots, have a better way to handle those later on. i dont think this is needed though
    DrawTextureS(ui, {0, 238}, {18, 18}, {6, 8}, {18, 18}, 4);
    DrawTextureS(ui, {0, 238}, {18, 18}, {6, 29}, {18, 18}, 4);
    DrawTextureS(ui, {0, 238}, {18, 18}, {6, 50}, {18, 18}, 4);

    DrawTextureS(ui, {0, 238}, {18, 18}, {46, 8}, {18, 18}, 4);
    DrawTextureS(ui, {0, 238}, {18, 18}, {46, 29}, {18, 18}, 4);
    DrawTextureS(ui, {0, 238}, {18, 18}, {46, 50}, {18, 18}, 4);

    // close the SCRAM button with the cover.
    DrawTextureS(ui, {229, 172}, {25, 51}, {31, 138}, {25, 51}, 4);
}