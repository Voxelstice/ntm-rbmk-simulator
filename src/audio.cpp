// There wasn't going to be that much audio to begin with but i didnt want to reload them multiple times
// i just made it absolute horrible to not use much time

#include "raylib.h"
#include "audio.h"

Sound audioSample_click;
Sound audioSample_az5_lid;
Sound audioSample_shutdown;
Sound audioSample_explosion;

void Audio_Init() {
    if (IsAudioDeviceReady() == false) return;
    
    audioSample_click = LoadSound("assets/sfx/click.ogg");
    audioSample_az5_lid = LoadSound("assets/sfx/rbmk_az5_cover.ogg");
    audioSample_shutdown = LoadSound("assets/sfx/shutdown.ogg");
    audioSample_explosion = LoadSound("assets/sfx/rbmk_explosion.ogg");
}
void Audio_Deinit() {
    if (IsAudioDeviceReady() == false) return;

    UnloadSound(audioSample_click);
    UnloadSound(audioSample_az5_lid);
    UnloadSound(audioSample_shutdown);
    UnloadSound(audioSample_explosion);
}
void Audio_PlaySound(AudioSample sample) {
    if (IsAudioDeviceReady() == false) return;

    switch (sample) {
        case AUDIOSAMPLE_CLICK: return PlaySound(audioSample_click);
        case AUDIOSAMPLE_AZ5_LID: return PlaySound(audioSample_az5_lid);
        case AUDIOSAMPLE_SHUTDOWN: return PlaySound(audioSample_shutdown);
        case AUDIOSAMPLE_EXPLOSION: return PlaySound(audioSample_explosion);
    }
}