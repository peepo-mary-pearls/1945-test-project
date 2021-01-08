#include "components.h"

audio_emitter* audio_emitter_new(gameObject* go, const char* path, int loops, audio_extension audio_ext, int volume, boolean play_on_start){

    component* comp = __component_new(go);

    audio_emitter* a = (audio_emitter*)malloc(sizeof(audio_emitter));
    a->loops = loops;
    a->path = path;
    //a->__is_playing = false;
    a->play_on_start = play_on_start;
    a->audio_ext = audio_ext;
    a->volume = volume;

    switch (audio_ext)
    {
    case WAV:
        a->audio = Mix_LoadWAV(path);
        break;
    case MP3:
        a->audio = Mix_LoadMUS(path);
        break;
    default:
        break;
    }

    comp->init = audio_init;
    comp->update = audio_update;
    comp->destroy = audio_destroy;
    comp->on_enable = audio_on_enable;
    comp->on_disable = audio_on_disable;
    comp->type = AUDIO_EMITTER_T;
    comp->data = a;
    
    return a;
}

void audio_destroy(component* comp){
    audio_emitter* a = (audio_emitter*)comp->data;
    switch (a->audio_ext)
    {
        case WAV:
            Mix_FreeChunk(a->audio);
            break;
        case MP3:
            Mix_FreeMusic(a->audio);
            break;
        default:
            break;
    }
}

void audio_init(component* comp){
    audio_emitter* a = (audio_emitter*)comp->data;
    if(a->play_on_start)
        audio_play(a);
}

void audio_update(component* comp){
}

void audio_play(audio_emitter* a){
    switch (a->audio_ext)
    {
        case WAV:
            Mix_PlayChannel(-1, a->audio, a->loops);
            Mix_VolumeChunk(a->audio, a->volume);
            break;
        case MP3:
            Mix_PlayMusic(a->audio, a->loops);
            Mix_VolumeChunk(a->audio, a->volume);
            break;
        default:
            break;
    }
}

void audio_on_enable(component* comp){

}

void audio_on_disable(component* comp){

}
