#pragma once

typedef enum AudioSample {
    AUDIOSAMPLE_CLICK,
    AUDIOSAMPLE_AZ5_LID,
    AUDIOSAMPLE_SHUTDOWN,
    AUDIOSAMPLE_EXPLOSION,
} AudioSample;

void Audio_Init();
void Audio_Deinit();
void Audio_PlaySound(AudioSample sample);