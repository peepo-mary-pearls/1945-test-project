#include "particles.h"

particles* particles_new(scene* s, int size){
    particles* p = (particles*)malloc(sizeof(particles));
    p->particles = createQueue();
    for (int i = 0; i < size; i++)
    {
        gameObject* go = gameObject_new(s, "particle");
        sprite_new_animated_loops(go, "resources/assets/enemy/explosion1_strip6.png", 1, 6, 0.07, 1);
        gameObject_set_active(go, false);
        enqueue(p->particles, go);
    }
    return p;
}