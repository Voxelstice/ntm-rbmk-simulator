#pragma once

enum AudioSample {
    AUDIOSAMPLE_CLICK,
    AUDIOSAMPLE_AZ5_LID,
    AUDIOSAMPLE_SHUTDOWN,
    AUDIOSAMPLE_EXPLOSION,
};

void Audio_Init();
void Audio_Deinit();
void Audio_PlaySound(AudioSample sample);