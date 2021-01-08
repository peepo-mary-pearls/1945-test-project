#ifndef PARTICLES_H
#define PARTICLES_H
#include "components.h"
#include "game.h"
#include "gameObject.h"

struct particles
{
    queue* particles;
}; typedef struct particles particles;

particles* particles_new(scene* s, int size);

#endif