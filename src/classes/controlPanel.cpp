#include "raylib.h"



#include "controlPanel.h"
#include "rbmk.h"
#include "dials.h"

#include "../main.h"
#include "../audio.h"
#include "../utils.h"
#include "../tooltip.h"

ControlPanel::ControlPanel() {
    ui = LoadTexture("assets/gui/gui_rbmk_console.png");
}

void ControlPanel::update() {
    // colors
    for (int i = 0; i < 5; i++) {
        Rectangle colorRect = {(6 + 11*float(i)) * 4, 70 * 4, 10 * 4, 10 * 4};
        const char *colorText = TextToLower(columnColorToStr((ColumnColor)i).c_str());
        SetTooltipOnHover(TextFormat("Left click: Select %s group\nRight click: Assign %s group", colorText, colorText), colorRect);
        if (CheckCollisionPointRec(GetMousePosition(), colorRect)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Audio_PlaySound(AUDIOSAMPLE_CLICK);
                rbmkBuilder->resetSelector();
                rbmk->selectGroup((ColumnColor)i);
            } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                Audio_PlaySound(AUDIOSAMPLE_CLICK);
                rbmk->assignGroup((ColumnColor)i);
            }
        }
    }

    // select stuff
    Rectangle controlRodsSelectRect = {61*4, 70*4, 10*4, 10*4};
    SetTooltipOnHover("Select all control rods", controlRodsSelectRect);
    if (CheckCollisionPointRec(GetMousePosition(), controlRodsSelectRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Audio_PlaySound(AUDIOSAMPLE_CLICK);
        rbmkBuilder->resetSelector();
        rbmk->selectControlRods();
    }

    Rectangle deselectRect = {72*4, 70*4, 10*4, 10*4};
    SetTooltipOnHover("Deselect all", deselectRect);
    if (CheckCollisionPointRec(GetMousePosition(), deselectRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Audio_PlaySound(AUDIOSAMPLE_CLICK);
        rbmkBuilder->resetSelector();
    }

    // other
    Rectangle cycleCompressorsRect = {70*4, 82*4, 12*4, 12*4};
    SetTooltipOnHover("Cycle steam channel compressor setting", cycleCompressorsRect);
    if (CheckCollisionPointRec(GetMousePosition(), cycleCompressorsRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Audio_PlaySound(AUDIOSAMPLE_CLICK);
        rbmk->cycleCompressors();
    }

    if (rbmk->state == RUNNING || rbmk->state == MELTED) {
        Rectangle az5Rect = {31 * 4, 138 * 4, 25 * 4, 51 * 4};
        if (CheckCollisionPointRec(GetMousePosition(), az5Rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
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