#include "raylib.h"

/*#define RAYGUI_IMPLEMENTATION
#include "libs/raygui.h"

// Dear ImGui
#include "imgui/imgui.h"
#include "libs/rlImGui.h"*/

#include "logger.h"

#include "classes/rbmk.h"
#include "classes/controlPanel.h"

RBMK *rbmk;
ControlPanel *controlPanel;

int main() {
    // Initialization
    Logger_init();

    TraceLog(LOG_INFO, "Application started");
    TraceLog(LOG_INFO, "Running version 0.0.1a");
    TraceLog(LOG_INFO, "NTM RBMK SIMULATOR - A SIMULATOR MADE BY VOXELSTICE");
    TraceLog(LOG_INFO, "HBM'S NUCLEAR TECH MOD MADE BY HBMMODS - PLAY IT IMMEDIATELY");
    TraceLog(LOG_INFO, "----------------------------------------------------------------------------------");

    InitWindow((244+20)*4, (172)*4, "NTM RBMK Simulator");
    InitAudioDevice();
    SetTargetFPS(60);

    //----------------------------------------------------------------------------------

    rbmk = new RBMK();
    controlPanel = new ControlPanel(rbmk);

    //----------------------------------------------------------------------------------

    while (!WindowShouldClose()) {
        // Update
        rbmk->update();
        controlPanel->update();

        if (IsKeyPressed(KEY_Z)) rbmk->changeState(RUNNING);
        if (IsKeyPressed(KEY_X)) rbmk->changeState(OFFLINE);

        //----------------------------------------------------------------------------------

        // Draw
        BeginDrawing();
            ClearBackground(GRAY);
            
            controlPanel->draw();
            rbmk->draw();
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