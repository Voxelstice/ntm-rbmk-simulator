#include "raylib.h"

#include "controlPanel.h"
#include "rbmk.h"
#include "dials.h"

#include "../audio.h"
#include "../utils.h"

ControlPanel::ControlPanel(RBMK* m_rbmk) {
    ui = LoadTexture("assets/gui/gui_rbmk_console.png");
    rbmk = m_rbmk;
}

void ControlPanel::update() {
    if (rbmk->state == RUNNING || rbmk->state == MELTED) {
        Rectangle rect = {31 * 4, 138 * 4, 25 * 4, 51 * 4};
        if (CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (az5Lid == true) {
                az5Lid = false;
                Audio_PlaySound(AUDIOSAMPLE_AZ5_LID);
            } else if (az5Lid == false && lastPress + 3 < GetTime()) {
                lastPress = GetTime();

                rbmk->az5();
                Audio_PlaySound(AUDIOSAMPLE_CLICK);
            }
        }
    } else {
        az5Lid = true;
    }
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
    if (az5Lid == true) DrawTextureS(ui, {229, 172}, {25, 51}, {31, 138}, {25, 51}, 4);
}