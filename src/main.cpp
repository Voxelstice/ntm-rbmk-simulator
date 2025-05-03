#include "raylib.h"

#include "text.h"
#include "audio.h"
#include "logger.h"
#include "tooltip.h"
#include "textureCache.h"

#include "classes/dials.h"
#include "classes/rbmk.h"
#include "classes/rbmkBuilder.h"
#include "classes/controlPanel.h"

#include "classes/fuel/fuelRegistry.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

RBMK* rbmk;
RBMKBuilder* rbmkBuilder;
ControlPanel* controlPanel;

double tickRate;
double nextTick;

void update() {
    // Update
    if (rbmkDials.varsSlowTicking == true) {
        if (GetTime() >= nextTick) {
            nextTick = GetTime() + tickRate;
            rbmk->update();
        }
    } else {
        rbmk->update();
    }

    rbmk->updateControl(); // we also still update it, just for the controls
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

        DrawTooltip();
    EndDrawing();
    //----------------------------------------------------------------------------------
}

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

    SetExitKey(NULL);

    //----------------------------------------------------------------------------------

    rbmk = new RBMK();
    rbmkBuilder = new RBMKBuilder();
    controlPanel = new ControlPanel(rbmk);

    RegisterFuels();
    TextLoad();
    TexCache_Init();
    Audio_Init();

    //----------------------------------------------------------------------------------

    // internal tick mechanism
    tickRate = 1.0 / 20.0;
    nextTick = GetTime() + tickRate;

    int fps = 60;
    if (rbmkDials.varsEmbedded == true) fps = 20;

    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(update, fps, 1);
    #else
        SetTargetFPS(fps);

        while (!WindowShouldClose()) {
            update();
        }
    #endif

    TraceLog(LOG_INFO, "----------------------------------------------------------------------------------");
    TraceLog(LOG_INFO, "Application closing");

    // De-Initialization
    //--------------------------------------------------------------------------------------
    TextUnload();
    TexCache_UnloadAll();
    Audio_Deinit();

    if (IsAudioDeviceReady()) CloseAudioDevice();
    CloseWindow();
    //--------------------------------------------------------------------------------------

    TraceLog(LOG_INFO, "----------------------------------------------------------------------------------");
    Logger_close();

    return 0;
}