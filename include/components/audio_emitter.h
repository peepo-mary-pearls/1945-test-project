#ifndef AUDIO_EMITTER_H
#define AUDIO_EMITTER_H
#include "SDL_mixer.h"

enum audio_extension{
    WAV,
    MP3,
}; typedef enum audio_extension audio_extension;

struct audio_emitter{
    void* audio;
    const char* path;
    int loops;
    int volume;
    audio_extension audio_ext;
    boolean play_on_start;
}; typedef struct audio_emitter audio_emitter;

audio_emitter* audio_emitter_new(gameObject* go, const char* path, int loops, audio_extension audio_ext, int volume, boolean play_on_start);
void audio_destroy(component* comp);
void audio_init(component* comp);
void audio_update(component* comp);
void audio_play(audio_emitter* a);
void audio_on_enable(component* comp);
void audio_on_disable(component* comp);

#endif