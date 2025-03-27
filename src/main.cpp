#include "raylib.h"

/*#define RAYGUI_IMPLEMENTATION
#include "libs/raygui.h"

// Dear ImGui
#include "imgui/imgui.h"
#include "libs/rlImGui.h"*/

#include "logger.h"

#include "classes/dials.h"
#include "classes/rbmk.h"
#include "classes/rbmkBuilder.h"
#include "classes/controlPanel.h"

RBMK* rbmk;
RBMKBuilder* rbmkBuilder;
ControlPanel* controlPanel;

int main() {
    // Initialization
    Logger_init();

    TraceLog(LOG_INFO, "Application started");
    TraceLog(LOG_INFO, "Running version 0.0.1a");
    TraceLog(LOG_INFO, "NTM RBMK SIMULATOR - A SIMULATOR MADE BY VOXELSTICE");
    TraceLog(LOG_INFO, "HBM'S NUCLEAR TECH MOD MADE BY HBMMODS - PLAY IT IMMEDIATELY");
    TraceLog(LOG_INFO, "----------------------------------------------------------------------------------");

    if (rbmkDials.varsEmbedded == true) {
        TraceLog(LOG_INFO, "Launching in embedded mode");
        InitWindow(150*4, 150*4, "NTM RBMK Simulator");
        SetTargetFPS(20); // optimal tick speed in minecraft
    } else {
        TraceLog(LOG_INFO, "Launching in dedicated mode");
        InitWindow((244+60)*4, (172)*4, "NTM RBMK Simulator");
        InitAudioDevice();
        SetTargetFPS(60);
    }

    //----------------------------------------------------------------------------------

    rbmk = new RBMK();
    rbmkBuilder = new RBMKBuilder();
    controlPanel = new ControlPanel(rbmk);

    //----------------------------------------------------------------------------------

    while (!WindowShouldClose()) {
        // Update
        rbmk->update();
        rbmkBuilder->update();
        controlPanel->update();

        if (IsKeyPressed(KEY_Z)) rbmk->changeState(RUNNING);
        if (IsKeyPressed(KEY_X)) rbmk->changeState(OFFLINE);

        //----------------------------------------------------------------------------------

        // Draw
        BeginDrawing();
            ClearBackground(GRAY);
            
            controlPanel->draw();
            rbmk->draw();
            rbmkBuilder->draw();
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    TraceLog(LOG_INFO, "----------------------------------------------------------------------------------");
    TraceLog(LOG_INFO, "Application closing");

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseAudioDevice();
    CloseWindow();
    //--------------------------------------------------------------------------------------

    TraceLog(LOG_INFO, "----------------------------------------------------------------------------------");
    Logger_close();

    return 0;
}